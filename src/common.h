#ifndef tula_common_h
#define tula_common_h

#include <limits.h>
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
typedef UInt8           Byte;

typedef signed int      Int16;
typedef unsigned int    UInt16;

typedef signed long     Int32;

/* ========================================================================= */


/*
 * ============================================================================
 * Just here to add some consistiency and readability for boolean-treated ints
 * ============================================================================
 */

typedef UInt8 Bool;
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
#define TULA_EXIT_BUFF_OVERFLOW 3
#define TULA_EXIT_ACCESS_STATE_BEFOR_INIT 4


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


/*
 * ============================================================================
 * Array interactions
 * ============================================================================
 */

#define TULA_ARRAY_GROW_CAPACITY(capacity) \
    ((capacity) < TULA_ARRAY_MIN_THRESHOLD \
        ? TULA_ARRAY_MIN_THRESHOLD \
        : (capacity) * TULA_ARRAY_GROW_FACTOR)


#define TULA_ARRAY_RESIZE(type, pointer, oldCount, newCount) \
    (type*)tula_reallocateArray( \
        pointer, \
        sizeof(type) * (oldCount), \
        sizeof(type) * (newCount) \
    )


#define TULA_ARRAY_FREE(type, pointer, oldCount) \
    tula_reallocateArray(pointer, sizeof(type) * (oldCount), 0)



/**
 * \brief           Reallocates an array to be the new size
 * \note            Designed to be used in the GROW_ARRAY & FREE_ARRAY macros
 * \param[in]       pointer: Pointer to the array to reallocate
 * \param[in]       oldSize: The old size of the array
 * \param[in]       newSize: The new size of the array
 * \return          Returns a pointer to the new array
 */
TULA_FUNC void* tula_reallocateArray(void* pointer, size_t oldSize, size_t newSize);

/* ========================================================================= */


/*
 * ============================================================================
 * Safe functions implementations
 * ============================================================================
 */

/**
 * \brief           Safely copies a string into a destination buffer, at most
 *                  copying size - 1 characters and always ensuring a null
 *                  termination
 * \param[in]       destination: The destination buffer to write into
 * \param[in]       source: The source to copy into the destination
 * \param[in]       destSize: The size of the destination buffer
 * \return          Returns the total number of characters copied including
 *                  the null terminator
 */
TULA_FUNC UInt16 tula_safeStrCpy(
    char* destination,
    const char* source,
    UInt16 destSize
);

/* ========================================================================= */

#endif /* tula_common_h */