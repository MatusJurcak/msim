

#include "intc.h"

#include "../../../arch/endianness.h"
#include "../../../assert.h"
#include "../../../physmem.h"

#include <string.h>

#define address(ptr) (uintptr_t) (ptr)

static sh2e_intc_icr_t
sh2e_intc_icr_reg_read(sh2e_intc_t * intc) {
    return (sh2e_intc_icr_t) be16toh(physmem_read16(0, address(&intc->intc_regs->icr), true));
}

static sh2e_intc_isr_t
sh2e_intc_isr_reg_read(sh2e_intc_t * intc) {
    return (sh2e_intc_isr_t) be16toh(physmem_read16(0, address(&intc->intc_regs->isr), true));
}

static void
sh2e_intc_icr_reg_write(sh2e_intc_t * intc, uint16_t value) {
    physmem_write16(0, address(&intc->intc_regs->icr), htobe16(value), true);
}

static void
sh2e_intc_isr_reg_write(sh2e_intc_t * intc, uint16_t value) {
    physmem_write16(0, address(&intc->intc_regs->isr), htobe16(value), true);
}

static uint16_t
sh2e_intc_priority_register_read(sh2e_intc_t * intc, uint8_t index) {
    ASSERT(index < INTC_IPR_REGISTERS_COUNT && "wrong index for priority register");
    return be16toh(physmem_read16(0, address(&intc->intc_regs->priority[index]), true));
}

static void
sh2e_intc_priority_register_write(sh2e_intc_t * intc, uint8_t index, uint16_t value) {
    ASSERT(index < INTC_IPR_REGISTERS_COUNT && "wrong index for priority register");

    physmem_write16(0, address(&intc->intc_regs->priority[index]), htobe16(value), true);
}

void
sh2e_intc_init_regs(sh2e_intc_t * intc, uint32_t regs_addr) {
    ASSERT(intc != NULL);

    intc->intc_regs = (sh2e_intc_regs_t *) (address(regs_addr));

    /** Setup the priority registers */
    for (size_t i = 0; i < INTC_IPR_REGISTERS_COUNT; i++) {
        sh2e_intc_priority_register_write(intc, i, 0);
    }

    /** Setup the system registers */
    // TODO: if the NMI pin is high the value of ICR should be H'8000
    sh2e_intc_icr_reg_write(intc, 0);
    sh2e_intc_isr_reg_write(intc, 0);
}

void
sh2e_intc_reset(sh2e_intc_t * intc) {
    ASSERT(intc != NULL);

    sh2e_intc_init_regs(intc, INTC_REGISTERS_START_ADDRESS);
}

void
sh2e_intc_init(sh2e_intc_t * intc, unsigned int id) {
    memset(intc, 0, sizeof(sh2e_intc_t));

    intc->id = id;

    sh2e_intc_reset(intc);
}

void
sh2e_intc_done(sh2e_intc_t * intc) {
    ASSERT(intc != NULL);
    // Nothing to do here yet
}

void
sh2e_intc_add_interrupt_source(sh2e_intc_t * intc, uint8_t source_id, uint8_t priority_pool_index, uint8_t priority) {
    ASSERT(intc != NULL);
    ASSERT(source_id >= INTC_SOURCE_MIN_VALUE && source_id <= INTC_SOURCE_MAX_VALUE && "Interrupt source ID out of range");
    ASSERT(priority_pool_index < INTC_IPR_HALF_BYTES_LENGTH);
    ASSERT(priority <= INTC_PRIORITY_MAX_VALUE);

    if (intc->sources[source_id].registered) {
        ASSERT(false && "Interrupt source with this ID already registered");
        return;
    }

    intc->sources[source_id].priority_pool_index = priority_pool_index;
    intc->sources[source_id].pending = true;
    intc->sources[source_id].registered = true;

    uint16_t priority_register = sh2e_intc_priority_register_read(intc, priority_pool_index / 4);

    // Need this in order to reverse the order and shift correctly
    int shift = (3 - (priority_pool_index % 4)) * 4;

    priority_register &= ~(0xF << shift);
    priority_register |= (priority & 0xF) << shift;

    sh2e_intc_priority_register_write(intc, priority_pool_index / 4, priority_register);
}

static bool
sh2e_check_nmi_interrupt(sh2e_intc_t * intc) {
    ASSERT(intc != NULL);

    sh2e_intc_icr_t icr = sh2e_intc_icr_reg_read(intc);
    bool detected = false;
    if (icr.nmie) {
        // NMI interrupt is detected on the rising edge of the NMI pin
        detected = intc->nmi_prev_value == 0 && icr.nmil;
    } else {
        // NMI interrupt is detected on the falling edge of the NMI pin
        detected = intc->nmi_prev_value != 0 && !icr.nmil;
    }
    intc->nmi_prev_value = icr.nmil;
    return detected;
}

static bool
sh2e_check_irq_interrupt(sh2e_intc_t * intc, unsigned int irq_index) {
    ASSERT(intc != NULL);
    ASSERT(irq_index < INTC_IRQ_NUMBER_OF_SOURCES && "IRQ index out of range");

    sh2e_intc_isr_t isr = sh2e_intc_isr_reg_read(intc);
    uint8_t shift = 7 - irq_index;

    return (isr.value >> shift) & 1;
}

static void
sh2e_assert_irq_interrupt(sh2e_intc_t * intc, unsigned int irq_index) {
    ASSERT(intc != NULL);
    ASSERT(irq_index < INTC_IRQ_NUMBER_OF_SOURCES && "IRQ index out of range");

    sh2e_intc_isr_t isr = sh2e_intc_isr_reg_read(intc);
    uint8_t shift = 7 - irq_index;

    isr.value |= (1 << shift);
    sh2e_intc_isr_reg_write(intc, isr.value);
}

static void
sh2e_clear_irq_interrupt(sh2e_intc_t * intc, unsigned int irq_index) {
    ASSERT(intc != NULL);
    ASSERT(irq_index < INTC_IRQ_NUMBER_OF_SOURCES && "IRQ index out of range");

    sh2e_intc_icr_t icr = sh2e_intc_icr_reg_read(intc);
    sh2e_intc_isr_t isr = sh2e_intc_isr_reg_read(intc);

    uint8_t shift = 7 - irq_index;
    uint8_t irq_sense = (icr.value >> shift) & 1;

    // Edge detection
    if (irq_sense) {
        isr.value &= ~(1 << shift);
        sh2e_intc_isr_reg_write(intc, isr.value);
    }

    // If the level detection is used, the interrupt request remains set until the IRQ pin is cleared
}


static uint8_t
sh2e_get_priority(sh2e_intc_source_t * source, uint16_t * priority_regs) {
    ASSERT(source != NULL);

    uint8_t reg_index = source->priority_pool_index / 4;
    uint8_t half_byte_index = (source->priority_pool_index % 4);

    // Need this in order to reverse the order and shift correctly
    int shift = (3 - half_byte_index) * 4;

    return (priority_regs[reg_index] >> shift) & 0x0F;
}

uint8_t
sh2e_check_pending_interrupts(sh2e_intc_t * intc, uint8_t mask) {
    ASSERT(intc != NULL);

    // Check NMI
    if (sh2e_check_nmi_interrupt(intc)) {
        intc->interrupt_out = INTC_NMI_VECTOR_ADDRESS_OFFSET;
        intc->priority_out = INTC_PRIORITY_MAX_VALUE;
        return INTC_NMI_VECTOR_ADDRESS_OFFSET;
    }

    // Load all priority registers
    uint16_t priority_regs[INTC_IPR_REGISTERS_COUNT];
    for (int i = 0; i < INTC_IPR_REGISTERS_COUNT; i++) {
        priority_regs[i] = sh2e_intc_priority_register_read(intc, i);
    }

    uint8_t interrupt_source = 0;
    uint8_t interrupt_priority = 0;

    // Check if any other interrupt is pending.
    for (unsigned int i = 0; i < INTC_SOURCE_MAX_VALUE; i++) {
        if (INTC_VALID_SOURCE_ID(i) && intc->sources[i].registered) {
            bool interrupt_pending = INTC_VALID_IRQ_SOURCE_ID(i) ? sh2e_check_irq_interrupt(intc, i - INTC_IRQ_VECTOR_ADDRESS_OFFSET) : intc->sources[i].pending;
            if (interrupt_pending) {
                uint8_t curr_priority = sh2e_get_priority(&intc->sources[i], priority_regs);
                if ((curr_priority > interrupt_priority || !interrupt_source) && curr_priority > mask) {
                    interrupt_priority = curr_priority;
                    interrupt_source = i;
                }
            }
        }
    }

    intc->priority_out = interrupt_priority;
    intc->interrupt_out = interrupt_source;
    return interrupt_source;
}


uint8_t
sh2e_accept_interrupt(sh2e_intc_t * intc) {
    ASSERT(intc != NULL);

    uint8_t new_mask = intc->priority_out;

    intc->priority_out = 0;

    // Clear the interrupt request
    if (INTC_VALID_IRQ_SOURCE_ID(intc->interrupt_out)) {
        sh2e_clear_irq_interrupt(intc, intc->interrupt_out - INTC_IRQ_VECTOR_ADDRESS_OFFSET);
    } else {
        intc->sources[intc->interrupt_out].pending = false;
        intc->interrupt_out = 0;
    }

    return new_mask;
}


/** Assert the specified interrupt. */
void
sh2e_intc_assert_interrupt(sh2e_intc_t * intc, unsigned int num) {
    ASSERT(intc != NULL);
    ASSERT(INTC_VALID_SOURCE_ID(num) && "Interrupt number out of range");
    ASSERT(intc->sources[num].registered && "Interrupt source not registered!");

    if (INTC_VALID_IRQ_SOURCE_ID(num)) {
        sh2e_assert_irq_interrupt(intc, num - INTC_IRQ_VECTOR_ADDRESS_OFFSET);
        return;
    }

    intc->sources[num].pending = true;
}

/** Deassert the specified interrupt */
void
sh2e_intc_deassert_interrupt(sh2e_intc_t * intc, unsigned int num) {
    ASSERT(intc != NULL);
    ASSERT(INTC_VALID_SOURCE_ID(num) && "Interrupt number out of range");
    ASSERT(intc->sources[num].registered && "Interrupt source not registered!");

    if (INTC_VALID_IRQ_SOURCE_ID(num)) {
        sh2e_clear_irq_interrupt(intc, num - INTC_IRQ_VECTOR_ADDRESS_OFFSET);
        return;
    }

    intc->sources[num].pending = false;
}
