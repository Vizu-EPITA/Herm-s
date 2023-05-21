#include "forward_table.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <err.h>


InvertedTable* ft_create_table(uint32_t size)
{
	ForwardTable* Ft = malloc(sizeof(ForwardTable));
	Ft->size = size;
	Ft->urls = malloc(sizeof(char *) * size);

	return Ft;
}

void ft_free_table(InvertedTable* table)
{
	for(uint32_t i = 0; i < table->size; i++)
	{
		if(table->urls[i] != NULL)
			free(table->urls[i]);
	}
	free(table->urls);
	free(table);
}

void it_insert(InvertedTable* table, uint32_t index, char* url)
{
	if(index < table->size)
	{
		char* heap_url = malloc(sizeof(char) * strlen(url) + 1);
		memcpy(heap_url, url, sizeof(char) * strlen(url));
		heap_url[strlen(url)] = 0;
		table->urls[i] = heap_url;
	}
	else
	{
		uint32_t initialSize = table->size;
		table->size = table->size * 2;
		table->urls = realloc(table->urls, sizeof(char *) * table->size);

		it_insert(table, index, value);
	}
}

char* it_search(InvertedTable* table, uint32_t index)
{
	if(index >= table->size)
	{
		return NULL;
	}
	
	return table->urls[index];
}
