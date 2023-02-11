#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "data_structures/graph.h"

//Initializes the pagerank with 1/order for all nodes
//HAS TO USE FLOATS
void initRank(struct Graph* graph)
{
    for (int node = 0; node < graph->order; node++)
    {
        graph->nodes[node]->pageRank = 1.0f/(double)graph->order;
    }
}

int main()
{
    struct Graph* graph = graphInit(4);
    addEdge(graph, graph->nodes[0], graph->nodes[1]);
    addEdge(graph, graph->nodes[0], graph->nodes[2]);
    addEdge(graph, graph->nodes[1], graph->nodes[2]);
    addEdge(graph, graph->nodes[2], graph->nodes[0]);
    addEdge(graph, graph->nodes[1], graph->nodes[0]);
    addEdge(graph, graph->nodes[3], graph->nodes[0]);


    initRank(graph);
    printGraph(graph);

    return 0;
}
