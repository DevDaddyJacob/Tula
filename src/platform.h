#ifndef tula_platform_h
#define tula_platform_h

#include "common.h"


#define MAX_PATH_LENGTH 4096


TULA_FUNC Bool tulaPlat_pathExists(const char* path);


TULA_FUNC Bool tulaPlat_isFile(const char* path);


TULA_FUNC void tulaPlat_normalizePath(char* path);


TULA_FUNC const char* tulaPlat_findLastPathSep(const char* path);


TULA_FUNC void tulaPlat_extractDirName(const char* path, char* outDir);


TULA_FUNC void tulaPlat_joinPaths(
    const char* dir, 
    const char* file, 
    char* outPath
);

#endif /* tula_platform_h */