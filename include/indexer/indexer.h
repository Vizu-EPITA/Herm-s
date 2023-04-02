#ifndef INDEXER_H
#define INDEXER_H

#include <stdint.h>
#include <stddef.h>
#include "../../src/searcher/data_structures/graph.h"

typedef struct htmlStruct
{
	int32_t docid;
	int32_t pagelen;
	char* page;
	int32_t urllen;
	char* url;
} htmlStruct;


void *indexer(void *arg);

void free_htmlstruct(htmlStruct *htmlInfo);

htmlStruct* decompress_file(int32_t file);

void parseText(htmlStruct *htmlInfo, HashTable *table_docID, URLQueue *queue_url, struct Graph *graph);

#endif
