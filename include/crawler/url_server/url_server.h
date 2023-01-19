#ifndef URL_SERVER_H
#define URL_SERVER_H

typedef struct URLStruct{
    char *url;
    struct URLStruct *next;
    struct URLStruct *prev;
} URLStruct;

typedef struct URLQueue{
    URLStruct *urlstruct;
    size_t length;
} URLQueue;

#endif
