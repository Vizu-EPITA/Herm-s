/*
 * Author: Izoulet Aurélien
 * Purpose: Oriented Graph implementation 
 * Language: C.
 */

#include "stack.h"

int is_empty(struct stack *stack)
{
    if(stack->last == NULL)
    {
        return 1;
    }
    return 0;
}