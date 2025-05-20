#include <stdio.h>

#include "compiler.h"
#include "scanner.h"

BOOL compile(const char* source, Chunk* chunk) {
    /* Initialize the scanner */
    Scanner scanner;
    initScanner(&scanner, source);


    /* Walk the scanner */
    int line = -1;
    for (;;) {
        Token token = scanToken(&scanner);
        if (token.line != line) {
          printf("%4d ", token.line);
          line = token.line;
        } else {
          printf("   | ");
        }
        printf("%2d '%.*s'\n", token.type, token.length, token.start); 
    
        if (token.type == TOKEN_EOF) break;
    }
}
