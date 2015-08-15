#ifndef NUMSTACK_H
#define NUMSTACK_H

struct nsel_t {
    struct numlist_t *nl;
    struct nsel_t *prev;
    struct nsel_t *next;
};

struct numstack_t {
    size_t len;
    struct nsel_t *top;
    struct nsel_t *bot;
};

void numstack_init(struct numstack_t *);
void numstack_release(struct numstack_t *);
void numstack_top(struct numstack_t *, struct numlist_t **);
void numstack_push(struct numstack_t *, struct numlist_t *);
void numstack_pop(struct numstack_t *, struct numlist_t **);
void numstack_pop_bot(struct numstack_t *, struct numlist_t **);

#endif /* NUMSTACK_H */
