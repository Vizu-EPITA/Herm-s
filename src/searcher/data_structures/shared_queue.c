#include <err.h>
#include <stdlib.h>
#include "shared_queue.h"

shared_queue* shared_queue_new()
{
    struct shared_queue *nSQueue = malloc(sizeof(struct shared_queue));
    if (nSQueue == NULL)
        err(1, "shared_queue.c: Could not allocate the memory for the creation of a shared_queue");
    if (sem_init(&nSQueue->lock, 0, 1) != 0)
        err(1, "shared_queue.c: Could not initialize the lock semaphore");
    if (sem_init(&nSQueue->size, 0, 0) != 0)
        err(1, "shared_queue.c: Could not initialize the size semaphore");
    //struct queue *nQueue = malloc(sizeof(struct queue));
    //if (nQueue == NULL)
        //err(1, "shared_queue.c: Could not allocate the memory for the creation of a queue");
    nSQueue->queue = NULL;
    return nSQueue;
}


void shared_queue_push(shared_queue* sq, int val)
{
    if (sem_wait(&sq->lock) != 0)
        err(1, "shared_queue.c: Could not wait for the lock semaphore");
    sq->queue = queue_push(sq->queue, val);
    if (sem_post(&sq->lock) != 0)
        err(1, "shared_queue.c: Could not post the lock semaphore");
    if (sem_post(&sq->size) != 0)
        err(1, "shared_queue.c: Could not post the size semaphore");
}

int shared_queue_pop(shared_queue* sq)
{
    if (sem_wait(&sq->size) != 0)
        err(1, "shared_queue.c: Could not wait for the size semaphore");
    if (sem_wait(&sq->lock) != 0)
        err(1, "shared_queue.c: Could not wait for the lock semaphore");
    int val;
    sq->queue = queue_pop(sq->queue, &val);
    if (sem_post(&sq->lock) != 0)
        err(1, "shared_queue.c: Could not post for the lock semaphore");
    return val;
}

void shared_queue_destroy(shared_queue* sq)
{
    queue *start = sq->queue;
    queue **pstart = &start;
    queue_empty(pstart);
    if (sem_destroy(&sq->lock) != 0)
        err(1, "shared_queue.c: Could not destroy for the lock semaphore");
    if (sem_destroy(&sq->size) != 0)
        err(1, "shared_queue.c: Could not destroy for the size semaphore");
    free(sq);
}
