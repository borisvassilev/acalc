#ifndef NUMLIST_H
#define NUMLIST_H

#include "gmp.h"

enum numtype_e {
    INTEGER, /* not used inside number lists */
    RATIONAL,
    DECFRAC,
    NaN,
    NA
};

struct number_t {
    enum numtype_e type;
    mpq_t num;
};

struct numlist_t {
    size_t alloc;
    size_t len;
    struct number_t *buf;
};

void numlist_init(struct numlist_t **);
void numlist_push(struct numlist_t *, const enum numtype_e, char *numstr);
void numlist_grow(struct numlist_t *);
void numlist_release(struct numlist_t *);
size_t numlist_print(struct numlist_t *);

#endif /* NUMLIST_H */
