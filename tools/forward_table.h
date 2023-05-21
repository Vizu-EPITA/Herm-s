#ifndef	FORWARD_TABLE_H 
#define FORWARD_TABLE_H

#include <stdint.h>
#include <stddef.h>

typedef struct ForwardTable
{
	char** urls;
	uint32_t size;
} ForwardTable;

ForwardTable* ft_create_table(uint32_t size);

void it_free_table(ForwardTable* table);

void it_insert(ForwardTable* table, uint32_t index, char* url);

char* it_search(ForwardTable* table, uint32_t index);

#endif
