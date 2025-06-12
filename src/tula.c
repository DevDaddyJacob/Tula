#include <stdio.h>
#include <stdlib.h>

#include "tula.h"
#include "common.h"
#include "core/lexer.h"
#include "utils/cli.h"
#include "utils/io.h"

/*
 * ==================================================
 * Macros
 * ==================================================
 */

#if !defined(TULA_PROGRAM_NAME)
    #define TULA_PROGRAM_NAME "tula"
#endif


/*
 * ==================================================
 * Typedefs & Prototypes
 * ==================================================
 */

static void repl_run();

static char* repl_read();

static char* repl_eval(char* line);

static void repl_print(char* output);


/*
 * ==================================================
 * Module Level Variables & Constants
 * ==================================================
 */

const char* PROG_NAME = TULA_PROGRAM_NAME;


/*
 * ==================================================
 * Function Definitions
 * ==================================================
 */

static void repl_run() {
    char* line;

    /* Initialize variables */
    line = NULL;

    
    /* Run loop */
    for (;;) {
        /* Read */
        line = repl_read();
        if (line == NULL) continue;


        /* Eval */
        line = repl_eval(line);


        /* Print */
        repl_print(line);

        free(line);
        line = NULL;
    }
}

static char* repl_read() {
    int readSuccessful, lineLength;
    char* buffer;
    
    /* Allocate the buffer */
    buffer = (char*)malloc(sizeof(char) * TULA_MAX_INPUT);
    if (buffer == NULL) {
        tula_errPrintFatal("Failed to allocate memory");
        exit(TULA_EXIT_NO_MEM);
    }


    /* Prompt for and scan input */
    fputs("> ", stdout);
    fflush(stdout);
    readSuccessful = fgets(buffer, TULA_MAX_INPUT, stdin) != NULL;
    if (readSuccessful == FALSE) {
        free(buffer);
        return NULL;
    }


    /* Ensure the string has a line terminator */
    lineLength = strlen(buffer);
    if (lineLength > 0 && buffer[lineLength - 1] == '\n') {
        buffer[--lineLength] = '\0';
    }

    return buffer;
}

static char* repl_eval(char* line) {
    return line;
}

static void repl_print(char* output) {
    fputs(output, stdout);
    fputs("\n", stdout);
    fflush(stdout);
}

static void test(const char* path) {
    int i;
    FILE* file;
    size_t fileSize;
    char* buffer;
    size_t bytesRead;
    Lexer* lexer;
    Token token;


    file = fopen(path, "rb");
    if (file == NULL) {
        tula_errPrintFatal("Could not open file.");
        exit(74);
    }
  
    
    fseek(file, 0L, SEEK_END);
    fileSize = ftell(file);
    rewind(file);
  

    buffer = (char*)malloc(fileSize + 1);
    if (buffer == NULL) {
        tula_errPrintFatal("Not enough memory to read");
        exit(74);
    }

    bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (bytesRead < fileSize) {
        tula_errPrintFatal("Could not read file");
        exit(74);
    }

    buffer[bytesRead] = '\0';
  
    fclose(file);

    lexer = tulaLex_new(buffer);
    do {
        token = tulaLex_nextToken(lexer);
        printf(
            "Token{type:%d, length:%d, line:%d, start[@ %p]:\"",
            token.type,
            token.length,
            token.line,
            token.start
        );

        for (i = 0; i < token.length; i++) {
            printf("%c", token.start[i]);
        }

        printf("\"}\n");
    } while (token.type != TOK_ERROR && token.type != TOK_EOF);
}

int main(int argc, const char* argv[]) {
    CliConfig* cli;

    /* Initialize variables */
    cli = NULL;


    /* Parse the command line arguments */
    cli = tula_parseCliArgs(argc, argv);
    if (cli == NULL) {
        tula_errPrintFatal("Failed to allocate memory");
        exit(TULA_EXIT_NO_MEM);
    }

    test("./tests/debug.tula");

    /* Determine what we are running based on cli args */
    if (cli->interactive == TRUE) {
        repl_run();
    }

    return TULA_EXIT_GODD;
}