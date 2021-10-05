#ifndef Structs_program_h
#define Structs_program_h

/*char *cases[] = {"METHOD", "STM", "LITERAL", "VAR", "SUMA", "MULT", "RESTA", "PROG", "BLOCK", "PARAMETERS", "IDENTIFICADOR", "EXPR", "NEGACION", "NEGATIVO", "PORC", "DIV", "IAND", "IOR", "IGUAL", "MENOR", "MAYOR"};*/

/*Definicion de tipos enumerados*/
enum tLabel
{
    METHOD,
    STM,
    LITERAL,
    VAR,
    SUMA,
    MULT,
    RESTA,
    PROG,
    BLOCK,
    PARAMETERS,
    IDENTIFICADOR,
    EXPR,
    NEGACION,
    NEGATIVO,
    PORC,
    DIV,
    IAND,
    IOR,
    IGUAL,
    MENOR,
    MAYOR
};

enum tType
{
    boolean,
    integer
};

/*Estructura del nodo*/
struct infoToken
{
    int value;
    int line;
    enum tType type;
    char *name;
};

/*Definicion del nombre del nodo*/
typedef struct infoToken info;

/*Estructura del arbol*/
struct bTree
{
    enum tLabel fact;
    struct bTree *right, *middle, *left;
    info *infoN;
};

/*Definicion del nombre del arbol*/
typedef struct bTree bNode;

/*perfil de las funciones*/
bNode *create_node(enum tLabel label, info *infN, bNode *left, bNode *middle, bNode *right);
void freeMemory(bNode *tree);
void inOrder(bNode *tree);

#endif