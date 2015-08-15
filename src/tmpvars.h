#ifndef TMPVARS_H
#define TMPVARS_H

#include "gmp.h"

extern mpz_t mpz1, mpz2, mpz3;
extern mpq_t mpq1, mpq2, mpq3;

void tmpvars_init();
void tmpvars_clear();

#endif /* TMPVARS_H */
