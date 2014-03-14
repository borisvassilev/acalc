#include <stdlib.h>

#include "memwrap.h"

#include "strbuf.h"

void strbuf_init(struct strbuf_t *sb, const size_t hint)
{
    sb->alloc = hint ? hint + 1 : STRBUF_DEFAULT_ALLOC;
    sb->str = (char *) xmalloc(sb->alloc * sizeof (char));
    sb->len = 0;
    sb->str[0] = '\0';
}

void strbuf_reinit(struct strbuf_t *sb, const size_t hint)
{
    size_t new_alloc = hint ? hint + 1 : STRBUF_DEFAULT_ALLOC;

    if (sb->alloc > new_alloc) {
        sb->alloc = new_alloc;
        xrealloc(sb->str, sb->alloc * sizeof (char));
    }
    sb->len = 0;
    sb->str[0] = '\0';
}

void strbuf_free(struct strbuf_t *sb)
{
    free(sb->str);
}

void strbuf_grow(struct strbuf_t *sb, const size_t hint)
{
    if (hint && hint < sb->alloc)
        return;

    sb->alloc = hint ? hint + 1: alloc_nr(sb->alloc);
    sb->str = xrealloc(sb->str, sb->alloc * sizeof (char));
}

void strbuf_putc(struct strbuf_t *sb, const char c)
{
    if (sb->len == sb->alloc - 1)
        strbuf_grow(sb, 0);

    sb->str[sb->len++] = c;
}

void strbuf_terminate(struct strbuf_t *sb)
{
    sb->str[sb->len++] = '\0';
}

void strbuf_reset(struct strbuf_t *sb)
{
    sb->len = 0;
    sb->str[0] = '\0';
}

char *strbuf_str(struct strbuf_t *sb, size_t offset)
{
    if (offset > sb->len - 1)
        offset = sb->len - 1;

    return sb->str + offset;
}
