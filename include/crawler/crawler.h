#ifndef CRAWLER_H
#define CRAWLER_H

#include <stdlib.h>
#include <curl/curl.h>
#include <stdio.h>

typedef struct MemoryStruct{
    char *buf;
    size_t size;
} MemoryStruct;

MemoryStruct* download(char *url);

CURL *make_handle(char *url);

int is_html(char *ctype);

#endif
