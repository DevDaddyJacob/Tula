#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* pChunk) {
    pChunk->count = 0;
    pChunk->capacity = 0;
    pChunk->pCode = NULL;
    initValueArray(&pChunk->constants);
}

void writeChunk(Chunk* pChunk, uint8_t byte) {
    // Check if we need to expand the capacity of the array
    if (pChunk->capacity < pChunk->count + 1) {
        int oldCapacity = pChunk->capacity;
        pChunk->capacity = GROW_CAPACITY(oldCapacity);
        pChunk->pCode = GROW_ARRAY(uint8_t, pChunk->pCode, oldCapacity, pChunk->capacity);
    }

    pChunk->pCode[pChunk->count] = byte;
    pChunk->count++;
}

void freeChunk(Chunk* pChunk) {
    FREE_ARRAY(uint8_t, pChunk->pCode, pChunk->capacity);
    freeValueArray(&pChunk->constants);
    initChunk(pChunk);
}

int addConstant(Chunk* pChunk, Value value) {
    writeValueArray(&pChunk->constants, value);
    return pChunk->constants.count - 1;
}
