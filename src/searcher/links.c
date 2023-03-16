#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <stdio.h>
#include "data_structures/graph.h"

struct Node *findOrCreateNode(struct Graph *graph, int docID)
{
    while (docID >= graph->order)
        addNode(graph);
    return graph->nodes[docID];
}

int main()
{
    /*
     * Issue when adding edges, might come from how I initialize the Graph
     *
     * Having -1 as docId for the first nodes after the init might fuck it up
     *
     * Also, I still need to figure when to save/load the graph and where. I
     * could use the same format as the links.txt file to save some work. Then
     * maybe instead of having to save the graph we could just put the
     * links.txt files together but maybe it wont fucking work
     *
     * Dont forget to delete the prints, but not now. They still remain useful
     */
    struct Graph *graph = graphInit(1);
    int fd;
    int ableToRead;
    int readFrom = 1;
    int fromId = 0;
    int toId = 0;
    struct Node *fromNode;
    struct Node *toNode;
    char character;
    if ((fd = open("links.txt", O_RDONLY)) == -1)
        err(1, "links.c: Could not open the link file");
    ableToRead = read(fd, &character, 1);
    while (ableToRead > 0)
    {
        printf("Char : %c\n", character);
        if (character == '|')
        {
            fromNode = findOrCreateNode(graph, fromId);
            readFrom = 0;
            ableToRead = read(fd, &character, 1);
            continue;
        }
        if (character == ',')
        {
            toNode = findOrCreateNode(graph, toId);
            printf("Linking %i to %i\n", fromId, toId);
            addEdge(graph, fromNode, toNode);
            toId = 0;
            ableToRead = read(fd, &character, 1);
            continue;
        }
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
        err(1, "links.c: Trouble reading the link file");
    printGraph(graph);
    freeGraph(graph);
    return 0;
}
