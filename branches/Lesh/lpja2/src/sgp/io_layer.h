#ifndef __IO_LAYER_H_
#define __IO_LAYER_H_

// Author: Lesh		May 2007

//===================================================================
//	io_layer.h - I/O layer header
//
//	This file is intended for I/O access portability
//	  
//===================================================================

#include "types.h"
#include "platform.h"

typedef INT32	IOFILE;

#define IO_ACCESS_READ	    0x01
#define IO_ACCESS_WRITE	    0x02
#define IO_ACCESS_READWRITE	0x03

#define IO_CREATE_NEW		0x0010	// create new file. fail if exists
#define IO_CREATE_ALWAYS	0x0020	// create new file. overwrite existing
#define IO_OPEN_EXISTING	0x0040	// open a file. fail if doesn't exist
#define IO_OPEN_ALWAYS		0x0080	// open a file, create if doesn't exist

#define IO_SEEK_FROM_START		0x01
#define IO_SEEK_FROM_END		0x02
#define IO_SEEK_FROM_CURRENT	0x04

BOOLEAN IO_IsDirectory(const CHAR8 *path);
BOOLEAN IO_IsRegularFile(const CHAR8 *path);

BOOLEAN IO_File_Delete( const CHAR8 *path );
INT32 	IO_File_GetSize( IOFILE file );
INT32 	IO_File_GetSize( const CHAR8 *path );
IOFILE 	IO_File_Open( const CHAR8 *path, UINT32 flags );
BOOLEAN IO_File_Close( IOFILE file );
INT32 	IO_File_Read( IOFILE file, PTR buffer, INT32 size );
INT32 	IO_File_Write( IOFILE file, PTR buffer, INT32 size );
BOOLEAN IO_File_Seek( IOFILE file, INT32 distance, UINT8 method );
INT32 	IO_File_GetPosition( IOFILE file );
BOOLEAN IO_File_GetFirst( const CHAR8 *pattern, CHAR8 *buffer, UINT16 bufferSize );
BOOLEAN IO_File_GetNext( CHAR8 *buffer, UINT16 bufferSize );
BOOLEAN IO_File_GetClose( void );

BOOLEAN IO_Dir_SetCurrentDirectory( const CHAR8 *path );
BOOLEAN IO_Dir_GetCurrentDirectory( CHAR8 *path, UINT16 strLen );
BOOLEAN IO_Dir_DirectoryExists( const CHAR8 *path );
BOOLEAN IO_Dir_MakeDirectory( const CHAR8 *path );
BOOLEAN IO_Dir_Delete( const CHAR8 *path );

#endif
