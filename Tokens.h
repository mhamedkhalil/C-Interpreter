#ifndef TOKENS_H
#define TOKENS_H

typedef enum {
    // Keywords
    PROGRAM,     // "Program"
    IF,          // "if"
    ELSE,        // "else"
    WHILE,       // "while"
    TYPE,        // "void", "int", "float"

    // Identifiers and literals
    ID,          // Variable name
    NUM,         // Numeric literal

    // Symbols / Punctuation
    OPEN_PAR,     // (
    CLOSED_PAR,   // )
    SEMICOLON,    // ;
    COMMA,        // ,

    // Operators
    ASSIGN,       // =
    RELOP,        // >=, >, <, <=, !=
    ADDOP,        // + or -
    MULOP,        // * or /

    // Special tokens
    END_OF_FILE,  // End of file/input
    ERROR         // Invalid/unrecognized token
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[100];
    int line;
    int position;
} Token;

// Token type to string helper function
const char* get_token_type(TokenType token)
{
    switch(token){
        case PROGRAM:      return "PROGRAM";
        case IF:           return "IF";
        case ELSE:         return "ELSE";
        case WHILE:        return "WHILE";
        case TYPE:         return "TYPE";
        case ID:           return "ID";
        case NUM:          return "NUM";
        case OPEN_PAR:     return "OPEN_PAR";
        case CLOSED_PAR:   return "CLOSED_PAR";
        case SEMICOLON:    return "SEMICOLON";
        case COMMA:        return "COMMA";
        case ASSIGN:       return "ASSIGN";
        case RELOP:        return "RELOP";
        case ADDOP:        return "ADDOP";
        case MULOP:        return "MULOP";
        case END_OF_FILE:  return "EOF";
        case ERROR:        return "ERROR";
        default:           return "UNKNOWN_TOKEN";
    }
}

#endif // TOKENS_H
