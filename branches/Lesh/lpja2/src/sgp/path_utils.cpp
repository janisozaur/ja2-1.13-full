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

BOOLEAN DoesFilenameMatchesPattern( const CHAR8 *pattern, const CHAR8 *filename )
{
	return( fnmatch ( pattern, filename, FNM_NOESCAPE | FNM_CASEFOLD ) == 0 );
}
