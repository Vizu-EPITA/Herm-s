#include "searcher.h"

linked_list *get_word_list(char *query)
{
	linked_list *list = malloc(sizeof(linked_list));
	if (list == NULL) errx(1, "searcher.c: Could not create the wordlist");
	list->word = NULL; // Header
	list->wordId = -1;
	linked_list *iterator = list;
	size_t buffer_size = 100;
	char *buffer = malloc(sizeof(char)*buffer_size);
	if (buffer == NULL) errx(1, "searcher.c: Could not create a buffer to get the wordlist");
	size_t i = 0;
	size_t count = 0;
	while (query[i] != 0)
	{
		if (query[i] == ' ' && count != 0)
		{
			iterator->next = malloc(sizeof(linked_list));
			if (iterator->next == NULL) errx(1, "searcher.c: Could not create the wordlist");
			iterator = iterator->next;
			iterator->next = NULL;
			iterator->word = malloc(sizeof(char)*count+1);
			if (iterator->word == NULL) errx(1, "searcher.c: Could not create the wordlist");
			for (int j = 0; j < count; j++)
			{
				iterator->word[j] = buffer[j];
			}
			iterator->word[count] = 0;
			count = 0;
		}
		else if (query[i] != ' ')
		{
			if (query[i] >= 'A' && query[i] <= 'Z')
			{
				buffer[count] = query[i]+32;
			}
			else
			{
				buffer[count] = query[i];
			}
			count++;
			if (count == buffer_size)
			{
				buffer_size = buffer_size*2;
				buffer = realloc(buffer, sizeof(char)*buffer_size);
			}
		}
		i++;
	}
	free(buffer);
	return list;
}

void search_query(char *query, HashTable *table_docId, HashTable *table_wordId, HashTable *table_inverted, Graph *graph)
{
	linked_list *wordlist = get_word_list(query);
	linked_list *iterator = wordlist->next;
	while (iterator != NULL)
	{
		iterator->wordId = ht_search(table_wordId, iterator->word);
		iterator = iterator->next;
	}
}
