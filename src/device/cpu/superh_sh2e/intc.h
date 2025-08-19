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
#define INTC_REGISTERS_MEMORY_SIZE 28
#define INTC_IPR_REGISTERS_COUNT 12
#define INTC_SYSTEM_REGISTERS_COUNT 2
#define INTC_PRIORITY_MAX_VALUE 15
#define INTC_SOURCE_MAX_VALUE 231

/** Interrupt sources defined as vector numbers from Table 7.3 SH-2E SH7055S F-ZTAT TM Hardware Manual  */
typedef enum {
    // For internal use
    no_interrupt = 0,

    // Interrupts with the highest priority
    nmi = 11,
    ubc = 12,
    h_udi = 14,

    // IRQ Interrupts
    irq0 = 64,
    irq1 = 65,
    irq2 = 66,
    irq3 = 67,
    irq4 = 68,
    irq5 = 69,
    irq6 = 70,
    irq7 = 71,

    // On-Chip Peripheral Module Interrupts
    dmac0_dei0 = 72,
    dmac1_dei1 = 74,
    dmac2_dei2 = 76,
    dmac3_dei3 = 78,

    atu01 = 80,
    atu02_ici0a = 84,
    atu02_ici0b = 86,
    atu03_ici0c = 88,
    atu03_ici0d = 90,
    atu04 = 92,
    atu11_imi1a = 96,
    atu11_imi1b = 97,
    atu11_imi1c = 98,
    atu11_imi1d = 99,
    atu12_imi1e = 100,
    atu12_imi1f = 101,
    atu12_imi1g = 102,
    atu12_imi1h = 103,
    atu13 = 104,
    atu21_imi2a = 108,
    atu21_imi2b = 109,
    atu21_imi2c = 110,
    atu21_imi2d = 111,
    atu22_imi2e = 112,
    atu22_imi2f = 113,
    atu22_imi2g = 114,
    atu22_imi2h = 115,
    atu23 = 116,
    atu31_imi3a = 120,
    atu31_imi3b = 121,
    atu31_imi3c = 122,
    atu31_imi3d = 123,
    atu32 = 124,
    atu41_imi4a = 128,
    atu41_imi4b = 129,
    atu41_imi4c = 130,
    atu41_imi4d = 131,
    atu42 = 132,
    atu51_imi5a = 136,
    atu51_imi5b = 137,
    atu51_imi5c = 138,
    atu51_imi5d = 139,
    atu52 = 140,
    atu6_cmi6a = 144,
    atu6_cmi6b = 145,
    atu6_cmi6c = 146,
    atu6_cmi6d = 147,
    atu7_cmi7a = 148,
    atu7_cmi7b = 149,
    atu7_cmi7c = 150,
    atu7_cmi7d = 151,
    atu81_osi8a = 152,
    atu81_osi8b = 153,
    atu81_osi8c = 154,
    atu81_osi8d = 155,
    atu82_osi8e = 156,
    atu82_osi8f = 157,
    atu82_osi8g = 158,
    atu82_osi8h = 159,
    atu83_osi8i = 160,
    atu83_osi8j = 161,
    atu83_osi8k = 162,
    atu83_osi8l = 163,
    atu84_osi8m = 164,
    atu84_osi8n = 165,
    atu84_osi8o = 166,
    atu84_osi8p = 167,
    atu91_cmi9a = 168,
    atu91_cmi9b = 169,
    atu91_cmi9c = 170,
    atu91_cmi9d = 171,
    atu92_cmi9e = 172,
    atu92_cmi9f = 174,
    atu101_cmi10a = 176,
    atu101_cmi10b = 178,
    atu102_ici10a = 180,
    atu11_imi11a = 184,
    atu11_imi11b = 186,
    atu11_ovi11 = 187,

    cmt0 = 188,
    a_d0 = 190,
    cmt1 = 192,
    a_d1 = 194,
    a_d2 = 196,

    sci0_eri0 = 200,
    sci0_rxi0 = 201,
    sci0_txi0 = 202,
    sci0_tei0 = 203,
    sci1_eri1 = 204,
    sci1_rxi1 = 205,
    sci1_txi1 = 206,
    sci1_tei1 = 207,
    sci2_eri2 = 208,
    sci2_rxi2 = 209,
    sci2_txi2 = 210,
    sci2_tei2 = 211,
    sci3_eri3 = 212,
    sci3_rxi3 = 213,
    sci3_txi3 = 214,
    sci3_tei3 = 215,
    sci4_eri4 = 216,
    sci4_rxi4 = 217,
    sci4_txi4 = 218,
    sci4_tei4 = 219,

    hcan0_ers0 = 220,
    hcan0_ovr0 = 221,
    hcan0_rm0 = 222,
    hcan0_sle0 = 223,

    wdt = 224,

    hcan1_ers1 = 228,
    hcan1_ovr1 = 229,
    hcan1_rm1 = 230,
    hcan1_sle1 = 231,
} sh2e_intc_source_t;

typedef struct sh2e_cpu sh2e_cpu_t;

typedef union sh2e_intc_ipra {
    uint16_t value;

    PACKED struct {
#ifdef WORDS_BIGENDIAN
        uint16_t irq0 : 4; /** Bits 15-12 */
        uint16_t irq1 : 4; /** Bits 11-8 */
        uint16_t irq2 : 4; /** Bits 7-4 */
        uint16_t irq3 : 4; /** Bits 3-0 */
#else
        uint16_t irq3 : 4; /** Bits 3-0 */
        uint16_t irq2 : 4; /** Bits 7-4 */
        uint16_t irq1 : 4; /** Bits 11-8 */
        uint16_t irq0 : 4; /** Bits 15-12 */
#endif
    };
} sh2e_intc_ipra_t;


typedef union sh2e_intc_iprb {
    uint16_t value;

    PACKED struct {
#ifdef WORDS_BIGENDIAN
        uint16_t irq4 : 4; /** Bits 15-12 */
        uint16_t irq5 : 4; /** Bits 11-8 */
        uint16_t irq6 : 4; /** Bits 7-4 */
        uint16_t irq7 : 4; /** Bits 3-0 */
#else
        uint16_t irq7 : 4; /** Bits 3-0 */
        uint16_t irq6 : 4; /** Bits 7-4 */
        uint16_t irq5 : 4; /** Bits 11-8 */
        uint16_t irq4 : 4; /** Bits 15-12 */
#endif
    };
} sh2e_intc_iprb_t;


typedef union sh2e_intc_iprc {
    uint16_t value;

    PACKED struct {
#ifdef WORDS_BIGENDIAN
        uint16_t dmac01 : 4; /** Bits 15-12 */
        uint16_t dmac23 : 4; /** Bits 11-8 */
        uint16_t atu01 : 4; /** Bits 7-4 */
        uint16_t atu02 : 4; /** Bits 3-0 */
#else
        uint16_t atu02 : 4; /** Bits 3-0 */
        uint16_t atu01 : 4; /** Bits 7-4 */
        uint16_t dmac23 : 4; /** Bits 11-8 */
        uint16_t dmac01 : 4; /** Bits 15-12 */
#endif
    };
} sh2e_intc_iprc_t;


typedef union sh2e_intc_iprd {
    uint16_t value;

    PACKED struct {
#ifdef WORDS_BIGENDIAN
        uint16_t atu03 : 4; /** Bits 15-12 */
        uint16_t atu04 : 4; /** Bits 11-8 */
        uint16_t atu11 : 4; /** Bits 7-4 */
        uint16_t atu12 : 4; /** Bits 3-0 */
#else
        uint16_t atu12 : 4; /** Bits 3-0 */
        uint16_t atu11 : 4; /** Bits 7-4 */
        uint16_t atu04 : 4; /** Bits 11-8 */
        uint16_t atu03 : 4; /** Bits 15-12 */
#endif
    };
} sh2e_intc_iprd_t;

typedef union sh2e_intc_ipre {
    uint16_t value;

    PACKED struct {
#ifdef WORDS_BIGENDIAN
        uint16_t atu13 : 4; /** Bits 15-12 */
        uint16_t atu21 : 4; /** Bits 11-8 */
        uint16_t atu22 : 4; /** Bits 7-4 */
        uint16_t atu23 : 4; /** Bits 3-0 */
#else
        uint16_t atu23 : 4; /** Bits 3-0 */
        uint16_t atu22 : 4; /** Bits 7-4 */
        uint16_t atu21 : 4; /** Bits 11-8 */
        uint16_t atu13 : 4; /** Bits 15-12 */
#endif
    };
} sh2e_intc_ipre_t;


typedef union sh2e_intc_iprf {
    uint16_t value;

    PACKED struct {
#ifdef WORDS_BIGENDIAN
        uint16_t atu31 : 4; /** Bits 15-12 */
        uint16_t atu32 : 4; /** Bits 11-8 */
        uint16_t atu41 : 4; /** Bits 7-4 */
        uint16_t atu42 : 4; /** Bits 3-0 */
#else
        uint16_t atu42 : 4; /** Bits 3-0 */
        uint16_t atu41 : 4; /** Bits 7-4 */
        uint16_t atu32 : 4; /** Bits 11-8 */
        uint16_t atu31 : 4; /** Bits 15-12 */
#endif
    };
} sh2e_intc_iprf_t;


typedef union sh2e_intc_iprg {
    uint16_t value;

    PACKED struct {
#ifdef WORDS_BIGENDIAN
        uint16_t atu51 : 4; /** Bits 15-12 */
        uint16_t atu52 : 4; /** Bits 11-8 */
        uint16_t atu6 : 4; /** Bits 7-4 */
        uint16_t atu7 : 4; /** Bits 3-0 */
#else
        uint16_t atu7 : 4; /** Bits 3-0 */
        uint16_t atu6 : 4; /** Bits 7-4 */
        uint16_t atu52 : 4; /** Bits 11-8 */
        uint16_t atu51 : 4; /** Bits 15-12 */
#endif
    };
} sh2e_intc_iprg_t;


typedef union sh2e_intc_iprh {
    uint16_t value;

    PACKED struct {
#ifdef WORDS_BIGENDIAN
        uint16_t atu81 : 4; /** Bits 15-12 */
        uint16_t atu82 : 4; /** Bits 11-8 */
        uint16_t atu83 : 4; /** Bits 7-4 */
        uint16_t atu84 : 4; /** Bits 3-0 */
#else
        uint16_t atu84 : 4; /** Bits 3-0 */
        uint16_t atu83 : 4; /** Bits 7-4 */
        uint16_t atu82 : 4; /** Bits 11-8 */
        uint16_t atu81 : 4; /** Bits 15-12 */
#endif
    };
} sh2e_intc_iprh_t;


typedef union sh2e_intc_ipri {
    uint16_t value;

    PACKED struct {
#ifdef WORDS_BIGENDIAN
        uint16_t atu91 : 4; /** Bits 15-12 */
        uint16_t atu92 : 4; /** Bits 11-8 */
        uint16_t atu101 : 4; /** Bits 7-4 */
        uint16_t atu102 : 4; /** Bits 3-0 */
#else
        uint16_t atu102 : 4; /** Bits 3-0 */
        uint16_t atu101 : 4; /** Bits 7-4 */
        uint16_t atu92 : 4; /** Bits 11-8 */
        uint16_t atu91 : 4; /** Bits 15-12 */
#endif
    };
} sh2e_intc_ipri_t;

typedef union sh2e_intc_iprj {
    uint16_t value;

    PACKED struct {
#ifdef WORDS_BIGENDIAN
        // ????????? ATU 11 ??????????
        uint16_t atu11 : 4; /** Bits 15-12 */
        uint16_t cmt0_ad0 : 4; /** Bits 11-8 */
        uint16_t cmt1_ad1 : 4; /** Bits 7-4 */
        uint16_t ad2 : 4; /** Bits 3-0 */
#else
        uint16_t ad2 : 4; /** Bits 3-0 */
        uint16_t cmt1_ad1 : 4; /** Bits 7-4 */
        uint16_t cmt0_ad0 : 4; /** Bits 11-8 */
        uint16_t atu11 : 4; /** Bits 15-12 */
#endif
    };
} sh2e_intc_iprj_t;

typedef union sh2e_intc_iprk {
    uint16_t value;

    PACKED struct {
#ifdef WORDS_BIGENDIAN
        uint16_t sci0 : 4; /** Bits 15-12 */
        uint16_t sci1 : 4; /** Bits 11-8 */
        uint16_t sci2 : 4; /** Bits 7-4 */
        uint16_t sci3 : 4; /** Bits 3-0 */
#else
        uint16_t sci3 : 4; /** Bits 3-0 */
        uint16_t sci2 : 4; /** Bits 7-4 */
        uint16_t sci1 : 4; /** Bits 11-8 */
        uint16_t sci0 : 4; /** Bits 15-12 */
#endif
    };
} sh2e_intc_iprk_t;


typedef union sh2e_intc_iprl {
    uint16_t value;

    PACKED struct {
#ifdef WORDS_BIGENDIAN
        uint16_t sci4 : 4; /** Bits 15-12 */
        uint16_t hcan0 : 4; /** Bits 11-8 */
        uint16_t wdt : 4; /** Bits 7-4 */
        uint16_t hcan1 : 4; /** Bits 3-0 */
#else
        uint16_t hcan1 : 4; /** Bits 3-0 */
        uint16_t wdt : 4; /** Bits 7-4 */
        uint16_t hcan0 : 4; /** Bits 11-8 */
        uint16_t sci4 : 4; /** Bits 15-12 */
#endif
    };
} sh2e_intc_iprl_t;

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

typedef enum {
    sh2e_intc_ipra = 0,
    sh2e_intc_iprb = 1,
    sh2e_intc_iprc = 2,
    sh2e_intc_iprd = 3,
    sh2e_intc_ipre = 4,
    sh2e_intc_iprf = 5,
    sh2e_intc_iprg = 6,
    sh2e_intc_iprh = 7,
    sh2e_intc_ipri = 8,
    sh2e_intc_iprj = 9,
    sh2e_intc_iprk = 10,
    sh2e_intc_iprl = 11,
} sh2e_ipr_index_t;

typedef struct {
    sh2e_intc_ipra_t ipra;
    sh2e_intc_iprb_t iprb;
    sh2e_intc_iprc_t iprc;
    sh2e_intc_iprd_t iprd;
    sh2e_intc_ipre_t ipre;
    sh2e_intc_iprf_t iprf;
    sh2e_intc_iprg_t iprg;
    sh2e_intc_iprh_t iprh;
    sh2e_intc_ipri_t ipri;
    sh2e_intc_iprj_t iprj;
    sh2e_intc_iprk_t iprk;
    sh2e_intc_iprl_t iprl;
} sh2e_intc_priority_regs_t;

typedef struct sh2e_intc_regs {

    union {
        uint16_t priority[INTC_IPR_REGISTERS_COUNT];

        PACKED sh2e_intc_priority_regs_t priority_regs;
    };

    union {
        uint16_t system[INTC_SYSTEM_REGISTERS_COUNT];

        PACKED struct {
            /** Interrupt Control Register (ICR) */
            sh2e_intc_icr_t icr;

            /** IRQ Status Register (ISR) */
            sh2e_intc_isr_t isr;
        };
    };


} sh2e_intc_regs_t;

_Static_assert(sizeof(sh2e_intc_regs_t) == sizeof(uint16_t) * (INTC_IPR_REGISTERS_COUNT + INTC_SYSTEM_REGISTERS_COUNT), "invalid sh2e_intc_regs_t size!");

typedef struct sh2e_intc {

    sh2e_intc_regs_t * intc_regs;

    uint16_t nmi_prev_value; /** Previous value of NMI pin */

    /** Interrupts table */
    bool interrupts_table[INTC_SOURCE_MAX_VALUE + 1];


    /** Internal use */
    // sh2e_intc_source_t to_be_accepted_interrupt_source; /** Interrupt source that is to be accepted by the CPU */
    uint16_t to_be_accepted_priority; /** Priority of the interrupt source that is to be accepted by the CPU */

} sh2e_intc_t;

extern void sh2e_intc_init(sh2e_cpu_t * cpu);

extern sh2e_intc_source_t sh2e_check_pending_interrupts(sh2e_cpu_t * cpu);

extern void sh2e_accept_interrupt(sh2e_cpu_t * cpu, sh2e_intc_source_t interrupt_source);

#endif // SUPERH_SH2E_INTC_H_
