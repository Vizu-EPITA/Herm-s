#ifndef SEARCHER
#define SEARCHER

#include "../../tools/hash_table.h"
#include "../../tools/graph.h"
#include "../../tools/linked_list.h"
#include "../../tools/forward_table.h"
#include "../../tools/inverted_table.h"
#include <err.h>
#include <pthread.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

LinkedList *get_word_list(char *query);

double get_ten_rank(int32_t *tenRankIndexArray, size_t index, int32_t *docIdArray, size_t nbAdded, double formerMax, Graph *graph);

char **search_query(char *query, ForwardTable *forward, HashTable *table_wordId, InvertedTable *inverted,Graph *graph);
#endif
