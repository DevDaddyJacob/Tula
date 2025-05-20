#ifndef tulac_value_h
#define tulac_value_h

#include "../common.h"

typedef double Value;


typedef struct {
    int count;          /** The amount of the elements which are in use */
    int capacity;       /** The amount of elements in the array */
    Value* values;      /** Pointer to the first element of the array */
} ValueArray;


/**
 * \brief           Initializes the provided array
 * \param[in]       array: Pointer to the array to initialize
 */
void initValueArray(ValueArray* array);


/**
 * \brief           Releases the provided array
 * \param[in]       array: Pointer to the array to free
 */
void freeValueArray(ValueArray* array);


/**
 * \brief           Writes the provided value to the array
 * \param[in]       array: Pointer to the array to write to
 * \param[in]       value: The value to write to the array
 */
void writeValueArray(ValueArray* array, Value value);


#endif /* tulac_value_h */