#include "../../include/searcher/searcher.h"

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

double get_ten_rank(size_t *tenRankIndexArray, size_t index, int32_t *docIdArray, size_t nbAdded, double formerMax, Graph *graph)
{
	double temp = 0;
	double max = 0;
	size_t maxIndex = 0;
	if (formerMax != -1)
	{
		for (size_t i = 0; i < nbAdded; i++)
		{
			temp = graph->nodes[i]->pageRank;
			if (temp > max  && temp <= formerMax)
			{
				max = temp;
				maxIndex = i;
			}
		}
	}
	else
	{
		for (size_t i = 0; i < nbAdded; i++)
		{
			temp = graph->nodes[i]->pageRank;
			if (temp > max)
			{
				max = temp;
				maxIndex = i;
			}
		}
	}
	tenRankIndexArray[index] = maxIndex;
	return max;
}

char **search_query(char *query, HashTable *table_docId, HashTable *table_wordId, InvertedTable *inverted, Graph *graph)
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
	It_item structInvertedArray[count];
	for (int i = 0; i < count; i++)
	{
		structInvertedArray[i] = it_search(inverted, getElement(wordlist, i)->word);
	}
	It_item firstStruct = structInvertedArray[0];
	int32_t fullSize = firstStruct->size;
	int32_t *docIdFullArray = firstStruct->values;
	int32_t nbAdded;
	int32_t docIdArray[fullSize];
	int32_t docId;
	int found = 0;
	//Iterate over the first table of docIds
	for (int i = 0; i < fullSize; i++)
	{
		docId = docIdFullArray[i];
		int j = 1;
		//Iterate over the structs
		while (j < nbWords)
		{
			It_item currentStruct = structInvertedArray[j];
			found = 0;
			int k = 0;
			//Iterate over the arrays of the structs
			while (k < currentStruct->size)
			{
				if (currentStruct->values[k] == docId)
				{
					found = 1;
					break;
				}
				k++;
			}
			if (found == 0)
			{
				break;
			}
			j++;
		}
		if (j == nbWords)
		{
			docIdArray[nbAdded] = docId;
			nbAdded++;
		}
	}

	//Get the top 10 biggest ranks
	int32_t *tenRankIndexArray = malloc(sizeof(int32_t)*10);
	if (tenRankIndexArray == NULL) errx(1, "Could not allocate tenRankArray");
	double formerMax = get_ten_rank(tenRankIndexArray, 0, docIdArray, nbAdded, -1);
	for (int32_t i = 1; i < 10 && i < nbAdded; i++)
	{
		formerMax = get_ten_rank(tenRankIndexArray, i, docIdArray, nbAdded, formerMax);
	}

	//Retrieve the urls
	char **urlArray = malloc(sizeof(char*)*10);
	if (urlArray == NULL) errx(1, "COuld not allocate the urlArray");
	int32_t i = 0;
	while (i < 10 && i < nbAdded)
	{
		urlArray[i] = ht_search(table_docId, docIdArray[tenRankIndexArray[i]]);
		i++;
	}
	while (i < 10)
	{
		urlArray[i] = 0
		i++;
	}

	free(tenRankIndexArray);
	return urlArray;
}
