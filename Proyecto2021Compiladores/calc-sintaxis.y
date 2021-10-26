%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tree.c"

struct bTree *ast;
extern int yylineno;
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
%%
prog: PROGRAM '{' list_var_decl list_method_decl '}'{  
                                                        ast = create_bNode(PROG, NULL, $3, NULL, $4);
                                                        inOrder(ast); }
| PROGRAM '{' list_var_decl '}'                     {  
                                                        ast = create_bNode(PROG, NULL, $3, NULL, NULL); 
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
                                                        $$ = create_bNode(LISTMETHOD, NULL, $1, NULL, $2); }
;

parameters: type ID                                 {   info *infP = (info *)malloc(sizeof(info));
                                                        infP->label = PARAMETERS;
                                                        infP->name = $2;
                                                        infP->type = (enum tType)$1;
                                                        infP->line = yylineno;
                                                        $$ = create_bNode(PARAMETERS, infP, NULL, NULL, NULL); }
| type ID ',' parameters                            {   info *infP = (info *)malloc(sizeof(info));
                                                        infP->label = PARAMETERS;
                                                        infP->name = $2;
                                                        infP->type = (enum tType)$1;
                                                        infP->line = yylineno;
                                                        $$ = create_bNode(PARAMETERS, infP, NULL, NULL, $4); }
;

method_decl: type ID '(' parameters')' block    {   info *infM = (info *)malloc(sizeof(info));
                                                    infM->label = PMETHOD;
                                                    infM->name = $2;
                                                    infM->type = (enum tType)$1;
                                                    infM->line = yylineno;
                                                    $$ = create_bNode(PMETHOD, infM, $4, NULL, $6); }

| type ID '(' parameters')' EXTERN ';'          {   info *infM = (info *)malloc(sizeof(info));
                                                    infM->label = PMETHOD;
                                                    infM->name = $2;
                                                    infM->type = (enum tType)$1;
                                                    infM->line = yylineno;
                                                    $$ = create_bNode(PMETHOD, infM, $4, NULL, NULL); }

| VOID ID '(' parameters')' block               {   info *infM = (info *)malloc(sizeof(info));
                                                    infM->label = PMETHOD;
                                                    infM->name = $2;
                                                    infM->line = yylineno;
                                                    $$ = create_bNode(PMETHOD, infM, $4, NULL, $6); }

| VOID ID '(' parameters')' EXTERN ';'          {   info *infM = (info *)malloc(sizeof(info));
                                                    infM->label = PMETHOD;
                                                    infM->name = $2;
                                                    infM->line = yylineno;
                                                    $$ = create_bNode(PMETHOD, infM, $4, NULL, NULL); }

| VOID ID '('  ')' block                        {   info *infM = (info *)malloc(sizeof(info));
                                                    infM->label = METHOD2;
                                                    infM->name = $2;
                                                    infM->line = yylineno;
                                                    $$ = create_bNode(METHOD2, infM, NULL, NULL, $5); }

| type ID '(' ')' EXTERN ';'                    {   info *infM = (info *)malloc(sizeof(info));
                                                    infM->label = METHOD2;
                                                    infM->name = $2;
                                                    infM->type = (enum tType)$1;
                                                    infM->line = yylineno;
                                                    $$ = create_bNode(METHOD3, infM, NULL, NULL, NULL); }
;

block: '{' list_var_decl '}'                    {   
                                                    $$ = create_bNode(BLOCK1, NULL, $2, NULL, NULL); }
| '{' list_statement '}'                        {   
                                                    $$ = create_bNode(BLOCK2, NULL, NULL, NULL, $2); }
| '{' list_var_decl list_statement'}'           {   
                                                    $$ = create_bNode(BLOCK3, NULL, $2, NULL, $3); }
;

list_var_decl: var_decl                         {   $$ = $1; }
| list_var_decl var_decl                        {   
                                                    $$ = create_bNode(LISTVAR, NULL, $2, NULL, $1); }
;

var_decl: type list_id ';'                      {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->label = VAR;
                                                    infI->type = (enum tType)$1;
                                                    infI->line = yylineno;
                                                    $$ = create_bNode(VAR, infI, $2, NULL, NULL); }
;

list_id: ID                                     {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->label = IDENTIFICADOR;
                                                    infI->name = $1;
                                                    infI->line = yylineno;
                                                    $$ = create_bNode(IDENTIFICADOR, infI, NULL, NULL, NULL); }
| ID ',' list_id                                {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->label = IDENTIFICADOR;
                                                    infI->name = $1;
                                                    infI->line = yylineno;
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
                                                    $$ = create_bNode(LISTSTM, NULL, $1, NULL, $2); }
;

statement: ID '=' expr ';'                      {   info *infS = (info *)malloc(sizeof(info));
                                                    infS->label = STM1;
                                                    infS->name = $1;
                                                    infS->line = yylineno;
                                                    $$ = create_bNode(STM1, infS, NULL, NULL, $3); }
| method_call ';'                               {   
                                                    $$ = create_bNode(STM2, NULL, $1, NULL, NULL); }
| IF '(' expr ')' THEN block ELSE block         {   
                                                    $$ = create_bNode(IF, NULL, $3, $6, $8); }
| WHILE '(' expr ')' block                      {   
                                                    $$ = create_bNode(WHILE, NULL, $3, NULL, $5); }
| RETURN expr ';'                               {   
                                                    $$ = create_bNode(RETURN, NULL, $2, NULL, NULL); }
| RETURN ';'                                    {   
                                                    $$ = create_bNode(RETURN, NULL, NULL, NULL, NULL); }
| ';'                                           {   
                                                    $$ = create_bNode(STM3, NULL, NULL, NULL, NULL); }
| block                                         {   $$ = $1; }
;

method_call: ID '(' ')'                         {   info *infM = (info *)malloc(sizeof(info));
                                                    infM->label = CMETHOD;
                                                    infM->name = $1;
                                                    infM->line = yylineno;
                                                    $$ = create_bNode(CMETHOD, infM, NULL, NULL, NULL); }
| ID '(' list_expr ')'                          {   info *infM = (info *)malloc(sizeof(info));
                                                    infM->label = CPMETHOD;
                                                    infM->name = $1;
                                                    infM->line = yylineno;
                                                    $$ = create_bNode(CPMETHOD, infM, $3, NULL, NULL); }
;

list_expr: expr                                 {   $$ = $1; }
| expr ',' list_expr                            {   
                                                    $$ = create_bNode(EXPR, NULL, $1, NULL, $3); }
;

expr: ID                                        {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->label = IDENTIFICADOR;
                                                    infI->name = $1;
                                                    infI->line = yylineno;
                                                    $$ = create_bNode(IDENTIFICADOR, infI, NULL, NULL, NULL); }
| method_call                                   {   $$ = $1; }
| litaral                                       {   $$ = $1; }
| expr '+' expr                                 {   
                                                    $$ = create_bNode(SUMA, NULL, $1, NULL, $3); }
| expr '-' expr                                 {   
                                                    $$ = create_bNode(RESTA, NULL, $1, NULL, $3); }
| expr '*' expr                                 {   
                                                    $$ = create_bNode(MULT, NULL, $1, NULL, $3); }
| expr '/' expr                                 {   
                                                    $$ = create_bNode(DIV, NULL, $1, NULL, $3); }
| expr '%' expr                                 {   
                                                    $$ = create_bNode(PORC, NULL, $1, NULL, $3); }
| expr AND expr                                 {   
                                                    $$ = create_bNode(IAND, NULL, $1, NULL, $3); }
| expr OR expr                                  {   
                                                    $$ = create_bNode(IOR,NULL, $1, NULL, $3); }
| expr EQUAL expr                               {   
                                                    $$ = create_bNode(IGUAL, NULL, $1, NULL, $3); }
| expr '<' expr                                 {   
                                                    $$ = create_bNode(MENOR, NULL, $1, NULL, $3); }
| expr '>' expr                                 {   
                                                    $$ = create_bNode(MAYOR, NULL, $1, NULL, $3); }
| '-' expr %prec UNARY                          {   
                                                    $$ = create_bNode(NEGATIVO, NULL, $2, NULL, NULL); }
| '!' expr %prec UNARY                          {   
                                                    $$ = create_bNode(NEGACION, NULL, $2, NULL, NULL); }
| '(' expr ')'                                  {   $$ = $2; }
;

litaral: INT                                    {   info *infS = (info *)malloc(sizeof(info));
                                                    infS->label = LITERAL;
                                                    infS->value = $1;
                                                    infS->type = integer;
                                                    infS->line = yylineno;
                                                    $$ = create_bNode(LITERAL, infS, NULL, NULL, NULL); }
| bool_literal                                  {   $$ = $1; }
;

bool_literal: TRUE                              {   info *infS = (info *)malloc(sizeof(info));
                                                    infS->label = LITERAL2;
                                                    infS->value = 1;
                                                    infS->type = boolean;
                                                    infS->line = yylineno;
                                                    $$ = create_bNode(LITERAL2, infS, NULL, NULL, NULL); }
| FALSE                                         {   info *infS = (info *)malloc(sizeof(info));
                                                    infS->label = LITERAL3;
                                                    infS->value = 0;
                                                    infS->type = boolean;
                                                    infS->line = yylineno;
                                                    $$ = create_bNode(LITERAL3, infS, NULL, NULL, NULL); }
;
%%