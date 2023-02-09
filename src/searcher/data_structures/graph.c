#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include "graph.h"

// Initializes a new node and returns it. Is
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

    newNode->nbAdj = 0;
    newNode->adjListSize = 5;
    return newNode;
}

struct Graph* graphInit(int order)
{
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL)
        err(1, "graph.c: something went wrong while creating the graph");

    graph->order = order;

    graph->nodes = malloc(sizeof(struct Node*) * order);
    if (graph->nodes == NULL)
        err(1, "graph.c: something went wrong while creating the graph");
    for (int i = 0; i < order; i++)
    {
        graph->nodes[i] = newNode(i);
    }

    graph->sizeNodesList = order;

  return graph;
}

// Different from newNode(). Creates a new node and adds it into the graph
void addNode(struct Graph* graph)
{
    struct Node* addedNode = malloc(sizeof(struct Node));
    if (addedNode == NULL)
        err(1, "graph.c: something went wrong while adding a node");

    addedNode->ID = graph->order;
    graph->order += 1;
    // 5 is used as an arbitrary number. If the value is exceeded, it will
    // be realloced bit twice itself
    addedNode->adjList = malloc(sizeof(struct Node) * 5);
    if (addedNode->adjList == NULL)
        err(1, "graph.c: something went wrong while adding a node");
    addedNode->nbAdj = 0;
    addedNode->adjListSize = 5;

    if(graph->order == graph->sizeNodesList)
    {
        graph->nodes = realloc(graph->nodes, (sizeof(struct Node*) * graph->order * 2));
        if (graph->nodes == NULL)
            err(1, "graph.c: something went wrong while realloc to add a node");
    }

    graph->nodes[graph->order] = addedNode;
    graph->order += 1;
    graph->sizeNodesList = graph->sizeNodesList * 2;
}

void addEdge(struct Graph* graph, struct Node* src, struct Node* dest)
{
    if(src->ID >= graph->order || dest->ID >= graph->order)
        err(1, "graph.c: trying to add an edge with a node not in the graph");
    struct Node* test;
    for (int i = 0; i < src->nbAdj; i++)
    {
        test = src->adjList[i];
        if (test == dest)
            return;
    }
    if (src->nbAdj == src->adjListSize)
        src->adjList = realloc(src->adjList, (sizeof(struct Node*) * src->adjListSize * 2));
    if (src->adjList == NULL)
        err(1, "graph.c: something went wrong while realloc to add an edge");
    src->adjList[src->nbAdj] = dest;

    src->nbAdj += 1;
    src->adjListSize = src->adjListSize * 2;
}

 void freeGraph(struct Graph* graph)
{
    struct Node* destroyer;
    for (int i = 0; i < graph->order; i++)
    {
        destroyer = graph->nodes[i];
        free(destroyer->adjList);
        free(destroyer);
    }
    free(graph->nodes);
    free(graph);
}


void printGraph(struct Graph* graph)
{
    printf("The graph has %i vertices. Here they are with their neighbours\n", graph->order);
    for (int iD = 0; iD < graph->order; iD++)
    {
        struct Node* temp = graph->nodes[iD];
        struct Node* temp2;
        printf("\n  (%d): ", iD);
        for (int i = 0; i < temp->nbAdj; i++)
        {
            temp2 = temp->adjList[i];
            printf(" %d |", temp2->ID);
        }
    }
    printf("\n");
}


