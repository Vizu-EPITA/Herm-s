#include <stdio.h>
#include "graph.h"

int main()
{

    printf("Let's create a web pages network graph !\n");
    printf("How many web pages will be in it ?\n-> ");
    int order;
    scanf("%i", &order);
    struct Graph* graph = graphInit(order);
    printf("\nNow, let's link them ! You will have to: \n");
    printf("- tell me the first node to link\n- press enter, \n- then the second one\n");
    printf("If you added enough, just enter 0 and 0\n\n");
    int node1;
    int node2;
    while(1)
    {
        scanf("%i", &node1);
        scanf("%i", &node2);
        if (node1 == 0 && node2 == 0)
            break;
        addEdge(graph, graph->nodes[node1], graph->nodes[node2]);
        printf("\nEnter a new couple :\n");
    }

    printf("\nOk now lets see if it works :\n");
/*

    struct Graph* graph = graphInit(3);
    addEdge(graph, graph->nodes[0], graph->nodes[1]);
    addEdge(graph, graph->nodes[0], graph->nodes[2]);
    addEdge(graph, graph->nodes[1], graph->nodes[2]);
    addEdge(graph, graph->nodes[2], graph->nodes[0]);
*/

    printGraph(graph);

    addNode(graph);

    printf("The node (%i) has been added, and been linked to 0 and 1, and 2 to him\n", graph->order);
    int nodenb = graph->order-1;
    addEdge(graph, graph->nodes[nodenb], graph->nodes[0]);
    addEdge(graph, graph->nodes[nodenb], graph->nodes[1]);
    addEdge(graph, graph->nodes[2], graph->nodes[nodenb]);

    printGraph(graph);
    freeGraph(graph);

    return 0;
}
