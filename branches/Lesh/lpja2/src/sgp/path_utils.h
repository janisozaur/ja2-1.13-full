#ifndef _PATH_UTILS_H
#define _PATH_UTILS_H

#include "types.h"

// this function make all letters to be lowercased
// and changes slashes '\' to '/'.
void 	LowercaseAndChangeSlash( CHAR8 *string );
const CHAR8*	GetDirSeparator( void );
BOOLEAN DoesFilenameMatchesPattern( const CHAR8 *pattern, const CHAR8 *filename );

#endif
