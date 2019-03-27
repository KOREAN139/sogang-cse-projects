/****************************************************/
/* File: tiny.l                                     */
/* Lex specification for TINY                       */
/* Compiler Construction: Principles and Practice   */
/* Kenneth C. Louden                                */
/****************************************************/

%{
#include "globals.h"
#include "util.h"
#include "stdio.h"
/* follow standard C identifier length */
#define MAXTOKENLEN 31
char tokenString[MAXTOKENLEN+1];
void yyerror(char const *msg);
%}

digit       [0-9]
number      {digit}+
letter      [a-zA-Z]
identifier  {letter}+
newline     \n
whitespace  [ \t]+

%%

"if"            {return IF;}
"else"          {return ELSE;}
"int"           {return INT;}
"return"        {return RETURN;}
"void"          {return VOID;}
"while"         {return WHILE;}
"=="            {return EQ;}
"="             {return ASSIGN;}
"!="            {return NEQ;}
"<"             {return LT;}
"<="            {return LTE;}
">"             {return GT;}
">="            {return GTE;}
"+"             {return PLUS;}
"-"             {return MINUS;}
"*"             {return TIMES;}
"/"             {return OVER;}
"("             {return LPAREN;}
")"             {return RPAREN;}
"["             {return LBRAC;}
"]"             {return RBRAC;}
"{"             {return LCURLY;}
"}"             {return RCURLY;}
","             {return COMMA;}
";"             {return SEMI;}
"/*"            { register int c;
                  for ( ; ; )
                  { while ( (c = input()) != '*' && c != EOF );
                    if ( c == '*' )
                    { while ( (c = input()) == '*' );
                      if ( c == '/' )
                        break;
                    }
                    if ( c == EOF )
                    { yyerror( "EOF in comment" );
                      break;
                    }
                  }
                }
{number}        {return NUM;}
{identifier}    {return ID;}
{newline}       {lineno++;}
{whitespace}    {/* skip whitespace */}
.               {return ERROR;}

%%

TokenType getToken(void)
{ static int firstTime = TRUE;
  TokenType currentToken;
  if (firstTime)
  { firstTime = FALSE;
    lineno++;
    yyin = source;
    yyout = listing;
  }
  currentToken = yylex();
  strncpy(tokenString,yytext,MAXTOKENLEN);
  if (TraceScan) {
    fprintf(listing,"\t%d",lineno);
    printToken(currentToken,tokenString);
  }
  return currentToken;
}

int yywrap() { /* actions for reaching EOF */ }
void yyerror(char const *msg) { fprintf (stderr, "%s", msg); }
