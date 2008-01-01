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
	#include "library_database.h"
	#include "vfs.hpp"
	#include "io_layer.h"
	#include "sgp_str.h"
	#include "wcheck.h"
	#include "app_env.h"
	#include "profile_man.h"
	
#endif
//**************************************************************************
//
//				Defines
//
//**************************************************************************

#define FILENAME_LENGTH					600

#define PRINT_DEBUG_INFO	FileDebugPrint();

//**************************************************************************
//
//				Typedefs
//
//**************************************************************************

//**************************************************************************
//
//				Variables
//
//**************************************************************************

#define GAME_DIR_PREFIX			"ja2-1.13"
#define TEMP_DIR_PREFIX			".temp"

//The FileDatabaseHeader
DatabaseManagerHeaderStruct gFileDataBase;


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
	BOOLEAN		isVFS;
	union
	{
		IOFILE	file;
		INT32	vfsIndex;
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

void	FileDebugPrint( void );
HANDLE	GetHandleToRealFile( HWFILE hFile, BOOLEAN *pfDatabaseFile );

//**************************************************************************
//
//				Functions
//
//**************************************************************************

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
	
	InitializeProfileManager();
	
	Profile_GetGameHomeDirectory( gzHomePath );

	Profile_GetGameProfileDirectory( gzWorkPath );
	STR_SPrintf( gzTempPath, STRLEN(gzTempPath), "%s%s%c", gzWorkPath, TEMP_DIR_PREFIX, SLASH );
	printf("Using game temp dir: %s\n", gzTempPath);

	if ( !IO_Dir_DirectoryExists( gzTempPath ) )
		if ( !IO_Dir_MakeDirectory( gzTempPath ) )
		{
			fprintf(stderr, "Unable to create game temp directory '%s'\n", gzTempPath);
			return FALSE;
		}
	
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
	// todo: $$$ - clean temp dir
	UnRegisterDebugTopic( TOPIC_FILE_MANAGER, "File Manager" );
}

//**************************************************************************
//
// 	PathBackslash
//
//		Convert path to unix-style.
//
// Parameter List :	path - input string and output.
// Return Value :	always TRUE.
// Modification history :
//
//		22mar07:Lesh		-> creation
//
//**************************************************************************
BOOLEAN PathBackslash(STR path)
{
	STR src = path;

	while( (src = strchr(src, '\\')) )
	{
		*src = SLASH;
	}

	return TRUE;
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
	if ( IO_IsRootPath( strFilename ) )
		return IO_IsRegularFile( strFilename );

	return VFS.IsFileExist( strFilename );
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
//	vfsEntry	entry;

//	if ( VFS.FindResource( strFilename, entry ) )
//		return (entry.LibIndex == LIB_REAL_FILE);

	if ( IO_IsRootPath( strFilename ) )
		return IO_IsRegularFile( strFilename );

	return FALSE;
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
	if ( IO_IsRootPath( strFilename ) )
		return( IO_File_Delete( strFilename ) );
	return FALSE;
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
#ifdef VFS2
	INT32			access;
	INT32			freeSlot;
	FilemanEntry	fileObject;
	BOOLEAN			isRootPath;
	BOOLEAN			openSuccess;
	
//	printf("Request to open %s...\n", strFilename);
	isRootPath = IO_IsRootPath( strFilename );
	
	if ( !isRootPath && (uiOptions & FILE_ACCESS_WRITE) )
	{
		fprintf(stderr, "FileOpen: non-root path with write access detected (%s)\n", strFilename);
		return 0;
	}

	// parse access option
	if ( (uiOptions & FILE_ACCESS_READ) && (uiOptions & FILE_ACCESS_WRITE) )
		access = IO_ACCESS_READWRITE;
	else if ( uiOptions & FILE_ACCESS_READ )
		access = IO_ACCESS_READ;
	else if ( uiOptions & FILE_ACCESS_WRITE )
		access = IO_ACCESS_WRITE;

	if ( uiOptions & FILE_CREATE_NEW )
		access |= IO_CREATE_NEW;
	if ( uiOptions & FILE_CREATE_ALWAYS )
		access |= IO_CREATE_ALWAYS;

	fileObject.isFree = FALSE;
	fileObject.isVFS  = !isRootPath;
	
	// open files
	if ( fileObject.isVFS )
	{
		fileObject.handle.vfsIndex = VFS.Open( strFilename );
		openSuccess = (fileObject.handle.vfsIndex != -1);
	}
	else
	{
		fileObject.handle.file = IO_File_Open( strFilename, access );
		openSuccess = (fileObject.handle.file != -1);
	}
	
	// if there was an error
	if ( !openSuccess )
	{
		return 0;
	}
	
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
#else
	HWFILE	hFile;
	HANDLE	hRealFile;
	UINT32	dwAccess;
	UINT32	dwFlagsAndAttributes;

	hFile = 0;
	dwAccess = 0;
	
	if ( (uiOptions & FILE_ACCESS_READ) && (uiOptions & FILE_ACCESS_WRITE) )
		dwAccess = IO_ACCESS_READWRITE;
	else if ( uiOptions & FILE_ACCESS_READ )
		dwAccess = IO_ACCESS_READ;
	else if ( uiOptions & FILE_ACCESS_WRITE )
		dwAccess = IO_ACCESS_WRITE;

	if ( uiOptions & FILE_CREATE_NEW )
		dwAccess |= IO_CREATE_NEW;
	if ( uiOptions & FILE_CREATE_ALWAYS )
		dwAccess |= IO_CREATE_ALWAYS;

	vfsEntry	entry;

//	printf("Request to open %s...\n", strFilename);
	if ( !VFS.FindResource( strFilename, entry ) )
	{
		entry.LibIndex = LIB_REAL_FILE;
		entry.IsWriteable = TRUE;
		entry.IsDirectory = FALSE;
		entry.RealName = strFilename;
	}

	// where is resource ?
	if ( entry.LibIndex != LIB_REAL_FILE )
	{
		// resource is in container
		if ( gFileDataBase.fInitialized ) 
		{
//			printf("Opening file from container\n" );
			hFile = OpenFileFromLibrary( entry.RealName.c_str() );
		}
	}
	else
	{
		// resource is on file system
//		printf("Opening non-mapped resource %s... (0x%04X)\n", entry.RealName.c_str(), dwAccess);
		hRealFile = IO_File_Open( entry.RealName.c_str(), dwAccess );

		if ( hRealFile == -1 )
		{
			fprintf(stderr, "Error opening/creating file %s: %s\n", entry.RealName.c_str(), strerror(errno) );
			return(0);
		}

		hFile = CreateRealFileHandle( hRealFile );
	}

	if ( !hFile )
		return(0);
	
	return(hFile);
#endif
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
#ifdef VFS2
	if ( hFile < 0 || hFile >= OpenedFiles.size() )
	{
		fprintf(stderr, "FileClose: file descriptor out of bounds\n");
		return;
	}
	
	if ( OpenedFiles[ hFile ].isVFS )
	{
		VFS.Close( OpenedFiles[ hFile ].handle.vfsIndex );
	}
	else
	{
		IO_File_Close( OpenedFiles[ hFile ].handle.file );
	}
	
	OpenedFiles[ hFile ].isFree = TRUE;
#else
	INT16 sLibraryID;
	UINT32 uiFileNum;

	GetLibraryAndFileIDFromLibraryFileHandle( hFile, &sLibraryID, &uiFileNum );

	//if its the 'real file' library
	if( sLibraryID == REAL_FILE_LIBRARY_ID )
	{
		//if its not already closed
		if( gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].uiFileID != 0 )
		{
			if ( !IO_File_Close( gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle ) )
			{
				fprintf(stderr, "Error closing file %d: errno=%d\n",
					gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle, errno);
			}

			gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].uiFileID = 0;
			gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle= 0;
			gFileDataBase.RealFiles.iNumFilesOpen--;
			if( gFileDataBase.RealFiles.iNumFilesOpen < 0 )
			{
				//if for some reason we are below 0, report an error ( should never be )
				Assert( 0 );
			}
		}
	}
	else
	{
		//if the database is initialized
		if( gFileDataBase.fInitialized )
			CloseLibraryFile( sLibraryID, uiFileNum );
	}
#endif
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
#ifdef VFS2
	INT32	bytesHasBeenRead;
	
	if ( hFile < 0 || hFile >= OpenedFiles.size() )
	{
		fprintf(stderr, "FileRead: file descriptor out of bounds\n");
		return FALSE;
	}
	
	if ( OpenedFiles[ hFile ].isVFS )
	{
		bytesHasBeenRead = VFS.Read( OpenedFiles[ hFile ].handle.vfsIndex, pDest, iBytesToRead );
	}
	else
	{
		bytesHasBeenRead = IO_File_Read( OpenedFiles[ hFile ].handle.file, pDest, iBytesToRead );
	}
	
	if ( piBytesRead )
		*piBytesRead = bytesHasBeenRead;
		
	return iBytesToRead == bytesHasBeenRead;
#else
	HANDLE	hRealFile;
	UINT32		dwNumBytesToRead, dwNumBytesRead;
	BOOLEAN	fRet = FALSE;
	INT16 sLibraryID;
	UINT32 uiFileNum;

#ifdef JA2TESTVERSION
	UINT32 uiStartTime = GetJA2Clock();
#endif

	//init the variables
	dwNumBytesToRead = dwNumBytesRead = 0;

	GetLibraryAndFileIDFromLibraryFileHandle( hFile, &sLibraryID, &uiFileNum );

	dwNumBytesToRead	= iBytesToRead;

	//if its a real file, read the data from the file
	if( sLibraryID == REAL_FILE_LIBRARY_ID )
	{
		//if the file is opened
		if( uiFileNum != 0 )
		{
			hRealFile = gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle;
			dwNumBytesRead = IO_File_Read( hRealFile, pDest, dwNumBytesToRead);
			
			if ( piBytesRead )
				*piBytesRead = (UINT32)dwNumBytesRead;
/*
			if ( dwNumBytesToRead != dwNumBytesRead )
			{
				fprintf(stderr, "Error reading file %d: errno=%d\n",
					hRealFile, errno);
				return FALSE;
			}
*/
			fRet = TRUE;
		}
	}
	else
	{
		//if the database is initialized
		if( gFileDataBase.fInitialized )
		{
			//if the library is open
			if( IsLibraryOpened( sLibraryID ) )
			{
				//if the file is opened
				if( gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].uiFileID != 0 )
				{
					//read the data from the library
					fRet = LoadDataFromLibrary( sLibraryID, uiFileNum, pDest, dwNumBytesToRead, (UINT32 *) &dwNumBytesRead );
					if ( piBytesRead )
					{
						*piBytesRead = (UINT32)dwNumBytesRead;
					}
				}
			}
		}
	}
	#ifdef JA2TESTVERSION
		//Add the time that we spent in this function to the total.
		uiTotalFileReadTime += GetJA2Clock() - uiStartTime;
		uiTotalFileReadCalls++;
	#endif

	return(fRet);
#endif
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
#ifdef VFS2
	INT32	bytesHasBeenWritten;
	
	if ( hFile < 0 || hFile >= OpenedFiles.size() )
	{
		fprintf(stderr, "FileWrite: file descriptor out of bounds\n");
		return FALSE;
	}
	
	if ( OpenedFiles[ hFile ].isVFS )
	{
		bytesHasBeenWritten = VFS.Write( OpenedFiles[ hFile ].handle.vfsIndex, pDest, iBytesToWrite );
	}
	else
	{
		bytesHasBeenWritten = IO_File_Write( OpenedFiles[ hFile ].handle.file, pDest, iBytesToWrite );
	}
	
	if ( piBytesWritten )
		*piBytesWritten = bytesHasBeenWritten;
	
	return iBytesToWrite == bytesHasBeenWritten;
#else
	HANDLE	hRealFile;
	UINT32		dwNumBytesToWrite, dwNumBytesWritten;
	INT16 sLibraryID;
	UINT32 uiFileNum;


	GetLibraryAndFileIDFromLibraryFileHandle( hFile, &sLibraryID, &uiFileNum );

	//if its a real file, read the data from the file
	if( sLibraryID == REAL_FILE_LIBRARY_ID )
	{
		dwNumBytesToWrite = (UINT32)iBytesToWrite;

		//get the real file handle to the file
		hRealFile = gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle;
		dwNumBytesWritten = IO_File_Write( hRealFile, pDest, dwNumBytesToWrite );

		if ( piBytesWritten )
			*piBytesWritten = (UINT32)dwNumBytesWritten;

		if ( dwNumBytesWritten != dwNumBytesToWrite )
		{
				fprintf(stderr, "Error writing file %d: errno=%d\n",
					hRealFile, errno);
			return FALSE;
		}
	
	}
	else
	{
		//we cannot write to a library file
		if ( piBytesWritten )
			*piBytesWritten = 0;
		return(FALSE);
	}

	return TRUE;
#endif
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
#ifdef VFS2
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
/*
	if ( OpenedFiles[ hFile ].isVFS )
	{
		*piBytesWritten = VFS.Write( OpenedFiles[ hFile ].handle.vfsIndex, pDest, iBytesToWrite );
	}
	else
	{
		*piBytesWritten = IO_File_Write( OpenedFiles[ hFile ].handle.file, pDest, iBytesToWrite );
	}
	
	return iBytesToWrite == *piBytesWritten;
*/
	return TRUE;
#else
	CHAR8		strToSend[80];
	va_list	argptr;
	BOOLEAN fRetVal = FALSE;
	HANDLE	hRealFile;

	INT16 sLibraryID;
	UINT32 uiFileNum;

	GetLibraryAndFileIDFromLibraryFileHandle( hFile, &sLibraryID, &uiFileNum );

	//if its a real file, read the data from the file
	if( sLibraryID == REAL_FILE_LIBRARY_ID )
	{
		va_start(argptr, strFormatted);
		vsprintf( strToSend, strFormatted, argptr );
		va_end(argptr);
		hRealFile = gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle;
		IO_File_Write( hRealFile, strToSend, strlen(strToSend) );
	}
	else
	{
		//its a library file, cant write to it so return an error
		fRetVal = FALSE;
	}

	return( fRetVal );
#endif
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
#ifdef VFS2
	INT32	moveMethod;
	BOOLEAN	seekResult;
	
	if ( hFile < 0 || hFile >= OpenedFiles.size() )
	{
		fprintf(stderr, "FileSeek: file descriptor out of bounds\n");
		return FALSE;
	}

	if ( uiHow == FILE_SEEK_FROM_START )
		moveMethod = IO_SEEK_FROM_START;
	else if ( uiHow == FILE_SEEK_FROM_END )
		moveMethod = IO_SEEK_FROM_END;
	else
		moveMethod = IO_SEEK_FROM_CURRENT;
	
	if ( OpenedFiles[ hFile ].isVFS )
	{
		seekResult = VFS.Seek( OpenedFiles[ hFile ].handle.vfsIndex, iDistance, moveMethod );
	}
	else
	{
		seekResult = IO_File_Seek( OpenedFiles[ hFile ].handle.file, iDistance, moveMethod );
	}
	
	return seekResult;
#else
	HANDLE	hRealFile;
	UINT32		dwMoveMethod;

	INT16 sLibraryID;
	UINT32 uiFileNum;

	GetLibraryAndFileIDFromLibraryFileHandle( hFile, &sLibraryID, &uiFileNum );

	//if its a real file, read the data from the file
	if( sLibraryID == REAL_FILE_LIBRARY_ID )
	{
		//Get the handle to the real file
		hRealFile = gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle;

		if ( uiHow == FILE_SEEK_FROM_START )
			dwMoveMethod = IO_SEEK_FROM_START;
		else if ( uiHow == FILE_SEEK_FROM_END )
			dwMoveMethod = IO_SEEK_FROM_END;
		else
			dwMoveMethod = IO_SEEK_FROM_CURRENT;

		if ( !IO_File_Seek( hRealFile, iDistance, dwMoveMethod ) )
		{
			fprintf(stderr, "Error seeking file %d: errno=%d\n",
				hRealFile, errno);

			return(FALSE);
		}

	}
	else
	{
		//if the database is initialized
		if( gFileDataBase.fInitialized )
			LibraryFileSeek( sLibraryID, uiFileNum, iDistance, uiHow );
	}

	return(TRUE);
#endif
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
#ifdef VFS2
	INT32	filePos = -1;
	
	if ( hFile < 0 || hFile >= OpenedFiles.size() )
	{
		fprintf(stderr, "FileGetPos: file descriptor out of bounds\n");
		return 0;
	}
	
	if ( OpenedFiles[ hFile ].isVFS )
	{
		filePos = VFS.Tell( OpenedFiles[ hFile ].handle.vfsIndex );
	}
	else
	{
		filePos = IO_File_GetPosition( OpenedFiles[ hFile ].handle.file );
	}
	
	return filePos;
#else
	HANDLE	hRealFile;
	UINT32	uiPositionInFile=0;

	INT16 sLibraryID;
	UINT32 uiFileNum;

	GetLibraryAndFileIDFromLibraryFileHandle( hFile, &sLibraryID, &uiFileNum );

	//if its a real file, read the data from the file
	if( sLibraryID == REAL_FILE_LIBRARY_ID )
	{
		//Get the handle to the real file
		hRealFile = gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle;
		uiPositionInFile = IO_File_GetPosition( hRealFile );
		if( uiPositionInFile == -1 )
		{
			fprintf(stderr, "Error getting position in file %d: errno=%d\n",
				hRealFile, errno);
			uiPositionInFile = 0;
		}
		return( uiPositionInFile );
	}
	else
	{
		//if the library is open
		if( IsLibraryOpened( sLibraryID ) )
		{
			//check if the file is open
			if( gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].uiFileID != 0 )
			{
				uiPositionInFile = gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].uiFilePosInFile;
				return( uiPositionInFile );
			}
		}
	}

	return(BAD_INDEX);
#endif
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
#ifdef VFS2
	INT32	fileSize = 0;
	
	if ( hFile < 0 || hFile >= OpenedFiles.size() )
	{
		fprintf(stderr, "FileGetSize: file descriptor out of bounds\n");
		return 0;
	}
	
	if ( OpenedFiles[ hFile ].isVFS )
	{
		fileSize = VFS.GetSize( OpenedFiles[ hFile ].handle.vfsIndex );
	}
	else
	{
		fileSize = IO_File_GetSize( OpenedFiles[ hFile ].handle.file );
	}
	
	return fileSize;
#else
	HANDLE  hRealHandle;
	INT32	iFileSize = -1;

	INT16 sLibraryID;
	UINT32 uiFileNum;

	GetLibraryAndFileIDFromLibraryFileHandle( hFile, &sLibraryID, &uiFileNum );

	//if its a real file, read the data from the file
	if( sLibraryID == REAL_FILE_LIBRARY_ID )
	{
		//Get the handle to a real file
		hRealHandle = gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle;
		iFileSize = IO_File_GetSize( hRealHandle );
	}
	else
	{
		//if the library is open
		if( IsLibraryOpened( sLibraryID ) )
		{
			if( (uiFileNum >= gFileDataBase.pLibraries[ sLibraryID ].iSizeOfOpenFileArray ) )
				iFileSize = -1;
			else if( gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].uiFileID != 0 )
				iFileSize = gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].pFileHeader->uiFileLength;
			else
				iFileSize = -1;
		}
	}


	if ( iFileSize == -1 )
		return(0);
	else
		return( iFileSize );
#endif
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
	vfsEntry	entry;

	if ( IO_IsRootPath( strFilename ) )
		return IO_File_GetSize( strFilename );

	if ( !VFS.FindResource( strFilename, entry ) )
		return 0;

	if ( entry.LibIndex == LIB_REAL_FILE )
		return IO_File_GetSize( entry.RealName.c_str() );

	return 0;
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
#ifdef VFS2
	BOOLEAN	isEOF;
	IOFILE	file;
	
	if ( hFile < 0 || hFile >= OpenedFiles.size() )
	{
		fprintf(stderr, "FileCheckEndOfFile: file descriptor out of bounds\n");
		return TRUE; // return true to avoid dead loops
	}
	
	if ( OpenedFiles[ hFile ].isVFS )
	{
		isEOF = VFS.IsEOF( OpenedFiles[ hFile ].handle.vfsIndex );
	}
	else
	{
		file  = OpenedFiles[ hFile ].handle.file;
		isEOF = ( IO_File_GetPosition(file) >= IO_File_GetSize(file) );
	}
	
	return( isEOF );
#else
	INT16 sLibraryID;
	UINT32 uiFileNum;
	HANDLE	hRealFile;

	GetLibraryAndFileIDFromLibraryFileHandle( hFile, &sLibraryID, &uiFileNum );

	//if its a real file, read the data from the file
	if( sLibraryID == REAL_FILE_LIBRARY_ID )
	{
		UINT32 uiFileSize = FileGetSize( hFile );
		UINT32 uiFilePtr  = FileGetPos( hFile );

		return ( uiFilePtr >= uiFileSize );
	}

	//else it is a library file
	else
	{
		//if the database is initialized
		if( gFileDataBase.fInitialized )
		{
			//if the library is open
			if( IsLibraryOpened( sLibraryID ) )
			{
				//if the file is opened
				if( gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].uiFileID != 0 )
				{
					UINT32	uiLength;
					UINT32	uiCurPos;

					uiLength = gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].pFileHeader->uiFileLength;
					uiCurPos = gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].uiFilePosInFile;

					//if we are trying to read more data then the size of the file, return an error
					if( uiCurPos >= uiLength )
					{
						return( TRUE );
					}
				}
			}
		}
	}

	//we are not and the end of a file
	return( 0 );
#endif
}


//**************************************************************************
//
// FileDebugPrint
//
//		To print the state of memory to output.
//
// Parameter List :
// Return Value :
// Modification history :
//
//		24sep96:HJH		-> creation
//
//**************************************************************************

void FileDebugPrint( void )
{
}

//**************************************************************************
//
// GetHandleToRealFile
//
//		
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

HANDLE GetHandleToRealFile( HWFILE hFile, BOOLEAN *pfDatabaseFile )
{
	HANDLE	hRealFile;

	INT16 sLibraryID;
	UINT32 uiFileNum;

	GetLibraryAndFileIDFromLibraryFileHandle( hFile, &sLibraryID, &uiFileNum );

	//if its a real file, read the data from the file
	if( sLibraryID == REAL_FILE_LIBRARY_ID )
	{
		//Get the handle to the real file
		hRealFile = gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle;
		*pfDatabaseFile = FALSE;
	}
	else
	{
		*pfDatabaseFile = TRUE;
		hRealFile = (HANDLE) hFile;
	}

	return(hRealFile);
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
	if ( IO_IsRootPath( pcDirectory ) )
		IO_Dir_DirectoryExists( pcDirectory );

	return VFS.IsDirExist( pcDirectory );
}


BOOLEAN MakeFileManDirectory( STRING512 pcDirectory )
{
	if ( !IO_Dir_DirectoryExists(pcDirectory) )
		if ( !IO_Dir_MakeDirectory(pcDirectory) )
		{
			fprintf(stderr, "Error creating dir %s: errno=%d\n",
				pcDirectory, errno);
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
	const CHAR8	*pFileSpec = "*";
	BOOLEAN fRetval=FALSE;
	CHAR8		zOldDir[512];
	CHAR8		zSubdirectory[512];
	sgpStringArray	ToBeDeleted;
	STRING512		entry;
	INT32			i;

	GetFileManCurrentDirectory( zOldDir );

	if( !SetFileManCurrentDirectory( pcDirectory ) )
	{
		return( FALSE );		//Error going into directory
	}

	// get directory listing
	if ( !IO_File_GetFirst( pFileSpec, entry, 512 ) )
	{
		ToBeDeleted.push_back( entry );
		while ( IO_File_GetNext( entry, 512 ) )
		{
			ToBeDeleted.push_back( entry );
		}
		IO_File_GetClose();
	}

	// now parse list - delete files and recurse for directories
	for ( i = 0; i < ToBeDeleted.size(); i++ )
	{
		if ( IO_IsRegularFile( ToBeDeleted[i].c_str() ) )
		{
			IO_File_Delete( ToBeDeleted[i].c_str() );
		}
		else if ( IO_IsDirectory( ToBeDeleted[i].c_str() ) )
		{
			// only go in if the fRecursive flag is TRUE (like Deltree)
			if (fRecursive)
			{
				sprintf(zSubdirectory, "%s%c%s", pcDirectory, SLASH, ToBeDeleted[i].c_str() );
				RemoveFileManDirectory(zSubdirectory, TRUE);
			}
		}
	}
	
	// after wiping files in dir, remove dir itself
	if( !SetFileManCurrentDirectory( zOldDir ) )
	{
		return( FALSE );		//Error returning from subdirectory
	}

	fRetval = IO_Dir_Delete( pcDirectory );
	
	return fRetval;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Removes ALL FILES in the specified directory but leaves the directory alone.  Does not affect any subdirectories!
// Use RemoveFilemanDirectory() to also delete the directory itself, or to recursively delete subdirectories.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOLEAN EraseDirectory( STRING512 pcDirectory)
{
	const CHAR8	*pFileSpec = "*";
	CHAR8		zOldDir[512];
	sgpStringArray	ToBeDeleted;
	STRING512		entry;
	INT32			i;

//	printf("EraseDirectory\n");
	
	GetFileManCurrentDirectory( zOldDir );

	if( !SetFileManCurrentDirectory( pcDirectory ) )
	{
		FastDebugMsg(String("EraseDirectory: ERROR - SetFileManCurrentDirectory on %s failed, error %d", pcDirectory, GetLastError()));
		return( FALSE );		//Error going into directory
	}

	// get directory listing
	if ( !IO_File_GetFirst( pFileSpec, entry, 512 ) )
	{
		ToBeDeleted.push_back( entry );
		while ( IO_File_GetNext( entry, 512 ) )
		{
			ToBeDeleted.push_back( entry );
		}
		IO_File_GetClose();
	}

	// now parse list - delete files and leave directories alone
	for ( i = 0; i < ToBeDeleted.size(); i++ )
	{
		if ( IO_IsRegularFile( ToBeDeleted[i].c_str() ) )
		{
//			printf("deleting %s...\n", ToBeDeleted[i].c_str());
			IO_File_Delete( ToBeDeleted[i].c_str() );
		}
	}
	
	// after wiping files in dir, remove dir itself
	if( !SetFileManCurrentDirectory( zOldDir ) )
	{
		return( FALSE );		//Error returning from subdirectory
	}

	return( TRUE );
}


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

BOOLEAN GetHomeDirectory( STRING512 pcDirectory )
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
	strncpy( pcDirectory, gzHomePath, 512);
// -------------------- End of Linux-specific stuff ------------------------
#endif	

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
	INT32 x,iWhich=0;
	BOOLEAN fFound;
	UINT32	uiQuantity = 0;
	STRING512	pattern;

	strncpy( pattern, pSpec, 511 );
	BACKSLASH(pattern);

	CHECKF( pSpec != NULL );
	CHECKF( pGFStruct != NULL );

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
	pGFStruct->fUseVFS = !IO_IsRootPath(pSpec);

//	printf("Searching resources using %s pattern\n", pSpec);

	if ( pGFStruct->fUseVFS )
	{
		uiQuantity = VFS.StartFilePatternMatch( pSpec );
//		printf("Found %d matches\n", uiQuantity);
		if ( !VFS.GetNextMatch( pGFStruct->zFileName, 260 ) )
			return FALSE;
	}
	else
	{
		if ( !IO_File_GetFirst( pSpec, pGFStruct->zFileName, 260 ) )
			return FALSE;
	}

	fFindInfoInUse[iWhich] = TRUE;

	return(TRUE);
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
	CHECKF( pGFStruct != NULL );

	if ( pGFStruct->fUseVFS )
	{
		if ( VFS.GetNextMatch( pGFStruct->zFileName, 260 ) )
			return TRUE;
	}
	else
	{
		if ( IO_File_GetNext( pGFStruct->zFileName, 260 ) )
			return TRUE;
	}

	return(FALSE);
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

	if ( pGFStruct->fUseVFS )
	{
		VFS.FinishFilePatternMatch();
	}
	else
	{
		IO_File_GetClose();
	}
	fFindInfoInUse[pGFStruct->iFindHandle] = FALSE;
}



HANDLE	GetRealFileHandleFromFileManFileHandle( HWFILE hFile )
{
	INT16 sLibraryID;
	UINT32 uiFileNum;

	GetLibraryAndFileIDFromLibraryFileHandle( hFile, &sLibraryID, &uiFileNum );

	//if its the 'real file' library
	if( sLibraryID == REAL_FILE_LIBRARY_ID )
	{
		//if its not already closed
		if( gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].uiFileID != 0 )
		{
			return( gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle );
		}
	}
	else
	{
		//if the file is not opened, dont close it
		if( gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].uiFileID != 0 )
		{
			return( gFileDataBase.pLibraries[ sLibraryID ].hLibraryHandle );
		}
	}
	return( 0 );
}
