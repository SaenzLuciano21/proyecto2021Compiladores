#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Structs.h"
#define SIZE 16

char *cases[] = {"METHOD", "STM", "LITERAL", "VAR", "SUMA", "MULT", "RESTA", "PROG", "BLOCK", "PARAMETERS", "IDENTIFICADOR", "EXPR", "NEGACION", "NEGATIVO", "PORC", "DIV", "IAND", "IOR", "IGUAL", "MENOR", "MAYOR"};

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

/*int checkNode(bNode *tree)
{
    if (strcmp(tree->infoN->label, "SUMA") == 0)
    {
        if (tree->left->infoN->type != "INTEGER" || tree->right->infoN->type != "INTEGER")
        {
            printf("Los operandos de la operacion suma no son de tipo entero, revisar linea %d \n", tree->infoN->line);
            return -1;
        }
        checkNode(tree->right);
        checkNode(tree->left);
    }
    if (strcmp(tree->infoN->label, "RESTA") == 0)
    {
        if (tree->left->infoN->type != "INTEGER" || tree->right->infoN->type != "INTEGER")
        {
            printf("Los operandos de la operacion resta no son de tipo entero, revisar linea %d \n", tree->infoN->line);
            return -1;
        }
        checkNode(tree->right);
        checkNode(tree->left);
    }
    if (strcmp(tree->infoN->label, "MULT") == 0)
    {
        if (tree->left->infoN->type != "INTEGER" || tree->right->infoN->type != "INTEGER")
        {
            printf("Los operandos de la operacion multiplicacion no son de tipo entero, revisar linea %d \n", tree->infoN->line);
            return -1;
        }
        checkNode(tree->right);
        checkNode(tree->left);
    }
    if (strcmp(tree->infoN->label, "DIV") == 0)
    {
        if (tree->left->infoN->type != "INTEGER" || tree->right->infoN->type != "INTEGER")
        {
            printf("Los operandos de la operacion division no son de tipo entero, revisar linea %d \n", tree->infoN->line);
            return -1;
        }
        checkNode(tree->right);
        checkNode(tree->left);
    }
    if (strcmp(tree->infoN->label, "PORC") == 0)
    {
        if (tree->left->infoN->type != "INTEGER" || tree->right->infoN->type != "INTEGER")
        {
            printf("Los operandos de la operacion modulo no son de tipo entero, revisar linea %d \n", tree->infoN->line);
            return -1;
        }
        checkNode(tree->right);
        checkNode(tree->left);
    }
    if (strcmp(tree->infoN->label, "MENOR") == 0)
    {
        if (tree->left->infoN->type != "INTEGER" || tree->right->infoN->type != "INTEGER")
        {
            printf("Los operandos de la operacion MENOR no son de tipo integer, revisar linea %d \n", tree->infoN->line);
            return -1;
        }
        checkNode(tree->right);
        checkNode(tree->left);
    }
    if (strcmp(tree->infoN->label, "MAYOR") == 0)
    {
        if (tree->left->infoN->type != "INTEGER" || tree->right->infoN->type != "INTEGER")
        {
            printf("Los operandos de la operacion MAYOR no son de tipo integer, revisar linea %d \n", tree->infoN->line);
            return -1;
        }
        checkNode(tree->right);
        checkNode(tree->left);
    }
    if (strcmp(tree->infoN->label, "IGUAL") == 0)
    {
        if (tree->left->infoN->type != "INTEGER" || tree->right->infoN->type != "INTEGER")
        {
            printf("Los operandos de la operacion IGUAL no son de tipo integer, revisar linea %d \n", tree->infoN->line);
            return -1;
        }
        checkNode(tree->right);
        checkNode(tree->left);
    }
    if (strcmp(tree->infoN->label, "IOR") == 0)
    {
        if (tree->left->infoN->type != "BOOLEAN" || tree->right->infoN->type != "BOOLEAN")
        {
            printf("Los operandos de la operacion OR no son de tipo booleanos, revisar linea %d \n", tree->infoN->line);
            return -1;
        }
        checkNode(tree->right);
        checkNode(tree->left);
    }
    if (strcmp(tree->infoN->label, "IAND") == 0)
    {
        if (tree->left->infoN->type != "BOOLEAN" || tree->right->infoN->type != "BOOLEAN")
        {
            printf("Los operandos de la operacion AND no son de tipo booleanos, revisar linea %d \n", tree->infoN->line);
            return -1;
        }
        checkNode(tree->right);
        checkNode(tree->left);
    }
    if (strcmp(tree->infoN->label, "NEGACION") == 0)
    {
        if (tree->left->infoN->type != "BOOLEAN")
        {
            printf("El operandor de la operacion NEGACION (!) no es de tipo booleano, revisar linea %d \n", tree->infoN->line);
            return -1;
        }
        checkNode(tree->right);
        checkNode(tree->left);
    }
    return 0;
}*/