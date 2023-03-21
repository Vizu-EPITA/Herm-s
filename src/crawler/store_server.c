#include "../../include/crawler/store_server.h"
#include <stdio.h>
#include <stdlib.h>
#include <zlib.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <err.h>

void save(char *url, int32_t urllen, char *html, int32_t pagelen, int32_t docid)
{
    char repo[] = "./repository/";
	char docidStr[33] = "";
	sprintf(docidStr, "%d", docid);

    char *dest = calloc(strlen(docidStr) + strlen(repo) + 1, sizeof(char));
    if(dest == NULL)
    {
        errx(EXIT_FAILURE, "Out of memory\n");
    }

    strcpy(dest, repo);
    strcat(dest, docidStr);

    FILE *fp = fopen(dest, "w");
    if(fp == NULL)
    {
        errx(EXIT_FAILURE, "failed to open the file");
    }

    // Compress
    unsigned long compressedLength = compressBound(pagelen);
    char *compressed = calloc(compressedLength, sizeof(char));
    if(compressed == NULL)
    {
        errx(EXIT_FAILURE, "Out of memory\n");
    }
    if(compress2((unsigned char *)compressed, &compressedLength,
        (unsigned char *)html, strlen(html), 3) != Z_OK)
    {
        errx(EXIT_FAILURE, "compress failed\n");
    }


	// Writing in the file
	fwrite(&docid, sizeof(int32_t), 1, fp);

	fwrite(&urllen, sizeof(int32_t), 1, fp);

	fwrite(&pagelen, sizeof(int32_t), 1, fp);

	fwrite(url, urllen, 1, fp);

    fwrite(compressed, compressedLength, 1, fp);

    free(compressed);
    free(dest);
    fclose(fp);
}
