#include "./../../include/crawler/store_server.h"
#include <stdio.h>
#include <zlib.h>
#include <stdlib.h>
#include <string.h>

void save(char *filename, char *html)
{
    FILE *fp = fopen(filename, "w");

    fputs(html, fp);

    fclose(fp);
}
