#ifndef Structs_program_h
#define Structs_program_h

/*Definicion de tipos enumerados*/
typedef enum tLabel
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
} t;

typedef enum tType
{
    BOOLEAN,
    INTEGER
} tV;

/*Estructura del nodo*/
struct infoToken
{
    char *name;
    int value;
    int line;
    enum tType type;
    enum tLabel label;
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

#endif