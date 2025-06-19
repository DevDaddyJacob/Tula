#include <stdlib.h>

#include "value.h"
#include "../common.h"


/*
 * ==================================================
 * Function Definitions
 * ==================================================
 */

void tula_initValueArray(ValueArray* array) {
    array->count = 0;
    array->capacity = 0;
    array->values = NULL;
}


void tula_freeValueArray(ValueArray* array) {
    /* Deallocate the array */
    TULA_ARRAY_FREE(Value, array->values, array->capacity);
    
    /* Zero out the fields */
    tula_initValueArray(array);
}


void tula_writeValueArray(ValueArray* array, Value value) {
    /* Check if we need to expand the array size */
    if (array->capacity < array->count + 1) {
        int oldCapacity = array->capacity;
        array->capacity = TULA_ARRAY_GROW_CAPACITY(oldCapacity);
        array->values = TULA_ARRAY_RESIZE(
            Value, 
            array->values, 
            oldCapacity, 
            array->capacity
        );
    }

    /* Add the byte to the end of the array */
    array->values[array->count] = value;
    array->count++;
}
