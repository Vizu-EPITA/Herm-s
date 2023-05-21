#include "forward_table.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <err.h>


ForwardTable* ft_create_table(uint32_t size)
{
	ForwardTable* Ft = malloc(sizeof(ForwardTable));
	Ft->size = size;
	Ft->urls = malloc(sizeof(char *) * size);
	for(uint32_t i = 0; i < size; i++)
	{
		Ft->urls[i] = NULL;
	}

	return Ft;
}

void ft_free_table(ForwardTable* table)
{
	for(uint32_t i = 0; i < table->size; i++)
	{
		if(table->urls[i] != NULL)
			free(table->urls[i]);
	}
	free(table->urls);
	free(table);
}

void ft_insert(ForwardTable* table, uint32_t index, char* url)
{
	if(index < table->size)
	{
		char* heap_url = malloc(sizeof(char) * strlen(url) + 1);
		memcpy(heap_url, url, sizeof(char) * strlen(url));
		heap_url[strlen(url)] = 0;
		table->urls[index] = heap_url;
	}
	else
	{
		uint32_t initialSize = table->size;
		table->size = table->size * 2;
		table->urls = realloc(table->urls, sizeof(char *) * table->size);

		for(uint32_t i = initialSize; i < table->size; i++)
		{
			table->urls[i] = NULL;
		}

		ft_insert(table, index, url);
	}
}

char* ft_search(ForwardTable* table, uint32_t index)
{
	if(index >= table->size)
	{
		return NULL;
	}
	
	return table->urls[index];
}

void ft_save(ForwardTable* table, char* path)
{
	FILE *fp = fopen(path, "w");
	if(fp == NULL)
	{
		printf("opening file failed: %s\n", strerror(errno));
		errx(EXIT_FAILURE, "INDEXER: failed to open the file");
	}

	uint32_t size = 0;
	fwrite(&table->size, sizeof(uint32_t), 1, fp);
	for(uint32_t i = 0; i < table->size; i++)
	{
		if(table->urls[i] == NULL)
		{
			size = 0;
			fwrite(&size, sizeof(uint32_t), 1, fp);
		}
		else
		{
			size = strlen(table->urls[i]);
			fwrite(&size, sizeof(uint32_t), 1, fp);
			fwrite(table->urls[i], sizeof(char), size, fp);
		}
	}

	fclose(fp);
}

ForwardTable* ft_load(char* path)
{
	FILE *fp = fopen(path, "r");
	if(fp == NULL)
	{
		printf("opening file failed: %s\n", strerror(errno));
		errx(EXIT_FAILURE, "INDEXER: failed to open the file");
	}

	ForwardTable *Ft = malloc(sizeof(ForwardTable));
	if (Ft == NULL)
	{
		errx(EXIT_FAILURE, "Not enough memory !");
	}

	fread(&Ft->size, sizeof(uint32_t), 1, fp);

	Ft->urls = malloc(sizeof(char *) * Ft->size);

	uint32_t size = 0;
	for(uint32_t i = 0; i < table->size; i++)
	{
		fread(&size, sizeof(uint32_t), 1, fp);
		if(size == 0)
		{
			Ft->urls[i] = NULL;
		}
		else
		{
			Ft->urls[i] = malloc(sizeof(char) * size + 1);
			if(Ft->urls[i] == NULL):
				errx(EXIT_FAILURE, "Not enough memory !");
			fread(Ft->urls[i], sizeof(char), size, fp);
			Ft->urls[i][size] = 0;
		}
	}

	close(fp);

	return Ft;
}
