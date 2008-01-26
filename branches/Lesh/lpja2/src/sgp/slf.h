#ifndef _LIBRARY_DATABASE_H
#define _LIBRARY_DATABASE_H

#include "types.h"
#include <physfs.h>



typedef	INT32			slfHandler;
typedef PHYSFS_sint64	fsSize;
typedef PHYSFS_file*	fsFile;



//*************************************************************************
//
//  NOTE!  The following structs are also used by the datalib98 utility
//
//*************************************************************************


#ifdef __cplusplus
extern "C" {
#endif


//Function Prototypes

// Lesh: new slf interface

BOOLEAN		SLF_Init( );
void		SLF_Shutdown( );

void		SLF_Dump( const CHAR8 *filename );

BOOLEAN		SLF_AddLibrary( const CHAR8 *pLibName );

slfHandler	SLF_OpenResource ( const CHAR8 *pFilename );
BOOLEAN		SLF_CloseResource( slfHandler hResource );
fsSize		SLF_Read ( slfHandler hResource, void *pBuffer, INT32 iSize );
BOOLEAN		SLF_Seek ( slfHandler hResource, INT32 Position, INT32 iMethod );
fsSize		SLF_Tell ( slfHandler hResource );
BOOLEAN		SLF_IsEOF( slfHandler hResource );
fsSize		SLF_Size_Handler  ( slfHandler hResource );
fsSize		SLF_Size_Filename ( const CHAR8 *pFilename );
BOOLEAN		SLF_IsExist( const CHAR8 *pFilename );
BOOLEAN		SLF_IsValidHandle( slfHandler hResource );

#ifdef __cplusplus
}
#endif



#endif
