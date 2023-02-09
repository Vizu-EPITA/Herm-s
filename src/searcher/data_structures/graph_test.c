#include "graph.h"

int main()
{
  int V = 5;
  struct Graph* graph = createGraph(V);
  addEdge(graph, 0, 1);
  addEdge(graph, 0, 2);
  addEdge(graph, 0, 3);
  addEdge(graph, 1, 2);

  printGraph(graph);

  return 0;
}
