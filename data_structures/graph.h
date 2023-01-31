#ifndef GRAPH_H
#define GRAPH_H

struct graph
{
    struct graph_node *head;
    int order;
};

struct graph_node
{
    void *value;
    int id;
    struct list *adj_list;
};

#endif
