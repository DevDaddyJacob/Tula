#ifndef tulac_vm_h
#define tulac_vm_h

#include "chunk.h"
#include "value.h"
#include "../conf.h"


typedef enum {
    INTERPRET_OK,
    INTERPRET_COMPILE_ERROR,
    INTERPRET_RUNTIME_ERROR
} InterpretResult;


typedef struct {
    Chunk* chunk;                   /** The bytecode to read and interpert */
    uint8_t* ip;                    /** Instruction pointer. Points into the chunk */
    Value stack[TULA_STACK_MAX];    /** The vm's stack */
    Value* stackTop;                /** Pointer to the next empty slot in the stack */
} VM;


/**
 * \brief           Initializes the provided VM
 * \param[in]       vm: Pointer to the VM to initialize
 */
void tula_initVM(VM* vm);


/**
 * \brief           Releases the provided VM from memory
 * \param[in]       vm: Pointer to the VM to free
 */
void tula_freeVM(VM* vm);


/**
 * \brief           Uses the provided VM to interpert bytecode
 * \param[in]       vm: Pointer to the VM to use for the interpertation
 * \param[in]       chunk: Pointer to the bytecode to interpert
 * \return          Returns the interpertation result
 */
InterpretResult tula_interpretChunk(VM* vm, Chunk* chunk);


/**
 * \brief           Uses the provided VM to interpert source code
 * \param[in]       vm: Pointer to the VM to use for the interpertation
 * \param[in]       source: Pointer to the source code
 * \return          Returns the interpertation result
 */
InterpretResult tula_interpretSource(VM* vm, const char* source);


/**
 * \brief           Pushes the provided value onto the provided vm's stack
 * \param[in]       vm: Pointer to the VM to use
 * \param[in]       value: The value to add to the stack
 */
void tula_vmStackPush(VM* vm, Value value);


/**
 * \brief           Pops the top value off of the provided vm's stack and 
 *                  returns it
 * \param[in]       vm: Pointer to the VM to use
 * \return          Returns the top value on the vm's stack
 */
Value tula_vmStackPop(VM* vm);

#endif /* tulac_vm_h */