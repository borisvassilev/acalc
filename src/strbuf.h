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

#ifndef STRBUF_H
#define STRBUF_H

#define STRBUF_DEFAULT_ALLOC 2<<10

struct strbuf_t {
    char *str;
    size_t alloc;
    size_t len;
};

void strbuf_init(struct strbuf_t *, const size_t);
void strbuf_reinit(struct strbuf_t *, const size_t);
void strbuf_free(struct strbuf_t *);
void strbuf_grow(struct strbuf_t *, const size_t);
void strbuf_putc(struct strbuf_t *, const char);
void strbuf_terminate(struct strbuf_t *);
void strbuf_reset(struct strbuf_t *);

#endif /* STRBUF_H */
