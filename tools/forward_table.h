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

void ft_free_table(ForwardTable* table);

void ft_insert(ForwardTable* table, uint32_t index, char* url);

char* ft_search(ForwardTable* table, uint32_t index);

void ft_save(ForwardTable* table, char* path)

ForwardTable* ft_load(char* path)

#endif
