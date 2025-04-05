#ifndef tulac_debug_h
#define tulac_debug_h

#include "chunk.h"

/**
 * @brief Disassembles all of the instructions in an entire chunk
 * 
 * @param pChunk Pointer to the chunk to disassemble
 * @param pName Name of the chunk
 */
void disassembleChunk(Chunk* pChunk, const char* pName);

/**
 * @brief Reads a byte from the bytecode at the given offset, and 
 * then processes the opcode.
 * 
 * @param pChunk Pointer to the chunk to read from
 * @param offset The offset of the instruction to read
 * @return int The offset of the next instruction
 */
int disassembleInstruction(Chunk* pChunk, int offset);

#endif