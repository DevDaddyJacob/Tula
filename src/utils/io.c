#include <stdio.h>
#include <stdlib.h>

#include "io.h"
#include "uint64.h"
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

/**
 * TBD
 */
/* static void example(); */


/*
 * ==================================================
 * Module Level Variables & Constants
 * ==================================================
 */

/* int magicNumber = 420; */


/*
 * ==================================================
 * Function Definitions
 * ==================================================
 */

TULA_FUNC BufferReader* tulaIo_openBuffer(const char* data) {
    BufferReader* buffer = NULL;

    /* Allocate memory for the buffer */
    buffer = (BufferReader*)malloc(sizeof(BufferReader));
    if (buffer == NULL) return NULL;

    buffer->next = (char*)malloc(sizeof(char) * strlen(data) + 1);
    if (buffer->next == NULL) {
        tulaIo_closeBuffer(buffer);
        return NULL;
    }

    memcpy(buffer->next, data, strlen(data) + 1);


    /* Initialize the other variables */
    buffer->start = buffer->next;
    buffer->size = strlen(data) + 1;

    return buffer;
}


TULA_FUNC void tulaIo_closeBuffer(BufferReader* buffer) {
    if (buffer == NULL) return;

    /* Free the buffer's data first */
    if (buffer->next != NULL) {
        free(buffer->next);
    }


    /* Free the struct next */
    free(buffer);
}

