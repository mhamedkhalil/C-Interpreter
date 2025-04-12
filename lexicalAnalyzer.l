%{
// Keywords: if, else, int, return, void, while
// Special Symbols: + - * / < <= > >= == != = ; , ( ) [ ] { } /* */
// ID = letter (letter | digit)* ((“@”|”$” | “_”)? Digit+)?
// NUM = digit (“.”)? digit+((E |e) (+|-)? digit+)?
// letter = a|...|z|A|...|Z
// digit = 0|...|9
// Case sensitive 
// whitespaces should be ignored and seperate tokens (include \n \t and blank)  

//Changes
// Each keyword returns unique token
// Each special symbol is grouped with its kind (addop, relop, mulop..)
// Brackets have been separated into different types and tokens
// ID and NUM remain the same
// All errors have been given the token- ERROR

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "Tokens.h"

int line_number = 1;   // to track line numbers
int char_position = 1; // to track character position

Token currentToken;
%}

%x COMMENT_STATE 

%option noyywrap 
DIGIT  [0-9]
LETTER [a-zA-Z]
ID     {LETTER}({LETTER}|{DIGIT})*(("@"|"$"|"_")?{DIGIT}+)?
NUM    {DIGIT}(\.)?{DIGIT}+((E|e)(\+|-)?{DIGIT}+)?
WS     [ \t\r]+
NEWLINE \n

%%
"Program" { currentToken.type = PROGRAM; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
	    return currentToken;}

"else"    { currentToken.type = ELSE; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken;
	   }

"if"      { currentToken.type = IF; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken;}

"int"|"float"|"void"     { currentToken.type = TYPE; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken;}

"return"  { currentToken.type = RETURN; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken;}

"while"   { currentToken.type = WHILE; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken;
           }

[*/]        { currentToken.type = MULOP; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken;}

[+\-]        {currentToken.type = ADDOP; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken;}

==|!=|<=|>= { currentToken.type = RELOP; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken;}

= {currentToken.type = ASSIGN; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken;}


; { currentToken.type = SEMICOLON; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken;}

, {  currentToken.type = COMMA; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken; }

\( {  currentToken.type = OPEN_PAR; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken;}

\) { currentToken.type = CLOSED_PAR; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken;}

\[ { currentToken.type = OPEN_SQUARE; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken;}

\] {  currentToken.type = CLOSED_SQUARE; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken;}

"{" {currentToken.type = OPEN_CURL; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken;}

"}" {currentToken.type = CLOSED_CURL; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken;}


{ID}      { currentToken.type = ID; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken;}

{ID}[^0-9a-zA-Z \t\n]+ {currentToken.type = ERROR; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken;}

{NUM}     { currentToken.type = NUM; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken;}


{NUM}{ID} { fprintf(token_file,"TOKEN TYPE: NUM ID, Lexeme: %s\n", yytext); char_position += yyleng;}


{NUM}[a-zA-Z!@#$%&-="_]+ { currentToken.type = ERROR; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken;}

{WS}      { char_position += yyleng; char_position += yyleng; } 
{NEWLINE} { line_number++; char_position = 1; } 

"/*"                        { BEGIN(COMMENT_STATE); }
<COMMENT_STATE>{NEWLINE}    { line_number++; char_position=1; }
<COMMENT_STATE>[^*\n]*        { } 
<COMMENT_STATE>"*"+[^*/\n]    { } 
<COMMENT_STATE>"*/"         { BEGIN(INITIAL); }
<COMMENT_STATE><<EOF>>      { printf("ERROR: Unclosed comment at line %d, position %d\n", line_number, char_position); exit(1); }

[a-zA-Z]#([^a-zA-Z0-9]*) { currentToken.type = ERROR; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken;}

[0-9]+[eE][+-]?[^0-9]+ {currentToken.type = ERROR; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken; }

. { currentToken.type = ERROR; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return currentToken; }
%%

