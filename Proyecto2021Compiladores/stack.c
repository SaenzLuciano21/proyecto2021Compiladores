#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Structs.h"

// Append a new element to the top of the stack
void push(info *inf, sNode **stack) 
{   

    sNode *new = (sNode*) malloc(sizeof(sNode)); 
    if(new == NULL)
    {
        printf("Stack Overflow \n");
        getchar();
        exit(0);
    }	
    new -> infoN = inf; 
    new -> next = *stack;  
    (*stack) = new;  
}

// Remove element from the top of the stack
info * pop(sNode **stack) 
{
    if(*stack != NULL){
        sNode *tempPtr = *stack;
        info *res = tempPtr -> infoN;
        *stack = (*stack) -> next;
        free(tempPtr);
        return res;
    }
    else {
        printf("The stack is empty.\n");
        getchar();
        exit(0);
    }
}
// Returns the info at the top of the stack
info* top(sNode **stack) 
{
    if(*stack != NULL){
        sNode *tempPtr = *stack;
        info *res = tempPtr -> infoN;
        return res;
    }
    else {
        printf("The stack is empty.\n");
    }
}
