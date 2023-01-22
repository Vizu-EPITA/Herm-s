#include <./../../../include/crawler/url_server/url_server.h>
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
