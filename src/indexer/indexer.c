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
#include <wget.h>

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
		parseText(htmlInfo);


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

void printWord(char *wordBuf, size_t len)
{
    for (size_t i = 0; i < len; i++)
    {
        printf("%c", *wordBuf);
        wordBuf++;
    }
    printf("\n");
}

size_t parseWord(char *page, char *wordBuf /*,[HTBLE]*/)
{   
    size_t len = 0;
    while (*page != 0 && (
          (*page >= '0' && *page <= '9') ||
          (*page >= 'A' && *page <= 'Z') ||
          (*page >= 'a' && *page <= 'z')))
    {
        *wordBuf = *page;
        wordBuf++;
        page++;
        len++;
    }
    return len;
}

size_t parseLink(char *page, char *linkBuf)
{
    size_t len = 0;
    while (*page != 0 && *page != '\"')
    {
        *linkBuf = *page;
        linkBuf++;
        page++;
        len++;
    }
    return len;
}

void parseText(htmlStruct *htmlInfo)
{
	char *page = htmlInfo->page; 
	wget_iri_t *base = wget_iri_parse(htmlInfo->url, NULL);
    char *wordBuf = malloc(sizeof(char)*300);
    char *linkBuf = malloc(sizeof(char)*600);
    size_t wordLen;
    size_t linkLen; 
    while (*page != 0)
    {
        // End of file (EOF)
        if (*page == 0)
            return;
        // Tag detected
        else if (*page == '<')
        {
            // Search end of tag or EOF
            while (*page != '>' && *page != 0)
            {
                if (*page == 'h' && strncmp(page, "href=\"", 6) == 0)
                {
                    while (*page != 0 && *page != '\"')
                    {
                        page++;
                    }
                    page++;
                    linkLen = parseLink(page, linkBuf);
                    if (linkLen > 0)
                    {
                        //printf("%i\n", linkLen);
                        //EDIT
                        //printWord(linkBuf, linkLen);
						
						wget_buffer_t *buf = wget_buffer_alloc(linkLen + htmlInfo->urllen);
						printf("INDEXER: Link found: %s\n",
						wget_iri_relative_to_abs(base, linkBuf, linkLen, buf));

						wget_buffer_free(&buf);
                    }
                    page += linkLen;
                }
                else
                    page++;
            }
        }
        // End of tag
        else if ((*page >= '0' && *page <= '9') ||
                 (*page >= 'A' && *page <= 'Z') ||
                 (*page >= 'a' && *page <= 'z'))
        {
            wordLen = parseWord(page, wordBuf);
            //EDIT
            //printWord(wordBuf, wordLen);
            page += wordLen;
        }
        else
            page++;
    }
    free(wordBuf);
    free(linkBuf);
	wget_iri_free(&base);
}
/*
int main()
{
    char *page = malloc(sizeof(char)*250);
    char buf[] = "<href=\"printLink1\"> printTxt1 <sdass> <href=\"printLink2\"adkjnd>";
    strcpy(page, buf);
    parseText(page);
    free(page);
    return 0;
}
*/
