#include <stdio.h>
#include <stdlib.h>

#include "tula.h"
#include "common.h"
#include "cli.h"

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


    /* Determine what we are running based on cli args */
    if (cli->interactive == TRUE) {
        repl_run();
    }

    return TULA_EXIT_GODD;
}