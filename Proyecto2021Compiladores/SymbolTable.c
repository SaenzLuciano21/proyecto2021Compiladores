#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "List.c"

void check(bNode *root, sNode **symbolTable) {
    
    /* seteamos current con la raiz del arbol */
    bNode *current = root;
    if (current != NULL) {
        // caso program
        if (current->fact == PROG) {  
            // caso "base" donde generamos el primer nivel de la tabla y lo apilamos en la stack
            push(symbolTable);

            // visitamos subarbol izq y luego el subarbol derecho
            check(current->left, symbolTable);
            check(current->right, symbolTable);
            // no cerrar nivel, pues el nivel 0 es el que se mantiene
        }
        if (current->fact == LISTVAR)  { // si tenemos una lista de variables declaradas, vamos recorriendo e insertando
            check(current->left, symbolTable); // recorro los var que se encuentran en los hijos izq
            check(current->right, symbolTable); // la lista de declaraciones se extiende por la derecha
        }
        if (current->fact == VAR) { // OJO!!! que pasa con newLevel si hay un var en block???
            insertStack(symbolTable, current->infoN);
            check(current->left, symbolTable);
        }
        if (current->fact == IDENTIFICADOR) {
            insertStack(symbolTable, current->infoN);
            check(current->left, symbolTable);
        }
        if (current->fact == LISTMETHOD) { //recorro lista de metodos
            check(current->left, symbolTable);
            check(current->right, symbolTable);
        }
        if (current->fact == PMETHOD) {
            insertStack(symbolTable, current->infoN);
            check(current->left, symbolTable);
            check(current->right, symbolTable);
        }
        if (current->fact == PARAMETERS) {
            push(symbolTable); // se pushea nivel nuevo
            insertStack(symbolTable, current->infoN); // aca estan los parametros
            check(current->right, symbolTable); // reviso si hay mas parametros
        }
        if (current->fact == BLOCK3) {
            push(symbolTable);
            check(current->left, symbolTable);
            check(current->right, symbolTable);
        }
        if (current->fact == LISTSTM) {
            check(current->left, symbolTable);
            check(current->right, symbolTable);
        }
    } else {  // caso current == NULL
        
    }
}