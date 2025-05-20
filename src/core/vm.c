#include <stdio.h>

#include "compiler.h"
#include "vm.h"
#include "../common.h"
#include "../debug.h"


/**
 * \brief           Resets the vm's stack to the begining
 * \param[in]       vm: Pointer to the vm to reset
 */
static void resetVMStack(VM* vm) {
    vm->stackTop = vm->stack;
}


void initVM(VM* vm) {
    resetVMStack(vm);
}


void freeVM(VM* vm) {

}


void vmStackPush(VM* vm, Value value) {
    *(vm->stackTop) = value;
    vm->stackTop++;
}


Value vmStackPop(VM* vm) {
    vm->stackTop--;
    return *(vm->stackTop);
}


/**
 * \brief           Starts the VM and reads the bytecode
 * \param[in]       vm: Pointer to the VM to start
 * \return          Returns the interpertation result
 */
static InterpretResult runVM(VM* vm) {
/* ============================== */
#define READ_BYTE() (*(vm->ip++))
#define READ_CONSTANT() (vm->chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
    do { \
        double b = vmStackPop(vm); \
        double a = vmStackPop(vm); \
        vmStackPush(vm, a op b); \
    } while (FALSE)
/* ============================== */

#ifdef TULA_DEBUG
    printf(
        "%-7s| %-7s| %-16s| %-10s| %-10s\n",
        "Offset",
        "Line #",
        "OpCode",
        "Const Idx",
        "Const Val"
    );
#endif /* TULA_DEBUG */

    for (;;) {

#ifdef TULA_DEBUG
        /* Trace the stack */
        printf("%27s", "Stack: ");
        for (Value* slot = vm->stack; slot < vm->stackTop; slot++) {
            printf("[ ");
            printValue(*slot);
            printf(" ]");
        }
        printf("\n");

        disassembleInstruction(
            vm->chunk,
            (int)(vm->ip - vm->chunk->code)
        );
#endif /* TULA_DEBUG */

        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_RETURN: {
                printValue(vmStackPop(vm));
                printf("\n");
                return INTERPRET_OK;
            }

            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                vmStackPush(vm, constant);
                break;
            }

            case OP_NEGATE: {
                vmStackPush(vm, -vmStackPop(vm));
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
        }
    }

/* ============================== */
#undef READ_BYTE
#undef READ_CONSTANT
#undef BINARY_OP
/* ============================== */
}


InterpretResult interpretChunk(VM* vm, Chunk* chunk) {
    vm->chunk = chunk;
    vm->ip = vm->chunk->code;
    return runVM(vm);
}


InterpretResult interpretSource(VM* vm, const char* source) {
    Chunk chunk;
    initChunk(&chunk);


    /* Compile the chunk, catching any errors */
    if (compile)
}