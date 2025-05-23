#include <stdio.h>
#include <stdlib.h>

#include "compiler.h"
#include "scanner.h"
#include "value.h"
#include "../common.h"
#include "../debug.h"
#include "../utils/log.h"

/*
 * ==================================================
 * Typedefs & Prototypes
 * ==================================================
 */

typedef void (*ParseFn)();

typedef struct {
    Scanner scanner;        /** The scanner used by the parser */
    Token current;          /** The current token being parsed */
    Token previous;         /** The last token parsed */
    BOOL hadError;          /** If the parser has encountered an error */
    BOOL panicMode;         /** Flag to prevent cascading errors */
} Parser;

typedef enum {
    PREC_NONE,
    PREC_ASSIGNMENT,  // =
    PREC_OR,          // or
    PREC_AND,         // and
    PREC_EQUALITY,    // == !=
    PREC_COMPARISON,  // < > <= >=
    PREC_TERM,        // + -
    PREC_FACTOR,      // * /
    PREC_UNARY,       // ! -
    PREC_CALL,        // . ()
    PREC_PRIMARY
} Precedence;

typedef struct {
    ParseFn prefix;
    ParseFn infix;
    Precedence precedence;
} ParseRule;

/**
 * TBD
 */
static Chunk* currentChunk();

/**
 * TBD
 */
static void errorAt(Token* token, const char* message);

/**
 * TBD
 */
static void errorAtCurrent(const char* message);

/**
 * TBD
 */
static void error(const char* message);

/**
 * TBD
 */
static void emitByte(uint8_t byte);

/**
 * TBD
 */
static void emitBytes(uint8_t byte1, uint8_t byte2);

/**
 * TBD
 */
static void emitReturn();

/**
 * TBD
 */
static void emitConstant(Value value);


/**
 * TBD
 */
static uint8_t makeConstant(Value value);


/**
 * TBD
 */
static void grouping();


/**
 * TBD
 */
static void unary();


/**
 * TBD
 */
static void binary();


/**
 * TBD
 */
static void literal();


/**
 * TBD
 */
static void number();


/**
 * TBD
 */
static void parsePrecedence(Precedence precedence);


/**
 * TBD
 */
static ParseRule* getRule(TokenType type);


/**
 * TBD
 */
static void expression();

/**
 * TBD
 */
static void advance();

/**
 * TBD
 */
static void consume(TokenType type, const char* message);

/**
 * TBD
 */
static void endCompiler();


/*
 * ==================================================
 * Module Level Variables & Constants
 * ==================================================
 */

Parser parser;
Chunk* compilingChunk;

ParseRule rules[] = {
    /* Literals */
    [TOKEN_IDENTIFIER]  = {NULL,     NULL,   PREC_NONE},
    [TOKEN_STRING]      = {NULL,     NULL,   PREC_NONE},
    [TOKEN_NUMBER]      = {number,   NULL,   PREC_NONE},

    /* Single Char Tokens*/
    [TOKEN_LEFT_PAREN]      = {grouping, NULL,   PREC_NONE},
    [TOKEN_RIGHT_PAREN]     = {NULL,     NULL,   PREC_NONE},
    [TOKEN_LEFT_BRACE]      = {NULL,     NULL,   PREC_NONE}, 
    [TOKEN_RIGHT_BRACE]     = {NULL,     NULL,   PREC_NONE},
    [TOKEN_COMMA]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_DOT]             = {NULL,     NULL,   PREC_NONE},
    [TOKEN_SEMICOLON]       = {NULL,     NULL,   PREC_NONE},
    [TOKEN_MINUS]           = {unary,    binary, PREC_TERM},
    [TOKEN_PLUS]            = {NULL,     binary, PREC_TERM},
    [TOKEN_SLASH]           = {NULL,     binary, PREC_FACTOR},
    [TOKEN_STAR]            = {NULL,     binary, PREC_FACTOR},
    [TOKEN_BANG]            = {unary,    NULL,   PREC_NONE},
    [TOKEN_EQUAL]           = {NULL,     NULL,   PREC_NONE},
    [TOKEN_GREATER]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_LESS]            = {NULL,     NULL,   PREC_NONE},
    
    /* Multi Char Tokens */
    [TOKEN_GREATER_EQUAL]       = {NULL,     NULL,   PREC_NONE},
    [TOKEN_LESS_EQUAL]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_BANG_EQUAL]          = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EQUAL_EQUAL]         = {NULL,     NULL,   PREC_NONE},
    [TOKEN_BANG_EQUAL_EQUAL]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EQUAL_EQUAL_EQUAL]   = {NULL,     NULL,   PREC_NONE},

    /* Keywords */
    [TOKEN_PRINT]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_RETURN]  = {NULL,     NULL,   PREC_NONE},
    [TOKEN_FUNC]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_CLASS]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_SUPER]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_VAR]     = {NULL,     NULL,   PREC_NONE},
    [TOKEN_THIS]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_NIL]     = {literal,  NULL,   PREC_NONE},
    [TOKEN_FALSE]   = {literal,  NULL,   PREC_NONE},
    [TOKEN_TRUE]    = {literal,  NULL,   PREC_NONE},
    [TOKEN_AND]     = {NULL,     NULL,   PREC_NONE},
    [TOKEN_OR]      = {NULL,     NULL,   PREC_NONE},
    [TOKEN_IF]      = {NULL,     NULL,   PREC_NONE},
    [TOKEN_ELSE]    = {NULL,     NULL,   PREC_NONE},
    [TOKEN_FOR]     = {NULL,     NULL,   PREC_NONE},
    [TOKEN_WHILE]   = {NULL,     NULL,   PREC_NONE},
    
    /* Other */
    [TOKEN_ERROR]   = {NULL,     NULL,   PREC_NONE},
    [TOKEN_EOF]     = {NULL,     NULL,   PREC_NONE},
};


/*
 * ==================================================
 * Function Definitions
 * ==================================================
 */

static Chunk* currentChunk() {
    return compilingChunk;
}


static void errorAt(Token* token, const char* message) {
    if (parser.panicMode) return;
    parser.panicMode = TRUE;
    tula_printfErr("Encountered a compile error:\n[line %d] Error", token->line);

    if (token->type == TOKEN_EOF) {
        tula_rawPrintfErr(" at end");
    } else if (token->type == TOKEN_ERROR) {
        /* Nothing? I guess for now maybe. :shrug: */
    } else {
        tula_rawPrintfErr(" at '%.*s'", token->length, token->start);
    }

    tula_rawPrintfErr(": %s\n", message);
    parser.hadError = TRUE;
}


static void errorAtCurrent(const char* message) {
    errorAt(&(parser.current), message);
}


static void error(const char* message) {
    errorAt(&(parser.previous), message);
}


static void emitByte(uint8_t byte) {
    tula_writeChunk(currentChunk(), byte, parser.previous.line);
}


static void emitBytes(uint8_t byte1, uint8_t byte2) {
    emitByte(byte1);
    emitByte(byte2);
}


static void emitReturn() {
    emitByte(OP_RETURN);
}


static void emitConstant(Value value){
    emitBytes(OP_CONSTANT, makeConstant(value));
}


static uint8_t makeConstant(Value value) {
    int constant = tula_addChunkConstant(currentChunk(), value);
    if (constant > UINT8_MAX) {
        error("Too many constants in one chunk.");
        return 0;
    }

    return (uint8_t)constant;
}


static void grouping() {
    expression();
    consume(TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
}


static void unary() {
    TokenType operatorType = parser.previous.type;
    

    /* Compile the operand */
    parsePrecedence(PREC_UNARY);


    /* Emit the operator instruction */
    switch (operatorType) {
        case TOKEN_BANG: emitByte(OP_NOT); break;
        case TOKEN_MINUS: emitByte(OP_NEGATE); break;
        default: return; /* Unreachable */
    }
}


static void binary() {
    TokenType operatorType = parser.previous.type;
    ParseRule* rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence + 1));

    switch (operatorType) {
        case TOKEN_PLUS: emitByte(OP_ADD); break;
        case TOKEN_MINUS: emitByte(OP_SUBTRACT); break;
        case TOKEN_STAR: emitByte(OP_MULTIPLY); break;
        case TOKEN_SLASH: emitByte(OP_DIVIDE); break;
        default: return; /* Unreachable */
    }
}


static void literal() {
    switch (parser.previous.type) {
        case TOKEN_FALSE: emitByte(OP_FALSE); break;
        case TOKEN_NIL: emitByte(OP_NIL); break;
        case TOKEN_TRUE: emitByte(OP_TRUE); break;
        default: return; /* Unreachable */
    }
}


static void number() {
    double value = strtod(parser.previous.start, NULL);
    emitConstant(NUMBER_VAL(value));
}


static void parsePrecedence(Precedence precedence) {
    advance();
    ParseFn prefixRule = getRule(parser.previous.type)->prefix;
    if (prefixRule == NULL) {
        error("Expect expression.");
        return;
    }

    prefixRule();

    while (precedence <= getRule(parser.current.type)->precedence) {
        advance();
        ParseFn infixRule = getRule(parser.previous.type)->infix;
        infixRule();
    }
}


static ParseRule* getRule(TokenType type) {
    return &(rules[type]);
}


static void expression() {
    parsePrecedence(PREC_ASSIGNMENT);
}


static void advance() {
    /* Move the current to be the last */
    parser.previous = parser.current;


    /* Loop through tokens until we cant */
    for (;;) {
        parser.current = tula_scanToken(&(parser.scanner));
        if (parser.current.type != TOKEN_ERROR) break;

        errorAtCurrent(parser.current.start);
    }
}


static void consume(TokenType type, const char* message) {
    if (parser.current.type == type) {
        advance(parser);
        return;
    }

    errorAtCurrent(message);
}


static void endCompiler() {
    emitReturn();
#ifdef TULA_DEBUG
    if (!parser.hadError) {
        tula_disassembleChunk(currentChunk(), "code");
    }
#endif /* TULA_DEBUG */
}


BOOL tula_compile(const char* source, Chunk* chunk) {
    /* Initialize the parser */
    tula_initScanner(&(parser.scanner), source);
    compilingChunk = chunk;


    /* Walk the scanner */
    advance();
    expression();
    consume(TOKEN_EOF, "Expected end of expression.");

    endCompiler();

    return !parser.hadError;
}