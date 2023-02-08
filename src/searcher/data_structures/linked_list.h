#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct list
{
    void *value;
    struct list *prev;
    struct list *next;
};

struct list *init_list(void *value);

void add_end_list(void *value, struct list *list);

void delete_list(struct list *list);

void delete_node(str uct list *node);


#endif
