#ifndef tulac_scanner_h
#define tulac_scanner_h

typedef enum {
    /* Literals */
    TOKEN_IDENTIFIER,
    TOKEN_STRING,
    TOKEN_NUMBER,
    
    /* Single Char Tokens*/
    TOKEN_LEFT_PAREN,       /** ( */
    TOKEN_RIGHT_PAREN,      /** ) */
    TOKEN_LEFT_BRACE,       /** [ */
    TOKEN_RIGHT_BRACE,      /** ] */
    TOKEN_COMMA,            /** , */
    TOKEN_DOT,              /** . */
    TOKEN_SEMICOLON,        /** ; */
    TOKEN_MINUS,            /** - */
    TOKEN_PLUS,             /** + */
    TOKEN_SLASH,            /** / */
    TOKEN_STAR,             /** * */
    TOKEN_BANG,             /** ! */
    TOKEN_EQUAL,            /** = */
    TOKEN_GREATER,          /** > */
    TOKEN_LESS,             /** < */
    
    /* Multi Char Tokens */
    TOKEN_GREATER_EQUAL,        /** >= */
    TOKEN_LESS_EQUAL,           /** <= */
    TOKEN_BANG_EQUAL,           /** != */
    TOKEN_EQUAL_EQUAL,          /** == */
    TOKEN_BANG_EQUAL_EQUAL,     /** !== */
    TOKEN_EQUAL_EQUAL_EQUAL,    /** === */
    
    /* Keywords */
    TOKEN_PRINT,    /** print */
    TOKEN_RETURN,   /** return */
    TOKEN_FUNC,     /** func */
    TOKEN_CLASS,    /** class */
    TOKEN_SUPER,    /** super */
    TOKEN_THIS,     /** this */
    TOKEN_VAR,      /** var */
    TOKEN_NIL,      /** nil */
    TOKEN_FALSE,    /** false */
    TOKEN_TRUE,     /** true */
    TOKEN_AND,      /** and */
    TOKEN_OR,       /** or */
    TOKEN_IF,       /** if */
    TOKEN_ELSE,     /** else */
    TOKEN_FOR,      /** for */
    TOKEN_WHILE,    /** while */
    
    /* Other */
    TOKEN_ERROR,
    TOKEN_EOF,
} TokenType;


typedef struct {
    TokenType type;     /** The token's type */
    const char* start;  /** Pointer to the start of the token */
    int length;         /** The length of the token */
    int line;           /** The line the token is found on */
} Token;


typedef struct {
    const char* start;      /** Beginning of the current lexeme being scanned */
    const char* current;    /** Current character being read */
    int line;               /** The line the current lexeme is on */
} Scanner;


/**
 * \brief           Initializes the provided scanner
 * \param[in]       scanner: Pointer of the scanner to initialize
 * \param[in]       source: Pointer to the first character on the first line
 */
void tula_initScanner(Scanner* scanner, const char* source);

/**
 * \brief           Walks the scanner scanning the next token
 * \param[in]       scanner: Pointer of the scanner to walk
 * \return          Returns the created token
 */
Token tula_scanToken(Scanner* scanner);

#endif /* tulac_scanner_h */