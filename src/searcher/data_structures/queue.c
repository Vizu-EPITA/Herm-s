#include <err.h>
#include <stdlib.h>
#include "queue.h"

queue* queue_push(queue* start, int val)
{
    struct queue *newNode = malloc(sizeof(struct queue));
    if (newNode == NULL)
        errx(1, "queue.c: Could not allocate memory to the new element.");
    newNode->val = val;
    if (start == NULL) // The queue is empty
    {
        newNode->next = newNode;
    }
    else
    {
        newNode->next = start->next;
        start->next = newNode;
    }
    return newNode;
}

queue* queue_pop(queue* start, int *pval)
{
    if (start == NULL)
        return NULL;
    struct queue *pop = start->next;
    *pval = pop->val;
    if (start != pop)
    {
        start->next = pop->next;
        free(pop);
        return start;
    }   
    else
    {
		free(pop);
    	return NULL;
    }
}

void queue_empty(queue** pstart)
{
    int val;
    while (*pstart != NULL)
        *pstart = queue_pop(*pstart, &val);
}
