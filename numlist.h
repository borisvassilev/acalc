#ifndef NUMLIST_H
#define NUMLIST_H

enum number_t {
    INTEGER,
    RATIONAL,
    REAL,
    NA
};

union number_u {
    mpz_t integer;
    mpq_t rational;
    mpq_t real;
};

struct number {
    enum number_t type;
    union number_u num;
};

struct numlist {
    size_t alloc;
    size_t len;
    struct number *buf;
};

void numlist_first(struct numlist *, const enum number_t);
void numlist_push(struct numlist *, const enum number_t);
void numlist_grow(struct numlist *);
void numlist_release(struct numlist *);
size_t numlist_print(struct numlist *);

#endif /* NUMLIST_H */
