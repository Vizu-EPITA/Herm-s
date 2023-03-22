#include <err.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

void printWord(char *wordBuf, size_t len)
{
    for (int i = 0; i < len; i++)
    {
        printf("%c", *wordBuf);
        wordBuf++;
    }
    printf("\n");
}

size_t parseWord(char *page, char *wordBuf /*,[HTBLE]*/)
{   
    size_t len = 0;
    while(*page != 0 &&
          *page >= '0' && *page <= '9' ||
          *page >= 'A' && *page <= 'Z' ||
          *page >= 'a' && *page <= 'z')
    {
        *wordBuf = *page;
        wordBuf++;
        page++;
        len++;
    }
    return len;
}

void parseText(char *page)
{
    char *wordBuf = malloc(sizeof(char)*100);
    size_t len;
    while (*page != 0)
    {
    //printf("parseText loop\n");
    // End of file (EOF)
        if (*page == 0)
            return;
        // Tag detected
        else if (*page == '<')
        {
            // Search end of tag or EOF
            while (*page != '>' && *page != 0)
            page++;
        }
        // End of tag
        //else if (*page == '>')
            //*page++;
        else if (*page >= '0' && *page <= '9' ||
                 *page >= 'A' && *page <= 'Z' ||
                 *page >= 'a' && *page <= 'z')
        {
            len = parseWord(page, wordBuf);
            printWord(wordBuf, len);
            page += len;
        }
        else
            page++;
    }
    free(wordBuf);
}

int main()
{
    char page[] = "<test> This has to be printed<not this> \n Here is <some > more tests";
    parseText(page);
    return 0;
}
