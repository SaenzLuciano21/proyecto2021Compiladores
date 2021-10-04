%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tree.c"

struct bTree *ast;
%} 
%union { int i; char *s; struct bTree *p; enum tType * t; }
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

%type<p> prog
%type<p> list_method_decl
%type<p> method_decl
%type<p> parameters
%type<p> block
%type<p> list_var_decl
%type<p> var_decl
%type<p> list_id
%type<t> type
%type<p> list_statement
%type<p> statement
%type<p> method_call
%type<p> list_expr
%type<p> expr
%type<p> litaral
%type<p> bool_literal

/*precedence*/
%left AND OR
%nonassoc '<' '>' EQUAL
%left '+' '-'
%left '*' '/' '%'
%left UNARY
/*preguntar como representar la terna en el caso del IF*/
/*rellenar la informacion de los nodos*/
/*escribir tests correctos e incorrectos*/
/*testear el arbol*/
%%
prog: PROGRAM '{' list_var_decl list_method_decl '}' {  info *infP = (info *)malloc(sizeof(info));
                                                        ast = create_bNode(PROG,  infP, $3, $4); 
                                                        inOrder(ast); }
| PROGRAM '{' list_var_decl '}'                      {  info *infP = (info *)malloc(sizeof(info));
                                                        ast = create_bNode(PROG,  infP, $3, NULL); 
                                                        inOrder(ast); }
| PROGRAM '{' list_method_decl '}'                   {  info *infP = (info *)malloc(sizeof(info));
                                                        ast = create_bNode(PROG,  infP, NULL, $3); 
                                                        inOrder(ast); }
| PROGRAM '{' '}'                                    {  info *infP = (info *)malloc(sizeof(info));
                                                        ast = create_bNode(PROG,  infP, NULL, NULL); 
                                                        inOrder(ast); }
;

list_method_decl: method_decl                   {   $$ = $1; }
| method_decl list_method_decl                  {   info *infM = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(METHOD, infM, $1, $2); }
;

parameters: type ID                             {   info *infP = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(PARAMETERS, infP, NULL, NULL); }
| type ID ',' parameters                        {   info *infP = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(PARAMETERS, infP, NULL, $4); }
;

method_decl: type ID '(' parameters')' block    {   info *infM = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(METHOD, infM, $4, $6); }
| type ID '(' parameters')' EXTERN ';'          {   info *infM = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(METHOD, infM, $4, NULL); }
| VOID ID '(' parameters')' block               {   info *infM = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(METHOD, infM, $4, $6); }
| VOID ID '(' parameters')' EXTERN ';'          {   info *infM = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(METHOD, infM, $4, NULL); }
| VOID ID '('  ')' block                        {   info *infM = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(METHOD, infM, NULL, $5); }
| type ID '(' ')' EXTERN ';'                    {   info *infM = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(METHOD, infM, NULL, NULL); }
;

block: '{' list_var_decl '}'                    {   info *infB = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(BLOCK, infB, $2, NULL); }
| '{' list_statement '}'                        {   info *infB = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(BLOCK, infB, NULL, $2); }
| '{' list_var_decl list_statement'}'           {   info *infB = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(BLOCK, infB, $2, $3); }
;

list_var_decl: var_decl                         {   $$ = $1; }
| list_var_decl var_decl                        {   info *infV = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(VAR, infV, $2, $1); }
;

var_decl: type list_id ';'                      {   info *infV = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(VAR, infV, $2, NULL); }
;

list_id: ID                                     {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->name = $1;
                                                    $$ = create_bNode(IDENTIFICADOR, infI, NULL, NULL); }
| ID ',' list_id                                {   info *infI = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(IDENTIFICADOR, infI, $3, NULL); }
;

type: INTEGER                                   {   enum tType *aux = (enum tType *)malloc(sizeof(enum tType)); 
                                                    *aux = integer;
                                                    $$ = aux; }
| BOOL                                          {   enum tType *aux = (enum tType *)malloc(sizeof(enum tType)); 
                                                    *aux = boolean;
                                                    $$ = aux; }
;

list_statement: statement                       {   $$ = $1; }
| statement list_statement                      {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(STM, infS, $1, $2); }
;

statement: ID '=' expr ';'                      {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(STM, infS, NULL, $3); }
| method_call ';'                               {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(STM, infS, $1, NULL); }
| IF '(' expr ')' THEN block ELSE block         {   info *infS = (info *)malloc(sizeof(info));
                                                    info *infE = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(STM, infS, $3, create_bNode(STM, infE, $6, $8)); }
| WHILE '(' expr ')' block                      {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(STM, infS, $3, $5); }
| RETURN expr ';'                               {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(STM, infS, $2, NULL); }
| RETURN ';'                                    {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(STM, infS, NULL, NULL); }
| ';'                                           {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(STM, infS, NULL, NULL); }
| block                                         {   $$ = $1; }
;

method_call: ID '(' ')'                         {   info *infM = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(METHOD, infM, NULL, NULL); }
| ID '(' list_expr ')'                          {   info *infoM = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(METHOD, infoM, $3, NULL); }
;

list_expr: expr                                 {   $$ = $1; }
| expr ',' list_expr                            {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(EXPR, infS, $1, $3); }
;

expr: ID                                        {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->name = $1;
                                                    $$ = create_bNode(IDENTIFICADOR, infI, NULL, NULL); }
| method_call                                   {   $$ = $1; }
| litaral                                       {   $$ = $1; }
| expr '+' expr                                 {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(SUMA, infS, $1, $3); }
| expr '-' expr                                 {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(RESTA, infS, $1, $3); }
| expr '*' expr                                 {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(MULT, infS, $1, $3); }
| expr '/' expr                                 {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(DIV, infS, $1, $3); }
| expr '%' expr                                 {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(PORC, infS, $1, $3); }
| expr AND expr                                 {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(IAND, infS, $1, $3); }
| expr OR expr                                  {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(IOR, infS, $1, $3); }
| expr EQUAL expr                               {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(IGUAL, infS, $1, $3); }
| expr '<' expr                                 {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(MENOR, infS, $1, $3); }
| expr '>' expr                                 {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(MAYOR, infS, $1, $3); }
| '-' expr %prec UNARY                          {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(NEGATIVO, infS, $2, NULL); }
| '!' expr %prec UNARY                          {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(NEGACION, infS, $2, NULL); }
| '(' expr ')'                                  {   $$ = $2 }
;

litaral: INT                                    {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(LITERAL, infS, NULL, NULL); }
| bool_literal                                  {   $$ = $1; }
;

bool_literal: TRUE                              {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(LITERAL, infS, NULL, NULL); }
| FALSE                                         {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(LITERAL, infS, NULL, NULL); }
;
%%