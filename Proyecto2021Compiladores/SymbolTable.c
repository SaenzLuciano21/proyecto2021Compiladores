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

int boolCheck(bNode *expr) {
    if (expr->fact == CONJUNCION || expr->fact == DISYUNCION) {
        return (boolCheck(expr->right) == boolCheck(expr->left));      
    } else {
        if (expr->fact == NEGACION) {
            return boolCheck(expr->left);
        } else {
            return (expr->fact == LITERAL2 || expr->fact == LITERAL3);
        }
    }
}

int equalsCheck(bNode *expr) {
    return ((intCheck(expr->left) && intCheck(expr->right)) || 
    (boolCheck(expr->left) && boolCheck(expr->right)));
}

// todo void, pues si falla algo sale con exit(0)

void methodCheck(bNode *mthExpr, sNode **symbolTable) {
    // chequeamos si tiene parametros
    if (mthExpr->fact == CPMETHOD) {
        // invocamos paramCounter con el hijo izq, que es donde tenemos la list_expr
        list *formalParam = containsStack(symbolTable, mthExpr->infoN);
        paramCheck(mthExpr->left, formalParam);
    }
}

void paramCheck(bNode *currentp, list *formalp) {
    bNode *auxcp = currentp;
    list *auxf = formalp;
    int currentCount = 0;
    int formalCount = 0;
    enum tType ctype;
    enum tType ftype;

    // recorremos ambas estructuras y vamos chequeando los tipos y luego consistencia en cantidad
    while (auxcp != NULL || auxf != NULL) {
        if (auxcp != NULL) {
            currentCount++;
            ctype = auxcp->infoN->type; 
            auxcp = auxcp->right;
        }
        if (auxf != NULL) {
            formalCount++;
            ftype = auxf->infoN->type;
            auxf = auxf->next;
        }
        if (ctype != ftype) {
            printf("Inconsistencia de tipos entre parametros formales y parametros actuales \n");
            getchar();
            exit(0);
        }
    }
    if (currentCount != formalCount) {
        printf("El numero de parametros actuales difiere del numero de parametros formales \n");
        getchar();
        exit(0);
    }
}