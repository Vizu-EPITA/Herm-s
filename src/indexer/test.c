#include "../../include/indexer/indexer.h"
#include <err.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <wget.h>


int main()
{
	for(int i = 0; i < 40; i++)
	{
		htmlStruct *htmlInfo = decompress_file(i);
		if(htmlInfo == NULL)
			errx(EXIT_FAILURE, "failed to open the file");

		parseText(htmlInfo);


/*
		wget_iri_t *base = wget_iri_parse("https://en.wikipedia.org/wiki/Mathematics", NULL);
		char *relative_url = "https://www.samsung.com/fr/";
		wget_buffer_t *buf = wget_buffer_alloc(400);

		printf("%s\n", wget_iri_relative_to_abs(base, relative_url, strlen(relative_url), buf));
		*/

		//printf("%s", htmlInfo->page);
		free(htmlInfo->url);
		free(htmlInfo->page);
		free(htmlInfo);
	}

	//wget_buffer_free(&buf);
	//wget_iri_free(&base);
	return 0;
}
