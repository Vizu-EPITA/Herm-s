#ifndef GRAPH_H
#define GRAPH_H

struct Node
{
    int ID;
    struct Node* next;
};

struct Graph
{
    int order;
    struct Node** adjLists;
    struct Node** prevLists;
};

struct Node* newNode(int ID);

struct Graph* createGraph(int order);

void addEdge(struct Graph* graph, int src, int dest);

void freeGraph(struct Graph*);

void printGraph(struct Graph* graph);





#endif
