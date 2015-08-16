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

#include <stdio.h>
#include <stdlib.h>

#include "memwrap.h"

void die(const char *err)
{
    fprintf(stderr, "%s\n", err);
    exit(1);
}

void *xmalloc(size_t size)
{
    void *ret = malloc(size);
    if (!ret && !size)
        ret = malloc(1);
    if (!ret)
        die("Out of memory");
    return ret;
}

void *xrealloc(void *ptr, size_t size)
{
    void *ret;

    ret = realloc(ptr, size);
    if (!ret && !size)
        ret = realloc(ptr, 1);
    if (!ret) {
        die("Out of memory, realloc failed");
    }
    return ret;
}
