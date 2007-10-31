#include "build_defines.h"

#ifdef JA2_LINUX

#include "app_env.h"
#include "platform.h"

BOOLEAN ENV_GetUserDirectory( STRING512 user_dir )
{
	const struct passwd* passwd = getpwuid(getuid());

	if (passwd == NULL || passwd->pw_dir == NULL)
	{
		return FALSE;
	}

	strncpy( user_dir, passwd->pw_dir, 512 );
	
	return TRUE;
}

BOOLEAN ENV_GetConfigDirectory( STRING512 config_dir )
{
	strcpy( config_dir, "/etc/ja2-1.13/");
	return TRUE;
}

const CHAR8*	ENV_GetDefaultSharedDataDirectory( void )
{
	return "/usr/share/ja2-1.13/data/";
}

const CHAR8*	ENV_GetDefaultSharedModsDirectory( void )
{
	return "/usr/share/ja2-1.13/mods/";
}

#endif	// JA2_LINUX
