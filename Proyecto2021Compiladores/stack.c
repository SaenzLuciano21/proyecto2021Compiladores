#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Structs.h"

// Append a new element to the top of the stack
void push(info *inf, lnode **stack) 
{   

    lnode *new = (lnode*) malloc(sizeof(lnode)); 
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
info * pop(lnode **stack) 
{
    if(*stack != NULL){
        lnode *tempPtr = *stack;
        info *res = tempPtr -> infoN;
        *stack = (*stack) -> next;
        free(tempPtr);
        return res;
    }
    else{
        printf("The stack is empty.\n");
        getchar();
        exit(0);
    }
}
// Returns the info at the top of the stack
info* top(lnode* stack) 
{
    if(stack != NULL){
        return stack -> infoN;
    }
    else{
        printf("The stack is empty.\n");
    }
}
