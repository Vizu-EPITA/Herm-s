#include "linked_list.h"
#include <stdlib.h>
#include <err.h>

void free_list(linked_list *list)
{
	linked_list *tmp = list;
	while (list != NULL)
	{
		list = list->next;
		if (tmp->word != NULL) free(tmp->word);
		free(tmp);
		tmp = list;
	}
}

void push_list(linked_list *list, char *word)
{
	if (list == NULL) return;
	while (list->next != NULL)
	{
		list = list->next;
	}
	linked_list *new_node = malloc(sizeof(linked_list));
	if (new_node == NULL) errx(1, "linked_list.c: Could not create a new node to push");
	new_node->word = word;
	new_node->wordId = -1;
	new_node->next = NULL;
	list->next = new_node;
}
