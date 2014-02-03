#include "memwrap.h"
#include "numlist.h"

void numlist_first(struct numlist *nl, const struct number first)
{
    nl->buf = xmalloc(sizeof (struct number));
    nl->alloc = nl->len = 1;
    nl->buf[0].type = first.type;
    nl->buf[0].base = first.base;
    mpq_init(nl->buf[0].num);
    mpq_set(nl->buf[0].num, first.num);
}

void numlist_push(struct numlist *nl, const struct number num)
{
    if (nl->len == nl->alloc)
        numlist_grow(nl);

    nl->buf[nl->len].type = num.type;
    nl->buf[nl->len].base = num.base;
    mpq_init(nl->buf[nl->len].num);
    mpq_set(nl->buf[nl->len].num, num.num);
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
        mpq_clear(nl->buf[i].num);

    free(nl->buf);
}

size_t numlist_print(struct numlist *nl)
{
    size_t i;
    for (i = 0; i < nl->len - 1; ++i) {
        gmp_printf("%Qd ", nl->buf[i].num);
    }
    gmp_printf("%Qd\n", nl->buf[i].num);
    return ++i;
}
