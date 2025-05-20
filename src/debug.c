#include <stdio.h>

#include "debug.h"
#include "common.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

/*
 * ==================================================
 * Typedefs & Prototypes
 * ==================================================
 */

static int simpleInstruction(const char* name, int offset);

static int constantInstruction(const char* name, Chunk* chunk, int offset);


/*
 * ==================================================
 * Module Level Variables & Constants
 * ==================================================
 */

#ifdef TULA_DEBUG

const char* TokenTypes[] = {
    "TOKEN_IDENTIFIER",
    "TOKEN_STRING",
    "TOKEN_NUMBER",
    "TOKEN_LEFT_PAREN",       /** ( */
    "TOKEN_RIGHT_PAREN",      /** ) */
    "TOKEN_LEFT_BRACE",       /** [ */
    "TOKEN_RIGHT_BRACE",      /** ] */
    "TOKEN_COMMA",            /** , */
    "TOKEN_DOT",              /** . */
    "TOKEN_SEMICOLON",        /** ; */
    "TOKEN_MINUS",            /** - */
    "TOKEN_PLUS",             /** + */
    "TOKEN_SLASH",            /** / */
    "TOKEN_STAR",             /** * */
    "TOKEN_BANG",             /** ! */
    "TOKEN_EQUAL",            /** = */
    "TOKEN_GREATER",          /** > */
    "TOKEN_LESS",             /** < */
    "TOKEN_GREATER_EQUAL",        /** >= */
    "TOKEN_LESS_EQUAL",           /** <= */
    "TOKEN_BANG_EQUAL",           /** != */
    "TOKEN_EQUAL_EQUAL",          /** == */
    "TOKEN_BANG_EQUAL_EQUAL",     /** !== */
    "TOKEN_EQUAL_EQUAL_EQUAL",    /** === */
    "TOKEN_PRINT",    /** print */
    "TOKEN_RETURN",   /** return */
    "TOKEN_FUNC",     /** func */
    "TOKEN_CLASS",    /** class */
    "TOKEN_SUPER",    /** super */
    "TOKEN_THIS",     /** this */
    "TOKEN_VAR",      /** var */
    "TOKEN_NIL",      /** nil */
    "TOKEN_FALSE",    /** false */
    "TOKEN_TRUE",     /** true */
    "TOKEN_AND",      /** and */
    "TOKEN_OR",       /** or */
    "TOKEN_IF",       /** if */
    "TOKEN_ELSE",     /** else */
    "TOKEN_FOR",      /** for */
    "TOKEN_WHILE",    /** while */
    "TOKEN_ERROR",
    "TOKEN_EOF",
};

const char* OpCodes[] = {
    "OP_RETURN",
    "OP_CONSTANT",
    "OP_NEGATE",
    "OP_ADD",
    "OP_SUBTRACT",
    "OP_MULTIPLY",
    "OP_DIVIDE",

    "OP_NIL",
    "OP_TRUE",
    "OP_FALSE",
    "OP_POP",
    "OP_GET_LOCAL",
    "OP_SET_LOCAL",
    "OP_GET_GLOBAL",
    "OP_DEFINE_GLOBAL",
    "OP_SET_GLOBAL",
    "OP_GET_UPVALUE",
    "OP_SET_UPVALUE",
    "OP_GET_PROPERTY",
    "OP_SET_PROPERTY",
    "OP_GET_SUPER",
    "OP_EQUAL",
    "OP_GREATER",
    "OP_LESS",
    "OP_NOT",
    "OP_PRINT",
    "OP_JUMP",
    "OP_JUMP_IF_FALSE",
    "OP_LOOP",
    "OP_CALL",
    "OP_INVOKE",
    "OP_SUPER_INVOKE",
    "OP_CLOSURE",
    "OP_CLOSE_UPVALUE",
    "OP_CLASS",
    "OP_INHERIT",
    "OP_METHOD"
};

const char* InterpretResults[] = {
    "INTERPRET_OK",
    "INTERPRET_COMPILE_ERROR",
    "INTERPRET_RUNTIME_ERROR"
};

#endif


/*
 * ==================================================
 * Function Definitions
 * ==================================================
 */

#ifdef TULA_DEBUG
static int simpleInstruction(const char* name, int offset) {
    printf(" %s\n", name);
    return offset + 1;
}
#endif /* TULA_DEBUG */


#ifdef TULA_DEBUG
static int constantInstruction(const char* name, Chunk* chunk, int offset) {
    uint8_t constant = chunk->code[offset + 1];
    printf(" %-16s %10d   '", name, constant);
    tula_printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}
#endif /* TULA_DEBUG */


int tula_disassembleInstruction(Chunk* chunk, int offset) {
#ifdef TULA_DEBUG
    printf("  %04d  ", offset);

    if (offset > 0 &&
        chunk->lines[offset] == chunk->lines[offset - 1]) {
        printf("      \\/ ");
    } else {
        printf(" %7d ", chunk->lines[offset]);
    }
  
    uint8_t instruction = chunk->code[offset];
    switch (instruction) {
        case OP_RETURN:
        case OP_NEGATE:
        case OP_ADD:
        case OP_SUBTRACT:
        case OP_MULTIPLY:
        case OP_DIVIDE:
            return simpleInstruction(OpCodes[instruction], offset);
            
        case OP_CONSTANT:
            return constantInstruction(OpCodes[instruction], chunk, offset);

        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
#else
    return 0;
#endif /* TULA_DEBUG */
}


void tula_printToken(Token* token) {
#ifdef TULA_DEBUG
    printf(
        "Token:{type:%s, start:%p, length:%d, line:%d}\n",
        TokenTypes[token->type],
        token->start,
        token->length,
        token->line
    );
#endif /* TULA_DEBUG */
}


void tula_disassembleChunk(Chunk* chunk, const char* name) {
#ifdef TULA_DEBUG
    printf("== %s ==\n", name);
    
    printf(
        "%-7s| %-7s| %-16s| %-10s| %-10s\n",
        "Offset",
        "Line #",
        "OpCode",
        "Const Idx",
        "Const Val"
    );
    
    for (int offset = 0; offset < chunk->count;) {
        offset = tula_disassembleInstruction(chunk, offset);
    }
#endif /* TULA_DEBUG */
}


void tula_printValue(Value value) {
#ifdef TULA_DEBUG
    printf("%g", value);
#endif /* TULA_DEBUG */
}

#pragma GCC diagnostic pop
