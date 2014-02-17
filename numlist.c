#include <stdio.h>
#include "memwrap.h"
#include "numlist.h"

void num_init_set(struct number *np, const enum number_t type, FILE *input)
{
    np->type = type;
    switch (type) {
    case INTEGER:
        mpz_init(np->num.integer);
        mpz_inp_str(np->num.integer, input, 10);
        return;
    case RATIONAL:
    case REAL:
    case NA:
        return;
    }
}

void num_clear(struct number *np)
{
    switch (np->type) {
    case INTEGER:
        mpz_clear(np->num.integer);
        return;
    case RATIONAL:
        mpq_clear(np->num.rational);
        return;
    case REAL:
        mpq_clear(np->num.real);
        return;
    case NA:
        return;
    }
}

void num_print(struct number *np)
{
    switch (np->type) {
    case INTEGER:
        gmp_printf("%Zd", np->num.integer);
        return;
    case RATIONAL:
        gmp_printf("%Qd", np->num.rational);
        return;
    case REAL: /* to be done properly */
        gmp_printf("%Qd", np->num.real);
        return;
    case NA:
        printf("NA");
        return;
    }
}

void numlist_first(struct numlist *nl, const enum number_t type)
{
    nl->buf = xmalloc(sizeof (struct number));
    nl->alloc = nl->len = 1;
    num_init_set(nl->buf, type, stdin);
}

void numlist_push(struct numlist *nl, const enum number_t type)
{
    if (nl->len == nl->alloc)
        numlist_grow(nl);

    num_init_set(nl->buf + nl->len, type, stdin);
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
