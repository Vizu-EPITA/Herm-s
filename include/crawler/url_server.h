#ifndef URL_SERVER_H
#define URL_SERVER_H

#include <stdio.h>
#include <stdlib.h>

typedef struct URLStruct{
    char *url;
    struct URLStruct *next;
    struct URLStruct *prev;
} URLStruct;

typedef struct URLQueue{
    URLStruct *first;
    size_t size;
} URLQueue;


URLQueue *init_url_queue();

void free_url_queue(URLQueue *queue);

void add_url(URLQueue *queue, char *url);

char *pop_url(URLQueue *queue);


#endif
