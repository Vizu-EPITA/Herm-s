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
