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

#define SH2E_INTC_REGISTERS_START_ADDRESS UINT32_C(0xFFFFED00)
#define SH2E_INTC_IPR_REGISTERS_COUNT 12
#define SH2E_INTC_SYSTEM_REGISTERS_COUNT 2
#define SH2E_INTC_PRIORITY_MAX_VALUE 15
#define SH2E_INTC_SOURCE_MAX_VALUE 255
#define SH2E_INTC_IPR_HALF_BYTES_LENGTH (SH2E_INTC_IPR_REGISTERS_COUNT * 4)
#define SH2E_INTC_INTERRUPT_POOL_SIZE 16

#define SH2E_INTC_POWER_ON_RESET_EXTERNAL_OFFSET 0
#define SH2E_INTC_POWER_ON_RESET_INTERNAL_OFFSET 1
#define SH2E_INTC_MANUAL_RESET_OFFSET 2
#define SH2E_INTC_NMI_VECTOR_ADDRESS_OFFSET 11
#define SH2E_INTC_UBC_VECTOR_ADDRESS_OFFSET 12
#define SH2E_INTC_HUDI_VECTOR_ADDRESS_OFFSET 14
#define SH2E_INTC_IRQ_VECTOR_ADDRESS_OFFSET 64
#define SH2E_INTC_IRQ_NUMBER_OF_SOURCES 8

#define SH2E_INTC_VALID_RESET_ID(id) (id <= SH2E_INTC_MANUAL_RESET_OFFSET)
#define SH2E_INTC_VALID_IRQ_SOURCE_ID(id) (id >= SH2E_INTC_IRQ_VECTOR_ADDRESS_OFFSET && id < (SH2E_INTC_IRQ_VECTOR_ADDRESS_OFFSET + SH2E_INTC_IRQ_NUMBER_OF_SOURCES))
#define SH2E_INTC_VALID_OTHER_SOURCE_ID(id) (id > (SH2E_INTC_IRQ_VECTOR_ADDRESS_OFFSET + SH2E_INTC_IRQ_NUMBER_OF_SOURCES) && id <= SH2E_INTC_SOURCE_MAX_VALUE)

#define SH2E_INTC_VALID_SOURCE_ID(id) \
    (SH2E_INTC_VALID_RESET_ID(id) || (id == SH2E_INTC_HUDI_VECTOR_ADDRESS_OFFSET) || (id == SH2E_INTC_UBC_VECTOR_ADDRESS_OFFSET) || SH2E_INTC_VALID_IRQ_SOURCE_ID(id) || SH2E_INTC_VALID_OTHER_SOURCE_ID(id))


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

typedef union sh2e_intc_isr {
    uint16_t value;

    PACKED struct {
#ifdef WORDS_BIGENDIAN
        uint16_t _reserved : 4; /** Bits 15 to 8—Reserved: These bits always read 0. The write value should always be 0.  */
        uint16_t irq0f : 1; /** Bits 7 to 0—IRQ0–IRQ7 Flags (IRQ0F–IRQ7F): These bits display the IRQ0–IRQ7 interrupt request status.   */
        uint16_t irq1f : 1; /** More on page 112 in SH-2E SH7055S F-ZTAT TM Hardware Manual */
        uint16_t irq2f : 1;
        uint16_t irq3f : 1;
        uint16_t irq4f : 1;
        uint16_t irq5f : 1;
        uint16_t irq6f : 1;
        uint16_t irq7f : 1;
#else
        uint16_t irq7f : 1;
        uint16_t irq6f : 1;
        uint16_t irq5f : 1;
        uint16_t irq4f : 1;
        uint16_t irq3f : 1;
        uint16_t irq2f : 1;
        uint16_t irq1f : 1;
        uint16_t irq0f : 1; /** Bits 7 to 0—IRQ0–IRQ7 Flags (IRQ0F–IRQ7F): These bits display the IRQ0–IRQ7 interrupt request status.   */
        uint16_t _reserved : 8; /** Bits 15 to 8—Reserved: These bits always read 0. The write value should always be 0.  */
#endif
    };
} sh2e_intc_isr_t;

typedef struct sh2e_intc_regs {

    uint16_t priority[SH2E_INTC_IPR_REGISTERS_COUNT];

    union {
        uint16_t system[SH2E_INTC_SYSTEM_REGISTERS_COUNT];

        PACKED struct {
            /** Interrupt Control Register (ICR) */
            sh2e_intc_icr_t icr;

            /** IRQ Status Register (ISR) */
            sh2e_intc_isr_t isr;
        };
    };


} sh2e_intc_regs_t;

_Static_assert(sizeof(sh2e_intc_regs_t) == sizeof(uint16_t) * (SH2E_INTC_IPR_REGISTERS_COUNT + SH2E_INTC_SYSTEM_REGISTERS_COUNT), "invalid sh2e_intc_regs_t size!");

typedef struct sh2e_intc_source {
    uint8_t priority_pool_index; /** All priority registers are divided into 4 half bytes, so there are 48 possible indices (0-47) */

    bool registered; /** Whether the interrupt source was registered via config */

    bool pending; /** Flag indicating if the interrupt is pending */
} sh2e_intc_source_t;

typedef struct sh2e_intc {

    sh2e_intc_regs_t * intc_regs; /** Interrupt controller registers */

    uint16_t nmi_prev_value; /** Previous value of NMI pin */

    // NOTE: invalid values (SH2E_INTC_VALID_SOURCE_ID) are not used
    sh2e_intc_source_t sources[SH2E_INTC_SOURCE_MAX_VALUE + 1]; /** Interrupt sources pool */

    /** Internal use */
    uint8_t interrupt_out; /** Interrupt number with the highest priority */

    uint32_t priority_out; /** Priority of the interrupt source that could be accepted by the CPU */

    unsigned int id; /** ID of the INTC */

} sh2e_intc_t;


extern void sh2e_intc_init(sh2e_intc_t * intc, unsigned int id);

extern void sh2e_intc_init_regs(sh2e_intc_t * intc);

extern void sh2e_intc_change_regs_address(sh2e_intc_t * intc, uint32_t regs_addr);

extern void sh2e_intc_done(sh2e_intc_t * intc);

extern bool sh2e_check_pending_interrupts(sh2e_intc_t * intc, uint8_t mask, uint8_t * interrupt_out);

extern bool sh2e_check_pending_resets(sh2e_intc_t * intc, uint8_t * reset_out);

extern void sh2e_accept_interrupt(sh2e_intc_t * intc, uint32_t * new_mask_out);

extern void sh2e_accept_reset(sh2e_intc_t * intc);

extern void sh2e_intc_add_interrupt_source(sh2e_intc_t * intc, uint8_t source_id, uint8_t priority_pool_index, uint8_t priority);

extern void sh2e_intc_assert_interrupt(sh2e_intc_t * intc, unsigned int num);

extern void sh2e_intc_deassert_interrupt(sh2e_intc_t * intc, unsigned int num);

#endif // SUPERH_SH2E_INTC_H_
