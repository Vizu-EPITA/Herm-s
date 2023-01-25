#include <stddef.h>
#include "linked_list.h"

struct list *init_list()
{
    struct list *nde;
    nde->prev = NULL;
    nde->next = NULL;
    return nde;
}

void add_end_list(struct list *new_nde, struct list *list)
{
    while (list->next)
        list = list->next;
    list->next = new_nde;
    new_nde->prev = list;
}
