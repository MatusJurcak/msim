#define ehalt .word 0x8200
#define cpu_dump .word 0x8201

.section .vectors, "a"
    .org 0x0
    .long _start         /* Power on reset - PC */

    .org 0x4
    .long 0xFFFF8000     /* Power on reset - SP */

    .org 0x28
    .long _dmac_error_handler /* DMAC address error handler */

    .org 0x120
    .long _dmac_handler  /* DMAC channel 0 interrupt handler */

    .org 0x300
    .long _cmt_handler     /* CMT 1 interrupt handler */

.section .text
    .org 0x400
_start:
    ldc         r0, sr
    mov         #1, r8

    /* Write the values to source addresses */
    mov         #1, r0
    mov.l       source_address_0, r1
    mov.l       r0, @r1
    add         #1, r0
    add         #4, r1
    mov.l       r0, @r1
    add         #1, r0
    add         #4, r1
    mov.l       r0, @r1
    add         #1, r0
    add         #4, r1
    mov.l       r0, @r1

    mov.l       cmt_cmcsr0_address, r6
    /* Period is 8 cycles */
    mov         #0x40, r1
    mov.w       r1, @r6

    /* Prepare the DMAC channel 0 */
    mov.l       source_address_0, r10
    mov.l       dmac_sar0_address, r0
    mov.l       r10, @r0
    mov.l       destination_address_0, r10
    mov.l       dmac_dar0_address, r0
    mov.l       r10, @r0
    /* 4 transfers */
    mov         #4, r1
    mov.l       dmac_dmatcr0_address, r0
    mov.l       r1, @r0
    mov.l       dmac_chcr0_value, r1
    mov.l       dmac_chcr0_address, r0
    mov.l       r1, @r0

    /* Enable the DMAC */
    mov         #1, r0
    mov.l       dmac_dmaor_address, r1
    mov.w       r0, @r1

    /* Enable the CMT */
    mov.l       cmt_cmstr_address, r6
    mov         #1, r1
    mov.w       r1, @r6

main_loop:
    cmp/hs      r8, r9
    bf          main_loop
    nop

    /* Load the SAR register into CPU register, should be initial SAR + 0x10 (4 longword transfers) */
    mov.l       dmac_sar0_address, r2
    mov.l       @r2, r3
    cpu_dump
    /* Check if every value was correctly written into memory */
    mov.l       @r10+, r0
    cpu_dump
    mov.l       @r10+, r0
    cpu_dump
    mov.l       @r10+, r0
    cpu_dump
    mov.l       @r10+, r0
    cpu_dump
    ehalt

_dmac_handler:
    mov     #0, r0
    add     #1, r9
    ldc     r0, sr
    bra     main_loop
    nop

_dmac_error_handler:
    mov     #2, r9
    bra     main_loop
    nop

_cmt_handler:
    mov     #0, r0
    ldc     r0, sr
    bra     main_loop
    nop

.align 2
dmac_sar0_address:
    .long       0xFFFFECC0

dmac_dar0_address:
    .long       0xFFFFECC4

dmac_dmatcr0_address:
    .long       0xFFFFECC8

dmac_chcr0_address:
    .long       0xFFFFECCC

dmac_dmaor_address:
    .long       0xFFFFECB0

/* 00 - direct access, do not reload */
/* 01 - wait for CMT request */
/* 11 - increment both source address and destination address after transfer */
/* 25 - longword size, cycle-steal, interrupt generated, enabled channel */
dmac_chcr0_value:
    .long       0x00011125

source_address_0:
    .long       0xFFFFA000

destination_address_0:
    .long       0xFFFFB000

cmt_cmstr_address:
    .long       0xFFFFF710

cmt_cmcsr0_address:
    .long       0xFFFFF712