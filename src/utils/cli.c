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
    OPTION_INTERACTIVE,     /** -i or --interactive */
    OPTION_DUMP_DEBUG       /** --dump-debug */
} OptionType;

static void printHelpMenu();

static void dumpDebug();

static Bool hasNext(CliParams* params);

static const char* peekArgument(CliParams* params);

static const char* consumeArgument(CliParams* params);

static OptionType parseOptionType(const char* arg);

static Bool consumeNextOption(CliParams* params, CliConfig* config);


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
        "Usage: %s [OPTIONS...] [FILE]\n\n" \
        "Options:\n" \
        "\t%-*s indicates the end of the options\n" \
        "\t%-*s prints this menu\n" \
        "\t%-*s prints the version of the program\n" \
        "\t%-*s enters the REPL mode (Read-Evaluate-Print-Loop)\n" \
        "\t%-*s dumps debug info\n",

        PROG_NAME,
        RIGHT_PAD_WIDTH, "--, --end-of-options",
        RIGHT_PAD_WIDTH, "-h, --help",
        RIGHT_PAD_WIDTH, "-v, --version",
        RIGHT_PAD_WIDTH, "-i, --interactive",
        RIGHT_PAD_WIDTH, "--dump-debug"
    );

#undef RIGHT_PAD_WIDTH
    exit(TULA_EXIT_GODD);
}


static void dumpDebug() {
    printf(
        "Debug Info:\n" \
        "\t TULA_DEBUGGING: %s\n" \
        "\t TULA_OS_WINDOWS: %s\n" \
        "\t TULA_OS_MAC: %s\n" \
        "\t TULA_OS_LINUX: %s\n" \
        "\t TULA_OS_UNIX: %s\n" \
        "\t TULA_OS_POSIX_COMPLIANT: %s\n" \
        "\nVersion: %s",

#ifdef TULA_DEBUGGING
            "Yes",
#else
            "No",
#endif /* TULA_DEBUGGING */


#ifdef TULA_OS_WINDOWS
            "Yes",
#else
            "No",
#endif /* TULA_OS_WINDOWS */


#ifdef TULA_OS_MAC
            "Yes",
#else
            "No",
#endif /* TULA_OS_MAC */


#ifdef TULA_OS_LINUX
            "Yes",
#else
            "No",
#endif /* TULA_OS_LINUX */


#ifdef TULA_OS_UNIX
            "Yes",
#else
            "No",
#endif /* TULA_OS_UNIX */


#ifdef TULA_OS_POSIX_COMPLIANT
            "Yes",
#else
            "No",
#endif /* TULA_OS_POSIX_COMPLIANT */

        TULA_RELEASE
    );

    exit(TULA_EXIT_GODD);
}


static Bool hasNext(CliParams* params) {
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
                    } else if (ARG_EQ("--dump-debug")) {
                        return OPTION_DUMP_DEBUG;
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


static Bool consumeNextOption(CliParams* params, CliConfig* config) {
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

        case OPTION_DUMP_DEBUG: {
            dumpDebug();
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


    /* Consume the options */
    while (consumeNextOption(params, config));


    /* If we still have next args, store it as the file */
    if (hasNext(params)) {
        UInt16 strLen = strlen(peekArgument(params)) + 1;

        /* Allocate memory for the file */
        config->file = (char*)malloc(sizeof(char) * strLen);
        if (config->file == NULL) {
            tula_errPrintFatal("Failed to allocate memory");
            exit(TULA_EXIT_NO_MEM);
        }

        
        /* Copy the memory of the argument to the config */
        tula_safeStrCpy(config->file, consumeArgument(params), strLen);
    } else {
        config->file = NULL;
    }


    free(params);
    return config;
}