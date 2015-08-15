#include <stdio.h>
#include <stdlib.h>

#include "memwrap.h"

void die(const char *err)
{
    fprintf(stderr, "%s\n", err);
    exit(1);
}

void *xmalloc(size_t size)
{
    void *ret = malloc(size);
    if (!ret && !size)
        ret = malloc(1);
    if (!ret)
        die("Out of memory");
    return ret;
}

void *xrealloc(void *ptr, size_t size)
{
    void *ret;

    ret = realloc(ptr, size);
    if (!ret && !size)
        ret = realloc(ptr, 1);
    if (!ret) {
        die("Out of memory, realloc failed");
    }
    return ret;
}
