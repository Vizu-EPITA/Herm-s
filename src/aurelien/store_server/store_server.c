/*
 * Author: Izoulet Aurélien
 * Purpose: Store server.
 * Language: C.
 */

#include "store_server.h"

void compress_data(struct data *data)
{
    unsigned long output_size = compressBound(data->data_size);
    unsigned char *output = (unsigned char *)malloc(output_size);

    if (compress2(output, &output_size, data->data, data->data_size, Z_BEST_COMPRESSION) != Z_OK)
    {
        fprintf(stderr, "Compression failed\n");
        exit(1);
    }

    free(data->data);
    data->data = (unsigned char *)output;
    data->data_size = output_size;
}

void decompress_data(struct data *data)
{
    unsigned char *decompressed = (unsigned char *)malloc(data->data_capacity);

    if (uncompress(decompressed, &(data->data_capacity), data->data, data->data_size) != Z_OK)
    {
        fprintf(stderr, "Decompression failed\n");
        exit(1);
    }

    free(data->data);
    data->data = (unsigned char *)decompressed;
    data->data_size = data->data_capacity;
}

char *sha1_hash(const unsigned char *data, size_t len)
{
    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA1(data, len, digest);
    char *sha1string = (char *)malloc(SHA_DIGEST_LENGTH * 2 + 1);
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
    {
        sprintf(&sha1string[i * 2], "%02x", (unsigned int)digest[i]);
    }
    sha1string[SHA_DIGEST_LENGTH * 2] = '\0';
    return sha1string;
}

void add_to_server(char *page, size_t page_size, char *url, size_t url_size)
{
    normalize_html(page, &page_size);
    struct webpage *webpage = malloc(sizeof(struct webpage)); // TODO: free
    webpage->url = malloc(sizeof(char) * url_size);           // TODO: free
    strcpy(webpage->url, url);
    webpage->url_size = url_size;
    webpage->content = malloc(sizeof(struct data)); // TODO: free
    webpage->content->data = malloc(sizeof(char) * page_size);
    strcpy(webpage->content->data, page);
    webpage->content->data_size = page_size;
    webpage->content->data_capacity = page_size;
    compress_data(webpage->content);
    webpage->checksum = sha1_hash(webpage->content->data, webpage->content->data_size);
    webpage->checksum_size = strlen(webpage->checksum);
    printf("URL: %s\n", webpage->url);
    printf("Content: %s\n", webpage->content->data);
    printf("Checksum: %s\n", webpage->checksum);
}

void normalize_html(char* html, size_t *html_size)
{
    char *p = strstr(html, "<html"); // Find the first occurrence of occ

    if (p != NULL)
    {                                                  // If occ is found
        size_t offset = p - html + 2;                 // Calculate the offset of the first character after "\n\n"
        size_t remaining = *html_size - offset;  
        *html_size = remaining; // Calculate the length of the remaining string
        memmove(html, html + offset, remaining + 1); // Move the remaining string to the beginning of the array

        printf("%s", html); // Print the string
    }
}


int main() {
    
    

    return 0;
}

