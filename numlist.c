#include <stdio.h>
#include <stdlib.h>

#include "memwrap.h"
#include "tmpvars.h"
#include "globals.h"

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

    case REAL: /* to be done properly */
        mpz_tdiv_qr(mpz1, mpz2, mpq_numref(np->num.q), mpq_denref(np->num.q));
        if (mpz_cmp_ui(mpz2, 0) == 0) { /* remainder is 0 */
            gmp_printf("%Zd", mpz1);
            break;
        }
        size_t max_len =
              mpz_sizeinbase(mpz1, 10) /* length of the string in digits */
            + 1 /* '\0' terminator */
            /* + 1  possible minus sign: not necessary */
            + 1 /* leading 0 */
            + 1 /* decimal point */
            + real_precision + 1; /* precision and the extra digit */

        strbuf_reset(&iobuf);
        strbuf_grow(&iobuf, max_len);
        char *str = strbuf_str(&iobuf, 0);

        int negative = 0;
        if (mpz_sgn(mpz1) == -1) /* negative number */
            negative = 1;
        mpz_abs(mpz1, mpz1);

        *str++ = '0'; /* leading 0 */

        str += gmp_sprintf(str, "%Zd", mpz1);
        char *dec_point_pos = str; /* save the position for the point */

        mpz_abs(mpz2, mpz2);
        mpz_ui_pow_ui(mpz1, 10, real_precision + 1);
        mpz_mul(mpz2, mpz2, mpz1);
        mpz_tdiv_q(mpz2, mpz2, mpq_denref(np->num.q));
        str += gmp_sprintf(str, "%Zd", mpz2);

        if (negative)
            putc('-', stdout);
        printf("real%s", strbuf_str(&iobuf, 0));

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

    return ++i;
}
