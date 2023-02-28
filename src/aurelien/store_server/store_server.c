/*
 * Author: Izoulet Aurélien
 * Purpose: Store server.
 * Language: C.
 */

#include "store_server.h"

void compress_data(struct data *data)
{
    unsigned long output_size = compressBound(data->data_size);
    unsigned char* output = (unsigned char*)malloc(output_size);

    if (compress2(output, &output_size, data->data, data->data_size, Z_BEST_COMPRESSION) != Z_OK) {
        fprintf(stderr, "Compression failed\n");
        exit(1);
    }

    free(data->data);
    data->data = (unsigned char*)output;
    data->data_size = output_size;
}

void decompress_data(struct data *data)
{
    unsigned char* decompressed = (unsigned char*)malloc(data->data_capacity);

    if (uncompress(decompressed, &(data->data_capacity), data->data, data->data_size) != Z_OK) {
        fprintf(stderr, "Decompression failed\n");
        exit(1);
    }

    free(data->data);
    data->data = (unsigned char*)decompressed;
    data->data_size = data->data_capacity;
}

char* sha1_hash(const unsigned char *data, size_t len)
{
    unsigned char digest[SHA_DIGEST_LENGTH];
    SHA1(data, len, digest);
    char* sha1string = (char*) malloc(SHA_DIGEST_LENGTH*2 + 1);
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        sprintf(&sha1string[i*2], "%02x", (unsigned int) digest[i]);
    }
    sha1string[SHA_DIGEST_LENGTH*2] = '\0';
    return sha1string;
}

int main()
{
    char *hello = malloc(strlen("Hello world") + 1);  
    if (hello == NULL) {  
        printf("Error: memory allocation failed\n");
        return 1;
    }

    strcpy(hello, "Hello world");  

    struct data *data = malloc(sizeof(struct data));

    data->data = (unsigned char*) hello;
    data->data_size = strlen(hello);
    data->data_capacity = strlen(hello);

    compress_data(data);

    decompress_data(data);

    printf("%s\n", data->data);  
    printf("%s\n", sha1_hash(data->data, data->data_size));

    free(data->data);  
    free(data);

    return 0;
}
