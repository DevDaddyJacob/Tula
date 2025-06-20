#include <stdio.h>

#include "debug.h"
#include "common.h"
#include "core/lexer.h"
#include "core/chunk.h"
#include "core/value.h"

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

#ifdef TULA_DEBUGGING

const char* TokenTypes[] = {
    "TOK_EOF",
    "TOK_ERROR",
    "TOK_IDENTIFIER",
    "TOK_BOOLEAN",
    "TOK_STRING",
    "TOK_NUMBER",
    "TOK_NULL",
    "TOK_AND",
    "TOK_BREAK",
    "TOK_DO",
    "TOK_ELSE",
    "TOK_FALSE",
    "TOK_FOR",
    "TOK_FUNC",
    "TOK_IF",
    "TOK_IMPORT",
    "TOK_NOT",
    "TOK_OR",
    "TOK_RETURN",
    "TOK_THEN",
    "TOK_TRUE",
    "TOK_VAR",
    "TOK_WHILE",
    "TOK_L_PAREN",
    "TOK_R_PAREN",
    "TOK_L_BRACE",
    "TOK_R_BRACE",
    "TOK_L_CBRACE",
    "TOK_R_CBRACE",
    "TOK_COMMA",
    "TOK_DOT",
    "TOK_SEMICOLON",
    "TOK_MINUS",
    "TOK_PLUS",
    "TOK_SLASH",
    "TOK_STAR",
    "TOK_EXPO",
    "TOK_PERCENT",
    "TOK_EQUAL",
    "TOK_GREATER",
    "TOK_LESS",
    "TOK_GTR_EQU",
    "TOK_LESS_EQU",
    "TOK_BANG_EQU",
    "TOK_EQU_EQU"
};

const char* OpCodes[] = {
    "OP_RETURN",
    "OP_CONSTANT",
    "OP_NEGATE",
    "OP_ADD",
    "OP_SUBTRACT",
    "OP_MULTIPLY",
    "OP_DIVIDE",
    "OP_NULL",
    "OP_TRUE",
    "OP_FALSE",
    "OP_NOT",
    "OP_EQUAL",
    "OP_GREATER",
    "OP_LESS"
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

#ifdef TULA_DEBUGGING
static int simpleInstruction(const char* name, int offset) {
    printf(" %s\n", name);
    return offset + 1;
}
#endif /* TULA_DEBUGGING */


#ifdef TULA_DEBUGGING
static int constantInstruction(const char* name, Chunk* chunk, int offset) {
    Byte constant = chunk->code[offset + 1];
    printf(" %-16s %10d   '", name, constant);
    debug_printValue(chunk->constants.values[constant]);
    printf("'\n");
    return offset + 2;
}
#endif /* TULA_DEBUGGING */


int debug_disassembleInstruction(Chunk* chunk, int offset) {
#ifdef TULA_DEBUGGING
    Byte instruction;
    printf("  %04d  ", offset);

    if (offset > 0 &&
        chunk->lines[offset] == chunk->lines[offset - 1]) {
        printf("      \\/ ");
    } else {
        printf(" %7d ", chunk->lines[offset]);
    }
  
    instruction = chunk->code[offset];
    switch (instruction) {
        case OP_RETURN:
        case OP_NEGATE:
        case OP_ADD:
        case OP_SUBTRACT:
        case OP_MULTIPLY:
        case OP_DIVIDE:
        case OP_NULL:
        case OP_TRUE:
        case OP_FALSE:
        case OP_NOT:
        case OP_EQUAL:
        case OP_GREATER:
        case OP_LESS:
            return simpleInstruction(OpCodes[instruction], offset);
            
        case OP_CONSTANT:
            return constantInstruction(OpCodes[instruction], chunk, offset);

        default:
            printf("Unknown opcode %d\n", instruction);
            return offset + 1;
    }
#else
    return 0;
#endif /* TULA_DEBUGGING */
}


void debug_printToken(Token* token) {
#ifdef TULA_DEBUGGING
    printf(
        "Token:{type:%s, start:%p, length:%d, line:%d}\n",
        TokenTypes[token->type],
        token->start,
        token->length,
        token->line
    );
#endif /* TULA_DEBUGGING */
}


void debug_disassembleChunk(Chunk* chunk, const char* name) {
#ifdef TULA_DEBUGGING
    int offset;
    printf("== %s ==\n", name);
    
    printf(
        "%-7s| %-7s| %-16s| %-10s| %-10s\n",
        "Offset",
        "Line #",
        "OpCode",
        "Const Idx",
        "Const Val"
    );
    
    for (offset = 0; offset < chunk->count;) {
        offset = debug_disassembleInstruction(chunk, offset);
    }
#endif /* TULA_DEBUGGING */
}


void debug_printValue(Value value) {
#ifdef TULA_DEBUGGING
    switch (value.type) {
        case VAL_BOOL: {
            printf(AS_BOOL(value) ? "true" : "false");
            break;
        }
        
        case VAL_NIL: {
            printf("nil");
            break;
        }
        
        case VAL_NUMBER: {
            printf("%g", AS_NUMBER(value));
            break;
        }
    }
#endif /* TULA_DEBUGGING */
}

#pragma GCC diagnostic pop
