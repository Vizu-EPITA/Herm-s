#ifndef SEARCHER
#define SEARCHER

#include "../../tools/hash_table.h"
#include "./data_structures/graph.h"
#include "./data_structures/shared_queue.h"
#include "./data_structures/linked_list.h"
#include <err.h>
#include <pthread.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

linked_list *get_word_list(char *query);

void search_query(char *query, HashTable *table_docId, HashTable *table_wordId, HashTable *table_inverted,Graph *graph);
#endif
