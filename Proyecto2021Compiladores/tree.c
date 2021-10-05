#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Structs.h"
#define SIZE 16

/*Create bNode*/
bNode *create_bNode(enum tLabel label, info *infN, bNode *left, bNode *middle, bNode *right)
{
    bNode *new;
    new = (bNode *)malloc(sizeof(bNode));
    new->fact = label;
    new->left = left;
    new->middle = middle;
    new->right = right;
    new->infoN = infN;
    return new;
}

/*Free up memory*/
void freeMemory(bNode *tree)
{
    if (tree)
    {
        freeMemory(tree->left);
        freeMemory(tree->middle);
        freeMemory(tree->right);
        free(tree);
    }
}

/*Walk the tree*/
void inOrder(bNode *tree)
{
    if (tree)
    {   
        printf("----ENTRO AL ARBOL----\n");
        printf(" %d |", tree->fact);
        inOrder(tree->left);
        inOrder(tree->right);
    }
}