#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "List.c"

/*
                        // gen method level y cargarlo antes de push
                        list *newlevel = NULL; 
                        currentTableLevel = newlevel;
                        push(currentTableLevel, symbolTable);*/

// revisar si no hace falta mantener el nivel actual de la lista donde se està insertando todo
//void check(bNode *root, sNode **symbolTable) {
//    
//    /* seteamos current con la raiz del arbol */
//    bNode *current = root;
//    // nivel 0
//    list *newLevel;
//    if (current != NULL) {
//        // caso program
//        if (current->fact == PROG) {  
//            // caso "base" donde generamos el primer nivel de la tabla y lo apilamos en la stack
//            newLevel = (list *) malloc(sizeof(list)); // inicializamos la lista que representa el nivel 0
//            push(newLevel, symbolTable);
//
//            // visitamos subarbol izq y luego el subarbol derecho
//            check(current->left, symbolTable);
//            check(current->right, symbolTable);
//            // no cerrar nivel, pues el nivel 0 es el que se mantiene
//        } else {
//            if (current->fact == LISTVAR) { // si tenemos una lista de variables declaradas, vamos recorriendo e insertando
//                insert(current->left->infoN, &newLevel); // cada var se aloja en los hijos izq
//                check(current->right, symbolTable); // la lista de declaraciones se extiende por la derecha
//            } else {
//                if (current->fact == LISTMETHOD) { //recorro lista de metodos
//                    check(current->right, symbolTable);
//                } else {
//                    if (current->fact == PARAMETERS) {
//                        list *paramLevel = (list *) malloc(sizeof(list));
//                        push(paramLevel, symbolTable); // se pushea nivel nuevo
//                        insert(current->infoN, &paramLevel); // aca estan los parametros
//                        check(current->right, symbolTable); // reviso si hay mas parametros
//                    }
//                }
//            }
//        }
//
//
//    } // caso current == NULL
//    else {
//
//    }
//}



sNode * check(bNode *root, sNode **symbolTable) {
    
    /* seteamos current con la raiz del arbol */
    bNode *current = root;
    // nivel 0
    list *newLevel;
    if (current != NULL) {
        // caso program
        if (current->fact == PROG) {  
            // caso "base" donde generamos el primer nivel de la tabla y lo apilamos en la stack
            newLevel = (list *) malloc(sizeof(list)); // inicializamos la lista que representa el nivel 0
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
            insert(current->infoN, &newLevel);
            check(current->left, symbolTable);
        }
        if (current->fact == IDENTIFICADOR) {
            insert(current->infoN, &newLevel);
            check(current->left, symbolTable);
        }
        if (current->fact == LISTMETHOD) { //recorro lista de metodos
            check(current->left, symbolTable);
            check(current->right, symbolTable);
        }
        if (current->fact == PMETHOD) {
            insert(current->infoN, &newLevel);
            check(current->left, symbolTable);
            check(current->right, symbolTable);
        }
        if (current->fact == PARAMETERS) {
            list *paramLevel = (list *) malloc(sizeof(list));
            push(symbolTable); // se pushea nivel nuevo
            insert(current->infoN, &paramLevel); // aca estan los parametros
            check(current->right, symbolTable); // reviso si hay mas parametros
        }
        if (current->fact == BLOCK3) {
            list *blockLevel = (list *) malloc(sizeof(list));
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