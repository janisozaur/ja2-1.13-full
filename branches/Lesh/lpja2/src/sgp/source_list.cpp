#include "types.h"
#include "source_list.h"
#include "sgp_str.h"
#include "platform.h"

#define SOURCELIST_COMMENT	"#"

CHAR8*	SourceList_ParseLine( CHAR8 *string );



// SourceList_Read - read list of sources
// --------------------------------------------------------------------------
// in	listName - source list's filename
// out	list - the list of read strings
// ret	TRUE, if list was read successfully, or FALSE, if not

BOOLEAN	SourceList_Read( const CHAR8 *listName, sgpStringArray &list )
{
	STRING512	buffer;
	CHAR8		*string;
	FILE		*file;
	
	list.empty();
	
	// open file
	file = fopen( listName, "r" );
	if ( !file )
	{
		return FALSE;
	}
	
	// read and parse each line
	while ( !feof(file) )
	{
		buffer[0] = 0;
		fgets(buffer, STRLEN(buffer), file);
		string = SourceList_ParseLine(buffer);
		if ( string ) list.push_back(string);
	}
	
	// finish work
	fclose(file);
	return TRUE;
}




// SourceList_Dump - print contents of list for debugging purposes
// --------------------------------------------------------------------------
// out	list - list of strings

void	SourceList_Dump( sgpStringArray &list )
{
	INT32	i;
	
	printf("Contents of list:\n");
	for (i=0; i<list.size(); i++)
		printf("%s\n", list[i].c_str());
}




// SourceList_ParseLine - remove comments and spaces from left and right side
// --------------------------------------------------------------------------
// in	string - input string
// ret	NULL, if string doesn't contain meaningful data, or pointer, if contain

CHAR8*	SourceList_ParseLine( CHAR8 *string )
{
	CHAR8	*ptr = string;
	
	// find comment char and cut the rest of string
	ptr = strpbrk( string, SOURCELIST_COMMENT );
	if ( ptr )
	{
		*ptr = 0;
	}
	
	// then trim space from left and right
	ptr = STR_Trim(string);
	
	// if nothing left, ignore string
	if ( strlen(ptr) == 0 )
	{
		return NULL;
	}
	return ptr;
}
