/*
 * Author: Izoulet Aurélien
 * Purpose: Store server.
 * Language: C.
 */

#ifndef _STORE_SERVER_H_
#define _STORE_SERVER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include <openssl/sha.h>

/**
** @brief          Store server page structure.
** @param url      Full url from the webpage.
** @param content  Full html text from the webpage.
** @param checksum Checksum of the webpage.
*/

struct webpage
{
    char* url;
    size_t url_size;

    struct data *content;

    char* checksum;
    size_t checksum_size;
};

struct data
{
    unsigned char* data;
    size_t data_size;
    size_t data_capacity;
};

void compress_data(struct data *data);

void decompress_data(struct data *data);

char* sha1_hash(const unsigned char *data, size_t len);

void add_to_server(char* page, size_t page_size, char* url, size_t url_size);

#endif