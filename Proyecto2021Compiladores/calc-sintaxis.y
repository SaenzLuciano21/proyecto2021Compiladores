%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
%} 
%union { int i; char *s; }
%token<i> INT
%token<s> ID
%token<s> INTEGER
%token<s> BOOL
%token<s> EXTERN
%token<s> RETURN
%token<s> WHILE
%token<s> IF
%token<s> THEN
%token<s> ELSE
%token<s> FALSE
%token<s> TRUE
%token<s> VOID
%token<s> PROGRAM

/*precedence*/
%left AND OR
%nonassoc '<' '>' EQUAL
%left '+' '-'
%left '*' '/' '%'
%left UNARY
/*NEGATIVO UNITARIO como lo represento en el lexico?*/
/*escribir muchos test con errores y sin errores*/
/*ver los comentarios de ignorarlos*/
/*probar que funcione el ultimo token de /n /t y "" */
/*testear el arbol*/
/*method_decl no falta el caso donde la funcion no tiene parametros?*/
%%
prog: PROGRAM '{' list_var_decl list_method_decl '}' { printf("ENTRA PROGRAM LIST VAR AND METHOD DECL 1\n"); }
| PROGRAM '{' list_var_decl '}'                      { printf("ENTRA PROGRAM LIST VAR DECL 2\n"); }
| PROGRAM '{' list_method_decl '}'                   { printf("ENTRA PROGRAM LIST METHOD DECL3\n"); }
| PROGRAM '{' '}'                                    { printf("ENTRA PROGRAM 4\n"); }
;

list_method_decl: method_decl              { printf("ENTRA LIST METHOD DECL 1\n"); }
| method_decl list_method_decl             { printf("ENTRA LIST METHOD DECL 2\n"); }
;

parameters: type ID                        { printf("ENTRA PARAMETERS 1\n"); }
| type ID ',' parameters
;

method_decl: type ID '(' parameters')' block    { printf("ENTRA METHOD DECL 1\n"); }
| type ID '(' parameters')' EXTERN ';'
| VOID ID '(' parameters')' block
| VOID ID '(' parameters')' EXTERN ';'
;

block: '{' list_var_decl '}'                
| '{' list_statement '}'                    { printf("ENTRA BLOCK 2\n"); }
| '{' list_var_decl list_statement'}'
;

list_var_decl: var_decl                 { printf("ENTRA LIST VAR DECL 1\n"); }
| list_var_decl var_decl                { printf("ENTRA LIST VAR DECL 2\n"); }
;

var_decl: type list_id ';'
;

list_id: ID
| ID ',' list_id
;

type: INTEGER
| BOOL
;

list_statement: statement               { printf("ENTRA LIST STATEMENT 1\n"); }
| statement list_statement
;

statement: ID '=' expr ';'              { printf("ENTRA STATEMENT 1\n"); }
| method_call ';'
| IF '(' expr ')' THEN block ELSE block
| WHILE '(' expr ')' block
| RETURN expr ';'                      { printf("ENTRA STATEMENT RETURN EXPR 5\n"); }
| RETURN ';'
| ';'
| block
;

method_call: ID '(' ')'
| ID '(' list_expr ')'
;

list_expr: expr
| expr ',' list_expr
;

expr: ID                    { printf("ENTRA EXPR ID 1\n"); }
| method_call
| litaral                   { printf("ENTRA EXPR LITERAL 3\n"); }
| expr '+' expr             { printf("ENTRA EXPR EXPR + EXPR 4\n"); }
| expr '-' expr
| expr '*' expr
| expr '/' expr
| expr '%' expr
| expr AND expr
| expr OR expr
| expr EQUAL expr
| expr '<' expr
| expr '>' expr
| '-' expr %prec UNARY
| '!' expr %prec UNARY
| '(' expr ')'
;

litaral: INT                { printf("ENTRA LITERAL INT 1\n"); }
| bool_literal
;

bool_literal: TRUE
| FALSE
;