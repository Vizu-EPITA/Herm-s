#include <unistd.h>
#include <fcntl.h>
#include <err.h>
#include <stdio.h>
#include "../../tools/graph.h"

int main()
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
    if ((fd = open("links.txt", O_RDONLY)) == -1)
        err(1, "links.c: Could not open the link file");
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
        err(1, "links.c: Trouble reading the link file");
    printGraph(graph);
	saveGraph(graph);
	freeGraph(graph);
    return 0;
}
