/*
Copyright Boris Vassilev 2015

This file is part of acalc.

acalc is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

acalc is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with acalc.  If not, see <http://www.gnu.org/licenses/>.
*/

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
