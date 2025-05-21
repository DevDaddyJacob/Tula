#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "log.h"


/*
 * ==================================================
 * Module Level Variables & Constants
 * ==================================================
 */

extern char* PROGRAM_NAME;


/*
 * ==================================================
 * Function Definitions
 * ==================================================
 */

void tula_rawPrintfErr(const char* const format, ...) {
    // Handle variadic arguments
    va_list args;
    va_start(args, format);
    vfprintf(stderr, format, args);
    va_end(args);
}


void tula_putsErr(const char* message) {
    fputs(message, stderr);
}


void tula_printfErr(const char* const format, ...) {
    /* Calculate length for the new format string */
    size_t newFormatLen = strlen(PROGRAM_NAME) + strlen(format) + 10;
    char* newFormat = malloc(newFormatLen);

    /* Create the new format string */
    snprintf(newFormat, newFormatLen, "%s: Error: %s", PROGRAM_NAME, format);

    // Handle variadic arguments
    va_list args;
    va_start(args, format);
    vfprintf(stderr, newFormat, args);
    va_end(args);

    free(newFormat);
}
