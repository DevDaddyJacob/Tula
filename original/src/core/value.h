#ifndef tulac_value_h
#define tulac_value_h

#include "../common.h"


#define IS_TYPE(value, _type) ((value).type == _type)
#define IS_BOOL(value)      IS_TYPE(value, VAL_BOOL)
#define IS_NIL(value)       IS_TYPE(value, VAL_NIL)
#define IS_NUMBER(value)    IS_TYPE(value, VAL_NUMBER)

#define AS_VAL(value, field) ((value).as.field)
#define AS_BOOL(value)      AS_VAL(value, boolean)
#define AS_NUMBER(value)    AS_VAL(value, number)

#define CAST_VAL(_type, field, value) ((Value){ _type, { .field = value } })
#define BOOL_VAL(value)     CAST_VAL(VAL_BOOL, boolean, value)
#define NIL_VAL             CAST_VAL(VAL_NIL, number, 0)
#define NUMBER_VAL(value)   CAST_VAL(VAL_NUMBER, number, value)


typedef enum {
    VAL_BOOL,
    VAL_NIL,
    VAL_NUMBER,
} ValueType;


typedef struct {
    ValueType type;         /** The type of the value */
    union {
        BOOL boolean;
        double number;
    } as;                   /** Tagged union for the value payload */
} Value;


typedef struct {
    int count;          /** The amount of the elements which are in use */
    int capacity;       /** The amount of elements in the array */
    Value* values;      /** Pointer to the first element of the array */
} ValueArray;


/**
 * \brief           Initializes the provided array
 * \param[in]       array: Pointer to the array to initialize
 */
void tula_initValueArray(ValueArray* array);


/**
 * \brief           Releases the provided array
 * \param[in]       array: Pointer to the array to free
 */
void tula_freeValueArray(ValueArray* array);


/**
 * \brief           Writes the provided value to the array
 * \param[in]       array: Pointer to the array to write to
 * \param[in]       value: The value to write to the array
 */
void tula_writeValueArray(ValueArray* array, Value value);


#endif /* tulac_value_h */