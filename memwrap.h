#ifndef MEMWRAP_H
#define MEMWRAP_H

#include <stddef.h>

#define alloc_nr(x) (((x)+16)*3/2)

void die(const char *);
void *xmalloc(size_t);
void *xrealloc(void *, size_t);

#endif /* MEMWRAP_H */
