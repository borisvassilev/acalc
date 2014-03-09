#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memwrap.h"
#include "globals.h"
#include "tmpvars.h"
#include "strbuf.h"

#include "numlist.h"

void num_init_set(struct number_t *np, const enum numtype_e nt, char *buf)
{
    /* skip leading plus */
    if (*buf == '+')
        ++buf;

    switch (nt) {

    case INTEGER:
        mpz_init(np->num.z);
        mpz_set_str(np->num.z, buf, 10);
        np->type = INTEGER;
        break;

    case RATIONAL:
        mpq_set_str(mpq1, buf, 10);
        /* if the denumerator is 0, this is not a number */
        if (mpz_cmp_ui(mpq_denref(mpq1), 0) == 0) {
            np->type = NaN;
            break;
        }
        mpq_canonicalize(mpq1);
        /* if the denumerator is 1, this is an integer */
        if (mpz_cmp_ui(mpq_denref(mpq1), 1) == 0) {
            mpz_init(np->num.z);
            mpz_set(np->num.z, mpq_numref(mpq1));
            np->type = INTEGER;
        } else {
            mpq_init(np->num.q);
            mpq_set(np->num.q, mpq1);
            np->type = RATIONAL;
        }
        break;

    case REAL:
        mpq_init(np->num.q);
        mpq_set_str(np->num.q, buf, 10);
        mpq_canonicalize(np->num.q);
        np->type = REAL;
        break;

    case NA:
    case NaN:
        np->type = nt;
        break;
    }
}

void num_clear(struct number_t *np)
{
    switch (np->type) {
    case INTEGER:
        mpz_clear(np->num.z);
        break;
    case RATIONAL:
    case REAL:
        mpq_clear(np->num.q);
        break;
    case NaN:
    case NA:
        break;
    }
}

void num_print(struct number_t *np)
{
    switch (np->type) {

    case INTEGER:
        gmp_printf("%Zd", np->num.z);
        break;

    case RATIONAL:
        gmp_printf("%Qd", np->num.q);
        break;

    case REAL:
        mpq_set(mpq1, np->num.q);  /* copy number */
        int neg = 0;
        if (mpq_sgn(mpq1) == -1) {
            neg = 1;
            putc('-', stdout);
        }

        mpz_ui_pow_ui(mpz1, 10, real_precision);
        mpz_mul_ui(mpz1, mpz1, 2);       /* 2*10^precision */
        mpz_set_ui(mpq_numref(mpq2), 1); /* 1 */
        mpz_set(mpq_denref(mpq2), mpz1); /* 1/(2*10^precision) */
        if (neg) /* negative number */
            mpq_neg(mpq2, mpq2);

        mpq_add(mpq1, mpq1, mpq2); /* add 1/(2*10^precision) */

        mpq_abs(mpq1, mpq1); /* remove the sign */
        mpz_tdiv_qr(mpz2, mpz3, mpq_numref(mpq1), mpq_denref(mpq1));
        gmp_printf("%Zd", mpz2); /* integer part */

        /* get fractional part */
        mpz_mul_ui(mpz1, mpz1, 5); /* 10^(precision + 1) */
        mpz_mul(mpz3, mpz3, mpz1);
        mpz_tdiv_q(mpz3, mpz3, mpq_denref(mpq1));

        strbuf_grow(&iobuf, real_precision + 2);
        char *start = strbuf_str(&iobuf, 0); /* for convenience */
        size_t written = gmp_sprintf(start, "%Zd", mpz3);
        char *end = start + written - 1;

        while (end != start && *(end - 1) == '0')
            --end;
        if (end == start)
            break;

        putc('.', stdout);

        while (written++ < real_precision + 1)
            putc('0', stdout);

        while (start != end)
            putc(*start++, stdout);

        break;

    case NA:
        printf("NA");
        break;

    case NaN:
        printf("NaN");
        break;
    }
}

void numlist_init(struct numlist_t **nl)
{
    *nl = (struct numlist_t *) xmalloc(sizeof (struct numlist_t));
    (*nl)->alloc = 1;
    (*nl)->buf = (struct number_t *) xmalloc(sizeof (struct number_t));
    (*nl)->len = 0;
}

void numlist_push(struct numlist_t *nl, const enum numtype_e nt, char *buf)
{
    if (nl->len == nl->alloc)
        numlist_grow(nl);

    num_init_set(nl->buf + nl->len, nt, buf);
    nl->len++;
}


void numlist_grow(struct numlist_t *nl)
{
    nl->alloc = alloc_nr(nl->alloc);
    nl->buf = xrealloc(nl->buf, nl->alloc * sizeof (struct number_t));
}

void numlist_release(struct numlist_t *nl)
{
    size_t i;
    for (i = 0; i < nl->len; ++i)
        num_clear(nl->buf + i);

    free(nl->buf);
    free(nl);
}

size_t numlist_print(struct numlist_t *nl)
{
    size_t i;
    for (i = 0; i < nl->len - 1; ++i) {
        num_print(nl->buf + i);
        putc(' ', stdout);
    }

    num_print(nl->buf + i);
    putc('\n', stdout);

    /* in case we had a huge number while printing
     * this will reclaim the memory
     */
    strbuf_reinit(&iobuf, 0);

    return ++i;
}
