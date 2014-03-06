#include <stdio.h>
#include <stdlib.h>

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
    case REAL: /* to be done properly */
        mpz_tdiv_qr(mpz1, mpz2, mpq_numref(np->num.q), mpq_denref(np->num.q));
        gmp_printf("%Zd", mpz1);
        if (mpz_cmp_ui(mpz2, 0) != 0)
            gmp_printf("|%Zd/%Zd", mpz2, mpq_denref(np->num.q));
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
