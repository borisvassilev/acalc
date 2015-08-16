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

#ifndef NUMSTACK_H
#define NUMSTACK_H

struct nsel_t {
    struct numlist_t *nl;
    struct nsel_t *prev;
    struct nsel_t *next;
};

struct numstack_t {
    size_t len;
    struct nsel_t *top;
    struct nsel_t *bot;
};

void numstack_init(struct numstack_t *);
void numstack_release(struct numstack_t *);
void numstack_top(struct numstack_t *, struct numlist_t **);
void numstack_push(struct numstack_t *, struct numlist_t *);
void numstack_pop(struct numstack_t *, struct numlist_t **);
void numstack_pop_bot(struct numstack_t *, struct numlist_t **);

#endif /* NUMSTACK_H */
