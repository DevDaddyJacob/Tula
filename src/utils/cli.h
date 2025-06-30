#ifndef tula_utils_cli_h
#define tula_utils_cli_h

#include "../common.h"
#include "../tula.h"

extern const char* PROG_NAME;

typedef struct {
    /**
     * If the interactive option is selected
     */
    Bool interactive;

    /**
     * The file to run
     */
    char* file;
} CliConfig;


/**
 * \brief               Takes the command line arguments and parses them into
 *                      a CliConfig struct
 */
TULA_FUNC CliConfig* tulaCli_parseArgs(int argc, const char* argv[]);


TULA_FUNC void tulaCli_destroy(CliConfig* config);


#endif /* tula_utils_cli_h */