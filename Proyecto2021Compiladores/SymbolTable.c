#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Stack.c"

// Chequeo de tipos para expresiones
enum tType checkExprSemantic(bNode *expr, sNode **symbolTable) {
    // caso operador unario -
    if (expr->fact == NEGATIVO) {
        if (checkExprSemantic(expr->left, symbolTable) == boolean) {
            printf("Inconsistencia de tipos en la expresion \n");
            getchar();
            exit(0);
        } else return integer; 
    } else {
        // caso literales, retornamos tipo guardado en info
        if (expr->fact == LITERAL || expr->fact == LITERAL2 || expr->fact == LITERAL3) {
            return expr->infoN->type;
        } else {
            // ID lo buscamos en la ST y retornamos tipo
            if (expr->fact == IDENTIFICADOR2) {
                list * idNode = containsStack(expr->infoN, symbolTable);
                return idNode->infoN->type;
            } else {
                //method_call buscamos su ID en la ST, verificamos tipo de retorno
                if (expr->fact == CMETHOD || expr->fact == CPMETHOD) {
                    methodCheck(expr, symbolTable);
                } else {
                    if (expr->fact == NEGACION) {
                        if (checkExprSemantic(expr->left, symbolTable) == integer) {
                            printf("Inconsistencia de tipos en la expresion \n");
                            getchar();
                            exit(0);
                        } else return boolean; 
                    } else { // casos de operaciones binarias
                        enum tType op1 = checkExprSemantic(expr->left, symbolTable);
                        enum tType op2 = checkExprSemantic(expr->right, symbolTable);

                        if (op1 == op2) {
                            return op1;
                        } else {
                            printf("Inconsistencia de tipos en la expresion \n");
                            getchar();
                            exit(0);
                        }
                    }
                }
            }
        }
    }
}

// chequea expresiones de tipo method_call
void methodCheck(bNode *mthExpr, sNode **symbolTable) {
    // chequeamos si tiene parametros
    if (mthExpr->fact == CPMETHOD) {
        list *formalParam = containsStack(symbolTable, mthExpr->infoN);
        /* invocamos paramCheck con el hijo izq, que es donde tenemos la list_expr (parámetros actuales)
        y los parametros formales obtenidos de la invocación anterior */
        paramCheck(mthExpr->left, formalParam, symbolTable);
    } else {
        //HACER CASO DE METHOD CALL SIN PARAMETROS, SOLO HAY QUE CHEQUEAR EL TIPO DE RETORNO
        // BUSCAR SU CORRESPONDIENTE ID Y RETORNAR EL ENUM TTYPE
    }
}

// chequea que sea consistente la cantidad de parametros actuales y formales, y que correspondan sus tipos
void paramCheck(bNode *currentp, list *formalp, sNode **symbolTable) {
    // list_expr de parametros actuales en forma de bNode
    bNode *auxcp = currentp;
    // lista de parametros en la declaracion del metodo, en forma de lista
    list *auxf = formalp;
    int currentCount = 0;
    int formalCount = 0;
    enum tType ctype;
    enum tType ftype;
    enum tType ctype2;
    enum tType ftype2;
    // recorremos ambas estructuras y vamos chequeando los tipos y luego consistencia en cantidad
    while (auxf != NULL) {
        formalCount++;
        ftype = auxf->infoN->type;

        if (auxcp->right->fact == LISTEXPR) {
            currentCount++;
            ctype = checkExprSemantic(auxcp->left, symbolTable);
            auxcp = auxcp->right;
            if (ctype != ftype) {
                printf("Inconsistencia de tipos entre parametros formales y parametros actuales \n");
                getchar();
                exit(0);
            }
        } else { // caso base de la lista de expresiones, hijos izq y der con expr
            // como son expresiones hay que recursionar con checkExprSemantic y capturar el tipo de esas expr
            ctype = checkExprSemantic(auxcp->right, symbolTable); 
            ctype2 = checkExprSemantic(auxcp->left, symbolTable);
            currentCount++;
            currentCount++;
            // obtenemos el siguiente parametro formal de la lista
            if (auxf->next != NULL) {
                ftype2 = auxf->next->infoN->type;
                formalCount++;
            } else {    
                printf("El numero de parametros actuales difiere del numero de parametros formales \n");
                getchar();
                exit(0);
            }
            if (ctype != ftype || ctype2 != ftype2) {
                printf("Inconsistencia de tipos entre parametros formales y parametros actuales \n");
                getchar();
                exit(0);
            }
        }
        auxf = auxf->next;
    }
    if (currentCount != formalCount) {
        printf("El numero de parametros actuales difiere del numero de parametros formales \n");
        getchar();
        exit(0);
    }
}
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
        if (current->fact == LISTPARAM) {
            push(symbolTable);
            tableGen(current->left, symbolTable);
            pop(symbolTable);
        }
        if (current->fact == PARAMETERS) { 
            insertStack(symbolTable, current->infoN); // aca estan los parametros
            tableGen(current->right, symbolTable); // reviso si hay mas parametros
            
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
        // if (current->fact == LISTEXPR) {  caso considerado en checkexprsemantic
        //     tableGen(current->left, symbolTable);
        //     tableGen(current->right, symbolTable);
        // }
        enum tType capt;
        capt = checkExprSemantic(current, symbolTable);
}

/* old semantic check methods
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
}*/


