#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Structs.h"

// Append a new element to the top of the stack
void push(info *inf, lnode **stack) 
{
    lnode *new = (lnode*) malloc(sizeof(lnode)); 
    new -> infoN = inf; 
    new -> next = *stack;  
    (*stack) = new;  
}

// Remove element from the top of the stack
void pop(lnode **stack) 
{
    if(*stack != NULL){
        lnode *tempPtr = *stack;
        *stack = (*stack) -> next;
        free(tempPtr);
        printf("element popped!\n");
    }
    else{
        printf("The stack is empty.\n");
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
