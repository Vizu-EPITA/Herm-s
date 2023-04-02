#include "../main.h"
#include "./../../include/crawler/crawler.h"
#include "../../include/crawler/repository.h"
#include "../../include/crawler/store_server.h"
#include "../../include/crawler/url_server.h"
#include <err.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <stdio.h>
#include <string.h>

int max_con = 200;

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

void *crawler(void* arg)
{
	thread_data *thr_data = (thread_data *) arg;

	int max_requests = 10;

	curl_global_init(CURL_GLOBAL_ALL);
	CURLM *multi_handle = curl_multi_init();
    curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, max_con);
    curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, 200);

	// sets html start page
	URLStruct *urlStruct = pop_url(thr_data->queue_url);
	for(size_t i = 0; i < urlStruct->count; i++)
	{
		curl_multi_add_handle(multi_handle, make_handle(urlStruct->url[i]));
	}
	free_urlstruct(urlStruct);

	int msgs_left;
	int pending = 0;
	int complete = 0;
	int still_running = 1;
	while(1)
	{
		if(still_running == 0)
		{
			urlStruct = pop_url(thr_data->queue_url);
			for(size_t i = 0; i < urlStruct->count; i++)
			{
				curl_multi_add_handle(multi_handle, make_handle(urlStruct->url[i]));
			}
			pending += urlStruct->count;
			free_urlstruct(urlStruct);
			still_running = 1;
		}

		int numfds;
		curl_multi_wait(multi_handle, NULL, 0, 1000, &numfds);
		curl_multi_perform(multi_handle, &still_running);

		// See how the trasnfers went
		CURLMsg *m = NULL;
		while((m = curl_multi_info_read(multi_handle, &msgs_left)))
		{
			if(m->msg == CURLMSG_DONE)
			{
				CURL *handle = m->easy_handle;
				char *url;
				MemoryStruct *mem;
				curl_easy_getinfo(handle, CURLINFO_PRIVATE, &mem);
				curl_easy_getinfo(handle, CURLINFO_EFFECTIVE_URL, &url);
				if(m->data.result == CURLE_OK)
				{
					long res_status;
					curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &res_status);
					if(res_status == 200)
					{
						// SAVE
						//char filename[33];
						//sprintf(filename, "%d", ht_search(thr_data->table_docID, url));
						int32_t docID = ht_search(thr_data->table_docID, url);
						if (docID != -1)
						{
							save(url, strlen(url), mem->buf, mem->size, docID);
							add_file(thr_data->queue_file, docID);
						}
						char *ctype;
						curl_easy_getinfo(handle, CURLINFO_CONTENT_TYPE, &ctype);
						printf("CRAWLER: [%d] HTTP 200 (%s): %s\n", complete, ctype, url);
						if(is_html(ctype) && mem->size > 100)
						{
							if(pending < max_requests && thr_data->queue_url->first != NULL)
							{
								urlStruct = pop_url(thr_data->queue_url);
								for(size_t i = 0; i < urlStruct->count; i++)
								{
									curl_multi_add_handle(multi_handle, make_handle(urlStruct->url[i]));
								}
								pending += urlStruct->count;
								free_urlstruct(urlStruct);
								still_running = 1;
							}
						}
					}
					else
					{
						printf("CRAWLER: [%d] HTTP %d: %s\n", complete, (int) res_status, url);
					}
				}
				else
				{
					printf("[%d] Connection failure: %s\n", complete, url);
				}
				curl_multi_remove_handle(multi_handle, handle);
				curl_easy_cleanup(handle);
				free(mem->buf);
				free(mem);
				complete++;
				pending--;
			}
		}
	}
	curl_multi_cleanup(multi_handle);
}
