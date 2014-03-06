#ifndef STATE_H
#define STATE_H

#include <stddef.h>

#include "numstack.h"

struct state_t {
    struct numstack_t ns;
    size_t real_output_prec;
};

void state_init(struct state_t *);
void state_release(struct state_t *);

#endif /* STATE_H */
