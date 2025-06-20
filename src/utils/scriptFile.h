#ifndef tula_utils_script_file
#define tula_utils_script_file

#include "../common.h"

typedef struct tulaScr_ScriptFile {
    char* content;
    UInt32 capacity;
    UInt32 count;
} ScriptFile;


TULA_FUNC ScriptFile* tulaSrc_new(const char* filePath);


TULA_FUNC void tulaSrc_destroy(ScriptFile* script);


#endif /* tula_utils_script_file */