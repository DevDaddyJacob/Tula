#include <stdio.h>

#include "memory.h"
#include "value.h"

void initValueArray(ValueArray* pArray) {
    pArray->pValues = NULL;
    pArray->capacity = 0;
    pArray->count = 0;
}

void writeValueArray(ValueArray* pArray, Value value) {
    if (pArray->capacity < pArray->count + 1) {
      int oldCapacity = pArray->capacity;
      pArray->capacity = GROW_CAPACITY(oldCapacity);
      pArray->pValues = GROW_ARRAY(Value, pArray->pValues, oldCapacity, pArray->capacity);
    }
  
    pArray->pValues[pArray->count] = value;
    pArray->count++;
}

void freeValueArray(ValueArray* pArray) {
    FREE_ARRAY(Value, pArray->pValues, pArray->capacity);
    initValueArray(pArray);
}

void printValue(Value value) {
    printf("%g", value);
}