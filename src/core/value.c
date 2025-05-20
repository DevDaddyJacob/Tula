#include "value.h"
#include "../utils/arrays.h"

void initValueArray(ValueArray* array) {
    array->count = 0;
    array->capacity = 0;
    array->values = NULL;
}


void freeValueArray(ValueArray* array) {
    /* Deallocate the array */
    FREE_ARRAY(Value, array->values, array->capacity);
    
    /* Zero out the fields */
    initValueArray(array);
}


void writeValueArray(ValueArray* array, Value value) {
    /* Check if we need to expand the array size */
    if (array->capacity < array->count + 1) {
        int oldCapacity = array->capacity;
        array->capacity = GROW_ARRAY_CAPACITY(oldCapacity);
        array->values = GROW_ARRAY(
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
