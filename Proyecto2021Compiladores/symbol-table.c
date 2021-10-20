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

/* inorder tree traversal */
void tableGen(bNode *root, sNode **symbolTable)
{
    /* set current to root of binary tree */
    bNode *current = root;
    sNode *s = NULL; /* Initialize stack s for tree traversal */
    list *currentTableLevel = NULL;
    int done = 0;
    
    while (!done)
    {
        /* Reach the left most bNode of the current bNode */
        if(current != NULL) 
        {
            tpush(&s, current);											
            current = current->left;
        }
        else 
        {
            if (!emptyStack(s)) 
            {
                current = tpop(&s);
                enum tLabel currentLabel = current->fact;

                if (currentLabel == VAR) 
                {   
                    if (currentLabel == BLOCK) 
                    {   
                        /*if (el metodo posee parametros) {
                            list *paramLevel = (list *) malloc(sizeof(list));
                            insert(parametros, &paramLevel); 
                            push(paramLevel, symbolTable);
                        } */

                        // gen method level y cargarlo antes de push
                        list *newlevel = NULL; 
                        currentTableLevel = newlevel;
                        push(currentTableLevel, symbolTable);
                    }
                    /* if (se cierra el bloque) se hacen 2 pop (1 de param level otro del method level)
                    pop(symbolTable); param level
                    currentTableLevel = pop(symbolTable);  me tiene que devolver el tope del nivel que contiene al metodo */
                    insert(currentLabel, currentTableLevel);
                }

                /* Visit right subtree */
                current = current->right;
            }
            else
                done = 1;
        }
    } /* end of while */
}	





