#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tree.c"

/* Inserts a new element in the list */
void insert(info *inf, list **l) 
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
int contains(list *plist, char *name)
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
/*
                        // gen method level y cargarlo antes de push
                        list *newlevel = NULL; 
                        currentTableLevel = newlevel;
                        push(currentTableLevel, symbolTable);*/

void check(bNode *root, sNode **symbolTable) {
    
    /* seteamos current con la raiz del arbol */
    bNode *current = root;
    // nivel 0
    list *newLevel;
    if (current != NULL) {
        // caso program
        if (current == PROG) {  
            // caso "base" donde generamos el primer nivel de la tabla y lo apilamos en la stack
            newLevel = (list *) malloc(sizeof(list)); // inicializamos la lista que representa el nivel 0
            push(newLevel, symbolTable);

            // visitamos subarbol izq y luego el subarbol derecho
            check(current->left, symbolTable);
            check(current->right, symbolTable);
            // no cerrar nivel, pues el nivel 0 es el que se mantiene
        } else {
            if (current == LISTVAR) { // si tenemos una lista de variables declaradas, vamos recorriendo e insertando
                check(current->right, symbolTable); // la lista de declaraciones se extiende por la derecha
                insert(current->left->infoN, &newLevel); // cada var se aloja en los hijos izq
            } else {
            
            }
        }


    } // caso current == NULL
    else {

    }
}



