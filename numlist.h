#ifndef NUMLIST_H
#define NUMLIST_H

enum number_t {
    INTEGER,
    RATIONAL,
    FRACTION
};

struct number {
    enum number_t type;
    unsigned int base;
    mpq_t num;
};

struct numlist {
    size_t alloc;
    size_t len;
    struct number *buf;
};

void numlist_first(struct numlist *, const struct number);
void numlist_push(struct numlist *, const struct number);
void numlist_grow(struct numlist *);
void numlist_release(struct numlist *);
size_t numlist_print(struct numlist *);

#endif /* NUMLIST_H */
