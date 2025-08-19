

#include "cpu.h"
#include "memops.h"

#include <string.h>

#define address(ptr) (uintptr_t) (ptr)

static sh2e_intc_icr_t
sh2e_intc_icr_reg_read(sh2e_cpu_t * cpu) {
    return (sh2e_intc_icr_t) sh2e_physmem_read16(cpu->id, address(&cpu->intc.intc_regs->icr), true);
}

static sh2e_intc_isr_t
sh2e_intc_isr_reg_read(sh2e_cpu_t * cpu) {
    return (sh2e_intc_isr_t) sh2e_physmem_read16(cpu->id, address(&cpu->intc.intc_regs->isr), true);
}

static void
sh2e_intc_icr_reg_write(sh2e_cpu_t * cpu, uint16_t value) {
    physmem_write16(cpu->id, address(&cpu->intc.intc_regs->icr), htobe16(value), true);
}

static void
sh2e_intc_isr_reg_write(sh2e_cpu_t * cpu, uint16_t value) {
    physmem_write16(cpu->id, address(&cpu->intc.intc_regs->isr), htobe16(value), true);
}

static uint16_t
sh2e_intc_priority_register_read(sh2e_cpu_t * cpu, sh2e_ipr_index_t index) {
    ASSERT(index < INTC_IPR_REGISTERS_COUNT && "wrong index for priority register");
    return sh2e_physmem_read16(cpu->id, address(&cpu->intc.intc_regs->priority[index]), true);
}

static void
sh2e_intc_priority_register_write(sh2e_cpu_t * cpu, sh2e_ipr_index_t index, uint16_t value) {
    ASSERT(index < INTC_IPR_REGISTERS_COUNT && "wrong index for priority register");

    physmem_write16(cpu->id, address(&cpu->intc.intc_regs->priority[index]), htobe16(value), true);
}

static bool
sh2e_intc_is_interrupt_source_irq(sh2e_intc_source_t interrupt_source) {
    return interrupt_source >= irq0 && interrupt_source <= irq7;
}

void
sh2e_intc_init(sh2e_cpu_t * cpu) {
    memset(&cpu->intc, 0, sizeof(sh2e_intc_t));

    cpu->intc.intc_regs = (sh2e_intc_regs_t *) (uintptr_t) (INTC_REGISTERS_START_ADDRESS);

    /** Setup the priority registers */
    for (size_t i = 0; i < INTC_IPR_REGISTERS_COUNT; i++) {
        sh2e_intc_priority_register_write(cpu, i, 0);
    }

    /** Setup the system registers */
    // TODO: if the NMI pin is high the value of ICR should be H'8000
    sh2e_intc_icr_reg_write(cpu, 0);
    sh2e_intc_isr_reg_write(cpu, 0);
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

static bool
sh2e_check_irq_interrupt(sh2e_cpu_t * cpu, unsigned int irq_index) {
    ASSERT(cpu != NULL);
    ASSERT(irq_index < 8 && "IRQ index out of range");

    sh2e_intc_isr_t isr = sh2e_intc_isr_reg_read(cpu);

    return (isr.value >> irq_index) & 1;
}

static void
sh2e_clear_irq_interrupt(sh2e_cpu_t * cpu, unsigned int irq_index) {
    ASSERT(cpu != NULL);
    ASSERT(irq_index < 8 && "IRQ index out of range");

    sh2e_intc_isr_t isr = sh2e_intc_isr_reg_read(cpu);
    sh2e_intc_isr_reg_write(cpu, isr.value & ~(1 << irq_index));
}

static uint16_t
sh2e_intc_get_priority_from_source(sh2e_intc_priority_regs_t * priority_regs, sh2e_intc_source_t interrupt_source) {
    ASSERT(interrupt_source <= INTC_SOURCE_MAX_VALUE && "Interrupt source out of range");

    switch (interrupt_source) {
    case nmi:
    case ubc:
    case h_udi:
        return INTC_PRIORITY_MAX_VALUE;
    case irq0: {
        return priority_regs->ipra.irq0;
    }
    case irq1: {
        return priority_regs->ipra.irq1;
    }
    case irq2: {
        return priority_regs->ipra.irq2;
    }
    case irq3: {
        return priority_regs->ipra.irq3;
    }

    case irq4: {
        return priority_regs->iprb.irq4;
    }
    case irq5: {
        return priority_regs->iprb.irq5;
    }
    case irq6: {
        return priority_regs->iprb.irq6;
    }
    case irq7: {
        return priority_regs->iprb.irq7;
    }

    case dmac0_dei0:
    case dmac1_dei1: {
        return priority_regs->iprc.dmac01;
    }
    case dmac2_dei2:
    case dmac3_dei3: {
        return priority_regs->iprc.dmac23;
    }
    case atu01: {
        return priority_regs->iprc.atu01;
    }
    case atu02_ici0a:
    case atu02_ici0b: {
        return priority_regs->iprc.atu02;
    }

    case atu03_ici0c:
    case atu03_ici0d: {
        return priority_regs->iprd.atu03;
    }
    case atu04: {
        return priority_regs->iprd.atu04;
    }
    case atu11_imi1a:
    case atu11_imi1b:
    case atu11_imi1c:
    case atu11_imi1d: {
        return priority_regs->iprd.atu11;
    }
    case atu12_imi1e:
    case atu12_imi1f:
    case atu12_imi1g:
    case atu12_imi1h: {
        return priority_regs->iprd.atu12;
    }

    case atu13: {
        return priority_regs->ipre.atu13;
    }
    case atu21_imi2a:
    case atu21_imi2b:
    case atu21_imi2c:
    case atu21_imi2d: {
        return priority_regs->ipre.atu21;
    }
    case atu22_imi2e:
    case atu22_imi2f:
    case atu22_imi2g:
    case atu22_imi2h: {
        return priority_regs->ipre.atu22;
    }
    case atu23: {
        return priority_regs->ipre.atu23;
    }

    case atu31_imi3a:
    case atu31_imi3b:
    case atu31_imi3c:
    case atu31_imi3d: {
        return priority_regs->iprf.atu31;
    }
    case atu32: {
        return priority_regs->iprf.atu32;
    }
    case atu41_imi4a:
    case atu41_imi4b:
    case atu41_imi4c:
    case atu41_imi4d: {
        return priority_regs->iprf.atu41;
    }
    case atu42: {
        return priority_regs->iprf.atu42;
    }

    case atu51_imi5a:
    case atu51_imi5b:
    case atu51_imi5c:
    case atu51_imi5d: {
        return priority_regs->iprg.atu51;
    }
    case atu52: {
        return priority_regs->iprg.atu52;
    }
    case atu6_cmi6a:
    case atu6_cmi6b:
    case atu6_cmi6c:
    case atu6_cmi6d: {
        return priority_regs->iprg.atu6;
    }
    case atu7_cmi7a:
    case atu7_cmi7b:
    case atu7_cmi7c:
    case atu7_cmi7d: {
        return priority_regs->iprg.atu7;
    }

    case atu81_osi8a:
    case atu81_osi8b:
    case atu81_osi8c:
    case atu81_osi8d: {
        return priority_regs->iprh.atu81;
    }
    case atu82_osi8e:
    case atu82_osi8f:
    case atu82_osi8g:
    case atu82_osi8h: {
        return priority_regs->iprh.atu82;
    }
    case atu83_osi8i:
    case atu83_osi8j:
    case atu83_osi8k:
    case atu83_osi8l: {
        return priority_regs->iprh.atu83;
    }
    case atu84_osi8m:
    case atu84_osi8n:
    case atu84_osi8o:
    case atu84_osi8p: {
        return priority_regs->iprh.atu84;
    }

    case atu91_cmi9a:
    case atu91_cmi9b:
    case atu91_cmi9c:
    case atu91_cmi9d: {
        return priority_regs->ipri.atu91;
    }
    case atu92_cmi9e:
    case atu92_cmi9f: {
        return priority_regs->ipri.atu92;
    }
    case atu101_cmi10a:
    case atu101_cmi10b: {
        return priority_regs->ipri.atu101;
    }
    case atu102_ici10a: {
        return priority_regs->ipri.atu102;
    }

    case atu11_imi11a:
    case atu11_imi11b:
    case atu11_ovi11: {
        return priority_regs->iprj.atu11;
    }
    case cmt0:
    case a_d0: {
        return priority_regs->iprj.cmt0_ad0;
    }
    case cmt1:
    case a_d1: {
        return priority_regs->iprj.cmt1_ad1;
    }
    case a_d2: {
        return priority_regs->iprj.ad2;
    }

    case sci0_eri0:
    case sci0_rxi0:
    case sci0_txi0:
    case sci0_tei0: {
        return priority_regs->iprk.sci0;
    }
    case sci1_eri1:
    case sci1_rxi1:
    case sci1_txi1:
    case sci1_tei1: {
        return priority_regs->iprk.sci1;
    }
    case sci2_eri2:
    case sci2_rxi2:
    case sci2_txi2:
    case sci2_tei2: {
        return priority_regs->iprk.sci2;
    }
    case sci3_eri3:
    case sci3_rxi3:
    case sci3_txi3:
    case sci3_tei3: {
        return priority_regs->iprk.sci3;
    }

    case sci4_eri4:
    case sci4_rxi4:
    case sci4_txi4:
    case sci4_tei4: {
        return priority_regs->iprl.sci4;
    }
    case hcan0_ers0:
    case hcan0_ovr0:
    case hcan0_rm0:
    case hcan0_sle0: {
        return priority_regs->iprl.hcan0;
    }
    case wdt: {
        return priority_regs->iprl.wdt;
    }
    case hcan1_ers1:
    case hcan1_ovr1:
    case hcan1_rm1:
    case hcan1_sle1: {
        return priority_regs->iprl.hcan1;
    }

    default:
        return 0;
    }
}

sh2e_intc_source_t
sh2e_check_pending_interrupts(sh2e_cpu_t * cpu) {
    ASSERT(cpu != NULL);

    // Check NMI
    if (sh2e_check_nmi_interrupt(cpu)) {
        cpu->intc.to_be_accepted_priority = INTC_PRIORITY_MAX_VALUE;
        return nmi;
    }

    // Check UBC
    else if (cpu->intc.interrupts_table[ubc]) {
        cpu->intc.to_be_accepted_priority = INTC_PRIORITY_MAX_VALUE;
        return ubc;
    }

    // Check H-UDI
    else if (cpu->intc.interrupts_table[h_udi]) {
        cpu->intc.to_be_accepted_priority = INTC_PRIORITY_MAX_VALUE;
        return h_udi;
    }

    // Load all priority registers
    sh2e_intc_ipra_t ipra = (sh2e_intc_ipra_t) sh2e_intc_priority_register_read(cpu, sh2e_intc_ipra);
    sh2e_intc_iprb_t iprb = (sh2e_intc_iprb_t) sh2e_intc_priority_register_read(cpu, sh2e_intc_iprb);
    sh2e_intc_iprc_t iprc = (sh2e_intc_iprc_t) sh2e_intc_priority_register_read(cpu, sh2e_intc_iprc);
    sh2e_intc_iprd_t iprd = (sh2e_intc_iprd_t) sh2e_intc_priority_register_read(cpu, sh2e_intc_iprd);
    sh2e_intc_ipre_t ipre = (sh2e_intc_ipre_t) sh2e_intc_priority_register_read(cpu, sh2e_intc_ipre);
    sh2e_intc_iprf_t iprf = (sh2e_intc_iprf_t) sh2e_intc_priority_register_read(cpu, sh2e_intc_iprf);
    sh2e_intc_iprg_t iprg = (sh2e_intc_iprg_t) sh2e_intc_priority_register_read(cpu, sh2e_intc_iprg);
    sh2e_intc_iprh_t iprh = (sh2e_intc_iprh_t) sh2e_intc_priority_register_read(cpu, sh2e_intc_iprh);
    sh2e_intc_ipri_t ipri = (sh2e_intc_ipri_t) sh2e_intc_priority_register_read(cpu, sh2e_intc_ipri);
    sh2e_intc_iprj_t iprj = (sh2e_intc_iprj_t) sh2e_intc_priority_register_read(cpu, sh2e_intc_iprj);
    sh2e_intc_iprk_t iprk = (sh2e_intc_iprk_t) sh2e_intc_priority_register_read(cpu, sh2e_intc_iprk);
    sh2e_intc_iprl_t iprl = (sh2e_intc_iprl_t) sh2e_intc_priority_register_read(cpu, sh2e_intc_iprl);

    sh2e_intc_priority_regs_t priority_regs = {
        .ipra = ipra,
        .iprb = iprb,
        .iprc = iprc,
        .iprd = iprd,
        .ipre = ipre,
        .iprf = iprf,
        .iprg = iprg,
        .iprh = iprh,
        .ipri = ipri,
        .iprj = iprj,
        .iprk = iprk,
        .iprl = iprl
    };

    sh2e_intc_source_t interrupt_source = no_interrupt;
    uint16_t interrupt_priority = 0;

    uint16_t mask = cpu->cpu_regs.sr.im;

    // Check if any other interrupt is pending.
    for (unsigned int i = irq0; i <= INTC_SOURCE_MAX_VALUE; i++) {
        bool interrupt_pending = sh2e_intc_is_interrupt_source_irq(i) ? sh2e_check_irq_interrupt(cpu, i - irq0) : cpu->intc.interrupts_table[i];

        if (interrupt_pending) {
            uint16_t curr_priority = sh2e_intc_get_priority_from_source(&priority_regs, i);
            if ((curr_priority > interrupt_priority || interrupt_source == no_interrupt) && curr_priority > mask) {
                interrupt_priority = curr_priority;
                interrupt_source = i;
            }
        }
    }

    cpu->intc.to_be_accepted_priority = interrupt_priority;
    return interrupt_source;
}


void
sh2e_accept_interrupt(sh2e_cpu_t * cpu, sh2e_intc_source_t interrupt_source) {
    ASSERT(cpu != NULL);
    ASSERT(interrupt_source > no_interrupt && interrupt_source <= INTC_SOURCE_MAX_VALUE && "Interrupt source out of range");

    cpu->cpu_regs.sr.im = cpu->intc.to_be_accepted_priority;
    cpu->intc.to_be_accepted_priority = 0;

    // Clear the interrupt request
    if (sh2e_intc_is_interrupt_source_irq(interrupt_source)) {
        sh2e_clear_irq_interrupt(cpu, interrupt_source - irq0);
    }
    cpu->intc.interrupts_table[interrupt_source] = false;
}
