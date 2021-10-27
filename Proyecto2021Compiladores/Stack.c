#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Structs.c"

// Append a new element to the top of the stack
void push(sNode **stack) 
{   
    list *l = (list*) malloc(sizeof(list));
    sNode *new = (sNode*) malloc(sizeof(sNode)); 
    if(new == NULL)
    {
        printf("Stack Overflow \n");
        getchar();
        exit(0);
    }	
    new -> head = l; 
    new -> next = *stack;  
    (*stack) = new;  
}

// Remove element from the top of the stack
list * pop(sNode **stack) 
{
    if(*stack != NULL)
    {
        sNode *tempPtr = *stack;
        list *res = tempPtr -> head;
        *stack = (*stack) -> next;
        free(tempPtr);
        return res;
    }
    else 
    {
        printf("The stack is empty.\n");
        getchar();
        exit(0);
    }
}
// Returns the current lvl 
list * top(sNode **stack) 
{
    if(*stack != NULL)
    {
        sNode *tempPtr = *stack;
        list *res = tempPtr -> head;
        return res;
    }
    else 
    {
        printf("The stack is empty.\n");
    }
}

/* The function returns true if stack is empty, otherwise false */
int emptyStack(sNode *top)
{
    return (top == NULL);
}

int insertStack(sNode **stack, info *inf)
{
    if (!containsList((*stack)->head, inf)) {
        insertList(inf, (*stack)->head);
        return 1;
    } else {
        return 0;
    }
}

int containsStack(sNode **stack, info *inf)
{
    sNode **s = stack;
    if (stack != NULL) {
        if (!containsList((*s)->head, inf->name)) {
            pop(s); 
            containsStack(s, inf);  /*nivel anterior de s*/
        } else {
            return 1;
        }
    } else {
        return 0;
    }

}