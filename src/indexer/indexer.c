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
    while (*page != 0 &&
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

size_t parseLink(char *page, char *linkBuf)
{
    size_t len = 0;
    while (*page != 0 && *page != '\"')
    {
        *linkBuf = *page;
        linkBuf++;
        page++;
        len++;
    }
    return len;
}

void parseText(char *page)
{
    char *wordBuf = malloc(sizeof(char)*100);
    char *linkBuf = malloc(sizeof(char)*300);
    size_t wordLen;
    size_t linkLen; 
    while (*page != 0)
    {
        // End of file (EOF)
        if (*page == 0)
            return;
        // Tag detected
        else if (*page == '<')
        {
            // Search end of tag or EOF
            while (*page != '>' && *page != 0)
            {
                if (*page == 'h' && strncmp(page, "href=\"", 6) == 0)
                {
                    while (*page != 0 && *page != '\"')
                    {
                        page++;
                    }
                    page++;
                    linkLen = parseLink(page, linkBuf);
                    if (linkLen > 1)
                    {
                        //printf("%i\n", linkLen);
                        //EDIT
                        printWord(linkBuf, linkLen);
                    }
                    page += linkLen;
                }
                else
                    page++;
            }
        }
        // End of tag
        else if (*page >= '0' && *page <= '9' ||
                 *page >= 'A' && *page <= 'Z' ||
                 *page >= 'a' && *page <= 'z')
        {
            wordLen = parseWord(page, wordBuf);
            //EDIT
            printWord(wordBuf, wordLen);
            page += wordLen;
        }
        else
            page++;
    }
    free(wordBuf);
    free(linkBuf);
}

int main()
{
    char *page = malloc(sizeof(char)*250);
    char buf[] = "<href=\"printLink1\"> dasdasdasd <sdass>";
    strcpy(page, buf);
    parseText(page);
    free(page);
    return 0;
}
