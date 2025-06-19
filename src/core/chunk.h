#ifndef tula_core_chunk_h
#define tula_core_chunk_h

#include "value.h"
#include "../common.h"


typedef enum tula_OpCode {
    /**
     * 1 byte long
     * - byte 1: Opcode
     */
    OP_RETURN,              

    /**
     * 2 bytes long
     * - byte 1: Opcode
     * - byte 2: Constant index
     */
    OP_CONSTANT,
    
    /**
     * 1 byte long
     * - byte 1: Opcode
     */
    OP_NEGATE,
    
    /**
     * 1 byte long
     * - byte 1: Opcode
     */
    OP_ADD,
    
    /**
     * 1 byte long
     * - byte 1: Opcode
     */
    OP_SUBTRACT,
    
    /**
     * 1 byte long
     * - byte 1: Opcode
     */
    OP_MULTIPLY,
    
    /**
     * 1 byte long
     * - byte 1: Opcode
     */
    OP_DIVIDE,
    
    /**
     * 1 byte long
     * - byte 1: Opcode
     */
    OP_NULL,
    
    /**
     * 1 byte long
     * - byte 1: Opcode
     */
    OP_TRUE,
    
    /**
     * 1 byte long
     * - byte 1: Opcode
     */
    OP_FALSE,

    /**
     * 1 byte long
     * - byte 1: Opcode
     */
    OP_NOT
} OpCode;


typedef struct tula_Chunk {
    Int16 count;            /** The amount of the elements which are in use */
    Int16 capacity;         /** The amount of elements in the array */
    Int16* lines;           /** Array of lines which parallels the bytecodes */
    Byte* code;             /** Pointer to the first element of the array */
    ValueArray constants;   /** The array storing the chunk's constants */
} Chunk;


/**
 * \brief           Creates a new chunk in memory
 * \return          Pointer to the chunk to initialize
 */
TULA_FUNC Chunk* tulaChk_new();


/**
 * \brief           Releases the provided chunk
 * \param[in]       chunk: Pointer to the chunk to free
 */
TULA_FUNC void tulaChk_destroy(Chunk* chunk);


/**
 * \brief           Writes the provided bytecode to the chunk
 * \param[in]       chunk: Pointer to the chunk to write to
 * \param[in]       byte: The bytecode to write to the chunk
 * \param[in]       line: The line number the bytecode is located on
 */
TULA_FUNC void tulaChk_write(Chunk* chunk, Byte byte, Int16 line);


/**
 * \brief           Adds the provided constant to the chunk's constant array
 * \param[in]       chunk: Pointer to the chunk to write to
 * \param[in]       value: The value to write to the chunk's constant array
 * \return          Returns the index of where the constant was appended
 */
TULA_FUNC Int16 tulaChk_addConst(Chunk* chunk, Value value);

#endif /* tula_core_chunk_h */