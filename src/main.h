#ifndef MAIN_H
#define MAIN_H

#include "../include/crawler/url_server.h"
#include "../include/crawler/repository.h"
#include "../tools/hash_table.h"

typedef struct thread_data
{
	HashTable *table_docID;
	HashTable *table_wordID;
	URLQueue *queue_url;
	FileQueue *queue_file;
	struct Graph *graph;
} thread_data;

#endif
