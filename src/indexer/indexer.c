#include "../../include/indexer/indexer.h"
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <err.h>
#include <string.h>

/*
void Indexer()
{
}
*/

char* decompress_file(char* filename)
{
	FILE *fp = fopen(filename, "r");
	if(fp == NULL)
		return NULL;

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
	
	int32_t docid;
	int32_t urllen;
	int32_t pagelen;

	fread(&docid, sizeof(int32_t), 1, fp);
	fread(&urllen, sizeof(int32_t), 1, fp);
	fread(&pagelen, sizeof(int32_t), 1, fp);

	char *url = malloc(sizeof(char) * urllen);
	if(url == NULL)
		errx(EXIT_FAILURE, "Not enough memory!");
	fread(url, sizeof(char), urllen, fp);

	long compressedSize = fileSize - ftell(fp);
	


	char* compressed = malloc(sizeof(char) * compressedSize);
	if(compressed == NULL)
		errx(EXIT_FAILURE, "Not enough memory!");
	fread(compressed, sizeof(char), fileSize, fp);

	char *html = malloc(sizeof(char) * pagelen);
	if(html == NULL)
		errx(EXIT_FAILURE, "Not enough memory!");
	
	int res = uncompress((unsigned char *) html, (unsigned long *) &pagelen,
		(unsigned char *) compressed, (unsigned long) compressedSize);
	if(res != Z_OK)
		errx(EXIT_FAILURE, "Not enough memory!");

	free(compressed);
	free(url);

	
	fclose(fp);
	return html;
}
