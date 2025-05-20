#ifndef tulac_conf_h
#define tulac_conf_h

/*
 * ============================================================================
 * Configure and define the build mode macros and tools
 * ============================================================================
 */

#ifndef TULA_BUILD
    #warning "TULA_BUILD is not specified, defaulting to 0 (debug)"
    #define BUILD_MODE 0
#elif TULA_BUILD == 0
    #define BUILD_MODE 0
#elif TULA_BUILD == 1
    #define BUILD_MODE 1
#else
    #error "TULA_BUILD is assigned an invalid value, must be 0 for debug, or 1 for release"
#endif

#if BUILD_MODE == 0
    #define TULA_DEBUG
#endif

#define IS_DEBUG_ENABLED (BUILD_MODE == 0)

/* ========================================================================= */

/**
 * The number by which array sizes is multiplied by when increasing their size
 * For example, if an array has 16 elements, and we are wanting to increase the 
 * size, we will multiply 16 by the value of TULA_ARRAY_GROW_FACTOR
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

#endif /* tulac_conf_h */