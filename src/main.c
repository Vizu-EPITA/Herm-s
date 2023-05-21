#include "main.h"
#include "../include/crawler/url_server.h"
#include "../include/crawler/repository.h"
#include "../include/indexer/indexer.h"
#include "../tools/hash_table.h"
#include "../include/crawler/crawler.h"
#include "../tools/graph.h"
#include "../tools/inverted_table.h"
#include "../tools/forward_table.h"
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

	thr_data->ftable_docID = ft_create_table(1000);

	thr_data->table_inverted = it_create_table(1000);

	thr_data->queue_url = init_url_queue();
	thr_data->queue_file = init_file_queue();

	thr_data->graph = graphInit(1);

	pthread_create(&thr_crawler, NULL, crawler, (void *)thr_data);
	pthread_create(&thr_indexer, NULL, indexer, (void *)thr_data);

	pthread_exit(NULL);
}
