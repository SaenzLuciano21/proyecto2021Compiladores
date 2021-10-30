#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Stack.c"

void tableGen(bNode *root, sNode **symbolTable) {
    
    /* seteamos current con la raiz del arbol */
    bNode *current = root;
    if (current != NULL) {
        // caso program
        if (current->fact == PROG) {  
            // caso "base" donde generamos el primer nivel de la tabla y lo apilamos en la stack
            push(symbolTable);

            // visitamos subarbol izq y luego el subarbol derecho
            tableGen(current->left, symbolTable);
            tableGen(current->right, symbolTable);
            // no cerrar nivel, pues el nivel 0 es el que se mantiene
        }
        if (current->fact == LISTVAR)  { // si tenemos una lista de variables declaradas, vamos recorriendo e insertando
            tableGen(current->left, symbolTable); // recorro los var que se encuentran en los hijos izq
            tableGen(current->right, symbolTable); // la lista de declaraciones se extiende por la derecha
        }
        if (current->fact == VAR) {
            insertStack(symbolTable, current->infoN);
            tableGen(current->left, symbolTable);   //aca estan los ID
        }
        if (current->fact == IDENTIFICADOR) {
            if (containsList((*symbolTable)->head, current->infoN) == 0) {
                insertStack(symbolTable, current->infoN);
                tableGen(current->left, symbolTable);   //verifico si hay mas ID
            } else {
                printf("Variable ya declarada en el ambiente actual \n");
                exit(0);
            }
        }
        if (current->fact == IDENTIFICADOR2) {
            if (containsStack(symbolTable, current->infoN) == 0) {
                insertStack(symbolTable, current->infoN);
                tableGen(current->left, symbolTable);   //verifico si hay mas ID
            } else {
                printf("Variable no declarada \n");
                exit(0);            
            }
        }
        if (current->fact == LISTMETHOD) { //recorro lista de metodos
            tableGen(current->left, symbolTable);
            tableGen(current->right, symbolTable);
        }
        if (current->fact == PMETHOD) {
            if (containsStack(symbolTable, current->infoN) == 0) {
                insertStack(symbolTable, current->infoN);
                tableGen(current->left, symbolTable);
                tableGen(current->right, symbolTable);
            } else {
                printf("Metodo ya declarado \n");
                exit(0); 
            }
        }
        if (current->fact == PMETHODE) {
            if (containsStack(symbolTable, current->infoN) == 0) {
                insertStack(symbolTable, current->infoN);
                tableGen(current->left, symbolTable);
            } else {
                printf("Metodo ya declarado \n");
                exit(0); 
            }
        }
        if (current->fact == METHOD) {
            if (containsStack(symbolTable, current->infoN) == 0) {
                insertStack(symbolTable, current->infoN);
                tableGen(current->right, symbolTable);
            } else {
                printf("Metodo ya declarado \n");
                exit(0); 
            }
        }
        // Revisar el caso de declaracion de variables con un ID igual que los parametros
        if (current->fact == PARAMETERS) { 
            push(symbolTable); // se pushea nivel nuevo
            insertStack(symbolTable, current->infoN); // aca estan los parametros
            tableGen(current->right, symbolTable); // reviso si hay mas parametros
            pop(symbolTable);
        }
        if (current->fact == BLOCK3) {  //el unico caso que me interesa meter en la ST
            push(symbolTable);
            tableGen(current->left, symbolTable);
            tableGen(current->right, symbolTable);
            pop(symbolTable);
        }
        if (current->fact == LISTSTM) {
            tableGen(current->left, symbolTable);
            tableGen(current->right, symbolTable);
        }
        if (current->fact == STM1) {
            if (containsStack(symbolTable, current->infoN) == 0) {
                insertStack(symbolTable, current->infoN);
                tableGen(current->right, symbolTable);
            } else {
                printf("Variable no declarada \n");
                exit(0); 
            }
        }
        if (current->fact == STM2) {
            tableGen(current->left, symbolTable);
        }
        if (current->fact == IFTHENELSE) {
            tableGen(current->left, symbolTable);
            tableGen(current->middle, symbolTable);
            tableGen(current->right, symbolTable);
        }
        if (current->fact == WHILELOOP) {
            tableGen(current->left, symbolTable);
            tableGen(current->right, symbolTable);
        }
        if (current->fact == RETURN1) {
             tableGen(current->left, symbolTable);
        }
        if (current->fact == CMETHOD) {
            if (containsStack(symbolTable, current->infoN) == 0) {
                insertStack(symbolTable, current->infoN);
            } else {
                printf("Metodo no declarado \n");
                exit(0);
            }
        }
        if (current->fact == CPMETHOD) {
            if (containsStack(symbolTable, current->infoN) == 0) {
                insertStack(symbolTable, current->infoN);
                tableGen(current->left, symbolTable);   //verifico si hay mas ID
            } else {
                printf("Metodo no declarado \n");
                exit(0);
            }
        }
        if (current->fact == LISTEXPR) {
            tableGen(current->left, symbolTable);
            tableGen(current->right, symbolTable);
        }
        // semantica enteros
        if (current->fact == SUMA || current->fact == RESTA || current->fact == MULT || current->fact == DIV
            || current->fact == MENOR || current->fact == MAYOR || current->fact == NEGATIVO) {
            if (!intCheck(current)) { // Chequeo de tipos para enteros
                printf("Error de tipos, se espera: INT 'op' INT \n");
                getchar();
                exit(0);
            }
            // ???? hace falta seguir recursionando? creo que estamos en las hojas ya
            /*tableGen(current->left, symbolTable);
            tableGen(current->right, symbolTable);*/
        }
        // semantica booleanos
        if (current->fact == CONJUNCION || current->fact == DISYUNCION || current->fact == NEGACION) {
            if (!boolCheck(current)) { // Chequeo de tipos para booleanos
                printf("Error de tipos, se espera: BOOL 'op' BOOL \n");
                getchar();
                exit(0);
            }
            // ???? hace falta seguir recursionando? creo que estamos en las hojas ya
            /*tableGen(current->left, symbolTable);
            tableGen(current->right, symbolTable);*/
        }
        // asi deberian ser los dos casos anteriores, sin las recursiones
        if (current->fact == IGUAL && !equalsCheck(current)) {
                printf("Error de tipos, se espera: BOOL 'op' BOOL o INT 'op' INT \n");
                getchar();
                exit(0);
        }
    } else {  // caso current == NULL
        
    }
}

// Chequeo de tipos para enteros
int intCheck(bNode *exp) {
    if (exp->fact == SUMA || exp->fact == RESTA || exp->fact == MULT || exp->fact == PORC ||
    exp->fact == DIV || exp->fact == MENOR || exp->fact == MAYOR) {
        return (intCheck(exp->right) == intCheck(exp->left));      
    }else {
        if (exp->fact == NEGATIVO) {
            return intCheck(exp->left);
        } else {
            return (exp->fact == LITERAL);
        }
    }
}

int boolCheck(bNode *exp) {
    if (exp->fact == CONJUNCION || exp->fact == DISYUNCION) {
        return (boolCheck(exp->right) == boolCheck(exp->left));      
    } else {
        if (exp->fact == NEGACION) {
            return boolCheck(exp->left);
        } else {
            return (exp->fact == LITERAL2 || exp->fact == LITERAL3);
        }
    }
}

int equalsCheck(bNode *expr) {
    return ((intCheck(expr->left) && intCheck(expr->right)) || 
    (boolCheck(expr->left) && boolCheck(expr->right)));
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