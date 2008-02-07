#include "types.h"
#include "path_utils.h"
#include "platform.h"
#include <physfs.h>


// this function make all letters to be lowercased
// and changes slashes '\' to '/'.
void LowercaseAndChangeSlash( CHAR8 *string )
{
	CHAR8 *ch;
	
	ch = string;
	while ( *ch )
	{
		if ( *ch == '\\' ) *ch = '/';
		else *ch = tolower( *ch );
		ch++;
	}
}

const CHAR8*	GetDirSeparator( void )
{
	return PHYSFS_getDirSeparator();
}

//===================================================================
//
//	DoesFilenameMatchesPattern - tests, if filename matches the
//	pattern? (e.g. Sounds.slf matches *.slf). Case-insensitive!
//
//	in	pattern: filename with wildcards to use in comparison
//	in	filename: filename to check for match
//
//	return:	TRUE, if filename matches the pattern,
//			FALSE, if not
//	  
//===================================================================
BOOLEAN DoesFilenameMatchesPattern( const CHAR8 *pattern, const CHAR8 *filename )
{
	return( fnmatch ( pattern, filename, FNM_NOESCAPE | FNM_CASEFOLD ) == 0 );
}

//===================================================================
//
//	IsRootPath - tests, if it is root path or not.
//	i.e. if it starts from '/' under linux, it is considered
//	as root path.
//
//	in	path: a path to test
//
//	return:	TRUE, if path is root,
//			FALSE, if not
//	  
//===================================================================
BOOLEAN IsRootPath(const CHAR8 *path)
{
	if ( path[0] == '/' )
		return TRUE;
	return FALSE;
}

//===================================================================
//
//	AddEndingSlashIfNeeded - add slash to end of path, if there was
//	no slash. Leave path as is, if it ends by slash
//
//	in	path: a path
//
//	return:	a pointer to result string
//	  
//===================================================================
CHAR8*	AddEndingSlashIfNeeded( CHAR8 *path )
{
	INT32	strLen = strlen(path);
	
	if ( path[strLen-1] != SLASH )
	{
		strcat(path, GetDirSeparator());
	}
	return path;
}

//===================================================================
//
//	RemoveEndingSlashIfNeeded - remove slash from the end of path,
//  if there was any slash. Leave path as is, if it isn't end by slash
//
//	in	path: a path
//
//	return:	a pointer to result string
//	  
//===================================================================
CHAR8*	RemoveEndingSlashIfPresent( CHAR8 *path )
{
	INT32	strLen = strlen(path);
	
	if ( path[strLen-1] == SLASH )
	{
		path[strLen-1] == 0;
	}
	return path;
}
