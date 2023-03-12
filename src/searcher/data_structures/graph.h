#ifndef GRAPH_H
#define GRAPH_H

struct Node
{
    int ID;
    int docID;
    struct Node** adjList;
    int nbAdj;
    int adjListSize;
    struct Node** prevList;
    int nbPrev;
    int prevListSize;
    double pageRank;
};

struct Graph
{
    int order;
    struct Node** nodes;
    int sizeNodesList;
};

struct Node* newNode(int ID, int docId);

struct Graph* graphInit(int order);

void addNode(struct Graph* graph, int docID);

void addEdge(struct Graph* graph, struct Node* src, struct Node* dest);

void freeGraph(struct Graph*);

void printGraph(struct Graph* graph);





#endif
