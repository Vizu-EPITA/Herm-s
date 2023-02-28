#include "../../include/crawler/url_server.h"
#include "../../include/crawler/crawler.h"
#include "../../include/crawler/store_server.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <err.h>
#include <curl/curl.h>

int max_con = 200;
int max_total = 20000;
int max_requests = 500;

const size_t THREAD_COUNT = 100;

/*
void* worker(void *arg)
{
    URLQueue *queue = arg;
    char *url;

    while(queue->first != NULL)
    {
        URLStruct *urlStruct = pop_url(queue);
        for(size_t i = 0; i < urlStruct->count; i++)
        {
            url = urlStruct->url[i];
            //MemoryStruct *mem = download(url);
            //save(url+30, mem->memory);

            free(mem->buf);
            free(mem);
        }
        free_urlstruct(urlStruct);
    }

    pthread_exit(NULL);
}
*/

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        errx(EXIT_FAILURE, "wrong usage");
    }
    size_t number = strtoul(argv[1], NULL, 10);
    URLQueue *queue = init_url_queue();

    for(size_t i = 0; i < number/40; i++)
    {
        add_url(queue, "https://en.wikipedia.org/wiki/Main_Page");
        add_url(queue, "https://en.wikipedia.org/wiki/Mathematics");
        add_url(queue, "https://en.wikipedia.org/wiki/Physics");
        add_url(queue, "https://en.wikipedia.org/wiki/Elementary_particle");
        add_url(queue, "https://en.wikipedia.org/wiki/Particle_physics");
        add_url(queue, "https://en.wikipedia.org/wiki/Fundamental_interaction");
        add_url(queue, "https://en.wikipedia.org/wiki/Electromagnetism");
        add_url(queue, "https://en.wikipedia.org/wiki/Atom");
        add_url(queue, "https://en.wikipedia.org/wiki/Electron");
        add_url(queue, "https://en.wikipedia.org/wiki/Particle_accelerator");
        add_url(queue, "https://en.wikipedia.org/wiki/Radionuclide");
        add_url(queue, "https://en.wikipedia.org/wiki/Ionizing_radiation");
        add_url(queue, "https://en.wikipedia.org/wiki/Gamma_ray");
        add_url(queue, "https://en.wikipedia.org/wiki/Electromagnetic_radiation");
        add_url(queue, "https://en.wikipedia.org/wiki/Wave");
        add_url(queue, "https://en.wikipedia.org/wiki/Quantity");
        add_url(queue, "https://en.wikipedia.org/wiki/Mass");
        add_url(queue, "https://en.wikipedia.org/wiki/Physical_object");
        add_url(queue, "https://en.wikipedia.org/wiki/Classical_mechanics");
        add_url(queue, "https://en.wikipedia.org/wiki/Macroscopic_scale");
        add_url(queue, "https://en.wikipedia.org/wiki/Micrometre");
        add_url(queue, "https://en.wikipedia.org/wiki/Millimetre");
        add_url(queue, "https://en.wikipedia.org/wiki/Metre");
        add_url(queue, "https://en.wikipedia.org/wiki/International_System_of_Units");
        add_url(queue, "https://en.wikipedia.org/wiki/Second");
        add_url(queue, "https://en.wikipedia.org/wiki/Minute");
        add_url(queue, "https://en.wikipedia.org/wiki/Hour");
        add_url(queue, "https://en.wikipedia.org/wiki/International_Atomic_Time");
        add_url(queue, "https://en.wikipedia.org/wiki/Leap_second");
        add_url(queue, "https://en.wikipedia.org/wiki/Earth's_rotation");
        add_url(queue, "https://en.wikipedia.org/wiki/Millisecond");
        add_url(queue, "https://en.wikipedia.org/wiki/Picosecond");
        add_url(queue, "https://en.wikipedia.org/wiki/Nanosecond");
        add_url(queue, "https://en.wikipedia.org/wiki/Microsecond");
        add_url(queue, "https://en.wikipedia.org/wiki/Time");
        add_url(queue, "https://en.wikipedia.org/wiki/Event_(particle_physics)");
        add_url(queue, "https://en.wikipedia.org/wiki/Bubble_chamber");
        add_url(queue, "https://en.wikipedia.org/wiki/Superheating");
        add_url(queue, "https://en.wikipedia.org/wiki/Thermodynamics");
        add_url(queue, "https://en.wikipedia.org/wiki/Steam_engine");
    }

    curl_global_init(CURL_GLOBAL_ALL);
    CURLM *multi_handle = curl_multi_init();
    curl_multi_setopt(multi_handle, CURLMOPT_MAX_TOTAL_CONNECTIONS, max_con);
    curl_multi_setopt(multi_handle, CURLMOPT_MAX_HOST_CONNECTIONS, 200);

    #ifdef CURLPIPE_MULTIPLEX
        curl_multi_setopt(multi_handle, CURLMOPT_PIPELINING, CURLPIPE_MULTIPLEX);
    #endif

    // sets html start page
    URLStruct *urlStruct = pop_url(queue);
    for(size_t i = 0; i < urlStruct->count; i++)
    {
        curl_multi_add_handle(multi_handle, make_handle(urlStruct->url[i]));
    }
    free_urlstruct(urlStruct);

    int msgs_left;
    int pending = 0;
    int complete = 0;
    int still_running = 1;
    while(still_running || queue->first != NULL)
    {
        int numfds;
        curl_multi_wait(multi_handle, NULL, 0, 1000, &numfds);
        curl_multi_perform(multi_handle, &still_running);

        // See how the transfers went
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
                        // Save
                        char *filename = calloc(strlen(url+30) + 7, sizeof(char));
                        sprintf(filename, "%s_%05d", url+30, complete);
                        save(filename, mem->buf);
                        free(filename);

                        char *ctype;
                        curl_easy_getinfo(handle, CURLINFO_CONTENT_TYPE, &ctype);
                        printf("[%d] HTTP 200 (%s): %s\n", complete, ctype, url);
                        if(is_html(ctype) && mem->size > 100)
                        {
                            if(pending < max_requests && (complete + pending) < max_total && queue->first != NULL)
                            {
                                urlStruct = pop_url(queue);
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
                        printf("[%d] HTTP %d: %s\n", complete, (int) res_status, url);
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


	/*
    pthread_t thr[THREAD_COUNT];
    for(size_t i = 0; i < THREAD_COUNT; i++)
    {
        if(pthread_create(&thr[i], NULL, worker, (void *)queue) != 0)
        {
            errx(EXIT_FAILURE, "pthread_create()\n");
        }
    }
    for(size_t i = 0; i < THREAD_COUNT; i++)
    {
        pthread_join(thr[i], NULL);
    }
	*/



    curl_global_cleanup();
    free_url_queue(queue);

    return 0;
}

