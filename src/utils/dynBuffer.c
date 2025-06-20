#include <stdlib.h>

#include "dynBuffer.h"
#include "../common.h"

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

TULA_FUNC DynamicBuffer* tulaDynBuff_new() {
    DynamicBuffer* buffer;

    /* Allocate the buffer */
    buffer = (DynamicBuffer*)malloc(sizeof(DynamicBuffer));
    if (buffer == NULL) return NULL;


    /* Allocate the initial data */
    buffer->data = (char*)malloc(sizeof(char) * TULA_ARRAY_MIN_THRESHOLD);
    if (buffer->data == NULL) {
        tulaDynBuff_destroy(buffer);
        return NULL;
    }


    /* Init the fields */
    buffer->capacity = TULA_ARRAY_MIN_THRESHOLD;
    buffer->count = 0;

    return buffer;
}


TULA_FUNC void tulaDynBuff_destroy(DynamicBuffer* buffer) {
    if (buffer == NULL) return;

    if (buffer->data != NULL) free(buffer->data);

    free(buffer);
}


TULA_FUNC void tulaDynBuff_wipe(DynamicBuffer* buffer) {
    UInt32 i = 0;

    /* Null check of sanity */
    if (buffer == NULL) return;
    if (buffer->data == NULL) return;


    /* Resize the array to the default */
    buffer->data = TULA_ARRAY_RESIZE(
        char, 
        buffer->data, 
        buffer->capacity,
        TULA_ARRAY_MIN_THRESHOLD
    );

    buffer->capacity = TULA_ARRAY_MIN_THRESHOLD;
    buffer->count = 0;


    /* Zero the fields */
    for (i = 0; i < buffer->capacity; i++) {
        buffer->data[i] = '\0';
    }
}


TULA_FUNC char tulaDynBuff_get(DynamicBuffer* buffer, UInt32 i) {
    /* Null check of sanity */
    if (buffer == NULL) return;


    /* Check if the index is in range */
    if (i >= buffer->count) return EOF;

    return buffer->data[i];
}


TULA_FUNC void tulaDynBuff_add(DynamicBuffer* buffer, char value) {
    /* Null check of sanity */
    if (buffer == NULL) return;


    /* Check if we need to expand the array size */
    if (buffer->capacity < buffer->count + 1) {
        int oldCapacity = buffer->capacity;
        buffer->capacity = TULA_ARRAY_GROW_CAPACITY(oldCapacity);
        buffer->data = TULA_ARRAY_RESIZE(
            char, 
            buffer->data, 
            oldCapacity, 
            buffer->capacity
        );
    }

    
    /* Add the byte to the end of the array */
    buffer->data[buffer->count] = value;
    buffer->count++;
}