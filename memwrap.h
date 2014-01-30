#ifndef MEMWRAP_H
#define MEMWRAP_H

#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>

static void die(const char *err)
{
    fprintf(stderr, "%s\n", err);
    exit(1);
}

static void *xmalloc(size_t size)
{
    void *ret = malloc(size);
    if (!ret && !size)
        ret = malloc(1);
    if (!ret)
        die("Out of memory");
    return ret;
}

static void *xrealloc(void *ptr, size_t size)
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

#define alloc_nr(x) (((x)+16)*3/2)

#endif /* MEMWRAP_H */
