#ifndef URL_SERVER_H
#define URL_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

typedef struct URLStruct{
    char *url;
    struct URLStruct *next;
} URLStruct;

typedef struct URLQueue{
    sem_t lock;
    sem_t size;
    URLStruct *first;
} URLQueue;


URLQueue *init_url_queue();

void free_url_queue(URLQueue *q);

void add_url(URLQueue *q, char *url);

char *pop_url(URLQueue *q);


#endif
