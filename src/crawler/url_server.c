#include "../../include/crawler/url_server.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <err.h>

URLQueue *init_url_queue()
{
    // Allocating the memory for URLQueue struct
    URLQueue *q = malloc(sizeof(URLQueue));
    if(q == NULL)
    {
        errx(EXIT_FAILURE, "Out of memory\n");
    }

    // Init parameters
    // Init semaphores
    if(sem_init(&q->lock, 0, 1) != 0)
    {
        errx(EXIT_FAILURE, "Semaphores initialization error\n");
    }

    if(sem_init(&q->size, 0, 0) != 0)
    {
        errx(EXIT_FAILURE, "Semaphores initialization error\n");
    }

    q->first = NULL;

    return q;
}

void add_url(URLQueue *q, char *url)
{
    // Lock the queue
    sem_wait(&q->lock);

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

    if(q->first == NULL)
    {
        urlStruct->next = urlStruct;
    }
    else
    {
        urlStruct->next = q->first->next;
        q->first->next = urlStruct; 
    }
    q->first = urlStruct;

    // Unlock the queue
    sem_post(&q->lock);

    // Incrementing the size
    sem_post(&q->size);
}

char *pop_url(URLQueue *q)
{
    // Decrementing the size
    sem_wait(&q->size);

    // Lock the queue
    sem_wait(&q->lock);

    URLStruct *tmp;

    if(q->first->next == q->first)
    {
        tmp = q->first;
        q->first = NULL;
    }
    else
    {
        tmp = q->first->next;
        q->first->next = tmp->next;
    }

    char *url = tmp->url;

    free(tmp);

    // Unlocking the queue
    sem_post(&q->lock);

    return url;
}

void free_url_queue(URLQueue *q)
{
    if(q->first != NULL)
    {
        URLStruct *first = q->first;
        URLStruct *tmp1 = q->first->next;
        URLStruct *tmp2;

        while(tmp1 != first)
        {
            tmp2 = tmp1;
            tmp1 = tmp1->next;
            free(tmp2);
        }
        free(first);
    }

    if(sem_destroy(&q->lock) != 0 )
    {
        errx(EXIT_FAILURE, "Free semaphores error\n");
    }

    if(sem_destroy(&q->size) != 0)
    {
        errx(EXIT_FAILURE, "Free semaphores error\n");
    }

    free(q);
}
