#include <stdio.h>
#include <stdlib.h>
#include <err.h>
#include <unistd.h>
#include <fcntl.h>
#include "graph.h"

// Initializes a new node and returns it.
struct Node* newNode(int ID)
{
    struct Node* newNode = malloc(sizeof(struct Node));
    if (newNode == NULL)
        errx(1, "graph.c: something went wrong while creating a node");

    newNode->ID = ID;
    // 5 is used as an arbitrary number. If the value is exceeded, it will
    // be realloced bit twice itself
    newNode->adjList = malloc(sizeof(struct Node) * 5);
    if (newNode->adjList == NULL)
        errx(1, "graph.c: something went wrong while creating a node");
    newNode->prevList = malloc(sizeof(struct Node) * 5);
    if (newNode->prevList == NULL)
        errx(1, "graph.c: something went wrong while creating a node");

    newNode->nbAdj = 0;
    newNode->adjListSize = 5;
    newNode->nbPrev = 0;
    newNode->prevListSize = 5;
	newNode->pageRank = 0;
    return newNode;
}

// Pretty straight forward, initializes the graph.
struct Graph* graphInit(int order)
{
    struct Graph* graph = malloc(sizeof(struct Graph));
    if (graph == NULL)
        errx(1, "graph.c: something went wrong while creating the graph");

    graph->order = order;

    graph->nodes = malloc(sizeof(struct Node*) * order);
    if (graph->nodes == NULL)
        errx(1, "graph.c: something went wrong while creating the graph");
    // Setting up the nodes
    for (int i = 0; i < order; i++)
    {
        graph->nodes[i] = newNode(i);
    }

    graph->sizeNodesList = order;

  return graph;
}

// Different from newNode(). Creates a new node and adds it into the graph.
void addNode(struct Graph* graph)
{
    struct Node* addedNode = newNode(graph->order);
    // If we want to add a node while the array is full, resize by twice its size
    if(graph->order == graph->sizeNodesList)
    {
        graph->nodes = realloc(graph->nodes, (sizeof(struct Node*) * graph->order * 2));
        if (graph->nodes == NULL)
            errx(1, "graph.c: something went wrong while realloc to add a node");
        graph->sizeNodesList = graph->sizeNodesList * 2;
    }

    graph->nodes[graph->order] = addedNode;
    graph->order += 1;
}

// Again, obviously creates an edge between the two nodes given
void addEdge(struct Graph* graph, struct Node* src, struct Node* dest)
{
    if(src->ID >= graph->order || dest->ID >= graph->order)
        errx(1, "graph.c: trying to add an edge with a node not in the graph");
    struct Node* test;

	if(src->ID == dest->ID)
		return;

    // Checks if the edge already exists. If so, return
    for (int i = 0; i < src->nbAdj; i++)
    {
        test = src->adjList[i];
        if (test == dest)
            return;
    }

    // We now have to update the adjlist and the prevlist of the nodes.
    // If they are full, resize by twice, as usual
    if (src->nbAdj == src->adjListSize)
    {
        src->adjList = realloc(src->adjList, (sizeof(struct Node*) * src->adjListSize * 2));
        if (src->adjList == NULL)
            errx(1, "graph.c: something went wrong while realloc to add an edge");
        src->adjListSize = src->adjListSize * 2;
    }

    src->adjList[src->nbAdj] = dest;
    src->nbAdj += 1;

    if (dest->nbPrev == dest->prevListSize)
    {
        dest->prevList = realloc(dest->prevList, (sizeof(struct Node*) * dest->prevListSize * 2));
        if (dest->prevList == NULL)
            errx(1, "graph.c: something went wrong while realloc to add an edge");
        dest->prevListSize = dest->prevListSize * 2;
    }

    dest->prevList[dest->nbPrev] = src;
    dest->nbPrev += 1;
}

// Return the node which contains the given docID. If it dont exists,
// creates all the nodes with a docID inferior to his, creates the said node
// and returns it.
struct Node *findOrCreateNode(struct Graph *graph, int docID)
{
    while (docID >= graph->order)
        // Create all the missing nodes
        addNode(graph);
    return graph->nodes[docID];
}

// Simple free fct
void freeGraph(struct Graph* graph)
{
    struct Node* destroyer;
    for (int i = 0; i < graph->order; i++)
    {
        destroyer = graph->nodes[i];
        free(destroyer->adjList);
        free(destroyer->prevList);
        free(destroyer);
    }
    free(graph->nodes);
    free(graph);
}

// Simple print fct
void printGraph(struct Graph* graph)
{
    printf("The graph has %i vertices. Here they are with their adjacents:\n", graph->order);
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

    printf("\n\nAnd here are the predecessors:\n");
    for (int iD = 0; iD < graph->order; iD++)
    {
        struct Node* temp = graph->nodes[iD];
        struct Node* temp2;
        printf("\n  (%d): ", iD);
        for (int i = 0; i < temp->nbPrev; i++)
        {
            temp2 = temp->prevList[i];
            printf(" %d |", temp2->ID);
        }
    }
    printf("\n");
}

void saveGraph(struct Graph *graph)
{
	FILE *fptr = fopen("graphsave.txt", "w");
	if (fptr == NULL)
		errx(1, "graph.c: could not create a saving file");
	fprintf(fptr, "%i\n", graph->order);
	struct Node *current;
	for (int i = 0; i < graph->order; i++)
	{
		current = graph->nodes[i];
		fprintf(fptr, "%i %i %lf | ", current->ID, current->nbAdj, current->pageRank);
		for (int j = 0; j < current->nbAdj; j++)
			fprintf(fptr, "%i ", current->adjList[j]->ID);
		//fprintf(fptr, "| ");
		//for (int k = 0; k < current->nbPrev; k++)
		//	fprintf(fptr, "%i ", current->prevList[k]->ID);
		fprintf(fptr, "\n");
	}
	fclose(fptr);
}

struct Graph *loadGraph(char *filepath)
{
	FILE *fptr = fopen(filepath, "r");
	if (fptr == NULL)
		errx(1, "graph.c: could not create a saving file");
	int scanVal;
	int order;
	int Id;
	int nbAdj;
	int adj;
	double pageRank;
	char c;
	if (fscanf(fptr, "%i", &order) == 0)
		errx(1, "graph.c: Somethine went wrong while loading the order");
	struct Graph *graph = graphInit(order);
	while (!feof(fptr))
	{
		scanVal = fscanf(fptr, "%i %i %lf", &Id, &nbAdj, &pageRank);
		if (scanVal == 0)
			errx(1, "graph.c: Somethine went wrong while loading the Id and Pagerank");
		if (scanVal == EOF)
			break;
		graph->nodes[Id]->pageRank = pageRank;
		while ((c = fgetc(fptr)) != '|' && !feof(fptr))
		{
			continue;
		}
		if (feof(fptr))
			break;
		while (nbAdj > 0)
		{
			scanVal = fscanf(fptr, "%i", &adj);
			if (scanVal == 0)
				errx(1, "graph.c: Somethine went wrong while loading the Id and Pagerank");
			if (scanVal == EOF)
				break;
			addEdge(graph, graph->nodes[Id], graph->nodes[adj]);
			nbAdj--;
		}
	}
	return graph;
}

struct Graph *linkFromFile(char *filepath)
{
	// Has to initialize the graph before starting, call one time only
    struct Graph *graph = graphInit(1);
    int fd;
    int ableToRead;
    //Simple bool to state if we are readind the from ID (1) or to ID (!= 1)
    int readFrom = 1;
    int fromId = 0;
    int toId = 0;
    struct Node *fromNode;
    struct Node *toNode;
    char character;
    if ((fd = open(filepath, O_RDONLY)) == -1)
        err(1, "graph.c: Could not open the link file");
    ableToRead = read(fd, &character, 1);
    while (ableToRead > 0)
    {
        // The "from node" has been parsed
        if (character == '|')
        {
            fromNode = findOrCreateNode(graph, fromId);
            readFrom = 0;
            ableToRead = read(fd, &character, 1);
            continue;
        }
        // The "to node" has been parsed, proceeds to link them
        if (character == ',')
        {
            toNode = findOrCreateNode(graph, toId);
            printf("Linking %i to %i\n", fromId, toId);
            addEdge(graph, fromNode, toNode);
            toId = 0;
            ableToRead = read(fd, &character, 1);
            continue;
        }
        // Same as before but resets the IDs since its the end of the line
        if (character == '\n')
        {
            toNode = findOrCreateNode(graph, toId);
            printf("Linking %i to %i\n", fromId, toId);
            addEdge(graph, fromNode, toNode);
            readFrom = 1;
            fromId = 0;
            toId = 0;
            ableToRead = read(fd, &character, 1);
            continue;
        }
        if (readFrom == 1)
        {
            fromId = fromId * 10 ;
            fromId += character - '0';
        }
        else
        {
            toId = toId * 10;
            toId += character - '0';
        }
        ableToRead = read(fd, &character, 1);
    }

    if (ableToRead == -1)
	{
        err(1, "graph.c: Trouble reading the link file");
	}
	return graph;
}
