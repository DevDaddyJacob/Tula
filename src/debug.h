#ifndef tula_debug_h
#define tula_debug_h

#include "common.h"

#ifdef TULA_DEBUGGING

#include "core/lexer.h"
#include "core/chunk.h"
#include "core/value.h"

void debug_printToken(Token* token);
void debug_disassembleChunk(Chunk* chunk, const char* name);
int debug_disassembleInstruction(Chunk* chunk, int offset);
void debug_printValue(Value value);

#endif /* TULA_DEBUGGING */


#endif /* tula_debug_h */