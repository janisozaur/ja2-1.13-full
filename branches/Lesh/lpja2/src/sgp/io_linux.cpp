// Author: Lesh		May 2007

//===================================================================
//	io_linux.cpp - I/O layer for linux
//
//	This file is intended for I/O access portability
//	  
//===================================================================

#include "build_defines.h"

#ifdef JA2_LINUX

#include "io_layer.h"
#include "types.h"

static UINT32 globIndex;
static glob_t globResult;

//===================================================================
//
//	IO_IsDirectory - checks path as directory. Path can end by slash
//	or can be slash-less.
//
//	in	path: string, that holds path to be checked
//
//	return:	TRUE, if path is a directory, FALSE, if not
//	  
//===================================================================
BOOLEAN IO_IsDirectory(const CHAR8 *path)
{
	struct stat dir_stat;

    if (stat(path, &dir_stat) == -1)
	{
		return FALSE;
	}

	return( S_ISDIR(dir_stat.st_mode) );
}

//===================================================================
//
//	IO_IsRegularFile - checks path as file.
//
//	in	path: string, that holds path to be checked
//
//	return:	TRUE, if path is a file, FALSE, if not
//	  
//===================================================================
BOOLEAN IO_IsRegularFile(const CHAR8 *path)
{
	struct stat file_stat;

    if (stat(path, &file_stat) == -1)
	{
		return FALSE;
	}

	return( S_ISREG(file_stat.st_mode) );
}

//===================================================================
//
//	IO_Dir_SetCurrentDirectory - set current directory.
//
//	in	path: string, that contain path of the new directory
//
//	return:	TRUE, if path was set, FALSE, if not
//	  
//===================================================================
BOOLEAN IO_Dir_SetCurrentDirectory( const CHAR8 *path )
{
	if ( chdir( path ) == -1 )
	{
		return FALSE;
	}

	return TRUE;
}

//===================================================================
//
//	IO_Dir_GetCurrentDirectory - get current directory.
//
//	out	path: string, that will contain path of the current directory
//	in	strLen: max lenght of string 'path'
//
//	return:	TRUE, if path was get, FALSE, if there was an error
//	  
//===================================================================
BOOLEAN IO_Dir_GetCurrentDirectory( CHAR8 *path, UINT16 strLen )
{
	if ( !getcwd( path, strLen ) )
	{
		return FALSE;
	}

	return( TRUE );
}

//===================================================================
//
//	IO_Dir_DirectoryExists - checks directory for existance. Path can
//	end by slash or can be slash-less.
//
//	in	path: string, that holds path to be checked
//
//	return:	TRUE, if directory in path exists, FALSE, if not
//	  
//===================================================================
BOOLEAN IO_Dir_DirectoryExists( const CHAR8 *path )
{
	struct stat file_stat;

	if ( stat( path, &file_stat) == -1 )
		return FALSE;

	if ( S_ISDIR(file_stat.st_mode) )
		return TRUE;

	return FALSE;
}

//===================================================================
//
//	IO_Dir_MakeDirectory - make directory in specified place.
//	You can't create directory in non-existant directory.
//
//	in	path: string, containing path of directory to be created
//
//	return:	TRUE, if directory was successfully created,
//			FALSE, if there was an error
//	  
//===================================================================
BOOLEAN IO_Dir_MakeDirectory( const CHAR8 *path )
{
	if ( mkdir(path, 0700) == -1 )
	{
		return FALSE;
	}
	return TRUE;
}

//===================================================================
//
//	IO_Dir_Delete - delete specified directory.
//
//	in	path: string, containing path of directory to be deleted
//
//	return:	TRUE, if directory was successfully deleted,
//			FALSE, if there was an error
//	  
//===================================================================
BOOLEAN IO_Dir_Delete( const CHAR8 *path )
{
	return( rmdir(path) == 0 );
}

//===================================================================
//
//	IO_File_Delete - delete specified file.
//
//	in	path: string, containing path of file to be deleted
//
//	return:	TRUE, if file was successfully deleted,
//			FALSE, if there was an error
//	  
//===================================================================
BOOLEAN IO_File_Delete( const CHAR8 *path )
{
	return( unlink(path) == 0 );
}

//===================================================================
//
//	IO_File_GetSize - get size of specified file.
//
//	in	file: file handler
//
//	return:	size of file as UINT32, or 0, when error occured
//	  
//===================================================================
INT32 IO_File_GetSize( IOFILE file )
{
	struct stat file_stat;

	if ( fstat( file, &file_stat) == -1 )
	{
		return 0;
	}

	return( file_stat.st_size );
}

//===================================================================
//
//	IO_File_GetSize - get size of specified file.
//
//	in	path: string, containing path of file
//
//	return:	size of file as UINT32, or 0, when error occured
//	  
//===================================================================
INT32 IO_File_GetSize( const CHAR8 *path )
{
	struct stat file_stat;

	if ( stat( path, &file_stat) == -1 )
	{
		return 0;
	}
	return( file_stat.st_size );
}

//===================================================================
//
//	IO_File_Open - open/create specified file for reading/writing.
//
//	in	path:	string, containing path of file
//	in	flags:	open flags. Can be combined by OR operator.
//
//	Open flags reference:
//		IO_ACCESS_READ   - open file for reading.
//		IO_ACCESS_WRITE  - open file for writing.
//		IO_CREATE_NEW    - create file, if it is not exist, otherwise fail.
//		IO_CREATE_ALWAYS - always create file, even it is exist.
//
//	return:	handler of file as IOFILE (INT32), or -1, when error occured
//	  
//===================================================================
IOFILE IO_File_Open( const CHAR8 *path, UINT32 flags )
{
	UINT32	uiOpenFlags = 0;
	IOFILE	hFile;

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

	hFile = open( path, uiOpenFlags, S_IRWXU );
	if ( hFile == -1 )
	{
		return -1;
	}
	return hFile;
}

//===================================================================
//
//	IO_File_Close - close specified file.
//
//	in	file: file handler
//
//	return:	TRUE, if file was successfully closed,
//			FALSE, if there was an error
//	  
//===================================================================
BOOLEAN IO_File_Close( IOFILE file )
{
	if ( close( file ) == -1 )
	{
		return FALSE;
	}
	return TRUE;
}

//===================================================================
//
//	IO_File_Read - read from file into buffer specified amount of
//	bytes.
//
//	in	file:	file handler
//	in	buffer:	pointer to read buffer
//	in	size:	amount of bytes to read (buffer must hold this amount)
//
//	return:	amount of bytes actually read. -1 in case of error
//	  
//===================================================================
INT32 IO_File_Read( IOFILE file, PTR buffer, INT32 size )
{
	INT32 iBytesRead;

	iBytesRead = read( file, buffer, size );
	return iBytesRead;
}

//===================================================================
//
//	IO_File_Write - write to file from buffer specified amount of
//	bytes.
//
//	in	file:	file handler
//	in	buffer:	pointer to write buffer
//	in	size:	amount of bytes to write
//
//	return:	amount of bytes actually written. -1 in case of error
//	  
//===================================================================
INT32 IO_File_Write( IOFILE file, PTR buffer, INT32 size )
{
	INT32 iBytesWritten;

	iBytesWritten = write( file, buffer, size );
	return iBytesWritten;
}

//===================================================================
//
//	IO_File_Seek - seek specified position in file.
//
//	in	file:		file handler
//	in	distance:	offset
//	in	method:	method of seeking
//
//	Possible methods reference:
//		IO_SEEK_FROM_START   - seek from beginning of file
//		IO_SEEK_FROM_END     - seek from end of file
//		IO_SEEK_FROM_CURRENT - seek from current position
//
//	return:	TRUE, if file was successfully repositioned,
//			FALSE, if there was an error
//	  
//===================================================================
BOOLEAN IO_File_Seek( IOFILE file, INT32 distance, UINT8 method )
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
		return(FALSE);
	}
	return(TRUE);
}

//===================================================================
//
//	IO_File_GetPosition - get current position in file.
//
//	in	file:		file handler
//
//	return:	current position in file as INT32
//	  
//===================================================================
INT32 IO_File_GetPosition( IOFILE file )
{
	INT32 iPositionInFile;

	iPositionInFile = lseek( file, 0, SEEK_CUR );
	if( iPositionInFile == -1 )
	{
		iPositionInFile = 0;
	}
	return( iPositionInFile );
}

//===================================================================
//
//	IO_File_GetFirst - begin file search by specified mask in
//	in specified path. Return first found filename. Special dirs
//	("." and "..") are not returned.
//
//	in	pattern:	path with mask
//	out	buffer:		file or dir name returned here
//	in	bufferSize:	size of buffer (to exclude buffer overflow)
//
//	return:	TRUE, if at least one file was found,
//			FALSE, if there was an error or no found files
//	  
//===================================================================
BOOLEAN IO_File_GetFirst( const CHAR8 *pattern, CHAR8 *buffer, UINT16 bufferSize )
{
	memset( &globResult, GLOB_ERR | GLOB_NOSORT, sizeof(globResult) );

	if ( glob( pattern, 0, NULL, &globResult ) )
	{
		globfree( &globResult );
		return FALSE;
	}

	strncpy( buffer, globResult.gl_pathv[0], bufferSize - 1 );
	
	globIndex = 1;
	return TRUE;
}

//===================================================================
//
//	IO_File_GetNext - continue file search by specified mask in
//	in specified path. Return next found filename. Special dirs
//	("." and "..") are not returned.
//
//	out	buffer:		file or dir name returned here
//	in	bufferSize:	size of buffer (to exclude buffer overflow)
//
//	return:	TRUE, if next file was found,
//			FALSE, if there was an error or no found files
//	  
//===================================================================
BOOLEAN IO_File_GetNext( CHAR8 *buffer, UINT16 bufferSize )
{
	if ( globIndex >= globResult.gl_pathc )
		return FALSE;

	strncpy( buffer, globResult.gl_pathv[ globIndex++ ], bufferSize - 1 );
	
	return TRUE;
}

//===================================================================
//
//	IO_File_GetClose - close file search by specified mask in
//	in specified path and free all its associated resources.
//
//	return:	TRUE, if file search was successfully closed,
//			FALSE, if there was an error
//	  
//===================================================================
BOOLEAN IO_File_GetClose( void )
{
	globfree( &globResult );

	return TRUE;	
}

//===================================================================
//
//	IO_DoesFilenameMatchesPattern - tests, if filename matches the
//	pattern? (e.g. Sounds.slf matches *.slf). Case-insensitive!
//
//	in	pattern: filename with wildcards to use in comparison
//	in	filename: filename to check for match
//
//	return:	TRUE, if filename matches the pattern,
//			FALSE, if not
//	  
//===================================================================
BOOLEAN IO_DoesFilenameMatchesPattern( const CHAR8 *pattern, const CHAR8 *filename )
{
	return( fnmatch ( pattern, filename, FNM_NOESCAPE | FNM_CASEFOLD ) == 0 );
}

//===================================================================
//
//	IO_IsRootPath - tests, if it is root path or not.
//	i.e. if it starts from '/' under linux, it is considered
//	as root path.
//
//	in	path: a path to test
//
//	return:	TRUE, if path is root,
//			FALSE, if not
//	  
//===================================================================
BOOLEAN IO_IsRootPath(const CHAR8 *path)
{
	if ( path[0] == '/' )
		return TRUE;
	return FALSE;
}

//===================================================================
//
//	IO_CompareTime - compare two file times.
//
//	in	firstTime: first time
//	in	secondTime: second time
//
//	return:	== 0, if times are equal,
//			 > 0, if firstTime > secondTime,
//			 < 0, if firstTime < secondTime.
//	  
//===================================================================
INT32	IO_CompareTime( SGP_FILETIME *firstTime, SGP_FILETIME *secondTime )
{
	return( *firstTime - *secondTime );
}

//===================================================================
//
//	IO_File_GetTime - get creation, last access and last write time
//	of the file.
//
//	in	file: file handler
//	out	creationTime: time of file creation
//	out	lastAccessedTime: time of last access to file
//	out	lastWriteTime: time of last write to file
//
//	return:	TRUE, if times are successfully obtained
//			FALSE, if not
//	  
//===================================================================
BOOLEAN IO_File_GetTime( IOFILE file, SGP_FILETIME	*creationTime, SGP_FILETIME *lastAccessedTime, SGP_FILETIME *lastWriteTime )
{
	struct stat file_stat;

	if ( fstat( file, &file_stat) == -1 )
		return FALSE;

	if ( creationTime )
		*creationTime     = file_stat.st_ctime;
	if ( lastAccessedTime )
		*lastAccessedTime = file_stat.st_atime;
	if ( lastWriteTime )
		*lastWriteTime    = file_stat.st_mtime;

	return TRUE;
}

//===================================================================
//
//	IO_IsHiddenEntry - check, if file or directory has hidden-attribute
//
//	in	path: filesystem entry path
//
//	return:	TRUE, if file or directory is hidden
//			FALSE, if not
//	  
//===================================================================
BOOLEAN	IO_IsHiddenEntry(const CHAR8 *path)
{
	// for linux file or directory is hidden, when it's name start
	// from dot ".". Example: ".svn/"
	// so if the first char of path is '.' or substring "/." can
	// be found inside path, then it is hidden.

	if ( path[0] == '.' )
		return TRUE;

	if ( strstr( path, "/." ) != NULL )
		return TRUE;

	return FALSE;
}
#endif	//JA2_LINUX
