#include "platform.h"
#include "common.h"

#include <stdlib.h>

#ifdef TULA_OS_WINDOWS

#include <windows.h>

#endif /* TULA_OS_WINDOWS */


#ifdef TULA_OS_POSIX_COMPLIANT

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#endif /* TULA_OS_POSIX_COMPLIANT */


#ifdef TULA_OS_MAC

#endif /* TULA_OS_MAC */


#ifdef TULA_OS_LINUX

#endif /* TULA_OS_LINUX */


#ifdef TULA_OS_UNIX

#endif /* TULA_OS_UNIX */


/*
 * ==================================================
 * Macros
 * ==================================================
 */

/* #define XYZ "ABC" */

/*
 * ==================================================
 * Typedefs & Prototypes
 * ==================================================
 */

/**
 * TBD
 */
/* static void example(); */


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

TULA_FUNC Bool tulaPlat_pathExists(const char* path) {
#if defined(TULA_OS_WINDOWS)
    DWORD fileAttributes;

    if (path == NULL) return FALSE;


    /* Get the file attributes */
    fileAttributes = GetFileAttributesA(path);
    if (fileAttributes == INVALID_FILE_ATTRIBUTES) return FALSE;

    return TRUE;
#elif defined(TULA_OS_POSIX_COMPLIANT)
    struct stat fileStat;

    if (path == NULL) return FALSE;

    
    /* Try to stat the file */
    if (stat(path, &fileStat) != 0) return FALSE;

    return TRUE;
#else
    #error Unsupported platform, \
        no compliant implementation of function 'tulaPlat_fileExists'.
#endif
}


TULA_FUNC Bool tulaPlat_isFile(const char* path) {
#if defined(TULA_OS_WINDOWS)
    DWORD fileAttributes;

    if (path == NULL) return FALSE;


    /* Get the file attributes */
    fileAttributes = GetFileAttributesA(path);
    if (fileAttributes == INVALID_FILE_ATTRIBUTES) return FALSE;


    /* Check if the path is a directory */
    if (fileAttributes & FILE_ATTRIBUTE_DIRECTORY) return FALSE;

    return TRUE;
#elif defined(TULA_OS_POSIX_COMPLIANT)
    struct stat fileStat;

    if (path == NULL) return FALSE;

    
    /* Try to stat the file */
    if (stat(path, &fileStat) != 0) return FALSE;


    /* Check if the path is a file */
    return S_ISREG(fileStat.st_mode);
    /* return (fileStat.st_mode & S_IFMT) == S_IFREG;*/
#else
    #error Unsupported platform, \
        no compliant implementation of function 'tulaPlat_fileExists'.
#endif
}