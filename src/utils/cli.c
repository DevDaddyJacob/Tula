#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "cli.h"
#include "../common.h"
#include "../tula.h"

/*
 * ==================================================
 * Typedefs & Prototypes
 * ==================================================
 */

typedef struct {
    int pointer;
    int argc;
    const char** argv;
} CliParams;

typedef enum {
    OPTION_UNKNOWN,         /** Not an option */
    OPTION_INVALID,         /** Invalid option */
    OPTION_END_OF_OPTIONS,  /** -- or --end-of-options */
    OPTION_HELP,            /** -h or --help */
    OPTION_VERSION,         /** -v or --version */
    OPTION_INTERACTIVE      /** -i or --interactive */
} OptionType;

static void printHelpMenu();

static BOOL hasNext(CliParams* params);

static const char* peekArgument(CliParams* params);

static const char* consumeArgument(CliParams* params);

static OptionType parseOptionType(const char* arg);

static BOOL consumeNext(CliParams* params, CliConfig* config);


/*
 * ==================================================
 * Module Level Variables & Constants
 * ==================================================
 */



/*
 * ==================================================
 * Function Definitions
 * ==================================================
 */

static void printHelpMenu() {
#define RIGHT_PAD_WIDTH 25

    /* Print the usage */
    fprintf(
        stdout,
        "Usage: %s [OPTIONS] [FILE]\n\n" \
        "Options:\n" \
        "\t%-*s indicates the end of the options\n" \
        "\t%-*s prints this menu\n" \
        "\t%-*s prints the version of the program\n" \
        "\t%-*s enters the REPL mode (Read-Evaluate-Print-Loop)\n",

        PROG_NAME,
        RIGHT_PAD_WIDTH, "--, --end-of-options",
        RIGHT_PAD_WIDTH, "-h, --help",
        RIGHT_PAD_WIDTH, "-v, --version",
        RIGHT_PAD_WIDTH, "-i, --interactive"
    );

#undef RIGHT_PAD_WIDTH
    exit(TULA_EXIT_GODD);
}

static BOOL hasNext(CliParams* params) {
    if (params->pointer >= params->argc) return FALSE;
    return TRUE;
}

static const char* peekArgument(CliParams* params) {
    if (!hasNext(params)) return NULL;
    return params->argv[params->pointer];
}


static const char* consumeArgument(CliParams* params) {
    if (!hasNext(params)) return NULL;
    return params->argv[params->pointer++];
}


static OptionType parseOptionType(const char* arg) {
    if (arg[0] != '-' || strlen(arg) < 2) return OPTION_UNKNOWN;

    switch (arg[1]) {
        case '-': {
            if (strlen(arg) == 2) return OPTION_END_OF_OPTIONS;

            switch (arg[2]) {
                default: {
#define ARG_EQ(str) STR_EQ(arg, str)
                    if (ARG_EQ("--end-of-options")) {
                        return OPTION_END_OF_OPTIONS;
                    } else if (ARG_EQ("--help")) {
                        return OPTION_HELP;
                    } else if (ARG_EQ("--version")) {
                        return OPTION_VERSION;
                    } else if (ARG_EQ("--interactive")) {
                        return OPTION_INTERACTIVE;
                    } else {
                        return OPTION_INVALID;
                    }
#undef ARG_EQ
                }
            }
        }

        case 'h': return OPTION_HELP;
        case 'v': return OPTION_VERSION;
        case 'i': return OPTION_INTERACTIVE;
        default: return OPTION_INVALID;
    }
}


static BOOL consumeNext(CliParams* params, CliConfig* config) {
    if (!hasNext(params)) return FALSE;

    switch (parseOptionType(peekArgument(params))) {
        case OPTION_UNKNOWN: {
            return FALSE;
        }

        case OPTION_END_OF_OPTIONS: {
            consumeArgument(params);
            return FALSE;
        }

        case OPTION_INVALID: {
            tula_errPrint(
                "invalid option: use 'tula --help' for more information"
            );

            exit(TULA_EXIT_BAD_USAGE);
            return FALSE; /* Unreachable */
        }

        case OPTION_HELP: {
            consumeArgument(params);
            printHelpMenu();
            return FALSE;
        }

        case OPTION_VERSION: {
            consumeArgument(params);
            printf(TULA_RELEASE);

            return FALSE;
        }

        case OPTION_INTERACTIVE: {
            consumeArgument(params);
            config->interactive = TRUE;
            return TRUE;
        }
    }

    return TRUE; /* Unreachable? */
}


CliConfig* tula_parseCliArgs(int argc, const char** argv) {
    CliConfig* config;
    CliParams* params;
    
    /* Allocate memory for params & config */
    params = (CliParams*)malloc(sizeof(CliParams));
    if (params == NULL) {
        tula_errPrintFatal("Failed to allocate memory");
        exit(TULA_EXIT_NO_MEM);
    }

    config = (CliConfig*)malloc(sizeof(CliConfig));
    if (config == NULL) {
        tula_errPrintFatal("Failed to allocate memory");
        exit(TULA_EXIT_NO_MEM);
    }

    
    /* Initialize the params */
    params->pointer = 1;
    params->argc = argc;
    params->argv = argv;


    /* Consume the arguments */
    while (consumeNext(params, config));

    return config;
}