#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "graph.h"

struct Node* newNode(int ID)
{
    struct Node* newNode = malloc(sizeof(struct Node));
    if (newNode == NULL)
        err(1, "graph.c: something went wrong while creating a node");

    newNode->ID = ID;
    // 5 is used as an arbitrary number. If the value is exceeded, it will
    // be realloced bit twice itself
    newNode->adjList = malloc(sizeof(struct Node) * 5);
    if (newNode->adjList == NULL)
        err(1, "graph.c: something went wrong while creating a node");

    return newNode;
}

struct Graph* createGraph(int order)
{
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL)
        err(1, "graph.c: something went wrong while creating the graph");

    graph->order = order;

    for (int i = 0; i < order; i++)
    {
        graph->nodes[i] = newNode(i);
    }

  return graph;
}

// HAS TO MANAGE THE PREVLIST
void addEdge(struct Graph* graph, int src, int dest)
{
    struct Node* tempNode = newNode(dest);
    tempNode->next = graph->adjLists[src];
    graph->adjLists[src] = tempNode;

}

void freeGraph(struct Graph*)
{

}


void printGraph(struct Graph* graph)
{
    printf("The graph has %i vertices. Here they are with their neighbours\n", graph->order);
    for (int iD = 0; iD < graph->order; iD++)
    {
        struct Node* temp = graph->adjLists[iD];
        printf("\n  (%d): ", iD);
        while (temp)
        {
            printf(" %d |", temp->ID);
            temp = temp->next;
        }
    }
    printf("\n");
}


