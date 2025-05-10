%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Tokens.h"

int line_number = 1;
int char_position = 1;

Token currentToken;

void make_token(TokenType type) {
    currentToken.type = type;
    strcpy(currentToken.lexeme, yytext);
    currentToken.line = line_number;
    currentToken.position = char_position;
    char_position += yyleng;
    printf("TOKEN: %-14s | Line: %d | Pos: %d | Lexeme: %s\n", 
        get_token_type(type), currentToken.line, currentToken.position, currentToken.lexeme);
}
%}

%x COMMENT_STATE
%option noyywrap

DIGIT  [0-9]
LETTER [a-zA-Z]
ID     {LETTER}({LETTER}|{DIGIT})*(("@"|"$"|"_")?{DIGIT}+)?
NUM    {DIGIT}+(\.{DIGIT}+)?([Ee][+-]?{DIGIT}+)?
WS     [ \t\r]+
NEWLINE \n

%%

"Program"           { make_token(PROGRAM); return PROGRAM; }
"if"                { make_token(IF); return IF; }
"else"              { make_token(ELSE); return ELSE; }
"while"             { make_token(WHILE); return WHILE; }
"return"            { make_token(RETURN); return RETURN; }
"int"|"float"|"void" { make_token(TYPE); return TYPE; }

[+\-]               { make_token(ADDOP); return ADDOP; }
[*/]                { make_token(MULOP); return MULOP; }
==|!=|<=|>=|>|<     { make_token(RELOP); return RELOP; }
=                   { make_token(ASSIGN); return ASSIGN; }

;                   { make_token(SEMICOLON); return SEMICOLON; }
,                   { make_token(COMMA); return COMMA; }
"("                 { make_token(OPEN_PAR); return OPEN_PAR; }
")"                 { make_token(CLOSED_PAR); return CLOSED_PAR; }
"["                 { make_token(OPEN_SQUARE); return OPEN_SQUARE; }
"]"                 { make_token(CLOSED_SQUARE); return CLOSED_SQUARE; }
"{"                 { make_token(OPEN_CURL); return OPEN_CURL; }
"}"                 { make_token(CLOSED_CURL); return CLOSED_CURL; }
{WS}                { char_position += yyleng; }
{NEWLINE}           { line_number++; char_position = 1; }
{ID}                { make_token(ID); return ID; }


{NUM}               { make_token(NUM); return NUM; }
{NUM}[a-zA-Z_]+ { make_token(ERROR); return ERROR; }

"/*"                { BEGIN(COMMENT_STATE); }
<COMMENT_STATE>{NEWLINE}    { line_number++; char_position = 1; }
<COMMENT_STATE>[^*\n]*      { }
<COMMENT_STATE>"*"+[^*/\n]  { }
<COMMENT_STATE>"*/"         { BEGIN(INITIAL); }
<COMMENT_STATE><<EOF>>      { 
    fprintf(stderr, "ERROR: Unclosed comment at line %d, pos %d\n", line_number, char_position);
    exit(1); 
}

[a-zA-Z]#([^a-zA-Z0-9]*)    { make_token(ERROR); return ERROR; }
[0-9]+[eE][+-]?[^0-9]+      { make_token(ERROR); return ERROR; }
.                           { make_token(ERROR); return ERROR; }

%%
