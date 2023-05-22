#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <stdint.h>
#include <stddef.h>

typedef struct Ht_item
{
	char* key;
	int32_t value;
	struct Ht_item* next;
} Ht_item;

typedef struct HashTable
{
	Ht_item** items;
	uint32_t size;
	uint32_t count;
} HashTable;

uint32_t Jenkins_one_at_a_time_hash(const char* key, size_t len);

Ht_item* create_item(const char* key, int32_t value);

HashTable* create_table(uint32_t size);

void free_item(Ht_item* item);

void free_table(HashTable* table);

void ht_insert(HashTable* table, const char* key, int32_t value);

int32_t ht_search(HashTable* table, const char* key);

void print_table(HashTable* table);

void save_table(HashTable* table, char* path);

HashTable* load_table(char* path);

#endif
