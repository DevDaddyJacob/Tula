#ifndef tula_h
#define tula_h

#include "conf.h"

#define TULA_VERSION_MAJOR	    "1"
#define TULA_VERSION_MINOR	    "0"
#define TULA_VERSION_RELEASE	"0"

#define TULA_VERSION "Tula v" TULA_VERSION_MAJOR "." TULA_VERSION_MINOR

#define TULA_RELEASE TULA_VERSION "." TULA_VERSION_RELEASE


void tula_exit(int exitCode);
void tula_exitError(int exitCode, const char* errorMessage);
void tula_exitFatal(int exitCode, const char* errorMessage);


#endif /* tula_h */