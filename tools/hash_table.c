#include "hash_table.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <err.h>
#include <errno.h>


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

Ht_item* create_item(const char* key, int32_t value)
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
	#include <stdio.h>
	free(table);
}

void ht_insert(HashTable* table, const char* key, int32_t value)
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

int32_t ht_search(HashTable* table, const char* key)
{
	// Searches for the key in the HashTable.
	// Returns NULL if it doesn't exist.
	uint32_t index = Jenkins_one_at_a_time_hash(key, strlen(key)) % table->size;
	Ht_item* item =  table->items[index];
	
	while(item != NULL)
	{
		if(strcmp(item->key, key) == 0)
			return item->value;

		item = item->next;
	}

	return -1;
}

void print_table(HashTable* table)
{
	printf("\nHash Table\n------------------\n");
	for(uint32_t i = 0; i < table->size; i++)
	{
		Ht_item* item = table->items[i];
		printf("Index:%u |", i);
		while(item != NULL)
		{
			printf(" Key:%s, Value:%u |", item->key, item->value);
			item = item->next;
		}
		printf("\n");
	}
	printf("------------------\n\n");
}

void save_table(HashTable* table, char* path)
{
	FILE *fp = fopen(path, "w");
	if(fp == NULL)
	{
		printf("opening file failed: %s\n", strerror(errno));
		errx(EXIT_FAILURE, "SAVE HASHTABLE: failed to open the file");
	}

	fwrite(&table->size, sizeof(uint32_t), 1, fp);

	fwrite(&table->count, sizeof(uint32_t), 1,fp);

	int32_t stop = -2;
	uint32_t size = 0;
	for(uint32_t i = 0; i < table->size; i++)
	{
		Ht_item *tmp = table->items[i];
		while(tmp != NULL)
		{
			fwrite(&tmp->value, sizeof(int32_t), 1, fp);

			size = strlen(tmp->key);
			fwrite(&size, sizeof(uint32_t), 1, fp);

			fwrite(tmp->key, sizeof(char), size, fp);

			tmp = tmp->next;
		}
		fwrite(&stop, sizeof(int32_t), 1, fp);
	}

	fclose(fp);
}

HashTable* load_table(char* path)
{
	FILE *fp = fopen(path, "r");
	if(fp == NULL)
	{
		printf("opening file failed: %s\n", strerror(errno));
		errx(EXIT_FAILURE, "LOAD HASHTABLE: failed to open the file");
	}
	
	// Alloc Hash Table
	HashTable *Ht = malloc(sizeof(HashTable));
	if(Ht == NULL)
		errx(EXIT_FAILURE, "Not enough memory");

	// Read parameter Size of the Hash Table
	fread(&Ht->size, sizeof(uint32_t), 1, fp);

	fread(&Ht->count, sizeof(uint32_t), 1, fp);

	// Alloc size of the Hash Table
	Ht->items = malloc(sizeof(Ht_item *) * Ht->size);
	if (Ht->items == NULL)
		errx(EXIT_FAILURE, "Not enough memory");
	
	int32_t value;
	uint32_t len;
	Ht_item *tmp;
	Ht_item *first;

	for(uint32_t i = 0; i < Ht->size; i++)
	{
		tmp = NULL;
		fread(&value, sizeof(int32_t), 1, fp);
		while(value != -2)
		{	
			// Alloc Hash Table item and set parameters
			Ht_item *item = malloc(sizeof(Ht_item));
			if(item == NULL)
				errx(EXIT_FAILURE, "Not enough memory");
			item->next = NULL;
			item->value = value;

			// If it's the first we see
			if(tmp == NULL)
			{
				first = item;
			}

			// Read len of the key and malloc the size for it
			fread(&len, sizeof(uint32_t), 1, fp);
			item->key = malloc(sizeof(char) * len + 1);
			if(item->key == NULL)
				errx(EXIT_FAILURE, "Not enough memory");

			// Read the key
			fread(item->key, sizeof(char), len, fp);
			item->key[len] = 0;

			if(tmp != NULL)
			{
				tmp->next = item;
			}

			tmp = item;
			fread(&value, sizeof(int32_t), 1, fp);
		}
		if(tmp == NULL)
		{
			Ht->items[i] = NULL;
		}
		else
		{
			Ht->items[i] = first;
		}
		
	}
	fclose(fp);

	return Ht;
}
