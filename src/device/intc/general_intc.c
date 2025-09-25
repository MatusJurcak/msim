

#include "general_intc.h"

#include "../../assert.h"
#include "../../list.h"
#include "../../utils.h"

// list of all interrupt controllers
list_t intc_list = LIST_INITIALIZER;

general_intc_t *
get_intc(unsigned int no) {
    general_intc_t * intc;
    for_each(intc_list, intc, general_intc_t) {
        if (intc->intcno == no) {
            return intc;
        }
    }
    return NULL;
}

unsigned int
get_free_intcno(void) {
    unsigned int c;
    unsigned int id_mask = 0;
    general_intc_t * intc;

    for_each(intc_list, intc, general_intc_t) {
        id_mask |= 1 << (intc->intcno);
    }

    for (c = 0; c < MAX_INTCS; c++, id_mask >>= 1) {
        if (!(id_mask & 1)) {
            return c;
        }
    }

    return MAX_INTCS;
}

void
add_intc(general_intc_t * intc) {
    item_init(&intc->item);
    list_append(&intc_list, &intc->item);
}

void
remove_intc(general_intc_t * intc) {
    list_remove(&intc_list, &intc->item);
}

static general_intc_t *
get_fallback_intc(void) {
    general_intc_t * intc = get_intc(0);
    if (intc == NULL) {
        intc = (general_intc_t *) intc_list.head;
    }
    ASSERT(intc != NULL);
    return intc;
}

void
intc_interrupt_up(general_intc_t * intc, unsigned int no) {
    if (intc == NULL) {
        intc = get_fallback_intc();
    }
    intc->type->interrupt_up(intc->data, no);
}

void
intc_interrupt_down(general_intc_t * intc, unsigned int no) {
    if (intc == NULL) {
        intc = get_fallback_intc();
    }
    intc->type->interrupt_down(intc->data, no);
}

uint8_t
intc_check_interrupts(general_intc_t * intc, uint8_t mask) {
    if (intc == NULL) {
        intc = get_fallback_intc();
    }
    return intc->type->check_interrupts(intc->data, mask);
}

uint8_t
intc_accept_interrupt(general_intc_t * intc) {
    if (intc == NULL) {
        intc = get_fallback_intc();
    }
    return intc->type->accept_interrupt(intc->data);
}
