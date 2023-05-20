#include "inverted_table.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <err.h>


It_item* it_create_item()
{
	It_item* item = malloc(sizeof(It_item));
	item->count = 0;
	item->size = 100; 
	item->values = malloc(sizeof(int32_t) * item->size);

	return item;
}

InvertedTable* it_create_table(uint32_t size)
{
	InvertedTable* It = malloc(sizeof(InvertedTable));
	It->size = size;
	It->items = malloc(sizeof(It_item *) * size);
	for(uint32_t i = 0; i < size; i++)
	{
		It->items[i] = it_create_item();
	}

	return It;
}

void it_free_item(It_item* item)
{
	free(item->values);
	free(item);
}

void it_free_table(InvertedTable* table)
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
		table->items = realloc(table->items, sizeof(It_item *) * table->size);

		for(uint32_t i = initialSize; i < table->size; i++)
		{
			table->items[i] = it_create_item();	
		}

		it_insert(table, index, value);
	}
}

It_item* it_search(InvertedTable* table, uint32_t index)
{
	if(table->size <= index)
	{
		return NULL;
	}
	
	It_item* item = malloc(sizeof(It_item));

	item->size = table->items[index]->count;

	item->values = malloc(sizeof(int32_t) * item->size);
	memcpy(item->values, table->items[index]->values, sizeof(int32_t) * item->size);

	return item;
}
