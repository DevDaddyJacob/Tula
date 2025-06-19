#ifndef tula_core_lexer_h
#define tula_core_lexer_h

#include "../common.h"
#include "../utils/io.h"

typedef enum tula_TokenType {
    /* Literals */
    TOK_EOF,
    TOK_ERROR,
    TOK_IDENTIFIER,
    TOK_BOOLEAN,
    TOK_STRING,
    TOK_NUMBER,
    TOK_NULL,       /** null */

    /* Keywords */
    TOK_AND,        /** and */
    TOK_BREAK,      /** break */
    TOK_DO,         /** do */
    TOK_ELSE,       /** else */
    TOK_FALSE,      /** false */
    TOK_FOR,        /** for */
    TOK_FUNC,       /** function */
    TOK_IF,         /** if */
    TOK_IMPORT,     /** import */
    TOK_NOT,        /** not */
    TOK_OR,         /** or */
    TOK_RETURN,     /** return */
    TOK_THEN,       /** then */
    TOK_TRUE,       /** true */
    TOK_VAR,        /** var */
    TOK_WHILE,      /** while */
    
    /* Single Char Tokens*/
    TOK_L_PAREN,        /** ( */
    TOK_R_PAREN,        /** ) */
    TOK_L_BRACE,        /** [ */
    TOK_R_BRACE,        /** ] */
    TOK_L_CBRACE,       /** { */
    TOK_R_CBRACE,       /** } */
    TOK_COMMA,          /** , */
    TOK_DOT,            /** . */
    TOK_SEMICOLON,      /** ; */
    TOK_MINUS,          /** - */
    TOK_PLUS,           /** + */
    TOK_SLASH,          /** / */
    TOK_STAR,           /** * */
    TOK_EXPO,           /** ^ */
    TOK_PERCENT,        /** % */
    TOK_EQUAL,          /** = */
    TOK_GREATER,        /** > */
    TOK_LESS,           /** < */
    
    /* Multi Char Tokens */
    TOK_GTR_EQU,        /** >= */
    TOK_LESS_EQU,       /** <= */
    TOK_BANG_EQU,       /** != */
    TOK_EQU_EQU         /** == */
} TokenType;


typedef struct tula_Token {
    TokenType type;     /** The token's type */
    const char* start;  /** Pointer to the start of the token */
    Int16 length;       /** The length of the token */
    Int16 line;         /** The line the token is found on */
} Token;


typedef struct tula_Lexer {
    BufferReader* reader;       /** The buffer reader with the data to read */
    Int16 line;                 /** The line the current lexeme is on */
    const char* curLexStart;    /** Pointer to the start of the current lexeme */
} Lexer;


TULA_FUNC Lexer* tulaLex_new(const char* source);


TULA_FUNC void tulaLex_destroy(Lexer* lexer);


TULA_FUNC Token tulaLex_nextToken(Lexer* lexer);


#endif /* tula_core_lexer_h */