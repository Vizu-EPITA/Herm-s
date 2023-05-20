#include "main.h"
#include "../include/crawler/url_server.h"
#include "../include/crawler/repository.h"
#include "../include/indexer/indexer.h"
#include "../tools/hash_table.h"
#include "../include/crawler/crawler.h"
#include "./searcher/data_structures/graph.h"
#include "../tools/inverted_table.h"
#include <pthread.h>
#include <err.h>

int main(int argc, char *argv[])
{
	if(argc != 2)
		errx(EXIT_FAILURE, "wrong usage");
	
	thread_data *thr_data = malloc(sizeof(thread_data));
	if(thr_data == NULL)
		errx(EXIT_FAILURE, "Not enough memory!");

	pthread_t thr_crawler;
	pthread_t thr_indexer;

	thr_data->limit = strtoul(argv[1], NULL, 10);

	thr_data->table_docID = create_table(400);
	thr_data->table_wordID = create_table(1000);

	thr_data->table_inverted = it_create_table(1000);

	thr_data->queue_url = init_url_queue();
	thr_data->queue_file = init_file_queue();

	thr_data->graph = graphInit(1);

	pthread_create(&thr_crawler, NULL, crawler, (void *)thr_data);
	pthread_create(&thr_indexer, NULL, indexer, (void *)thr_data);

	ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Main_Page", 0);
	/*
   	ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Mathematics", 1);
   	ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Physics", 2);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Elementary_particle", 3);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Particle_physics", 4);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Fundamental_interaction", 5);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Electromagnetism", 6);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Atom", 7);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Electron", 8);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Particle_accelerator", 9);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Radionuclide", 10);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Ionizing_radiation", 11);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Gamma_ray", 12);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Electromagnetic_radiation", 13);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Wave", 14);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Quantity", 15);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Mass", 16);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Physical_object", 17);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Classical_mechanics", 18);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Macroscopic_scale", 19);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Micrometre", 20);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Millimetre", 21);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Metre", 22);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/International_System_of_Units", 23);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Second", 24);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Minute", 25);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Hour", 26);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/International_Atomic_Time", 27);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Leap_second", 28);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Earth's_rotation", 29);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Millisecond", 30);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Picosecond", 31);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Nanosecond", 32);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Microsecond", 33);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Time", 34);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Event_(particle_physics)", 35);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Bubble_chamber", 36);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Superheating", 37);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Thermodynamics", 38);
    ht_insert(thr_data->table_docID,"https://en.wikipedia.org/wiki/Steam_engine", 39);
	*/


	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Main_Page");
	/*
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Mathematics");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Physics");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Elementary_particle");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Particle_physics");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Fundamental_interaction");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Electromagnetism");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Atom");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Electron");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Particle_accelerator");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Radionuclide");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Ionizing_radiation");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Gamma_ray");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Electromagnetic_radiation");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Wave");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Quantity");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Mass");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Physical_object");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Classical_mechanics");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Macroscopic_scale");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Micrometre");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Millimetre");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Metre");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/International_System_of_Units");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Second");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Minute");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Hour");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/International_Atomic_Time");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Leap_second");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Earth's_rotation");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Millisecond");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Picosecond");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Nanosecond");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Microsecond");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Time");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Event_(particle_physics)");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Bubble_chamber");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Superheating");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Thermodynamics");
	add_url(thr_data->queue_url, "https://en.wikipedia.org/wiki/Steam_engine");
	*/

	pthread_exit(NULL);
}
