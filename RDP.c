#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Tokens.h"

Token lookahead;
extern Token yylex();
extern int line_number;   // to track line numbers
extern int char_position; // to track character position
extern FILE *yyin;

// Functions Prototyping
void program(); //
void declaration_list(); //
void statement_list(); //
void var_declaration(); //
void var_declaration_tail(); //
void type_specifier(); //
void params(); //
void param_list(); //
void param_list_tail(); //
void param(); //
void param_tail(); //
void compound_stmt(); //
void statement(); //
void assignment_stmt();
void selection_stmt();
void iteration_stmt();
void expression();
void var();
void relop();
void additive_expression();
void addop();
void term();
void mulop();
void factor();
void match(TokenType expectedType);
void syntax_error(const TokenType expected);

void program() {
    if(lookahead.type == PROGRAM) {
        match(PROGRAM);
        match(ID);
        match(OPEN_CURL);
        declaration_list();
        statement_list();
        match(CLOSED_CURL);
    }
    else {
        syntax_error(PROGRAM);
    }
}

void declaration_list()
{
    while(lookahead.type == TYPE)
    {
        var_declaration();
    }
}

void var_declaration()
{
    type_specifier();
    match(ID);
    var_declaration_tail();   
}

void var_declaration_tail()
{
    switch(lookahead.type) {
        case SEMICOLON: match(SEMICOLON); break;
        case OPEN_SQUARE: match(OPEN_SQUARE); match(NUM); match(CLOSED_SQUARE); match(SEMICOLON); break;
        default: syntax_error(SEMICOLON);
    }
}

void type_specifier()
{
    if(lookahead.type == TYPE)
    {
        if(lookahead.lexeme == "int" || lookahead.lexeme == "float")
            match(TYPE);
        else 
            syntax_error(TYPE);
    }
    else 
        syntax_error(TYPE);
}

void params()
{
    if(lookahead.lexeme == "void")
        match(TYPE);
    else 
        param_list();
}

void param_list()
{
    param();
    param_list_tail();
}

void param_list_tail()
{
    while(lookahead.type == COMMA)
    {
        match(COMMA);
        param();
    }
}

void param()
{
    type_specifier();
    match(ID);
    param_tail();
}

void param_tail()
{
    if(lookahead.type == OPEN_SQUARE)
    {
        match(OPEN_CURL);
        match(CLOSED_CURL);
    }
}

void compound_stmt()
{
    match(OPEN_CURL);
    statement_list();
    match(CLOSED_CURL);
}

void statement_list()
{
   while(lookahead.type == ID || lookahead.lexeme == "{" 
    || lookahead.type == IF || lookahead.type == WHILE)
    {
        statement();
    }     
}

void statement()
{

}

void match(TokenType expectedType) {
    if (lookahead.type == expectedType) {
        lookahead = yylex();  
    } 
    else {
        syntax_error(expectedType);
    }
}

void syntax_error(const TokenType expected) {
    fprintf(stderr, "Syntax error at line %d, pos %d: expected %s but found '%s'\n",
            lookahead.line, lookahead.position, get_token_type(expected), get_token_type(lookahead.type));
    exit(1);
}

int main() 
{
    lookahead = yylex();
    return 0;
}