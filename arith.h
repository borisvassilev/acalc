#ifndef ARITH_H
#define ARITH_H

#include "exit_status.h"

enum arithop_e {
    ADD,
    SUB,
    MUL,
    DIV
};

exit_status apply_arithop(enum arithop_e);

#endif /* ARITH_H */
