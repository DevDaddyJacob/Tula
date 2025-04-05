#include <stdio.h>

#include "common.h"
#include "chunk.h"
#include "debug.h"
#include "vm.h"

int main(int argc, const char* argv[]) {
    VM vm;
    Chunk chunk;
    
    initVM(&vm);
    initChunk(&chunk);

    
    int constIndex = addConstant(&chunk, 1.2);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constIndex, 123);

    constIndex = addConstant(&chunk, 3.4);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constIndex, 123);

    writeChunk(&chunk, OP_ADD, 123);

    constIndex = addConstant(&chunk, 5.6);
    writeChunk(&chunk, OP_CONSTANT, 123);
    writeChunk(&chunk, constIndex, 123);

    writeChunk(&chunk, OP_DIVIDE, 123);

    writeChunk(&chunk, OP_NEGATE, 123);

    writeChunk(&chunk, OP_RETURN, 123);


    disassembleChunk(&chunk, "test chunk");
    interpret(&vm, &chunk);

    freeVM(&vm);
    freeChunk(&chunk);
    
    return 0;
}