#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main() 
{
    token_file = fopen("tokens.txt", "w");
    if (token_file == NULL) {
        fprintf(stderr, "Could not open tokens.txt for writing\n");
        exit(1);
    }
    yylex();
    fclose(token_file);
    return 0;
}