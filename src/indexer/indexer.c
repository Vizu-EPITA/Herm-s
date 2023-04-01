#include "../main.h"
#include "../errno.h"
#include "../../include/indexer/indexer.h"
#include "../../include/crawler/url_server.h"
#include "../../include/crawler/repository.h"
#include "../../tools/hash_table.h"
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <err.h>
#include <string.h>

void *indexer(void *arg)
{
	thread_data *thr_data = (thread_data *) arg;

//	URLQueue *urlQueue;
	int32_t file;
	htmlStruct *htmlInfo;
	while(1)
	{
		file = pop_file(thr_data->queue_file);
		htmlInfo = decompress_file(file);

		// TRAITEMENT
		printf("INDEXER: indexing the url: %s\n", htmlInfo->url);


		free_htmlstruct(htmlInfo);
	}
}

void free_htmlstruct(htmlStruct *htmlInfo)
{
	free(htmlInfo->page);
	free(htmlInfo->url);
	free(htmlInfo);
}

htmlStruct* decompress_file(int32_t file)
{
	char path[60];
	sprintf(path, "./repository/%d", file);

	// Open the file
	FILE *fp = fopen(path, "r");
	if(fp == NULL)
	{
		printf("opening file failed: %s\n", strerror(errno));
		errx(EXIT_FAILURE, "INDEXER: failed to open the file");
	}

	// Calculate the length of the file
	if(fseek(fp, 0, SEEK_END) < 0)
	{
		fclose(fp);
		return NULL;
	}
	long fileSize = ftell(fp);

	if(fseek(fp, 0, SEEK_SET) < 0)
	{
		fclose(fp);
		return NULL;
	}
	
	// Allocating the htmlstruct info
	htmlStruct *htmlInfo = malloc(sizeof(htmlStruct));
	if(htmlInfo == NULL)
		errx(EXIT_FAILURE, "Not enough memory!");

	// Reading info from the file
	fread(&htmlInfo->docid, sizeof(int32_t), 1, fp);
	fread(&htmlInfo->urllen, sizeof(int32_t), 1, fp);
	fread(&htmlInfo->pagelen, sizeof(int32_t), 1, fp);

	// Read the url from the file
	htmlInfo->url = malloc(sizeof(char) * htmlInfo->urllen + 1);
	if(htmlInfo->url == NULL)
		errx(EXIT_FAILURE, "Not enough memory!");
	fread(htmlInfo->url, sizeof(char), htmlInfo->urllen, fp);
	htmlInfo->url[htmlInfo->urllen] = 0;

	// Read compressed page
	long compressedSize = fileSize - ftell(fp);
	char* compressed = malloc(sizeof(char) * compressedSize);
	if(compressed == NULL)
		errx(EXIT_FAILURE, "Not enough memory!");
	fread(compressed, sizeof(char), compressedSize, fp);

	// Uncomporess the compressed page
	char *page = malloc(sizeof(char) * htmlInfo->pagelen + 1);
	if(page == NULL)
		errx(EXIT_FAILURE, "Not enough memory!");
	int res = uncompress((unsigned char *) page, (unsigned long *) &htmlInfo->pagelen,
		(unsigned char *) compressed, (unsigned long) compressedSize);
	if(res != Z_OK)
		errx(EXIT_FAILURE, "Uncompression error!");
	page[htmlInfo->pagelen] = 0;
	htmlInfo->page = page;

	free(compressed);
	fclose(fp);
	return htmlInfo;
}
