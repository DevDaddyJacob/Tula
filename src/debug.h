#ifndef tulac_debug_h
#define tulac_debug_h

#include "conf.h"
#include "core/scanner.h"
#include "core/chunk.h"
#include "core/value.h"

void printToken(Token* token);
void disassembleChunk(Chunk* chunk, const char* name);
int disassembleInstruction(Chunk* chunk, int offset);
void printValue(Value value);

#endif /* tulac_debug_h */