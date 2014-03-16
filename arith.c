#include <stdio.h>

#include "arith.h"
#include "globals.h"
#include "numlist.h"
#include "numstack.h"

void check_lengths(struct numlist_t *longer, struct numlist_t *shorter)
{
    if (longer->len % shorter->len != 0)
        fprintf(stderr, "Warning: "
                "Longer array length %zu not a multiple of "
                "shorter array length %zu\n",
                longer->len, shorter->len);
}

int na_or_nan(struct number_t *op1_res, struct number_t *op2)
{
    if (op1_res->type == NA || op2->type == NA) {
        op1_res->type = NA;
        return 1;
    }

    if (op1_res->type == NaN || op2->type == NaN) {
        op1_res->type = NaN;
        return 1;
    }

    return 0;
}

void set_result_type(struct number_t *op1_res, struct number_t *op2)
{
    if (op1_res->type == DECFRAC || op2->type == DECFRAC)
        op1_res->type = DECFRAC;
    /* implicit:
    else
        op1_res->type = RATIONAL;
    */
}

void arith_binary_commutative(enum arithop_e op)
{
    struct numlist_t *op1, *op2;
    numstack_pop(&global_stack, &op2);
    numstack_pop(&global_stack, &op1);

    struct numlist_t *l, *s; /* longer and shorter arrays */
    if (op1->len >= op2->len) {
        l = op1;
        s = op2;
    } else {
        l = op2;
        s = op1;
    }

    check_lengths(l, s);

    size_t il, is;
    for (il = 0, is = 0; il != l->len; ++il, ++is) {
        if (is == s->len)
            is = 0;

        if (!na_or_nan(l->buf + il, s->buf + is)) {

            switch (op)
            {
            case ADD:
                mpq_add(l->buf[il].num, l->buf[il].num, s->buf[is].num);
                break;
            case MUL:
                mpq_mul(l->buf[il].num, l->buf[il].num, s->buf[is].num);
                break;
            default:
                break;
            }

            set_result_type(l->buf + il, s->buf + is);
        }
    }

    numlist_release(s);
    numstack_push(&global_stack, l);
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
    /* binary commutative operators */
    case ADD: case MUL:
        arith_binary_commutative(op);
        break;
        
    case SUB: case DIV:
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

