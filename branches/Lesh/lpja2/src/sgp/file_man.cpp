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
#ifdef JA2_LINUX
// ----------------------- Linux-specific stuff ----------------------------

	char DataPath[512];
	const char* Home;
	const struct passwd* passwd = getpwuid(getuid());

	if (passwd == NULL || passwd->pw_dir == NULL)
	{
		fprintf(stderr, "Unable to locate user home directory\n");
		return FALSE;
	}

	Home = passwd->pw_dir;

	// assign home dir for game
	// data dir will be read from INI/CFG file, located in home dir
	printf("User home is %s\n", Home);

	STR_SPrintf( gzHomePath, sizeof(gzHomePath), "%s%c%s%c", Home, SLASH, GAME_DIR_PREFIX, SLASH );
	printf("Using game home dir: %s\n", gzHomePath);

	if ( !IO_Dir_DirectoryExists( gzHomePath ) )
		if ( !IO_Dir_MakeDirectory( gzHomePath ) )
		{
			fprintf(stderr, "Unable to create game home directory \"%s\"\n", gzHomePath);
			return FALSE;
		}

	STR_SPrintf( gzTempPath, sizeof(gzTempPath), "/tmp/%s.%s%c", GAME_DIR_PREFIX, passwd->pw_name, SLASH );
	printf("Using game temp dir: %s\n", gzTempPath);

	if ( !IO_Dir_DirectoryExists( gzTempPath ) )
		if ( !IO_Dir_MakeDirectory( gzTempPath ) )
		{
			fprintf(stderr, "Unable to create game temp directory \"%s\"\n", gzTempPath);
			return FALSE;
		}

// -------------------- End of Linux-specific stuff ------------------------
#endif	


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
	vfsEntry	entry;

	if ( VFS.FindResource( strFilename, entry ) )
		return (entry.LibraryID == LIB_REAL_FILE);

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
BOOLEAN	FileDelete( STR strFilename )
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
		entry.LibraryID = LIB_REAL_FILE;
		entry.IsWriteable = TRUE;
		entry.IsDirectory = FALSE;
		entry.RealName = strFilename;
	}

	// where is resource ?
	if ( entry.LibraryID != LIB_REAL_FILE )
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


BOOLEAN FileRead( HWFILE hFile, PTR pDest, UINT32 uiBytesToRead, UINT32 *puiBytesRead )
{
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

	dwNumBytesToRead	= uiBytesToRead;

	//if its a real file, read the data from the file
	if( sLibraryID == REAL_FILE_LIBRARY_ID )
	{
		//if the file is opened
		if( uiFileNum != 0 )
		{
			hRealFile = gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle;
			dwNumBytesRead = IO_File_Read( hRealFile, pDest, dwNumBytesToRead);
			
			if ( puiBytesRead )
				*puiBytesRead = (UINT32)dwNumBytesRead;

			if ( dwNumBytesToRead != dwNumBytesRead )
			{
				fprintf(stderr, "Error reading file %d: errno=%d\n",
					hRealFile, errno);
				return FALSE;
			}

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
					if ( puiBytesRead )
					{
						*puiBytesRead = (UINT32)dwNumBytesRead;
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

BOOLEAN FileWrite( HWFILE hFile, PTR pDest, UINT32 uiBytesToWrite, UINT32 *puiBytesWritten )
{
	HANDLE	hRealFile;
	UINT32		dwNumBytesToWrite, dwNumBytesWritten;
	INT16 sLibraryID;
	UINT32 uiFileNum;


	GetLibraryAndFileIDFromLibraryFileHandle( hFile, &sLibraryID, &uiFileNum );

	//if its a real file, read the data from the file
	if( sLibraryID == REAL_FILE_LIBRARY_ID )
	{
		dwNumBytesToWrite = (UINT32)uiBytesToWrite;

		//get the real file handle to the file
		hRealFile = gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle;
		dwNumBytesWritten = IO_File_Write( hRealFile, pDest, dwNumBytesToWrite );

		if ( puiBytesWritten )
			*puiBytesWritten = (UINT32)dwNumBytesWritten;

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
		if ( puiBytesWritten )
			*puiBytesWritten = 0;
		return(FALSE);
	}

	return TRUE;
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
BOOLEAN FilePrintf( HWFILE hFile, char * strFormatted, ... )
{
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

BOOLEAN FileSeek( HWFILE hFile, UINT32 uiDistance, UINT8 uiHow )
{
	HANDLE	hRealFile;
	UINT32		dwMoveMethod;
	INT32		iDistance=0;

	INT16 sLibraryID;
	UINT32 uiFileNum;

	GetLibraryAndFileIDFromLibraryFileHandle( hFile, &sLibraryID, &uiFileNum );

	//if its a real file, read the data from the file
	if( sLibraryID == REAL_FILE_LIBRARY_ID )
	{
		//Get the handle to the real file
		hRealFile = gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle;

		iDistance = (INT32) uiDistance;
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
			LibraryFileSeek( sLibraryID, uiFileNum, uiDistance, uiHow );
	}

	return(TRUE);
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

UINT32 FileGetSize( HWFILE hFile )
{
	HANDLE  hRealHandle;
	UINT32	uiFileSize = 0xFFFFFFFF;

	INT16 sLibraryID;
	UINT32 uiFileNum;

	GetLibraryAndFileIDFromLibraryFileHandle( hFile, &sLibraryID, &uiFileNum );

	//if its a real file, read the data from the file
	if( sLibraryID == REAL_FILE_LIBRARY_ID )
	{
		//Get the handle to a real file
		hRealHandle = gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle;
		uiFileSize = IO_File_GetSize( hRealHandle );
	}
	else
	{
		//if the library is open
		if( IsLibraryOpened( sLibraryID ) )
		{
			if( (uiFileNum >= (UINT32)gFileDataBase.pLibraries[ sLibraryID ].iSizeOfOpenFileArray ) )
				uiFileSize = 0xFFFFFFFF;
			else if( gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].uiFileID != 0 )
				uiFileSize = gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].pFileHeader->uiFileLength;
			else
				uiFileSize = 0xFFFFFFFF;
		}
	}


	if ( uiFileSize == 0xFFFFFFFF )
		return(0);
	else
		return( uiFileSize );
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
	BACKSLASH(pcDirectory);
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
	BACKSLASH(pcDirectory);
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
	HANDLE		SearchHandle;
	const CHAR8	*pFileSpec = "*.*";
	BOOLEAN	fDone = FALSE;
	BOOLEAN fRetval=FALSE;
	CHAR8		zOldDir[512];
	CHAR8		zSubdirectory[512];

	BACKSLASH(pcDirectory);

	GetFileManCurrentDirectory( zOldDir );

	if( !SetFileManCurrentDirectory( pcDirectory ) )
	{
		FastDebugMsg(String("RemoveFileManDirectory: ERROR - SetFileManCurrentDirectory on %s failed, error %d", pcDirectory, GetLastError()));
		return( FALSE );		//Error going into directory
	}

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	WIN32_FIND_DATA sFindData;

	//If there are files in the directory, DELETE THEM
	SearchHandle = FindFirstFile( pFileSpec, &sFindData);
	if( SearchHandle !=  INVALID_HANDLE_VALUE )
	{

		fDone = FALSE;
		do
		{
			// if the object is a directory
			if( GetFileAttributes( sFindData.cFileName ) == FILE_ATTRIBUTE_DIRECTORY )
			{
				// only go in if the fRecursive flag is TRUE (like Deltree)
				if (fRecursive)
				{
					sprintf(zSubdirectory, "%s\\%s", pcDirectory, sFindData.cFileName);

					if ((strcmp(sFindData.cFileName, ".") != 0) && (strcmp(sFindData.cFileName, "..") != 0))
					{
						if (!RemoveFileManDirectory(zSubdirectory, TRUE))
						{
				   		FastDebugMsg(String("RemoveFileManDirectory: ERROR - Recursive call on %s failed", zSubdirectory));
							break;
						}
					}
				}
				// otherwise, all the individual files will be deleted, but the subdirectories remain, causing
				// RemoveDirectory() at the end to fail, thus this function will return FALSE in that event (failure)
			}
			else
			{
				FileDelete( sFindData.cFileName );
			}

			//find the next file in the directory
			fRetval = FindNextFile( SearchHandle, &sFindData );
			if( fRetval == 0 )
			{
				fDone = TRUE;
			}
		}	while(!fDone);

		// very important: close the find handle, or subsequent RemoveDirectory() calls will fail
		FindClose( SearchHandle );
	}

	if( !SetFileManCurrentDirectory( zOldDir ) )
	{
		FastDebugMsg(String("RemoveFileManDirectory: ERROR - SetFileManCurrentDirectory on %s failed, error %d", zOldDir, GetLastError()));
		return( FALSE );		//Error returning from subdirectory
	}

	
	// The directory MUST be empty
	fRetval = RemoveDirectory( pcDirectory );
	if (!fRetval)
	{
		FastDebugMsg(String("RemoveFileManDirectory: ERROR - RemoveDirectory on %s failed, error %d", pcDirectory, GetLastError()));
	}

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------
	fprintf(stderr, "RemoveFileManDirectory(): %s, %d\n", pcDirectory, fRecursive);
// -------------------- End of Linux-specific stuff ------------------------
#endif	

	return fRetval;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Removes ALL FILES in the specified directory but leaves the directory alone.  Does not affect any subdirectories!
// Use RemoveFilemanDirectory() to also delete the directory itself, or to recursively delete subdirectories.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOLEAN EraseDirectory( STRING512 pcDirectory)
{
	HANDLE		SearchHandle;
	const CHAR8	*pFileSpec = "*.*";
	BOOLEAN	fDone = FALSE;
	CHAR8		zOldDir[512];

	BACKSLASH(pcDirectory);

	GetFileManCurrentDirectory( zOldDir );

	if( !SetFileManCurrentDirectory( pcDirectory ) )
	{
		FastDebugMsg(String("EraseDirectory: ERROR - SetFileManCurrentDirectory on %s failed, error %d", pcDirectory, GetLastError()));
		return( FALSE );		//Error going into directory
	}

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	WIN32_FIND_DATA sFindData;

	//If there are files in the directory, DELETE THEM
	SearchHandle = FindFirstFile( pFileSpec, &sFindData);
	if( SearchHandle !=  INVALID_HANDLE_VALUE )
	{

		fDone = FALSE;
		do
		{
			// if it's a file, not a directory
			if( GetFileAttributes( sFindData.cFileName ) != FILE_ATTRIBUTES_DIRECTORY )
			{
				FileDelete( sFindData.cFileName );
			}

			//find the next file in the directory
			if ( !FindNextFile( SearchHandle, &sFindData ))
			{
				fDone = TRUE;
			}
		} while(!fDone);

		// very important: close the find handle, or subsequent RemoveDirectory() calls will fail
		FindClose( SearchHandle );
	}

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------
	fprintf(stderr, "EraseDirectory(): %s\n", pcDirectory);
// -------------------- End of Linux-specific stuff ------------------------
#endif	

	// Snap: Delete the directory from the default Data catalogue (if it is there)
	// Since the path can be either relative or absolute, try both methods
//	gDefaultDataCat.RemoveDir(pcDirectory, true);
//	gDefaultDataCat.RemoveDir(pcDirectory, false);

	if( !SetFileManCurrentDirectory( zOldDir ) )
	{
		FastDebugMsg(String("EraseDirectory: ERROR - SetFileManCurrentDirectory on %s failed, error %d", zOldDir, GetLastError()));
		return( FALSE );		//Error returning from directory
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
	strncpy( pcDirectory, gzHomePath, 511);
// -------------------- End of Linux-specific stuff ------------------------
#endif	

	return( TRUE );
}

BOOLEAN GetTempDirectory( STRING512 pcDirectory )
{
	strncpy( pcDirectory, gzTempPath, 511);
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

//	printf("Searching resources using %s pattern\n", pSpec);
	uiQuantity = VFS.StartFilePatternMatch( pSpec );
//	printf("Found %d matches\n", uiQuantity);
	if ( !VFS.GetNextMatch( pGFStruct->zFileName, 260 ) )
		return FALSE;

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
	if ( VFS.GetNextMatch( pGFStruct->zFileName, 260 ) )
		return TRUE;

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
	VFS.FinishFilePatternMatch();
	fFindInfoInUse[pGFStruct->iFindHandle] = FALSE;

	return;
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
}


//**************************************************************************
//
// GetFileManFileTime()
//		Obtain file creation time, last access time and last write time.
// Parameter List :
//		hFile - file handler
//		pCreationTime - pointer to creation time
//		pLastAccessedTime - pointer to last access time
//		pLastWriteTime - pointer to last write time
// Return Value :
//		BOOLEAN - TRUE if success, FALSE if not
//
//**************************************************************************
BOOLEAN GetFileManFileTime( HWFILE hFile, SGP_FILETIME	*pCreationTime, SGP_FILETIME *pLastAccessedTime, SGP_FILETIME *pLastWriteTime )
{
	HANDLE	hRealFile;
	INT16 sLibraryID;
	UINT32 uiFileNum;

	//Initialize the passed in variables
	memset( pCreationTime,     0, sizeof( SGP_FILETIME ) );
	memset( pLastAccessedTime, 0, sizeof( SGP_FILETIME ) );
	memset( pLastWriteTime,    0, sizeof( SGP_FILETIME ) );


	GetLibraryAndFileIDFromLibraryFileHandle( hFile, &sLibraryID, &uiFileNum );

	//if its a real file, read the data from the file
	if( sLibraryID == REAL_FILE_LIBRARY_ID )
	{
		//get the real file handle to the file
		hRealFile = gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle;

		if ( !IO_File_GetTime( hRealFile, pCreationTime, pLastAccessedTime, pLastWriteTime ) )
		{
			fprintf(stderr, "Error getting time: errno=%d\n",
				errno);
			return FALSE;
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
					if( !GetLibraryFileTime( sLibraryID, uiFileNum, pLastWriteTime ) )
					{
						return( FALSE );
					}
				}
			}
		}
	}

	return( TRUE );
}


//**************************************************************************
//
// CompareSGPFileTimes()
//		Compares two times. If time1 > time2, return positive number.
//		If time1 < time2, return negative, if equal, return zero.
// Parameter List :
//		pFirstFileTime - pointer to time1
//		pSecondFileTime - pointer to time2
// Return Value :
//		INT32 - difference in times
//
//**************************************************************************
INT32	CompareSGPFileTimes( SGP_FILETIME	*pFirstFileTime, SGP_FILETIME *pSecondFileTime )
{
	return( IO_CompareTime( pFirstFileTime, pSecondFileTime ) );
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
UINT32 FileSize(STR strFilename)
{
	vfsEntry	entry;

	if ( IO_IsRootPath( strFilename ) )
		return IO_File_GetSize( strFilename );

	if ( !VFS.FindResource( strFilename, entry ) )
		return 0;

	if ( entry.LibraryID == LIB_REAL_FILE )
		return IO_File_GetSize( entry.RealName.c_str() );

	return 0;
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

BOOLEAN	FileCreateEmptyFile( STR strFilename )
{
	HANDLE	NullFile;

	NullFile = IO_File_Open( strFilename, IO_CREATE_ALWAYS | IO_ACCESS_WRITE );
	if ( NullFile == -1 )
		return FALSE;
	IO_File_Close( NullFile );

	return TRUE;
}
