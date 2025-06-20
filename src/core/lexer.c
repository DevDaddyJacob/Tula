#include <stdlib.h>
#include <string.h>

#include "lexer.h"
#include "../common.h"
#include "../utils/io.h"

/*
 * ==================================================
 * Macros
 * ==================================================
 */

#if !defined(CHAR_IS_IDENT_ALPHA)
    #define CHAR_IS_IDENT_ALPHA(c) \
        (c == '_' || CHAR_IS_ALPHA(c))
#endif

/*
 * ==================================================
 * Typedefs & Prototypes
 * ==================================================
 */

/**
 * \brief           Creates from the lexer the given token type
 * \param[in]       lexer: Pointer to the lexer to use
 * \param[in]       type: The type of token to make
 * \return          Returns the created token
 */
static Token makeToken(Lexer* lexer, TokenType type);


/**
 * \brief           Creates from the lexer the an error token
 * \param[in]       lexer: Pointer to the lexer to use
 * \param[in]       message: The error message
 * \return          Returns the created error token
 */
static Token errorToken(Lexer* lexer, const char* message);


/**
 * \brief           Conditionally consumes the current character if it matches
 *                  a given character
 * \note            Output is a boolean treated integer
 * \param[in]       lexer: Pointer to a lexer
 * \param[in]       expected: The expected character to consume if seen
 * \return          Returns 1 if the current character is the expected one, 
 *                  and 0 otherwise
 */
static Bool readIf(Lexer* lexer, char expected);


/**
 * \brief           Walks the reader until the next token is not whitespace
 * \param[in]       lexer: Pointer to a lexer
 */
static void skipWhitespace(Lexer* lexer);


/**
 * \brief           Used to test if the rest of a lexeme matches the provided
 *                  string
 * \param[in]       lexer: Pointer to a lexer
 * \param[in]       start: How many characters into the lexeme to start 
 *                  checking from
 * \param[in]       length: The length of the remaining characters
 * \param[in]       rest: The remaining characters to check for
 * \param[in]       type: The type of the token to return if a match is found
 * \return          Returns the type param if there is a match, otherwise
 *                  TOKEN_IDENTIFIER is returned
 */
static TokenType checkKeyword(
    Lexer* lexer, 
    Int16 start, 
    Int16 length, 
    const char* rest, 
    TokenType type
);


/**
 * \brief           Walks the lexeme and checks if it is a reserved word
 * \param[in]       lexer: Pointer to a lexer
 * \return          Returns the token type if there is a match, otherwise
 *                  TOKEN_IDENTIFIER is returned
 */
static TokenType identifierType(Lexer* lexer);


/**
 * \brief           Scans the rest of a identifier
 * \param[in]       lexer: Pointer to a lexer
 * \return          Returns the created token for the identifier
 */
static Token scanIdentifier(Lexer* lexer);


/**
 * \brief           Scans the rest of a number
 * \param[in]       lexer: Pointer to a lexer
 * \return          Returns the created number token
 */
static Token scanNumber(Lexer* lexer);


/**
 * \brief           Scans the rest of a string
 * \param[in]       lexer: Pointer to a lexer
 * \return          Returns the created string token
 */
static Token scanString(Lexer* lexer);


/*
 * ==================================================
 * Module Level Variables & Constants
 * ==================================================
 */

/* int magicNumber = 420; */


/*
 * ==================================================
 * Function Definitions
 * ==================================================
 */

static Token makeToken(Lexer* lexer, TokenType type) {
    Token token;
    token.type = type;
    token.start = lexer->curLexStart;
    token.length = (Int16)(lexer->reader->next - lexer->curLexStart);
    token.line = lexer->line;
    return token;
}


static Token errorToken(Lexer* lexer, const char* message) {
    Token token;
    token.type = TOK_ERROR;
    token.start = message;
    token.length = (Int16)strlen(message);
    token.line = lexer->line;
    return token;
}


static Bool readIf(Lexer* lexer, char expected) {
    if (!tulaIo_hasNext(lexer->reader)) return FALSE;
    if (tulaIo_peek(lexer->reader) != expected) return FALSE;
    tulaIo_comsume(lexer->reader);
    return TRUE;
}


static void skipWhitespace(Lexer* lexer) {
    while (tulaIo_hasNext(lexer->reader)) {
        switch (tulaIo_peek(lexer->reader)) {
            case ' ':
            case '\r':
            case '\t': {
                tulaIo_comsume(lexer->reader);
                break;
            }

            case '\n': {
                lexer->line++;
                tulaIo_comsume(lexer->reader);
                break;
            }

            case '/': {
                char* next = tulaIo_peekN(lexer->reader, 1);
                
                if (next == NULL || *next != '/') return;
                
                /* In event of a //, eat the entire line as honorary whitespace */
                while (
                    tulaIo_hasNext(lexer->reader) 
                    && tulaIo_peek(lexer->reader) != '\n'
                ) {
                    tulaIo_comsume(lexer->reader);
                }
            }
            
            default: return;
        }
    }
}


static TokenType checkKeyword(
    Lexer* lexer, 
    Int16 start, 
    Int16 length, 
    const char* rest, 
    TokenType type
) {
    if (
        lexer->reader->next - lexer->curLexStart == start + length
        && memcmp(lexer->curLexStart + start, rest, length) == 0
    ) {
        return type;
    }

    return TOK_IDENTIFIER;
}


static TokenType identifierType(Lexer* lexer) {
    switch (lexer->curLexStart[0]) {
        case 'a': return checkKeyword(lexer, 1, 2, "nd", TOK_AND);
        case 'b': return checkKeyword(lexer, 1, 4, "reak", TOK_BREAK);
        case 'd': return checkKeyword(lexer, 1, 1, "o", TOK_DO);
        case 'f': {
            if (lexer->reader->next - lexer->curLexStart <= 1) break;
            switch (lexer->curLexStart[1]) {
                case 'a': return checkKeyword(lexer, 2, 3, "lse", TOK_FALSE);
                case 'o': return checkKeyword(lexer, 2, 1, "r", TOK_FOR);
                case 'u': return checkKeyword(lexer, 2, 6, "nction", TOK_FUNC);
            }
            break;
        }
        case 'i': {
            if (lexer->reader->next - lexer->curLexStart <= 1) break;
            switch (lexer->curLexStart[1]) {
                case 'f': return TOK_IF;
                case 'm': return checkKeyword(lexer, 2, 4, "port", TOK_IMPORT);
            }
            break;
        }
        case 'n': {
            if (lexer->reader->next - lexer->curLexStart <= 1) break;
            switch (lexer->curLexStart[1]) {
                case 'u': return checkKeyword(lexer, 2, 2, "ll", TOK_NULL);
                case 'o': return checkKeyword(lexer, 2, 1, "t", TOK_NOT);
            }
            break;
        }
        case 'o': return checkKeyword(lexer, 1, 1, "r", TOK_OR);
        case 'r': return checkKeyword(lexer, 1, 5, "eturn", TOK_RETURN);
        case 't': {
            if (lexer->reader->next - lexer->curLexStart <= 1) break;
            switch (lexer->curLexStart[1]) {
                case 'h': return checkKeyword(lexer, 2, 2, "en", TOK_THEN);
                case 'r': return checkKeyword(lexer, 2, 2, "ue", TOK_TRUE);
            }
            break;
        }
        case 'v': return checkKeyword(lexer, 1, 2, "ar", TOK_VAR);
        case 'w': return checkKeyword(lexer, 1, 4, "hile", TOK_WHILE);
    }

    return TOK_IDENTIFIER;
}


static Token scanIdentifier(Lexer* lexer) {
    while (
        CHAR_IS_IDENT_ALPHA(tulaIo_peek(lexer->reader)) 
        || CHAR_IS_DIGIT(tulaIo_peek(lexer->reader)) 
    ) tulaIo_comsume(lexer->reader);

    return makeToken(lexer, identifierType(lexer));
}


static Token scanNumber(Lexer* lexer) {
    while (CHAR_IS_DIGIT(tulaIo_peek(lexer->reader))) {
        tulaIo_comsume(lexer->reader);
    }

    /* If we have a fractional part / have a decimal, consume it properly */
    if (
        tulaIo_peek(lexer->reader) == '.' 
        && CHAR_IS_DIGIT(tulaIo_peekN(lexer->reader, 1))
    ) {
        tulaIo_comsume(lexer->reader);
        while (CHAR_IS_DIGIT(tulaIo_peek(lexer->reader))) {
            tulaIo_comsume(lexer->reader);
        }
    }

    return makeToken(lexer, TOK_NUMBER);
}


static Token scanString(Lexer* lexer) {
    /* Keep scanning until we hit a string terminator, or the end of file */
    while (tulaIo_peek(lexer->reader) != '"' && tulaIo_hasNext(lexer->reader)) {
        if (tulaIo_peek(lexer->reader) == '\n') lexer->line++;

        tulaIo_comsume(lexer->reader);
    }

    if (!tulaIo_hasNext(lexer->reader)) {
        return errorToken(lexer, "Unterminated string");
    }

    /* Consume the terminating quote */
    tulaIo_comsume(lexer->reader);
    return makeToken(lexer, TOK_STRING);
}


TULA_FUNC Lexer* tulaLex_new(const char* source) {
    Lexer* lexer;

    /* Allocate the memory for the lexer */
    lexer = (Lexer*)malloc(sizeof(Lexer));
    if (lexer == NULL) return NULL;


    /* Open the buffer reader for the source */
    lexer->reader = tulaIo_openBuffer(source);
    if (lexer == NULL) {
        free(lexer);
        return NULL;
    }


    /* Prime the other variables */
    lexer->curLexStart = lexer->reader->start;
    lexer->line = 1;

    return lexer;
}


TULA_FUNC void tulaLex_destroy(Lexer* lexer) {
    if (lexer == NULL) return;

    /* Close the reader first */
    tulaIo_closeBuffer(lexer->reader);


    /* Release the whole lexer */
    free(lexer);
}


TULA_FUNC Token tulaLex_nextToken(Lexer* lexer) {
    char c;

    /* Eat any whitespace, and then reset the lexeme start */
    skipWhitespace(lexer);
    lexer->curLexStart = lexer->reader->next;


    /* If we are at the end of the buffer, return a EOF */
    if (!tulaIo_hasNext(lexer->reader)) return makeToken(lexer, TOK_EOF);

    
    /* Read the next character and decide what to do with it */
    c = tulaIo_read(lexer->reader);
    if (CHAR_IS_IDENT_ALPHA(c)) return scanIdentifier(lexer);
    if (CHAR_IS_DIGIT(c)) return scanNumber(lexer);

    switch (c) {
        case '\0': return makeToken(lexer, TOK_EOF);
        case '"': return scanString(lexer);
        case '(': return makeToken(lexer, TOK_L_PAREN);
        case ')': return makeToken(lexer, TOK_R_PAREN);
        case '[': return makeToken(lexer, TOK_L_BRACE);
        case ']': return makeToken(lexer, TOK_R_BRACE);
        case '{': return makeToken(lexer, TOK_L_CBRACE);
        case '}': return makeToken(lexer, TOK_R_CBRACE);
        case ',': return makeToken(lexer, TOK_COMMA);
        case '.': return makeToken(lexer, TOK_DOT);
        case ';': return makeToken(lexer, TOK_SEMICOLON);
        case '-': return makeToken(lexer, TOK_MINUS);
        case '+': return makeToken(lexer, TOK_PLUS);
        case '/': return makeToken(lexer, TOK_SLASH);
        case '*': return makeToken(lexer, TOK_STAR);
        case '^': return makeToken(lexer, TOK_EXPO);
        case '%': return makeToken(lexer, TOK_PERCENT);
        case '=': {
            if (readIf(lexer, '=')) return makeToken(lexer, TOK_EQU_EQU);
            return makeToken(lexer, TOK_EQUAL);
        }
        case '>': {
            if (readIf(lexer, '=')) return makeToken(lexer, TOK_GTR_EQU);
            return makeToken(lexer, TOK_GREATER);
        }
        case '<': {
            if (readIf(lexer, '=')) return makeToken(lexer, TOK_LESS_EQU);
            return makeToken(lexer, TOK_LESS);
        }
        case '!': {
            if (readIf(lexer, '=')) return makeToken(lexer, TOK_BANG_EQU);
            break;
        }
    }
    
    
    /* Just a fallback incase for some reason we make it here */
    return errorToken(lexer, "Unexpected character");
}
