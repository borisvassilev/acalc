#include <stddef.h>

#include "numstack.h"
#include "strbuf.h"

#define DEFAULT_REAL_PRECISION 5

struct numstack_t global_stack;
size_t real_precision;
struct strbuf_t iobuf;

void globals_init()
{
    numstack_init(&global_stack);
    real_precision = DEFAULT_REAL_PRECISION;
    strbuf_init(&iobuf, 0);
}

void globals_release()
{
    numstack_release(&global_stack);
    strbuf_free(&iobuf);
}
