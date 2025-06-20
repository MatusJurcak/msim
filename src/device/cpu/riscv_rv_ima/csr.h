/*
 * Copyright (c) 2022 Jan Papesch
 * Copyright (c) 2025 Martin Rosenberg
 * All rights reserved.
 *
 * Distributed under the terms of GPL.
 *
 *
 *  RISC-V CSR
 *
 */

#ifndef RISCV_RV_CSR_H_
#define RISCV_RV_CSR_H_

#include <stdbool.h>
#include <stdint.h>

#include "exception.h"
#include "types.h"

/**
 * The numerical codes of CSRs
 */
typedef enum {

    /********************************
     * Unprivileged Counters/Timers *
     ********************************/

    csr_cycle = 0xC00, // Cycle counter for RCCYCLE
    csr_time = 0xC01, // Timer (real time) for RDTIME
    csr_instret = 0xC02, // Instruction-retired counter for RDINSTRET

    /* Architecture-Defined Performance-Monitoring Counters (lower 32 bits) */
    csr_hpmcounter3 = 0xC03,
    csr_hpmcounter4 = 0xC04,
    csr_hpmcounter5 = 0xC05,
    csr_hpmcounter6 = 0xC06,
    csr_hpmcounter7 = 0xC07,
    csr_hpmcounter8 = 0xC08,
    csr_hpmcounter9 = 0xC09,
    csr_hpmcounter10 = 0xC0A,
    csr_hpmcounter11 = 0xC0B,
    csr_hpmcounter12 = 0xC0C,
    csr_hpmcounter13 = 0xC0D,
    csr_hpmcounter14 = 0xC0E,
    csr_hpmcounter15 = 0xC0F,
    csr_hpmcounter16 = 0xC10,
    csr_hpmcounter17 = 0xC11,
    csr_hpmcounter18 = 0xC12,
    csr_hpmcounter19 = 0xC13,
    csr_hpmcounter20 = 0xC14,
    csr_hpmcounter21 = 0xC15,
    csr_hpmcounter22 = 0xC16,
    csr_hpmcounter23 = 0xC17,
    csr_hpmcounter24 = 0xC18,
    csr_hpmcounter25 = 0xC19,
    csr_hpmcounter26 = 0xC1A,
    csr_hpmcounter27 = 0xC1B,
    csr_hpmcounter28 = 0xC1C,
    csr_hpmcounter29 = 0xC1D,
    csr_hpmcounter30 = 0xC1E,
    csr_hpmcounter31 = 0xC1F,

    /* Upper 32 bits of timers/counters */
    csr_cycleh = 0xC80,
    csr_timeh = 0xC81,
    csr_instreth = 0xC82,
    csr_hpmcounter3h = 0xC83,
    csr_hpmcounter4h = 0xC84,
    csr_hpmcounter5h = 0xC85,
    csr_hpmcounter6h = 0xC86,
    csr_hpmcounter7h = 0xC87,
    csr_hpmcounter8h = 0xC88,
    csr_hpmcounter9h = 0xC89,
    csr_hpmcounter10h = 0xC8A,
    csr_hpmcounter11h = 0xC8B,
    csr_hpmcounter12h = 0xC8C,
    csr_hpmcounter13h = 0xC8D,
    csr_hpmcounter14h = 0xC8E,
    csr_hpmcounter15h = 0xC8F,
    csr_hpmcounter16h = 0xC90,
    csr_hpmcounter17h = 0xC91,
    csr_hpmcounter18h = 0xC92,
    csr_hpmcounter19h = 0xC93,
    csr_hpmcounter20h = 0xC94,
    csr_hpmcounter21h = 0xC95,
    csr_hpmcounter22h = 0xC96,
    csr_hpmcounter23h = 0xC97,
    csr_hpmcounter24h = 0xC98,
    csr_hpmcounter25h = 0xC99,
    csr_hpmcounter26h = 0xC9A,
    csr_hpmcounter27h = 0xC9B,
    csr_hpmcounter28h = 0xC9C,
    csr_hpmcounter29h = 0xC9D,
    csr_hpmcounter30h = 0xC9E,
    csr_hpmcounter31h = 0xC9F,

    /*************************
     * Supervisor level CSRs *
     *************************/

    /* Trap Setup */
    csr_sstatus = 0x100, // Supervisor status register
    csr_sie = 0x104, // Sup. interrupt-enable reg.
    csr_stvec = 0x105, // Sup. trap handler base address
    csr_scounteren = 0x106, // Sup. counter enable

    /* Configuration */
    csr_senvcfg = 0x10A, // Sup. environment configuration reg.

    /* Trap Handling */
    csr_sscratch = 0x140, // Scratch reg. for sup. tram handlers
    csr_sepc = 0x141, // Sup. exception program counter
    csr_scause = 0x142, // Sup. trap cause
    csr_stval = 0x143, // Sup. bad address or instruction
    csr_sip = 0x144, // Sup. interrupt pending

    /* Address Translation and Protection */
    csr_satp = 0x180, // Sup. addr. translation and protection

    /* Debug/Trace */
    csr_scontext = 0x5A8, // Supervisor-mode context reg.

    /* Custom */
    csr_scyclecmp = 0x5C0, // Supervisor Cycle compare (r/w)

    /**********************
     * Machine level CSRs *
     **********************/

    /* Machine information */
    csr_mvendorid = 0xF11, // Vendor id
    csr_marchid = 0xF12, // Architecture id
    csr_mimpid = 0xF13, // Implementation id
    csr_mhartid = 0xF14, // Hardware thread id (procno)
    csr_mconfigptr = 0xF15, // Pointer to configuration data structure

    /* Trap Setup */
    csr_mstatus = 0x300, // Machine status register
    csr_misa = 0x301, // ISA and extensions
    csr_medeleg = 0x302, // Mch. exception delegation reg.
    csr_mideleg = 0x303, // Mch. interrupt delegation reg.
    csr_mie = 0x304, // Mch. interrupt-enable reg.
    csr_mtvec = 0x305, // Mch. trap-handler base addr.
    csr_mcounteren = 0x306, // Mch. counter enable
    csr_mstatush = 0x310, // Additional mch. status register

    /* Trap Handling */
    csr_mscratch = 0x340, // Scratch register for mch. trap handlers
    csr_mepc = 0x341, // Mch. exception pc
    csr_mcause = 0x342, // Mch. trap cause
    csr_mtval = 0x343, // Mch. bad addr. or instr.
    csr_mip = 0x344, // Mch. interrupt pending
    csr_mtinst = 0x34A, // Mch. trap instruction (transformed)
    csr_mtval2 = 0x34B, // Mch. bad guest physical addr.

    /* Machine Configuration */
    csr_menvcfg = 0x30A, // Mch. environment conf. reg.
    csr_menvcfgh = 0x31A, // Additional mch. env. conf. reg.
    csr_mseccfg = 0x747, // Mch. security conf. reg.
    csr_mseccfgh = 0x757, // Additional mch. security conf. reg.

    /* Memory Protection */

    /* Phys. mem. protection configuration */
    csr_pmpcfg0 = 0x3A0,
    csr_pmpcfg1 = 0x3A1,
    csr_pmpcfg2 = 0x3A2,
    csr_pmpcfg3 = 0x3A3,
    csr_pmpcfg4 = 0x3A4,
    csr_pmpcfg5 = 0x3A5,
    csr_pmpcfg6 = 0x3A6,
    csr_pmpcfg7 = 0x3A7,
    csr_pmpcfg8 = 0x3A8,
    csr_pmpcfg9 = 0x3A9,
    csr_pmpcfg10 = 0x3AA,
    csr_pmpcfg11 = 0x3AB,
    csr_pmpcfg12 = 0x3AC,
    csr_pmpcfg13 = 0x3AD,
    csr_pmpcfg14 = 0x3AE,
    csr_pmpcfg15 = 0x3AF,

    /* Phys. mem. protection address */
    csr_pmpaddr0 = 0x3B0,
    csr_pmpaddr1 = 0x3B1,
    csr_pmpaddr2 = 0x3B2,
    csr_pmpaddr3 = 0x3B3,
    csr_pmpaddr4 = 0x3B4,
    csr_pmpaddr5 = 0x3B5,
    csr_pmpaddr6 = 0x3B6,
    csr_pmpaddr7 = 0x3B7,
    csr_pmpaddr8 = 0x3B8,
    csr_pmpaddr9 = 0x3B9,
    csr_pmpaddr10 = 0x3BA,
    csr_pmpaddr11 = 0x3BB,
    csr_pmpaddr12 = 0x3BC,
    csr_pmpaddr13 = 0x3BD,
    csr_pmpaddr14 = 0x3BE,
    csr_pmpaddr15 = 0x3BF,
    csr_pmpaddr16 = 0x3C0,
    csr_pmpaddr17 = 0x3C1,
    csr_pmpaddr18 = 0x3C2,
    csr_pmpaddr19 = 0x3C3,
    csr_pmpaddr20 = 0x3C4,
    csr_pmpaddr21 = 0x3C5,
    csr_pmpaddr22 = 0x3C6,
    csr_pmpaddr23 = 0x3C7,
    csr_pmpaddr24 = 0x3C8,
    csr_pmpaddr25 = 0x3C9,
    csr_pmpaddr26 = 0x3CA,
    csr_pmpaddr27 = 0x3CB,
    csr_pmpaddr28 = 0x3CC,
    csr_pmpaddr29 = 0x3CD,
    csr_pmpaddr30 = 0x3CE,
    csr_pmpaddr31 = 0x3CF,
    csr_pmpaddr32 = 0x3D0,
    csr_pmpaddr33 = 0x3D1,
    csr_pmpaddr34 = 0x3D2,
    csr_pmpaddr35 = 0x3D3,
    csr_pmpaddr36 = 0x3D4,
    csr_pmpaddr37 = 0x3D5,
    csr_pmpaddr38 = 0x3D6,
    csr_pmpaddr39 = 0x3D7,
    csr_pmpaddr40 = 0x3D8,
    csr_pmpaddr41 = 0x3D9,
    csr_pmpaddr42 = 0x3DA,
    csr_pmpaddr43 = 0x3DB,
    csr_pmpaddr44 = 0x3DC,
    csr_pmpaddr45 = 0x3DD,
    csr_pmpaddr46 = 0x3DE,
    csr_pmpaddr47 = 0x3DF,
    csr_pmpaddr48 = 0x3E0,
    csr_pmpaddr49 = 0x3E1,
    csr_pmpaddr50 = 0x3E2,
    csr_pmpaddr51 = 0x3E3,
    csr_pmpaddr52 = 0x3E4,
    csr_pmpaddr53 = 0x3E5,
    csr_pmpaddr54 = 0x3E6,
    csr_pmpaddr55 = 0x3E7,
    csr_pmpaddr56 = 0x3E8,
    csr_pmpaddr57 = 0x3E9,
    csr_pmpaddr58 = 0x3EA,
    csr_pmpaddr59 = 0x3EB,
    csr_pmpaddr60 = 0x3EC,
    csr_pmpaddr61 = 0x3ED,
    csr_pmpaddr62 = 0x3EE,
    csr_pmpaddr63 = 0x3EF,

    /* Machine Counters/Timers */

    csr_mcycle = 0xB00, // Machine cycle counter
    csr_minstret = 0xB02, // Machine intructions retired counter

    /* Machine performance-monitoring counters (lower 32 bits) */
    csr_mhpmcounter3 = 0xB03,
    csr_mhpmcounter4 = 0xB04,
    csr_mhpmcounter5 = 0xB05,
    csr_mhpmcounter6 = 0xB06,
    csr_mhpmcounter7 = 0xB07,
    csr_mhpmcounter8 = 0xB08,
    csr_mhpmcounter9 = 0xB09,
    csr_mhpmcounter10 = 0xB0A,
    csr_mhpmcounter11 = 0xB0B,
    csr_mhpmcounter12 = 0xB0C,
    csr_mhpmcounter13 = 0xB0D,
    csr_mhpmcounter14 = 0xB0E,
    csr_mhpmcounter15 = 0xB0F,
    csr_mhpmcounter16 = 0xB10,
    csr_mhpmcounter17 = 0xB11,
    csr_mhpmcounter18 = 0xB12,
    csr_mhpmcounter19 = 0xB13,
    csr_mhpmcounter20 = 0xB14,
    csr_mhpmcounter21 = 0xB15,
    csr_mhpmcounter22 = 0xB16,
    csr_mhpmcounter23 = 0xB17,
    csr_mhpmcounter24 = 0xB18,
    csr_mhpmcounter25 = 0xB19,
    csr_mhpmcounter26 = 0xB1A,
    csr_mhpmcounter27 = 0xB1B,
    csr_mhpmcounter28 = 0xB1C,
    csr_mhpmcounter29 = 0xB1D,
    csr_mhpmcounter30 = 0xB1E,
    csr_mhpmcounter31 = 0xB1F,

    /* Upper 32-bit of machine counters */
    csr_mcycleh = 0xB80,
    csr_minstreth = 0xB82,
    csr_mhpmcounter3h = 0xB83,
    csr_mhpmcounter4h = 0xB84,
    csr_mhpmcounter5h = 0xB85,
    csr_mhpmcounter6h = 0xB86,
    csr_mhpmcounter7h = 0xB87,
    csr_mhpmcounter8h = 0xB88,
    csr_mhpmcounter9h = 0xB89,
    csr_mhpmcounter10h = 0xB8A,
    csr_mhpmcounter11h = 0xB8B,
    csr_mhpmcounter12h = 0xB8C,
    csr_mhpmcounter13h = 0xB8D,
    csr_mhpmcounter14h = 0xB8E,
    csr_mhpmcounter15h = 0xB8F,
    csr_mhpmcounter16h = 0xB90,
    csr_mhpmcounter17h = 0xB91,
    csr_mhpmcounter18h = 0xB92,
    csr_mhpmcounter19h = 0xB93,
    csr_mhpmcounter20h = 0xB94,
    csr_mhpmcounter21h = 0xB95,
    csr_mhpmcounter22h = 0xB96,
    csr_mhpmcounter23h = 0xB97,
    csr_mhpmcounter24h = 0xB98,
    csr_mhpmcounter25h = 0xB99,
    csr_mhpmcounter26h = 0xB9A,
    csr_mhpmcounter27h = 0xB9B,
    csr_mhpmcounter28h = 0xB9C,
    csr_mhpmcounter29h = 0xB9D,
    csr_mhpmcounter30h = 0xB9E,
    csr_mhpmcounter31h = 0xB9F,

    /* Counter Setup */
    csr_mcountinhibit = 0x320, // Mch. counter-inhibit reg.

    /* Machine performance-monitoring event selectors */
    csr_mhpmevent3 = 0x323,
    csr_mhpmevent4 = 0x324,
    csr_mhpmevent5 = 0x325,
    csr_mhpmevent6 = 0x326,
    csr_mhpmevent7 = 0x327,
    csr_mhpmevent8 = 0x328,
    csr_mhpmevent9 = 0x329,
    csr_mhpmevent10 = 0x32A,
    csr_mhpmevent11 = 0x32B,
    csr_mhpmevent12 = 0x32C,
    csr_mhpmevent13 = 0x32D,
    csr_mhpmevent14 = 0x32E,
    csr_mhpmevent15 = 0x32F,
    csr_mhpmevent16 = 0x330,
    csr_mhpmevent17 = 0x331,
    csr_mhpmevent18 = 0x332,
    csr_mhpmevent19 = 0x333,
    csr_mhpmevent20 = 0x334,
    csr_mhpmevent21 = 0x335,
    csr_mhpmevent22 = 0x336,
    csr_mhpmevent23 = 0x337,
    csr_mhpmevent24 = 0x338,
    csr_mhpmevent25 = 0x339,
    csr_mhpmevent26 = 0x33A,
    csr_mhpmevent27 = 0x33B,
    csr_mhpmevent28 = 0x33C,
    csr_mhpmevent29 = 0x33D,
    csr_mhpmevent30 = 0x33E,
    csr_mhpmevent31 = 0x33F,

    /* Debug/Trace */
    csr_tselect = 0x7A0, // Debug/Trace trigger register select
    csr_tdata1 = 0x7A1, // First Debug/Trace trigger data register
    csr_tdata2 = 0x7A2, // Second Debug/Trace trigger data register
    csr_tdata3 = 0x7A3, // Third Debug/Trace trigger data register
    csr_mcontext = 0x7A8, // Machine-mode context reg.

    /* Debug Mode */
    csr_dcsr = 0x7B0, // Dbg. control and status reg.
    csr_dpc = 0x7B1, // Dbg. pc
    csr_dscratch0 = 0x7B2, // Dbg. scratch reg. 0
    csr_dscratch1 = 0x7B3 // Dbg. scratch reg. 1

} csr_num_t;

/**
 * Types of HPM events
 */
typedef enum {
    hpm_no_event,
    hpm_u_cycles,
    hpm_s_cycles,
    hpm_r_cycles, //! RESERVED
    hpm_m_cycles,
    hpm_w_cycles,
    // rest TBA
    hpm_event_count // Last enum member holding the count
} rv_csr_hpm_event_t;

/**
 * Structure holding CSR data
 */
typedef struct {
    /* Counters/Timers */
    uint64_t cycle;
    uint64_t instret;

    uint64_t hpmcounters[29];

    /* Event selectors */
    uxlen_t hpmevents[29];

    /* Machine-level registers */

    /* information */
    uxlen_t misa;
    uxlen_t mvendorid;
    uxlen_t marchid;
    uxlen_t mimpid;
    uxlen_t mhartid;
    uxlen_t mconfigptr;

    uint64_t mstatus; // subset shared with supervisor level

    /* trap handling */
    uxlen_t mtvec;
    uxlen_t medeleg;
    uxlen_t mideleg;
    uxlen_t mip;
    uxlen_t mie;
    uxlen_t mscratch;
    uxlen_t mepc;
    uxlen_t mcause;
    uxlen_t mtval;

    /* counter control*/
    uxlen_t mcounteren;
    uxlen_t mcountinhibit;

    /* configs */
    uint64_t menvcfg;
    uxlen_t mseccfg;

    /* debug/trace */
    uxlen_t mcontext;

    /* physical memory protection */
    uint8_t pmpcfgs[64];
    uxlen_t pmpaddrs[64];

    /* Supervisor level registers*/

    // sstatus shared witm m-mode
    uxlen_t stvec;
    // sip and sie shared with m-mode

    uxlen_t scounteren;

    /* Trap handling */
    uxlen_t sscratch;
    uxlen_t sepc;
    uxlen_t scause;
    uxlen_t stval;

    /* config */
    uxlen_t senvcfg;

    /* Address translation */
    uxlen_t satp;

    /* Debug */
    uxlen_t scontext;

    /* Extra fields used for implementation purposes */

    // next value for stval/mtval
    uxlen_t tval_next;

    // Keeps whether SEI is pending from an external source
    // Full explanation RISC-V Privileged spec section 3.1.9 Machine Interrupt Registers (mip and mie)
    bool external_SEIP;

    // Value of memory-mapped register mtime
    uint64_t mtime;
    // The timestamp of the last clock cycle
    uint64_t last_tick_time;
    // Value of memory-mapped register mtimecmp
    uint64_t mtimecmp;

    // Supervisor cycle compare used for STI
    //? is XLEN=32 for rv32 enough?
    uxlen_t scyclecmp;
    bool external_STIP;

    // Number of bits used in the ASID field of SATP CSR - Should be between 0 and 9.
    unsigned asid_len;

} rv_csr_t;

#define RV_START_ADDRESS XLEN_C(0xF0000000)
#define RV_MTIME_ADDRESS XLEN_C(0xFF000000)
#define RV_MTIMECMP_ADDRESS XLEN_C(0xFF000008)

#define RV_A_EXTENSION_BITS XLEN_C(1 << 0)
#define RV_C_EXTENSION_BITS XLEN_C(1 << 2)
#define RV_D_EXTENSION_BITS XLEN_C(1 << 3)
#define RV_E_EXTENSION_BITS XLEN_C(1 << 4)
#define RV_F_EXTENSION_BITS XLEN_C(1 << 5)
#define RV_H_EXTENSION_BITS XLEN_C(1 << 7)
#define RV_I_EXTENSION_BITS XLEN_C(1 << 8)
#define RV_M_EXTENSION_BITS XLEN_C(1 << 12)
#define RV_Q_EXTENSION_BITS XLEN_C(1 << 16)
#define RV_S_IMPLEMENTED_BITS XLEN_C(1 << 18)
#define RV_U_IMPLEMENTED_BITS XLEN_C(1 << 20)

#define RV_32_MXLEN_BITS XLEN_C(0x40000000)
#define RV_64_MXLEN_BITS XLEN_C(0x80000000)
#define RV_128_MXLEN_BITS XLEN_C(0xC0000000)

#define RV_32_XLEN_2_BITS 0b01

#if XLEN == 32
#define RV_ISA RV_32_MXLEN_BITS | RV_I_EXTENSION_BITS | RV_M_EXTENSION_BITS | RV_A_EXTENSION_BITS | RV_U_IMPLEMENTED_BITS | RV_S_IMPLEMENTED_BITS
#else
#define RV_ISA RV_64_MXLEN_BITS | RV_I_EXTENSION_BITS | RV_M_EXTENSION_BITS | RV_A_EXTENSION_BITS | RV_U_IMPLEMENTED_BITS | RV_S_IMPLEMENTED_BITS
#endif

#define RV_VENDOR_ID 0
#define RV_ARCH_ID 0
#define RV_IMPLEMENTATION_ID 0

#define rv_csr_mstatus_tsr_mask (XLEN_C(1) << 22)
#define rv_csr_mstatus_tw_mask (XLEN_C(1) << 21)
#define rv_csr_mstatus_tvm_mask (XLEN_C(1) << 20)
#define rv_csr_mstatus_mprv_mask (XLEN_C(1) << 17)
#define rv_csr_mstatus_mpp_mask (XLEN_C(3) << 11)
#define rv_csr_mstatus_mpie_mask (XLEN_C(1) << 7)
#define rv_csr_mstatus_mie_mask (XLEN_C(1) << 3)

// These are RV64 specific
#define rv_csr_mstatus_mbe_mask (XLEN_C(1) << 37)
#define rv_csr_mstatus_sbe_mask (XLEN_C(1) << 36)

// These are RV32 specific
#define rv_csr_mstatush_sbe_mask (UINT32_C(1) << 4)
#define rv_csr_mstatush_mbe_mask (UINT32_C(1) << 5)

#define rv_csr_mstatus_mpp_pos 11

#define rv_csr_mstatus_tsr(cpu) (bool) ((cpu)->csr.mstatus & rv_csr_mstatus_tsr_mask)
#define rv_csr_mstatus_tw(cpu) (bool) ((cpu)->csr.mstatus & rv_csr_mstatus_tw_mask)
#define rv_csr_mstatus_tvm(cpu) (bool) ((cpu)->csr.mstatus & rv_csr_mstatus_tvm_mask)
#define rv_csr_mstatus_mprv(cpu) (bool) ((cpu)->csr.mstatus & rv_csr_mstatus_mprv_mask)
#define rv_csr_mstatus_mpie(cpu) (bool) ((cpu)->csr.mstatus & rv_csr_mstatus_mpie_mask)
#define rv_csr_mstatus_mie(cpu) (bool) ((cpu)->csr.mstatus & rv_csr_mstatus_mie_mask)
#define rv_csr_mstatus_mpp(cpu) (enum rv_priv_mode)(((cpu)->csr.mstatus & rv_csr_mstatus_mpp_mask) >> rv_csr_mstatus_mpp_pos)
#define rv_csr_mstatus_mbe(cpu) (bool) ((cpu)->csr.mstatus & rv_csr_mstatus_mbe_mask)
#define rv_csr_mstatus_sbe(cpu) (bool) ((cpu)->csr.mstatus & rv_csr_mstatus_sbe_mask)

#define rv_csr_sstatus_mxr_mask (XLEN_C(1) << 19)
#define rv_csr_sstatus_sum_mask (XLEN_C(1) << 18)
#define rv_csr_sstatus_spp_mask (XLEN_C(1) << 8)
#define rv_csr_sstatus_ube_mask (XLEN_C(1) << 6)
#define rv_csr_sstatus_spie_mask (XLEN_C(1) << 5)
#define rv_csr_sstatus_sie_mask (XLEN_C(1) << 1)

#define rv_csr_sstatus_spp_pos 8

// Doesn't include UBE, because riscv in msim is strictly Little Endian
// Also does not include SD, XS, FS abd VS, because these are used for extensions that are not implemented
#define rv_csr_sstatus_mask (rv_csr_sstatus_mxr_mask | rv_csr_sstatus_sum_mask | rv_csr_sstatus_spp_mask | rv_csr_sstatus_spie_mask | rv_csr_sstatus_sie_mask)
#define rv_csr_mstatus_mask (rv_csr_sstatus_mask | rv_csr_mstatus_tsr_mask | rv_csr_mstatus_tw_mask | rv_csr_mstatus_tvm_mask | rv_csr_mstatus_mprv_mask | rv_csr_mstatus_mpp_mask | rv_csr_mstatus_mpie_mask | rv_csr_mstatus_mie_mask)

#define rv_csr_sstatus_mxr(cpu) (bool) ((cpu)->csr.mstatus & rv_csr_sstatus_mxr_mask)
#define rv_csr_sstatus_sum(cpu) (bool) ((cpu)->csr.mstatus & rv_csr_sstatus_sum_mask)
#define rv_csr_sstatus_spie(cpu) (bool) ((cpu)->csr.mstatus & rv_csr_sstatus_spie_mask)
#define rv_csr_sstatus_sie(cpu) (bool) ((cpu)->csr.mstatus & rv_csr_sstatus_sie_mask)
#define rv_csr_sstatus_spp(cpu) (enum rv_priv_mode)(((cpu)->csr.mstatus & rv_csr_sstatus_spp_mask) >> (rv_csr_sstatus_spp_pos))
#define rv_csr_sstatus_ube(cpu) (bool) ((cpu)->csr.mstatus & rv_csr_sstatus_ube_mask)

#define rv_csr_sei_mask (1U << 9)
#define rv_csr_sti_mask (1U << 5)
#define rv_csr_ssi_mask (1U << 1)
#define rv_csr_mei_mask (1U << 11)
#define rv_csr_mti_mask (1U << 7)
#define rv_csr_msi_mask (1U << 3)
#define rv_csr_si_mask (rv_csr_sei_mask | rv_csr_sti_mask | rv_csr_ssi_mask)
#define rv_csr_mi_mask (rv_csr_si_mask | rv_csr_mei_mask | rv_csr_mti_mask | rv_csr_msi_mask)

#define rv_csr_mtvec_mode_mask XLEN_C(0b11)
#define rv_csr_mtvec_mode_direct XLEN_C(0)
#define rv_csr_mtvec_mode_vectored XLEN_C(1)

#if XLEN == 32

#define rv_csr_satp_mode_mask 0x80000000
#define rv_csr_asid_mask 0x7FC00000
#define rv_csr_satp_ppn_mask 0x003FFFFF

#define rv_csr_satp_asid_offset 22
#define rv_csr_satp_asid(cpu) (((cpu)->csr.satp & rv_csr_asid_mask) >> 22)
#define rv_asid_len 9

#elif XLEN == 64

#define rv_csr_satp_mode_mask (UINT64_C(8) << 60)
#define rv_csr_asid_mask 0xFFFF00000000000
#define rv_csr_satp_ppn_mask 0xFFFFFFFFFFF

#define rv_csr_satp_asid_offset 44
#define rv_csr_satp_asid(cpu) (((cpu)->csr.satp & rv_csr_asid_mask) >> rv_csr_satp_asid_offset)
#define rv_asid_len 16

#else

#error "XLEN must be either 32 or 64"

#endif

#define rv_csr_satp_is_bare(cpu) (~(cpu)->csr.satp & rv_csr_satp_mode_mask)
#define rv_csr_satp_ppn(cpu) ((cpu)->csr.satp & rv_csr_satp_ppn_mask)

#define sv39_effective_priv(cpu) (rv_csr_mstatus_mprv(cpu) ? rv_csr_mstatus_mpp(cpu) : cpu->priv_mode)
#define rv_effective_priv(cpu) (rv_csr_satp_is_bare(cpu) ? cpu->priv_mode : sv39_effective_priv(cpu))

#define sv32_effective_priv(cpu) (rv_csr_mstatus_mprv(cpu) ? rv_csr_mstatus_mpp(cpu) : cpu->priv_mode)
#define rv32_effective_priv(cpu) (rv_csr_satp_is_bare(cpu) ? cpu->priv_mode : sv32_effective_priv(cpu))

#define rv_asid_mask ((1 << rv_asid_len) - 1)

#define rv_csr_is_read_only(csr) (((csr) >> 30) == 0b11)

#endif // RISCV_RV_CSR_H_
