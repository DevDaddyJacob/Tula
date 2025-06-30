#include <stdio.h>

#include "errors.h"
#include "lexer.h"
#include "../tula.h"
#include "../common.h"


/*
 * ==================================================
 * Macros
 * ==================================================
 */

/* #define XYZ "ABC" */

/*
 * ==================================================
 * Typedefs & Prototypes
 * ==================================================
 */


/*
 * ==================================================
 * Module Level Variables & Constants
 * ==================================================
 */




/*
 * ==================================================
 * Function Definitions
 * ==================================================
 */

TULA_FUNC void tulaErr_raw(
    const char* errType,
    const char* errMsg,
    const char* errStackTrace,
    const char* fileName,
    Int16 fileLineNum,
    const char* fileLine,
    Int16 sourceCharIndex
) {
    fprintf(
        stderr, 
        "* %s Error (%s)\n" \
        "* Located in file \"%s\" at line %d\n",
        PROG_NAME, TULA_RELEASE,
        fileName, fileLineNum
    );


    if (fileLine != NULL) {
        Int16 i = 0;
        char workingLine[TULA_MAX_INPUT] = "";

        /* Walk the file line until we hit the end of the line */
        while (
            fileLine[i] != '\n' 
            && fileLine[i] != '\0' 
            && fileLine[i] != EOF
            && i < TULA_MAX_INPUT
        ) {
            workingLine[i] = fileLine[i];
            i++;
        }
    
        workingLine[i] = '\0';

        fprintf(stderr, "%s\n", workingLine);

        if (sourceCharIndex != -1) {
            fprintf(stderr, "%-*c\n", sourceCharIndex - 1, '^');
        }
        
        fprintf(stderr, "\n");
    }


    fprintf(stderr, "%s: %s\n", errType, errMsg);

    if (errStackTrace != NULL) {
        fprintf(stderr, "%s\n", errStackTrace);
    }

    fprintf(stderr, "\n");
}


TULA_FUNC void tulaErr_parseError(
    Token* token,
    const char* errMsg,
    const char* errType,
    const char* errStackTrace,
    const char* fileName,
    const char* fileLine
) {
    tulaErr_raw(
        (errType != NULL ? errType : "ParseError"),
        errMsg,
        errStackTrace,
        fileName,
        token->line,
        fileLine,
        -1
    );
}