//**************************************************************************
//
// Filename :	FileMan.c
//
//	Purpose :	function definitions for the memory manager
//
// Modification history :
//
//		24sep96:HJH		-> creation
//    08Apr97:ARM   -> Assign return value from Push() calls back to HStack
//                     handle, because it may possibly do a MemRealloc()
//		29Dec97:Kris Morness 
//									-> Added functionality for setting file attributes which
//									   allows for read-only attribute overriding
//									-> Also added a simple function that clears all file attributes
//										 to normal.
//
//		5 Feb 98:Dave French -> extensive modification to support libraries
//
//**************************************************************************

//**************************************************************************
//
//				Includes
//
//**************************************************************************
#ifdef JA2_PRECOMPILED_HEADERS
	#include "ja2_sgp_all.h"
#else
	#include "types.h"
	#include "platform.h"
	#include "file_man.h"
	#include "mem_man.h"
	#include "debug.h"
	#include "slf.h"
	#include "io_layer.h"
	#include "sgp_str.h"
	#include "wcheck.h"
	#include "app_env.h"
	#include "profile_man.h"
	#include "command_line.h"
		
#include <physfs.h>
#include "path_utils.h"
#include "source_list.h"
#include "resource_db.h"

#endif
//**************************************************************************
//
//				Defines
//
//**************************************************************************

#define FILENAME_LENGTH					600

//**************************************************************************
//
//				Debugging
//
//**************************************************************************

// Uncomment this, if you want to see debug output
//#define	DEBUG_FILEMAN

#ifdef DEBUG_FILEMAN
#define FDEBUG(fmt, args...)	printf(fmt, ## args)
#else
#define FDEBUG(fmt, args...)
#endif

//**************************************************************************
//
//				Variables
//
//**************************************************************************

#define GAME_DIR_PREFIX			"ja2-1.13"
#define TEMP_DIR_PREFIX			"temp"


BOOLEAN fFindInfoInUse[20] = {FALSE,FALSE,FALSE,FALSE,FALSE,
								FALSE,FALSE,FALSE,FALSE,FALSE,
								FALSE,FALSE,FALSE,FALSE,FALSE,
								FALSE,FALSE,FALSE,FALSE,FALSE };

// Lesh: gzDataPath will represent path to dir, containing Data and Data-1.13 dirs
// gzHomePath will represent a path to game home dir, where ja2.ini, savedgames, logs
// and temps will be. Under windows it will not be used, under linux it is ~/ja2_113/
STRING512	gzDataPath;
STRING512	gzHomePath;
STRING512	gzTempPath;
STRING512	gzWorkPath;

// Lesh: this struct contains info about opened files
typedef struct
{
	BOOLEAN		isFree;
	BOOLEAN		isSLF;
	union
	{
		fsFile		file;
		slfHandler	slf;
	} handle;
} FilemanEntry;

typedef std::vector<FilemanEntry>	FilemanOpenedFiles;

// declare opened files array
FilemanOpenedFiles		OpenedFiles;

INT32	FindFreeOpenedFilesSlot( void );

//**************************************************************************
//
//				Function Prototypes
//
//**************************************************************************

HANDLE	GetHandleToRealFile( HWFILE hFile, BOOLEAN *pfDatabaseFile );

//**************************************************************************
//
//				Functions
//
//**************************************************************************

void SLF_Debug( void )
{
	slfHandler	file;
	INT32		size, wasRead;
	void		*buffer;
	FILE		*dump;
	CHAR8		test1[8], test2[4], test3[16];
	INT32		i;
	
	file = SLF_OpenResource("ambient/bird4.wav");
	if ( !SLF_IsValidHandle(file) )
	{
		printf("SLF_Debug: Invalid handle!\n");
		return;
	}
	printf("SLF_Debug: handle = %d\n", file);
	size = SLF_Size_Handler( file );
	printf("SLF_Debug: size = %d\n", size);

	SLF_Seek(file, 8, FILE_SEEK_FROM_START);
	printf("SLF_Debug: pos = %d\n", SLF_Tell(file) );
	wasRead = SLF_Read(file, test1, 8);
	printf("SLF_Debug: was read = %d\n", wasRead);

	SLF_Seek(file, 20, FILE_SEEK_FROM_CURRENT);
	printf("SLF_Debug: pos = %d\n", SLF_Tell(file) );
	wasRead = SLF_Read(file, test2, 4);
	printf("SLF_Debug: was read = %d\n", wasRead);

	SLF_Seek(file, 16, FILE_SEEK_FROM_END);
	printf("SLF_Debug: pos = %d\n", SLF_Tell(file) );
	wasRead = SLF_Read(file, test3, 16);
	printf("SLF_Debug: was read = %d\n", wasRead);

	printf("SLF_Debug: test1 = ");
	for ( i=0; i<8; i++)
		printf("%c", test1[i]);
	printf("\n");

	printf("SLF_Debug: test2 = ");
	for ( i=0; i<4; i++)
		printf("%c", test2[i]);
	printf("\n");

	printf("SLF_Debug: test3 = ");
	for ( i=0; i<16; i++)
		printf("%c", test3[i]);
	printf("\n");
/*
	size = SLF_Size_Handler( file );
	printf("SLF_Debug: size = %d\n", size);
	buffer = MemAlloc( size );
	if ( !buffer )
	{
		printf("SLF_Debug: No memory!\n");
		SLF_CloseResource( file );
		return;
	}
	wasRead = SLF_Read(file, buffer, size);
	printf("SLF_Debug: was read = %d\n", wasRead);
	
	dump = fopen("dump1.dat", "w");
	if ( !dump )
	{
		printf("SLF_Debug: Can't create dump!\n");
		SLF_CloseResource( file );
		return;
	}
	fwrite(buffer, 1, wasRead, dump);
	fclose(dump);
*/	
//	MemFree( buffer );
	SLF_CloseResource( file );
}

//===================================================================
//
//	FindFreeOpenedFilesSlot - obtain a free slot of opened files array
//
//	return:	(>=0), if free slot was found, (-1), if no free slot
//
//===================================================================
INT32	FindFreeOpenedFilesSlot( void )
{
	INT32	cnt;
	
	// Lesh: index starts from 1 because index 0 is reserved for error reporting
	for ( cnt = 1; cnt < OpenedFiles.size(); cnt++ )
	{
		if ( OpenedFiles[cnt].isFree )
			return cnt;
	}
	
	return -1;	// failed to find
}

//**************************************************************************
//
// FileSystemInit
//
//		Starts up the file system.
//
// Parameter List :
// Return Value :
// Modification history :
//
//		24sep96:HJH		-> creation
//
//**************************************************************************
BOOLEAN	InitializeFileManager(  STR strIndexFilename )
{
	printf("Initializing File Manager...\n");

	if ( !ResourceDB_Init() )
		return FALSE;

//	ResourceDB_Dump( );
//	SLF_Dump("slf-dump.txt");
//	SLF_Debug();
	
	Profile_GetGameHomeDirectory( gzHomePath );
	gzWorkPath[0] = 0;
	
	STR_SPrintf( gzTempPath, STRLEN(gzTempPath), "%s%c", TEMP_DIR_PREFIX, SLASH );
	printf("Using game temp dir: %s\n", gzTempPath);
	printf("Using game home dir: %s\n", gzHomePath);
	printf("Using game profile dir: %s\n", gzWorkPath);

	OpenedFiles.clear();
	
	// index 0 of file handlers, returned by fileman, is reserved for error reporting
	OpenedFiles.resize(1);
	OpenedFiles[0].isFree = FALSE;
	RegisterDebugTopic( TOPIC_FILE_MANAGER, "File Manager" );
	return( TRUE );
}



//**************************************************************************
//
// FileSystemShutdown
//
//		Shuts down the file system.
//
// Parameter List :
// Return Value :
// Modification history :
//
//		24sep96:HJH		-> creation
//
//		9 Feb 98	DEF - modified to work with the library system
//
//**************************************************************************

void ShutdownFileManager( void )
{
	OpenedFiles.clear();
	ResourceDB_Shutdown();
	// todo: $$$ - clean temp dir
	UnRegisterDebugTopic( TOPIC_FILE_MANAGER, "File Manager" );
}


//**************************************************************************
//
// FileExists
//
//		Checks if a file exists.
//
// Parameter List :
//
//		STR	-> name of file to check existence of
//
// Return Value :
//
//		BOOLEAN	-> TRUE if it exists
//					-> FALSE if not
//
// Modification history :
//
//		24sep96:HJH		-> creation
//
//		9 Feb 98	DEF - modified to work with the library system
//
//    Oct 2005: Snap - Rewrote, made to check data catalogues
//
//**************************************************************************
BOOLEAN	FileExists( STR strFilename )
{
	STRING512	name;

	strncpy(name, strFilename, STRLEN(name));
	LowercaseAndChangeSlash(name);
//	FDEBUG("FileExists: %s\n", name);
	if ( PHYSFS_exists(name) ) return TRUE;
	return SLF_IsExist(name);
}

//**************************************************************************
//
// FileExistsNoDB
//
//		Checks if a file exists, but doesn't check the database files.
//
// Parameter List :
//
//		STR	-> name of file to check existence of
//
// Return Value :
//
//		BOOLEAN	-> TRUE if it exists
//					-> FALSE if not
//
// Modification history :
//
//		24sep96:HJH		-> creation
//
//    Oct 2005: Snap - Rewrote, made to check data catalogues
//
//**************************************************************************
BOOLEAN	FileExistsNoDB( STR strFilename )
{
	STRING512	name;

	strncpy(name, strFilename, STRLEN(name));
	LowercaseAndChangeSlash(name);
	FDEBUG("FileExistsNoDB: %s\n", name);
	return PHYSFS_exists(name) != 0;
}

//**************************************************************************
//
// FileDelete
//
//		Deletes a file.
//
// Parameter List :
//
//		STR	-> name of file to delete
//
// Return Value :
//
//		BOOLEAN	-> TRUE if successful
//					-> FALSE if not
//
// Modification history :
//
//		24sep96:HJH		-> creation
//
//**************************************************************************	
BOOLEAN	FileDelete( const CHAR8 *strFilename )
{
	STRING512	name;
	BOOLEAN		result;

	strncpy(name, strFilename, STRLEN(name));
	LowercaseAndChangeSlash(name);
	FDEBUG("FileDelete: %s\n", name);
	result = PHYSFS_delete(name) != 0;
	if ( !result )
	{
		fprintf(stderr, "Fileman: error in FileDelete('%s'). Reason: %s\n", name, PHYSFS_getLastError() );
	}
	return result;
}

//**************************************************************************
//
// FileOpen
//
//		Opens a file.
//
// Parameter List :
//
//		STR	   -> filename
//		UIN32		-> access - read or write, or both
//		BOOLEAN	-> delete on close
//
// Return Value :
//
//		HWFILE	-> handle of opened file
//
// Modification history :
//
//		24sep96:HJH		-> creation
//
//		9 Feb 98	DEF - modified to work with the library system
//
//    Oct 2005: Snap - modified to work with the custom Data directory
//
//**************************************************************************
HWFILE FileOpen( STR strFilename, UINT32 uiOptions, BOOLEAN fDeleteOnClose )
{
	STRING512		name;
	fsFile			handler;
	slfHandler		slfResource;
	BOOLEAN			useSLF = FALSE;
	BOOLEAN			result;
	FilemanEntry	fileObject;
	INT32			freeSlot;

	strncpy(name, strFilename, STRLEN(name));
	LowercaseAndChangeSlash(name);
	FDEBUG("FileOpen: %s, %04X\n", name, uiOptions);

	if (uiOptions & FILE_ACCESS_WRITE)
	{
		// open for write only through PhysFS
		handler = PHYSFS_openWrite(name);
		result = handler != NULL;
		useSLF = FALSE;
	}
	else
	{
		// open for read through PhysFS first, then try SLF
		handler = PHYSFS_openRead(name);
		if ( handler )
		{
			result = TRUE;
			useSLF = FALSE;
		}
		else
		{
			slfResource = SLF_OpenResource(name);
			result = SLF_IsValidHandle(slfResource);
			useSLF = TRUE;
		}
	}
	
	// check, if we succeeded or not?
	if ( !result )
	{
		// bad luck. let's report it
		if ( useSLF ) fprintf(stderr, "Fileman: SLF error in FileOpen('%s').\n", name );
		else fprintf(stderr, "Fileman: PhysFS error in FileOpen('%s'). Reason: %s\n", name, PHYSFS_getLastError() );
		return 0;
	}

	// fill fileman entry struct
	fileObject.isFree = FALSE;
	fileObject.isSLF  = useSLF;
	if ( fileObject.isSLF )
		fileObject.handle.slf = slfResource;
	else
		fileObject.handle.file = handler;
	
	// add file object to opened files array
	freeSlot = FindFreeOpenedFilesSlot();
	if ( freeSlot == -1 )
	{
		freeSlot = OpenedFiles.size();
		OpenedFiles.push_back( fileObject );
	}
	else
	{
		OpenedFiles[ freeSlot ] = fileObject;
	}
	
	return freeSlot;
}



//**************************************************************************
//
// FileClose
//
//
// Parameter List :
//
//		HWFILE hFile	-> handle to file to close
//
// Return Value :
// Modification history :
//
//		24sep96:HJH		-> creation
//
//		9 Feb 98	DEF - modified to work with the library system
//
//**************************************************************************

void FileClose( HWFILE hFile )
{
	if ( hFile < 0 || hFile >= OpenedFiles.size() )
	{
		fprintf(stderr, "FileClose: file descriptor out of bounds\n");
		return;
	}
	
	if ( OpenedFiles[ hFile ].isSLF )
	{
		SLF_CloseResource( OpenedFiles[ hFile ].handle.slf );
	}
	else
	{
		PHYSFS_close( OpenedFiles[ hFile ].handle.file );
	}
	
	OpenedFiles[ hFile ].isFree = TRUE;
}

//**************************************************************************
//
// FileRead
//
//		To read a file.
//
// Parameter List :
//
//		HWFILE		-> handle to file to read from
//		void	*	-> source buffer
//		UINT32	-> num bytes to read
//		UINT32	-> num bytes read
//
// Return Value :
//
//		BOOLEAN	-> TRUE if successful
//					-> FALSE if not
//
// Modification history :
//
//		24sep96:HJH		-> creation
//		08Dec97:ARM		-> return FALSE if bytes to read != bytes read
//
//		9 Feb 98	DEF - modified to work with the library system
//
//**************************************************************************

#ifdef JA2TESTVERSION
	extern UINT32 uiTotalFileReadTime;
	extern UINT32 uiTotalFileReadCalls;
	#include "timer_control.h"
#endif


BOOLEAN FileRead( HWFILE hFile, PTR pDest, UINT32 iBytesToRead, UINT32 *piBytesRead )
{
	INT32	bytesHasBeenRead;
	
	if ( hFile < 0 || hFile >= OpenedFiles.size() )
	{
		fprintf(stderr, "FileRead: file descriptor out of bounds\n");
		return FALSE;
	}
	
	if ( OpenedFiles[ hFile ].isSLF )
	{
		bytesHasBeenRead = SLF_Read( OpenedFiles[ hFile ].handle.slf, pDest, iBytesToRead );
	}
	else
	{
		bytesHasBeenRead = PHYSFS_read( OpenedFiles[ hFile ].handle.file, pDest, 1, iBytesToRead );
	}
	
	if ( piBytesRead )
		*piBytesRead = bytesHasBeenRead;
		
	return iBytesToRead == bytesHasBeenRead;
}

//**************************************************************************
//
// FileWrite
//
//		To write a file.
//
// Parameter List :
//
//		HWFILE		-> handle to file to write to
//		void	*	-> destination buffer
//		UINT32	-> num bytes to write
//		UINT32	-> num bytes written
//
// Return Value :
//
//		BOOLEAN	-> TRUE if successful
//					-> FALSE if not
//
// Modification history :
//
//		24sep96:HJH		-> creation
//		08Dec97:ARM		-> return FALSE if dwNumBytesToWrite != dwNumBytesWritten
//
//		9 Feb 98	DEF - modified to work with the library system
//
//**************************************************************************

BOOLEAN FileWrite( HWFILE hFile, PTR pDest, UINT32 iBytesToWrite, UINT32 *piBytesWritten )
{
	INT32	bytesHasBeenWritten;
	
	if ( hFile < 0 || hFile >= OpenedFiles.size() )
	{
		fprintf(stderr, "FileWrite: file descriptor out of bounds\n");
		return FALSE;
	}
	
	if ( !OpenedFiles[ hFile ].isSLF )
	{
		bytesHasBeenWritten = PHYSFS_write( OpenedFiles[ hFile ].handle.file, pDest, 1, iBytesToWrite );
	}
	
	if ( piBytesWritten )
		*piBytesWritten = bytesHasBeenWritten;
	
	return iBytesToWrite == bytesHasBeenWritten;
}


//**************************************************************************
//
// FilePrintf
//
//		To printf to a file.
//
// Parameter List :
//
//		HWFILE	-> handle to file to seek in
//		...		-> arguments, 1st of which should be a string
//
// Return Value :
//
//		BOOLEAN	-> TRUE if successful
//					-> FALSE if not
//
// Modification history :
//
//		24sep96:HJH		-> creation
//
//		9 Feb 98	DEF - modified to work with the library system
//
//**************************************************************************
BOOLEAN FilePrintf( HWFILE hFile, CHAR8 * strFormatted, ... )
{
	STRING512	strToSend;
	va_list		argptr;

	if ( hFile < 0 || hFile >= OpenedFiles.size() )
	{
		fprintf(stderr, "FilePrintf: file descriptor out of bounds\n");
		return FALSE;
	}

	va_start(argptr, strFormatted);
	vsprintf( strToSend, strFormatted, argptr );
	va_end(argptr);

	FileWrite( hFile, strToSend, strlen(strToSend) + 1, NULL );
	return TRUE;
}

//**************************************************************************
//
// FileSeek
//
//		To seek to a position in a file.
//
// Parameter List :
//
//		HWFILE	-> handle to file to seek in
//		UINT32	-> distance to seek
//		UINT8		-> how to seek
//
// Return Value :
//
//		BOOLEAN	-> TRUE if successful
//					-> FALSE if not
//
// Modification history :
//
//		24sep96:HJH		-> creation
//
//		9 Feb 98	DEF - modified to work with the library system
//
//**************************************************************************

BOOLEAN FileSeek( HWFILE hFile, INT32 iDistance, UINT8 uiHow )
{
	INT32	moveMethod;
	INT32	position;
	BOOLEAN	seekResult;
	
	if ( hFile < 0 || hFile >= OpenedFiles.size() )
	{
		fprintf(stderr, "FileSeek: file descriptor out of bounds\n");
		return FALSE;
	}

	if ( OpenedFiles[ hFile ].isSLF )
	{
		seekResult = SLF_Seek( OpenedFiles[ hFile ].handle.slf, iDistance, uiHow );
	}
	else
	{
		switch( uiHow )
		{
		case FILE_SEEK_FROM_START:
			position = iDistance;
			break;
		
		case FILE_SEEK_FROM_END:
			position = PHYSFS_fileLength(OpenedFiles[ hFile ].handle.file) - iDistance;
			break;
		
		case FILE_SEEK_FROM_CURRENT:
			position = PHYSFS_tell(OpenedFiles[ hFile ].handle.file) + iDistance;
		}
		seekResult = PHYSFS_seek( OpenedFiles[ hFile ].handle.file, position ) != 0;
	}
	
	return seekResult;
}

//**************************************************************************
//
// FileGetPos
//
//		To get the current position in a file.
//
// Parameter List :
//
//		HWFILE	-> handle to file
//
// Return Value :
//
//		INT32		-> current offset in file if successful
//					-> -1 if not
//
// Modification history :
//
//		24sep96:HJH		-> creation
//
//		9 Feb 98	DEF - modified to work with the library system
//
//**************************************************************************

INT32 FileGetPos( HWFILE hFile )
{
	INT32	filePos = -1;
	
	if ( hFile < 0 || hFile >= OpenedFiles.size() )
	{
		fprintf(stderr, "FileGetPos: file descriptor out of bounds\n");
		return 0;
	}
	
	if ( OpenedFiles[ hFile ].isSLF )
	{
		filePos = SLF_Tell( OpenedFiles[ hFile ].handle.slf );
	}
	else
	{
		filePos = PHYSFS_tell( OpenedFiles[ hFile ].handle.file );
	}
	
	return filePos;
}

//**************************************************************************
//
// FileGetSize
//
//		To get the current file size.
//
// Parameter List :
//
//		HWFILE	-> handle to file
//
// Return Value :
//
//		INT32		-> file size in file if successful
//					-> 0 if not
//
// Modification history :
//
//		24sep96:HJH		-> creation
//
//		9 Feb 98	DEF - modified to work with the library system
//
//**************************************************************************

INT32 FileGetSize( HWFILE hFile )
{
	INT32	fileSize = 0;
	
	if ( hFile < 0 || hFile >= OpenedFiles.size() )
	{
		fprintf(stderr, "FileGetSize: file descriptor out of bounds\n");
		return 0;
	}
	
	if ( OpenedFiles[ hFile ].isSLF )
	{
		fileSize = SLF_Size_Handler( OpenedFiles[ hFile ].handle.slf );
	}
	else
	{
		fileSize = PHYSFS_fileLength( OpenedFiles[ hFile ].handle.file );
	}
	
	return fileSize;
}

//**************************************************************************
//
// FileSize()
//		Get a filesize by filename.
// Parameter List :
//		strFilename - filename
// Return Value :
//		UINT32 - file size in bytes
//
//**************************************************************************
INT32 FileSize(STR strFilename)
{
	INT32		fileSize = 0;
	fsFile		file;
	STRING512	name;

	strncpy(name, strFilename, STRLEN(name));
	LowercaseAndChangeSlash(name);
	FDEBUG("FileSize: %s\n", name);

	// first, try to open it using PhysFS
	// then try SLF	
	file = PHYSFS_openRead(name);
	if ( file )
	{
		fileSize = PHYSFS_fileLength(file);
	}
	else
	{
		fileSize = SLF_Size_Filename(name);
	}
	
	return fileSize;
}


//**************************************************************************
//
// FileCheckEndOfFile()
//		Returns true if file pointer is at end of file, else false
// Parameter List :
//		hFile - file handler
// Return Value :
//		BOOLEAN - TRUE if end of file, FALSE if not
//
//**************************************************************************
BOOLEAN	FileCheckEndOfFile( HWFILE hFile )
{
	BOOLEAN	isEOF;
	
	if ( hFile < 0 || hFile >= OpenedFiles.size() )
	{
		fprintf(stderr, "FileCheckEndOfFile: file descriptor out of bounds\n");
		return TRUE; // return true to avoid dead loops
	}
	
	if ( OpenedFiles[ hFile ].isSLF )
	{
		isEOF = SLF_IsEOF( OpenedFiles[ hFile ].handle.slf );
	}
	else
	{
		isEOF = PHYSFS_eof(OpenedFiles[ hFile ].handle.file) != 0;
	}
	
	return( isEOF );
}



BOOLEAN SetFileManCurrentDirectory( STR pcDirectory )
{
	return( IO_Dir_SetCurrentDirectory( pcDirectory ) );
}


BOOLEAN GetFileManCurrentDirectory( STRING512 pcDirectory )
{
	if ( !IO_Dir_GetCurrentDirectory(pcDirectory, 511 ) )
	{
		fprintf(stderr, "Error getting dir %s: errno=%d\n",
			pcDirectory, errno);
		return FALSE;
	}
	return( TRUE );
}


BOOLEAN DirectoryExists( STRING512 pcDirectory )
{
	STRING512	name;

	strncpy(name, pcDirectory, STRLEN(name));
	LowercaseAndChangeSlash(name);
	FDEBUG("DirectoryExists: %s\n", name);
	return PHYSFS_isDirectory(name) != 0;
}


BOOLEAN MakeFileManDirectory( STRING512 pcDirectory )
{
	STRING512	name;
	INT32		lastChar;

	strncpy(name, pcDirectory, STRLEN(name));
	LowercaseAndChangeSlash(name);
	FDEBUG("MakeFileManDirectory: %s\n", name);

	// PhysFS doesn't like creating dirs with the slash at end - so remove it
	lastChar = strlen(name) - 1;
	if ( name[lastChar] == '/' )
		name[lastChar] = 0;
	
	if ( DirectoryExists(name) )
		return TRUE;

	if ( PHYSFS_mkdir(name) == 0 )
	{
		fprintf(stderr, "Fileman: PhysFS error in MakeFileManDirectory('%s'). Reason: %s\n", name, PHYSFS_getLastError() );
		return FALSE;
	}
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Removes ALL FILES in the specified directory (and all subdirectories with their files if fRecursive is TRUE)
// Use EraseDirectory() to simply delete directory contents without deleting the directory itself
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOLEAN RemoveFileManDirectory( STRING512 pcDirectory, BOOLEAN fRecursive )
{
	CHAR8 **rc;
	CHAR8 **i;
	STRING512	name, entry;
	INT32		lastChar;

	strncpy(name, pcDirectory, STRLEN(name));
	LowercaseAndChangeSlash(name);
	FDEBUG("RemoveFileManDirectory: %s\n", name);

	// PhysFS doesn't like handling dirs with the slash at end - so remove it
	lastChar = strlen(name) - 1;
	if ( name[lastChar] == '/' )
		name[lastChar] = 0;

	rc = PHYSFS_enumerateFiles(name);
	for (i = rc; *i != NULL; i++)
	{
		STR_SPrintf(entry, STRLEN(entry), "%s%s%s", name, GetDirSeparator(), *i);
		if ( PHYSFS_isDirectory(entry) )
		{
			// only go in if the fRecursive flag is TRUE (like Deltree)
			if (fRecursive)
			{
				RemoveFileManDirectory(entry, TRUE);
			}
		}
		else
		{
			// delete a file
			PHYSFS_delete(entry);
		}
	}

	PHYSFS_freeList(rc);
	PHYSFS_delete(name);
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Removes ALL FILES in the specified directory but leaves the directory alone.  Does not affect any subdirectories!
// Use RemoveFilemanDirectory() to also delete the directory itself, or to recursively delete subdirectories.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOLEAN EraseDirectory( STRING512 pcDirectory)
{
	CHAR8 **rc;
	CHAR8 **i;
	STRING512	name, entry;
	INT32		lastChar;

	strncpy(name, pcDirectory, STRLEN(name));
	LowercaseAndChangeSlash(name);
	FDEBUG("EraseDirectory: %s\n", name);

	// PhysFS doesn't like handling dirs with the slash at end - so remove it
	lastChar = strlen(name) - 1;
	if ( name[lastChar] == '/' )
		name[lastChar] = 0;

	rc = PHYSFS_enumerateFiles(name);
	for (i = rc; *i != NULL; i++)
	{
		STR_SPrintf(entry, STRLEN(entry), "%s%s%s", name, GetDirSeparator(), *i);
		if ( !PHYSFS_isDirectory(entry) )
		{
			// delete a file
			PHYSFS_delete(entry);
		}
	}

	PHYSFS_freeList(rc);
	return TRUE;
}

#if 0
BOOLEAN GetExecutableDirectory( STRING512 pcDirectory )
{
	SGPFILENAME	ModuleFilename;
	UINT32 cnt;

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	if ( GetModuleFileName( NULL, ModuleFilename, sizeof( ModuleFilename ) ) == 0 )
	{
		return( FALSE );
	}

	// Now get directory
	strcpy( pcDirectory, ModuleFilename );

	for ( cnt = strlen( pcDirectory ) - 1; cnt >= 0; cnt -- )
	{
		if ( pcDirectory[ cnt ] == '\\' )
		{
			 pcDirectory[ cnt ] = '\0';
			 break;
		}
	}

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------
	fprintf(stderr, "GetExecutableDirectory()\n");
//	strncpy( pcDirectory, gzDataPath, 512);
	strncpy( pcDirectory, gzHomePath, 512);
// -------------------- End of Linux-specific stuff ------------------------
#endif	

	return( TRUE );
}
#endif

BOOLEAN GetHomeDirectory( STRING512 pcDirectory )
{
	strncpy( pcDirectory, gzHomePath, 512);
	return( TRUE );
}

BOOLEAN GetTempDirectory( STRING512 pcDirectory )
{
	strncpy( pcDirectory, gzTempPath, 512);
	return TRUE;
}

BOOLEAN GetWorkDirectory( STRING512 pcDirectory )
{
	strncpy( pcDirectory, gzWorkPath, 512);
	return TRUE;
}

//**************************************************************************
//
// GetFileFirst()
//		Start file search, that suits to our pattern (e.g. *.jsd, *.*)
// Parameter List :
//		pSpec - path and/or mask
//		pGFStruct - SGP file find info (and handler)
// Return Value :
//		BOOLEAN - TRUE if end of file, FALSE if not
//
//**************************************************************************
BOOLEAN GetFileFirst( CHAR8 * pSpec, GETFILESTRUCT *pGFStruct )
{
	STRING512	name, directory, mask, entry;
	BOOLEAN fFound;
	CHAR8	*lastSlash;
	INT32	x, iWhich=0;

	CHECKF( pSpec != NULL );
	CHECKF( pGFStruct != NULL );

	strncpy(name, pSpec, STRLEN(name));
	LowercaseAndChangeSlash(name);
	FDEBUG("GetFileFirst: %s\n", name);

	// split pSpec into directories and mask
	lastSlash = strrchr( name, '/' );
	if ( !lastSlash )
	{
		// special case - pSpec goes without directories, only mask
		directory[0] = 0;
		strncpy(mask, name, STRLEN(mask));
	}
	else
	{
		strncpy(mask, (lastSlash + 1), STRLEN(mask));
		*lastSlash = 0;
		strncpy(directory, name, STRLEN(directory));
	}
	
	// find free search slot
	fFound = FALSE;
	for( x = 0; x < 20 && !fFound; x++)
	{
		if( !fFindInfoInUse[x] )
		{
			iWhich = x;
			fFound = TRUE;
		}
	}

	if ( !fFound )
		return(FALSE);

	pGFStruct->iFindHandle = iWhich;
//	pGFStruct->fUseVFS     = FALSE;
	pGFStruct->list        = PHYSFS_enumerateFiles(directory);
	pGFStruct->iterator    = pGFStruct->list;
	pGFStruct->mask        = (CHAR8*) MemAlloc( strlen(mask)+1 );
	pGFStruct->directory   = (CHAR8*) MemAlloc( strlen(directory)+1 );

	if ( !pGFStruct->mask )
		return FALSE;
	if ( !pGFStruct->directory )
		return FALSE;

	strcpy(pGFStruct->mask, mask);
	strcpy(pGFStruct->directory, directory);

	// get the first file, that matches the pattern
	if ( !GetFileNext(pGFStruct) )
		return FALSE;
	
	fFindInfoInUse[iWhich] = TRUE;
	return TRUE;
}

//**************************************************************************
//
// GetFileNext()
//		Get next file, that suits to our pattern.
// Parameter List :
//		pGFStruct - SGP file find info (and handler)
// Return Value :
//		BOOLEAN - TRUE if end of file, FALSE if not
//
//**************************************************************************
BOOLEAN GetFileNext( GETFILESTRUCT *pGFStruct )
{
	BOOLEAN	found = FALSE;
	
	while ( (*pGFStruct->iterator != NULL) && !found )
	{
		if ( DoesFilenameMatchesPattern( pGFStruct->mask, *pGFStruct->iterator ) )
		{
			STR_SPrintf(pGFStruct->zFileName, 260, "%s%s%s", pGFStruct->directory, GetDirSeparator(), *pGFStruct->iterator );
			found = TRUE;
		}
		pGFStruct->iterator++;
	}
	return found;
}


//**************************************************************************
//
// GetFileClose()
//		Shut up file pattern searching. Release searching resources.
// Parameter List :
//		pGFStruct - SGP file find info (and handler)
//
//**************************************************************************
void GetFileClose( GETFILESTRUCT *pGFStruct )
{
	if ( pGFStruct == NULL )
		return;
	
	PHYSFS_freeList(pGFStruct->list);
	MemFree(pGFStruct->mask);
	MemFree(pGFStruct->directory);
	fFindInfoInUse[pGFStruct->iFindHandle] = FALSE;
}

BOOLEAN  FileIsValidHandle( HWFILE hFile )
{
	return hFile != 0;
}
