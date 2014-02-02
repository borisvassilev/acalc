#ifndef NUMLIST_H
#define NUMLIST_H

struct numlist {
    size_t alloc;
    size_t len;
    mpq_t *buf;
};

void numlist_first(struct numlist *, const mpq_t);
void numlist_grow(struct numlist *);
void numlist_release(struct numlist *);
void numlist_push(struct numlist *, const mpq_t);
size_t numlist_print(struct numlist *);

#endif /* NUMLIST_H */
