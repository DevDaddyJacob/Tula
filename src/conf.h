#ifndef tula_conf_h
#define tula_conf_h

/*
 * ============================================================================
 * Development debugging config
 * ============================================================================
 */

/**
 * Used manually if you want to have some extra debugging output, mainly for
 * development work
 */
#define TULA_DEBUGGING

/* ========================================================================= */


/*
 * ============================================================================
 * Configure the C standard and OS
 * ============================================================================
 */

#if defined(TULA_OS_WINDOWS)
    #define _TULA_OS_DEF
    #
#endif


#if defined(TULA_OS_MAC)
    #define TULA_OS_POSIX_COMPLIANT
    #define _TULA_OS_DEF
#endif


#if defined(TULA_OS_LINUX)
    #define TULA_OS_POSIX_COMPLIANT
    #define _TULA_OS_DEF
#endif


#if defined(TULA_OS_UNIX)
    #define TULA_OS_POSIX_COMPLIANT
    #define _TULA_OS_DEF
#endif


#if !defined(_TULA_OS_DEF)
    #if defined(_WIN32) || defined(_WIN64)
        #define TULA_OS_WINDOWS
        #define _TULA_OS_DEF
    
    #elif defined(__APPLE__) && defined(__MACH__)
        #define TULA_OS_MAC
        #define TULA_OS_POSIX_COMPLIANT
        #define _TULA_OS_DEF
    
    #elif defined(__linux__)
        #define TULA_OS_LINUX
        #define TULA_OS_POSIX_COMPLIANT
        #define _TULA_OS_DEF

    #elif defined(__unix__)
        #define TULA_OS_UNIX
        #define TULA_OS_POSIX_COMPLIANT
        #define _TULA_OS_DEF
    
    #else
        #error "Unknown or unsupported platform"
    #endif
#endif



/*
 * Specifies the C standard being used.
 * Defaults to C89
 *
 * Usage:
 *      -DCSTD=89 
 *      -DCSTD=99 
 *      -DCSTD=80 
 */
#if defined(CSTD) && CSTD == 89
    #define TULA_STD_C89
#elif defined(CSTD) && CSTD == 90
    #define TULA_STD_C90
#elif defined(CSTD) && CSTD == 99
    #define TULA_STD_C99
#else
    #define TULA_STD_C89
#endif


#define TULA_IS_INT_32 ((UINT_MAX >> 30) >=3)

/* ========================================================================= */


/**
 * The number by which array sizes is multiplied by when increasing their size
 * For example, if an array has 16 elements, and we are wanting to increase the 
 * size, we will multiply 16 by the value of TULA_ARRAY_GROW_FACTOR
 * 
 * Usage
 */
#define TULA_ARRAY_GROW_FACTOR 2


/**
 * The minimum array size. If we have an minimum of 8 and an array of size 3 
 * and we attempt to resize it, a check will occur to see if the size is lower 
 * than the minimum and if so, rather than multiplying by the grow factor,
 * instead we set the size to the minimum.
 */
#define TULA_ARRAY_MIN_THRESHOLD 8


/**
 * The maximum size the stack can get
 */
#define TULA_STACK_MAX 256


/**
 * When in REPL / interactive mode this is the size of the buffer to use
 */
#define TULA_MAX_INPUT 512

#endif /* tula_conf_h */