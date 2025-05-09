#include <stdio.h>

#include "debug.h"

/**
 * @brief Prints the name of the opcode and then returns the 
 * next byte offset past the this instruction
 * 
 * @param pName Name of the opcode
 * @param offset The current offset
 * @return int The next offset
 */
static int simpleInstruction(const char* pName, int offset) {
    printf("%s\n", pName);
    return offset + 1;
}

static int constantInstruction(const char* name, Chunk* pChunk, int offset) {
    uint8_t constant = pChunk->pCode[offset + 1];
    printf("%-16s %4d '", name, constant);
    printValue(pChunk->constants.pValues[constant]);
    printf("'\n");
    return offset + 2;
}

void disassembleChunk(Chunk* pChunk, const char* pName) {
    printf("== %s ==\n", pName);

    for (int offset = 0; offset < pChunk->count;) {
        offset = disassembleInstruction(pChunk, offset);
    }
}

int disassembleInstruction(Chunk* pChunk, int offset) {
    printf("%04d ", offset);

    if (offset > 0 && pChunk->pLines[offset] == pChunk->pLines[offset - 1]) {
        printf("   | ");
    } else {
        printf("%4d ", pChunk->pLines[offset]);
    }

    uint8_t instruction = pChunk->pCode[offset];
    switch (instruction) {
        case OP_CONSTANT:
            return constantInstruction("OP_CONSTANT", pChunk, offset);

            case OP_ADD:
                return simpleInstruction("OP_ADD", offset);

            case OP_SUBTRACT:
                return simpleInstruction("OP_SUBTRACT", offset);

            case OP_MULTIPLY:
                return simpleInstruction("OP_MULTIPLY", offset);

            case OP_DIVIDE:
                return simpleInstruction("OP_DIVIDE", offset);

            case OP_NEGATE:
                return simpleInstruction("OP_NEGATE", offset);

            case OP_RETURN:
                return simpleInstruction("OP_RETURN", offset);
        
        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
}