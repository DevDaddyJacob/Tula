#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#include "log.h"


extern char* PROGRAM_NAME;

void printfErr(const char *const format, ...) {
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
