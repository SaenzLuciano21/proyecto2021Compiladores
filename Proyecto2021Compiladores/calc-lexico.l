%{
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "calc-sintaxis.tab.h"

void yyerror();
%}
%option noyywrap
%option yylineno

letra [a-zA-Z]
digito [0-9]
arith_op [-+*/%]
rel_op [<>]
%%
"integer"   				{
								yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
								strcpy(yylval.s,yytext);
								printf("INTEGER: %s\n",yytext);
								return INTEGER; }

"bool"						{	
								yylval.s=(char*) malloc(sizeof(yytext)*yyleng);
								strcpy(yylval.s,yytext);
								printf("BOOL: %s\n",yytext);
								return BOOL; }

"false"     				{
								yylval.s=(char*) malloc(sizeof(yytext)*yyleng);
								strcpy(yylval.s,yytext);
								printf("FALSE: %s\n",yytext);
								return FALSE; }

"true"      				{
								yylval.s=(char*) malloc(sizeof(yytext)*yyleng);
								strcpy(yylval.s,yytext);
								printf("TRUE: %s\n",yytext);
								return TRUE; }

"return"                    {
                                yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
                                strcpy(yylval.s,yytext);
                                printf("RETURN: %s\n",yytext);
                                return RETURN; }

"if"                    	{
                                yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
                                strcpy(yylval.s,yytext);
                                printf("IF: %s\n",yytext);
                                return IF; }

"then"                    	{
                                yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
                                strcpy(yylval.s,yytext);
                                printf("THEN: %s\n",yytext);
                                return THEN; }

"else"                    	{
                                yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
                                strcpy(yylval.s,yytext);
                                printf("ELSE: %s\n",yytext);
                                return ELSE; }

"while"                    	{
                                yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
                                strcpy(yylval.s,yytext);
                                printf("WHILE: %s\n",yytext);
                                return WHILE; }

"extern"                    {
                                yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
                                strcpy(yylval.s,yytext);
                                printf("EXTERN: %s\n",yytext);
                                return EXTERN; }

"void"                   	{
                                yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
                                strcpy(yylval.s,yytext);
                                printf("VOID: %s\n",yytext);
                                return VOID; }

"program"                   {
                                yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
                                strcpy(yylval.s,yytext);
                                printf("PROGRAM: %s\n",yytext);
                                return PROGRAM; }		

"||"                        {
                                yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
                                strcpy(yylval.s,yytext);
                                printf("OR: %s\n",yytext);
                                return OR; }

"&&"                        {
                                yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
                                strcpy(yylval.s,yytext);
                                printf("AND: %s\n",yytext);
                                return AND; }								

"=="                        {
                                yylval.s=(char *) malloc(sizeof(yytext)*yyleng);
                                strcpy(yylval.s,yytext);
                                printf("EQUAL: %s\n",yytext);
                                return EQUAL; }

{digito}+                   { 
                                yylval.i=atoi(yytext);
								printf("INT: %d\n",atoi(yytext)); 
                                return INT; }

{letra}({letra}|{digito})*  { 
								yylval.s=(char*) malloc(sizeof(yytext)*yyleng);
								strcpy(yylval.s,yytext);
                                printf("ID: %s\n",yytext);
                                return ID; }

{arith_op}|{rel_op}|"="|"!" { 
                                printf("OPERATION: %s\n",yytext);
                                return *yytext; }                               

[;(){}]       	            {	
                                printf("%s\n",yytext);
                            	return *yytext; }

\n|\                        { ; }
\t                          { ; } /*Comentario:Ignore whitespace and lineskip*/
"//".*\n                    { printf("Comentario: %s\n",yytext); }
.                           yyerror(); /* ignore all the rest */
%%

void yyerror() {
	printf("%s%d\n","-> ERROR Sintactico en la linea: ",yylineno);
}

int main(int argc,char *argv[]) {
	++argv,--argc;
	if (argc > 0)
		yyin = fopen(argv[0],"r");
	else
		yyin = stdin;
	yyparse();
}