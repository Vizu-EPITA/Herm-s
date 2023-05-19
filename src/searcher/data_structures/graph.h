#ifndef GRAPH_H
#define GRAPH_H

struct Node
{
    int ID;
    struct Node **adjList;
    int nbAdj;
    int adjListSize;
    struct Node **prevList;
    int nbPrev;
    int prevListSize;
    double pageRank;
};

typedef struct Graph
{
    int order;
    struct Node **nodes;
    int sizeNodesList;
} Graph;

struct Node *newNode(int ID);

struct Graph *graphInit(int order);

void addNode(struct Graph *graph);

void addEdge(struct Graph *graph, struct Node *src, struct Node *dest);

struct Node *findOrCreateNode(struct Graph *graph, int docID);

void freeGraph(struct Graph *graph);

void printGraph(struct Graph *graph);

void saveGraph(struct Graph *graph);

struct Graph *loadGraph(char *filepath);

struct Graph *linkFromFile(char *filepath);

#endif
