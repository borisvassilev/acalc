#include <stddef.h>
#include "numlist.h"
#include "memwrap.h"

#include "numstack.h"

void numstack_init(struct numstack **ns)
{
    *ns = (struct numstack *) xmalloc(sizeof (struct numstack));
    (*ns)->len = 0;
    (*ns)->top = (*ns)->bot = NULL;
}

void numstack_release(struct numstack *ns)
{
    struct numlist *nl;
    while (ns->bot != NULL) {
        numstack_pop_bottom(ns, &nl);
        numlist_print(nl);
        numlist_release(nl);
    }
    free(ns);
}

void numstack_top(struct numstack *ns, struct numlist **nl)
{
    *nl = NULL;
    if (ns->top != NULL)
        *nl = ns->top->nl;
}

void numstack_push(struct numstack *ns, struct numlist *nl)
{
    struct nsel *new = (struct nsel *) xmalloc(sizeof (struct nsel));
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

void numstack_pop(struct numstack *ns, struct numlist **nl)
{
    *nl = NULL;
    if (ns->top != NULL) {
        *nl = ns->top->nl;
        struct nsel *popped = ns->top;
        ns->top = ns->top->next;
        if (ns->top != NULL)
            ns->top->prev = NULL;
        free(popped);
        --ns->len;
    }
}

void numstack_pop_bottom(struct numstack *ns, struct numlist **nl)
{
    *nl = NULL;
    if (ns->bot != NULL) {
        *nl = ns->bot->nl;
        struct nsel *popped = ns->bot;
        ns->bot = ns->bot->prev;
        if (ns->bot != NULL)
            ns->bot->next = NULL;
        free(popped);
        --ns->len;
    }
}
