#ifndef tulac_vm_h
#define tulac_vm_h

#include "chunk.h"
#include "value.h"

#define STACK_MAX 256

typedef struct {
    /**
     * @brief Instruction pointer
     */
    uint8_t* pIP;
    Chunk* pChunk;
    Value stack[STACK_MAX];
    Value* pStackTop;
} VM;

typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;

/**
 * @brief Initialized an instance of a VM
 * 
 * @param pVM Pointer to the VM to initialize
 */
void initVM(VM* pVM);

/**
 * @brief Deallocates a VM
 * 
 * @param pVM Pointer to the VM to free
 */
void freeVM(VM* pVM);

/**
 * @brief Instructs a VM to interpert a chunk
 * 
 * @param pVM Pointer to the VM
 * @param source The source code to interpret
 * @return InterpretResult The result of the interpertation
 */
InterpretResult interpret(VM* pVM, const char* source);

/**
 * @brief Pushes a value onto the top of the VM's stack
 * 
 * @param pVM Pointer to the VM
 * @param value The value to add to the stack
 */
void push(VM* pVM, Value value);

/**
 * @brief Pops the top most value off of the stack and returns it
 * 
 * @param pVM Pointer to the VM
 * @return Value The top most value on the stack
 */
Value pop(VM* pVM);

#endif