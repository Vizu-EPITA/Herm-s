#include "inverted_index.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <err.h>


It_item* create_item()
{
	It_item* item = malloc(sizeof(It_item));
	item->count = 0;
	item->size = 100; 
	item->values = malloc(sizeof(int32_t) * item->size);

	return item;
}

InvertedTable* create_table(uint32_t size)
{
	InvertedTable* It = malloc(sizeof(InvertedTable));
	It->size = size;
	It->Items = malloc(sizeof(It_item *) * size);
	for(uint32_t i = 0; i < size; i++)
	{
		It->items[i] = create_item();
	}

	return It;
}

void free_item(Ht_item* item)
{
	free(item->value);
	free(item);
}

void free_table(HashTable* table)
{
	free(table->items);
	free(table);
}

void it_insert(InvertedTable* table, uint32_t index, int32_t value)
{
	if(index < table->size)
	{
		if(table->items[index]->count < table->items[index]->size)
		{
			table->items[index]->values[table->items[index]->count] = value;
			table->items[index]->count++;
		}
		else
		{
			table->items[index]->size = table->items[index]->size * 2;
			table->items[index]->values = realloc(table->items[index]->values,
				sizeof(int32_t) * table->items[index]->size);

			it_insert(table, index, value);
		}
	}
	else
	{
		uint32_t initialSize = table->size;
		table->size = table->size * 2;
		table->items = realloc(sizeof(It_item *) * table->size);

		for(uint32_t i = initialSize; i < table->size; i++)
		{
			table->items[i] = create_item();	
		}

		it_insert(table, index, value);
	}
}

It_item* it_search(InvertedTable* table, uint32_t index)
{
}

