#include <stdlib.h>

#include "chunk.h"
#include "lexer.h"
#include "value.h"
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

TULA_FUNC Chunk* tulaChk_new() {
    Chunk* chunk = NULL;

    /* Allocate memory for the chunk */
    chunk = (Chunk*)malloc(sizeof(Chunk));
    if (chunk == NULL) return NULL;


    /* Zero the values */
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;


    /* Initialize the constant array */
    tula_initValueArray(&chunk->constants);

    return chunk;
}


TULA_FUNC void tulaChk_destroy(Chunk* chunk) {
    /* Deallocate the bytecode and lines arrays */
    TULA_ARRAY_FREE(Byte, chunk->code, chunk->capacity);
    TULA_ARRAY_FREE(Int16, chunk->lines, chunk->capacity);

    free(chunk);
}


TULA_FUNC void tulaChk_write(Chunk* chunk, Byte byte, Int16 line) {
    /* Check if we need to expand the array size */
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = TULA_ARRAY_GROW_CAPACITY(oldCapacity);
        chunk->code = TULA_ARRAY_RESIZE(
            Byte, 
            chunk->code, 
            oldCapacity, 
            chunk->capacity
        );
        chunk->lines = TULA_ARRAY_RESIZE(
            Int16, 
            chunk->lines, 
            oldCapacity, 
            chunk->capacity
        );
    }

    /* Add the byte to the end of the array */
    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}


TULA_FUNC Int16 tulaChk_addConst(Chunk* chunk, Value value) {
    tula_writeValueArray(&(chunk->constants), value);
    return chunk->constants.count - 1;
}
