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
    printf("Original Content: %s\n\n", page);

    normalize_html(page, &page_size);

    struct webpage *webpage = malloc(sizeof(struct webpage));

    webpage->url = malloc(sizeof(char) * url_size);
    strcpy(webpage->url, url);
    webpage->url_size = url_size;

    webpage->content = malloc(sizeof(struct data));
    webpage->content->data = malloc(sizeof(char) * page_size);
    strcpy((char *)webpage->content->data, (const char *)page);
    webpage->content->data_size = page_size;
    webpage->content->data_capacity = page_size;

    webpage->checksum = sha1_hash(webpage->content->data, webpage->content->data_size);
    webpage->checksum_size = strlen(webpage->checksum);

    printf("URL: %s\n\n", webpage->url);

    printf("Original data Checksum: %s\n\n", webpage->checksum);

    compress_data(webpage->content);
    printf("Compressed data size: %ld\n\n", webpage->content->data_size);
    printf("Compressed data: %s\n\n", webpage->content->data);

    decompress_data(webpage->content);
    printf("Uncompressed data size: %ld\n\n", webpage->content->data_size);
    printf("Uncompress data: %s\n\n", webpage->content->data);

    compress_data(webpage->content);

    write_page_to_disk(webpage);
}

void write_page_to_disk(struct webpage *webpage)
{
    // Print a message to the console
    printf("Writing to disk...\n\n");

    char *db_directory = string_to_heap("database/", 9);

    // Get folder name
    char *folder_name = sha1_hash((const unsigned char *)webpage->url, webpage->url_size);

    folder_name = string_concat(db_directory, folder_name);

    printf("Folder name: %s\n\n", folder_name);

    // Get file name
    char *file_name = webpage->checksum;

    printf("File name: %s\n\n", file_name);

    if (access(db_directory, F_OK) == -1)
    {
        // If not, create it
        mkdir(db_directory, 0777);
    }

    // Check if the folder located at /database/folder_name exists
    if (access(folder_name, F_OK) == -1)
    {
        // If not, create it
        mkdir(folder_name, 0777);
    }

    char *path = string_concat(folder_name, "/");
    path = string_concat(path, file_name);

    printf("Path: %s\n\n", path);

    // Create the file
    FILE *file = fopen(path, "w");

    // Write the content of the webpage to the file
    fwrite(webpage->content->data, sizeof(char), webpage->content->data_size, file);

    // Close the file
    fclose(file);
}

void normalize_html(char *html, size_t *html_size)
{
    char *p = strstr(html, "<html");

    if (p != NULL)
    {
        size_t offset = p - html;
        size_t remaining = *html_size - offset;
        *html_size = remaining;
        memmove(html, html + offset, remaining + 1);
    }
}

char *get_first_file_in_path(char *path, size_t path_size)
{
    DIR *dir = opendir(path);
    struct dirent *entry;

    if (dir == NULL)
    {
        printf("Failed to open directory at path %s\n", path);
        return;
    }

    entry = readdir(dir);
    entry = readdir(dir);
    closedir(dir);

    char* full_path = string_concat(path, "/");
    full_path = string_concat(full_path, entry->d_name);


    return full_path;
}

unsigned char *get_page_from_disk_with_url(char *url, size_t url_size)
{
    printf("URL: %s\n\n", url);
    char *db_directory = string_to_heap("database/", 9);

    // Get folder name
    char *folder_name = sha1_hash((const unsigned char *)url, url_size);

    //
    char *path = string_concat(db_directory, folder_name);

    printf("Folder name: %s\n\n", path);

    // Open the file located at path
    path = get_first_file_in_path(path, strlen(path));

    printf("Path: %s\n\n", path);

    // Open the file for reading
    FILE *file = fopen(path, "r");

    // Check if the file was successfully opened
    if (file == NULL)
    {
        printf("Failed to open file at path %s\n", path);
        return NULL;
    }

    // Determine the size of the file
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Allocate a buffer for the file contents
    unsigned char *buffer = (unsigned char *)malloc(file_size + 1);

    // Read the file contents into the buffer
    fread(buffer, 1, file_size, file);
    buffer[file_size] = '\0';

    // Close the file
    fclose(file);

    struct data *data = (struct data *)malloc(sizeof(struct data));
    data->data = buffer;
    data->data_size = file_size;
    data->data_capacity = 115;
    printf("Hello World!\n");

    printf("Original data size: %ld\n\n", data->data_size);

    // Display the content of the data
    printf("Original data: %s\n\n", data->data);

    decompress_data(data);

    printf("Uncompressed data size: %ld\n\n", data->data_size);
    printf("Uncompress data: %s\n\n", data->data);

    return data->data;
}

int main()
{

    char *str_test = "TIME : 12/89/09 <html><head><title>Page Title</title></head><body><h1>My First Heading</h1><p>My first paragraph.</p></body></html>";
    size_t str_test_size = strlen(str_test);
    char *new_str = string_to_heap(str_test, str_test_size);
    add_to_server(new_str, str_test_size, "http://www.google.com", 22);

    char *html_test2 = "<html><head><title>Page Title</title></head><body><h1>My First Heading</h1><p>My first paragraph.</p></body></html>";
    size_t html_test2_size = strlen(html_test2);
    char *new_str2 = string_to_heap(html_test2, html_test2_size);
    add_to_server(new_str2, html_test2_size, "http://www.googles.com", 22);

    char *url = "http://www.google.com";
    char *url_heap = string_to_heap(url, 22);

    // Test get_page_from_disk_with_url
    unsigned char *html = get_page_from_disk_with_url(url_heap, 22);
    

    printf("Hello World!\n");

    return 0;
}
