#include "../../include/crawler/url_server.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

URLQueue *init_url_queue()
{
    URLQueue *q = malloc(sizeof(URLQueue));
    if(q == NULL)
    {
        errx(EXIT_FAILURE, "Out of memory\n");
    }

    q->size = 0;
    q->first = NULL;

    return q;
}

void add_url(URLQueue *queue, char *url)
{
    URLStruct *urlStruct = malloc(sizeof(URLStruct));
    if(urlStruct == NULL)
    {
        errx(EXIT_FAILURE, "Out of memory\n");
    }  

    char *urlPtr = calloc(strlen(url) + 1, sizeof(char));
    if(urlPtr == NULL)
    {
        errx(EXIT_FAILURE, "Out of memory\n");
    }

    strcpy(urlPtr, url);
    urlStruct->url = urlPtr;

    if(queue->size == 0)
    {
        urlStruct->next = urlStruct;
    }
    else
    {
        urlStruct->next = queue->first->next;
        queue->first->next = urlStruct; 
    }
    queue->first = urlStruct;
    queue->size++;
}

char *pop_url(URLQueue *queue)
{
    URLStruct *tmp;

    if(queue->size == 0)
    {
        return NULL;
    }
    else if(queue->size == 1)
    {
        tmp = queue->first;
        queue->first = NULL;
    }
    else
    {
        tmp = queue->first->next;
        queue->first->next = tmp->next;
    }

    queue->size--;
    char *url = tmp->url;

    free(tmp);

    return url;
}

void free_url_queue(URLQueue *queue)
{
    URLStruct *tmp = queue->first;

    for(size_t i = 0; i < queue->size; i++)
    {
       tmp = tmp->next;

       free(queue->first->url);
       free(queue->first);

       queue->first = tmp;
    }

    free(queue);
}
