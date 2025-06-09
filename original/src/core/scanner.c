#include <stdio.h>
#include <string.h>

#include "scanner.h"
#include "../common.h"

/*
 * ==================================================
 * Typedefs & Prototypes
 * ==================================================
 */

/**
 * \brief           Creates from the scanner the given token type
 * \param[in]       scanner: Pointer to the scanner to use
 * \param[in]       type: The type of token to make
 * \return          Returns the created token
 */
static Token makeToken(Scanner* scanner, TokenType type);


/**
 * \brief           Creates from the scanner the an error token
 * \param[in]       scanner: Pointer to the scanner to use
 * \param[in]       message: The error message
 * \return          Returns the created error token
 */
static Token errorToken(Scanner* scanner, const char* message);


/**
 * \brief           Checks if a character is alphabetical
 * \note            Output is a boolean treated integer
 * \param[in]       c: The character to check
 * \return          Returns 1 if the character is alphabetical, and 0 otherwise
 */
static BOOL isAplha(char c);


/**
 * \brief           Checks if a character is a digit
 * \note            Output is a boolean treated integer
 * \param[in]       c: The character to check
 * \return          Returns 1 if the character is a digit, and 0 otherwise
 */
static BOOL isDigit(char c);


/**
 * \brief           Checks if the provided scanner at at the end of a line
 * \note            Output is a boolean treated integer
 * \param[in]       scanner: Pointer to a scanner
 * \return          Returns 1 if the scanner is at the end of the line, 
 *                  and 0 otherwise
 */
static BOOL isAtEnd(Scanner* scanner);


/**
 * \brief           Consumes the current character and returns it
 * \param[in]       scanner: Pointer to a scanner
 * \return          Returns the current character
 */
static char advance(Scanner* scanner);


/**
 * \brief           Returns the current character without consuiming it
 * \param[in]       scanner: Pointer to a scanner
 * \return          Returns the current character
 */
static char peek(Scanner* scanner);


/**
 * \brief           Returns the next character without consuiming characters
 * \param[in]       scanner: Pointer to a scanner
 * \return          Returns the next character
 */
static char peekNext(Scanner* scanner);


/**
 * \brief           Conditionally consumes the current character if it matches
 *                  a given character
 * \note            Output is a boolean treated integer
 * \param[in]       scanner: Pointer to a scanner
 * \param[in]       expected: The expected character to consume if seen
 * \return          Returns 1 if the current character is the expected one, 
 *                  and 0 otherwise
 */
static BOOL match(Scanner* scanner, char expected);


/**
 * \brief           Walks the scanner until the next token is not whitespace
 * \param[in]       scanner: Pointer to a scanner
 */
static void skipWhitespace(Scanner* scanner);


/**
 * \brief           Used to test the rest of a potential keyword
 * \param[in]       scanner: Pointer to a scanner
 * \param[in]       start: How many characters into the lexeme to start 
 *                  checking from
 * \param[in]       length: The length of the remaining characters
 * \param[in]       rest: The remaining characters to check for
 * \param[in]       type: The type of the token to return if a match is found
 * \return          Returns the type param if there is a match, otherwise
 *                  TOKEN_IDENTIFIER is returned
 */
static TokenType checkKeyword(
    Scanner* scanner, 
    int start, 
    int length, 
    const char* rest, 
    TokenType type
);


/**
 * \brief           Walks the lexeme and checks if it is a reserved word
 * \param[in]       scanner: Pointer to a scanner
 * \return          Returns the token type if there is a match, otherwise
 *                  TOKEN_IDENTIFIER is returned
 */
static TokenType identifierType(Scanner* scanner);


/**
 * \brief           Scans the rest of a identifier
 * \param[in]       scanner: Pointer to a scanner
 * \return          Returns the created token for the identifier
 */
static Token scanIdentifier(Scanner* scanner);


/**
 * \brief           Scans the rest of a number
 * \param[in]       scanner: Pointer to a scanner
 * \return          Returns the created number token
 */
static Token scanNumber(Scanner* scanner);


/**
 * \brief           Scans the rest of a string
 * \param[in]       scanner: Pointer to a scanner
 * \return          Returns the created string token
 */
static Token scanString(Scanner* scanner);


/*
 * ==================================================
 * Function Definitions
 * ==================================================
 */

static Token makeToken(Scanner* scanner, TokenType type) {
    Token token;
    token.type = type;
    token.start = scanner->start;
    token.length = (int8_t)(scanner->current - scanner->start);
    token.line = scanner->line;
    return token;
}


static Token errorToken(Scanner* scanner, const char* message) {
    Token token;
    token.type = TOKEN_ERROR;
    token.start = message;
    token.length = (int8_t)strlen(message);
    token.line = scanner->line;
    return token;
}


static BOOL isAplha(char c) {
    if (
        (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        c == '_'
    ) return TRUE;
    else return FALSE;
}


static BOOL isDigit(char c) {
    if (c >= '0' && c <= '9') return TRUE;
    else return FALSE;
}


static BOOL isAtEnd(Scanner* scanner) {
    if (*(scanner->current) == '\0') return TRUE;
    else return FALSE;
}


static char advance(Scanner* scanner) {
    scanner->current++;
    return scanner->current[-1];
}


static char peek(Scanner* scanner) {
    return *(scanner->current);
}


static char peekNext(Scanner* scanner) {
    if (isAtEnd(scanner)) return '\0';
    return scanner->current[1];
}


static BOOL match(Scanner* scanner, char expected) {
    if (isAtEnd(scanner)) return FALSE;
    if (*(scanner->current) != expected) return FALSE;
    scanner->current++;
    return TRUE;
}


static void skipWhitespace(Scanner* scanner) {
    for (;;) {
        char c = peek(scanner);

        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                advance(scanner);
                break;

            case '\n':
                scanner->line++;
                advance(scanner);
                break;

            case '/':
                /* 
                 * If the next char is another slash (//) this is a comment and 
                 * we should walk it to the end of the line
                 */
                if (peekNext(scanner) == '/') {
                    while (peek(scanner) != '\n' && !isAtEnd(scanner)) {
                        advance(scanner);
                    }
                } else {
                    return;
                }
            
            default:
                return;
        }
    }
}


static TokenType checkKeyword(
    Scanner* scanner, 
    int start, 
    int length, 
    const char* rest, 
    TokenType type
) {
    if (
        scanner->current - scanner->start == start + length
        && memcmp(scanner->start + start, rest, length) == 0
    ) {
        return type;
    }

    return TOKEN_IDENTIFIER;
}


static TokenType identifierType(Scanner* scanner) {
    switch (scanner->start[0]) {
        case 'a': return checkKeyword(scanner, 1, 2, "nd", TOKEN_AND);
        case 'c': return checkKeyword(scanner, 1, 4, "lass", TOKEN_CLASS);
        case 'e': return checkKeyword(scanner, 1, 3, "lse", TOKEN_ELSE);
        case 'i': return checkKeyword(scanner, 1, 1, "f", TOKEN_IF);
        case 'n': return checkKeyword(scanner, 1, 2, "il", TOKEN_NIL);
        case 'o': return checkKeyword(scanner, 1, 1, "r", TOKEN_OR);
        case 'p': return checkKeyword(scanner, 1, 4, "rint", TOKEN_PRINT);
        case 'r': return checkKeyword(scanner, 1, 5, "eturn", TOKEN_RETURN);
        case 's': return checkKeyword(scanner, 1, 4, "uper", TOKEN_SUPER);
        case 'v': return checkKeyword(scanner, 1, 2, "ar", TOKEN_VAR);
        case 'w': return checkKeyword(scanner, 1, 4, "hile", TOKEN_WHILE);

        case 'f': 
            if (scanner->current - scanner->start > 1) {
                switch (scanner->start[1]) {
                    case 'a': 
                        return checkKeyword(scanner, 2, 3, "lse", TOKEN_FALSE);
                    case 'o':
                        return checkKeyword(scanner, 2, 1, "r", TOKEN_FOR);
                    case 'u':
                        return checkKeyword(scanner, 2, 2, "nc", TOKEN_FUNC);
                }
            }
            break;

        case 't':
            if (scanner->current - scanner->start > 1) {
                switch (scanner->start[1]) {
                    case 'h': 
                        return checkKeyword(scanner, 2, 2, "is", TOKEN_THIS);
                    case 'r':
                        return checkKeyword(scanner, 2, 2, "ue", TOKEN_TRUE);
                }
            }
            break;
    }

    return TOKEN_IDENTIFIER;
}


static Token scanIdentifier(Scanner* scanner) {
    while (isAplha(peek(scanner)) || isDigit(peek(scanner))) advance(scanner);
    return makeToken(scanner, identifierType(scanner));
}


static Token scanNumber(Scanner* scanner) {
    while (isDigit(peek(scanner))) advance(scanner);

    /* If we have a fractional part / have a decimal, consume it properly */
    if (peek(scanner) == '.' && isDigit(peekNext(scanner))) {
        advance(scanner);
        while (isDigit(peek(scanner))) advance(scanner);
    }

    return makeToken(scanner, TOKEN_NUMBER);
}


static Token scanString(Scanner* scanner) {
    /* Keep scanning until we hit a string terminator, or the end of file */
    while (peek(scanner) != '"' && !isAtEnd(scanner)) {
        if (peek(scanner) == '\n') scanner->line++;
        advance(scanner);
    }

    if (isAtEnd(scanner)) return errorToken(scanner, "Unterminated string");

    /* Consume the terminating quote */
    advance(scanner);
    return makeToken(scanner, TOKEN_STRING);
}


void tula_initScanner(Scanner* scanner, const char* source) {
    scanner->start = source;
    scanner->current = source;
    scanner->line = 1;
}


Token tula_scanToken(Scanner* scanner) {
    char c;
    skipWhitespace(scanner);
    scanner->start = scanner->current;

    if (isAtEnd(scanner)) return makeToken(scanner, TOKEN_EOF);
    c = advance(scanner);

    if (isAplha(c)) return scanIdentifier(scanner);
    if (isDigit(c)) return scanNumber(scanner);

    switch (c) {
        case '"': return scanString(scanner);
        case '(': return makeToken(scanner, TOKEN_LEFT_PAREN);
        case ')': return makeToken(scanner, TOKEN_RIGHT_PAREN);
        case '{': return makeToken(scanner, TOKEN_LEFT_BRACE);
        case '}': return makeToken(scanner, TOKEN_RIGHT_BRACE);
        case ';': return makeToken(scanner, TOKEN_SEMICOLON);
        case ',': return makeToken(scanner, TOKEN_COMMA);
        case '.': return makeToken(scanner, TOKEN_DOT);
        case '-': return makeToken(scanner, TOKEN_MINUS);
        case '+': return makeToken(scanner, TOKEN_PLUS);
        case '/': return makeToken(scanner, TOKEN_SLASH);
        case '*': return makeToken(scanner, TOKEN_STAR);

        case '!':
            if (!match(scanner, '=')) return makeToken(scanner, TOKEN_BANG);
            if (match(scanner, '=')) return makeToken(scanner, TOKEN_BANG_EQUAL_EQUAL);
            return makeToken(scanner, TOKEN_BANG_EQUAL);

        case '=':
            if (!match(scanner, '=')) return makeToken(scanner, TOKEN_EQUAL);
            if (match(scanner, '=')) return makeToken(scanner, TOKEN_EQUAL_EQUAL_EQUAL);
            return makeToken(scanner, TOKEN_EQUAL_EQUAL);
            
        case '<':
            if (match(scanner, '=')) return makeToken(scanner, TOKEN_LESS_EQUAL);
            return makeToken(scanner, TOKEN_LESS);

        case '>':
            if (match(scanner, '=')) return makeToken(scanner, TOKEN_GREATER_EQUAL);
            return makeToken(scanner, TOKEN_GREATER);
    }

    return errorToken(scanner, "Unexpected character");
}
