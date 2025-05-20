#include "chunk.h"
#include "../conf.h"
#include "../utils/arrays.h"


/*
 * ==================================================
 * Function Definitions
 * ==================================================
 */

void tula_initChunk(Chunk* chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    tula_initValueArray(&(chunk->constants));
}


void tula_freeChunk(Chunk* chunk) {
    /* Deallocate the bytecode and lines arrays */
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(int, chunk->lines, chunk->capacity);
    
    /* Deallocate the constants array too */
    tula_freeValueArray(&(chunk->constants));

    /* Zero out the fields */
    tula_initChunk(chunk);
}


void tula_writeChunk(Chunk* chunk, uint8_t byte, int line) {
    /* Check if we need to expand the array size */
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_ARRAY_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(
            uint8_t, 
            chunk->code, 
            oldCapacity, 
            chunk->capacity
        );
        chunk->lines = GROW_ARRAY(
            int, 
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


int tula_addChunkConstant(Chunk* chunk, Value value) {
    tula_writeValueArray(&(chunk->constants), value);
    return chunk->constants.count - 1;
}
