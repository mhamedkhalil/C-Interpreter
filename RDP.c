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
void type_specifier(); //
void params(); //
void param_list(); //
void param(); //
void compound_stmt(); //
void statement(); //
void assignment_stmt(); //
void selection_stmt(); //
void iteration_stmt(); //
void expression(); //
void var(); //
void additive_expression(); //
void term(); //
void factor(); //
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
    if(lookahead.type == OPEN_SQUARE)
    {
        match(OPEN_SQUARE);
        match(CLOSED_SQUARE);
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
    switch(lookahead.type) {
        case ID: assignment_stmt(); break;
        case OPEN_CURL: compound_stmt(); break;
        case IF: selection_stmt(); break;
        case WHILE: iteration_stmt(); break;
        default: syntax_error(ID);
    }
}

void assignment_stmt()
{
    var();
    match(ASSIGN);
    expression();
    match(SEMICOLON);
}

void selection_stmt()
{
    match(IF); 
    match(OPEN_PAR);
    expression();
    match(CLOSED_PAR);
    statement();

    if(lookahead.type == ELSE)
    {
        match(ELSE);
        statement();
    }
}

void iteration_stmt()
{
    match(WHILE);
    match(OPEN_PAR);
    expression();
    match(CLOSED_PAR);
    statement();
}

void var()
{
    match(ID);
    if(lookahead.type == OPEN_SQUARE)
    {
        match(OPEN_SQUARE);
        expression();
        match(CLOSED_SQUARE);
    }
}

void expression()
{
    additive_expression();
    while(lookahead.type == RELOP)
    {
        match(RELOP);
        additive_expression();
    }
}

void additive_expression()
{
    term();
    while(lookahead.type == ADDOP)
    {
        match(ADDOP);
        term();
    }
}

void term()
{
    factor();
    while(lookahead.type == MULOP)
    {
        match(MULOP);
        factor();
    }
}

void factor()
{
    switch(lookahead.type) {
        case OPEN_PAR: match(OPEN_PAR); expression(); match(CLOSED_PAR); break;
        case ID: var(); break;
        case NUM: match(NUM); break;
        default: syntax_error(ID);
    }
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