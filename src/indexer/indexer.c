#include <err.h>
#include <string.h>
#include <stdlib.h>

void parseWord(char *page, char *wordBuf /*,[HTBLE]*/)
{
}

void parseText(char *page)
{
    char *wordBuf = malloc(sizeof(char)*100);
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
            page++;
    }
    // End of tag
    else if (*page == '>')
        *page++;
    else if (*page >= '0' && *page <= '9' ||
             *page >= 'A' && *page <= 'Z' ||
             *page >= 'a' && *page <= 'z')
        parseWord(page, wordBuf);
}
}
