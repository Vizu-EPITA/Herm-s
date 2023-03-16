#ifndef CRAWLER_H
#define CRAWLER_H

typedef struct Ht_item
{
	char* key;
	uint32_t value;
	Ht_item* next;
} Ht_item;

typedef struct HashTable
{
	Ht_item** items;
	uint32_t size;
	uint32_t count;
} HashTable;

Ht_item* create_item(char* key, uint32_t value);

HashTable* create_table(uint32_t size);

void free_item(Ht_item* item);

void free_table(HashTable* table);

void ht_insert(HashTable* table, char* key, uint32_t value);

uint32_t ht_search(HashTable* table, char* key);

void print_table(HashTable* table);

#endif
