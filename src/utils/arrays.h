#ifndef tulac_arrays_h
#define tulac_arrays_h

#include "../conf.h"


#define GROW_ARRAY_CAPACITY(capacity) \
    ((capacity) < TULA_ARRAY_MIN_THRESHOLD \
        ? TULA_ARRAY_MIN_THRESHOLD \
        : (capacity) * TULA_ARRAY_GROW_FACTOR)


#define GROW_ARRAY(type, pointer, oldCount, newCount) \
    (type*)reallocateArray( \
        pointer, \
        sizeof(type) * (oldCount), \
        sizeof(type) * (newCount) \
    )


#define FREE_ARRAY(type, pointer, oldCount) \
    reallocateArray(pointer, sizeof(type) * (oldCount), 0)

    
/**
 * \brief           Reallocates an array to be the new size
 * \note            Designed to be used in the GROW_ARRAY & FREE_ARRAY macros
 * \param[in]       pointer: Pointer to the array to reallocate
 * \param[in]       oldSize: The old size of the array
 * \param[in]       newSize: The new size of the array
 * \return          Returns a pointer to the new array
 */
void* reallocateArray(void* pointer, size_t oldSize, size_t newSize);




#endif /* tulac_arrays_h */