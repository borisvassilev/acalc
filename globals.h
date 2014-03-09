#ifndef GLOBALS_H
#define GLOBALS_H

#include <stddef.h>

#include "strbuf.h"

extern struct numstack_t global_stack;
extern size_t decfrac_prec;
extern struct strbuf_t iobuf;

void globals_init();
void globals_release();

#endif /* GLOBALS_H */
