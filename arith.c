#include <stdio.h>

#include "arith.h"
#include "globals.h"
#include "numlist.h"
#include "numstack.h"

void arith_add()
{
    struct numlist_t *op1, *op2;
    numstack_pop(&global_stack, &op2);
    numstack_pop(&global_stack, &op1);

    struct numlist_t *longer, *shorter;
    if (op1->len >= op2->len) {
        longer = op1;
        shorter = op2;
    } else {
        longer = op2;
        shorter = op1;
    }

    if (longer->len % shorter->len != 0)
        fprintf(stderr, "Warning: "
                "Length of longer array (%zu) not a multiple of the "
                "length of the shorter array (%zu)\n",
                longer->len, shorter->len);

    size_t il, is;
    for (il = 0, is = 0; il != longer->len; ++il, ++is) {
        if (is == shorter->len)
            is = 0;

        if (longer->buf[il].type == NA
                || shorter->buf[is].type == NA)
            longer->buf[il].type = NA;
        else if (longer->buf[il].type == NaN
                || shorter->buf[is].type == NaN)
            longer->buf[il].type = NaN;
        else
            mpq_add(longer->buf[il].num,
                    longer->buf[il].num,
                    shorter->buf[is].num);
    }

    numlist_release(shorter);
    numstack_push(&global_stack, longer);
}
/*
 * Make sure the necessary operands are there,
 * call the appropriate arithmetic function
 */
size_t arithop_arity(enum arithop_e);
exit_status apply_arithop(enum arithop_e op)
{
    if (global_stack.len < arithop_arity(op))
        return ARITH_TOO_FEW_OPERANDS;

    switch (op)
    {
    /* binary operators */
    case ADD:
        arith_add();
        break;
        
    case SUB: case MUL: case DIV:
        break;
    }
    return SUCCESS;
}

size_t arithop_arity(enum arithop_e op)
{
    size_t a;
    switch (op)
    {
    case ADD: case SUB: case MUL: case DIV:
        a = 2;
        break;
    }
    return a;
}

