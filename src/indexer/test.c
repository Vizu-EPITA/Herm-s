#include "../../include/indexer/indexer.h"
#include <err.h>
#include <stdlib.h>
#include <stdio.h>


int main()
{
	htmlStruct *htmlInfo = decompress_file("../crawler/repository/3");
	if(htmlInfo == NULL)
		errx(EXIT_FAILURE, "failed to open the file");
	printf("%s", htmlInfo->page);
	free(htmlInfo->url);
	free(htmlInfo->page);
	free(htmlInfo);
	return 0;
}
