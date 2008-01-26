#include "types.h"
#include "resource_db.h"
#include "profile_man.h"
#include "command_line.h"
#include <physfs.h>
#include "path_utils.h"
#include "source_list.h"
#include "sgp_str.h"
#include "slf.h"


// constant names
#define SOURCES_LIST_NAME	"sources.list"
#define MODS_LIST_NAME		"mods.list"

// resource types
enum ResourceType
{
	RES_DIR,
	RES_ZIP,
	RES_SLF,
	RES_OTHER
};

// relevant strings
STRING512	zBaseDir;
STRING512	zModsDir;
STRING512	zWriteDir;

// necessary local functions
void	ResourceDB_HandleResourceList( const CHAR8 *listDir, sgpStringArray &list );
INT32	ResourceDB_GetResourceType( sgpString &resource );



// ResourceDB_Init - initialization of resource database, setup of the PhysFS
// --------------------------------------------------------------------------
// ret	TRUE, if initialization was successful, or FALSE, if not

BOOLEAN	ResourceDB_Init( void )
{
	sgpStringArray	list, mods;
	STRING512		listName, currentModDir;
	INT32			i;
	
	if ( !PHYSFS_init( argv0 ) )
	{
		fprintf(stderr, "Unable to init PhysicsFS. Reason: %s\n", PHYSFS_getLastError() );
		return FALSE;
	}
	
	if ( !Profile_Init( NULL ) )
		return FALSE;
	
	Profile_GetBaseDirectory( zBaseDir );
	Profile_GetModsDirectory( zModsDir );
	Profile_GetGameProfileDirectory( zWriteDir );
	
	// add base dir to be able to read slf libraries
	if ( !PHYSFS_addToSearchPath( zBaseDir, 0 ) )
	{
		fprintf(stderr, "Failed to add path %s. Reason: %s\n", zBaseDir, PHYSFS_getLastError() );
		return FALSE;
	}
	if ( !SLF_Init() )
		return FALSE;
	
	// prepare base sources list name, read it and parse
	STR_SPrintf( listName, STRLEN(listName), "%s%s", zBaseDir, SOURCES_LIST_NAME );
	if ( !SourceList_Read(listName, list) )
	{
		fprintf(stderr, "Failed to read base source list\n");
		return FALSE;
	}
	ResourceDB_HandleResourceList(zBaseDir, list);
	
	// read mods list from profile directory
	STR_SPrintf( listName, STRLEN(listName), "%s%s", zWriteDir, MODS_LIST_NAME );
	if ( !SourceList_Read(listName, mods) )
	{
		fprintf(stderr, "Failed to read mods list\n");
		return FALSE;
	}
	
	// for each mod in mods list
	for ( i=0; i<mods.size(); i++ )
	{
		// prepare mod sources list name, read it and parse
		STR_SPrintf( currentModDir, STRLEN(currentModDir), "%s%s", zModsDir, mods[i].c_str() );
		STR_SPrintf( listName, STRLEN(listName), "%s%s%s", currentModDir, GetDirSeparator(), SOURCES_LIST_NAME );
		if ( SourceList_Read(listName, list) )
		{
			ResourceDB_HandleResourceList(currentModDir, list);
		}
		else
		{
			fprintf(stderr, "Failed to read mod source list for mod %s\n", mods[i].c_str() );
		}
	}
	
	// at the end set write directory and add it to search path
	if ( !PHYSFS_setWriteDir(zWriteDir) )
	{
		fprintf(stderr, "Failed to set write directory. Reason: %s\n", PHYSFS_getLastError() );
		return FALSE;
	}
	if ( !PHYSFS_addToSearchPath( zWriteDir, 0 ) )
	{
		fprintf(stderr, "Failed to add path %s. Reason: %s\n", zWriteDir, PHYSFS_getLastError() );
		return FALSE;
	}
	
	return TRUE;
}




// ResourceDB_Shutdown - deinitialization of resource database
// --------------------------------------------------------------------------

void	ResourceDB_Shutdown( void )
{
	SLF_Shutdown();
	if ( !PHYSFS_deinit( ) )
	{
		fprintf(stderr, "Something is wrong, while deinit PhysicsFS. Reason: %s\n", PHYSFS_getLastError() );
	}
}




// ResourceDB_HandleResourceList - handle resource list, attach any directory,
// slf or zip, specified in it.
// --------------------------------------------------------------------------
// in	list - list of source names

void	ResourceDB_HandleResourceList( const CHAR8 *listDir, sgpStringArray &list )
{
	INT32		i;
	STRING512	path;
	
	for ( i=0; i<list.size(); i++ )
	{
		switch( ResourceDB_GetResourceType( list[i] ) )
		{
		case RES_DIR:
		case RES_ZIP:
			STR_SPrintf( path, STRLEN(path), "%s%s", listDir, list[i].c_str() );
			if ( !PHYSFS_addToSearchPath( path, 0 ) )
			{
				fprintf(stderr, "Failed to add resource %s. Reason: %s\n", path, PHYSFS_getLastError() );
			}
			break;
		
		case RES_SLF:
			if ( !SLF_AddLibrary( list[i].c_str() ) )
			{
				fprintf(stderr, "Failed to add slf-file %s.\n", list[i].c_str() );
			}
			break;
		
		default:
			fprintf(stderr, "Unknown resource %s.\n", list[i].c_str() );
			break;
		}
	}
}




// ResourceDB_GetResourceType - get type of resource
// --------------------------------------------------------------------------
// in	resource - resource name
// ret	resource type according to ResourceType enum

INT32	ResourceDB_GetResourceType( sgpString &resource )
{
	// check for directory
	if ( PHYSFS_isDirectory( resource.c_str() ) )
	{
		return RES_DIR;
	}
	
	// check against zip
	if ( resource.find(".zip") != sgpString::npos )
	{
		return RES_ZIP;
	}
	
	// check against slf
	if ( resource.find(".slf") != sgpString::npos )
	{
		return RES_SLF;
	}
	
	return RES_OTHER;	
}




// ResourceDB_Dump - dump list of search paths
// --------------------------------------------------------------------------

void	ResourceDB_Dump( void )
{
	char **i;
	char **rc = PHYSFS_getSearchPath();

	printf("List of search paths:\n");
	for ( i = rc; *i != NULL; i++)
		printf(" * [%s]\n", *i);	

	PHYSFS_freeList(rc);	
}
