#include "profile_man.h"
#include "app_env.h"
#include "types.h"
#include "sgp_str.h"
#include "io_layer.h"
#include "SDL_config_lib.h"

#define GAME_DIR_PREFIX			"ja2-1.13"
#define GAME_PROFILE_PREFIX		"profiles"
#define GAME_LAST_PROFILE		".last_profile"
#define GAME_DEFAULT_PROFILE	"default"
#define GAME_CONFIG_FILENAME	"ja2-1.13.conf"
#define GAME_MODS_PREFIX		"mods"


STRING512	zGameHomeDirectory;
STRING512	zGameProfilesDirectory;
STRING512	zCurrentGameProfileDirectory;

STRING512	zSharedDataDirectory;
STRING512	zSharedModsDirectory;
STRING512	zUserModsDirectory;

BOOLEAN ParseConfigFile( STRING512 config_file );

BOOLEAN InitializeProfileManager( void )
{
	STRING512	user_home_dir;
	STRING512	config_file;

	printf("Initializing Profile Manager...\n");
	
	if ( !ENV_GetUserDirectory( user_home_dir ) )
	{
		fprintf(stderr, "Unable to locate user home directory\n");
		return FALSE;
	}
	
	// assign home dir for game
	// data dir will be read from INI/CFG file, located in home dir
	printf("User home is %s\n", user_home_dir);

	// get game home directory
	STR_SPrintf( zGameHomeDirectory, STRLEN(zGameHomeDirectory), "%s%c%s%c", user_home_dir, SLASH, GAME_DIR_PREFIX, SLASH );
	printf("Using game home dir: %s\n", zGameHomeDirectory);

	if ( !IO_Dir_DirectoryExists( zGameHomeDirectory ) )
		if ( !IO_Dir_MakeDirectory( zGameHomeDirectory ) )
		{
			fprintf(stderr, "Unable to create game home directory \"%s\"\n", zGameHomeDirectory);
			return FALSE;
		}

	// get game profiles directory
	STR_SPrintf( zGameProfilesDirectory, STRLEN(zGameProfilesDirectory), "%s%s%c", zGameHomeDirectory, GAME_PROFILE_PREFIX, SLASH );

	if ( !IO_Dir_DirectoryExists( zGameProfilesDirectory ) )
		if ( !IO_Dir_MakeDirectory( zGameProfilesDirectory ) )
		{
			fprintf(stderr, "Unable to create game profiles directory \"%s\"\n", zGameProfilesDirectory);
			return FALSE;
		}

	// get config filename
	ENV_GetConfigDirectory( config_file );
	strcat( config_file, GAME_CONFIG_FILENAME );
	if ( !ParseConfigFile( config_file ) )
	{
		fprintf(stderr, "Failed to open config \"%s\"\n", config_file);
		//return FALSE;
	}
	
	// get user mods directory
	STR_SPrintf( zUserModsDirectory, STRLEN(zUserModsDirectory), "%s%s%c", zGameHomeDirectory, GAME_MODS_PREFIX, SLASH );
	printf("Shared data dir: %s\n", zSharedDataDirectory);
	printf("Shared mods dir: %s\n", zSharedModsDirectory);
	printf("User mods dir  : %s\n", zUserModsDirectory);

	return TRUE;
}

BOOLEAN ParseConfigFile( STRING512 config_file )
{
	CFG_File		cfg;
	const CHAR8		*ini_string;
	
	if ( CFG_OpenFile( config_file, &cfg ) != CFG_OK )
	{
		CFG_OpenFile( NULL, &cfg );
		//return FALSE;
	}

	CFG_SelectGroup( NULL, CFG_True );
	
	ini_string = CFG_ReadText( "shared_data_dir", ENV_GetDefaultSharedDataDirectory() );
	strncpy( zSharedDataDirectory, ini_string, 511 );
	
	ini_string = CFG_ReadText( "shared_mods_dir", ENV_GetDefaultSharedModsDirectory() );
	strncpy( zSharedModsDirectory, ini_string, 511 );
	
	CFG_CloseFile( &cfg );
	
	return TRUE;
}
