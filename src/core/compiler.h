#ifndef tulac_compiler_h
#define tulac_compiler_h

#include "chunk.h"
#include "../common.h"

/**
 * \brief           Compiles bytecode from provided source code
 * \note            Output is a boolean treated integer
 * \param[in]       source: The source code to compile
 * \param[out]      chunk: The chunk of bytecode to compile to
 * \return          Returns 1 if the source was compiled successfully, 
 *                  and 0 otherwise
 */
BOOL tula_compile(const char* source, Chunk* chunk);

#endif /* tulac_compiler_h */