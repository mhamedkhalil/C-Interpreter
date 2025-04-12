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
#include <string.h>
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

printf("Info: Line: %d | Position: %d | 1 \n",
        currentToken.line,
        currentToken.position);

	    return PROGRAM;}

"else"    { currentToken.type = ELSE; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
printf("Info: Line: %d | Position: %d | 2 \n",
        currentToken.line,
        currentToken.position);
	    return ELSE;
	   }

"if"      { currentToken.type = IF; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
printf("Info: Line: %d | Position: %d | 3 \n",
        currentToken.line,
        currentToken.position);
	    return IF;}

"int"|"float"|"void"     { currentToken.type = TYPE; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
printf("Info: Line: %d | Position: %d | 4 \n",
        currentToken.line,
        currentToken.position);
	    return TYPE;}

"return"  { currentToken.type = RETURN; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
printf("Info: Line: %d | Position: %d | 5 \n",
        currentToken.line,
        currentToken.position);

	    return RETURN;}

"while"   { currentToken.type = WHILE; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
printf("Info: Line: %d | Position: %d | 6 \n",
        currentToken.line,
        currentToken.position);
	    return WHILE;
           }

[*/]        { currentToken.type = MULOP; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
printf("Info: Line: %d | Position: %d | 7 \n",
        currentToken.line,
        currentToken.position);
	    return MULOP;}

[+\-]        {currentToken.type = ADDOP; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
printf("Info: Line: %d | Position: %d | 8 \n",
        currentToken.line,
        currentToken.position);
	    return ADDOP;}

==|!=|<=|>=|>|< { currentToken.type = RELOP; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
printf("Info: Line: %d | Position: %d | 9 \n",
        currentToken.line,
        currentToken.position);
	    return RELOP;}

= {currentToken.type = ASSIGN; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
printf("Info: Line: %d | Position: %d | 10 \n",
        currentToken.line,
        currentToken.position);
	    return ASSIGN;}


; { currentToken.type = SEMICOLON; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
printf("Info: Line: %d | Position: %d | 11 \n",
        currentToken.line,
        currentToken.position);
	    return SEMICOLON;}

, {  currentToken.type = COMMA; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
printf("Info: Line: %d | Position: %d | 12 \n",
        currentToken.line,
        currentToken.position);
	    return COMMA; }

\( {  currentToken.type = OPEN_PAR; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
printf("Info: Line: %d | Position: %d | 13 \n",
        currentToken.line,
        currentToken.position);
	    return OPEN_PAR;}

\) { currentToken.type = CLOSED_PAR; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
printf("Info: Line: %d | Position: %d | 14 \n",
        currentToken.line,
        currentToken.position);
	    return CLOSED_PAR;}

\[ { currentToken.type = OPEN_SQUARE; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
printf("Info: Line: %d | Position: %d | 15 \n",
        currentToken.line,
        currentToken.position);
	    return OPEN_SQUARE;}

\] {  currentToken.type = CLOSED_SQUARE; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
printf("Info: Line: %d | Position: %d | 16 \n",
        currentToken.line,
        currentToken.position);
	    return CLOSED_SQUARE;}

"{" {currentToken.type = OPEN_CURL; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
printf("Info: Line: %d | Position: %d | 17 \n",
        currentToken.line,
        currentToken.position);
	    return OPEN_CURL;}

"}" {currentToken.type = CLOSED_CURL; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
printf("Info: Line: %d | Position: %d | 18 \n",
        currentToken.line,
        currentToken.position);
	    return CLOSED_CURL;}


{ID}      { currentToken.type = ID; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
printf("Info: Line: %d | Position: %d | 19 \n",
        currentToken.line,
        currentToken.position);
	    return ID;}

{ID}[^0-9a-zA-Z \t\n]+ {currentToken.type = ERROR; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
printf("Info: Line: %d | Position: %d | 20 \n",
        currentToken.line,
        currentToken.position);
	    return ERROR;}

{NUM}     { currentToken.type = NUM; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
printf("Info: Line: %d | Position: %d | 21 \n",
        currentToken.line,
        currentToken.position);
	    return NUM;}

{NUM}{ID} { printf("TOKEN TYPE: NUM ID, Lexeme: %s\n", yytext); char_position += yyleng;}


{NUM}[a-zA-Z!@#$%&-="_]+ { currentToken.type = ERROR; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
printf("Info: Line: %d | Position: %d | 22 \n",
        currentToken.line,
        currentToken.position);
	    return ERROR;}

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
	    return ERROR;}

[0-9]+[eE][+-]?[^0-9]+ {currentToken.type = ERROR; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return ERROR; }

. { currentToken.type = ERROR; 
	    strcpy(currentToken.lexeme, yytext);
            currentToken.line = line_number;
            currentToken.position = char_position;
            char_position += yyleng; 
	    return ERROR; }
%%

