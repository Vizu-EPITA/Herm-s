#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "data_structures/graph.h"

//Initializes the pagerank with 1/order for all nodes
//HAS TO USE FLOATS
void initRank(struct Graph* graph)
{
    for(int node = 0; node < graph->order; node++)
    {
        graph->nodes[node]->pageRank = 1.0f/(double)graph->order;
    }
}

void rank(struct Graph* graph, int cycle)
{
    struct Node* curNode;
    struct Node* prevNode;
    double damp = 0.85f;
    double sum;
    for(;cycle > 0; cycle --)
    {
        for(int node = 0; node < graph->order; node++)
        {
            curNode = graph->nodes[node];
            sum = 0;
            for(int prev = 0; prev < curNode->nbPrev; prev++)
            {
                prevNode = curNode->prevList[prev];
                sum += (double)prevNode->pageRank/(double)prevNode->nbAdj;
            }
            curNode->pageRank = (1.0f - damp) + damp * sum;
        }
    }
}

void printRank(struct Graph* graph)
{
    for(int node = 0; node < graph->order; node++)
    {
        printf("The page rank value of (%i) is [%lf] \n",
                node, graph->nodes[node]->pageRank);
    }
}

int main()
{
    struct Graph* graph = graphInit(5);
    addEdge(graph, graph->nodes[0], graph->nodes[3]);
    addEdge(graph, graph->nodes[1], graph->nodes[3]);
    addEdge(graph, graph->nodes[2], graph->nodes[3]);
    addEdge(graph, graph->nodes[3], graph->nodes[4]);

    printGraph(graph);

    initRank(graph);
    printf("\n\n");

    rank(graph, 1);
    printRank(graph);
    printf("\n\n");

    rank(graph, 5);
    printRank(graph);
    printf("\n\n");

    rank(graph, 50);
    printRank(graph);

    freeGraph(graph);

    return 0;
}
