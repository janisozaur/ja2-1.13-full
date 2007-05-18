// Author: Lesh		May 2007

//===================================================================
//	io_linux.cpp - I/O layer for linux
//
//	This file is intended for I/O access portability
//	  
//===================================================================

#ifdef JA2_LINUX

#include "io_layer.h"
#include "types.h"

static UINT32 globIndex;
static glob_t globResult;


BOOLEAN IO_IsDirectory(const STR8 path)
{
	struct stat dir_stat;

    if (stat(path, &dir_stat) == -1)
	{
		printf("IsDirectory() fails: path %s\n", path);
		return FALSE;
	}

	return( S_ISDIR(dir_stat.st_mode) );
}

BOOLEAN IO_IsRegularFile(const STR8 path)
{
	struct stat file_stat;

    if (stat(path, &file_stat) == -1)
	{
		printf("IsRegularFile() fails: path %s\n", path);
		return FALSE;
	}

	return( S_ISREG(file_stat.st_mode) );
}

BOOLEAN IO_Dir_SetCurrentDirectory( const STR8 path )
{
	if ( chdir( path ) == -1 )
	{
		fprintf(stderr, "Error setting dir %s: errno=%d\n",
			path, errno);
		return FALSE;
	}

	return TRUE;
}

BOOLEAN IO_Dir_GetCurrentDirectory( STRING512 path )
{
	if ( !getcwd( path, 512 ) )
	{
		fprintf(stderr, "Error getting dir %s: errno=%d\n",
			path, errno);
		return FALSE;
	}

	return( TRUE );
}

BOOLEAN IO_Dir_DirectoryExists( const STRING512 path )
{
	struct stat file_stat;

	if ( stat( path, &file_stat) == -1 )
		return FALSE;

	if ( S_ISDIR(file_stat.st_mode) )
		return TRUE;

	return FALSE;
}

BOOLEAN IO_Dir_MakeDirectory( const STRING512 path )
{
	if ( mkdir(path, 0755) == -1 )
	{
		fprintf(stderr, "Error creating dir %s: errno=%d\n",
			path, errno);
		return FALSE;
	}
	return TRUE;
}

BOOLEAN IO_Dir_Delete( const STR8 path )
{
	return( unlink(path) == 0 );
}

BOOLEAN IO_File_Delete( const STR8 path )
{
	return( unlink(path) == 0 );
}

UINT32 IO_File_GetSize( HANDLE file )
{
	struct stat file_stat;

	if ( fstat( file, &file_stat) == -1 )
	{
		fprintf(stderr, "Error getting size of file %d: errno=%d\n",
			file, errno);
		return 0;
	}

	return( file_stat.st_size );
}

UINT32 IO_File_GetSize( const STR8 path )
{
	struct stat file_stat;

	if ( stat( path, &file_stat) == -1 )
	{
		fprintf(stderr, "Error getting size of file %s: errno=%d\n",
			path, errno);
		return 0;
	}
	return( file_stat.st_size );
}

HWFILE IO_File_Open( const STR8 path, UINT32 flags )
{
	UINT32	uiOpenFlags = 0;
	HWFILE	hFile;

	if  ( flags & (IO_ACCESS_READ | IO_ACCESS_WRITE) )
		uiOpenFlags = O_RDWR;
	else if ( flags & IO_ACCESS_READ )
		uiOpenFlags = O_RDONLY;
	else if ( flags & IO_ACCESS_WRITE )
		uiOpenFlags = O_WRONLY;

	if  ( flags & IO_CREATE_NEW )
		uiOpenFlags |= O_CREAT | O_EXCL;
	else if ( flags & IO_CREATE_ALWAYS )
		uiOpenFlags |= O_CREAT;

	hFile = open( path, uiOpenFlags );
	if ( hFile == -1 )
	{
		fprintf(stderr, "Error opening file %s: errno=%d\n",
			path, errno);
		return -1;
	}
	return hFile;
}

BOOLEAN IO_File_Close( HWFILE file )
{
	if ( close( file ) == -1 )
	{
		fprintf(stderr, "Error closing file %d: errno=%d\n",
			file, errno);
		return FALSE;
	}
	return TRUE;
}

UINT32 IO_File_Read( HWFILE file, PTR buffer, UINT32 size )
{
	UINT32 uiBytesRead;

	uiBytesRead = read( file, buffer, size );
	if ( size != uiBytesRead )
	{
		fprintf(stderr, "Error reading file %d: errno=%d\n",
			file, errno);
		return FALSE;
	}
	return TRUE;
}

UINT32 IO_File_Write( HWFILE file, PTR buffer, UINT32 size )
{
	if ( write( file, buffer, size ) == -1 )
	{
		fprintf(stderr, "Error writing file %d: errno=%d\n",
			file, errno);
		return FALSE;
	}
	return TRUE;
}

BOOLEAN IO_File_Seek( HWFILE file, INT32 distance, UINT8 method )
{
	UINT32 uiMoveMethod;

	if ( method == IO_SEEK_FROM_START )
		uiMoveMethod = SEEK_SET;
	else if ( method == IO_SEEK_FROM_END )
	{
		uiMoveMethod = SEEK_END;
		if( distance > 0 )
			distance = -(distance);
	}
	else
		uiMoveMethod = SEEK_CUR;

	if ( lseek( file, distance, uiMoveMethod ) == -1 )
	{
		fprintf(stderr, "Error seeking file %d: errno=%d\n",
			file, errno);
		return(FALSE);
	}
	return(TRUE);
}

INT32 IO_File_GetPosition( HWFILE file )
{
	INT32 iPositionInFile;

	iPositionInFile = lseek( file, 0, SEEK_CUR );
	if( iPositionInFile == -1 )
	{
		fprintf(stderr, "Error getting position in file %d: errno=%d\n",
			file, errno);
		iPositionInFile = 0;
	}
	return( iPositionInFile );
}

BOOLEAN IO_File_GetFirst( const STR8 pattern, STR8 buffer, UINT16 bufferSize )
{
	memset( globResult, GLOB_ERR | GLOB_MARK | GLOB_NOSORT, sizeof(globResult) );

	if ( glob( pattern, 0, NULL, &globResult )
	{
		fprintf(stderr, "Error while trying to glob() using mask: %s, errno=%d\n", pattern, errno);
		globfree( &globResult );
		return FALSE;
	}

	strncpy( buffer, globResult.gl_pathv[0], bufferSize );
	
	globIndex = 1;
	return TRUE;
}

BOOLEAN IO_File_GetNext( STR8 buffer, UINT16 bufferSize )
{
	if ( globIndex >= globResult.gl_pathc )
		return FALSE;

	strncpy( buffer, globResult.gl_pathv[ globIndex++ ], bufferSize );
	
	return TRUE;
}

BOOLEAN IO_File_GetClose( void )
{
	globfree( &globResult );

	return TRUE;	
}

#endif	//JA2_LINUX
