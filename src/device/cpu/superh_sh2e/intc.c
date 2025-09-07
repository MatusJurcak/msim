

#include "cpu.h"
#include "memops.h"

#include <string.h>

#define address(ptr) (uintptr_t) (ptr)

static sh2e_intc_icr_t
sh2e_intc_icr_reg_read(sh2e_cpu_t * cpu) {
    return (sh2e_intc_icr_t) sh2e_physmem_read16(cpu->id, address(&cpu->intc.intc_regs->icr), true);
}

// static uint16_t
// sh2e_intc_isr_reg_read(sh2e_cpu_t * cpu) {
//     return sh2e_physmem_read16(cpu->id, address(&cpu->intc.intc_regs->isr), true);
// }

static void
sh2e_intc_icr_reg_write(sh2e_cpu_t * cpu, uint16_t value) {
    physmem_write16(cpu->id, address(&cpu->intc.intc_regs->icr), htobe16(value), true);
}

static void
sh2e_intc_isr_reg_write(sh2e_cpu_t * cpu, uint16_t value) {
    physmem_write16(cpu->id, address(&cpu->intc.intc_regs->isr), htobe16(value), true);
}

static uint16_t
sh2e_intc_priority_register_read(sh2e_cpu_t * cpu, uint8_t index) {
    ASSERT(index < INTC_IPR_REGISTERS_COUNT && "wrong index for priority register");
    return sh2e_physmem_read16(cpu->id, address(&cpu->intc.intc_regs->priority[index]), true);
}

static void
sh2e_intc_priority_register_write(sh2e_cpu_t * cpu, uint8_t index, uint16_t value) {
    ASSERT(index < INTC_IPR_REGISTERS_COUNT && "wrong index for priority register");

    physmem_write16(cpu->id, address(&cpu->intc.intc_regs->priority[index]), htobe16(value), true);
}

static void
sh2e_intc_sources_pool_resize(sh2e_cpu_t * cpu) {
    ASSERT(cpu != NULL);
    sh2e_intc_sources_pool_t * pool = &cpu->intc.pool;

    sh2e_intc_source_t * sources = safe_malloc(sizeof(sh2e_intc_source_t) * (pool->size * 2));
    if (!sources) {
        ASSERT("Not enough memory for interrupt sources pool");
        return;
    }

    memcpy(sources, pool->sources, sizeof(sh2e_intc_source_t) * pool->count);

    safe_free(pool->sources);
    pool->sources = sources;
    pool->size *= 2;
}

static void
sh2e_intc_sources_pool_init(sh2e_cpu_t * cpu) {
    ASSERT(cpu != NULL);
    sh2e_intc_sources_pool_t * pool = &cpu->intc.pool;

    pool->sources = safe_malloc(sizeof(sh2e_intc_source_t) * INTC_INTERRUPT_POOL_SIZE);
    if (!pool->sources) {
        ASSERT("Not enough memory for interrupt sources pool");
        return;
    }

    pool->count = 0;
    pool->size = INTC_INTERRUPT_POOL_SIZE;
}

void
sh2e_intc_init_regs(sh2e_cpu_t * cpu, uint32_t regs_addr) {
    ASSERT(cpu != NULL);

    cpu->intc.intc_regs = (sh2e_intc_regs_t *) (address(regs_addr));

    /** Setup the priority registers */
    for (size_t i = 0; i < INTC_IPR_REGISTERS_COUNT; i++) {
        sh2e_intc_priority_register_write(cpu, i, 0);
    }

    /** Setup the system registers */
    // TODO: if the NMI pin is high the value of ICR should be H'8000
    sh2e_intc_icr_reg_write(cpu, 0);
    sh2e_intc_isr_reg_write(cpu, 0);
}

void
sh2e_intc_init(sh2e_cpu_t * cpu) {
    memset(&cpu->intc, 0, sizeof(sh2e_intc_t));

    sh2e_intc_init_regs(cpu, INTC_REGISTERS_START_ADDRESS);

    sh2e_intc_sources_pool_init(cpu);
}


void
sh2e_intc_add_interrupt_source(sh2e_cpu_t * cpu, uint16_t source_id, uint8_t priority_pool_index, uint8_t priority) {
    ASSERT(cpu != NULL);
    ASSERT(source_id >= INTC_SOURCE_MIN_VALUE);
    ASSERT(priority_pool_index < INTC_IPR_HALF_BYTES_LENGTH);
    ASSERT(priority <= INTC_PRIORITY_MAX_VALUE);

    for (uint16_t i = 0; i < cpu->intc.pool.count; i++) {
        if (cpu->intc.pool.sources[i].source_id == source_id) {
            ASSERT(false && "Interrupt source already exists");
            return;
        }
    }

    if (cpu->intc.pool.count == cpu->intc.pool.size) {
        sh2e_intc_sources_pool_resize(cpu);
    }

    sh2e_intc_source_t * source = &cpu->intc.pool.sources[cpu->intc.pool.count];

    source->source_id = source_id;
    source->priority_pool_index = priority_pool_index;
    source->pending = false;
    cpu->intc.pool.count++;

    uint16_t priority_register = sh2e_intc_priority_register_read(cpu, priority_pool_index / 4);

    // Need this in order to reverse the order and shift correctly
    int shift = (3 - (priority_pool_index % 4)) * 4;

    priority_register &= ~(0xF << shift);
    priority_register |= (priority & 0xF) << shift;

    sh2e_intc_priority_register_write(cpu, priority_pool_index / 4, priority_register);
}

static bool
sh2e_check_nmi_interrupt(sh2e_cpu_t * cpu) {
    ASSERT(cpu != NULL);

    sh2e_intc_icr_t icr = sh2e_intc_icr_reg_read(cpu);
    bool detected = false;
    if (icr.nmie) {
        // NMI interrupt is detected on the rising edge of the NMI pin
        detected = cpu->intc.nmi_prev_value == 0 && icr.nmil;
    } else {
        // NMI interrupt is detected on the falling edge of the NMI pin
        detected = cpu->intc.nmi_prev_value != 0 && !icr.nmil;
    }
    cpu->intc.nmi_prev_value = icr.nmil;
    return detected;
}

static uint16_t
sh2e_get_priority(sh2e_intc_source_t * source, uint16_t * priority_regs) {
    ASSERT(source != NULL);

    uint16_t reg_index = source->priority_pool_index / 4;
    uint16_t half_byte_index = (source->priority_pool_index % 4);

    // Need this in order to reverse the order and shift correctly
    int shift = (3 - half_byte_index) * 4;

    return (priority_regs[reg_index] >> shift) & 0x0F;
}

uint16_t
sh2e_check_pending_interrupts(sh2e_cpu_t * cpu) {
    ASSERT(cpu != NULL);

    // Check NMI
    if (sh2e_check_nmi_interrupt(cpu)) {
        cpu->intc.to_be_accepted_priority = INTC_PRIORITY_MAX_VALUE;
        return INTC_NMI_VECTOR_ADDRESS_OFFSET;
    }

    // Load all priority registers
    uint16_t priority_regs[INTC_IPR_REGISTERS_COUNT];
    for (int i = 0; i < INTC_IPR_REGISTERS_COUNT; i++) {
        priority_regs[i] = sh2e_intc_priority_register_read(cpu, i);
    }

    uint16_t interrupt_source = 0;
    uint16_t interrupt_priority = 0;

    uint16_t mask = cpu->cpu_regs.sr.im;

    // Check if any other interrupt is pending.
    for (unsigned int i = 0; i <= cpu->intc.pool.count; i++) {
        if (cpu->intc.pool.sources[i].pending) {

            uint16_t curr_priority = sh2e_get_priority(&cpu->intc.pool.sources[i], priority_regs);
            if ((curr_priority > interrupt_priority || !interrupt_source) && curr_priority > mask) {
                interrupt_priority = curr_priority;
                interrupt_source = cpu->intc.pool.sources[i].source_id;
            }
        }
    }

    cpu->intc.to_be_accepted_priority = interrupt_priority;
    return interrupt_source;
}


void
sh2e_accept_interrupt(sh2e_cpu_t * cpu, uint16_t interrupt_source) {
    ASSERT(cpu != NULL);
    ASSERT(interrupt_source > INTC_SOURCE_MIN_VALUE && "Interrupt source out of range");

    cpu->cpu_regs.sr.im = cpu->intc.to_be_accepted_priority;
    cpu->intc.to_be_accepted_priority = 0;

    // Clear the interrupt request
    // if (sh2e_intc_is_interrupt_source_irq(interrupt_source)) {
    //     sh2e_clear_irq_interrupt(cpu, interrupt_source - irq0);
    // }

    for (unsigned int i = 0; i < cpu->intc.pool.count; i++) {
        if (cpu->intc.pool.sources[i].source_id == interrupt_source) {
            cpu->intc.pool.sources[i].pending = false;
            break;
        }
    }
}
