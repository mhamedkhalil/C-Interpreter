#ifndef TOKENS_H
#define TOKENS_H

typedef enum {
    // Keywords
    PROGRAM,     // "Program"
    IF,          // "if"
    ELSE,        // "else"
    WHILE,       // "while"
    TYPE,        // "void", "int", "float"
    RETURN,      // return

    // Identifiers and literals
    ID,          // Variable name
    NUM,         // Numeric literal

    // Symbols / Punctuation
    OPEN_PAR,     // (
    CLOSED_PAR,   // )
    SEMICOLON,    // ;
    COMMA,        // ,
    OPEN_CURL,    // {
    CLOSED_CURL,  // }
    OPEN_SQUARE,  // [
    CLOSED_SQUARE,// ]

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
    switch (token) {
    case PROGRAM:      return "PROGRAM";
    case RETURN:       return "RETURN";
    case IF:           return "IF";
    case ELSE:         return "ELSE";
    case WHILE:        return "WHILE";
    case TYPE:         return "TYPE";
    case ID:           return "ID";
    case NUM:          return "NUM";
    case OPEN_PAR:     return "OPEN_PAR";
    case CLOSED_PAR:   return "CLOSED_PAR";
    case OPEN_CURL:    return "OPEN_CURL";
    case CLOSED_CURL:  return "CLOSED_CURL";
    case OPEN_SQUARE:  return "OPEN_SQUARE";
    case CLOSED_SQUARE:  return "CLOSED_SQUARE";
    case SEMICOLON:    return "SEMICOLON";
    case COMMA:        return "COMMA";
    case ASSIGN:       return "ASSIGN";
    case RELOP:        return "RELOP";
    case ADDOP:        return "ADDOP";
    case MULOP:        return "MULOP";
    case END_OF_FILE:  return "EOF";
    default:           return "UNKNOWN_TOKEN";
    }
}

#endif // TOKENS_H