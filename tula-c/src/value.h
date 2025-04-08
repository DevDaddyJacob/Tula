#ifndef tulac_value_h
#define tulac_value_h

#include "common.h"

typedef enum {
    VAL_NIL,
    VAL_BOOL,
    VAL_NUMBER,
} ValueType;

typedef struct {
    ValueType type;
    union {
        bool boolean;
        double number;
    } as;
} Value;

#define IS_NIL(value) ((value).type == VAL_NIL)
#define IS_BOOL(value) ((value).type == VAL_BOOL)
#define IS_NUMBER(value) ((value).type == VAL_NUMBER)

#define AS_BOOL(value) ((value).as.boolean)
#define AS_NUMBER(value) ((value).as.number)

#define NIL_VAL ((Value){VAL_NIL, { .number = 0 }})
#define BOOL_VAL(value) ((Value){VAL_BOOL, { .boolean = value }})
#define NUMBER_VAL(value) ((Value){VAL_NUMBER, { .number = value }})

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