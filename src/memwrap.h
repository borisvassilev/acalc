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

#ifndef MEMWRAP_H
#define MEMWRAP_H

#include <stddef.h>

#define alloc_nr(x) (((x)+16)*3/2)

void die(const char *);
void *xmalloc(size_t);
void *xrealloc(void *, size_t);

#endif /* MEMWRAP_H */
