#ifndef URL_SERVER_H
#define URL_SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

typedef struct URLStruct{
    size_t count;
    char **url;
    struct URLStruct *next;
} URLStruct;

typedef struct URLQueue{
    sem_t lock;
    sem_t size;
    URLStruct *first;
} URLQueue;


URLQueue *init_url_queue();

void free_urlstruct(URLStruct *urlStruct);

void free_url_queue(URLQueue *q);

void add_url(URLQueue *q, char *url);

URLStruct* pop_url(URLQueue *q);


#endif
