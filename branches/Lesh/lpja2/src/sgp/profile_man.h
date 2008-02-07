#ifndef _PROFILE_MAN_H
#define _PROFILE_MAN_H

#include "types.h"


BOOLEAN Profile_Init( const CHAR8 *profile );

void	Profile_GetGameHomeDirectory( STRING512 directory );
void	Profile_GetGameProfileDirectory( STRING512 directory );
void	Profile_GetBaseDirectory( STRING512 directory );
void	Profile_GetModsDirectory( STRING512 directory );
void	Profile_GetGameDirectory( STRING512 directory );

const CHAR8*	Profile_GetLanguage( void );

#endif
