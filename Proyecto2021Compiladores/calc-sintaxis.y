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
/*escribir tests correctos e incorrectos*/
/*testear el arbol*/
%%
prog: PROGRAM '{' list_var_decl list_method_decl '}' {  }
| PROGRAM '{' list_var_decl '}'                      {  }
| PROGRAM '{' list_method_decl '}'                   {  }
| PROGRAM '{' '}'                                    {  }
;

list_method_decl: method_decl                   {  }
| method_decl list_method_decl                  {  }
;

parameters: type ID                             {  }
| type ID ',' parameters                        {  }
;

method_decl: type ID '(' parameters')' block    {  }
| type ID '(' parameters')' EXTERN ';'          {  }
| VOID ID '(' parameters')' block               {  }
| VOID ID '(' parameters')' EXTERN ';'          {  }
| VOID ID '('  ')' block                        {  }
| type ID '(' ')' EXTERN ';'                    {  }
;

block: '{' list_var_decl '}'                    {  }
| '{' list_statement '}'                        {  }
| '{' list_var_decl list_statement'}'           {  }
;

list_var_decl: var_decl                         {  }
| list_var_decl var_decl                        {  }
;

var_decl: type list_id ';'                      {  }
;

list_id: ID                                     {  }
| ID ',' list_id                                {  }
;

type: INTEGER
| BOOL
;

list_statement: statement                       {  }
| statement list_statement                      {  }
;

statement: ID '=' expr ';'                      {  }
| method_call ';'                               {  }
| IF '(' expr ')' THEN block ELSE block         {  }
| WHILE '(' expr ')' block                      {  }
| RETURN expr ';'                               {  }
| RETURN ';'                                    {  }
| ';'                                           {  }
| block                                         {  }
;

method_call: ID '(' ')'                         {  }
| ID '(' list_expr ')'                          {  }
;

list_expr: expr                                 {  }
| expr ',' list_expr                            {  }
;

expr: ID                                        {  }
| method_call                                   {  }
| litaral                                       {  }
| expr '+' expr                                 {  }
| expr '-' expr                                 {  }
| expr '*' expr                                 {  }
| expr '/' expr                                 {  }
| expr '%' expr                                 {  }
| expr AND expr                                 {  }
| expr OR expr                                  {  }
| expr EQUAL expr                               {  }
| expr '<' expr                                 {  }
| expr '>' expr                                 {  }
| '-' expr %prec UNARY                          {  }
| '!' expr %prec UNARY                          {  }
| '(' expr ')'                                  {  }
;

litaral: INT                                    {  }
| bool_literal                                  {  }
;

bool_literal: TRUE                              {  }
| FALSE                                         {  }
;
%%