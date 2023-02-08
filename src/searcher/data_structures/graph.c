#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "graph.h"
#include "linked_list.h"

struct graph *init_graph(struct graph_node *head)
{
    struct graph *graph = malloc(sizeof(struct graph));
    if (!graph)
        err(1, "graph.c: init_graph() couldn't malloc the graph");
    graph->order = 1;
    graph->head = head;
    return graph;
}
