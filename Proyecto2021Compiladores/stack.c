#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Structs.h"

// Append a new element to the top of the stack
void push(list *lvl, sNode **stack) 
{   
    sNode *new = (sNode*) malloc(sizeof(sNode)); 
    if(new == NULL)
    {
        printf("Stack Overflow \n");
        getchar();
        exit(0);
    }	
    new -> head = lvl; 
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