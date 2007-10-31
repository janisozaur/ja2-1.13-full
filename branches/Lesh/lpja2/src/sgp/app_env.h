#ifndef _APPLILCATION_ENVIRONMENT_H
#define _APPLILCATION_ENVIRONMENT_H

#include "types.h"

BOOLEAN ENV_GetUserDirectory( STRING512 user_dir );
BOOLEAN ENV_GetConfigDirectory( STRING512 config_dir );

const CHAR8*	ENV_GetDefaultSharedDataDirectory( void );
const CHAR8*	ENV_GetDefaultSharedModsDirectory( void );

#endif

