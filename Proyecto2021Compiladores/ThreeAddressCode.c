#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "SymbolTable.c"

tStack * loadInstruction(tStack **tac, enum tTAC type, info *arg1, info *arg2, info *result){
	tStack **new = (tStack *)malloc(sizeof(tStack));
	(*new)->op = type;
	(*new)->arg1 = arg1;
	(*new)->arg2 = arg2;
	(*new)->result = result;
	(*tac)->next = new; 
	return &new;
}

tStack * instructionList(bNode *tree, tStack **tac){
    bNode *current = tree;
	if (current->fact != NULL) {
		if (current->fact == PROG) {
			loadInstruction(tac, BEGIN_PROG, NULL, NULL, NULL);
			instructionList(current->right, tac);
			loadInstruction(tac, END_PROG, NULL, NULL, NULL);
		}
		if (current->fact == PMETHOD || current->fact == METHOD) {
			loadInstruction(tac, BEGIN_FUNCTION, NULL, NULL, current->infoN);
			instructionList(current->right, tac);
			loadInstruction(tac, END_FUNCTION, NULL, NULL, current->infoN);
		}
		if (current->fact == BLOCK3 || current->fact == BLOCK2) {
			instructionList(current->right, tac);
		}
		if (current->fact == LISTSTM) {
			instructionList(current->left, tac);
			instructionList(current->right, tac);
		}
		if (current->fact == STM1) {
			instructionList(current->right, tac);
			info * x = current->right->infoN; 
			loadInstruction(tac, ASSIG, x, NULL, current->infoN);
		}
		if (current->fact == STM2) {
			instructionList(current->left, tac);

		}
		//Tengo que preguntar como se hace
		if (current->fact == IFTHENELSE) {
			instructionList(current->left, tac);
			info * x1 = current->left->infoN;
			instructionList(current->middle, tac);
			instructionList(current->right, tac);

		}
		//Tengo que preguntar como se hace
		if (current->fact == WHILELOOP) {
			
		}
		//Donde guardo el resultado, en x2 o current->infoN
		if (current->fact == RETURN1) {
			instructionList(current->left, tac);
			info * x = current->left->infoN;
			loadInstruction(tac, RET, x, NULL, current->infoN);
		}
		if(current->fact == CMETHOD) {
			loadInstruction(tac, CALL_F, NULL, NULL, current->infoN);
		}
		if(current->fact == CPMETHOD) {
			instructionList(current->left, tac);
			if(current->left->fact != LISTEXPR) {
				info * x = current->left->infoN;
				info * p;		// Aca voy a llevar el orden de los parametros
				p->value = 1; 	// En este caso como es un solo parametro pongo 1
				loadInstruction(tac, LOAD_PARAM, p, NULL, x);
			}
			loadInstruction(tac, CALL_F, NULL, NULL, current->infoN);
		}
		if (current->fact == LISTEXPR) {
			int cant = 1;
			instructionList(current->left, tac);
			info * x1 = current->left->infoN;
			info * p1;
			p1->value = cant;
			loadInstruction(tac, LOAD_PARAM, p1, NULL, x1);
			int b = 1;
			current = current->right;
			while (b != 0) {
				if (current->fact == LISTEXPR) {
					instructionList(current->left, tac);
					info * x2 = current->left->infoN;
					info * p2;
					cant++;
					p2->value = cant;
					loadInstruction(tac, LOAD_PARAM, p2, NULL, x2);
					current = current->right;
				}
				else {
					instructionList(current->right, tac);	
					info * x2 = current->right->infoN;
					info * p2;
					cant++;
					p2->value = cant;
					loadInstruction(tac, LOAD_PARAM, p2, NULL, x2);
					b = 0;
				}
			}
		}
		// Estos casos son todos iguales, solo que la etiqueta va a ser diferente. 
		//Tengo que preguntar si puedo usar la misma estructura de los label
		if (current->fact == SUMA || current->fact == RESTA || current->fact == MULT || current->fact == DIV || current->fact == PORC) {
			instructionList(current->left, tac);
			instructionList(current->right, tac);
			info * x1 = current->left->infoN;
			info * x2 = current->right->infoN;
			loadInstruction(tac, SUM, x1, x2, current->infoN);
		}
		//Preguntar tambien, donde guardo el resultado y tambien irian otros casos
		if (current->fact == PARENTESIS) {
			instructionList(current->left, tac);
			info * x = current->left->infoN;
			loadInstruction(tac, PARENT, x, NULL, current->infoN);
		}
	}

	
}













/*tStack *loadInstrucciones(tStack *tree, enum tLabel type, info *arg1, info *arg2, info *result)
{
	tStack *new = malloc(sizeof(tStack));
	new->next = tree;
	new->op = type;
	new->arg1 = arg1;
	new->arg2 = arg2;
	new->result = result;
	return new;
}*/

/*
tStack *createInstruction(tStack **tac, enum tLabel type, bNode *tree) {
	tStack * auxLeft = NULL;
	tStack * auxRight = NULL;
	tStack * nodeLeftAux = (tStack *) malloc(sizeof(tStack)); //en listInstruct deberiamos traer los sub arboles en este aux)
	tStack * nodeRightAux = (tStack *) malloc(sizeof(tStack));
	info * infoNLeft;
	info * infoNRight;

	if ((strcmp(tree->left->infoN->type, "boolean") == 0) || (strcmp(tree->left->infoN->type, "integer") == 0))
		infoNLeft = tree->left->(NECESITAMOS LA INFO DE LA TABLA);
	else{
		auxLeft = listInstrcciones(tree->left, nodeLeft);
		infoNLeft = auxLeft->result;(va a traer el resultado final)
	}

	if ((strcmp(tree->right->infoN->type, "boolean") == 0) || (strcmp(tree->right->infoN->type, "integer") == 0))
		infoNRight = tree->right-> (NECESITAMOS LA INFO DE LA TABLA); 
	else{
		auxRight = listInstrcciones(tree->right, nodeRight);
		infoNRight = auxRight->result;
	}
	
	if (auxLeft != NULL){
		list->sig = nodeLeftAux->sig;
		list = auxLeft; 
	}
	if (auxRight != NULL){
		list->sig = nodeRightAux->sig;
		list = auxRight;
	}
	return loadInstrucciones(list, type, infoNLeft, infoNRight, VARIABLE AUX);
}
*/