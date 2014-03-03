#ifndef NUMLIST_H
#define NUMLIST_H

#include "gmp.h"

enum number_t {
    INTEGER,
    RATIONAL,
    REAL,
    NA
};

union number_u {
    mpz_t z;
    mpq_t q;
};

struct number {
    enum number_t type;
    union number_u num;
};

struct numlist {
    size_t alloc;
    size_t len;
    struct number *buf;
    mpq_t tmpq;
};

void numlist_init(struct numlist **);
void numlist_push(struct numlist *, const enum number_t, char *numstr);
void numlist_grow(struct numlist *);
void numlist_release(struct numlist *);
size_t numlist_print(struct numlist *);

#endif /* NUMLIST_H */
