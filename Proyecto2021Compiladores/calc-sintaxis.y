%{
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Tree.c"

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
%type<p> list_parameters
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

list_parameters: parameters                         { $$ = create_bNode(LISTPARAM, NULL, $1, NULL, NULL); }
;

parameters: type ID                                 {   info *infP = (info *)malloc(sizeof(info));
                                                        infP->flag = PARAMETERS;
                                                        infP->name = $2;
                                                        infP->type = (enum tType)$1;
                                                        infP->line = yylineno;
                                                        $$ = create_bNode(PARAMETERS, infP, NULL, NULL, NULL); }
| type ID ',' parameters                            {   info *infP = (info *)malloc(sizeof(info));
                                                        infP->flag = PARAMETERS;
                                                        infP->name = $2;
                                                        infP->type = (enum tType)$1;
                                                        infP->line = yylineno;
                                                        $$ = create_bNode(PARAMETERS, infP, NULL, NULL, $4); }
;

method_decl: type ID '(' list_parameters')' block    {   info *infM = (info *)malloc(sizeof(info));
                                                    infM->flag = PMETHOD;
                                                    infM->name = $2;
                                                    infM->type = (enum tType)$1;
                                                    infM->line = yylineno;
                                                    $$ = create_bNode(PMETHOD, infM, $4, NULL, $6); }

| type ID '(' list_parameters')' EXTERN ';'          {   info *infM = (info *)malloc(sizeof(info));
                                                    infM->flag = PMETHODE;
                                                    infM->name = $2;
                                                    infM->type = (enum tType)$1;
                                                    infM->line = yylineno;
                                                    $$ = create_bNode(PMETHODE, infM, $4, NULL, NULL); }

| VOID ID '(' list_parameters')' block               {   info *infM = (info *)malloc(sizeof(info));
                                                    infM->flag = PMETHOD;
                                                    infM->name = $2;
                                                    infM->line = yylineno;
                                                    $$ = create_bNode(PMETHOD, infM, $4, NULL, $6); }

| VOID ID '(' list_parameters')' EXTERN ';'          {   info *infM = (info *)malloc(sizeof(info));
                                                    infM->flag = PMETHODE;
                                                    infM->name = $2;
                                                    infM->line = yylineno;
                                                    $$ = create_bNode(PMETHODE, infM, $4, NULL, NULL); }

| VOID ID '('  ')' block                        {   info *infM = (info *)malloc(sizeof(info));
                                                    infM->flag = METHOD;
                                                    infM->name = $2;
                                                    infM->line = yylineno;
                                                    $$ = create_bNode(METHOD, infM, NULL, NULL, $5); }

| type ID '(' ')' EXTERN ';'                    {   info *infM = (info *)malloc(sizeof(info));
                                                    infM->flag = METHOD2;
                                                    infM->name = $2;
                                                    infM->type = (enum tType)$1;
                                                    infM->line = yylineno;
                                                    $$ = create_bNode(METHOD2, infM, NULL, NULL, NULL); }
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
                                                    infI->flag = VAR;
                                                    infI->type = (enum tType)$1;
                                                    infI->line = yylineno;
                                                    $$ = create_bNode(VAR, infI, $2, NULL, NULL); }
;

list_id: ID                                     {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->flag = IDENTIFICADOR;
                                                    infI->name = $1;
                                                    infI->line = yylineno;
                                                    $$ = create_bNode(IDENTIFICADOR, infI, NULL, NULL, NULL); }
| ID ',' list_id                                {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->flag = IDENTIFICADOR;
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
                                                    infS->flag = STM1;
                                                    infS->name = $1;
                                                    infS->line = yylineno;
                                                    $$ = create_bNode(STM1, infS, NULL, NULL, $3); }
| method_call ';'                               {   
                                                    $$ = create_bNode(STM2, NULL, $1, NULL, NULL); }
| IF '(' expr ')' THEN block ELSE block         {   
                                                    $$ = create_bNode(IFTHENELSE, NULL, $3, $6, $8); }
| WHILE '(' expr ')' block                      {   
                                                    $$ = create_bNode(WHILELOOP, NULL, $3, NULL, $5); }
| RETURN expr ';'                               {   info *infS = (info *)malloc(sizeof(info));
                                                    infS->flag = RETURN1;
                                                    infS->line = yylineno;
                                                    $$ = create_bNode(RETURN1, infS, $2, NULL, NULL); }
| RETURN ';'                                    {   
                                                    $$ = create_bNode(RETURN2, NULL, NULL, NULL, NULL); }
| ';'                                           {   
                                                    $$ = create_bNode(STM3, NULL, NULL, NULL, NULL); }
| block                                         {   $$ = $1; }
;

method_call: ID '(' ')'                         {   info *infM = (info *)malloc(sizeof(info));
                                                    infM->flag = CMETHOD;
                                                    infM->name = $1;
                                                    infM->line = yylineno;
                                                    $$ = create_bNode(CMETHOD, infM, NULL, NULL, NULL); }
| ID '(' list_expr ')'                          {   info *infM = (info *)malloc(sizeof(info));
                                                    infM->flag = CPMETHOD;
                                                    infM->name = $1;
                                                    infM->line = yylineno;
                                                    $$ = create_bNode(CPMETHOD, infM, $3, NULL, NULL); }
;

list_expr: expr                                 {   $$ = $1; }
| expr ',' list_expr                            {   
                                                    $$ = create_bNode(LISTEXPR, NULL, $1, NULL, $3); }
;

expr: ID                                        {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->flag = IDENTIFICADOR2;
                                                    infI->name = $1;
                                                    infI->line = yylineno;
                                                    $$ = create_bNode(IDENTIFICADOR2, infI, NULL, NULL, NULL); }
| method_call                                   {   $$ = $1; }
| litaral                                       {   $$ = $1; }
| expr '+' expr                                 {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->flag = SUMA;
                                                    infI->line = yylineno;
                                                    $$ = create_bNode(SUMA, infI, $1, NULL, $3); }
| expr '-' expr                                 {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->flag = RESTA;
                                                    infI->line = yylineno;
                                                    $$ = create_bNode(RESTA, infI, $1, NULL, $3); }
| expr '*' expr                                 {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->flag = MULT;
                                                    infI->line = yylineno;
                                                    $$ = create_bNode(MULT, infI, $1, NULL, $3); }
| expr '/' expr                                 {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->flag = DIV;
                                                    infI->line = yylineno;
                                                    $$ = create_bNode(DIV, infI, $1, NULL, $3); }
| expr '%' expr                                 {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->flag = PORC;
                                                    infI->line = yylineno;
                                                    $$ = create_bNode(PORC, infI, $1, NULL, $3); }
| expr AND expr                                 {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->flag = CONJUNCION;
                                                    infI->line = yylineno;
                                                    $$ = create_bNode(CONJUNCION, infI, $1, NULL, $3); }
| expr OR expr                                  {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->flag = DISYUNCION;
                                                    infI->line = yylineno;
                                                    $$ = create_bNode(DISYUNCION,infI, $1, NULL, $3); }
| expr EQUAL expr                               {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->flag = IGUAL;
                                                    infI->line = yylineno;
                                                    $$ = create_bNode(IGUAL, infI, $1, NULL, $3); }
| expr '<' expr                                 {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->flag = MENOR;
                                                    infI->line = yylineno;
                                                    $$ = create_bNode(MENOR, infI, $1, NULL, $3); }
| expr '>' expr                                 {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->flag = MAYOR;
                                                    infI->line = yylineno;
                                                    $$ = create_bNode(MAYOR, infI, $1, NULL, $3); }
| '-' expr %prec UNARY                          {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->flag = NEGATIVO;
                                                    infI->line = yylineno;
                                                    $$ = create_bNode(NEGATIVO, infI, $2, NULL, NULL); }
| '!' expr %prec UNARY                          {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->flag = NEGACION;
                                                    infI->line = yylineno;
                                                    $$ = create_bNode(NEGACION, infI, $2, NULL, NULL); }
| '(' expr ')'                                  {   info *infI = (info *)malloc(sizeof(info));
                                                    infI->flag = PARENTESIS;
                                                    infI->line = yylineno;
                                                    $$ = create_bNode(PARENTESIS, infI, $2, NULL, NULL); }
;

litaral: INT                                    {   info *infS = (info *)malloc(sizeof(info));
                                                    infS->flag = LITERAL;
                                                    infS->value = $1;
                                                    infS->type = integer;
                                                    infS->line = yylineno;
                                                    $$ = create_bNode(LITERAL, infS, NULL, NULL, NULL); }
| bool_literal                                  {   $$ = $1; }
;

bool_literal: TRUE                              {   info *infS = (info *)malloc(sizeof(info));
                                                    infS->flag = LITERAL2;
                                                    infS->value = 1;
                                                    infS->type = boolean;
                                                    infS->line = yylineno;
                                                    $$ = create_bNode(LITERAL2, infS, NULL, NULL, NULL); }
| FALSE                                         {   info *infS = (info *)malloc(sizeof(info));
                                                    infS->flag = LITERAL3;
                                                    infS->value = 0;
                                                    infS->type = boolean;
                                                    infS->line = yylineno;
                                                    $$ = create_bNode(LITERAL3, infS, NULL, NULL, NULL); }
;
%%