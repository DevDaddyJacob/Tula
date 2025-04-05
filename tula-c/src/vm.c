#include <stdio.h>

#include "common.h"
#include "debug.h"
#include "vm.h"

static InterpretResult run(VM* pVM) {
#define READ_BYTE() (*pVM->pIP++)
#define READ_CONSTANT() (pVM->pChunk->constants.pValues[READ_BYTE()])

    for (;;) {
#ifdef DEBUG_TRACE_EXECUTION
        printf("          ");
        for (Value* pSlot = pVM->stack; pSlot < pVM->pStackTop; pSlot++) {
            printf("[ ");
            printValue(*pSlot);
            printf(" ]");
        }
        printf("\n");
        disassembleInstruction(pVM->pChunk, (int)(pVM->pIP - pVM->pChunk->pCode));
#endif

        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                push(pVM, constant);
                break;
            }

            case OP_RETURN: {
                printValue(pop(pVM));
                printf("\n");
                return INTERPRET_OK;
            }
        }
    }

#undef READ_BYTE
#undef READ_CONSTANT
}

static void resetStack(VM* pVM) {
    pVM->pStackTop = pVM->stack;
}

void initVM(VM* pVM) {
    resetStack(pVM);
}

void freeVM(VM* pVM) {
    
}

InterpretResult interpret(VM* pVM, Chunk* pChunk) {
    pVM->pChunk = pChunk;
    pVM->pIP = pVM->pChunk->pCode;
    return run(pVM);
}

void push(VM* pVM, Value value) {
    // Store value at the pStackTop position
    *pVM->pStackTop = value;

    // Increment the top position by 1
    pVM->pStackTop++;
}

Value pop(VM* pVM) {
    // Move the stack pointer back to the last used position
    pVM->pStackTop--;

    // Dereference and return the value in that position
    return *pVM->pStackTop;
}