#ifndef tula_core_errors_h
#define tula_core_errors_h

#include "lexer.h"
#include "../tula.h"
#include "../common.h"


extern const char* PROG_NAME;


TULA_FUNC void tulaErr_raw(
    const char* errType,
    const char* errMsg,
    const char* errStackTrace,
    const char* fileName,
    Int16 fileLineNum,
    const char* fileLine,
    Int16 sourceCharIndex
);


TULA_FUNC void tulaErr_parseError(
    Token* token,
    const char* errMsg,
    const char* errType,
    const char* errStackTrace,
    const char* fileName,
    const char* fileLine
);

#endif /* tula_core_errors_h */