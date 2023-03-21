#include "../../include/indexer/indexer.h"
#include <err.h>
#include <stdlib.h>
#include <stdio.h>


int main()
{
	char* str = decompress_file("../crawler/repository/3");
	if(str == NULL)
		errx(EXIT_FAILURE, "failed to open the file");
	printf("%s", str);
	free(str);
	return 0;
}
