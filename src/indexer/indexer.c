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

size_t parseLink(char *page, char *linkBuf, size_t *stepForward)
{
    size_t len = 1;
    page++;
    *stepForward++;
    if (*page == 'r')
    {
        page++;
        *stepForward++;
        if (*page == 'e')
        {
            page++;
            *stepForward++;
            if (*page == 'f')
            {
                while (*page != 0 && *page != '\"')
                {
                    page++;
                    *stepForward++;
                }
                page++;
                *stepForward++;
                while (*page != 0 && *page != '\"')
                {
                    *linkBuf = *page;
                    linkBuf++;
                    page++;
                    len++;
                    *stepForward++;
                }
            }
        }
    }
    return len;
}

void parseText(char *page)
{
    char *wordBuf = malloc(sizeof(char)*100);
    char *linkBuf = malloc(sizeof(char)*300);
    size_t wordLen;
    size_t linkLen; 
    size_t stepForward;
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
            {
                if (*page == 'h')
                {
                    linkLen = parseLink(page, linkBuf, &stepForward);
                    if (linkLen > 1)
                    {
                        printf("%i\n", linkLen);
                        printWord(linkBuf, linkLen);
                    }
                    page += stepForward;
                    stepForward = 0;
                }
                else
                    page++;
            }
        }
        // End of tag
        //else if (*page == '>')
            //*page++;
        else if (*page >= '0' && *page <= '9' ||
                 *page >= 'A' && *page <= 'Z' ||
                 *page >= 'a' && *page <= 'z')
        {
            wordLen = parseWord(page, wordBuf);
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
    char page[] = "printOutside1 <href=\"printLink1\"> <s>";
    parseText(page);
    return 0;
}
