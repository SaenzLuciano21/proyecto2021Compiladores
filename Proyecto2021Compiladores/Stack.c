#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "List.c"

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

sNode * upLevel(sNode **stack) 
{
    if(*stack != NULL)
    {
        return (*stack) -> next;
    }
    else 
    {
        printf("The stack is empty.\n");
        getchar();
        exit(0);
    }
}

/* The function returns true if stack is empty, otherwise false */
int emptyStack(sNode *top)
{
    return (top == NULL);
}

void insertStack(sNode **stack, info *inf)
{
    insertList(&(*stack)->head, inf);
}

// retorna el nodo del identificador
list * containsStack(sNode **stack, info *inf)
{
    sNode **aux = stack;
    if (stack != NULL) {
        // capturo el nodo que me devuelve containsList()
        list *result = containsList((*aux)->head, inf);
        if (result == NULL) {
            containsStack(upLevel(aux), inf);  // llamamos recursivamente con un nivel anterior
        } else {
            return result;
        }
    }
    // revisar si podemos devolver NULL, O 0
    return NULL;
}