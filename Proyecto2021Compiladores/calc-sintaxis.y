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
/*rellenar la informacion de los nodos*/
/*testear el arbol*/
%%
prog: PROGRAM '{' list_var_decl list_method_decl '}'{  
                                                        ast = create_bNode(PROG, NULL, $3, NULL, $4);
                                                        inOrder(ast); }
| PROGRAM '{' list_var_decl '}'                     {  
                                                        ast = create_bNode(PROG, NULL, $3, NULL, NULL); 
                                                        printf("----ENTRO----\n");
                                                        inOrder(ast); }
| PROGRAM '{' list_method_decl '}'                  {  
                                                        ast = create_bNode(PROG, NULL, NULL, NULL, $3); 
                                                        inOrder(ast); }
| PROGRAM '{' '}'                                   {  
                                                        ast = create_bNode(PROG, NULL, NULL, NULL, NULL); 
                                                        inOrder(ast); }
;

list_method_decl: method_decl                       {   $$ = $1; }
| method_decl list_method_decl                      {   
                                                        $$ = create_bNode(METHOD, NULL, $1, NULL, $2); }
;

parameters: type ID                                 {   info *infP = (info *)malloc(sizeof(info));
                                                        infP->name = $2;
                                                        //infP->type = $1;
                                                        $$ = create_bNode(PARAMETERS, infP, NULL, NULL, NULL); }
| type ID ',' parameters                            {   info *infP = (info *)malloc(sizeof(info));
                                                        infP->name = $2;
                                                        //infP->type = $1;
                                                        $$ = create_bNode(PARAMETERS, NULL, NULL, NULL, $4); }
;

method_decl: type ID '(' parameters')' block    {   info *infM = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(METHOD, NULL, $4, NULL, $6); }
| type ID '(' parameters')' EXTERN ';'          {   info *infM = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(METHOD, NULL, $4, NULL, NULL); }
| VOID ID '(' parameters')' block               {   info *infM = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(METHOD, NULL, $4, NULL, $6); }
| VOID ID '(' parameters')' EXTERN ';'          {   info *infM = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(METHOD, NULL, $4, NULL, NULL); }
| VOID ID '('  ')' block                        {   info *infM = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(METHOD, NULL, NULL, NULL, $5); }
| type ID '(' ')' EXTERN ';'                    {   info *infM = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(METHOD, NULL, NULL, NULL, NULL); }
;

block: '{' list_var_decl '}'                    {   
                                                    $$ = create_bNode(BLOCK, NULL, $2, NULL, NULL); }
| '{' list_statement '}'                        {   
                                                    $$ = create_bNode(BLOCK, NULL, NULL, NULL, $2); }
| '{' list_var_decl list_statement'}'           {   
                                                    $$ = create_bNode(BLOCK, NULL, $2, NULL, $3); }
;

list_var_decl: var_decl                         {   $$ = $1; }
| list_var_decl var_decl                        {   
                                                    $$ = create_bNode(VAR, NULL, $2, NULL, $1); }
;

var_decl: type list_id ';'                      {
                                                    $$ = create_bNode(VAR, NULL, $2, NULL, NULL); }
;

list_id: ID                                     {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->name = $1;
                                                    $$ = create_bNode(IDENTIFICADOR, infI, NULL, NULL, NULL); }
| ID ',' list_id                                {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->name = $1;
                                                    $$ = create_bNode(IDENTIFICADOR, infI, $3, NULL, NULL); }
;

type: INTEGER                                   {   enum tType *aux = (enum tType *)malloc(sizeof(enum tType)); 
                                                    *aux = integer;
                                                    $$ = aux; }
| BOOL                                          {   enum tType *aux = (enum tType *)malloc(sizeof(enum tType)); 
                                                    *aux = boolean;
                                                    $$ = aux; }
;

list_statement: statement                       {   $$ = $1; }
| statement list_statement                      {   
                                                    $$ = create_bNode(STM, NULL, $1, NULL, $2); }
;

statement: ID '=' expr ';'                      {   info *infS = (info *)malloc(sizeof(info));
                                                    infS->name = $1;
                                                    $$ = create_bNode(STM, infS, NULL, NULL, $3); }
| method_call ';'                               {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(STM, infS, $1, NULL, NULL); }
| IF '(' expr ')' THEN block ELSE block         {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(STM, infS, $3, $6, $8); }
| WHILE '(' expr ')' block                      {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(STM, infS, $3, NULL, $5); }
| RETURN expr ';'                               {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(STM, infS, $2, NULL, NULL); }
| RETURN ';'                                    {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(STM, infS, NULL, NULL, NULL); }
| ';'                                           {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(STM, infS, NULL, NULL, NULL); }
| block                                         {   $$ = $1; }
;

method_call: ID '(' ')'                         {   info *infM = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(METHOD, infM, NULL, NULL, NULL); }
| ID '(' list_expr ')'                          {   info *infoM = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(METHOD, infoM, $3, NULL, NULL); }
;

list_expr: expr                                 {   $$ = $1; }
| expr ',' list_expr                            {   
                                                    $$ = create_bNode(EXPR, NULL, $1, NULL, $3); }
;

expr: ID                                        {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->name = $1;
                                                    $$ = create_bNode(IDENTIFICADOR, infI, NULL, NULL, NULL); }
| method_call                                   {   $$ = $1; }
| litaral                                       {   $$ = $1; }
| expr '+' expr                                 {   info *infS = (info *)malloc(sizeof(info));
                                                    /*infS->value = expr + expr?*/
                                                    $$ = create_bNode(SUMA, infS, $1, NULL, $3); }
| expr '-' expr                                 {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(RESTA, infS, $1, NULL, $3); }
| expr '*' expr                                 {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(MULT, infS, $1, NULL, $3); }
| expr '/' expr                                 {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(DIV, infS, $1, NULL, $3); }
| expr '%' expr                                 {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(PORC, infS, $1, NULL, $3); }
| expr AND expr                                 {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(IAND, infS, $1, NULL, $3); }
| expr OR expr                                  {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(IOR, infS, $1, NULL, $3); }
| expr EQUAL expr                               {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(IGUAL, infS, $1, NULL, $3); }
| expr '<' expr                                 {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(MENOR, infS, $1, NULL, $3); }
| expr '>' expr                                 {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(MAYOR, infS, $1, NULL, $3); }
| '-' expr %prec UNARY                          {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(NEGATIVO, infS, $2, NULL, NULL); }
| '!' expr %prec UNARY                          {   info *infS = (info *)malloc(sizeof(info));
                                                    $$ = create_bNode(NEGACION, infS, $2, NULL, NULL); }
| '(' expr ')'                                  {   $$ = $2; }
;

litaral: INT                                    {   info *infS = (info *)malloc(sizeof(info));
                                                    infS->value = $1;
                                                    infS->type = integer;
                                                    $$ = create_bNode(LITERAL, infS, NULL, NULL, NULL); }
| bool_literal                                  {   $$ = $1; }
;

bool_literal: TRUE                              {   info *infS = (info *)malloc(sizeof(info));
                                                    infS->value = 1;
                                                    infS->type = boolean;
                                                    $$ = create_bNode(LITERAL, infS, NULL, NULL, NULL); }
| FALSE                                         {   info *infS = (info *)malloc(sizeof(info));
                                                    infS->value = 0;
                                                    infS->type = boolean;
                                                    $$ = create_bNode(LITERAL, infS, NULL, NULL, NULL); }
;
%%