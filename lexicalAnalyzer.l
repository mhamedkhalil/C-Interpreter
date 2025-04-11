%{
// Keywords: if, else, int, return, void, while
// Special Symbols: + - * / < <= > >= == != = ; , ( ) [ ] { } /* */
// ID = letter (letter | digit)* ((“@”|”$” | “_”)? Digit+)?
// NUM = digit (“.”)? digit+((E |e) (+|-)? digit+)?
// letter = a|...|z|A|...|Z
// digit = 0|...|9
// Case sensitive 
// whitespaces should be ignored and seperate tokens (include \n \t and blank)  

#include <stdio.h>
#include <stdlib.h>

int line_number = 1;   // to track line numbers
int char_position = 1; // to track character position

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
"Program" { return PROGRAM; }
"else"    { return KEYWORD; char_position += yyleng;}
"if"      { return KEYWORD; char_position += yyleng;}
"int"     { return TYPE; char_position += yyleng;}
"float"   { return TYPE; char_position += yyleng;}
"void"    { return TYPE; char_position += yyleng;}
"return"  { return KEYWORD; char_position += yyleng;}
"while"   { return KEYWORD; char_position += yyleng;}

[+\-*/<>]|==|!=|=|<=|>=  { fprintf(token_file,"TOKEN TYPE: OPERATOR, Lexeme: %s\n", yytext); char_position += yyleng;}
[;,\(\)\[\]{}] { fprintf(token_file,"TOKEN TYPE: SYMBOL, Lexeme: %s\n", yytext); char_position += yyleng;}

{ID}      { fprintf(token_file,"TOKEN TYPE: ID, Lexeme: %s\n", yytext); char_position += yyleng;}
{ID}[^0-9a-zA-Z \t\n]+ {printf("TOKEN TYPE: Wrong ID, Lexeme: %s at line %d, position %d\n", yytext, line_number, char_position); char_position += yyleng;}

{NUM}     { fprintf(token_file,"TOKEN TYPE: NUM, Lexeme: %s\n", yytext); char_position += yyleng;}
{NUM}{ID} { fprintf(token_file,"TOKEN TYPE: NUM ID, Lexeme: %s\n", yytext); char_position += yyleng;}
{NUM}[a-zA-Z!@#$%&-="_]+ { printf("TOKEN TYPE: Wrong NUM, Lexeme: %s at line %d, position %d\n", yytext, line_number, char_position); char_position += yyleng;}

{WS}      { char_position += yyleng; char_position += yyleng; } 
{NEWLINE} { line_number++; char_position = 1; } 

"/*"                        { BEGIN(COMMENT_STATE); }
<COMMENT_STATE>{NEWLINE}    { line_number++; char_position=1; }
<COMMENT_STATE>[^*\n]*        { } 
<COMMENT_STATE>"*"+[^*/\n]    { } 
<COMMENT_STATE>"*/"         { BEGIN(INITIAL); }
<COMMENT_STATE><<EOF>>      { printf("ERROR: Unclosed comment at line %d, position %d\n", line_number, char_position); exit(1); }

[a-zA-Z]#([^a-zA-Z0-9]*) { printf("ERROR: Invalid identifier '%s' at line %d, position %d\n", yytext, line_number, char_position); char_position += yyleng; }
[0-9]+[eE][+-]?[^0-9]+ { printf("ERROR: Invalid number format '%s' at line %d, position %d\n", yytext, line_number, char_position); char_position += yyleng; }
. { printf("ERROR: Unknown character '%s' at line %d, position %d\n", yytext, line_number, char_position); char_position += yyleng; }
%%

