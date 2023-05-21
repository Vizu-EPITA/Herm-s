#include "main.h"
#include "../include/crawler/url_server.h"
#include "../include/crawler/repository.h"
#include "../include/indexer/indexer.h"
#include "../tools/hash_table.h"
#include "../include/crawler/crawler.h"
#include "../tools/graph.h"
#include "../tools/inverted_table.h"
#include "../tools/forward_table.h"
#include <pthread.h>
#include <err.h>

int main()//int argc, char *argv[])
{
	Graph *graph = graphInit(2);
	addNode(graph);
	return 0;
}
