#ifndef LINKED_LIST
#define LINKED_LIST
#include <stddef.h>
#include <stdint.h>
// LNode structure
typedef struct LNode {
    char *word;
	int32_t wordId;
    struct LNode* next;
} LNode;

// Linked list structure
typedef struct {
    LNode* head;
} LinkedList;

LNode *createNode();
void addElement(LinkedList* list, char *word);
LNode *getElement(LinkedList* list, int index);
void freeList(LinkedList* list);

#endif
