#ifndef tulac_chunk_h
#define tulac_chunk_h

#include "value.h"


typedef enum {
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
    
    OP_NIL,
    OP_TRUE,
    OP_FALSE,
    OP_POP,
    OP_GET_LOCAL,
    OP_SET_LOCAL,
    OP_GET_GLOBAL,
    OP_DEFINE_GLOBAL,
    OP_SET_GLOBAL,
    OP_GET_UPVALUE,
    OP_SET_UPVALUE,
    OP_GET_PROPERTY,
    OP_SET_PROPERTY,
    OP_GET_SUPER,
    OP_EQUAL,
    OP_GREATER,
    OP_LESS,
    OP_NOT,
    OP_PRINT,
    OP_JUMP,
    OP_JUMP_IF_FALSE,
    OP_LOOP,
    OP_CALL,
    OP_INVOKE,
    OP_SUPER_INVOKE,
    OP_CLOSURE,
    OP_CLOSE_UPVALUE,
    OP_CLASS,
    OP_INHERIT,
    OP_METHOD
} OpCode;


typedef struct {
    int count;              /** The amount of the elements which are in use */
    int capacity;           /** The amount of elements in the array */
    int* lines;             /** Array of lines which parallels the bytecodes */
    uint8_t* code;          /** Pointer to the first element of the array */
    ValueArray constants;   /** The array storing the chunk's constants */
} Chunk;


/**
 * \brief           Initializes the provided chunk
 * \param[in]       chunk: Pointer to the chunk to initialize
 */
void tula_initChunk(Chunk* chunk);


/**
 * \brief           Releases the provided chunk
 * \param[in]       chunk: Pointer to the chunk to free
 */
void tula_freeChunk(Chunk* chunk);


/**
 * \brief           Writes the provided bytecode to the chunk
 * \param[in]       chunk: Pointer to the chunk to write to
 * \param[in]       byte: The bytecode to write to the chunk
 * \param[in]       line: The line number the bytecode is located on
 */
void tula_writeChunk(Chunk* chunk, uint8_t byte, int line);


/**
 * \brief           Adds the provided constant to the chunk's constant array
 * \param[in]       chunk: Pointer to the chunk to write to
 * \param[in]       value: The value to write to the chunk's constant array
 * \return          Returns the index of where the constant was appended
 */
int tula_addChunkConstant(Chunk* chunk, Value value);

#endif /* tulac_chunk_h */