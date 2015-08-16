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

#include <stddef.h>

#include "numstack.h"
#include "strbuf.h"

#define DEFAULT_DECFRAC_PRECISION 5

struct numstack_t global_stack;
size_t decfrac_prec;
struct strbuf_t iobuf;

void globals_init()
{
    numstack_init(&global_stack);
    decfrac_prec = DEFAULT_DECFRAC_PRECISION;
    strbuf_init(&iobuf, 0);
}

void globals_release()
{
    numstack_release(&global_stack);
    strbuf_free(&iobuf);
}
