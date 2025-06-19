#ifndef tula_core_parser_h
#define tula_core_parser_h

#include "chunk.h"
#include "../common.h"

/**
 * \brief           Compiles bytecode from provided source code into the 
 *                  provided chunk
 * \note            Output is a boolean treated integer
 * \param[in]       source: The source code to compile
 * \param[out]      chunk: The chunk of bytecode to compile to
 * \return          Returns 1 if the source was compiled successfully, 
 *                  and 0 otherwise
 */
TULA_FUNC Bool tula_parseSource(const char* source, Chunk* chunk);



#endif /* tula_core_parser_h */