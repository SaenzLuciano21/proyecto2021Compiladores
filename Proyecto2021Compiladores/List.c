#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Tree.c"

/* Inserts a new element in the list */
void insertList(list **l, info *inf) 
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
int containsList(list *plist, info *inf)
{
    if(plist != NULL)
    {
        list *aux = malloc(sizeof(list));
        aux = plist;
        while (aux != NULL)
        {
            if (strcmp(aux->infoN->name, inf->name) == 0)
            {
                return 1;
            }
            aux = aux->next;
        }
    }
    return 0;
}

/* necesito que ademas de contarme la cantidad de parametros del nivel
chequee que los tipos de cada parametro sean consistentes, es decir que coincidan. Si encuentra 1 parametro
con tipo diferente getchar(); exit(0);*/
int listLength(list *plist) {
    int counter = 0;
    list *aux = plist;
    while (aux != NULL) {
        counter++;
        aux = aux->next;
    }
    return counter;
}


