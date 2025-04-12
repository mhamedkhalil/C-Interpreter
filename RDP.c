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

void program() {
    if(lookahead.type == PROGRAM) {
        match(PROGRAM);
    }
}

void match(TokenType expectedType) {
    if (lookahead.type == expectedType) {
        lookahead = yylex();  
    } 
    else {
        syntax_error(&expectedType);
    }
}

void syntax_error(const TokenType *expected) {
    fprintf(stderr, "Syntax error at line %d, pos %d: expected %s but found '%s'\n",
            lookahead.line, lookahead.position, expected, lookahead.lexeme);
    exit(1);
}

int main() 
{

    yylex();
    return 0;
}