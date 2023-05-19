#include "searcher.h"

linked_list *get_word_list(char *query)
{
	linked_list *list = malloc(sizeof(linked_list));
	if (list == NULL) errx(1, "searcher.c: Could not create the wordlist");
	list->value = NULL; // Header
	linked_list *iterator = list;
	size_t buffer_size = 100;
	char *buffer = malloc(sizeof(char)*buffer_size);
	if (buffer == NULL) errx(1, "searcher.c: Could not create a buffer to get the wordlist");
	size_t i = 0;
	size_t count = 0;
	while (query[i] != 0)
	{
		if (count == buffer_size)
		{
			buffer_size = buffer_size*2;
			buffer = realloc(buffer, sizeof(char)*buffer_size);
		}
		if (query[i] == ' ')
		{
			if (count == 0) break;
			iterator->next = malloc(sizeof(linked_list));
			if (iterator->next == NULL) errx(1, "searcher.c: Could not create the wordlist");
			iterator = iterator->next;
			iterator->next = NULL;
			iterator->value = malloc(sizeof(char)*count+1);
			if (iterator->value == NULL) errx(1, "searcher.c: Could not create the wordlist");
			for (int j = 0; j < count; j++)
			{
				iterator->value[j] = buffer[j];
			}
			iterator->value[count] = 0;
			count = 0;
		}
		else
		{
			buffer[count] = query[i];
		}
		count++;
		i++;
	}
	free(buffer);
	return list;
}

void search_query(char *query, HashTable *table_docId, HashTable *table_wordId, Graph *graph)
{
	linked_list *list = get_word_list("This is a test and all words shall be printed ");
	list = list->next;
	while (list != NULL)
	{
		printf("%s\n", list->value);
		list = list->next;
	}
}
