#include "profile_man.h"
#include "app_env.h"
#include "types.h"
#include "sgp_str.h"
#include "io_layer.h"
#include "SDL_config_lib.h"
#include <physfs.h>
#include "path_utils.h"


#define GAME_DIR_PREFIX			"ja2-1.13"
#define GAME_PROFILE_PREFIX		"profiles"
#define GAME_PROFILE			"profile.ini"
#define GAME_DEFAULT_PROFILE	"default"
#define GAME_CONFIG_FILENAME	"ja2-1.13.conf"
#define GAME_MODS_PREFIX		"mods"
#define GAME_BASE_PREFIX		"base"
#define DEFAULT_LANGUAGE		"en"


STRING512	zUserHome;
STRING512	zGameHomeDirectory;
STRING512	zGameProfilesDirectory;
STRING512	zCurrentGameProfileDirectory;

STRING512	zUserModsDirectory;

STRING512	zModsDirectory;
STRING512	zBaseDirectory;
STRING512	zGameDirectory;
STRING512	zGameLanguage;
STRING512	zGameDirPrefix;

BOOLEAN	HandleGlobalConfig( const CHAR8 *filename );
BOOLEAN	ParseConfigFile( const CHAR8 *config_file,
						 STRING512 lang,
						 STRING512 gameDir,
						 STRING512 baseDir,
						 STRING512 modsDir );

BOOLEAN	SetCurrentProfile( const CHAR8 *profile );
BOOLEAN	GetCurrentProfile( STRING512 profile );
BOOLEAN	CreateNewProfile( const CHAR8 *profile, const CHAR8 *path );


BOOLEAN Profile_Init( const CHAR8 *profile )
{
	STRING512	config_file;
	STRING512	currentProfile;
	STRING512	configDir;

	printf("Initializing profile...\n");
	
	// obtain a user home
	strncpy( zUserHome, PHYSFS_getUserDir(), STRLEN(zUserHome) );

	// compose config filename inside user home and read it
	STR_SPrintf( config_file, STRLEN(config_file), "%s.%s", zUserHome, GAME_CONFIG_FILENAME);
	if ( !HandleGlobalConfig( config_file ) )
	{
		printf("No user config %s. Reading system config.\n", config_file);
		ENV_GetConfigDirectory( configDir );
		STR_SPrintf( config_file, STRLEN(config_file), "%s%s", configDir, GAME_CONFIG_FILENAME);
		if ( !HandleGlobalConfig( config_file ) )
		{
			printf("No system config %s. Abort.\n", config_file);
			fprintf(stderr, "No system config nor user found!\n" );
			return FALSE;
		}
	}
	
	// get game home directory
	STR_SPrintf( zGameHomeDirectory, STRLEN(zGameHomeDirectory), "%s%s", zUserHome, zGameDirPrefix );
	if ( !IO_Dir_DirectoryExists( zGameHomeDirectory ) )
		if ( !IO_Dir_MakeDirectory( zGameHomeDirectory ) )
		{
			fprintf(stderr, "Unable to create game home directory '%s'\n", zGameHomeDirectory);
			return FALSE;
		}

	// get game profiles directory
	STR_SPrintf( zGameProfilesDirectory, STRLEN(zGameProfilesDirectory), "%s%s%s", zGameHomeDirectory, GAME_PROFILE_PREFIX, GetDirSeparator() );
	printf("Profile dir: %s\n", zGameProfilesDirectory);
	if ( !IO_Dir_DirectoryExists( zGameProfilesDirectory ) )
		if ( !IO_Dir_MakeDirectory( zGameProfilesDirectory ) )
		{
			fprintf(stderr, "Unable to create game profiles directory '%s'\n", zGameProfilesDirectory);
			return FALSE;
		}

	// retrieve currently saved profile
	if ( !GetCurrentProfile(currentProfile) )
	{
		strcpy( currentProfile, GAME_DEFAULT_PROFILE );
	}

	STR_SPrintf( zCurrentGameProfileDirectory, STRLEN(zCurrentGameProfileDirectory), "%s%s%s",
		zGameProfilesDirectory, currentProfile, GetDirSeparator() );	
	if ( !IO_Dir_DirectoryExists( zCurrentGameProfileDirectory ) )
	{
		CreateNewProfile( currentProfile, zCurrentGameProfileDirectory );
	}

	SetCurrentProfile( currentProfile );

	printf("Base  dir: %s\n", zBaseDirectory);
	printf("Mods  dir: %s\n", zModsDirectory);
	printf("Write dir: %s\n", zCurrentGameProfileDirectory);
	return TRUE;
}

BOOLEAN	ParseConfigFile( const CHAR8 *config_file,
						 STRING512 lang,
						 STRING512 gameDir,
						 STRING512 baseDir,
						 STRING512 modsDir )
{
	CFG_File		cfg;
	const CHAR8		*ini_string;
	
	if ( CFG_OpenFile( config_file, &cfg ) != CFG_OK )
	{
		return FALSE;
	}

	CFG_SelectGroup( NULL, CFG_True );
	
	ini_string = CFG_ReadText( "lang", DEFAULT_LANGUAGE );
	strncpy( lang, ini_string, 512 );

	ini_string = CFG_ReadText( "game_dir", GAME_DIR_PREFIX );
	strncpy( gameDir, ini_string, 512 );

	ini_string = CFG_ReadText( "base_dir", ENV_GetDefaultBaseDirectory() );
	strncpy( baseDir, ini_string, 512 );
	
	ini_string = CFG_ReadText( "mods_dir", ENV_GetDefaultModsDirectory() );
	strncpy( modsDir, ini_string, 512 );
	
	CFG_CloseFile( &cfg );
	
	return TRUE;
}


BOOLEAN SetCurrentProfile( const CHAR8 *profile )
{
	CFG_File		cfg;
	STRING512		zFilename;

	STR_SPrintf( zFilename, STRLEN(zFilename), "%s%s", zGameProfilesDirectory, GAME_PROFILE );

	CFG_OpenFile( NULL, &cfg );
	if ( CFG_SelectGroup( CFG_GLOBAL, TRUE ) == CFG_ERROR )
	{
		return FALSE;
	}
	CFG_WriteText( "profile", profile );
	if ( CFG_SaveFile( zFilename ) != CFG_OK )
	{
		return FALSE;
	}
	CFG_CloseFile( &cfg );
	return TRUE;
}


BOOLEAN	GetCurrentProfile( STRING512 profile )
{
	CFG_File		cfg;
	STRING512		zFilename;
	const CHAR8		*profileName;

	STR_SPrintf( zFilename, STRLEN(zFilename), "%s%s", zGameProfilesDirectory, GAME_PROFILE );

	if ( CFG_OpenFile( zFilename, &cfg ) != CFG_OK )
	{
		return FALSE;
	}
	profileName = CFG_ReadText( "profile", GAME_DEFAULT_PROFILE );
	strncpy(profile, profileName, 512);
	CFG_CloseFile( &cfg );
	return TRUE;
}

void	Profile_GetGameHomeDirectory( STRING512 directory )
{
	strcpy( directory, zGameHomeDirectory );
}

void	Profile_GetGameProfileDirectory( STRING512 directory )
{
	strcpy( directory, zCurrentGameProfileDirectory );
}

void	Profile_GetBaseDirectory( STRING512 directory )
{
	strcpy( directory, zBaseDirectory );
}

void	Profile_GetModsDirectory( STRING512 directory )
{
	strcpy( directory, zModsDirectory );
}

void	Profile_GetGameDirectory( STRING512 directory )
{
	strcpy( directory, zGameDirectory );
}

BOOLEAN CreateNewProfile( const CHAR8 *profile, const CHAR8 *path )
{
	if ( !IO_Dir_MakeDirectory( path ) )
	{
		fprintf(stderr, "Unable to create game profile directory '%s'\n", path);
		return FALSE;
	}
	printf("Profile subdirectory '%s' was created\n", path);
}

BOOLEAN	HandleGlobalConfig( const CHAR8 *filename )
{
	STRING512 lang;
	STRING512 gameDir;
	STRING512 baseDir;
	STRING512 modsDir;
	
	if ( !ParseConfigFile(filename, lang, gameDir, baseDir, modsDir) )
		return FALSE;
	
	// make lang
	strcpy(zGameLanguage, lang);
	
	// make game dir prefix
	AddEndingSlashIfNeeded(gameDir);
	strcpy(zGameDirPrefix, gameDir);
	
	// make base dir
	AddEndingSlashIfNeeded(baseDir);
	if ( IsRootPath(baseDir) )
	{
		strcpy(zBaseDirectory, baseDir);
	}
	else
	{
		STR_SPrintf( zBaseDirectory, STRLEN(zBaseDirectory), "%s%s%s",
			zUserHome,
			zGameDirPrefix,
			baseDir );
	}

	// make mods dir	
	AddEndingSlashIfNeeded(modsDir);
	if ( IsRootPath(modsDir) )
	{
		strcpy(zModsDirectory, modsDir);
	}
	else
	{
		STR_SPrintf( zModsDirectory, STRLEN(zModsDirectory), "%s%s%s",
			zUserHome,
			zGameDirPrefix,
			modsDir );
	}

	return TRUE;
}

const CHAR8*	Profile_GetLanguage( void )
{
	return zGameLanguage;
}
