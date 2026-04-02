/*
 * Copyright (c) 2025 Lubomir Bulej
 * Copyright (c) 2026 Matus Jurcak
 *
 * All rights reserved.
 *
 * Distributed under the terms of GPL.
 *
 *
 * Hitachi/Renesas SuperH SH-2E microprocessor device (32-bit, FPU).
 *
 */

#include <stdio.h>

#include "../../../assert.h"
#include "debug.h"
#include "intc.h"

/** Processor register names in various styles. */

char const *const sh2e_intc_priority_reg_names[SH2E_INTC_IPR_REGISTERS_COUNT] = {
    "ipra",
    "iprb",
    "iprc",
    "iprd",
    "ipre",
    "iprf",
    "iprg",
    "iprh",
    "ipri",
    "iprj",
    "iprk",
    "iprl",
};

char const *const sh2e_intc_system_reg_names[SH2E_INTC_SYSTEM_REGISTERS_COUNT] = {
    "icr",
    "isr",
};

/**
 * @brief Dump the content of the INTC registers.
 */
void sh2e_intc_dump_regs(sh2e_intc_t const *const restrict intc)
{
    ASSERT(intc != NULL);

    printf("intc %u\n", intc->id);

    // General registers.
    for (unsigned int i = 0; i < SH2E_INTC_IPR_REGISTERS_COUNT; ++i) {
        printf(
                // I used this format in order to see all the digits as every digit
                // represents one priority pool index (4-bit value)
                " %5s: %04x\n",
                sh2e_intc_priority_reg_names[i],
                intc->intc_regs.priority[i]);
    }

    // System registers.
    for (unsigned int i = 0; i < SH2E_INTC_SYSTEM_REGISTERS_COUNT; ++i) {
        printf(
                " %5s: %4x\n",
                sh2e_intc_system_reg_names[i],
                intc->intc_regs.system[i]);
    }
}
