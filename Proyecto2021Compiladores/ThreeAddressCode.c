#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "SymbolTable.c"

tStack *createInstrucciones(enum tLabel type, bNode* tree, tStack* list){
	tStack* auxLeft = NULL;
	tStack* auxRight = NULL;
	tStack* nodeLeftAux = malloc(sizeof(tStack));(en listInstruct deberiamos traer los sub arboles en este aux)
	tStack* nodeRightAux = malloc(sizeof(tStack));
	infoN* infoNLeft;
	infoN* infoNRight;

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

tStack* loadInstrucciones(tStack* tree, enum tLabel type, info* arg1, info* arg2, info* result){
	tStack* new = malloc(sizeof(tStack));
	anterior->sig = new;
	new->op = type;
	new->arg1 = arg1;
	new->arg2 = arg2;
	new->result = result;
	return new;
}

tStack *listInstrcciones(bNode *tree, tStack *list){
    tStack *aux = malloc(sizeof(tStack));

    if (strcmp(tree->fact,"SUMA") == 0){
		return createInstructLabel(SUMA, tree, list);
	}

	if (strcmp(tree->fact,"RESTA") == 0){
		return createInstructLabel(RESTA, tree,list);
	}

	if (strcmp(tree->fact,"MULT") == 0){
		return createInstructLabel(MULT, tree, list);
	}

	if (strcmp(tree->fact,"DIV") == 0){
		return createInstructLabel(DIV, tree, list);
	}

	if (strcmp(tree->fact,"PORC") == 0){
		return createInstructLabel(PORC, tree, list);
	}

	if (strcmp(tree->fact,"MAYOR") == 0){
		return createInstructLabel(MAYOR, tree, list);
	}

	if (strcmp(tree->fact,"MENOR") == 0){
		return createInstructLabel(MENOR, tree, list);
	}

	if (strcmp(tree->fact,"OR") == 0){
		return createInstructLabel(IOR, tree, list);
	}

	if (strcmp(tree->fact,"AND") == 0){
		return createInstructLabel(IAND, tree, list);
	}

	if (strcmp(tree->fact,"IGUAL") == 0){
		return createInstructLabel(IGUAL, tree, list);
	}

	if (strcmp(tree->fact,"NEG") == 0){
		if ((strcmp(tree->hd->fact, "BOOL") == 0) || (strcmp(tree->hd->fact,"VAR") == 0)){
			return load_inst(list, NEG, tree->hd->data , NULL, createTempVar());
		}
		tStack* a = gen_list_instruc(tree->hd, list);
		return load_inst(a, NEG, a->op3, NULL, createTempVar());
	}
}