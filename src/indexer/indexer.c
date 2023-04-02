#include "../main.h"
#include "../errno.h"
#include "../../include/indexer/indexer.h"
#include "../../include/crawler/url_server.h"
#include "../../include/crawler/repository.h"
#include "../../tools/hash_table.h"
#include "../searcher/data_structures/graph.h"
#include "../searcher/pagerank.h"
#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <err.h>
#include <string.h>
#include <wget.h>

int32_t docID_count = 0;

void *indexer(void *arg)
{
	thread_data *thr_data = (thread_data *) arg;
	docID_count = thr_data->table_docID->count;

//	URLQueue *urlQueue;
	int32_t file;
	htmlStruct *htmlInfo;
	int file_count = 0;
	while(1)
	{
		if(file_count > 3000)
			break;
		file = pop_file(thr_data->queue_file);
		htmlInfo = decompress_file(file);

		// TRAITEMENT
		printf("INDEXER: indexing the url: %s\n", htmlInfo->url);
		parseText(htmlInfo, thr_data->table_docID, thr_data->queue_url, thr_data->graph);


		free_htmlstruct(htmlInfo);
		file_count++;
	}
	printGraph(thr_data->graph);
	initRank(thr_data->graph);
	rank(thr_data->graph, 20);
	printRank(thr_data->graph);
	return NULL;
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

void parseText(htmlStruct *htmlInfo, HashTable *table_docID, URLQueue *queue_url, struct Graph *graph)
{
	char *page = htmlInfo->page;
	wget_iri_t *base = wget_iri_parse(htmlInfo->url, NULL);
    char *wordBuf = malloc(sizeof(char)*2500);
    char *linkBuf = malloc(sizeof(char)*2500);
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
						const char *absurl = wget_iri_relative_to_abs(base, linkBuf, linkLen, buf);
						char *wikipedia_url = "https://en.wikipedia.org";
						if(strncmp(wikipedia_url, absurl, strlen(wikipedia_url)) == 0)
						{
							//printf("INDEXER: Link found: id = %d, url =%s\n", docID_count, absurl);
							if(ht_search(table_docID, absurl) == -1)
							{
								docID_count++;
								ht_insert(table_docID, absurl, docID_count);
								add_url(queue_url, absurl);
								addEdge(graph, findOrCreateNode(graph, htmlInfo->docid), findOrCreateNode(graph, docID_count));
							}
						}

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
