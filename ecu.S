.section .vectors, "a"
    .global _start

    .long _start      /* Power on reset - PC */

    .org 0x4
    .long 0xFFFF8000     /* Power on reset - SP */

    .org 0x2F0
    .long _cmt0_start    /* CMT0 EPVA value */

    .org 0x300
    .long _cmt1_start    /* CMT1 EPVA value */

    .section .text
    .global _start
    .align 2

_start:
    MOV.L   printer_address, R1
    MOV.L   intc_iprj_address, R4
    MOV.W   @R4, R2
    MOV.L   intc_iprj_new_address, R4
    MOV.W   @R4, R3
    MOV.L   sr_value_for_ldc_instruction, R0
    LDC     R0, SR
    NOP
    NOP
    MOV     #77, R0
    MOV.L   R0, @R1
    MOV     #97, R0
    MOV.L   R0, @R1
    MOV     #114, R0
    MOV.L   R0, @R1
    MOV     #116, R0
    MOV.L   R0, @R1
    MOV     #105, R0
    MOV.L   R0, @R1
    MOV     #110, R0
    MOV.L   R0, @R1
    MOV     #10, R0
    MOV.L   R0, @R1
    
    .word 0x8200

.align 2
sr_value_for_ldc_instruction:
    .long 0x00000000

.align 2
printer_address:
    .long 0x90000000

intc_iprj_address:
    .long 0xFFFFED12

intc_iprj_new_address:
    .long 0xFFFFE012

    /* ---- CMT0 interrupt handler ---- */
    .section _cmt0, "ax"
    .org 0x7000
    .global _cmt0_start
_cmt0_start:
    MOV     #99, R0
    MOV.L   R0, @R1
    MOV     #109, R0
    MOV.L   R0, @R1
    MOV     #116, R0
    MOV.L   R0, @R1
    MOV     #48, R0
    MOV.L   R0, @R1
    MOV     #10, R0
    MOV.L   R0, @R1
    RTE


    /* ---- CMT1 interrupt handler ---- */
    .section _cmt1, "ax"
    .org 0x8000
    .global _cmt1_start
_cmt1_start:
    MOV     #99, R0
    MOV.L   R0, @R1
    MOV     #109, R0
    MOV.L   R0, @R1
    MOV     #116, R0
    MOV.L   R0, @R1
    MOV     #49, R0
    MOV.L   R0, @R1
    MOV     #10, R0
    MOV.L   R0, @R1
    RTE

