#include <stdlib.h>

#include "state.h"
#include "../common.h"
#include "../core/chunk.h"
#include "../utils/cli.h"
#include "../utils/scriptFile.h"

/*
 * ==================================================
 * Macros
 * ==================================================
 */

/* #define XYZ "ABC" */

/*
 * ==================================================
 * Typedefs & Prototypes
 * ==================================================
 */


/*
 * ==================================================
 * Module Level Variables & Constants
 * ==================================================
 */

static GlobalState* GLOBAL_STATE = NULL;


/*
 * ==================================================
 * Function Definitions
 * ==================================================
 */

TULA_FUNC void tulaState_setup(Int16 argc, const char** argv) {
    int i = 0;

    if (GLOBAL_STATE != NULL) return;

    /* Initialize the state in heap */
    GLOBAL_STATE = (GlobalState*)malloc(sizeof(GlobalState));
    if (GLOBAL_STATE == NULL) {
        tula_exitFatal(TULA_EXIT_NO_MEM, "Failed to allocate memory");
        return; /* Unreachable */
    }


    /* Initialize all fields */
    GLOBAL_STATE->argc = argc;
    GLOBAL_STATE->argv = argv;
    GLOBAL_STATE->cli = NULL;
    GLOBAL_STATE->script = NULL;
    GLOBAL_STATE->scriptChunk = NULL;

    
    /* Setup the cli config */
    GLOBAL_STATE->cli = tulaCli_parseArgs(
        GLOBAL_STATE->argc, 
        GLOBAL_STATE->argv
    );
}


TULA_FUNC void tulaState_teardown() {
    if (GLOBAL_STATE == NULL) return;


    /* Destroy the cli config */
    if (GLOBAL_STATE->cli != NULL) {
        free(GLOBAL_STATE->cli);
        GLOBAL_STATE->cli = NULL;
    }
    

    /* Destroy the script file */
    if (GLOBAL_STATE->script != NULL) {
        tulaSrc_destroy(GLOBAL_STATE->script);
        GLOBAL_STATE->script = NULL;
    }
    

    /* Destroy the script chunk */
    if (GLOBAL_STATE->scriptChunk != NULL) {
        tulaChk_destroy(GLOBAL_STATE->scriptChunk);
        GLOBAL_STATE->scriptChunk = NULL;
    }


    free(GLOBAL_STATE);
    GLOBAL_STATE = NULL;
}


TULA_FUNC CliConfig* tulaState_getGlobal() {
    if (GLOBAL_STATE == NULL) {
        tula_exitFatal(
            TULA_EXIT_ACCESS_STATE_BEFOR_INIT,
            "Attempted to access global state before initialization"
        );
        return NULL; /* Unreachable */
    }

    return GLOBAL_STATE;
}