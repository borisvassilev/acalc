#include "memwrap.h"
#include "numlist.h"

void numlist_first(struct numlist *nl, const mpq_t first)
{
    nl->buf = xmalloc(sizeof (mpq_t));
    nl->alloc = nl->len = 1;
    mpq_init(nl->buf[0]);
    mpq_set(nl->buf[0], first);
}

void numlist_push(struct numlist *nl, const mpq_t num)
{
    if (nl->len == nl->alloc)
        numlist_grow(nl);

    mpq_init(nl->buf[nl->len]);
    mpq_set(nl->buf[nl->len], num);
    nl->len++;
}


void numlist_grow(struct numlist *nl)
{
    nl->alloc = alloc_nr(nl->alloc);
    nl->buf = xrealloc(nl->buf, nl->alloc * sizeof (mpq_t));
}

void numlist_release(struct numlist *nl)
{
    size_t i;
    for (i = 0; i < nl->len; ++i)
        mpq_clear(nl->buf[i]);

    free(nl->buf);
}

size_t numlist_print(struct numlist *nl)
{
    size_t i;
    for (i = 0; i < nl->len - 1; ++i)
        gmp_printf("%Qd ", nl->buf[i]);
    gmp_printf("%Qd\n", nl->buf[i]);
    return ++i;
}
