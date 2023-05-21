#include "linked_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <err.h>

LNode *createNode()
{
	LNode *newLNode = malloc(sizeof(LNode));
	if (newLNode == NULL) errx(1, "COuld not create a new node");
	return newLNode;
}

// Function to add an element at the end of the list
void addElement(LinkedList* list, char *word)
{
    LNode* newLNode = (LNode*)malloc(sizeof(LNode));
    newLNode->word = word;
	newLNode->wordId = -1;
    newLNode->next = NULL;

    if (list->head == NULL) {
        list->head = newLNode;
        return;
    }

    LNode* curr = list->head;
    while (curr->next != NULL) {
        curr = curr->next;
    }

    curr->next = newLNode;
}

// Function to get an element at a specific index
LNode *getElement(LinkedList* list, int index)
{
    LNode* curr = list->head->next;
    int count = 0;

    while (curr != NULL) {
        if (count == index) {
            return curr;
        }
        count++;
        curr = curr->next;
    }

    return NULL;
}

// Function to free the whole list
void freeList(LinkedList* list)
{
    LNode* curr = list->head;
    LNode* next;

    while (curr != NULL) {
        next = curr->next;
        free(curr->word);
		free(curr);
        curr = next;
    }

	free(list);
}
