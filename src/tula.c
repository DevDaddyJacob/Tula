#include <stdio.h>
#include <stdlib.h>

#include "tula.h"
#include "common.h"
#include "debug.h"
#include "core/parser.h"
#include "core/state.h"
#include "utils/cli.h"
#include "utils/scriptFile.h"

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

static void setup();

static void teardown(int exitCode, const char* exitMessage, Bool isFatal);



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
        tula_exitFatal(TULA_EXIT_NO_MEM, "Failed to allocate memory");
        return NULL; /* Unreachable */
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


static void setup(int argc, const char* argv[]) {
    tulaState_setup(argc, argv);
}


static void teardown(int exitCode, const char* exitMessage, Bool isFatal) {
    tulaState_teardown();

    if (exitMessage != NULL) {
        if (isFatal) tula_errPrintFatal(exitMessage);
        else tula_errPrint(exitMessage);
    }

    exit(exitCode);
}


void tula_exit(int exitCode) {
    teardown(exitCode, NULL, FALSE);
}


void tula_exitError(int exitCode, const char* errorMessage) {
    teardown(exitCode, errorMessage, FALSE);
}


void tula_exitFatal(int exitCode, const char* errorMessage) {
    teardown(exitCode, errorMessage, TRUE);
}


int main(int argc, const char* argv[]) {
    GlobalState* gState = NULL;

    /* Run setup operations */
    setup(argc, argv);
    gState = tulaState_getGlobal();

    
    /* Determine what we are running based on cli args */
    if (gState->cli->interactive == TRUE) {
        repl_run();
        tula_exit(TULA_EXIT_GODD);

        return -1; /* Unreachable */
    }


    /* Run the file */
    if (gState->cli->file != NULL) {
        Bool parsed;
        
        /* Create the script file */
        gState->script = tulaSrc_new(gState->cli->file);
        if (gState->script == NULL) {
            tula_exitFatal(TULA_EXIT_NO_MEM, "Failed to allocate memory");
            return -1; /* Unreachable */
        }


        /* Parse the script file */
        gState->scriptChunk = tulaChk_new();
        parsed = tula_parseSource(gState->script->content, gState->scriptChunk);
        
        printf("Parsed: %d\n", parsed);
        debug_disassembleChunk(gState->scriptChunk, "code");
    }


    tula_exit(TULA_EXIT_GODD);
    return -1; /* Unreachable */
}