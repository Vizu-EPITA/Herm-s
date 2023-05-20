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

void search_query(char *query, HashTable *table_docId, HashTable *table_wordId, TYPE *INVERTED, Graph *graph)
{
	LinkedList *wordlist = get_word_list(query);
	LNode *iterator = wordlist->head->next;
	size_t nbWords = 0;

	//Get all the word IDs and count the number of words
	while (iterator != NULL)
	{
		nbWords ++;
		iterator->wordId = ht_search(table_wordId, iterator->word);
		iterator = iterator->next;
	}

	//Query empty
	if (nbWords == 0)
	{
		printf("Please search for at least one word\n");
		return;
	}

	//Get all the docIds, not sorted yet
	STRUCT_INVERTED_TYPE structInvertedArray[count];
	for (int i = 0; i < count; i++)
	{
		structInvertedArray[i] = INVERTED_SESARCH(INVERTED, getElement(wordlist, i)->word);
	}

	STRUCT_INVERTED_TYPE firstStruct = structInvertedArray[0];
	size_t fullSize = firstStruct->size;
	int32_t docIdFullArray = firstStruct->array;
	size_t nbAdded;
	int32_t docIdArray[fullSize];
	int32_t docId;
	//Iterate over the first table of docIds
	for (int i = 0; i < fullSize; i++)
	{
		docId = docIdFullArray[i];
		//Iterate over the structs
		for (int j = 1; j < nbWords; j++)
		{
			//Iterate over the arrays of the structs
			for (int k = 0; k < structInvertedArray[j]->size)
			{

			}
		}
	}

}
//inverted -> struct array
