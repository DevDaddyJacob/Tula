#include <stdio.h>
#include <stdlib.h>

#include "cli.h"
#include "conf.h"
#include "common.h"
#include "debug.h"
#include "core/scanner.h"
#include "core/chunk.h"
#include "core/vm.h"
#include "utils/log.h"


int EXIT_CODE = 0;
TulaState TULA_STATE;

static char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");
    if (file == NULL) {
        tula_printfErr("Could not open file \"%s\".\n", path);
        exit(74);
    }
  
    
    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);
  

    char* buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL) {
        tula_printfErr("Not enough memory to read \"%s\".\n", path);
        exit(74);
    }

    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
        tula_printfErr("Could not read file \"%s\".\n", path);
        exit(74);
    }

    buffer[bytesRead] = '\0';
  

    fclose(file);
    return buffer;
}

static void repl() {
    Scanner scanner;
    Token token;

    char line[1024];
    for (;;) {
        printf("> ");

        if (!fgets(line, sizeof(line), stdin)) {
            printf("\n");
            break;
        }

        tula_initScanner(&scanner, line);
        
        do {
            token = tula_scanToken(&scanner);
            tula_printToken(&token);
        } while (token.type != TOKEN_EOF);
    }
}

static void executeFile(const char* path) {
    VM vm;
    tula_initVM(&vm);

    char* source = readFile(path);
    InterpretResult result = tula_interpretSource(&vm, source);
    free(source); 
  
    if (result == INTERPRET_COMPILE_ERROR) exit(65);
    if (result == INTERPRET_RUNTIME_ERROR) exit(70);
}

static void debug() {
    VM vm;
    Chunk chunk;

    tula_initVM(&vm);
    tula_initChunk(&chunk);

    /*
    int constantIndex = tula_addChunkConstant(&chunk, 1.2);
    tula_writeChunk(&chunk, OP_CONSTANT, 123);
    tula_writeChunk(&chunk, constantIndex, 123);
  
    constantIndex = tula_addChunkConstant(&chunk, 3.4);
    tula_writeChunk(&chunk, OP_CONSTANT, 123);
    tula_writeChunk(&chunk, constantIndex, 123);
  
    tula_writeChunk(&chunk, OP_ADD, 123);
  
    constantIndex = tula_addChunkConstant(&chunk, 5.6);
    tula_writeChunk(&chunk, OP_CONSTANT, 123);
    tula_writeChunk(&chunk, constantIndex, 123);
  
    tula_writeChunk(&chunk, OP_DIVIDE, 123);
    tula_writeChunk(&chunk, OP_NEGATE, 123);
    */

    tula_writeChunk(&chunk, OP_RETURN, 123);
    
    tula_interpretChunk(&vm, &chunk);

    // return -((1.2 + 3.4) / 5.6)

    tula_freeChunk(&chunk);
    tula_freeVM(&vm);
}


int main(int argc, const char* argv[]) {
    tula_parseCliArgs(argc, argv, &TULA_STATE);
    printf(
        "TulaState={helpMenu:%d,interactive:%d,helpItem:\"%s\",scriptFile:\"%s\"}\n",
        TULA_STATE.helpMenu,
        TULA_STATE.interactive,
        TULA_STATE.helpItem,
        TULA_STATE.scriptFile
    );

    if (TULA_STATE.interactive) {
        repl();

    } else if (TULA_STATE.scriptFile != NULL) {
        executeFile(TULA_STATE.scriptFile);

    } else if (TULA_STATE.helpMenu) {
        printf("Help menu");

    } else {
        tula_printfErr("Improper usage");
        EXIT_CODE = 1;
    }

    tula_freeState(&TULA_STATE);
    return EXIT_CODE;
}