#include "platform.h"
#include "common.h"

#include <stdlib.h>
#include <string.h>

#ifdef TULA_OS_WINDOWS

#include <windows.h>

#endif /* TULA_OS_WINDOWS */


#ifdef TULA_OS_POSIX_COMPLIANT

#include <stdio.h>
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

#if defined(TULA_OS_WINDOWS)
    #define PATH_SEPARATOR '\\'
#elif defined(TULA_OS_POSIX_COMPLIANT)
    #define PATH_SEPARATOR '/'
#endif

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


TULA_FUNC void tulaPlat_normalizePath(char* path) {
    Bool isFirstToken = TRUE;
    int top = -1;
    int i = 0;
    char result[MAX_PATH_LENGTH] = "";
    char temp[MAX_PATH_LENGTH];
    char* parts[MAX_PATH_LENGTH];
    char* p;
    char* token;

    /* Copy the path */
    tula_safeStrCpy(temp, path, MAX_PATH_LENGTH);


    /* Make separators consistient */
    for (p = temp; *p; p++) {
#if defined(TULA_OS_WINDOWS)
        if (*p == '/') *p = '\\';
#elif defined(TULA_OS_POSIX_COMPLIANT)
        if (*p == '\\') *p = '/';
#else
    #error Unsupported platform, \
        no compliant implementation of function 'tulaPlat_findLastPathSep'.
#endif
    }


    /* Tokenize the path */
    token = strtok(temp, (const char[]){ PATH_SEPARATOR, '\0' });
    while (token != NULL) {
        if (strcmp(token, ".") == 0) {
            if (isFirstToken) parts[++top] = token;
        
        } else if (strcmp(token, "..") == 0) {
            if (top >= 0) top--;
        } else {
            parts[++top] = token;
        }

        token = strtok(NULL, (const char[]){ PATH_SEPARATOR, '\0' });
        isFirstToken = FALSE;
    }


    /* Rebuild the normalized path */
    for (i = 0; i <= top; i++) {
        strcat(result, (i == 0) ? "" : (const char[]){ PATH_SEPARATOR, '\0' });
        strcat(result, parts[i]);
    }

    if (result[0] == '\0') strcpy(result, ".");

    tula_safeStrCpy(path, result, MAX_PATH_LENGTH);
}


TULA_FUNC const char* tulaPlat_findLastPathSep(const char* path) {
#if defined(TULA_OS_WINDOWS)
    const char* lastSlash = strrchr(path, '/');
    const char* lastBackslash = strrchr(path, '\\');

    if (!lastSlash) return lastBackslash;
    if (!lastBackslash) return lastSlash;

    return (lastSlash > lastBackslash) ? lastSlash : lastBackslash;
#elif defined(TULA_OS_POSIX_COMPLIANT)
    return strrchr(path, '/');
#else
    #error Unsupported platform, \
        no compliant implementation of function 'tulaPlat_findLastPathSep'.
#endif
}


TULA_FUNC void tulaPlat_extractDirName(const char* path, char* outDir) {
    const char* lastSeparator = tulaPlat_findLastPathSep(path);
    UInt32 length;

    if (!lastSeparator) {
        strcpy(outDir, ".");
        return;
    }

    length = lastSeparator - path;
    if (length == 0) {
        strncpy(outDir, path, 1);
        outDir[1] = '\0';
        return;
    }

    strncpy(outDir, path, length);
    outDir[length] = '\0';
    return;
}


TULA_FUNC void tulaPlat_joinPaths(
    const char* dir, 
    const char* file, 
    char* outPath
) {
    UInt32 length = strlen(dir);

    if (length == 0) {
        tula_safeStrCpy(outPath, file, strlen(file));
        return;
    }

    if (dir[length - 1] == PATH_SEPARATOR) {
        snprintf(outPath, MAX_PATH_LENGTH, "%s%s", dir, file);
        return;
    }

    snprintf(outPath, MAX_PATH_LENGTH, "%s%c%s", dir, PATH_SEPARATOR, file);
    return;
}
