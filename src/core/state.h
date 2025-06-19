#ifndef tula_core_state_h
#define tula_core_state_h

#include "../common.h"

typedef struct {
    struct ThreadState* mainThread;
} GlobalState;

typedef struct {
    GlobalState* global;
} ThreadState;

#endif /* tula_core_state_h */