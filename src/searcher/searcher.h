#include "../../tools/hash_table.h"
#include "./data_structures/graph.h"
#include "./data_structures/shared_queue.h"
#include <err.h>
#include <pthread.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

void search_query(char *query, HashTable *table_docId, HashTable *table_wordId, Graph *graph);
