#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <string.h>
#include "Tokens.h"
#include "lex.yy.c"
#include "symbolTable.c"

Token lookahead;
extern int yylex();
extern Token currentToken;
extern FILE *yyin;
int state;
int is_executing = 1; // Global flag to control symbol table modification

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
    Symbol *sym = malloc(sizeof(Symbol));
    if (!sym) {
        fprintf(stderr, "Memory allocation failed for Symbol\n");
        exit(1);
    }
    sym->Line = lookahead.line;
    type_specifier(sym);

    if (lookahead.type == ID) {
        sym->Name = strdup(lookahead.lexeme); 
    } else {
        syntax_error(ID);
    }
    match(ID);

    switch (lookahead.type) {
        case SEMICOLON: {
            match(SEMICOLON);
            if (!Insert_Symbol(sym)) {
                free(sym->Name);
                free(sym);
                semantic_error("Symbol already exists");
            }
            break;
        }
        case OPEN_SQUARE: {
            match(OPEN_SQUARE);
            int allocationSize = 0;

            if (lookahead.type == NUM) {
                char *endptr;
                allocationSize = strtol(lookahead.lexeme, &endptr, 10);
                if (*endptr != '\0' || allocationSize <= 0) {
                    free(sym->Name);
                    free(sym);
                    semantic_error("Invalid array size");
                }
            } else {
                free(sym->Name);
                free(sym);
                syntax_error(NUM);
            }
            match(NUM);
            match(CLOSED_SQUARE);
            match(SEMICOLON);

            // Declare array entries a[0], a[1], ..., a[n-1]
            for (int i = 0; i < allocationSize; i++) {
                Symbol *element = malloc(sizeof(Symbol));
                if (!element) {
                    semantic_error("Memory allocation failed for array element");
                }

                // Allocate name: "a[0]" etc.
                int name_len = strlen(sym->Name) + 12;
                element->Name = malloc(name_len);
                if (!element->Name) {
                    free(element);
                    semantic_error("Memory allocation failed for array element name");
                }

                snprintf(element->Name, name_len, "%s[%d]", sym->Name, i);
                element->Type = sym->Type;
                element->Line = sym->Line;
                element->value.int_val = 0;  // default initialization
                printf("Inserting: %s\n", element->Name);
                if (!Insert_Symbol(element)) {
                    free(element->Name);
                    free(element);
                    free(sym->Name);
                    free(sym);
                    semantic_error("Duplicate array element in symbol table");
                }
            }

            // Clean up base symbol after inserting array elements
            free(sym->Name);
            free(sym);
            break;
        }

        default:
            free(sym->Name);
            free(sym);
            syntax_error(SEMICOLON);
    }
}


void type_specifier(Symbol* sym)
{
    if(lookahead.type == TYPE)
    {
        if(strcmp(lookahead.lexeme, "int") == 0)
        {
            match(TYPE);
            sym->Type = INT_TYPE;
        }
        else if(strcmp(lookahead.lexeme, "float") == 0)
        {
            match(TYPE);            
            sym->Type = FLOAT_TYPE;
        }
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
    Symbol* sym = malloc(sizeof(Symbol));
    if (!sym) {
        fprintf(stderr, "Memory allocation failed for Symbol in param()\n");
        exit(1);
    }
    type_specifier(sym);
    match(ID);
    free(sym); // Free the allocated memory after use
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
    char* name = malloc(100);
    if (!name) {
        fprintf(stderr, "Memory allocation failed for assignment statement\n");
        exit(1);
    }
    SYMBOL_TYPE type;
    var(&type, name);
    match(ASSIGN);
    SYMBOL_TYPE expType;
    union {
        int int_val;
        float float_val;
    } expValue;
    expression(&expType, &expValue);
    if(type != expType)
    {
        free(name);
        semantic_error("Type mismatch in assignment");
    }

    if (is_executing) {
        Symbol* sym = Symbol_Exists(name);
        if(type == INT_TYPE)
            sym->value.int_val = expValue.int_val;
        else if(type == FLOAT_TYPE)
            sym->value.float_val = expValue.float_val;
    }

    free(name);
    match(SEMICOLON);
}


void selection_stmt()
{
    match(IF); 
    match(OPEN_PAR);
    SYMBOL_TYPE expType;
    int condition = 0;
    expression(&expType, &condition);
    if(expType != INT_TYPE)
    {
        semantic_error("Condition must result in a boolean");
    }
    match(CLOSED_PAR);

    int prev_exec = is_executing;

    if (condition) {
        is_executing = prev_exec;     
        statement();
        if (lookahead.type == ELSE) {
            match(ELSE);
            is_executing = 0;        
            statement();
        }
    } else {
        is_executing = 0;             
        statement();
        is_executing = prev_exec;     
        if (lookahead.type == ELSE) {
            match(ELSE);
            is_executing = prev_exec;
            statement();
        }
    }

    is_executing = prev_exec;
}

void iteration_stmt()
{
    match(WHILE);
    match(OPEN_PAR);
    SYMBOL_TYPE condType;
    int condValue = 0;
    expression(&condType, &condValue);
    if (condType != INT_TYPE) {
        semantic_error("While condition must result in boolean");
    }
    match(CLOSED_PAR);

    int prev_exec = is_executing;

    while (condValue) {
        is_executing = prev_exec;
        statement();

        match(WHILE);            
        match(OPEN_PAR);
        expression(&condType, &condValue);
        if (condType != INT_TYPE) {
            semantic_error("While condition must result in boolean");
        }
        match(CLOSED_PAR);
    }

    // After loop: parse once more without executing
    is_executing = 0;
    statement();

    is_executing = prev_exec; // restore
}


void var(SYMBOL_TYPE* type, char* name)
{
    strcpy(name, lookahead.lexeme);
    match(ID);
    if(lookahead.type == OPEN_SQUARE)
    {
        match(OPEN_SQUARE);
        SYMBOL_TYPE expType;
        int arrayIndex;
        expression(&expType, &arrayIndex);
        match(CLOSED_SQUARE);
        if(expType != INT_TYPE)
        {
            free(name);
            semantic_error("Array index must be an integer");
        }
        char indexedName[256];
        snprintf(indexedName, sizeof(indexedName), "%s[%d]", name, arrayIndex);
        Symbol* arrayElement = Symbol_Exists(indexedName);
        if(arrayElement == NULL)
        {
            free(name);
            semantic_error("Array element not declared");
        }
        *type = arrayElement->Type;
        *name = strdup(indexedName);
    }
    else 
    {
        Symbol *sym = Symbol_Exists(name);
        if(sym == NULL)
        {
            free(name);
            semantic_error("Variable not declared");
            *type = sym->Type;
        }
        *type = sym->Type;
        *name = strdup(sym->Name);
    }
}

void expression(SYMBOL_TYPE* expType, void* value)
{
    SYMBOL_TYPE leftType;
    union {
        int int_val;
        float float_val;
    } leftValue;
    additive_expression(&leftType, &leftValue);
    if(lookahead.type == RELOP)
    {
        *expType = INT_TYPE;
        char* relop = strdup(lookahead.lexeme);
        match(RELOP);
        SYMBOL_TYPE rightType;
        union {
            int int_val;
            float float_val;
        } rightValue;
        additive_expression(&rightType, &rightValue);
        if(leftType != rightType)
        {
            semantic_error("Type mismatch in expression");
        }
        if(leftType == INT_TYPE)
        {
            if(strcmp(relop, "<") == 0)
                *((int *)value) = leftValue.int_val < rightValue.int_val;
            else if(strcmp(relop, "<=") == 0)
                *((int *)value) = leftValue.int_val <= rightValue.int_val;
            else if(strcmp(relop, ">") == 0)
                *((int *)value) = leftValue.int_val > rightValue.int_val;
            else if(strcmp(relop, ">=") == 0)
                *((int *)value) = leftValue.int_val >= rightValue.int_val;
            else if(strcmp(relop, "==") == 0)
                *((int *)value) = leftValue.int_val == rightValue.int_val;
            else if(strcmp(relop, "!=") == 0)
                *((int *)value) = leftValue.int_val != rightValue.int_val;
        }
        else if(leftType == FLOAT_TYPE)
        {
            if(strcmp(relop, "<") == 0)
                *((int *)value) = leftValue.float_val < rightValue.float_val;
            else if(strcmp(relop, "<=") == 0)
                *((int *)value) = leftValue.float_val <= rightValue.float_val;
            else if(strcmp(relop, ">") == 0)
                *((int *)value) = leftValue.float_val > rightValue.float_val;
            else if(strcmp(relop, ">=") == 0)
                *((int *)value) = leftValue.float_val >= rightValue.float_val;
            else if(strcmp(relop, "==") == 0)
                *((int *)value) = leftValue.float_val == rightValue.float_val;
            else if(strcmp(relop, "!=") == 0)
                *((int *)value) = leftValue.float_val != rightValue.float_val;
        }    
    }
    else
    {
        *expType = leftType;
        if(leftType == INT_TYPE)
            *((int *)value) = leftValue.int_val;
        else if(leftType == FLOAT_TYPE)
            *((float *)value) = leftValue.float_val;
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
        state = yylex();
        lookahead = currentToken;  
    } 
    else {
        syntax_error(expectedType);
    }
}

void syntax_error(const TokenType expected) {
    printf("Lookahead Token:%s\n",lookahead.lexeme);
    fprintf(stderr, "Syntax error at line %d, pos %d: expected %s but found '%s'\n",
            lookahead.line, lookahead.position, get_token_type(expected), get_token_type(lookahead.type));
    exit(1);
}

void semantic_error(const char *message) {
    fprintf(stderr, "Semantic error at line %d, pos %d: %s\n",
            lookahead.line, lookahead.position, message);
    Print_Symbol_Table();
    exit(1);
}

int main(int argc, char **argv) 
{
    if (argc > 1) {
        FILE *infile = fopen(argv[1], "r");
        if (!infile) {
            perror("Error opening file");
            return 1;
        }
        yyin = infile;
    }
    state = yylex();
    lookahead = currentToken;
    program();
    printf("State:%d\n",state);
    if(state == 0)
        printf("Parsing was successful! Lookahead: %s\n",lookahead.lexeme);
    Print_Symbol_Table();
    return 0;
}