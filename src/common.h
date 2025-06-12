#ifndef tula_common_h
#define tula_common_h

#include <stdio.h>
#include <string.h>

#include "conf.h"
#include "tula.h"
#include "utils/uint64.h"


/*
 * ============================================================================
 * Define macros for API functions
 * ============================================================================
 */

/**
 * Marks a core API function
 * (Equivilant of LUA_API)
 */
#define TULA_API extern

/**
 * Marks a auxiliary lib function
 * (Equivilant of LUALIB_API)
 */
#define TULA_AUXLIB_API TULA_API

/**
 * Marks a standard lib function
 * (Equivilant of LUAMOD_API)
 */
#define TULA_STDLIB_API TULA_API

/**
 * Marks a function that are not to be exported tooutside modules
 * (Equivilant of LUAI_FUNC)
 */
#define TULA_FUNC extern


/* ========================================================================= */


/*
 * ============================================================================
 * Define some shorthands to make interacting with numbers a bit better
 * ============================================================================
 */

#define TULA_IS_32BIT_INT ((UINT_MAX >> 30) >= 3)

typedef signed char     Int8;
typedef unsigned char   UInt8;
typedef UInt8           byte;

typedef signed int      Int16;
typedef unsigned int    UInt16;

typedef signed long     Int32;
typedef unsigned long   UInt32;

/* ========================================================================= */


/*
 * ============================================================================
 * Just here to add some consistiency and readability for boolean-treated ints
 * ============================================================================
 */

#define BOOL UInt8
#define TRUE 1
#define FALSE 0

/* ========================================================================= */


/*
 * ============================================================================
 * Program possible exit codes
 * ============================================================================
 */

#define TULA_EXIT_GODD 0

#define TULA_EXIT_BAD_USAGE 1

#define TULA_EXIT_NO_MEM 2


/* ========================================================================= */


/*
 * ============================================================================
 * Console writting tools (Lua inspired)
 * ============================================================================
 */

#if !defined(tula_errPrint)
    #define tula_errPrint(msg) \
        (fprintf(stderr, "tula: Error: %s\n", (msg)), fflush(stderr))
#endif

#if !defined(tula_errPrintFatal)
    #define tula_errPrintFatal(msg) \
        (fprintf(stderr, "tula: Fatal Error: %s\n", (msg)), fflush(stderr))
#endif


/* ========================================================================= */


/*
 * ============================================================================
 * Some simple misc utilities
 * ============================================================================
 */

#if !defined(STR_EQ)
    #define STR_EQ(strA, strB) \
        (strcmp(strA, strB) == 0)
#endif

#if !defined(CHAR_IS_ALPHA)
    #define CHAR_IS_ALPHA(c) \
        ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
#endif

#if !defined(CHAR_IS_DIGIT)
    #define CHAR_IS_DIGIT(c) \
        (c >= '0' && c <= '9')
#endif

/* ========================================================================= */


#endif /* tula_common_h */