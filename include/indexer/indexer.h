#ifndef INDEXER_H
#define INDEXER_H

#include <stdint.h>
#include <stddef.h>

typedef struct htmlStruct
{
	int32_t docid;
	int32_t pagelen;
	char* page;
	int32_t urllen;
	char* url;
} htmlStruct;

htmlStruct* decompress_file(char* filename);

#endif
