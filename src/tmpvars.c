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
#include <stdarg.h>
#include "gmp.h"

mpz_t mpz1, mpz2, mpz3;
mpq_t mpq1, mpq2, mpq3;

void tmpvars_init()
{
    mpz_inits(mpz1, mpz2, mpz3, NULL);
    mpq_inits(mpq1, mpq2, mpq3, NULL);
}

void tmpvars_clear()
{
    mpz_clears(mpz1, mpz2, mpz3, NULL);
    mpq_clears(mpq1, mpq2, mpq3, NULL);
}

