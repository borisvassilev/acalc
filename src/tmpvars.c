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

