#include "./../../include/crawler/crawler.h"
#include <err.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <stdio.h>
#include <string.h>

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    MemoryStruct *mem = (MemoryStruct *)userp;

    char *ptr = realloc(mem->buf, mem->size + realsize + 1);
    if(!ptr)
    {
        // Out of memory!
        printf("not enough memory (realloc returned NULL)\n");
        return 0;
    }

    mem->buf = ptr;
    memcpy(&(mem->buf[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->buf[mem->size] = 0;

    return realsize;
}

int is_html(char *ctype)
{
    return ctype != NULL && strlen(ctype) > 10 && strstr(ctype, "text/html");
}

CURL *make_handle(char *url)
{
    CURL *handle = curl_easy_init();

    // Important: use HTTP2 over HTTPS
    curl_easy_setopt(handle, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2TLS);
    curl_easy_setopt(handle, CURLOPT_URL, url);
    
    // Buffer body
    MemoryStruct *mem = malloc(sizeof(MemoryStruct));
    mem->size = 0;
    mem->buf = malloc(1);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, mem);
    curl_easy_setopt(handle, CURLOPT_PRIVATE, mem);

    // For completeness
    curl_easy_setopt(handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
    curl_easy_setopt(handle, CURLOPT_FOLLOWLOCATION, 1L);

    return handle;
}
/*
MemoryStruct *download(char *url)
{
    CURL *curl_handle;
    CURLcode res;

    MemoryStruct *mem = malloc(sizeof(MemoryStruct));
    if(mem == NULL)
    {
        errx(EXIT_FAILURE, "Out of memory\n");
    }

    mem->buf = malloc(1);    // Will be grown as needed by the realloc
    mem->size = 0;              // No data at this point

//    curl_global_init(CURL_GLOBAL_ALL);

    // Init the curl session
    curl_handle = curl_easy_init();

    // Set URL to get here
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);

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

    // Free MemoryStruct
//    free(mem->buf);
//    free(mem);

    // Cleanup curl stuff
    curl_easy_cleanup(curl_handle);

//    curl_global_cleanup();

    return mem;
}
*/
