#include "./../../../include/crawler/crawler/crawler.h"
#include <err.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <stdio.h>
#include <string.h>

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(!ptr)
    {
        // Out of memory!
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}


int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        errx(EXIT_FAILURE, "usage: argv[0] link\n");
    }

    CURL *curl_handle;
    CURLcode res;

    MemoryStruct *mem = malloc(sizeof(MemoryStruct));
    if(mem == NULL)
    {
        errx(EXIT_FAILURE, "Memory allocation error\n");
    }

    mem->memory = malloc(1);    // Will be grown as needed by the realloc
    mem->size = 0;              // No data at this point

    curl_global_init(CURL_GLOBAL_ALL);

    // Init the curl session
    curl_handle = curl_easy_init();

    // Set URL to get here
    curl_easy_setopt(curl_handle, CURLOPT_URL, argv[1]);

    /*
    // Switch on full protocol/debug output while testing
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);

    // Disable progress meter, set to 0L to enable it
    curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
    */

    // Some servers do not like requests that are made without a user-agent
    // field, so we provide one
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    // send all data to this function
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

    // Write the page body to this file handle
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)mem);

    // Get it
    res = curl_easy_perform(curl_handle);

    // Check for errors
    if(res != CURLE_OK)
    {
        fprintf(stderr, "curl_easy_perform() failed: %s\n",
            curl_easy_strerror(res));
    }
    else
    {
        printf("%lu bytes retrieved\n", (unsigned long)mem->size);
        printf("%s", mem->memory);
    }

    // Free MemoryStruct
    free(mem->memory);
    free(mem);

    // Cleanup curl stuff
    curl_easy_cleanup(curl_handle);

    curl_global_cleanup();

    return 0;
}
