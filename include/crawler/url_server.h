#ifndef URL_SERVER_H
#define URL_SERVER_H

typedef struct URLStruct{
    char url[256];
    struct URLStruct *next;
    struct URLStruct *prev;
} URLStruct;

typedef struct URLQueue{
    URLStruct *first;
    size_t size;
} URLQueue;

#endif
