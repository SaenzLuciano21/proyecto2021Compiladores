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
    boolean,
    integer
} tV;

/*Estructura del nodo*/
struct infoToken
{
    char *name;
    int value;
    int line;
    enum tType type;
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

struct treeStack
{
    struct bTree *treeNode;
    struct treeStack *next;
};

typedef struct treeStack tStack;

#endif

/* ------------------ List ------------------- */

/* Estructura de la tabla de simbolos */
struct listNode
{
    info *infoN;
    struct listNode *next;
};

/* Nodo para lista */
typedef struct listNode list;


/* ------------------ Stack para la tabla de simbolos ------------------- */

/* Estructura de la pila */
struct stackNode
{
    list *head;
    struct stackNode *next;
};

typedef struct stackNode sNode;



 
