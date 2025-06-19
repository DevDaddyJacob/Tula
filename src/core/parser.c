#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

#include "parser.h"
#include "lexer.h"
#include "../common.h"
#include "../debug.h"

/*
 * ==================================================
 * Macros
 * ==================================================
 */

/* #define XYZ "ABC" */

/*
 * ==================================================
 * Typedefs & Prototypes
 * ==================================================
 */

/**
 * Function for parsing a specific operator
 */
typedef void (*ParseFn)();


/**
 * Operator precedence from lowest to highest
 */
typedef enum tula_Precedence {
    PREC_NONE,
    PREC_ASSIGNMENT,        /** `=` */
    PREC_OR,                /** `or` */
    PREC_AND,               /** `and` */
    PREC_EQUALITY,          /** `==` `!=` */
    PREC_COMPARATIVE,       /** `<` `>` `<=` `>=` */
    PREC_ADDATIVE,          /** `+` `-` */
    PREC_MULTIPLICATIVE,    /** `*` `/` */
    PREC_UNARY,             /** `not` `-` */
    PREC_EXPONENT,          /** `^` */
    PREC_CALL,              /** `.` `()` */
    PREC_PRIMARY
} Precedence;


typedef struct {
    /**
     * The function to compile a prefix expression starting with a token of a
     * specific type
     */
    ParseFn prefix;

    /**
     * The function to compile a infix expression, whose left operand is
     * followed by a token of a specific type
     */
    ParseFn infix;

    /**
     * The precedence of an infix expression that uses a specific token as
     * an operator
     */
    Precedence precedence;
} ParseRule;


typedef struct {
    Lexer* lexer;           /** The lexer used by the parser */
    Token current;          /** The current token being parsed */
    Token previous;         /** The last token parsed */
    Bool hadError;          /** If the parser has encountered an error */
    Bool panicMode;         /** Flag to prevent cascading errors */
} Parser;


/**
 * \brief           Accessor method for the current chunk being compiled
 * \returns         Returns a pointer to the chunk currently being compiled
 */
static Chunk* currentChunk();


/**
 * \brief           Advances the parser's lexer to the next token
 */
static void advanceParser();


/**
 * \brief           Advances the parser's lexer past the current token if the 
 *                  current token's type matches the provided type
 * \param[in]       type: The type of token to check for
 * \param[in]       message: The error message to emit if the current token's
 *                  type does not match
 */
static void advanceParserIf(TokenType type, const char* message);


/**
 * \brief           Ends the parser and emits the return op code
 */
static void endParser();


/**
 * \brief           Emits a error for a given token with a specific message
 * \param[in]       token: Pointer to the token to error at
 * \param[in]       message: The error message to use
 */
static void errorAt(Token* token, const char* message);


/**
 * \brief           Emits an error at the current token with a specifc message
 * \param[in]       message: The error message to use
 */
static void errorAtCurrent(const char* message);


/**
 * \brief           Emits an error at the previous token with a specifc message
 * \param[in]       message: The error message to use
 */
static void error(const char* message);


/**
 * \brief           Adds the provided value as a constant in the current chunk
 * \param[in]       value: The value to add as a constant
 * \return          Returns the index of where the constant was appended, or 0
 *                  if it was not able to be appended
 */
static Byte makeConstant(Value value);


/**
 * \brief           Writes the provided byte to the current chunk
 * \param[in]       byte: The byte to write
 */
static void emitByte(Byte byte);


/**
 * \brief           Writes the provided bytes to the current chunk
 * \param[in]       byte1: The first byte to write
 * \param[in]       byte2: The second byte to write
 */
static void emitBytes(Byte byte1, Byte byte2);


/**
 * \brief           Writes the return op code to the current chunk
 */
static void emitReturn();


/**
 * \brief           Writes a constant with the provided value to the current
 *                  chunk
 * \param[in]       value: The value to write as a constant
 */
static void emitConstant(Value value);


/**
 * \brief           Converts the provided token type into a parse rule
 * \param[in]       type: The type of token to get the parse rule for
 * \return          Returns a pointer to the parse rule struct for the provided
 *                  type
 */
static ParseRule* getRule(TokenType type);


/**
 * \brief           Parses tokens until a token with a lower precedence than
 *                  the specified is encountered
 * \param[in]       precedence: The lowest precendence to parse, meaning any
 *                  precedence greater or equal to this will be parsed
 */
static void parsePrecedence(Precedence precedence);


/**
 * \brief           Handles the parsing of a expression
 */
static void parseExpression();


/**
 * \brief           Handles the parsing of groupings
 */
static void parseGrouping();


/**
 * \brief           Handles the parsing of unary operators
 */
static void parseUnary();


/**
 * \brief           Handles the parsing of binary operators
 */
static void parseBinary();


/**
 * \brief           Handles the parsing of literals
 */
static void parseLiteral();


/**
 * \brief           Handles the parsing of numbers
 */
static void parseNumber();


/*
 * ==================================================
 * Module Level Variables & Constants
 * ==================================================
 */

Parser parser;

Chunk* compilingChunk = NULL;

static ParseRule PARSE_RULES[] = {
    /* Literals */
    { NULL,             NULL,           PREC_NONE },        /* TOK_EOF */
    { NULL,             NULL,           PREC_NONE },        /* TOK_ERROR */
    { NULL,             NULL,           PREC_NONE },        /* TOK_IDENTIFIER */
    { NULL,             NULL,           PREC_NONE },        /* TOK_BOOLEAN */
    { NULL,             NULL,           PREC_NONE },        /* TOK_STRING */
    { parseNumber,      NULL,           PREC_NONE },        /* TOK_NUMBER */
    { parseLiteral,     NULL,           PREC_NONE },        /* TOK_NULL */


    /* Keywords */
    { NULL,             NULL,           PREC_NONE },        /* TOK_AND */
    { NULL,             NULL,           PREC_NONE },        /* TOK_BREAK */
    { NULL,             NULL,           PREC_NONE },        /* TOK_DO */
    { NULL,             NULL,           PREC_NONE },        /* TOK_ELSE */
    { parseLiteral,     NULL,           PREC_NONE },        /* TOK_FALSE */
    { NULL,             NULL,           PREC_NONE },        /* TOK_FOR */
    { NULL,             NULL,           PREC_NONE },        /* TOK_FUNC */
    { NULL,             NULL,           PREC_NONE },        /* TOK_IF */
    { NULL,             NULL,           PREC_NONE },        /* TOK_IMPORT */
    { parseUnary,       NULL,           PREC_NONE },        /* TOK_NOT */
    { NULL,             NULL,           PREC_NONE },        /* TOK_OR */
    { NULL,             NULL,           PREC_NONE },        /* TOK_RETURN */
    { NULL,             NULL,           PREC_NONE },        /* TOK_THEN */
    { parseLiteral,     NULL,           PREC_NONE },        /* TOK_TRUE */
    { NULL,             NULL,           PREC_NONE },        /* TOK_VAR */
    { NULL,             NULL,           PREC_NONE },        /* TOK_WHILE */


    /* Single Char Tokens*/
    { parseGrouping,    NULL,           PREC_NONE },            /* TOK_L_PAREN */
    { NULL,             NULL,           PREC_NONE },            /* TOK_R_PAREN */
    { NULL,             NULL,           PREC_NONE },            /* TOK_L_BRACE */
    { NULL,             NULL,           PREC_NONE },            /* TOK_R_BRACE */
    { NULL,             NULL,           PREC_NONE },            /* TOK_L_CBRACE */
    { NULL,             NULL,           PREC_NONE },            /* TOK_R_CBRACE */
    { NULL,             NULL,           PREC_NONE },            /* TOK_COMMA */
    { NULL,             NULL,           PREC_NONE },            /* TOK_DOT */
    { NULL,             NULL,           PREC_NONE },            /* TOK_SEMICOLON */
    { parseGrouping,    parseBinary,    PREC_ADDATIVE },        /* TOK_MINUS */
    { NULL,             parseBinary,    PREC_ADDATIVE },        /* TOK_PLUS */
    { NULL,             parseBinary,    PREC_MULTIPLICATIVE },  /* TOK_SLASH */
    { NULL,             parseBinary,    PREC_MULTIPLICATIVE },  /* TOK_STAR */
    { NULL,             NULL,           PREC_NONE },            /* TOK_EXPO */
    { NULL,             NULL,           PREC_NONE },            /* TOK_PERCENT */
    { NULL,             NULL,           PREC_NONE },            /* TOK_EQUAL */
    { NULL,             NULL,           PREC_NONE },            /* TOK_GREATER */
    { NULL,             NULL,           PREC_NONE },            /* TOK_LESS */
    

    /* Multi Char Tokens */
    { NULL,             NULL,           PREC_NONE },        /* TOK_GTR_EQU */
    { NULL,             NULL,           PREC_NONE },        /* TOK_LESS_EQU */
    { NULL,             NULL,           PREC_NONE },        /* TOK_BANG_EQU */
    { NULL,             NULL,           PREC_NONE }         /* TOK_EQU_EQU */
};


/*
 * ==================================================
 * Function Definitions
 * ==================================================
 */


static Chunk* currentChunk() {
    return compilingChunk;
}


static void advanceParser() {
    /* Move the current token to now be the previous one */
    parser.previous = parser.current;


    /* iterate through tokens until we cannot anymore */
    for (;;) {
        parser.current = tulaLex_nextToken(parser.lexer);
        if (parser.current.type != TOK_ERROR) break;

        errorAtCurrent(parser.current.start);
    }
}


static void advanceParserIf(TokenType type, const char* message) {
    if (parser.current.type == type) {
        advanceParser(parser);
        return;
    }

    errorAtCurrent(message);
}


static void endParser() {
    emitReturn();

#ifdef TULA_DEBUGGING
    if (!parser.hadError) {
        debug_disassembleChunk(currentChunk(), "code");
    }
#endif
}


static void errorAt(Token* token, const char* message) {
    if (parser.panicMode) return;
    parser.panicMode = TRUE;

    fprintf(
        stderr,
        "Encountered a compile error:\n[line %d] Error",
        token->line
    );


    if (token->type == TOK_EOF) {
        fprintf(stderr, " at end");
    } else if (token->type == TOK_ERROR) {
        /* Nothing? I guess for now maybe. :shrug: */
    } else {
        fprintf(stderr, " at '%.*s'", token->length, token->start);
    }

    fprintf(stderr, ": %s\n", message);
    parser.hadError = TRUE;
}


static void errorAtCurrent(const char* message) {
    errorAt(&(parser.current), message);
}


static void error(const char* message) {
    errorAt(&(parser.previous), message);
}


static Byte makeConstant(Value value) {
    Int16 constant = tulaChk_addConst(currentChunk(), value);
    if (constant > INT_MAX) {
        error("Too many constants in one chunk");
        return 0;
    }

    return (Byte)constant;
}


static void emitByte(Byte byte) {
    tulaChk_write(currentChunk(), byte, parser.previous.line);
}


static void emitBytes(Byte byte1, Byte byte2) {
    emitByte(byte1);
    emitByte(byte2);
}


static void emitReturn() {
    emitByte(OP_RETURN);
}


static void emitConstant(Value value) {
    emitBytes(OP_CONSTANT, makeConstant(value));
}


static ParseRule* getRule(TokenType type) {
    return &(PARSE_RULES[type]);
}


static void parsePrecedence(Precedence precedence) {
    ParseFn prefixRule;

    advanceParser();
    prefixRule = getRule(parser.previous.type)->prefix;
    if (prefixRule == NULL) {
        error("Expected expression");
        return;
    }

    prefixRule();


    while (precedence <= getRule(parser.current.type)->precedence) {
        ParseFn infixRule;
        
        advanceParser();
        infixRule = getRule(parser.previous.type)->infix;
        infixRule();
    }
}


static void parseExpression() {
    parsePrecedence(PREC_ASSIGNMENT);
}


static void parseGrouping() {
    parseExpression();
    advanceParserIf(TOK_R_PAREN, "Expect ')' after expression");
}


static void parseUnary() {
    TokenType operatorType = parser.previous.type;
    

    /* Compile the operand */
    parsePrecedence(PREC_UNARY);


    /* Emit the operator instruction */
    switch (operatorType) {
        case TOK_NOT: emitByte(OP_NOT); break;
        case TOK_MINUS: emitByte(OP_NEGATE); break;
        default: return; /* Unreachable */
    }
}


static void parseBinary() {
    TokenType operatorType = parser.previous.type;
    ParseRule* rule = getRule(operatorType);
    parsePrecedence((Precedence)(rule->precedence + 1));

    switch (operatorType) {
        case TOK_PLUS: emitByte(OP_ADD); break;
        case TOK_MINUS: emitByte(OP_SUBTRACT); break;
        case TOK_STAR: emitByte(OP_MULTIPLY); break;
        case TOK_SLASH: emitByte(OP_DIVIDE); break;
        default: return; /* Unreachable */
    }
}


static void parseLiteral() {
    switch (parser.previous.type) {
        case TOK_FALSE: emitByte(OP_FALSE); break;
        case TOK_NULL: emitByte(OP_NULL); break;
        case TOK_TRUE: emitByte(OP_TRUE); break;
        default: return; /* Unreachable */
    }
}


static void parseNumber() {
    double value = strtod(parser.previous.start, NULL);
    emitConstant(NUMBER_VAL(value));
}


TULA_FUNC Bool tula_parseSource(const char* source, Chunk* chunk) {
    /* Initialize the lexer */
    parser.lexer = tulaLex_new(source);
    compilingChunk = chunk;


    /* Walk the lexer */
    advanceParser();
    parseExpression();
    advanceParserIf(TOK_EOF, "Expected end of expression");

    endParser();

    return !parser.hadError;
}