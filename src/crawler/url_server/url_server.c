#include <./../../../include/crawler/url_server/url_server.h>
#include <string.h>
#include <err.h>

URLQueue *InitQueue()
{
    URLQueue *q = malloc(sizeof(URLQueue));
    if(q == NULL)
    {
        errx(EXIT_FAILURE, "Out of memory\n");
    }

    q->size = 0;
    q->first = 0;

    return q;
}

void AddURL(char *str, URLQueue *queue)
{
    URLStruct *urlStruct = malloc(sizeof(URLStruct));
    if(urlStruct == NULL)
    {
        errx(EXIT_FAILURE, "Out of memory\n");
    }  

    strcpy(urlStruct->url, str);
    if(queue->size == 0)
    {
        urlStruct->prev = urlStruct;
        urlStruct->next = urlStruct;
        urlStruvt->first = urlStruct;
    }
    else
    {
        urlStruct->prev = queue->first->prev;
        urlStruct->next = queue->first->next;
        queue->first->prev = urlStruct;
        queue->first = urlStruct; 
    }
    queue->size++;
}
