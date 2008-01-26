#ifndef _SOURCE_LIST_H
#define _SOURCE_LIST_H

#include "types.h"

BOOLEAN	SourceList_Read( const CHAR8 *listName, sgpStringArray &list );
void	SourceList_Dump( sgpStringArray &list );

#endif
