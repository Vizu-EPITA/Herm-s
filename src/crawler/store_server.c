#include "../../include/crawler/store_server.h"
#include <stdio.h>
#include <zlib.h>
#include <stdlib.h>
#include <string.h>
#include <err.h>

void save(char *filename, char *html)
{
    char repo[] = "./repository/";

    char *dest = calloc(strlen(filename) + strlen(repo) + 1, sizeof(char));
    if(dest == NULL)
    {
        errx(EXIT_FAILURE, "Out of memory\n");
    }

    strcpy(dest, repo);
    strcat(dest, filename);

    FILE *fp = fopen(dest, "w");
    if(fp == NULL)
    {
        errx(EXIT_FAILURE, "failed to open the file\n");
    }

    fputs(html, fp);

    free(dest);
    fclose(fp);
}
