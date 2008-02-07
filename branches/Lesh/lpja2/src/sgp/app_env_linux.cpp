#include "build_defines.h"

#ifdef JA2_LINUX

#include "app_env.h"
#include "platform.h"

BOOLEAN ENV_GetConfigDirectory( STRING512 config_dir )
{
	strcpy( config_dir, "/etc/ja2-1.13/");
	return TRUE;
}

const CHAR8*	ENV_GetDefaultBaseDirectory( void )
{
	return "/usr/share/ja2-1.13/base/";
}

const CHAR8*	ENV_GetDefaultModsDirectory( void )
{
	return "/usr/share/ja2-1.13/mods/";
}

#endif	// JA2_LINUX
