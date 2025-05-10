#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TABLE_SIZE 100

typedef enum { INT_TYPE, FLOAT_TYPE, VOID_TYPE } SYMBOL_TYPE;

typedef struct {
    char *Name;
    SYMBOL_TYPE Type;
    union {
        int int_val;
        float float_val;
    } value;
    int Line;
} Symbol;

Symbol *symbol_table[TABLE_SIZE];
int table_count = 0;

Symbol* Symbol_Exists(char* symbol)
{
    for (int i = 0; i < table_count; i++)
    {
        if (symbol_table[i] && strcmp(symbol_table[i]->Name, symbol) == 0)
            return symbol_table[i];
    }
    return NULL;
}

bool Insert_Symbol(Symbol* symbol)
{
    if (Symbol_Exists(symbol->Name) != NULL)
        return false;

    if (table_count >= TABLE_SIZE) {
        fprintf(stderr, "Symbol table overflow\n");
        return false;
    }

    symbol_table[table_count++] = symbol;
    return true;
}

bool Update_Table(Symbol* symbol)
{
    Symbol* current = Symbol_Exists(symbol->Name);
    if (current == NULL)
        return false;

    current->Type = symbol->Type;
    current->value = symbol->value;
    current->Line = symbol->Line;
    return true;
}

void Print_Symbol_Table()
{
    printf("Symbol Table:\n");
    for (int i = 0; i < table_count; i++)
    {
        if (symbol_table[i] && symbol_table[i]->Name) {
            printf("Name: %s, Type: %d, Line: %d\n",
                   symbol_table[i]->Name,
                   symbol_table[i]->Type,
                   symbol_table[i]->Line);
        }
    }
}
