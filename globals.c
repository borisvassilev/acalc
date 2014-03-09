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
