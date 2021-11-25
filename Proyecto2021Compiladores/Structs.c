#ifndef Structs_program_h
#define Structs_program_h

/*Definicion de tipos enumerados*/
typedef enum tLabel
{
    PMETHOD,
    PMETHODE,
    METHOD,
    METHOD2,
    CMETHOD,
    CPMETHOD,
    LISTMETHOD,
    STM1,
    STM2,
    STM3,
    LISTSTM,
    LITERAL,
    LITERAL2,
    LITERAL3,
    VAR,
    LISTVAR,
    SUMA,
    MULT,
    RESTA,
    PROG,
    BLOCK1,
    BLOCK2,
    BLOCK3,
    PARAMETERS,
    LISTPARAM,
    IDENTIFICADOR,
    IDENTIFICADOR2,
    LISTEXPR,
    NEGACION,
    NEGATIVO,
    PORC,
    DIV,
    CONJUNCION,
    DISYUNCION,
    IGUAL,
    MENOR,
    MAYOR,
    WHILELOOP,
    IFTHENELSE,
    RETURN1,
    RETURN2,
    PARENTESIS
} t;

typedef enum tType
{
    boolean,
    integer
} tV;

typedef enum tTAC
{
    BEGIN_PROG,
    END_PROG,
    SUM,
    RES,
    MUL,
    SPLIT,
    PERCENTAGE,
    AAND,
    OOR,
    EEQUAL,
    LESS,
    HIGHER,
    NEGATIVE,
    DENIAL,
    BEGIN_FUNCTION,
    END_FUNCTION,
    ASSIG,
    CALL_F,
    LOAD_PARAM,
    RET,
    PARENT, 
    IIF, 
    WWHILE,
    BLOCK_T,
    BLOCK_F,
    BLOCK_W,
    END_BLOCK_T,
    END_BLOCK_F,
    END_BLOCK_W
} tT;

/*Estructura del nodo*/
struct infoToken
{
    enum tLabel flag;
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

/* ------------------ Estructura para el codigo de tres direcciones ------------------- */

/* Estructura TAC */
struct quaternary
{
    enum tTAC op;
    info *arg1;
    info *arg2;
    info *result;
    struct quaternary *next;
};

typedef struct quaternary tStack;

 
#endif