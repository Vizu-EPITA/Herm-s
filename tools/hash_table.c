#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

uint32_t Jenkins_one_at_a_time_hash(const char* key, size_t len)
{
	uint32_t hash;
	uint32_t i;
	for(hash = i = 0; i < len; ++i)
	{
		hash += key[i];
		hash += (hash << 10);
		hash ^= (hash >> 6);
	}
	hash += (hash << 3);
	hash ^= (hash >> 11);
	hash += (hash << 15);
	return hash;
}

Ht_item* create_item(char* key, uint32_t value)
{
	// Creates a pointer to a new HashTable item.
	Ht_item* item = (Ht_item*) malloc(sizeof(Ht_item));
	if(item == NULL)
		errx(EXIT_FAILURE, "Not enough memory!");
	
	item->key = (char*) malloc(strlen(key) + 1);
	if(item->key == NULL)
		errx(EXIT_FAILURE, "Not enough memory!");

	item->value = value;
	item->next = NULL;
	strcpy(item->key, key);

	return item;
}

HashTable* create_table(uint32_t size)
{
	// Creates a new HashTable.
	HashTable* table = (HashTable*) malloc(sizeof(HashTable));
	if(table == NULL)
		errx(EXIT_FAILURE, "Not enough memory!");

	table->size = size;
	table->count = 0;
	table->items = (Ht_item**) malloc(sizeof(Ht_item*) * size);

	for(uint32_t i = 0; i < table->size; i++)
		table->items[i] = NULL;

	return table;
}

void free_item(Ht_item* item)
{
	// Frees an item.
	free(item->key);
	free(item->value);
	free(item);
}

void free_table(HashTable* table)
{
	// Frees the table
	for(uint32_t i = 0; i < table->size; i++)
	{
		Ht_item* item = table->items[i];

		while(item != NULL)
		{
			Ht_item* tmp = item;
			item = item->next;
			free_item(tmp);
		}
	}
	free(table->items);
	free(table);
}

void ht_insert(HashTable* table, char* key, uint32_t value)
{
	// Creates the item.
	Ht_item* item = create_item(key, value);
	
	// Computes the index.
	uint32_t index = Jenkins_one_at_a_time_hash(key, strlen(key)) % table->size;

	Ht_item* current_item = table->items[index];

	if(current_item == NULL)
	{
		// Key does not exist.
		table->items[index] = item;
		table->count++;
	}
	else
	{
		while(current_item->next != NULL)
		{
			current_item = current_item->next;
		}
		current_item->next = item;
		table->count++;
	}
}

uint32_t ht_search(HashTable* table, char* key)
{
	// Searches for the key in the HashTable.
	// Returns NULL if it doesn't exist.
	uint32_t index = Jenkins_one_at_a_time_hash(key, strlen(key)) % table->size;
	Ht_item* item =  table->items[index];
	
	while(item != NULL)
	{
		if(strcmp(item->key, key) == 0)
			return value;

		item = item->next;
	}

	return NULL;
}

void print_table(HashTable* table)
{
	printf("\nHash Table\n------------------\n");
	for(uint32_t i = 0; i < table->size; i++)
	{
		Ht_item* item = table->items[i];
		printf("Index:%d |")
		while(item != NULL)
		{
			printf(" Key:%s, Value:%s |",
			i, table->items[i]->key, table->items[i]->value);
			item = item->next;
		}
		printf("\n");
	}
	printf("------------------\n\n");
}
