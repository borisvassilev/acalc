#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memwrap.h"
#include "globals.h"
#include "tmpvars.h"
#include "strbuf.h"

#include "numlist.h"

void numlist_init(struct numlist_t **nl)
{
    *nl = (struct numlist_t *) xmalloc(sizeof (struct numlist_t));
    (*nl)->alloc = 1;
    (*nl)->buf = (struct number_t *) xmalloc(sizeof (struct number_t));
    (*nl)->len = 0;
}

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

    case DECFRAC:
        mpq_init(np->num.q);
        mpq_set_str(np->num.q, buf, 10);
        mpq_canonicalize(np->num.q);
        np->type = DECFRAC;
        break;

    case NA:
    case NaN:
        np->type = nt;
        break;
    }
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

void num_clear(struct number_t *np)
{
    switch (np->type) {
    case INTEGER:
        mpz_clear(np->num.z);
        break;
    case RATIONAL:
    case DECFRAC:
        mpq_clear(np->num.q);
        break;
    case NaN:
    case NA:
        break;
    }
}

void numlist_release(struct numlist_t *nl)
{
    size_t i;
    for (i = 0; i < nl->len; ++i)
        num_clear(nl->buf + i);

    free(nl->buf);
    free(nl);
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

    case DECFRAC:
        mpq_set(mpq1, np->num.q);  /* copy number */
        if (mpq_sgn(mpq1) == -1) { /* negative number */
            putc('-', stdout);
            if (mpq_sgn(mpq2) == 1)
                mpq_neg(mpq2, mpq2);
        } else
            mpq_abs(mpq2, mpq2);

        mpq_add(mpq1, mpq1, mpq2); /* add 1/(2*10^precision) */

        mpq_abs(mpq1, mpq1); /* remove the sign */
        mpz_tdiv_qr(mpz2, mpz3, mpq_numref(mpq1), mpq_denref(mpq1));
        gmp_printf("%Zd", mpz2); /* integer part */

        /* get fractional part */
        mpz_mul(mpz3, mpz3, mpz1);
        mpz_tdiv_q(mpz3, mpz3, mpq_denref(mpq1));

        char *start = strbuf_str(&iobuf, 0); /* for convenience */
        size_t written = gmp_sprintf(start, "%Zd", mpz3);
        char *end = start + written - 1;

        /* remove trailing zeroes */
        while (end != start && *(end - 1) == '0')
            --end;
        if (end == start) /* there is nothing after the integer part */
            break;
        putc('.', stdout);

        /* print out the necesssary 0s */
        while (written++ < decfrac_prec + 1)
            putc('0', stdout);

        /* and print out the rest of the significant digits */
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

size_t numlist_print(struct numlist_t *nl)
{
    /* Assuming that all decimal fractions on a line are with the same
     * precision, prepare some variable that are used
     */
    /* Maximum length that the io buffer might need */
    strbuf_grow(&iobuf, decfrac_prec + 2);

    /* The rational that will be added for rounding */
    mpz_ui_pow_ui(mpz1, 10, decfrac_prec);
    mpz_mul_ui(mpz1, mpz1, 2);       /* 2*10^precision */
    mpz_set_ui(mpq_numref(mpq2), 1); /* 1 */
    mpz_set(mpq_denref(mpq2), mpz1); /* 1/(2*10^precision) */

    /* The factor by which the remainder will be multiplied */
    mpz_mul_ui(mpz1, mpz1, 5); /* 10^(precision + 1) */

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
