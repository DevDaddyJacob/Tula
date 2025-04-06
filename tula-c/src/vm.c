#include <stdio.h>

#include "common.h"
#include "compiler.h"
#include "debug.h"
#include "vm.h"

static InterpretResult run(VM* pVM) {
#define READ_BYTE() (*pVM->pIP++)
#define READ_CONSTANT() (pVM->pChunk->constants.pValues[READ_BYTE()])
#define BINARY_OP(op) \
    do { \
        double b = pop(pVM); \
        double a = pop(pVM); \
        push(pVM, a op b); \
    } while (false)

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

            case OP_ADD: {
                BINARY_OP(+);
                break;
            }

            case OP_SUBTRACT: {
                BINARY_OP(-);
                break;
            }

            case OP_MULTIPLY: {
                BINARY_OP(*);
                break;
            }

            case OP_DIVIDE: {
                BINARY_OP(/);
                break;
            }
            
            case OP_NEGATE: {
                push(pVM, -pop(pVM));
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
#undef BINARY_OP
}

static void resetStack(VM* pVM) {
    pVM->pStackTop = pVM->stack;
}

void initVM(VM* pVM) {
    resetStack(pVM);
}

void freeVM(VM* pVM) {
    
}

InterpretResult interpret(VM* pVM, const char* source) {
    Chunk chunk;
    initChunk(&chunk);

    if (!compile(source, &chunk)) {
        freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }

    pVM->pChunk = &chunk;
    pVM->pIP = pVM->pChunk->pCode;

    InterpretResult result = run(pVM);

    freeChunk(&chunk);
    return result;
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