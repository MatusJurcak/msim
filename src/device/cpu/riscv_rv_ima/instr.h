/*
 * Copyright (c) 2022 Jan Papesch
 * Copyright (c) 2025 Martin Rosenberg
 * All rights reserved.
 *
 * Distributed under the terms of GPL.
 *
 *
 *  RISC-V Instructions
 *
 */

#ifndef RISCV_RV_INSTR_H_
#define RISCV_RV_INSTR_H_

#include <assert.h>
#include <stdint.h>

#include "../../../utils.h"
#include "exception.h"
#include "types.h"

typedef union {
    uint32_t val;
    struct {
        unsigned int opcode : 7;
        unsigned int rd : 5;
        unsigned int funct3 : 3;
        unsigned int rs1 : 5;
        unsigned int rs2 : 5;
        unsigned int funct7 : 7;
    } r;
    struct {
        unsigned int opcode : 7;
        unsigned int rd : 5;
        unsigned int funct3 : 3;
        unsigned int rs1 : 5;
        int imm : 12;
    } i;
    struct {
        unsigned int opcode : 7;
        unsigned int imm4_0 : 5;
        unsigned int funct3 : 3;
        unsigned int rs1 : 5;
        unsigned int rs2 : 5;
        int imm11_5 : 7;
    } s;
    struct {
        unsigned int opcode : 7;
        unsigned int imm11 : 1;
        unsigned int imm4_1 : 4;
        unsigned int funct3 : 3;
        unsigned int rs1 : 5;
        unsigned int rs2 : 5;
        unsigned int imm10_5 : 6;
        int imm12 : 1;
    } b;
    struct {
        unsigned int opcode : 7;
        unsigned int rd : 5;
        unsigned int imm : 20; // does not need to be sign extended
    } u;
    struct {
        unsigned int opcode : 7;
        unsigned int rd : 5;
        unsigned int imm19_12 : 8;
        unsigned int imm11 : 1;
        unsigned int imm10_1 : 10;
        int imm20 : 1;
    } j;
} rv_instr_t;

static_assert(sizeof(rv_instr_t) == 4, "rv_instr_t has wrong size");

#define RV_S_IMM(instr) (uxlen_t)((((xlen_t) instr.s.imm11_5) << 5) | ((0x1F) & instr.s.imm4_0))
#define RV_R_FUNCT(instr) (uxlen_t)(((uxlen_t) (instr.r.funct7) << 3) | (0x7 & instr.r.funct3))
#define RV_I_UNSIGNED_IMM(instr) (uxlen_t)(instr.i.imm & 0xFFF)
#define RV_J_IMM(instr) (uxlen_t)((((xlen_t) instr.j.imm20) << 20) | (instr.j.imm19_12 << 12) | (instr.j.imm11 << 11) | (instr.j.imm10_1 << 1))
#define RV_B_IMM(instr) (uxlen_t)((((xlen_t) instr.b.imm12) << 12) | (instr.b.imm11 << 11) | (instr.b.imm10_5 << 5) | (instr.b.imm4_1 << 1))
#define RV_AMO_FUNCT(instr) (instr.r.funct7 >> 2)

/** Opcodes*/
typedef enum {
    rv_opcLOAD = 0b0000011,
    rv_opcLOAD_FP = 0b0000111, // not supported
    rv_opcMISC_MEM = 0b0001111,
    rv_opcOP_IMM = 0b0010011,
    rv_opcAUIPC = 0b0010111,
    rv_opcSTORE = 0b0100011,
    rv_opcSTORE_FP = 0b0100111, // not supported
    rv_opcAMO = 0b0101111,
    rv_opcOP = 0b0110011,
    rv_opcLUI = 0b0110111,
    rv_opcOP_32 = 0b0111011,
    rv_opcOP_IMM_32 = 0b0011011,
    rv_opcMADD = 0b1000011, // not supported
    rv_opcMSUB = 0b1000111, // not supported
    rv_opcNMSUB = 0b1001011, // not supported
    rv_opcNMADD = 0b1001111, // not supported
    rv_opcOP_FP = 0b1010011, // not supported
    rv_opcBRANCH = 0b1100011,
    rv_opcJALR = 0b1100111,
    rv_opcJAL = 0b1101111,
    rv_opcSYSTEM = 0b1110011
} rv_opcode_t;

/** Funct values for OP instructions */
typedef enum {
    rv_func_ADD = 0b0000000000,
    rv_func_SUB = 0b0100000000,
    rv_func_SLL = 0b0000000001,
    rv_func_SLT = 0b0000000010,
    rv_func_SLTU = 0b0000000011,
    rv_func_XOR = 0b0000000100,
    rv_func_SRL = 0b0000000101,
    rv_func_SRA = 0b0100000101,
    rv_func_OR = 0b0000000110,
    rv_func_AND = 0b0000000111,
    /* M extension */
    rv_func_MUL = 0b0000001000,
    rv_func_MULH = 0b0000001001,
    rv_func_MULHSU = 0b0000001010,
    rv_func_MULHU = 0b0000001011,
    rv_func_DIV = 0b0000001100,
    rv_func_DIVU = 0b0000001101,
    rv_func_REM = 0b0000001110,
    rv_func_REMU = 0b0000001111
} rv_op_func_t;

/** Funct values for OP-imm instructions */
typedef enum {
    rv_func_ADDI = 0b000,
    rv_func_SLTI = 0b010,
    rv_func_SLTIU = 0b011,
    rv_func_XORI = 0b100,
    rv_func_ORI = 0b110,
    rv_func_ANDI = 0b111,
    rv_func_SLLI = 0b001,
    rv_func_SRI = 0b101 // same for SRLI and SRAI
} rv_op_imm_func_t;

/** Values of the top 7 bits of imm field determining the right shift type */
typedef enum {
    rv_SRAI = 0b0100000,
    rv_SRLI = 0b0000000,
} rv_op_imm_right_shift_type_t;

typedef enum {
    rv64_SRAI = 0b0100000,
    rv64_SRLI = 0b0000000,
} rv64_op_imm_right_shift_type_t;

/** Funct values for BRANCH instructions */
typedef enum {
    rv_func_BEQ = 0b000,
    rv_func_BNE = 0b001,
    rv_func_BLT = 0b100,
    rv_func_BLTU = 0b110,
    rv_func_BGE = 0b101,
    rv_func_BGEU = 0b111
} rv_branch_func_t;

/** Funct values for LOAD instructions */
typedef enum {
    rv_func_LB = 0b000,
    rv_func_LH = 0b001,
    rv_func_LW = 0b010,
    rv_func_LWU = 0b110,
    rv_func_LD = 0b011,
    rv_func_LBU = 0b100,
    rv_func_LHU = 0b101
} rv_load_func_t;

/** Funct values for STORE instructions */
typedef enum {
    rv_func_SB = 0b000,
    rv_func_SH = 0b001,
    rv_func_SW = 0b010,
    rv_func_SD = 0b011,
} rv_store_func_t;

/** Funct values for SYSTEM instructions */
typedef enum {
    rv_funcPRIV = 0b000,
    rv_funcCSRRW = 0b001,
    rv_funcCSRRS = 0b010,
    rv_funcCSRRC = 0b011,
    rv_funcCSRRWI = 0b101,
    rv_funcCSRRSI = 0b110,
    rv_funcCSRRCI = 0b111,

} rv_system_func_t;

/** Immediate values for PRIV SYSTEM instructions */
typedef enum {
    rv_privECALL = 0b000000000000,
    rv_privEBREAK = 0b000000000001,
    rv_privEHALT = 0b100011000000,
    rv_privEDUMP = 0b100011000001,
    rv_privETRACES = 0b100011000010,
    rv_privETRACER = 0b100011000011,
    rv_privECSRD = 0b100011000100,
    rv_privSRET = 0b000100000010,
    rv_privMRET = 0b001100000010,
    rv_privWFI = 0b000100000101
} rv_system_priv_imm_t;

#define rv_privSFENCEVMA_FUNCT7 0b0001001

/** Funct values for AMO instructions
 *  This value is the high 5 bits from funct7
 *  funct3 serves as the identifier of width of the instructions and only 32-bit width is supported now in msim
 *  the low 2 bits from funct7 describe the ordering constraints (acquire/release semantics) - we can ignore them in msim, because we do in-order processing
 */
typedef enum {
    rv_funcLR = 0b00010,
    rv_funcSC = 0b00011,
    rv_funcAMOSWAP = 0b00001,
    rv_funcAMOADD = 0b00000,
    rv_funcAMOXOR = 0b00100,
    rv_funcAMOAND = 0b01100,
    rv_funcAMOOR = 0b01000,
    rv_funcAMOMIN = 0b10000,
    rv_funcAMOMAX = 0b10100,
    rv_funcAMOMINU = 0b11000,
    rv_funcAMOMAXU = 0b11100
} rv_amo_funct_t;

#define RV_AMO_32_WLEN 0b010
#define RV_AMO_64_WLEN 0b011

typedef enum rv_exc (*rv_instr_func_t)(rv_cpu_t *, rv_instr_t);

#endif // RISCV_RV_INSTR_H_
