#ifndef tula_core_state_h
#define tula_core_state_h

#include "../common.h"
#include "../core/chunk.h"
#include "../utils/cli.h"
#include "../utils/scriptFile.h"

typedef struct tulaState_GlobalState {
    Int16 argc;
    char** argv;
    CliConfig* cli;
    ScriptFile* script;
    Chunk* scriptChunk;
} GlobalState;


TULA_FUNC void tulaState_setup(Int16 argc, const char** argv);


TULA_FUNC void tulaState_teardown();


TULA_FUNC CliConfig* tulaState_getGlobal();



#endif /* tula_core_state_h */