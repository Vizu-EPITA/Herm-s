/*
 * Author: Izoulet Aurélien
 * Purpose: Strings implementation
 * Language: C.
 */

#include "string.h"

char *string_concat(char *str1, char *str2)
{
    char *str = malloc(strlen(str1) + strlen(str2) + 1);
    strcpy(str, str1);
    strcat(str, str2);
    return str;
}

char *string_to_heap(char *string, size_t string_size)
{
    char *string_to_heap = malloc(sizeof(char) * string_size);
    strcpy(string_to_heap, string);
    return string_to_heap;
}
