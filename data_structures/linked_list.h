#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct list
{
    void *value;
    struct list *prev;
    struct list *next;
};

#endif
