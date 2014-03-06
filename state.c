#include "state.h"

void state_init(struct state_t *s)
{
    s->real_output_prec = 5;
    numstack_init(&(s->ns));
}

void state_release(struct state_t *s)
{
    numstack_release(&(s->ns));
}
