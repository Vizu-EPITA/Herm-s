#ifndef LINKED_LIST
#define LINKED_LIST

typedef struct linked_list {
	char *word;
	int wordId;
	struct linked_list *next;
} linked_list;

void free_list(linked_list *list);

void push_list(linked_list *list, char *word);

#endif
