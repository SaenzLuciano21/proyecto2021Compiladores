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
    RETURN2
} t;

typedef enum tType
{
    boolean,
    integer
} tV;

typedef enum tThreeAddress
{
    IC_BEGIN,
    IC_END,
    IC_ADD,
    IC_SUB,
    IC_PLUS,
    IC_DIV,
    IC_MOD,
    IC_AND,
    IC_OR,
    IC_NOT,
    IC_EQUALAR,
    IC_EQUALLOG,
    IC_NEG,
    IC_MINOR,
    IC_MAJOR,
    IC_ASSING,
    IC_IF,
    IC_WHILE,
    IC_LABEL,
    IC_JUMP,
    IC_RETINT,
    IC_RETBOOL,
    IC_RETVOID,
    IC_PPARAM,
    IC_CALL,
    IC_LOAD,
    IC_BEGIN_FUNCTION,
    IC_END_FUNCTION
}tTDC;

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
    enum tLabel op;
    info *arg1;
    info *arg2;
    info *result;
    struct quaternary *next;
};

typedef struct quaternary tStack;

 
#endif