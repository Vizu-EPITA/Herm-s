#include "searcher.h"

LinkedList *get_word_list(char *query)
{
	LinkedList *list = malloc(sizeof(LinkedList));
	if (list == NULL) errx(1, "searcher.c: Could not create the wordlist");
	list->head = createNode();
	LNode *iterator = list->head;
	iterator->word = NULL;
	iterator->wordId = -1;
	iterator->next = NULL;

	size_t buffer_size = 100;
	char *buffer = malloc(sizeof(char)*buffer_size);
	if (buffer == NULL) errx(1, "searcher.c: Could not create a buffer to get the wordlist");

	size_t i = 0;
	size_t count = 0;

	while (query[i] != 0)
	{
		if (query[i] == ' ' && count != 0)
		{
			iterator->next = createNode();
			iterator = iterator->next;
			iterator->next = NULL;
			iterator->wordId = -1;
			iterator->word = malloc(sizeof(char)*(count+1));
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
	if (count != 0)
	{
		iterator->next = createNode();
		iterator = iterator->next;
		iterator->next = NULL;
		iterator->wordId = -1;
		iterator->word = malloc(sizeof(char)*(count+1));
		if (iterator->word == NULL) errx(1, "searcher.c: Could not create the wordlist");

		for (int j = 0; j < count; j++)
		{
			iterator->word[j] = buffer[j];
		}
		iterator->word[count] = 0;
	}
	free(buffer);
	return list;
}

void search_query(char *query, HashTable *table_docId, HashTable *table_wordId, HashTable *table_inverted, Graph *graph)
{
	/*
	LinkedList *wordlist = get_word_list(query);
	LinkedList *iterator = wordlist->next;
	size_t count = 0;
	while (iterator != NULL)
	{
		count ++;
		iterator->wordId = ht_search(table_wordId, iterator->word);
		iterator = iterator->next;
	}
	//LINKLIST docIdArray[count];
	for (int i = 0; i < count; i++)
	{
		//docIdArray[i] = ht_search(table_inverted, )
	}*/
}

int main()
{
	/*
	LinkedList *l = get_word_list(" first  second     third   ");
	LNode *iterator = l->head->next;
	while (iterator != NULL)
	{
		printf("%s\n", iterator->word);
		iterator = iterator->next;
	}
	printf("%s\n", getElement(l,0)->word);
	printf("%s\n", getElement(l,1)->word);
	printf("%s\n", getElement(l,2)->word);
	freeList(l);
	return 0;
*/
}
