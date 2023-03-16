#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define

Ht_item* create_item(char* key, char* value)
{
	// Creates a pointer to a new HashTable item.
	Ht_item* item = (Ht_item*) malloc(sizeof(Ht_item));

	if(item == NULL)
		errx(EXIT_FAILURE, "Not enough memory!");
	
	item->key = (char*) malloc(strlen(key) + 1);
	if(item->key == NULL)
		errx(EXIT_FAILURE, "Not enough memory!");

	item->value = (char*) malloc(strlen(value) + 1);
	if(item->value == NULL)
		errx(EXIT_FAILURE, "Not enough memory!");

	item->next = NULL;

	strcpy(item->key, key);
	strcpy(item->value, value);

	return item;
}

HashTable* create_table(size_t size)
{
	// Creates a new HashTable.
	HashTable* table = (HashTable*) malloc(sizeof(HashTable));
	if(table == NULL)
		errx(EXIT_FAILURE, "Not enough memory!");

	table->size = size;
	table->count = 0;
	table->items = (Ht_item**) malloc(sizeof(Ht_item*) * size);

	for(size_t i = 0; i < table->size; i++)
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
	for(size_t i = 0; i < table->size; i++)
	{
		Ht_item* item = table->items[i];

		if(item != NULL)
			free_item(item);
	}
	free(table->items);
	free(table);
}

void ht_insert(HashTable* table, char* key, char* value)
{
	// Creates the item.
	Ht_item* item = create_item(key, value);
	
	// Computes the index.
	size_t index = hash_function(key);

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

char* ht_search(HashTable* table, char* key)
{
	// Searches for the key in the HashTable.
	// Returns NULL if it doesn't exist.
	size_t index = hash_function(key);
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
	for(size_t i = 0; i < table->size; i++)
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
