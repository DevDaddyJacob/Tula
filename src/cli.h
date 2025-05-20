#ifndef tulac_cli_h
#define tulac_cli_h

#include "common.h"


typedef struct {
    /* Flags */
    BOOL helpMenu;          /** If the program should output the help menu */
    BOOL interactive;       /** If the program should enter REPL */
    
    /* Options */
    char* helpItem;         /** The flag/option to display the help menu for */
    char* scriptFile;       /** The name/path to the script file to run */
} TulaState;


/**
 * \brief           Parses the program's command line arguments and builds 
 *                  the state based on them
 * \param[in]       argc: Argument count (from main)
 * \param[in]       argv: Argument vector (from main)
 * \param[out]      state: The state to initialize
 */
void tula_parseCliArgs(int argc, const char* argv[], TulaState* state);


/**
 * \brief           Frees the provided state from memory
 * \param[in]       state: The state to free
 */
void tula_freeState(TulaState* state);


#endif /* tulac_cli_h */