#include <stdio.h>

#include "vm.h"
#include "compiler.h"
#include "../common.h"
#include "../debug.h"

/*
 * ==================================================
 * Typedefs & Prototypes
 * ==================================================
 */

/**
 * \brief           Resets the vm's stack to the begining
 * \param[in]       vm: Pointer to the vm to reset
 */
static void resetVMStack(VM* vm);


/**
 * \brief           Starts the VM and reads the bytecode
 * \param[in]       vm: Pointer to the VM to start
 * \return          Returns the interpertation result
 */
static InterpretResult runVM(VM* vm);


/*
 * ==================================================
 * Function Definitions
 * ==================================================
 */

static void resetVMStack(VM* vm) {
    vm->stackTop = vm->stack;
}


static InterpretResult runVM(VM* vm) {
/* ============================== */
#define READ_BYTE() (*(vm->ip++))
#define READ_CONSTANT() (vm->chunk->constants.values[READ_BYTE()])
#define BINARY_OP(op) \
    do { \
        double b = tula_vmStackPop(vm); \
        double a = tula_vmStackPop(vm); \
        tula_vmStackPush(vm, a op b); \
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
            tula_printValue(*slot);
            printf(" ]");
        }
        printf("\n");

        tula_disassembleInstruction(
            vm->chunk,
            (int)(vm->ip - vm->chunk->code)
        );
#endif /* TULA_DEBUG */

        uint8_t instruction;
        switch (instruction = READ_BYTE()) {
            case OP_RETURN: {
                tula_printValue(tula_vmStackPop(vm));
                printf("\n");
                return INTERPRET_OK;
            }

            case OP_CONSTANT: {
                Value constant = READ_CONSTANT();
                tula_vmStackPush(vm, constant);
                break;
            }

            case OP_NEGATE: {
                tula_vmStackPush(vm, -tula_vmStackPop(vm));
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


void tula_initVM(VM* vm) {
    resetVMStack(vm);
}


void tula_freeVM(VM* vm) {

}


void tula_vmStackPush(VM* vm, Value value) {
    *(vm->stackTop) = value;
    vm->stackTop++;
}


Value tula_vmStackPop(VM* vm) {
    vm->stackTop--;
    return *(vm->stackTop);
}


InterpretResult tula_interpretChunk(VM* vm, Chunk* chunk) {
    vm->chunk = chunk;
    vm->ip = vm->chunk->code;
    return runVM(vm);
}


InterpretResult tula_interpretSource(VM* vm, const char* source) {
    Chunk chunk;
    tula_initChunk(&chunk);


    /* Compile the chunk, catching any errors */
    if (!tula_compile(source, &chunk)) {
        tula_freeChunk(&chunk);
        return INTERPRET_COMPILE_ERROR;
    }


    /* Insert the chunk of bytecode into the VM */
    vm->chunk = &chunk;
    vm->ip = vm->chunk->code;

    
    /* Run the VM with the bytecode */
    InterpretResult result = runVM(vm);

    tula_freeChunk(&chunk);
    return result;
}