#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Tree.c"

/* Inserts a new element in the list */
void insertList(info *inf, list **l) 
{   
    /* checks memory availability */
    list *new = (list*) malloc(sizeof(list)); 
    if(new == NULL)
    {
        printf("Stack Overflow \n");
        getchar();
        exit(0);
    }	
    new -> infoN = inf; 
    new -> next = *l;  
    (*l) = new;  
}

/* checks if the st contains a given element */
int containsList(list *plist, char *name)
{
    if(plist != NULL)
    {
        list *aux = malloc(sizeof(list));
        aux = plist;
        while (aux != NULL)
        {
            if (strcmp(aux->infoN->name, name) == 0)
            {
                return 1;
            }
            aux = aux->next;
        }
    }
    return 0;
}


