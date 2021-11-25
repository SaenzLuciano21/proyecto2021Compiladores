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
		if (current->fact == IFTHENELSE) {
			instructionList(current->left, tac);
			info * x1 = current->left->infoN;
			info * x2, *x3;
			x2->name = "BLOCK_T";
			x3->name = "BLOCK_F";
			loadInstruction(tac, IIF, x2, x3, x1);
			loadInstruction(tac, BLOCK_T, NULL, NULL, NULL);
			instructionList(current->middle, tac);
			loadInstruction(tac, END_BLOCK_T, NULL, NULL, NULL);
			loadInstruction(tac, BLOCK_F, NULL, NULL, NULL);
			instructionList(current->right, tac);
			loadInstruction(tac, END_BLOCK_F, NULL, NULL, NULL);
		}
		if (current->fact == WHILELOOP) {
			instructionList(current->left, tac);
			info * x1 = current->left->infoN;
			info * x2, * x3;
			x2->name = "BLOCK_W";
			x3->name = "END_BLOCK_W";
			loadInstruction(tac, WWHILE, x2, NULL, x1);
			loadInstruction(tac, BLOCK_W, NULL, NULL, NULL);
			instructionList(current->right, tac);
			loadInstruction(tac, END_BLOCK_W, NULL, NULL, NULL);
		}
		if (current->fact == RETURN1) {
			instructionList(current->left, tac);
			info * x = current->left->infoN;
			loadInstruction(tac, RET, x, NULL, current->infoN);
		}
		if(current->fact == CMETHOD) {
			loadInstruction(tac, CALL_F, NULL, NULL, current->infoN);
		}
		if(current->fact == CPMETHOD) {
			if(current->left->fact != LISTEXPR) {
				instructionList(current->left, tac);
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
		if (current->fact == SUMA) {
			instructionList(current->left, tac);
			instructionList(current->right, tac);
			info * x1 = current->left->infoN;
			info * x2 = current->right->infoN;
			loadInstruction(tac, SUM, x1, x2, current->infoN);
		}
		if (current->fact == RESTA) {
			instructionList(current->left, tac);
			instructionList(current->right, tac);
			info * x1 = current->left->infoN;
			info * x2 = current->right->infoN;
			loadInstruction(tac, RES, x1, x2, current->infoN);
		}
		if (current->fact == MULT) {
			instructionList(current->left, tac);
			instructionList(current->right, tac);
			info * x1 = current->left->infoN;
			info * x2 = current->right->infoN;
			loadInstruction(tac, MUL, x1, x2, current->infoN);
		}
		if (current->fact == DIV) {
			instructionList(current->left, tac);
			instructionList(current->right, tac);
			info * x1 = current->left->infoN;
			info * x2 = current->right->infoN;
			loadInstruction(tac, SPLIT, x1, x2, current->infoN);
		}
		if (current->fact == PORC) {
			instructionList(current->left, tac);
			instructionList(current->right, tac);
			info * x1 = current->left->infoN;
			info * x2 = current->right->infoN;
			loadInstruction(tac, PERCENTAGE, x1, x2, current->infoN);
		}
		if (current->fact == CONJUNCION) {
			instructionList(current->left, tac);
			instructionList(current->right, tac);
			info * x1 = current->left->infoN;
			info * x2 = current->right->infoN;
			loadInstruction(tac, AAND, x1, x2, current->infoN);
		}
		if (current->fact == DISYUNCION) {
			instructionList(current->left, tac);
			instructionList(current->right, tac);
			info * x1 = current->left->infoN;
			info * x2 = current->right->infoN;
			loadInstruction(tac, OOR, x1, x2, current->infoN);
		}
		if (current->fact == IGUAL) {
			instructionList(current->left, tac);
			instructionList(current->right, tac);
			info * x1 = current->left->infoN;
			info * x2 = current->right->infoN;
			loadInstruction(tac, EEQUAL, x1, x2, current->infoN);
		}
		if (current->fact == MENOR) {
			instructionList(current->left, tac);
			instructionList(current->right, tac);
			info * x1 = current->left->infoN;
			info * x2 = current->right->infoN;
			loadInstruction(tac, LESS, x1, x2, current->infoN);
		}
		if (current->fact == MAYOR) {
			instructionList(current->left, tac);
			instructionList(current->right, tac);
			info * x1 = current->left->infoN;
			info * x2 = current->right->infoN;
			loadInstruction(tac, HIGHER, x1, x2, current->infoN);
		}
		if (current->fact == NEGATIVO) {
			instructionList(current->left, tac);
			info * x1 = current->left->infoN;
			loadInstruction(tac, NEGATIVE, x1, NULL, current->infoN);
		}
		if (current->fact == NEGACION) {
			instructionList(current->left, tac);
			info * x1 = current->left->infoN;
			loadInstruction(tac, DENIAL, x1, NULL, current->infoN);
		}
		//Preguntar tambien, donde guardo el resultado y tambien irian otros casos
		if (current->fact == PARENTESIS) {
			instructionList(current->left, tac);
			info * x = current->left->infoN;
			loadInstruction(tac, PARENT, x, NULL, current->infoN);
		}
	}
}