#ifndef INVERTED_TABLE_H
#define INVERTED_TABLE_H

#include <stdint.h>
#include <stddef.h>

typedef struct It_item
{
	int32_t* values;
	uint32_t size;
	uint32_t count;
} It_item;

typedef struct InvertedTable
{
	It_item** items;
	uint32_t size;
} InvertedTable;

It_item* it_create_item();

InvertedTable* it_create_table(uint32_t size);

void it_free_item(It_item* item);

void it_free_table(InvertedTable* table);

void it_insert(InvertedTable* table, uint32_t index, int32_t value);

It_item* it_search(InvertedTable* table, uint32_t index);

#endif
