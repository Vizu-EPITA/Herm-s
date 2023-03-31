#include "../../include/crawler/repository.h"
#include <stdlib.h>
#include <stdio.h>
#include <err.h>
#include <string.h>

FileQueue *init_file_queue()
{
	// Allocating memory for FileQueue struct
	FileQueue *q = malloc(sizeof(FileQueue));
	if(q == NULL)
		errx(EXIT_FAILURE, "Out of memory");

	// Init parameters
	// Init semaphores
	if(sem_init(&q->lock, 0, 1) != 0)
		errx(EXIT_FAILURE, "Semaphores initialization error");
	if(sem_init(&q->size, 0, 0) != 0)
		errx(EXIT_FAILURE, "Semaphores initialization error");
	
	q->first = NULL;

	return q;
}

void free_filestruct(FileStruct *fileStruct)
{
	free(fileStruct->name);
	free(fileStruct);
}

void free_file_queue(FileQueue *q)
{
	if(q->first != NULL)
	{
		FileStruct *first = q->first;
		FileStruct *tmp1 = q->first->next;
		FileStruct *tmp2;
		while(tmp1 != first)
		{
			tmp2 = tmp1;
			tmp1 = tmp1->next;
			free_filestruct(tmp2);
		}
		free(first);
	}
	
	if(sem_destroy(&q->lock) != 0)
	{
		errx(EXIT_FAILURE, "Free semaphores error");
	}

	if(sem_destroy(&q->size) != 0)
	{
		errx(EXIT_FAILURE, "Free semaphores error");
	}
	
	free(q);
}

void add_file(FileQueue *q, char *file)
{
	// Lock the queue
	sem_wait(&q->lock);

	char *filename = calloc(strlen(file) + 1, sizeof(char));
	if(filename == NULL)
		errx(EXIT_FAILURE, "Out of memory");

	strcpy(filename, file);

	FileStruct *fileStruct = malloc(sizeof(FileStruct));
	if(fileStruct == NULL)
		errx(EXIT_FAILURE, "Out of memory");
	
	fileStruct->name = filename;
	
	if(q->first == NULL)
	{
		fileStruct->next = fileStruct;	
	}
	else
	{
		fileStruct->next = q->first->next;
		q->first->next = fileStruct;
	}
	q->first = fileStruct;

	// Unlock the queue
	sem_post(&q->lock);
	sem_post(&q->size);
}

char *pop_file(FileQueue *q)
{
	// Decrementing the size
	sem_wait(&q->size);

	// Lock the queue
	sem_wait(&q->lock);

	FileStruct *tmp;

	if(q->first->next == q->first)
	{
		tmp = q->first;
		q->first = NULL;
	}
	else
	{
		tmp = q->first->next;
		q->first->next = tmp->next;
	}

	char *file = tmp->name;
	free(tmp);

	//Unlocking the queue
	sem_post(&q->lock);

	return file;
}
