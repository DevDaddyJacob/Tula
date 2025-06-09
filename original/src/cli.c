#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli.h"
#include "debug.h"
#include "./utils/log.h"

/*
 * ==================================================
 * Typedefs & Prototypes
 * ==================================================
 */

typedef enum {
    PARSE_OK,
    PARSE_STOP,
    PARSE_ERROR_UNKNOWN,
    PARSE_ERROR_BAD_OPTION,
    PARSE_ERROR_BAD_USE,
} ParseResult;

typedef enum {
    OPTION_INVALID,
    OPTION_END_OF_OPTS,
    OPTION_INTERACTIVE,
    OPTION_HELP,
    OPTION_SCRIPT_FILE
} CliOptions;


static BOOL strprefix(const char* str, const char* pre);

static void initState(TulaState* state);

static CliOptions checkArg(
    const char* arg,
    const char* match,
    CliOptions option
);

static CliOptions parseArg(TulaState* state, const char* arg);


/*
 * ==================================================
 * Module Level Variables & Constants
 * ==================================================
 */

char* PROGRAM_NAME;


/*
 * ==================================================
 * Function Definitions
 * ==================================================
 */

static BOOL strprefix(const char* str, const char* pre) {
    if (strncmp(pre, str, strlen(pre)) == 0) {
        return TRUE;
    } else {
        return FALSE;
    }
}


static void initState(TulaState* state) {
    state = malloc(sizeof(TulaState));

    /* Set the flags */
    state->helpMenu = FALSE;
    state->interactive = FALSE;

    /* Set the options */
    state->helpItem = NULL;
    state->scriptFile = NULL;
}


static CliOptions checkArg(
    const char* arg,
    const char* match,
    CliOptions option
) {
    if (strprefix(arg, match)) {
        return option;
    }

    return OPTION_INVALID;
}


static CliOptions parseArg(TulaState* state, const char* arg) {
    printf("ParseArg: %s\n", arg);
    if (strlen(arg) <= 1 || arg[0] != '-') return OPTION_INVALID;


    switch (arg[1]) {
        case 'h': return checkArg(arg, "-h", OPTION_HELP);
        case 's': return checkArg(arg, "-s", OPTION_SCRIPT_FILE);
        case 'I': return checkArg(arg, "-I", OPTION_INTERACTIVE);

        case '-': {
            if (strlen(arg) == 2) return OPTION_END_OF_OPTS;
            switch (arg[2]) {
                case 'h': return checkArg(arg, "--help", OPTION_HELP);
                default: break;
            }
            break;
        }

        default: break;
    }

    return OPTION_INVALID;
}


void tula_freeState(TulaState* state) {
    free(state);
    state = NULL;
}


void tula_parseCliArgs(int argc, const char* argv[], TulaState* state) {
    initState(state);

    PROGRAM_NAME = argv[0];

    /* Handle no args */
    if (argc == 1) {
        if (IS_DEBUG_ENABLED) {
            state ->interactive = TRUE;
        } else {
            state->helpMenu = TRUE;
        }

        return;
    }


    /* If only 1 argument is provided, check if it can be assumed as the file */
    if (argc == 2 && argv[1][0] != '-') {
        state->scriptFile = argv[1];
        return;
    }


    /* Parse the command line args */
    CliOptions option;
    for (int i = 1; i < argc; i++) {
        switch (option = parseArg(state, argv[i])) {
            case OPTION_END_OF_OPTS: break;
            
            case OPTION_HELP: {
                state->helpMenu = TRUE;
                if (
                    i + 1 < argc 
                    && parseArg(state, argv[i + 1]) == OPTION_INVALID
                ) {
                    state->helpItem = argv[i + 1];
                    i++;
                }
                break;
            }

            case OPTION_INVALID:
            default: {
                tula_printfErr(
                    "bad option '%s'\n",
                    argv[i]
                );
                exit(1);
                return;
            }
        }
    }
}