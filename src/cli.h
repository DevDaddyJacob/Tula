#ifndef tula_cli_h
#define tula_cli_h

#include "common.h"
#include "tula.h"

extern const char* PROG_NAME;

typedef struct {
    BOOL interactive;           /** If the interactive option is selected */
} CliConfig;


/**
 * \brief               Takes the command line arguments and parses them into
 *                      a CliConfig struct
 */
CliConfig* tula_parseCliArgs(int argc, const char* argv[]);


#endif /* tula_cli_h */