#ifndef _APPLILCATION_ENVIRONMENT_H
#define _APPLILCATION_ENVIRONMENT_H

#include "types.h"

BOOLEAN ENV_GetConfigDirectory( STRING512 config_dir );

const CHAR8*	ENV_GetDefaultBaseDirectory( void );
const CHAR8*	ENV_GetDefaultModsDirectory( void );

#endif

