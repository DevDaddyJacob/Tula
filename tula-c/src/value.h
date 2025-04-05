#ifndef tulac_value_h
#define tulac_value_h

#include "common.h"

typedef double Value;

/**
 * @brief Dynamic array for storing multiple Values
 */
typedef struct {
    int capacity;
    int count;
    Value* pValues;
} ValueArray;

/**
 * @brief Initializes a new value array
 * 
 * @param pArray Pointer to the value array to initialize
 */
void initValueArray(ValueArray* pArray);

/**
 * @brief Used to append a value to the end of an array
 * 
 * @param pArray Pointer to the value array to add the value to
 * @param value The value to add
 */
void writeValueArray(ValueArray* pArray, Value value);

/**
 * @brief Deallocates all of the memory used by the array
 * 
 * @param pArray Pointer to the value array to free
 */
void freeValueArray(ValueArray* pArray);

/**
 * @brief Prints a value to the console
 * 
 * @param value The value to print
 */
void printValue(Value value);

#endif