/*
 * Copyright (c) X-Y Z
 * All rights reserved.
 *
 * Distributed under the terms of GPL.
 *
 *
 * Hitachi/Renesas SuperH SH-2E microprocessor device (32-bit, FPU).
 *
 */

#include "bitops.h"
#include "cpu.h"
#include "debug.h"
#include "decode.h"
#include "memops.h"

#include "../../../assert.h"

#include <ctype.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/** @brief Cached instruction item */
typedef struct {
    sh2e_insn_exec_fn_t insn;
    unsigned int cycles;
    bool disable_interrupts;
    bool disable_address_errors;
} cached_insn_t;


/** @brief Page of cached decoded instructions. */

typedef struct {
    item_t item;
    ptr36_t addr;
    cached_insn_t insns[FRAME_SIZE / sizeof(sh2e_insn_t)];
} cache_item_t;


#define PHYS2CACHEINSTR(phys) (((phys) & FRAME_MASK) / sizeof(sh2e_insn_t))

static void
cache_item_init(cache_item_t * cache_item) {
    ASSERT(cache_item != NULL);

    item_init(&cache_item->item);
    cache_item->addr = 0;
    // Skip insns init.
}


static list_t sh2e_insn_cache = LIST_INITIALIZER;


/**
 * @brief Retrieves instruction cache page for the given address.
 *
 * @return `true` if a page exists, `false` otherwise.
 */
static bool
sh2e_cpu_insn_cache_get(ptr36_t phys, cache_item_t ** const output_item) {
    ASSERT(output_item != NULL);

    ptr36_t const target_page = ALIGN_DOWN(phys, FRAME_SIZE);
    // printf("looking up insn cache page 0x%08" PRIx64 "\n", target_page);

    cache_item_t * item;
    for_each(sh2e_insn_cache, item, cache_item_t) {
        if (item->addr == target_page) {
            *output_item = item;
            return true;
        }
    }

    // printf("  insn cache page not found\n");
    return false;
}


/**
 * @brief Converts a virtual address to physical address.
 *
 * @return `true` if the conversion succeeded, `false` otherwise.
 */
bool
sh2e_cpu_convert_addr(
    sh2e_cpu_t const * const restrict cpu,
    ptr64_t const virt, ptr36_t * const phys, bool const write
) {
    ASSERT(cpu != NULL);

    // Use identity, because SH-2E does not support memory mapping.
    *phys = (ptr36_t) virt.ptr;
    return true;
}


/**
 * @brief Reads an instruction from memory and stores it
 * into `output_value`.
 *
 * @param cpu The CPU which makes the read.
 * @param addr The (physical) memory address to read from.
 * @param output_insn Pointer to where to store the instruction.
 * @return sh2e_exception_t Exception code related to the access.
 */
sh2e_exception_t
sh2e_cpu_fetch_insn(
    sh2e_cpu_t const * const restrict cpu, uint32_t const addr,
    sh2e_insn_t * const restrict output_insn
) {
    ASSERT(cpu != NULL);
    ASSERT(output_insn != NULL);

    // Check alignment (must be even address).
    if (addr & 1) {
        return SH2E_EXCEPTION_CPU_ADDRESS_ERROR;
    }

    output_insn->word = sh2e_physmem_read16(cpu->id, addr, true);
    return SH2E_EXCEPTION_NONE;
}


/**
 * @brief Sets the PC to the given address.
 */
static void
sh2e_set_pc(sh2e_cpu_t * const restrict cpu, uint32_t const addr) {
    cpu->cpu_regs.pc = addr;
    cpu->pc_next = addr + sizeof(sh2e_insn_t);
}


#define address(ptr) (uintptr_t) (ptr)

/** Sets initial values of registers. */
static void
sh2e_power_on_reset(sh2e_cpu_t * const restrict cpu) {
    ASSERT(cpu != NULL);

    // Registers with defined values.

    cpu->cpu_regs.vbr = 0x00000000;

    cpu->cpu_regs.sr.value = 0;
    cpu->cpu_regs.sr.im = 0b1111;

    cpu->fpu_regs.fpscr.value = 0;
    cpu->fpu_regs.fpscr.dn = 1;
    cpu->fpu_regs.fpscr.rm = 0b01;

    // Registers with derived values.
    // Ignore exceptions when loading register values from EPVA.

    sh2e_eva_t * epva = (sh2e_eva_t *) address(cpu->cpu_regs.vbr);

    uint32_t eva_sp_addr = (uint32_t) address(&epva->power_on_reset_sp);
    cpu->cpu_regs.sp = sh2e_physmem_read32(cpu->id, eva_sp_addr, false);

    uint32_t eva_pc_addr = (uint32_t) address(&epva->power_on_reset_pc);
    cpu->cpu_regs.pc = sh2e_physmem_read32(cpu->id, eva_pc_addr, false);

    sh2e_intc_init(cpu);

    cpu->pr_state = SH2E_PSTATE_POWER_ON_RESET;
}


/****************************************************************************
 * Instruction decode cache
 ****************************************************************************/

static void
sh2e_cpu_insn_cache_decode_page(sh2e_cpu_t * const restrict cpu, cache_item_t * cache_item) {
    for (size_t i = 0; i < FRAME_SIZE / sizeof(sh2e_insn_t); i++) {
        ptr36_t addr = cache_item->addr + (i * sizeof(sh2e_insn_t));
        sh2e_insn_t insn = (sh2e_insn_t) sh2e_physmem_read16(cpu->id, addr, false);
        sh2e_insn_desc_t const * desc = sh2e_insn_decode(insn);
        cache_item->insns[i].insn = desc->exec;
        cache_item->insns[i].disable_interrupts = desc->disable_interrupts;
        cache_item->insns[i].disable_address_errors = desc->disable_address_errors;
        cache_item->insns[i].cycles = desc->cycles;
    }
}

static void
sh2e_cpu_insn_cache_update(sh2e_cpu_t * const restrict cpu, cache_item_t * cache_item) {
    frame_t * frame = physmem_find_frame(cache_item->addr);
    ASSERT(frame != NULL);

    if (!frame->valid) {
        sh2e_cpu_insn_cache_decode_page(cpu, cache_item);
        frame->valid = true;
    }

    return;
}

static cache_item_t *
sh2e_cpu_insn_cache_try_add(sh2e_cpu_t * const restrict cpu, ptr36_t phys) {
    frame_t * frame = physmem_find_frame(phys);
    if (frame == NULL) {
        return NULL;
    }

    cache_item_t * cache_item = safe_malloc(sizeof(cache_item_t));
    cache_item_init(cache_item);
    cache_item->addr = ALIGN_DOWN(phys, FRAME_SIZE);
    list_push(&sh2e_insn_cache, &cache_item->item);

    sh2e_cpu_insn_cache_decode_page(cpu, cache_item);
    frame->valid = true;
    return cache_item;
}


static sh2e_insn_exec_fn_t
sh2e_cpu_fetch_insn_func(sh2e_cpu_t * const restrict cpu, ptr36_t phys, unsigned int * insn_cycles) {
    cache_item_t * cache_item = NULL;
    if (sh2e_cpu_insn_cache_get(phys, &cache_item)) {
        ASSERT(cache_item != NULL);

        sh2e_cpu_insn_cache_update(cpu, cache_item);

        // Move the instruction cache page to the head of the list
        // (if not already there) to speed up subsequent accesses.
        if (sh2e_insn_cache.head != &cache_item->item) {
            list_remove(&sh2e_insn_cache, &cache_item->item);
            list_push(&sh2e_insn_cache, &cache_item->item);
        }

        cached_insn_t * item = &cache_item->insns[PHYS2CACHEINSTR(phys)];

        cpu->disable_interrupts = item->disable_interrupts;
        cpu->disable_address_errors = item->disable_address_errors;
        *insn_cycles = item->cycles;

        // Return the function representing the instruction.
        return item->insn;
    }

    //
    // There is no instruction cache page for the given address.
    // Add it and return the instruction.
    //
    cache_item = sh2e_cpu_insn_cache_try_add(cpu, phys);
    if (cache_item != NULL) {

        cached_insn_t * item = &cache_item->insns[PHYS2CACHEINSTR(phys)];

        cpu->disable_interrupts = item->disable_interrupts;
        cpu->disable_address_errors = item->disable_address_errors;
        *insn_cycles = item->cycles;

        return item->insn;
    }

    alert("Trying to fetch instructions from outside of physical memory");
    return NULL;
}


/**
 * @brief Execute the instruction that the PC is pointing at
 * and handle interrupts or exceptions.
 */
static sh2e_exception_t
sh2e_cpu_execute_insn(sh2e_cpu_t * const restrict cpu, unsigned int * insn_cycles) {

    ptr36_t insn_phys = cpu->cpu_regs.pc;

    sh2e_insn_t insn;
    sh2e_exception_t fetch_ex = sh2e_cpu_fetch_insn(cpu, insn_phys, &insn);
    if (fetch_ex != SH2E_EXCEPTION_NONE) {
        return fetch_ex;
    }

    if (machine_trace) {
        sh2e_cpu_dump_insn(cpu, cpu->cpu_regs.pc, insn);
    }

    sh2e_insn_exec_fn_t exec_insn = sh2e_cpu_fetch_insn_func(cpu, insn_phys, insn_cycles);

    return exec_insn(cpu, insn);
}

static void
sh2e_cpu_handle_exception(sh2e_cpu_t * const restrict cpu, sh2e_exception_t ex) {
    cpu->pr_state = SH2E_PSTATE_EXCEPTION_PROCESSING;
    uint32_t vector_addr = 0;
    sh2e_eva_t * epva = (sh2e_eva_t *) address(cpu->cpu_regs.vbr);

    switch (ex) {
    case SH2E_EXCEPTION_ILLEGAL_INSTRUCTION: {
        vector_addr = (uint32_t) address(&epva->general_illegal_insn);
        break;
    }
    case SH2E_EXCEPTION_ILLEGAL_SLOT_INSTRUCTION: {
        vector_addr = (uint32_t) address(&epva->slot_illegal_insn);
        break;
    }
    case SH2E_EXCEPTION_CPU_ADDRESS_ERROR: {
        vector_addr = (uint32_t) address(&epva->cpu_address_error);
        break;
    }
    case SH2E_EXCEPTION_FPU_OPERATION: {
        vector_addr = (uint32_t) address(&epva->fpu_exception);
        break;
    }
    default: {
        ASSERT(false && "Unhandled exception");
    }
    }

    uint32_t const stack_sr = cpu->cpu_regs.sr.value;
    uint32_t const stack_sr_addr = cpu->cpu_regs.sp - sizeof(uint32_t);

    uint32_t stack_pc = 0;
    switch (ex) {
    case SH2E_EXCEPTION_ILLEGAL_INSTRUCTION: {
        stack_pc = cpu->cpu_regs.pc;
    }
    case SH2E_EXCEPTION_ILLEGAL_SLOT_INSTRUCTION:
    case SH2E_EXCEPTION_FPU_OPERATION:
    case SH2E_EXCEPTION_CPU_ADDRESS_ERROR: {
        stack_pc = cpu->pc_next;
    }
    default: {
        break;
        alert("Unsupported exception");
    }
    }

    uint32_t const stack_pc_addr = stack_sr_addr - sizeof(uint32_t);

    sh2e_exception_t cpu_write_ex;

    cpu_write_ex = sh2e_cpu_write_long(cpu, stack_sr_addr, stack_sr);
    // TODO:  handle address exception during stacking, now just assert
    if (cpu_write_ex != SH2E_EXCEPTION_NONE) {
        ASSERT(false && "Exception during the stacking of SR");
    }

    cpu_write_ex = sh2e_cpu_write_long(cpu, stack_pc_addr, stack_pc);
    // TODO:  handle address exception during stacking, now just assert
    if (cpu_write_ex != SH2E_EXCEPTION_NONE) {
        ASSERT(false && "Exception during the stacking of PC");
    }

    cpu->cpu_regs.sp = stack_pc_addr;
    // TODO: ugly, need to subtract only to add the same value later on
    cpu->cpu_regs.pc = vector_addr - sizeof(sh2e_insn_t);
    cpu->pc_next = cpu->cpu_regs.pc + sizeof(sh2e_insn_t);

    // Go back to program execution state
    cpu->pr_state = SH2E_PSTATE_PROGRAM_EXECUTION;
}

static void
sh2e_cpu_handle_interrupt(sh2e_cpu_t * const restrict cpu, uint16_t interrupt_source) {
    cpu->pr_state = SH2E_PSTATE_EXCEPTION_PROCESSING;
    // Push SR to stack
    uint32_t const stack_sr = cpu->cpu_regs.sr.value;
    uint32_t const stack_sr_addr = cpu->cpu_regs.sp - sizeof(uint32_t);

    // Push PC to stack (Address of instruction after executed instruction)
    uint32_t const stack_pc = cpu->pc_next;
    uint32_t const stack_pc_addr = stack_sr_addr - sizeof(uint32_t);

    sh2e_exception_t cpu_write_ex;

    cpu_write_ex = sh2e_cpu_write_long(cpu, stack_sr_addr, stack_sr);
    // TODO:  handle address exception during stacking, now just assert
    if (cpu_write_ex != SH2E_EXCEPTION_NONE) {
        ASSERT(false && "Exception during the stacking of SR");
    }

    cpu_write_ex = sh2e_cpu_write_long(cpu, stack_pc_addr, stack_pc);
    // TODO:  handle address exception during stacking, now just assert
    if (cpu_write_ex != SH2E_EXCEPTION_NONE) {
        ASSERT(false && "Exception during the stacking of PC");
    }

    sh2e_eva_t * epva = (sh2e_eva_t *) address(cpu->cpu_regs.vbr);

    cpu->cpu_regs.sp = stack_pc_addr;

    uint32_t eva_pc_addr = (uint32_t) address(&epva->vectors[interrupt_source]);
    // TODO: ugly, need to subtract only to add the same value later on
    cpu->cpu_regs.pc = (sh2e_physmem_read32(cpu->id, eva_pc_addr, false) - sizeof(sh2e_insn_t));
    cpu->pc_next = cpu->cpu_regs.pc + sizeof(sh2e_insn_t);

    sh2e_accept_interrupt(cpu, interrupt_source);

    // Go back to program execution state
    cpu->pr_state = SH2E_PSTATE_PROGRAM_EXECUTION;
    return;
}

static void
sh2e_try_handle_exceptions(sh2e_cpu_t * const restrict cpu, sh2e_exception_t ex) {
    uint16_t interrupt_source = 0;

    // If interrupts are not disabled, check for pending interrupts.
    if (!cpu->disable_interrupts) {
        interrupt_source = sh2e_check_pending_interrupts(cpu);
    }

    // Check if address errors are disabled.
    if (cpu->disable_address_errors && ex == SH2E_EXCEPTION_CPU_ADDRESS_ERROR) {
        cpu->pending_address_error = true;
        ex = SH2E_EXCEPTION_NONE;
    }

    // Exception Processing Priority Order:
    // - CPU address error
    if ((cpu->pending_address_error || ex == SH2E_EXCEPTION_CPU_ADDRESS_ERROR) && !cpu->disable_address_errors) {
        cpu->pending_address_error = false;
        sh2e_cpu_handle_exception(cpu, SH2E_EXCEPTION_CPU_ADDRESS_ERROR);
        return;
    }

    // - FPU exception
    if (ex == SH2E_EXCEPTION_FPU_OPERATION) {
        sh2e_cpu_handle_exception(cpu, ex);
        return;
    }

    // - Interrupts
    if (interrupt_source) {
        sh2e_cpu_handle_interrupt(cpu, interrupt_source);
        return;
    }

    // - General illegal instructions
    // - Illegal slot instructions
    if (ex != SH2E_EXCEPTION_NONE) {
        sh2e_cpu_handle_exception(cpu, ex);
    }
}

static void
sh2e_cpu_account(sh2e_cpu_t * const restrict cpu, unsigned int insn_cycles) {
    ASSERT(cpu != NULL);

    if (cpu->pr_state == SH2E_PSTATE_POWER_DOWN) {
        cpu->power_down_cycles++;
    }

    if (cpu->pr_state == SH2E_PSTATE_PROGRAM_EXECUTION) {
        cpu->program_execution_cycles += insn_cycles;
    }
}

/****************************************************************************
 * SH-2E CPU
 ****************************************************************************/

/** @brief Initialize the processor after power-on. */
void
sh2e_cpu_init(sh2e_cpu_t * const restrict cpu, unsigned int id) {
    ASSERT(cpu != NULL);

    memset(cpu, 0, sizeof(sh2e_cpu_t));
    cpu->id = id;

    // Requires 'id' to be set.
    sh2e_power_on_reset(cpu);

    // Execution state.
    cpu->pc_next = cpu->cpu_regs.pc + sizeof(sh2e_insn_t);
    cpu->pr_state = SH2E_PSTATE_PROGRAM_EXECUTION;
}


/** @brief Cleanup CPU structures. */
void
sh2e_cpu_done(sh2e_cpu_t * const restrict cpu) {
    ASSERT(cpu != NULL);

    // Clean the entire instruction cache.
    while (!is_empty(&sh2e_insn_cache)) {
        cache_item_t * cache_item = (cache_item_t *) (sh2e_insn_cache.head);
        list_remove(&sh2e_insn_cache, &cache_item->item);
        safe_free(cache_item);
    }
}


/** @brief Execute one instruction. */
void
sh2e_cpu_step(sh2e_cpu_t * const restrict cpu) {
    ASSERT(cpu != NULL);

    sh2e_exception_t ex = SH2E_EXCEPTION_NONE;
    unsigned int insn_cycles = 0;

    if (cpu->pr_state != SH2E_PSTATE_POWER_DOWN) {
        ex = sh2e_cpu_execute_insn(cpu, &insn_cycles);
    }

    sh2e_try_handle_exceptions(cpu, ex);

    // Accounting
    sh2e_cpu_account(cpu, insn_cycles);

    // Update program counter (respect delay slots).
    if (cpu->pr_state != SH2E_PSTATE_POWER_DOWN) {

        switch (cpu->br_state) {
        case SH2E_BRANCH_STATE_DELAY: // Delay branch instruction.
            cpu->br_state = SH2E_BRANCH_STATE_EXECUTE;
            // Fall-through.
        case SH2E_BRANCH_STATE_NONE: // Advance PC to next instruction.
            cpu->cpu_regs.pc = cpu->pc_next;
            cpu->pc_next += sizeof(sh2e_insn_t);
            break;

        case SH2E_BRANCH_STATE_EXECUTE: // Execute delayed branch.
            cpu->cpu_regs.pc = cpu->pc_next;
            cpu->pc_next += sizeof(sh2e_insn_t);
            cpu->br_state = SH2E_BRANCH_STATE_NONE;
            break;

        default:
            ASSERT(false && "invalid CPU branch state");
        }
    }
}


/** @brief Set the address of the next instruction to execute. */
void
sh2e_cpu_goto(sh2e_cpu_t * const restrict cpu, ptr64_t const addr) {
    ASSERT(cpu != NULL);

    if (IS_ALIGNED(addr.lo, sizeof(sh2e_insn_t))) {
        sh2e_set_pc(cpu, addr.lo);
    } else {
        ASSERT(false && "attempt to set PC to unaligned address");
    }
}


/** Assert the specified interrupt. */
void
sh2e_cpu_assert_interrupt(sh2e_cpu_t * const restrict cpu, unsigned int num) {
    ASSERT(cpu != NULL);
    ASSERT(num >= INTC_SOURCE_MIN_VALUE && "Interrupt number out of range");
    for (unsigned int i = 0; i < cpu->intc.pool.count; i++) {
        if (cpu->intc.pool.sources[i].source_id == num) {
            cpu->intc.pool.sources[i].pending = true;
            return;
        }
    }

    ASSERT(false && "Interrupt source not found!");
}


/** Deassert the specified interrupt */
void
sh2e_cpu_deassert_interrupt(sh2e_cpu_t * const restrict cpu, unsigned int num) {
    ASSERT(cpu != NULL);
    ASSERT(num >= INTC_SOURCE_MIN_VALUE && "Interrupt number out of range");
    for (unsigned int i = 0; i < cpu->intc.pool.count; i++) {
        if (cpu->intc.pool.sources[i].source_id == num) {
            cpu->intc.pool.sources[i].pending = false;
            return;
        }
    }

    ASSERT(false && "Interrupt source not found!");
}
