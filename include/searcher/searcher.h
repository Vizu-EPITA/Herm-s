#ifndef SEARCHER
#define SEARCHER

#include "../../tools/hash_table.h"
#include "../../tools/graph.h"
#include "../../tools/linked_list.h"
#include <err.h>
#include <pthread.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

LinkedList *get_word_list(char *query);

double get_ten_rank(size_t *tenRankIndexArray, size_t index, int32_t *docIdArray, size_t nbAdded, double formerMax, Graph *graph);

char **search_query(char *query, HashTable *table_docId, HashTable *table_wordId, HashTable *table_inverted,Graph *graph);
#endif
