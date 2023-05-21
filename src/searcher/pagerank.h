#ifndef PAGERANK_H
#define PAGERANK_H

void initRank(struct Graph* graph);

void rank(struct Graph* graph, int cycle);

void printRank(struct Graph* graph);

#endif
