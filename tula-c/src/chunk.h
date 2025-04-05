#ifndef tulac_chuck_h
#define tulac_chuck_h

#include "common.h"
#include "value.h"

/**
 * @brief Operation codes
 */
typedef enum {
    OP_CONSTANT,
    OP_NEGATE,
    OP_RETURN,
} OpCode;

/**
 * @brief Stores a series of bytecode instructions
 */
typedef struct {
    int count;
    int capacity;
    uint8_t* pCode;
    int* pLines;
    ValueArray constants;
} Chunk;

/**
 * @brief Initializes a new chunk
 * 
 * @param pChunk Pointer to the chunk to initialize
 */
void initChunk(Chunk* pChunk);

/**
 * @brief Used to append a byte to the end of a chunk.
 * 
 * @param pChunk Pointer to the chunk to add the byte to
 * @param byte The byte to add
 * @param line The line number of where byte is located
 */
void writeChunk(Chunk* pChunk, uint8_t byte, int line);

/**
 * @brief Deallocates all of the memory used by the chunk and
 * then zeros out the fields
 * 
 * @param pChunk Pointer to the chunk to free
 */
void freeChunk(Chunk* pChunk);

/**
 * @brief Convenience method for adding a new constant to a chunk.
 * 
 * @param pChunk Pointer to the chunk to add the constant to
 * @param value The constant value to add
 * @return int The index of where the constant was appended
 */
int addConstant(Chunk* pChunk, Value value);

#endif