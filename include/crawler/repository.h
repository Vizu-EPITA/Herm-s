#ifndef REPOSITORY_H
#define REPOSITORY_H

#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

typedef struct FileStruct{
	char *name;
	struct FileStruct *next;
} FileStruct;

typedef struct FileQueue{
	sem_t lock;
	sem_t size;
	FileStruct *first;
} FileQueue;

FileQueue *init_file_queue();

void free_filestruct(FileStruct *fileStruct);

void free_file_queue(FileQueue *q);

void add_file(FileQueue *q, char *file);

char *pop_file(FileQueue *q);


#endif
