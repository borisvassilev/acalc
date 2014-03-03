#include <stdio.h>
#include "memwrap.h"
#include "numlist.h"

void num_init_set(
        struct number *np,
        const enum number_t type,
        char *numstr,
        mpq_t tmpq)
{
    /* skip leading plus */
    if (*numstr == '+')
        numstr += 1;

    switch (type) {

    case INTEGER:
        mpz_init(np->num.z);
        mpz_set_str(np->num.z, numstr, 10);
        np->type = INTEGER;
        break;

    case RATIONAL:
        mpq_set_str(tmpq, numstr, 10);
        mpq_canonicalize(tmpq);
        if (mpz_cmp_ui(mpq_denref(tmpq), 1) == 0) {
            mpz_init(np->num.z);
            mpz_set(np->num.z, mpq_numref(tmpq));
            np->type = INTEGER;
        } else {
            mpq_init(np->num.q);
            mpq_set(np->num.q, tmpq);
            np->type = RATIONAL;
        }
        break;

    case REAL:
    case NA:
        break;
    }
}

void num_clear(struct number *np)
{
    switch (np->type) {
    case INTEGER:
        mpz_clear(np->num.z);
        return;
    case RATIONAL:
        mpq_clear(np->num.q);
        return;
    case REAL:
        mpq_clear(np->num.q);
        return;
    case NA:
        return;
    }
}

void num_print(struct number *np)
{
    switch (np->type) {
    case INTEGER:
        gmp_printf("%Zd", np->num.z);
        return;
    case RATIONAL:
        gmp_printf("%Qd", np->num.q);
        return;
    case REAL: /* to be done properly */
        gmp_printf("%Qd", np->num.q);
        return;
    case NA:
        printf("NA");
        return;
    }
}

void numlist_init(struct numlist **nl)/*, const enum number_t type, char *numstr)*/
{
    *nl = (struct numlist *) xmalloc(sizeof (struct numlist));
    (*nl)->alloc = 1;
    (*nl)->buf = (struct number *) xmalloc(sizeof (struct number));
    (*nl)->len = 0;
    mpq_init((*nl)->tmpq);
}

void numlist_push(struct numlist *nl, const enum number_t type, char *numstr)
{
    if (nl->len == nl->alloc)
        numlist_grow(nl);

    num_init_set(nl->buf + nl->len, type, numstr, nl->tmpq);
    nl->len++;
}


void numlist_grow(struct numlist *nl)
{
    nl->alloc = alloc_nr(nl->alloc);
    nl->buf = xrealloc(nl->buf, nl->alloc * sizeof (struct number));
}

void numlist_release(struct numlist *nl)
{
    size_t i;
    for (i = 0; i < nl->len; ++i)
        num_clear(nl->buf + i);

    free(nl->buf);
    mpq_clear(nl->tmpq);
    free(nl);
}

size_t numlist_print(struct numlist *nl)
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
