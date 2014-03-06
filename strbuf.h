#ifndef STRBUF_H
#define STRBUF_H

#define STRBUF_DEFAULT_ALLOC 2<<10

struct strbuf_t {
    char *str;
    size_t alloc;
    size_t len;
};

void strbuf_init(struct strbuf_t *, const size_t);
void strbuf_reinit(struct strbuf_t *, const size_t);
void strbuf_free(struct strbuf_t *);
void strbuf_grow(struct strbuf_t *, const size_t);
void strbuf_putc(struct strbuf_t *, const char);
void strbuf_terminate(struct strbuf_t *);
void strbuf_reset(struct strbuf_t *);
size_t strbuf_len(struct strbuf_t *);
char *strbuf_str(struct strbuf_t *, size_t);

#endif /* STRBUF_H */
