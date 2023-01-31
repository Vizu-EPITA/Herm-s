#include <stddef.h>
#include <stdlib.h>
#include <err.h>
#include "linked_list.h"

// Initializes the list
struct list *init_list(void *value)
{
    struct list *node = malloc(sizeof(struct list));
    if (!node)
        err(1, "linked_list.c: init_list() couldn't malloc the first node");
    node->prev = NULL;
    node->next = NULL;
    node->value = value;
    return node;
}

// Create a new node with value and adds it at the end of the list
void add_end_list(void *value, struct list *list)
{
    struct list *new_node = malloc(sizeof(struct list));
    if (!new_node)
        err(1, "linked_list.c: add_end_list() couldn't malloc the node");
    new_node->value = value;
    while (list->next)
        list = list->next;
    list->next = new_node;
    new_node->prev = list;
    new_node->next = NULL;
}

/*
int is_list_empty(struct list *list)
{
    if (list->next == NULL && list->prev == NULL)
        return 1;
    return 0;
}
*/

// Delete and free all the list
void delete_list(struct list *list)
{
    // To get to the first node of the list :
    while (list->prev != NULL)
        list = list->prev;

    struct list *temp = list->next;
    while (temp != NULL)
    {
        free(list);
        list = temp;
        temp = temp->next;
    }
    free(list);
}

// /!\ : DO NOTE DELETE THE LAST NODE OF THE LIST, BUT THE NODE GIVEN
void delete_node(struct list *node)
{
    if (node->prev == NULL && node->next == NULL)
        free(node);
    else if (node->prev == NULL && node->next != NULL)
    {
        node->next->prev = NULL;
        free(node);
    }
    else if (node->prev != NULL && node->next == NULL)
    {
        node->prev->next = NULL;
        free(node);
    }
    else
    {
        node->next->prev = node->prev;
        node->prev->next = node->next;
        free(node);
    }
}
