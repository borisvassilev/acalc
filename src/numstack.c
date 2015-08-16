/*
Copyright Boris Vassilev 2015

This file is part of acalc.

acalc is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

acalc is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with acalc.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stddef.h>
#include <stdlib.h>

#include "numlist.h"
#include "memwrap.h"

#include "numstack.h"

void numstack_init(struct numstack_t *ns)
{
    ns->len = 0;
    ns->top = ns->bot = NULL;
}

void numstack_release(struct numstack_t *ns)
{
    struct numlist_t *nl;
    while (ns->bot != NULL) {
        numstack_pop_bot(ns, &nl);
        numlist_print(nl);
        numlist_release(nl);
    }
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

    if (new->next != NULL)
        new->next->prev = new;
    else
        ns->bot = new;

    ns->top = new;
    ++ns->len;
}

void numstack_pop(struct numstack_t *ns, struct numlist_t **nl)
{
    *nl = NULL;
    if (ns->top == NULL)
        return;

    struct nsel_t *top = ns->top;
    *nl = top->nl;
    ns->top = top->next;

    if (top->next != NULL)
        top->next->prev = NULL;
    else
        ns->bot = NULL;

    free(top);
    --ns->len;
}

void numstack_pop_bot(struct numstack_t *ns, struct numlist_t **nl)
{
    *nl = NULL;
    if (ns->bot == NULL)
        return;

    struct nsel_t *bot = ns->bot;
    *nl = bot->nl;
    ns->bot = bot->prev;

    if (bot->prev != NULL)
        bot->prev->next = NULL;
    else
        ns->top = NULL;

    free(bot);
    --ns->len;
}
