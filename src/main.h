#ifndef MAIN_H
#define MAIN_H

#include "../include/crawler/url_server.h"
#include "../include/crawler/repository.h"
#include "../tools/hash_table.h"
#include "../tools/inverted_table.h"
#include "../tools/forward_table.h"

typedef struct thread_data
{
	unsigned long limit;
	HashTable *table_docID;
	HashTable *table_wordID;
	ForwardTable *ftable_docID;
	InvertedTable *table_inverted;
	URLQueue *queue_url;
	FileQueue *queue_file;
	struct Graph *graph;
} thread_data;

#endif
