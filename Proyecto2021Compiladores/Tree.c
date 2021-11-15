#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Structs.c"
#define SIZE 16

char *cases[] = {"PMETHOD", "PMETHODE", "METHOD", "METHOD2", "CMETHOD", "CPMETHOD", "LISTMETHOD", "STM1", "STM2",
                "STM3", "LISTSTM", "LITERAL", "LITERAL2", "LITERAL3", "VAR", "LISTVAR", "SUMA", "MULT", "RESTA", 
                "PROG", "BLOCK1", "BLOCK2", "BLOCK3", "PARAMETERS", "LISTPARAM", "IDENTIFICADOR", "IDENTIFICADOR2", 
                "LISTEXPR", "NEGACION", "NEGATIVO", "PORC", "DIV", "CONJUNCION", "DISYUNCION", "IGUAL", "MENOR", 
                "MAYOR", "WHILELOOP", "IFTHENELSE", "RETURN1", "RETURN2"};

char *types[] = {"BOOLEAN", "INTEGER"};

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
    if (tree == NULL)
        printf("NULL | \n");
    else
    {
        printf("%s | \n", cases[tree->fact]);
        inOrder(tree->left);
        inOrder(tree->right);
    }
}

