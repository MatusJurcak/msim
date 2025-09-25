
#include "../assert.h"
#include "../fault.h"
#include "../utils.h"
#include "device.h"
#include "dsh2eintc.h"
#include "intc/general_intc.h"
#include "intc/superh_sh2e/intc.h"

#define get_sh2e_intc(dev) ((sh2e_intc_t *) (((general_intc_t *) (dev)->data)->data))

static intc_ops_t const sh2e_intc_ops = {
    .interrupt_up = (interrupt_func_t) sh2e_intc_assert_interrupt,
    .interrupt_down = (interrupt_func_t) sh2e_intc_deassert_interrupt,

    .check_interrupts = (check_interrupts_func_t) sh2e_check_pending_interrupts,
    .accept_interrupt = (accept_interrupt_func_t) sh2e_accept_interrupt,

    .init = (init_intc_func_t) sh2e_intc_init_regs,
};

/** INTC initialization */
static bool
dsh2eintc_cmd_init(token_t *parm, device_t *const dev)
{
    ASSERT(dev != NULL);

    unsigned int id = get_free_intcno();
    if (id == MAX_INTCS) {
        error("Maximum INTC count exceeded (%u)", MAX_INTCS);
        return false;
    }

    sh2e_intc_t *sh2e_intc = safe_malloc_t(sh2e_intc_t);
    sh2e_intc_init(sh2e_intc, id);

    general_intc_t *generic_intc = safe_malloc_t(general_intc_t);
    *generic_intc = (general_intc_t) {
        .intcno = id,
        .data = sh2e_intc,
        .type = &sh2e_intc_ops
    };

    add_intc(generic_intc);

    dev->data = generic_intc;

    return true;
}

/** INTC cleanup */
static void
dsh2eintc_done(device_t *const dev)
{
    ASSERT(dev != NULL);

    general_intc_t *generic_intc = (general_intc_t *) dev->data;

    sh2e_intc_t *sh2e_intc = (sh2e_intc_t *) generic_intc->data;
    sh2e_intc_done(sh2e_intc);
    safe_free(sh2e_intc);

    safe_free(generic_intc);
}

/** Configure interrupt controller address command. */
static bool
dsh2eintc_cmd_configure_register_address(token_t *parm, device_t *const dev)
{
    ASSERT(dev != NULL);

    uint32_t addr = ALIGN_DOWN(parm_uint_next(&parm), sizeof(uint32_t));

    sh2e_intc_change_regs_address(get_sh2e_intc(dev), addr);

    return true;
}

/** Add interrupt source command. */
static bool
dsh2eintc_cmd_add_interrupt_source(token_t *parm, device_t *const dev)
{
    ASSERT(dev != NULL);

    uint16_t source_id = parm_uint_next(&parm);
    uint8_t priority_pool_index = parm_uint_next(&parm);
    uint8_t priority = parm_uint_next(&parm);

    sh2e_intc_add_interrupt_source(get_sh2e_intc(dev), source_id, priority_pool_index, priority);

    return true;
}

/*
 * Device commands
 */
static cmd_t dsh2eintc_cmds[] = {
    { "init",
            (fcmd_t) dsh2eintc_cmd_init,
            DEFAULT,
            DEFAULT,
            "Initialization",
            "Initialization",
            REQ STR "pname/processor name" END },
    { "help",
            (fcmd_t) dev_generic_help,
            DEFAULT,
            DEFAULT,
            "Display this help text",
            "Display this help text",
            OPT STR "cmd/command name" END },
    { "regaddr",
            (fcmd_t) dsh2eintc_cmd_configure_register_address,
            DEFAULT,
            DEFAULT,
            "Configure INTC registers addresses",
            "Configure INTC registers addresses",
            REQ INT "addr/address" END },
    { "addintsrc",
            (fcmd_t) dsh2eintc_cmd_add_interrupt_source,
            DEFAULT,
            DEFAULT,
            "Add interrupt source",
            "Add interrupt source <source_id> <priority_pool_index> <priority>",
            REQ INT "source_id" NEXT
                    REQ INT "priority_pool_index" NEXT
                            REQ INT "priority" END },
    LAST_CMD
};

device_type_t dsh2eintc = {
    /* SH-2E INTC is simulated deterministically */
    .nondet = false,

    .name = "dsh2eintc",
    .brief = "Interrupt controller",
    .full = "Interrupt controller device",

    /* Functions */
    .done = dsh2eintc_done,
    /* Commands */
    .cmds = dsh2eintc_cmds
};
