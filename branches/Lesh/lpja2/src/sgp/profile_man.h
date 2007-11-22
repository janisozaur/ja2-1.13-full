#ifndef _PROFILE_MAN_H
#define _PROFILE_MAN_H

#include "types.h"


BOOLEAN InitializeProfileManager( void );

void	Profile_GetGameHomeDirectory( STRING512 directory );
void	Profile_GetGameProfileDirectory( STRING512 directory );

#endif
