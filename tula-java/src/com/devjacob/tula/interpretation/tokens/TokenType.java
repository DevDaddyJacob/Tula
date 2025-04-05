package com.devjacob.tula.interpretation.tokens;

public enum TokenType {
    // Single-character tokens
    LEFT_PAREN,
    RIGHT_PAREN,
    LEFT_BRACE,
    RIGHT_BRACE,
    COMMA,
    DOT,
    MINUS,
    PLUS,
    SEMICOLON,
    SLASH,
    STAR,
    
    // 1-3 character tokens
    BANG,
    BANG_EQUAL,
    BANG_EQUAL_EQUAL,
    EQUAL, 
    EQUAL_EQUAL,
    EQUAL_EQUAL_EQUAL,
    GREATER, 
    GREATER_EQUAL,
    LESS, 
    LESS_EQUAL,

    // Literals.
    IDENTIFIER,
    STRING,
    NUMBER,
  
    // Keywords.
    TRUE,
    FALSE,
    OR,
    AND,
    NIL,
    CLASS,
    IF,
    ELSE,
    FUNC,
    FOR,
    PRINT,
    RETURN,
    SUPER,
    THIS,
    VAR,
    WHILE,
  
    EOF
}
