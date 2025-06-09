#ifndef tulac_debug_h
#define tulac_debug_h

#include "conf.h"

#ifdef TULA_DEBUG

#include "core/scanner.h"
#include "core/chunk.h"
#include "core/value.h"

void tula_printToken(Token* token);
void tula_disassembleChunk(Chunk* chunk, const char* name);
int tula_disassembleInstruction(Chunk* chunk, int offset);
void tula_printValue(Value value);

#endif /* TULA_DEBUG */

#endif /* tulac_debug_h */