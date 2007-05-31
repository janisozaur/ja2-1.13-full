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
	#include "db_man.h"
	#include "debug.h"
	#include "container.h"
	#include "library_database.h"
#endif
//**************************************************************************
//
//				Defines
//
//**************************************************************************

#define FILENAME_LENGTH					600

#define CHECKF(exp)  if (!(exp)) { return(FALSE); }
#define CHECKV(exp)  if (!(exp)) { return; }
#define CHECKN(exp)  if (!(exp)) { return(NULL); }
#define CHECKBI(exp) if (!(exp)) { return(-1); }

#define PRINT_DEBUG_INFO	FileDebugPrint();

//**************************************************************************
//
//				Typedefs
//
//**************************************************************************

typedef struct FMFileInfoTag
{
	CHAR8		strFilename[FILENAME_LENGTH];
	UINT8		uiFileAccess;
	UINT32		uiFilePosition;
	HANDLE		hFileHandle;
	HDBFILE		hDBFile;

} FMFileInfo;	// for 'File Manager File Information'

typedef struct FileSystemTag
{
	FMFileInfo	*pFileInfo;
	UINT32		uiNumHandles;
	BOOLEAN		fDebug;
	BOOLEAN		fDBInitialized;

	CHAR8		*pcFileNames;
	UINT32		uiNumFilesInDirectory;
} FileSystem;

//**************************************************************************
//
//				Variables
//
//**************************************************************************


//The FileDatabaseHeader
DatabaseManagerHeaderStruct gFileDataBase;


//FileSystem gfs;

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

WIN32_FIND_DATA Win32FindInfo[20];

void W32toSGPFileFind( GETFILESTRUCT *pGFStruct, WIN32_FIND_DATA *pW32Struct );
//INT32	GetFilesInDirectory( HCONTAINER hStack, CHAR *, HANDLE hFile, WIN32_FIND_DATA *pFind );

HANDLE hFindInfoHandle[20] = {INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
								INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
								INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
								INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
								INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
								INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
								INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
								INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
								INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE,
								INVALID_HANDLE_VALUE, INVALID_HANDLE_VALUE };

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

struct FileList
{
	glob_t	Info;
	UINT32	uiIndex;
};

typedef struct FileList	FILELIST;

FILELIST FileInfo[20];

void TransferToSGPFileFind( GETFILESTRUCT *pGFStruct, FILELIST *pFindEntry );

// -------------------- End of Linux-specific stuff ------------------------
#endif	

BOOLEAN fFindInfoInUse[20] = {FALSE,FALSE,FALSE,FALSE,FALSE,
								FALSE,FALSE,FALSE,FALSE,FALSE,
								FALSE,FALSE,FALSE,FALSE,FALSE,
								FALSE,FALSE,FALSE,FALSE,FALSE };

// Snap: At program launch we build two directory catalogues:
// one for the default Data directory, the other for the custom Data directory.
TFileCat gDefaultDataCat;  // Init in InitializeStandardGamingPlatform (sgp.cpp)
TFileCat gCustomDataCat;   // Init in InitializeStandardGamingPlatform (sgp.cpp)

// Lesh: gzDataPath will represent path to dir, containing Data and Data-1.13 dirs
// gzHomePath will represent a path to game home dir, where ja2.ini, savedgames, logs
// and temps will be. Under windows it will not be used, under linux it is ~/ja2_113/
STRING512	gzDataPath;
STRING512	gzHomePath;


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
#ifdef JA2_LINUX
// ----------------------- Linux-specific stuff ----------------------------

	char DataPath[512];
	const char* Home;
	

	Home = getenv("HOME");
	if (Home == NULL)
	{
		const struct passwd* passwd = getpwuid(getuid());

		if (passwd == NULL || passwd->pw_dir == NULL)
		{
			fprintf(stderr, "Unable to locate user home directory\n");
			return FALSE;
		}

		Home = passwd->pw_dir;
	}

	printf("User home is %s\n", Home);
	strncpy( gzHomePath, Home, 512 );

	// assign home dir for game
	// data dir will be read from INI/CFG file, located in home dir
	strcat( gzHomePath, "/ja2_113");
	if (mkdir(gzHomePath, 0700) != 0 && errno != EEXIST)
	{
		fprintf(stderr, "Unable to create directory \"%s\"\n", gzHomePath);
		return FALSE;
	}

	printf("Using game home dir: %s\n", gzHomePath);

//	snprintf(DataPath, lengthof(DataPath), "%s/Data", LocalPath);
//	if (mkdir(DataPath, 0700) != 0 && errno != EEXIST)
//	{
//		fprintf(stderr, "Unable to create directory \"%s\"\n", DataPath);
//		return FALSE;
//	}

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
	BACKSLASH(strFilename);

	// First check to see if it's in a library (most files should be there)
	if ( gFileDataBase.fInitialized && CheckIfFileExistInLibrary( (STR) strFilename ) )
		return TRUE;

	// ... then check if it's in the custom Data directory
	if ( gCustomDataCat.FindFile(strFilename) )
		return TRUE;

	// ... then check if it's in the default Data directory
	if ( gDefaultDataCat.FindFile(strFilename) )
		return TRUE;

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	// ... lastly, try to locate it in the file system
	UINT32 attribs = GetFileAttributes(strFilename);
	if ( attribs != INVALID_FILE_ATTRIBUTES && !(attribs & FILE_ATTRIBUTE_DIRECTORY) )
		return TRUE;

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	// check, if it is a regular file
	struct stat file_stat;

	if ( stat( strFilename, &file_stat) == -1 )
		return FALSE;

	if ( S_ISREG(file_stat.st_mode) )
		return TRUE;

// -------------------- End of Linux-specific stuff ------------------------
#endif	
	return FALSE;
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
extern BOOLEAN	FileExistsNoDB( STR strFilename )
{
	BACKSLASH(strFilename);

	// First check if it's in the custom Data directory
	if ( gCustomDataCat.FindFile(strFilename) )
		return TRUE;

	// ... then check if it's in the default Data directory
	if ( gDefaultDataCat.FindFile(strFilename) )
		return TRUE;

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	// ... lastly, try to locate it in the file system
	UINT32 attribs = GetFileAttributes(strFilename);
	if ( attribs != INVALID_FILE_ATTRIBUTES && !(attribs & FILE_ATTRIBUTE_DIRECTORY) )
		return TRUE;
	

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	// check, if it is a regular file
	struct stat file_stat;

	if ( stat( strFilename, &file_stat) == -1 )
		return FALSE;

	if ( S_ISREG(file_stat.st_mode) )
		return TRUE;

// -------------------- End of Linux-specific stuff ------------------------
#endif

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
	BACKSLASH(strFilename);

	// Snap: delete the file from the default Data catalogue (if it is there)
	// Since the path can be either relative or absolute, try both methods
	gDefaultDataCat.RemoveFile(strFilename, true);
	gDefaultDataCat.RemoveFile(strFilename, false);

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	return( DeleteFile( (LPCSTR) strFilename ) );

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	return( unlink(strFilename) == 0 );

// -------------------- End of Linux-specific stuff ------------------------
#endif	
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
	BOOLEAN	fExists;
	HDBFILE	hDBFile;
	HWFILE hLibFile;
	UINT32	dwAccess;
	UINT32	dwFlagsAndAttributes;
	UINT32	dwCreationFlags;

	hFile = 0;
	hDBFile = 0;
	dwCreationFlags = 0;
	dwAccess = 0;
	
	BACKSLASH(strFilename);

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	if ( uiOptions & FILE_ACCESS_READ )
		dwAccess |= GENERIC_READ;
	if ( uiOptions & FILE_ACCESS_WRITE )
		dwAccess |= GENERIC_WRITE;

	dwFlagsAndAttributes = FILE_FLAG_RANDOM_ACCESS;
	if ( fDeleteOnClose )
		dwFlagsAndAttributes |= FILE_FLAG_DELETE_ON_CLOSE;

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	if ( (uiOptions & FILE_ACCESS_READ) && (uiOptions & FILE_ACCESS_WRITE) )
		dwAccess = O_RDWR;
	else if ( uiOptions & FILE_ACCESS_READ )
		dwAccess = O_RDONLY;
	else if ( uiOptions & FILE_ACCESS_WRITE )
		dwAccess = O_WRONLY;

// -------------------- End of Linux-specific stuff ------------------------
#endif	

	// Snap: This seems like an unnecessary check, but I don't feel like
	// rewriting the function to eliminate it...

	// check if the file exists - note that we use the function FileExistsNoDB
	// because it doesn't check the databases, and we don't want to do that here
	fExists = FileExistsNoDB( strFilename );

	// Snap: First see if the file is in the custom Data catalogue:
	std::string filePath;
	if ( gCustomDataCat.FindFile(strFilename) ) {
		filePath = gCustomDataCat.GetRootDir() + SLASH;
	}
	filePath += strFilename;
	// Bad cast! strFilename should have been const.  Oh well...
	strFilename = const_cast<STR>( filePath.c_str() );
	// Now strFilename points either to the original file name,
	// or to the full file path in the custom Data directory.
	// Except for this substitution, the rest of the function is unchanged.

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	//if the file is on the disk
	if ( fExists )
	{
		hRealFile = CreateFile( (LPCSTR) strFilename, dwAccess, 0, NULL, OPEN_ALWAYS,
										dwFlagsAndAttributes, NULL );

		if ( hRealFile == INVALID_HANDLE_VALUE )
		{
			return(0);
		}

		//create a file handle for the 'real file'
		hFile = CreateRealFileHandle( hRealFile );
	}
	// if the file did not exist, try to open it from the database
	else if ( gFileDataBase.fInitialized ) 
	{
		//if the file is to be opened for writing, return an error cause you cant write a file that is in the database library
		if( fDeleteOnClose )
		{
			return( 0 );
		}

		//if the file doesnt exist on the harddrive, but it is to be created, dont try to load it from the file database
		if( uiOptions & FILE_ACCESS_WRITE )
		{
			//if the files is to be written to
			if( ( uiOptions & FILE_CREATE_NEW ) || ( uiOptions & FILE_OPEN_ALWAYS ) || ( uiOptions & FILE_CREATE_ALWAYS ) || ( uiOptions & FILE_TRUNCATE_EXISTING ) )
			{
				hFile = 0;
			}
		}
		//else if the file is to be opened using FILE_OPEN_EXISTING, and the file doesnt exists, fail out of the function)
//		else if( uiOptions & FILE_OPEN_EXISTING )
//		{
			//fail out of the function
//			return( 0 );
//		}
		else
		{
			//If the file is in the library, get a handle to it.
			hLibFile = OpenFileFromLibrary( (STR) strFilename );

			//tried to open a file that wasnt in the database
			if( !hLibFile )
				return( 0 );
			else			
				return( hLibFile );		//return the file handle
		}
	}

	if ( !hFile )
	{
		if ( uiOptions & FILE_CREATE_NEW )
		{
			dwCreationFlags = CREATE_NEW;
		}
		else if ( uiOptions & FILE_CREATE_ALWAYS )
		{
			dwCreationFlags = CREATE_ALWAYS;
		}
		else if ( uiOptions & FILE_OPEN_EXISTING || uiOptions & FILE_ACCESS_READ )
		{
			dwCreationFlags = OPEN_EXISTING;
		}
		else if ( uiOptions & FILE_OPEN_ALWAYS )
		{
			dwCreationFlags = OPEN_ALWAYS;
		}
		else if ( uiOptions & FILE_TRUNCATE_EXISTING )
		{
			dwCreationFlags = TRUNCATE_EXISTING;
		}
		else
		{
			dwCreationFlags = OPEN_ALWAYS;
		}

		
		hRealFile = CreateFile( (LPCSTR) strFilename, dwAccess, 0, NULL, dwCreationFlags,
										dwFlagsAndAttributes, NULL );
		if ( hRealFile == INVALID_HANDLE_VALUE )
		{
				UINT32 uiLastError = GetLastError();
				char zString[1024];
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, 0, uiLastError, 0, zString, 1024, NULL);

			return(0);
		}

		hFile = CreateRealFileHandle( hRealFile );
	}

	if ( !hFile )
		return(0);

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	//if the file is on the disk
	if ( fExists )
	{
		hRealFile = open( strFilename, dwAccess );

		if ( hRealFile == -1 )
		{
			fprintf(stderr, "Error opening file %s: errno=%d\n", strFilename, errno);
			return(0);
		}

		//create a file handle for the 'real file'
		hFile = CreateRealFileHandle( hRealFile );
	}
	// if the file did not exist, try to open it from the database
	else if ( gFileDataBase.fInitialized ) 
	{
		//if the file is to be opened for writing, return an error cause you cant write a file that is in the database library
		if( fDeleteOnClose )
		{
			return( 0 );
		}

		//if the file doesnt exist on the harddrive, but it is to be created, dont try to load it from the file database
		if( uiOptions & FILE_ACCESS_WRITE )
		{
			//if the files is to be written to
			if( ( uiOptions & FILE_CREATE_NEW ) || ( uiOptions & FILE_OPEN_ALWAYS ) || ( uiOptions & FILE_CREATE_ALWAYS ) || ( uiOptions & FILE_TRUNCATE_EXISTING ) )
			{
				hFile = 0;
			}
		}
		else
		{
			//If the file is in the library, get a handle to it.
			hLibFile = OpenFileFromLibrary( (STR) strFilename );

			//tried to open a file that wasnt in the database
			if( !hLibFile )
				return( 0 );
			else			
				return( hLibFile );		//return the file handle
		}
	}

	if ( !hFile )
	{
		if ( uiOptions & FILE_CREATE_NEW )
		{
			dwAccess |= O_CREAT | O_EXCL;
		}
		else if ( uiOptions & FILE_CREATE_ALWAYS )
		{
			dwAccess |= O_CREAT;
		}
		else if ( uiOptions & FILE_TRUNCATE_EXISTING )
		{
			dwAccess |= O_CREAT | O_TRUNC;
		}

		hRealFile = open( strFilename, dwAccess );

		if ( hRealFile == -1 )
		{
			fprintf(stderr, "Error opening/creating file %s: errno=%d\n", strFilename, errno);
			return(0);
		}

		hFile = CreateRealFileHandle( hRealFile );
	}

	if ( !hFile )
		return(0);

// -------------------- End of Linux-specific stuff ------------------------
#endif	

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
#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

			CloseHandle( gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle );

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

			if ( close( gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle ) == -1 )
			{
				fprintf(stderr, "Error closing file %d: errno=%d\n",
					gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle, errno);
			}

// -------------------- End of Linux-specific stuff ------------------------
#endif	

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

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

			fRet = ReadFile( hRealFile, pDest, dwNumBytesToRead, &dwNumBytesRead, NULL );
			if ( dwNumBytesToRead != dwNumBytesRead )
			{
				UINT32 uiLastError = GetLastError();
				char zString[1024];
				FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, 0, uiLastError, 0, zString, 1024, NULL);

				return FALSE;
			}

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

			dwNumBytesRead = read( hRealFile, pDest, dwNumBytesToRead);
			if ( dwNumBytesToRead != dwNumBytesRead )
			{
				fprintf(stderr, "Error reading file %d: errno=%d\n",
					hRealFile, errno);

				return FALSE;
			}

// -------------------- End of Linux-specific stuff ------------------------
#endif	

			if ( puiBytesRead )
				*puiBytesRead = (UINT32)dwNumBytesRead;

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
	BOOLEAN	fRet;
	INT16 sLibraryID;
	UINT32 uiFileNum;


	GetLibraryAndFileIDFromLibraryFileHandle( hFile, &sLibraryID, &uiFileNum );

	//if its a real file, read the data from the file
	if( sLibraryID == REAL_FILE_LIBRARY_ID )
	{
		dwNumBytesToWrite = (UINT32)uiBytesToWrite;

		//get the real file handle to the file
		hRealFile = gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle;

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

		fRet = WriteFile( hRealFile, pDest, dwNumBytesToWrite, &dwNumBytesWritten, NULL );

		if (dwNumBytesToWrite != dwNumBytesWritten)
			fRet = FALSE;

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

		if ( write( hRealFile, pDest, dwNumBytesToWrite ) == -1 )
		{
				fprintf(stderr, "Error reading file %d: errno=%d\n",
					hRealFile, errno);
			fRet = FALSE;
		}

// -------------------- End of Linux-specific stuff ------------------------
#endif

		if ( puiBytesWritten )
			*puiBytesWritten = (UINT32)dwNumBytesWritten;

	
	}
	else
	{
		//we cannot write to a library file
		if ( puiBytesWritten )
			*puiBytesWritten = 0;
		return(FALSE);
	}

	return(fRet);
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

	INT16 sLibraryID;
	UINT32 uiFileNum;

	GetLibraryAndFileIDFromLibraryFileHandle( hFile, &sLibraryID, &uiFileNum );

	//if its a real file, read the data from the file
	if( sLibraryID == REAL_FILE_LIBRARY_ID )
	{
		va_start(argptr, strFormatted);
		vsprintf( (char *)strToSend, (char *)strFormatted, argptr );
		va_end(argptr);

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------
		
		fRetVal = FileWrite( hFile, strToSend, strlen(strToSend), NULL );

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

		if ( write( hFile, strToSend, strlen(strToSend) ) == -1 )
		{
				fprintf(stderr, "Error reading file %d: errno=%d\n",
					hFile, errno);
			fRetVal = FALSE;
		}

// -------------------- End of Linux-specific stuff ------------------------
#endif	
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
//	UINT32		lDistanceToMove;
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

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

		if ( uiHow == FILE_SEEK_FROM_START )
			dwMoveMethod = FILE_BEGIN;
		else if ( uiHow == FILE_SEEK_FROM_END )
		{
			dwMoveMethod = FILE_END;
			if( iDistance > 0 )
				iDistance = -(iDistance);
		}
		else
			dwMoveMethod = FILE_CURRENT;

//		lDistanceToMove = (UINT32)uiDistance;

		if ( SetFilePointer( hRealFile, iDistance, NULL, dwMoveMethod ) == 0xFFFFFFFF )
			return(FALSE);

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

		if ( uiHow == FILE_SEEK_FROM_START )
			dwMoveMethod = SEEK_SET;
		else if ( uiHow == FILE_SEEK_FROM_END )
		{
			dwMoveMethod = SEEK_END;
			if( iDistance > 0 )
				iDistance = -(iDistance);
		}
		else
			dwMoveMethod = SEEK_CUR;

		if ( lseek( hRealFile, iDistance, dwMoveMethod ) == -1 )
		{
			fprintf(stderr, "Error seeking file %d: errno=%d\n",
				hRealFile, errno);

			return(FALSE);
		}

// -------------------- End of Linux-specific stuff ------------------------
#endif	

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

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

		uiPositionInFile = SetFilePointer( hRealFile, 0, NULL, FILE_CURRENT);
		if( uiPositionInFile == 0xFFFFFFFF )
		{
			uiPositionInFile = 0;
		}
		return( uiPositionInFile );

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

		uiPositionInFile = lseek( hRealFile, 0, SEEK_CUR );
		if( uiPositionInFile == -1 )
		{
			fprintf(stderr, "Error getting position in file %d: errno=%d\n",
				hRealFile, errno);
			uiPositionInFile = 0;
		}
		return( uiPositionInFile );

// -------------------- End of Linux-specific stuff ------------------------
#endif	

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

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

		uiFileSize = GetFileSize( hRealHandle, NULL );

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

		struct stat file_stat;

		if ( fstat( hRealHandle, &file_stat) == -1 )
		{
			fprintf(stderr, "Error getting size of file %d: errno=%d\n",
				hRealHandle, errno);
			return 0;
		}

		uiFileSize = file_stat.st_size;

// -------------------- End of Linux-specific stuff ------------------------
#endif	
	}
	else
	{
		//if the library is open
		if( IsLibraryOpened( sLibraryID ) )
			uiFileSize = gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].pFileHeader->uiFileLength;
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

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	 return( SetCurrentDirectory( pcDirectory ) );

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	if ( chdir( pcDirectory ) == -1 )
	{
		fprintf(stderr, "Error setting dir %s: errno=%d\n",
			pcDirectory, errno);
		return FALSE;
	}

	return TRUE;

// -------------------- End of Linux-specific stuff ------------------------
#endif	
}


BOOLEAN GetFileManCurrentDirectory( STRING512 pcDirectory )
{
#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	if (GetCurrentDirectory( 512, pcDirectory ) == 0)
	{
		return( FALSE );
	}

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	if ( !getcwd( pcDirectory, 512 ) )
	{
		fprintf(stderr, "Error getting dir %s: errno=%d\n",
			pcDirectory, errno);
		return FALSE;
	}

// -------------------- End of Linux-specific stuff ------------------------
#endif	

	return( TRUE );
}


BOOLEAN DirectoryExists( STRING512 pcDirectory )
{
	UINT32	uiAttribs;
	UINT32		uiLastError;

	BACKSLASH(pcDirectory);

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	uiAttribs = GetFileAttributes( pcDirectory );

	if ( uiAttribs == 0xFFFFFFFF )
	{
		// an error, make sure it's the right error
		uiLastError =	GetLastError();

		if (uiLastError != ERROR_FILE_NOT_FOUND)
		{
   		FastDebugMsg(String("DirectoryExists: ERROR - GetFileAttributes failed, error #%d on file %s", uiLastError, pcDirectory));
		}
	}
	else
	{
		// something's there, make sure it's a directory
		if ( uiAttribs & FILE_ATTRIBUTE_DIRECTORY )
		{
			return TRUE;
		}
	}

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	// check, if it is a regular file
	struct stat file_stat;

	if ( stat( pcDirectory, &file_stat) == -1 )
		return FALSE;

	if ( S_ISDIR(file_stat.st_mode) )
		return TRUE;


// -------------------- End of Linux-specific stuff ------------------------
#endif	

	// this could also mean that the name given is that of a file, or that an error occurred
	return FALSE;
}


BOOLEAN MakeFileManDirectory( STRING512 pcDirectory )
{
	BACKSLASH(pcDirectory);

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	return CreateDirectory( pcDirectory, NULL );

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	if ( mkdir(pcDirectory, 0755) == -1 )
	{
		fprintf(stderr, "Error creating dir %s: errno=%d\n",
			pcDirectory, errno);
		return FALSE;
	}
	return TRUE;

// -------------------- End of Linux-specific stuff ------------------------
#endif	
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
	gDefaultDataCat.RemoveDir(pcDirectory, true);
	gDefaultDataCat.RemoveDir(pcDirectory, false);

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
	strncpy( pcDirectory, gzHomePath, 512);
// -------------------- End of Linux-specific stuff ------------------------
#endif	

	return( TRUE );
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

	BACKSLASH(pSpec);

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

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	hFindInfoHandle[iWhich] = FindFirstFile( pSpec, &Win32FindInfo[iWhich] );
	
	if ( hFindInfoHandle[iWhich] == INVALID_HANDLE_VALUE )
		return(FALSE);

	W32toSGPFileFind( pGFStruct, &Win32FindInfo[iWhich] );

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	if ( glob( pSpec, 0, NULL, &FileInfo[iWhich].Info ) )
	{
		fprintf(stderr, "Error while trying to GetFileFirst() using mask: %s, errno=%d\n", pSpec, errno);
		globfree( &FileInfo[iWhich].Info );
		return FALSE;
	}

	FileInfo[iWhich].uiIndex = 0;
	TransferToSGPFileFind( pGFStruct, &FileInfo[iWhich] );

// -------------------- End of Linux-specific stuff ------------------------
#endif	

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

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------
	
	if ( FindNextFile(hFindInfoHandle[pGFStruct->iFindHandle], &Win32FindInfo[pGFStruct->iFindHandle]) )
	{
		W32toSGPFileFind( pGFStruct, &Win32FindInfo[pGFStruct->iFindHandle] );
		return(TRUE);
	}

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	if ( FileInfo[ pGFStruct->iFindHandle ].uiIndex < FileInfo[ pGFStruct->iFindHandle ].Info.gl_pathc )
	{
		TransferToSGPFileFind( pGFStruct, &FileInfo[ pGFStruct->iFindHandle ] );
		FileInfo[ pGFStruct->iFindHandle ].uiIndex++;
		return TRUE;
	}

// -------------------- End of Linux-specific stuff ------------------------
#endif	

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

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	FindClose( hFindInfoHandle[pGFStruct->iFindHandle] );
	hFindInfoHandle[pGFStruct->iFindHandle] = INVALID_HANDLE_VALUE;

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	globfree( &FileInfo[ pGFStruct->iFindHandle ].Info );

// -------------------- End of Linux-specific stuff ------------------------
#endif	

	fFindInfoInUse[pGFStruct->iFindHandle] = FALSE;

	return;
}

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

//**************************************************************************
//
// W32toSGPFileFind()
//		Windows version! Convert results of file searching to SGP format
// Parameter List :
//		pGFStruct - SGP find file info
//		pW32Struct - info about one found file
//
//**************************************************************************
void W32toSGPFileFind( GETFILESTRUCT *pGFStruct, WIN32_FIND_DATA *pW32Struct )
{
	UINT32 uiAttribMask;

	// Copy the filename
	strcpy(pGFStruct->zFileName, pW32Struct->cFileName);

	// Get file size
	if ( pW32Struct->nFileSizeHigh != 0 )
		pGFStruct->uiFileSize = 0xffffffff;
	else
		pGFStruct->uiFileSize = pW32Struct->nFileSizeLow;

	// Copy the file attributes
	pGFStruct->uiFileAttribs = 0;

	for( uiAttribMask = 0x80000000; uiAttribMask > 0; uiAttribMask >>= 1)
	{
		switch( pW32Struct->dwFileAttributes & uiAttribMask )
		{
			case FILE_ATTRIBUTE_ARCHIVE:
				pGFStruct->uiFileAttribs |= FILE_IS_ARCHIVE;
				break;

			case FILE_ATTRIBUTE_DIRECTORY:
				pGFStruct->uiFileAttribs |= FILE_IS_DIRECTORY;
				break;
			
			case FILE_ATTRIBUTE_HIDDEN:
				pGFStruct->uiFileAttribs |= FILE_IS_HIDDEN;
				break;
			
			case FILE_ATTRIBUTE_NORMAL:	
				pGFStruct->uiFileAttribs |= FILE_IS_NORMAL;
				break;
			
			case FILE_ATTRIBUTE_READONLY:
				pGFStruct->uiFileAttribs |= FILE_IS_READONLY;
				break;
			
			case FILE_ATTRIBUTE_SYSTEM:
				pGFStruct->uiFileAttribs |= FILE_IS_SYSTEM;
				break;
			
			case FILE_ATTRIBUTE_TEMPORARY:
				pGFStruct->uiFileAttribs |= FILE_IS_TEMPORARY;
				break;

			case FILE_ATTRIBUTE_COMPRESSED:
				pGFStruct->uiFileAttribs |= FILE_IS_COMPRESSED;
				break;

			case FILE_ATTRIBUTE_OFFLINE:
				pGFStruct->uiFileAttribs |= FILE_IS_OFFLINE;
				break;
		}
	}
}

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

//**************************************************************************
//
// TransferToSGPFileFind()
//		Linux version! Convert results of file searching to SGP format
// Parameter List :
//		pGFStruct - SGP find file info
//		pFindEntry - info about one found file
//
//**************************************************************************
void TransferToSGPFileFind( GETFILESTRUCT *pGFStruct, FILELIST *pFindEntry )
{
	// Copy the filename
	strncpy(pGFStruct->zFileName, pFindEntry->Info.gl_pathv[ pFindEntry->uiIndex ], 260);

//	pGFStruct->uiFileAttribs |= FILE_IS_NORMAL;
}

// -------------------- End of Linux-specific stuff ------------------------
#endif	


//**************************************************************************
//
// FileSetAttributes()
//		Set file attributes. Actually, not used.
// Parameter List :
//		strFilename - file name
//		uiNewAttribs - new attributes
// Return Value :
//		UINT32 - file attributes as a combination of bits
//
//**************************************************************************
BOOLEAN FileSetAttributes( STR strFilename, UINT32 uiNewAttribs )
{
#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	UINT32	uiFileAttrib = 0;

	if( uiNewAttribs & FILE_ATTRIBUTES_ARCHIVE )
		uiFileAttrib |= FILE_ATTRIBUTE_ARCHIVE;

	if( uiNewAttribs & FILE_ATTRIBUTES_HIDDEN )
		uiFileAttrib |= FILE_ATTRIBUTE_HIDDEN;

	if( uiNewAttribs & FILE_ATTRIBUTES_NORMAL )
		uiFileAttrib |= FILE_ATTRIBUTE_NORMAL;

	if( uiNewAttribs & FILE_ATTRIBUTES_OFFLINE )
		uiFileAttrib |= FILE_ATTRIBUTE_OFFLINE;

	if( uiNewAttribs & FILE_ATTRIBUTES_READONLY )
		uiFileAttrib |= FILE_ATTRIBUTE_READONLY;

	if( uiNewAttribs & FILE_ATTRIBUTES_SYSTEM	)
		uiFileAttrib |= FILE_ATTRIBUTE_SYSTEM;

	if( uiNewAttribs & FILE_ATTRIBUTES_TEMPORARY )
		uiFileAttrib |= FILE_ATTRIBUTE_TEMPORARY;

	return SetFileAttributes( strFilename, uiFileAttrib );

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	// Lesh: not implemented, because function isn't in use

// -------------------- End of Linux-specific stuff ------------------------
#endif	
}

//**************************************************************************
//
// FileGetAttributes()
//		Get file attributes.
// Parameter List :
//		strFilename - file name
// Return Value :
//		UINT32 - file attributes as a combination of bits
//
//**************************************************************************
UINT32 FileGetAttributes( STR strFilename )
{
	UINT32	uiFileAttrib = 0;

	BACKSLASH(strFilename);

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	UINT32	uiAttribs = 0;

	uiAttribs = GetFileAttributes( strFilename );

	if( uiAttribs == 0xFFFFFFFF )
		return( uiAttribs );

	if( uiAttribs & FILE_ATTRIBUTE_ARCHIVE )
		uiFileAttrib |= FILE_ATTRIBUTES_ARCHIVE;

	if( uiAttribs & FILE_ATTRIBUTE_HIDDEN )
		uiFileAttrib |= FILE_ATTRIBUTES_HIDDEN;

	if( uiAttribs & FILE_ATTRIBUTE_NORMAL )
		uiFileAttrib |= FILE_ATTRIBUTES_NORMAL;

	if( uiAttribs & FILE_ATTRIBUTE_OFFLINE )
		uiFileAttrib |= FILE_ATTRIBUTES_OFFLINE;

	if( uiAttribs & FILE_ATTRIBUTE_READONLY )
		uiFileAttrib |= FILE_ATTRIBUTES_READONLY;

	if( uiAttribs & FILE_ATTRIBUTE_SYSTEM	)
		uiFileAttrib |= FILE_ATTRIBUTES_SYSTEM;

	if( uiAttribs & FILE_ATTRIBUTE_TEMPORARY )
		uiFileAttrib |= FILE_ATTRIBUTES_TEMPORARY;

	if( uiAttribs & FILE_ATTRIBUTE_DIRECTORY )
		uiFileAttrib |= FILE_ATTRIBUTES_DIRECTORY;

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	struct stat file_stat;

	if ( stat( strFilename, &file_stat) == -1 )
		return 0xFFFFFFFF;

	if ( S_ISDIR(file_stat.st_mode) )
		uiFileAttrib |= FILE_ATTRIBUTES_DIRECTORY;

// -------------------- End of Linux-specific stuff ------------------------
#endif	

	return( uiFileAttrib );
}


//**************************************************************************
//
// FileClearAttributes()
//		Clear file attributes.
// Parameter List :
//		strFilename - file name
// Return Value :
//		BOOLEAN - TRUE if end of file, FALSE if not
//
//**************************************************************************
BOOLEAN FileClearAttributes( STR strFilename )
{
	BACKSLASH(strFilename);

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	return SetFileAttributes( (LPCSTR) strFilename, FILE_ATTRIBUTE_NORMAL );

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	return TRUE;

// -------------------- End of Linux-specific stuff ------------------------
#endif	
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
		//Get the handle to the real file
		hRealFile = gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle;

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

		UINT32	uiOldFilePtrLoc=0;
		UINT32	uiEndOfFilePtrLoc=0;
		UINT32	temp=0;

		//Get the current position of the file pointer
		uiOldFilePtrLoc = SetFilePointer( hRealFile, 0, NULL, FILE_CURRENT );

		//Get the end of file ptr location
		uiEndOfFilePtrLoc = SetFilePointer( hRealFile, 0, NULL, FILE_END );

		//reset back to the original location
		temp = SetFilePointer( hRealFile, -( (INT32)( uiEndOfFilePtrLoc - uiOldFilePtrLoc ) ), NULL, FILE_END );

		//if the 2 pointers are the same, we are at the end of a file
		if( uiEndOfFilePtrLoc <= uiOldFilePtrLoc )
		{
			return( 1 );
		}

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

		UINT32 uiFileSize = FileGetSize( hRealFile );
		UINT32 uiFilePtr  = FileGetPos( hRealFile );

		return ( uiFilePtr >= uiFileSize );

// -------------------- End of Linux-specific stuff ------------------------
#endif	

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
					UINT32	uiLength;					//uiOffsetInLibrary
//					HANDLE	hLibraryFile;
//					UINT32	uiNumBytesRead;
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
	memset( pCreationTime, 0, sizeof( SGP_FILETIME ) );
	memset( pLastAccessedTime, 0, sizeof( SGP_FILETIME ) );
	memset( pLastWriteTime, 0, sizeof( SGP_FILETIME ) );


	GetLibraryAndFileIDFromLibraryFileHandle( hFile, &sLibraryID, &uiFileNum );

	//if its a real file, read the data from the file
	if( sLibraryID == REAL_FILE_LIBRARY_ID )
	{
		//get the real file handle to the file
		hRealFile = gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle;

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

		FILETIME	sCreationUtcFileTime;
		FILETIME	sLastAccessedUtcFileTime;
		FILETIME	sLastWriteUtcFileTime;

		//Gets the UTC file time for the 'real' file				
		GetFileTime( hRealFile, &sCreationUtcFileTime, &sLastAccessedUtcFileTime, &sLastWriteUtcFileTime );

		//converts the creation UTC file time to the current time used for the file
		FileTimeToLocalFileTime( &sCreationUtcFileTime, pCreationTime );
		
		//converts the accessed UTC file time to the current time used for the file
		FileTimeToLocalFileTime( &sLastAccessedUtcFileTime, pLastAccessedTime );

		//converts the write UTC file time to the current time used for the file
		FileTimeToLocalFileTime( &sLastWriteUtcFileTime, pLastWriteTime );

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

		struct stat file_stat;

		if ( fstat( hFile, &file_stat) == -1 )
		{
			fprintf(stderr, "Error getting time of file %d: errno=%d\n",
				hFile, errno);
			return 0;
		}

		*pCreationTime     = file_stat.st_ctime;
		*pLastAccessedTime = file_stat.st_atime;
		*pLastWriteTime    = file_stat.st_mtime;

// -------------------- End of Linux-specific stuff ------------------------
#endif	
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
#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	return( CompareFileTime( pFirstFileTime, pSecondFileTime ) );

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	return( *pFirstFileTime - *pSecondFileTime );

// -------------------- End of Linux-specific stuff ------------------------
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
UINT32 FileSize(STR strFilename)
{
	UINT32 uiSize;

	BACKSLASH(strFilename);

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	HWFILE hFile;

	if((hFile=FileOpen(strFilename, FILE_OPEN_EXISTING | FILE_ACCESS_READ, FALSE))==0)
		return(0);
		
	uiSize=FileGetSize(hFile);
	FileClose(hFile);				

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	struct stat file_stat;

	if ( stat( strFilename, &file_stat) == -1 )
	{
		fprintf(stderr, "Error getting size of file %s: errno=%d\n",
			strFilename, errno);
		return 0;
	}
	uiSize = file_stat.st_size;

// -------------------- End of Linux-specific stuff ------------------------
#endif	

	return(uiSize);
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

#if 0

UINT32 GetFreeSpaceOnHardDriveWhereGameIsRunningFrom( )
{
  STRING512		zExecDir;
  STRING512		zDrive;
	STRING512		zDir;
	STRING512		zFileName;
	STRING512		zExt;

	UINT32 uiFreeSpace = 0;

	GetExecutableDirectory( zExecDir );

	//get the drive letter from the exec dir
	_splitpath( zExecDir, zDrive, zDir, zFileName, zExt);

	sprintf( zDrive, "%s\\", zDrive );
	
	uiFreeSpace = GetFreeSpaceOnHardDrive( zDrive );

	return( uiFreeSpace );
}




UINT32 GetFreeSpaceOnHardDrive( STR pzDriveLetter )
{
	UINT32			uiBytesFree=0;

	UINT32			uiSectorsPerCluster=0;
	UINT32			uiBytesPerSector=0;
	UINT32			uiNumberOfFreeClusters=0;
	UINT32			uiTotalNumberOfClusters=0;

	if( !GetDiskFreeSpace( pzDriveLetter, (LPUINT32) &uiSectorsPerCluster, (LPUINT32) &uiBytesPerSector, 
			(LPUINT32) &uiNumberOfFreeClusters, (LPUINT32) &uiTotalNumberOfClusters ) )
	{
		UINT32 uiLastError = GetLastError();
		char zString[1024];
		FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM, 0, uiLastError, 0, zString, 1024, NULL);

		return( TRUE );
	}

	uiBytesFree = uiBytesPerSector * uiNumberOfFreeClusters * uiSectorsPerCluster;

	return( uiBytesFree );
}

#endif
