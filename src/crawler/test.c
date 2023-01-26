#include <stdio.h>
#include <stdlib.h>
#include "../../include/crawler/url_server.h"
#include "../../include/crawler/crawler.h"
#include "../../include/crawler/store_server.h"

int main()
{
    URLQueue *queue = init_url_queue();

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

    while(queue->size != 0)
    {
        char *url = pop_url(queue);
        MemoryStruct *mem = download(url);
        save(url+30, mem->memory);

        free(url);
        free(mem->memory);
        free(mem);
    }

    free_url_queue(queue);

    return 0;
}
