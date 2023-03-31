#ifndef MAIN_H
#define MAIN_H

typedef struct thread_data
{
	HashTable *table_docID;
	HashTable *table_wordID;
	URLQueue *queue_url;
	FileQueue *queue_file;
} thread_data;

#endif
