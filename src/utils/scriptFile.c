#include <stdlib.h>
#include <string.h>

#include "scriptFile.h"
#include "fio.h"
#include "dynBuffer.h"
#include "../common.h"
#include "../platform.h"

/*
 * ==================================================
 * Macros
 * ==================================================
 */

#define ARRAY_MIN_THRESHOLD 1024

#define ARRAY_GROW_CAPACITY(capacity) \
    ((capacity) < ARRAY_MIN_THRESHOLD \
        ? ARRAY_MIN_THRESHOLD \
        : (capacity) * TULA_ARRAY_GROW_FACTOR)

/*
 * ==================================================
 * Typedefs & Prototypes
 * ==================================================
 */

static void fileReaderNextLine(FileReader* reader, DynamicBuffer* buffer);


static void parseFile(ScriptFile* script, const char* importPath);


static void writeFile(ScriptFile* script, char data);


static void writeFileStr(ScriptFile* script, const char* data, UInt32 dataLen);


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

static void fileReaderNextLine(FileReader* reader, DynamicBuffer* buffer) {
    /* Null check of sanity */
    if (reader == NULL) return;
    if (buffer == NULL) return;


    /* Read until end of line*/
    while (tulaFio_hasNext(reader) && tulaFio_peek(reader) != '\n') {
        tulaDynBuff_add(buffer, tulaFio_read(reader));
    }


    /* If the next char is the line return, add it */
    if (tulaFio_peek(reader) == '\n') {
        tulaDynBuff_add(buffer, tulaFio_read(reader));
    }
}


static void parseFile(ScriptFile* script, const char* importPath) {
    Bool canHaveImport = TRUE;
    FileReader* reader;
    DynamicBuffer* lineBuffer;
    char scriptPath[MAX_PATH_LENGTH];
    char scriptDir[MAX_PATH_LENGTH];

    /* Null check of sanity */
    if (script == NULL) return;
    if (importPath == NULL) return;


    /* Get the dir of this script */
    tula_safeStrCpy(scriptPath, importPath, MAX_PATH_LENGTH);
    tulaPlat_normalizePath(scriptPath);
    
    tulaPlat_extractDirName(scriptPath, scriptDir);


    /* Check if file exsits */
    if (!tulaPlat_isFile(scriptPath)) {
        fprintf("tula: Error: No such file \"%s\"", scriptPath);
        return;
    }


    /* Open the reader */
    reader = tulaFio_openReader(importPath);
    if (reader == NULL) return;


    /* Open the buffer */
    lineBuffer = tulaDynBuff_new();
    if (lineBuffer == NULL) {
        free(reader);
        return;
    }

    tulaDynBuff_wipe(lineBuffer);


    /* Start to read the file line by line */
    do {
        Bool isImportLine = FALSE;

        /* Read the line */
        fileReaderNextLine(reader, lineBuffer);


        /* Check if the line is an import line */
        isImportLine = strncmp("tula_import", lineBuffer->data, 11) == 0;

        
        /* Check if we can have an import line, and if so import it */
        if (canHaveImport && isImportLine) {
            char importPath[MAX_PATH_LENGTH];
            char nextPath[MAX_PATH_LENGTH];

            tula_safeStrCpy(
                importPath, 
                (lineBuffer->data + 12), 
                (lineBuffer->count - 12)
            );
            tulaPlat_normalizePath(importPath);

            tulaPlat_joinPaths(scriptDir, importPath, nextPath);
            tulaPlat_normalizePath(nextPath);
            
            parseFile(script, nextPath);
            writeFile(script, '\n');
        } else {
            writeFileStr(script, lineBuffer->data, lineBuffer->count);
        }

        isImportLine = !(canHaveImport && !isImportLine);
        tulaDynBuff_wipe(lineBuffer);
    } while (tulaFio_hasNext(reader));


    tulaFio_closeReader(reader);
    tulaDynBuff_destroy(lineBuffer);
}


static void writeFile(ScriptFile* script, char data) {
    /* Null check of sanity */
    if (script == NULL) return;


    /* Check if we need to expand the array size */
    if (script->capacity < script->count + 1) {
        int oldCapacity = script->capacity;
        script->capacity = ARRAY_GROW_CAPACITY(oldCapacity);
        script->content = TULA_ARRAY_RESIZE(
            char, 
            script->content, 
            oldCapacity, 
            script->capacity
        );
    }

    
    /* Add the byte to the end of the array */
    script->content[script->count] = data;
    script->count++;
}


static void writeFileStr(ScriptFile* script, const char* data, UInt32 dataLen) {
    UInt32 i = 0;

    /* Null check of sanity */
    if (script == NULL) return;
    if (data == NULL) return;


    /* Write each char to the script */
    for (i = 0; i < dataLen; i++) {
        writeFile(script, data[i]);
    }
}


TULA_FUNC ScriptFile* tulaSrc_new(const char* filePath) {
    ScriptFile* script;

    /* Null check of sanity */
    if (filePath == NULL) return NULL;


    /* Allocate the script file on the heap */
    script = (ScriptFile*)malloc(sizeof(ScriptFile));
    if (script == NULL) return NULL;


    /* Allocate the initial size of the array */
    script->content = (char*)malloc(sizeof(char) * ARRAY_MIN_THRESHOLD);
    if (script->content == NULL) {
        tulaSrc_destroy(script);
        return NULL;
    }

    script->capacity = ARRAY_MIN_THRESHOLD;
    script->count = 0;

    parseFile(script, filePath);
    writeFile(script, '\0');

    return script;
}


TULA_FUNC void tulaSrc_destroy(ScriptFile* script) {
    if (script == NULL) return;

    if (script->content != NULL) free(script->content);
}