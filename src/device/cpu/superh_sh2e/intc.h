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


#ifndef SUPERH_SH2E_INTC_H_
#define SUPERH_SH2E_INTC_H_

#include <stdbool.h>
#include <stdint.h>

#ifndef PACKED
#define PACKED __attribute__((packed))
#endif

#define INTC_REGISTERS_START_ADDRESS UINT32_C(0xFFFFED00)
#define INTC_IPR_REGISTERS_COUNT 12
#define INTC_SYSTEM_REGISTERS_COUNT 2
#define INTC_PRIORITY_MAX_VALUE 15
#define INTC_SOURCE_MIN_VALUE 64
#define INTC_SOURCE_MAX_VALUE 255
#define INTC_IPR_HALF_BYTES_LENGTH (INTC_IPR_REGISTERS_COUNT * 4)
#define INTC_NMI_VECTOR_ADDRESS_OFFSET 11
#define INTC_INTERRUPT_POOL_SIZE 16

typedef struct sh2e_cpu sh2e_cpu_t;

/** Struct for Interrupt Control Register (ICR) */
typedef union sh2e_intc_icr {
    uint16_t value;

    PACKED struct {
#ifdef WORDS_BIGENDIAN
        uint16_t nmil : 1; /** Bit 15 - NMI Input Level (NMIL): Sets the level of the signal input at the NMI pin. This bit can be read to determine the NMI pin level. This bit cannot be modified.   */
        uint16_t _reserved : 4; /** Bits 14 to 9--Reserved: These bits always read 0. The write value should always be 0. */
        uint16_t nmie : 1; /** Bit 8 - NMIE */
        uint16_t irq0s : 1; /** Bits 7 to 0—IRQ0–IRQ7 Sense Select (IRQ0S–IRQ7S): These bits set the IRQ0–IRQ7 interrupt request detection mode.  */
        uint16_t irq1s : 1;
        uint16_t irq2s : 1;
        uint16_t irq3s : 1;
        uint16_t irq4s : 1;
        uint16_t irq5s : 1;
        uint16_t irq6s : 1;
        uint16_t irq7s : 1;
#else
        uint16_t irq7s : 1;
        uint16_t irq6s : 1;
        uint16_t irq5s : 1;
        uint16_t irq4s : 1;
        uint16_t irq3s : 1;
        uint16_t irq2s : 1;
        uint16_t irq1s : 1;
        uint16_t irq0s : 1; /** Bits 7 to 0—IRQ0–IRQ7 Sense Select (IRQ0S–IRQ7S): These bits set the IRQ0–IRQ7 interrupt request detection mode.  */
        uint16_t nmie : 1; /** Bit 8 - NMIE */
        uint16_t _reserved : 4; /** Bits 14 to 9--Reserved: These bits always read 0. The write value should always be 0. */
        uint16_t nmil : 1; /** Bit 15 - NMI Input Level (NMIL): Sets the level of the signal input at the NMI pin. This bit can be read to determine the NMI pin level. This bit cannot be modified.   */
#endif
    };
} sh2e_intc_icr_t;


typedef struct sh2e_intc_regs {

    uint16_t priority[INTC_IPR_REGISTERS_COUNT];

    union {
        uint16_t system[INTC_SYSTEM_REGISTERS_COUNT];

        PACKED struct {
            /** Interrupt Control Register (ICR) */
            sh2e_intc_icr_t icr;

            /** IRQ Status Register (ISR) */
            uint16_t isr;
        };
    };


} sh2e_intc_regs_t;

_Static_assert(sizeof(sh2e_intc_regs_t) == sizeof(uint16_t) * (INTC_IPR_REGISTERS_COUNT + INTC_SYSTEM_REGISTERS_COUNT), "invalid sh2e_intc_regs_t size!");

typedef struct sh2e_intc_source {
    // Interrupt source ID which should be the vector number in EPVA
    uint16_t source_id;
    // All priority registers are divided into 4 half bytes, so there are 48 possible indices (0-47)
    uint8_t priority_pool_index;
    bool pending;
} sh2e_intc_source_t;

typedef struct sh2e_intc_sources_pool {
    sh2e_intc_source_t * sources;

    uint16_t count;
    uint16_t size;
} sh2e_intc_sources_pool_t;

typedef struct sh2e_intc {

    sh2e_intc_regs_t * intc_regs;

    uint16_t nmi_prev_value; /** Previous value of NMI pin */

    sh2e_intc_sources_pool_t pool; /** Interrupt sources pool  */

    /** Internal use */
    uint16_t to_be_accepted_priority; /** Priority of the interrupt source that is to be accepted by the CPU */

} sh2e_intc_t;

extern void sh2e_intc_init_regs(sh2e_cpu_t * cpu, uint32_t regs_addr);

extern void sh2e_intc_init(sh2e_cpu_t * cpu);

extern uint16_t sh2e_check_pending_interrupts(sh2e_cpu_t * cpu);

extern void sh2e_accept_interrupt(sh2e_cpu_t * cpu, uint16_t interrupt_source);

extern void sh2e_intc_add_interrupt_source(sh2e_cpu_t * cpu, uint16_t source_id, uint8_t priority_pool_index, uint8_t priority);

#endif // SUPERH_SH2E_INTC_H_
