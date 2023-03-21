#ifndef STORE_SERVER_H
#define STORE_SERVER_H

#include <stdint.h>

void save(char *url, int32_t urllen, char *html, int32_t pagelen, int32_t docid);

#endif
