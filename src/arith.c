/*
Copyright Boris Vassilev 2015

This file is part of acalc.

acalc is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

acalc is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with acalc.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdio.h>

#include "arith.h"
#include "globals.h"
#include "numlist.h"
#include "numstack.h"

void check_lengths(const size_t longer, const size_t shorter)
{
    if (longer % shorter != 0)
        fprintf(stderr, "Warning: "
                "Longer array length %zu not a multiple of "
                "shorter array length %zu\n",
                longer, shorter);
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

void arith_binary(enum arithop_e op)
{
    struct numlist_t *op1, *op2, *oplong, *opshort;
    size_t i1, i2, *ilong, *ishort;

    numstack_pop(&global_stack, &op2);
    numstack_pop(&global_stack, &op1);

    if (op1->len >= op2->len) {
        check_lengths(op1->len, op2->len);
        oplong = op1;
        opshort = op2;
        ilong = &i1;
        ishort = &i2;
    } else {
        check_lengths(op2->len, op1->len);
        oplong = op2;
        opshort = op1;
        ilong = &i2;
        ishort = &i1;
    }

    for (i1 = 0, i2 = 0; *ilong != oplong->len; ++i1, ++i2) {
        if (*ishort == opshort->len)
            *ishort = 0;

        if (!na_or_nan(op1->buf + i1, op2->buf + i2)) {

            switch (op)
            {
            case ADD:
                mpq_add(oplong->buf[*ilong].num,
                        op1->buf[i1].num,
                        op2->buf[i2].num);
                break;

            case SUB:
                mpq_sub(oplong->buf[*ilong].num,
                        op1->buf[i1].num,
                        op2->buf[i2].num);
                break;

            case MUL:
                mpq_mul(oplong->buf[*ilong].num,
                        op1->buf[i1].num,
                        op2->buf[i2].num);
                break;

            case DIV:
                mpq_div(oplong->buf[*ilong].num,
                        op1->buf[i1].num,
                        op2->buf[i2].num);
                break;
            }

            set_result_type(oplong->buf + *ilong, opshort->buf + *ishort);
        }
    }

    numlist_release(opshort);
    numstack_push(&global_stack, oplong);
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
    case ADD: case SUB: case MUL: case DIV:
        arith_binary(op);
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

