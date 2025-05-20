#include <stdlib.h>

#include "../common.h"


void* reallocateArray(void* pointer, size_t oldSize, size_t newSize) {
    /* If the new size is 0 handle the deallocation */
    if (newSize == 0) {
        free(pointer);
        return NULL;
    }

    void* result = realloc(pointer, newSize);
    return result;
}