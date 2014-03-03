#ifndef NUMSTACK_H
#define NUMSTACK_H

struct nsel {
    struct numlist *nl;
    struct nsel *prev;
    struct nsel *next;
};

struct numstack {
    size_t len;
    struct nsel *top;
    struct nsel *bot;
};

void numstack_init(struct numstack **);
void numstack_release(struct numstack *);
void numstack_top(struct numstack *, struct numlist **);
void numstack_push(struct numstack *, struct numlist *);
void numstack_pop(struct numstack *, struct numlist **);
void numstack_pop_bottom(struct numstack *, struct numlist **);

#endif /* NUMSTACK_H */
