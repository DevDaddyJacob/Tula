#ifndef tula_utils_dyn_buffer
#define tula_utils_dyn_buffer

#include "../common.h"

typedef struct tulaDynBuff_DynamicBuffer {
    char* data;
    UInt32 capacity;
    UInt32 count;
} DynamicBuffer;


TULA_FUNC DynamicBuffer* tulaDynBuff_new();


TULA_FUNC void tulaDynBuff_destroy(DynamicBuffer* buffer);


TULA_FUNC void tulaDynBuff_wipe(DynamicBuffer* buffer);


TULA_FUNC char tulaDynBuff_get(DynamicBuffer* buffer, UInt32 i);


TULA_FUNC void tulaDynBuff_add(DynamicBuffer* buffer, char value);


#endif /* tula_utils_dyn_buffer */