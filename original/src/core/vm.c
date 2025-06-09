#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "vm.h"
#include "compiler.h"
#include "../common.h"
#include "../debug.h"
#include "../utils/log.h"

/*
 * ==================================================
 * Typedefs & Prototypes
 * ==================================================
 */

/**
 * \brief           Checks if a provided value is a "falsey" value
 * \param[in]       value: The value to check
 * \return          Returns 1 if it is falsey, 0 otherwise
 */
static BOOL isFalsey(Value value);


/**
 * \brief           Returns a value from the provided vm's stack without 
 *                  poping it
 * \param[in]       vm: Pointer to the vm to look into
 * \param[in]       distance: How far into the stack to look
 */
static Value peekVMStack(VM* vm, int distance);


/**
 * \brief           Resets the vm's stack to the begining
 * \param[in]       vm: Pointer to the vm to reset
 */
static void resetVMStack(VM* vm);


/**
 * \brief           Triggers a runtime error in the provided VM
 * \param[in]       vm: Pointer to the vm running
 * \param[in]       format: The printf error message format specifier
 * \param[in]       ...: The printf arguments
 */
static void runtimeError(VM* vm, const char* format, ...);


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

static BOOL isFalsey(Value value) {
    return IS_NIL(value) || (IS_BOOL(value) && !AS_BOOL(value));
}

static Value peekVMStack(VM* vm, int distance) {
    return vm->stackTop[-1 - distance];
}


static void resetVMStack(VM* vm) {
    vm->stackTop = vm->stack;
}


static void runtimeError(VM* vm, const char* format, ...) {
    va_list args;
    va_start(args, format);
    tula_printfErr(format, args);
    va_end(args);
    tula_putsErr("\n");

    size_t instruction = vm->ip - vm->chunk->code - 1;
    int line = vm->chunk->lines[instruction];
    tula_rawPrintfErr("[line %d] in script\n", line);
    resetVMStack(vm);
}


static InterpretResult runVM(VM* vm) {
/* ============================== */
#define READ_BYTE() (*(vm->ip++))
#define READ_CONSTANT() (vm->chunk->constants.values[READ_BYTE()])
#define BINARY_OP(valueType, op) \
    do { \
        /* Check operand types */ \
        if ( \
            !IS_NUMBER(peekVMStack(vm, 0)) \
            || !IS_NUMBER(peekVMStack(vm, 1)) \
        ) { \
            runtimeError(vm, "Operands must be numbers."); \
            return INTERPRET_RUNTIME_ERROR; \
        } \
        \
        double b = AS_NUMBER(tula_vmStackPop(vm)); \
        double a = AS_NUMBER(tula_vmStackPop(vm)); \
        tula_vmStackPush(vm, valueType(a op b)); \
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
        if (vm->stack >= vm->stackTop) printf("<>");
        
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
                if (!IS_NUMBER(peekVMStack(vm, 0))) {
                    runtimeError(vm, "Operand must be a number.");
                    return INTERPRET_RUNTIME_ERROR;
                }
                tula_vmStackPush(vm, NUMBER_VAL(-AS_NUMBER(tula_vmStackPop(vm))));
                break;
            }

            case OP_ADD: {
                BINARY_OP(NUMBER_VAL, +);
                break;
            }

            case OP_SUBTRACT: {
                BINARY_OP(NUMBER_VAL, -);
                break;
            }

            case OP_MULTIPLY: {
                BINARY_OP(NUMBER_VAL, *);
                break;
            }

            case OP_DIVIDE: {
                BINARY_OP(NUMBER_VAL, /);
                break;
            }

            case OP_NIL: {
                tula_vmStackPush(vm, NIL_VAL);
                break;
            }

            case OP_TRUE: {
                tula_vmStackPush(vm, BOOL_VAL(TRUE));
                break;
            }

            case OP_FALSE: {
                tula_vmStackPush(vm, BOOL_VAL(FALSE));
                break;
            }

            case OP_NOT: {
                tula_vmStackPush(vm, BOOL_VAL(isFalsey(tula_vmStackPop(vm))));
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