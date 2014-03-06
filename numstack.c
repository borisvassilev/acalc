#include <stddef.h>
#include <stdlib.h>

#include "numlist.h"
#include "memwrap.h"

#include "numstack.h"

void numstack_init(struct numstack_t *ns)
{
    /*
    ns = (struct numstack_t *) xmalloc(sizeof (struct numstack_t));
    */
    ns->len = 0;
    ns->top = ns->bot = NULL;
}

void numstack_release(struct numstack_t *ns)
{
    struct numlist_t *nl;
    while (ns->bot != NULL) {
        numstack_pop_bottom(ns, &nl);
        numlist_print(nl);
        numlist_release(nl);
    }
    /*
    free(ns);
    */
}

void numstack_top(struct numstack_t *ns, struct numlist_t **nl)
{
    *nl = NULL;
    if (ns->top != NULL)
        *nl = ns->top->nl;
}

void numstack_push(struct numstack_t *ns, struct numlist_t *nl)
{
    struct nsel_t *new = (struct nsel_t *) xmalloc(sizeof (struct nsel_t));
    new->nl = nl;
    new->prev = NULL;
    new->next = ns->top;
    if (ns->top != NULL)
        ns->top->prev = new;
    ns->top = new;
    if (ns->bot == NULL)
        ns->bot = new;
    ++ns->len;
}

void numstack_pop(struct numstack_t *ns, struct numlist_t **nl)
{
    *nl = NULL;
    if (ns->top != NULL) {
        *nl = ns->top->nl;
        struct nsel_t *popped = ns->top;
        ns->top = ns->top->next;
        if (ns->top != NULL)
            ns->top->prev = NULL;
        free(popped);
        --ns->len;
    }
}

void numstack_pop_bottom(struct numstack_t *ns, struct numlist_t **nl)
{
    *nl = NULL;
    if (ns->bot != NULL) {
        *nl = ns->bot->nl;
        struct nsel_t *popped = ns->bot;
        ns->bot = ns->bot->prev;
        if (ns->bot != NULL)
            ns->bot->next = NULL;
        free(popped);
        --ns->len;
    }
}
