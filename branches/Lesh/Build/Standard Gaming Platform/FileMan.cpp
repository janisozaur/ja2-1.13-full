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
	#include "JA2 SGP ALL.H"
#else
	#include "Types.h"
	#include "Platform.h"
	#include "FileMan.h"
	#include "MemMan.h"
	#include "DbMan.h"
	#include "DEBUG.H"
	#include "Container.h"
	#include "LibraryDataBase.h"
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
INT32	GetFilesInDirectory( HCONTAINER hStack, CHAR *, HANDLE hFile, WIN32_FIND_DATA *pFind );

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

// -------------------- End of Linux-specific stuff ------------------------
#endif	

BOOLEAN fFindInfoInUse[20] = {FALSE,FALSE,FALSE,FALSE,FALSE,
								FALSE,FALSE,FALSE,FALSE,FALSE,
								FALSE,FALSE,FALSE,FALSE,FALSE,
								FALSE,FALSE,FALSE,FALSE,FALSE };
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
HWFILE	CreateFileHandle( HANDLE hRealFile, BOOLEAN fDatabaseFile );
void	DestroyFileHandle( HWFILE hFile );
void	BuildFileDirectory( void );

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
//	const char* Home;
	

//	Home = getenv("HOME");
//	if (Home == NULL)
//	{
//		const struct passwd* passwd = getpwuid(getuid());
//
//		if (passwd == NULL || passwd->pw_dir == NULL)
//		{
//			fprintf(stderr, "Unable to locate home directory\n");
//			return FALSE;
//		}
//
//		Home = passwd->pw_dir;
//		strncpy( gzHomePath, Home, 512 );
//	}

	// assign home dir for game
	// data dir will be read from INI/CFG file, located in home dir
	strncpy( gzHomePath, "~/ja2_113/", 512 );
	if (mkdir(gzHomePath, 0700) != 0 && errno != EEXIST)
	{
		fprintf(stderr, "Unable to create directory \"%s\"\n", gzHomePath);
		return FALSE;
	}

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

#ifdef JA2_WIN
#	define BACKSLASH(x)
#elif defined(JA2_LINUX)
#	define BACKSLASH(x)		PathBackslash(x)
#endif

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
	#include "Timer Control.h"
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

				fRet = FALSE;
			}

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

			dwNumBytesRead = read( hRealFile, pDest, dwNumBytesToRead);
			if ( dwNumBytesToRead != dwNumBytesRead )
			{
				fprintf(stderr, "Error reading file %d: errno=%d\n",
					hRealFile, errno);

				fRet = FALSE;
			}

// -------------------- End of Linux-specific stuff ------------------------
#endif	

			if ( puiBytesRead )
				*puiBytesRead = (UINT32)dwNumBytesRead;

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
// FileLoad
//
//		To open, read, and close a file.
//
// Parameter List :
//
//
// Return Value :
//
//		BOOLEAN	-> TRUE if successful
//					-> FALSE if not
//
// Modification history :
//
//		24sep96:HJH		-> creation
//		08Dec97:ARM		-> return FALSE if bytes to read != bytes read (CHECKF is inappropriate?)
//
//**************************************************************************

#if 0
BOOLEAN FileLoad( STR strFilename, PTR pDest, UINT32 uiBytesToRead, UINT32 *puiBytesRead )
{
	HWFILE	hFile;
	UINT32	uiNumBytesRead;
	BOOLEAN	fRet;

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	hFile = FileOpen( strFilename, FILE_ACCESS_READ, FALSE );
	if ( hFile )
	{
		fRet = FileRead( hFile, pDest, uiBytesToRead, &uiNumBytesRead );
		FileClose( hFile );

		if (uiBytesToRead != uiNumBytesRead)
			fRet = FALSE;

		if ( puiBytesRead )
			*puiBytesRead = uiNumBytesRead;

		CHECKF( uiNumBytesRead == uiBytesToRead );
	}
	else
		fRet = FALSE;

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

// -------------------- End of Linux-specific stuff ------------------------
#endif	

	return(fRet);
}
#endif

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

//**************************************************************************
//
// CreateFileHandle
//
//		
//
// Parameter List :
// Return Value :
// Modification history :
//
//		24sep96:HJH		-> creation
//
//**************************************************************************
/*

	not needed anymore

HWFILE CreateFileHandle( HANDLE hRealFile, BOOLEAN fDatabaseFile )
{
	UINT32		i, uiOldNumHandles;
	FMFileInfo		*pNewFileInfo;

	Assert( !fDatabaseFile || (fDatabaseFile && gfs.fDBInitialized) );

	// don't use 1st position - it'll confuse the users
	for ( i=1 ; i<gfs.uiNumHandles ; i++ )
	{
		if ( gfs.pFileInfo[i].hFileHandle == 0 && gfs.pFileInfo[i].hDBFile == 0 )
		{
			if ( fDatabaseFile )
				gfs.pFileInfo[i].hDBFile = (HDBFILE)hRealFile;
			else
				gfs.pFileInfo[i].hFileHandle = hRealFile;
			return( i );
		}
	}

	uiOldNumHandles = gfs.uiNumHandles;

	pNewFileInfo = (FMFileInfo *)MemRealloc( gfs.pFileInfo, gfs.uiNumHandles + NUM_FILES_TO_ADD_AT_A_TIME );
	if ( !pNewFileInfo )
	{
		// TBD: error error error
		return(0);
	}
	gfs.pFileInfo = (FMFileInfo *)pNewFileInfo;
	gfs.uiNumHandles = gfs.uiNumHandles + NUM_FILES_TO_ADD_AT_A_TIME;

	for ( i=uiOldNumHandles ; i<gfs.uiNumHandles ; i++ )
	{
		gfs.pFileInfo[i].hFileHandle = 0;
		gfs.pFileInfo[i].hDBFile = 0;
	}

	if ( fDatabaseFile )
		gfs.pFileInfo[uiOldNumHandles].hDBFile = (HDBFILE)hRealFile;
	else
		gfs.pFileInfo[uiOldNumHandles].hFileHandle = hRealFile;

	return(uiOldNumHandles);
}
*/

//**************************************************************************
//
// DestroyFileHandle
//
//		
//
// Parameter List :
// Return Value :
// Modification history :
//
//		24sep96:HJH		-> creation
//
//**************************************************************************
/*
void DestroyFileHandle( HWFILE hFile )
{
	if ( hFile < gfs.uiNumHandles && hFile )
	{
		gfs.pFileInfo[hFile].hFileHandle = 0;
		gfs.pFileInfo[hFile].hDBFile = 0;
	}
}
*/



//**************************************************************************
//
// BuildFileDirectory
//
//		
//
// Parameter List :
// Return Value :
// Modification history :
//
//		??nov96:HJH		-> creation
//
//**************************************************************************

void BuildFileDirectory( void )
{

	return;	// temporary until container stuff is fixed
/*
	INT32					i, iNumFiles = 0;
	HANDLE				hFile, hFileIn;
	WIN32_FIND_DATA	find, inFind;
	BOOLEAN				fMore = TRUE;
	CHAR					cName[FILENAME_LENGTH], cDir[FILENAME_LENGTH], cSubDir[FILENAME_LENGTH];
	HCONTAINER			hStack;



	//
	//	First, push all the file names in the directory (and subdirectories)
	//	onto the stack.
	//

	GetProfileChar( "Startup", "InstPath", "", cDir );

	if ( strlen( cDir ) == 0 )
		return;

	hStack = CreateStack( 100, FILENAME_LENGTH );
	if (hStack == NULL)
	{
		FastDebugMsg(String("BuildFileDirectory: CreateStack Failed for the filename stack"));
		return;
	}

	find.dwFileAttributes = FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY;

	strcpy( &(cDir[strlen(cDir)]), "\\*.*\0" );
	hFile = FindFirstFile( cDir, &find );
	while ( fMore )
	{
		if ( find.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if ( strcmp( find.cFileName, "." ) != 0 && strcmp( find.cFileName, ".." ) != 0 )
			{
				// a valid directory
				inFind.dwFileAttributes = FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY;
				strcpy( cSubDir, cDir );
				strcpy( &(cSubDir[strlen(cDir)-3]), find.cFileName );
				strcpy( &(cSubDir[strlen(cSubDir)]), "\\*.*\0" );
				hFileIn = FindFirstFile( cSubDir, &inFind );
				iNumFiles += GetFilesInDirectory( hStack, cSubDir, hFileIn, &inFind );
				FindClose( hFileIn );
			}
		}
		else
		{
			iNumFiles++;
			strcpy( cName, cDir );
			strcpy( &(cName[strlen(cName)-3]), find.cFileName );
			CharLower( cName );
			hStack = Push( hStack, cName );
		}
		find.dwFileAttributes = FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY;
		fMore = FindNextFile( hFile, &find );
	}
	FindClose( hFile );

	//
	//	Okay, we have all the files in the stack, now put them in place.
	//
	gfs.uiNumFilesInDirectory = iNumFiles;

	gfs.pcFileNames = (CHAR *)MemAlloc( iNumFiles * FILENAME_LENGTH );

	if ( gfs.pcFileNames )
	{
		for ( i=0 ; i<iNumFiles ; i++ )
		{
			Pop( hStack, (void *)(&gfs.pcFileNames[i*FILENAME_LENGTH]) );
		}
	}

	//
	//	Clean up.
	//

	DeleteStack( hStack );
*/
}

//**************************************************************************
//
// GetFilesInDirectory
//
//		Gets the files in a directory and the subdirectories.
//
// Parameter List :
// Return Value :
// Modification history :
//
//		??nov96:HJH		-> creation
//
//**************************************************************************

#if 0
INT32 GetFilesInDirectory( HCONTAINER hStack, CHAR *pcDir, HANDLE hFile, WIN32_FIND_DATA *pFind )
{
	INT32					iNumFiles;
	WIN32_FIND_DATA	inFind;
	BOOLEAN				fMore;
	CHAR					cName[FILENAME_LENGTH], cDir[FILENAME_LENGTH];
	HANDLE				hFileIn;

	fMore = TRUE;
	iNumFiles = 0;

	while ( fMore )
	{
		if ( pFind->dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
		{
			if ( strcmp( pFind->cFileName, "." ) != 0 && strcmp( pFind->cFileName, ".." ) != 0 )
			{
				// a valid directory - recurse and find the files in that directory

				inFind.dwFileAttributes = FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY;
				strcpy( cDir, pcDir );
				strcpy( &(cDir[strlen(cDir)-3]), pFind->cFileName );
				strcpy( &(cDir[strlen(cDir)]), "\\*.*\0" );
				hFileIn = FindFirstFile( cDir, &inFind );
				iNumFiles += GetFilesInDirectory( hStack, cDir, hFileIn, &inFind );
				FindClose( hFileIn );
			}
		}
		else
		{
			iNumFiles++;
			strcpy( cName, pcDir );
			strcpy( &(cName[strlen(cName)-3]), pFind->cFileName );
			CharLower( cName );
			hStack = Push( hStack, cName );
		}
		pFind->dwFileAttributes = FILE_ATTRIBUTE_NORMAL | FILE_ATTRIBUTE_DIRECTORY;
		fMore = FindNextFile( hFile, pFind );
	}

	return(iNumFiles);
}
#endif

BOOLEAN SetFileManCurrentDirectory( STR pcDirectory )
{
#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	 return( SetCurrentDirectory( pcDirectory ) );

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

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

// -------------------- End of Linux-specific stuff ------------------------
#endif	
	return( TRUE );
}


BOOLEAN DirectoryExists( STRING512 pcDirectory )
{
	UINT32	uiAttribs;
	UINT32		uiLastError;

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

// -------------------- End of Linux-specific stuff ------------------------
#endif	

	// this could also mean that the name given is that of a file, or that an error occurred
	return FALSE;
}


BOOLEAN MakeFileManDirectory( STRING512 pcDirectory )
{
#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	return CreateDirectory( pcDirectory, NULL );

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

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

// -------------------- End of Linux-specific stuff ------------------------
#endif	

	return( TRUE );
}

#if 0
BOOLEAN GetFileFirst( CHAR8 * pSpec, GETFILESTRUCT *pGFStruct )
{
#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	INT32 x,iWhich=0;
	BOOLEAN fFound;

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

	hFindInfoHandle[iWhich] = FindFirstFile( pSpec, &Win32FindInfo[iWhich] );
	
	if ( hFindInfoHandle[iWhich] == INVALID_HANDLE_VALUE )
		return(FALSE);
	fFindInfoInUse[iWhich] = TRUE;

	W32toSGPFileFind( pGFStruct, &Win32FindInfo[iWhich] );

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

// -------------------- End of Linux-specific stuff ------------------------
#endif	

	return(TRUE);
}
#endif

#if 0
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

// -------------------- End of Linux-specific stuff ------------------------
#endif	

	return(FALSE);
}
#endif

#if 0
void GetFileClose( GETFILESTRUCT *pGFStruct )
{
	if ( pGFStruct == NULL )
		return;

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	FindClose( hFindInfoHandle[pGFStruct->iFindHandle] );
	hFindInfoHandle[pGFStruct->iFindHandle] = INVALID_HANDLE_VALUE;
	fFindInfoInUse[pGFStruct->iFindHandle] = FALSE;

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

// -------------------- End of Linux-specific stuff ------------------------
#endif	

	return;
}
#endif

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

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

// -------------------- End of Linux-specific stuff ------------------------
#endif	


#if 0
BOOLEAN FileCopy(STR strSrcFile, STR strDstFile, BOOLEAN fFailIfExists)
{
#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	return(CopyFile(strSrcFile, strDstFile, fFailIfExists));

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

// -------------------- End of Linux-specific stuff ------------------------
#endif	

// Not needed, use Windows CopyFile
/*
	HWFILE hFile;
	UINT32 uiSize;
	CHAR *pBuffer;
	UINT32 uiBytesRead, uiBytesWritten;


	// open source file
  hFile = FileOpen(strSrcFile, FILE_ACCESS_READ, FALSE);
  if (hFile == 0)
  {
   	FastDebugMsg(String("FileCopy: FileOpen failed on Src file %s", strSrcFile));
    return(FALSE);
  }

	// get its size
	uiSize = FileGetSize(hFile);
	if (uiSize == 0)
	{
   	FastDebugMsg(String("FileCopy: size is 0, Src file %s", strSrcFile));
    FileClose(hFile);
    return(FALSE);
	}

	// allocate a buffer big enough to hold the entire file
	pBuffer = MemAlloc(uiSize);
	if (pBuffer == NULL)
	{
		FastDebugMsg(String("FileCopy: ERROR - MemAlloc pBuffer failed, size %d", uiSize));
    FileClose(hFile);
		return(FALSE);
	}

	// read the file into memory
  if (!FileRead(hFile, pBuffer, uiSize, &uiBytesRead))
  {
   	FastDebugMsg(String("FileCopy: FileRead failed, file %s", strSrcFile));
    FileClose(hFile);
    return(FALSE);
  }

	// close source file
  FileClose(hFile);


	// open destination file
  hFile = FileOpen(strDstFile, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE);
  if (hFile == 0)
  {
   	FastDebugMsg(String("FileCopy: FileOpen failed on Dst file %s", strDstFile));
    return(FALSE);
  }

	// write buffer to the destination file
  if (!FileWrite(hFile, pBuffer, uiSize, &uiBytesWritten))
  {
   	FastDebugMsg(String("FileCopy: FileWrite failed, file %s", strDstFile));
    FileClose(hFile);
    return(FALSE);
  }

	// close destination file
  FileClose(hFile);


  MemFree(pBuffer);
  pBuffer = NULL;
	return(TRUE);
*/
}
#endif

#if 0
BOOLEAN FileMove(STR strOldName, STR strNewName)
{
#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	// rename
	return(MoveFile(strOldName, strNewName));

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

// -------------------- End of Linux-specific stuff ------------------------
#endif	
}
#endif

#if 0
//Additions by Kris Morness
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

// -------------------- End of Linux-specific stuff ------------------------
#endif	
}
#endif

#if 0
UINT32 FileGetAttributes( STR strFilename )
{
#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	UINT32	uiAttribs = 0;
	UINT32	uiFileAttrib = 0;

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

// -------------------- End of Linux-specific stuff ------------------------
#endif	

	return( uiFileAttrib );
}
#endif

#if 0
BOOLEAN FileClearAttributes( STR strFilename )
{
#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	return SetFileAttributes( (LPCSTR) strFilename, FILE_ATTRIBUTE_NORMAL );

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

// -------------------- End of Linux-specific stuff ------------------------
#endif	
}
#endif

#if 0
//returns true if at end of file, else false
BOOLEAN	FileCheckEndOfFile( HWFILE hFile )
{
	INT16 sLibraryID;
	UINT32 uiFileNum;
	HANDLE	hRealFile;
//	UINT8		Data;
	UINT32	uiNumberOfBytesRead=0;
	UINT32	uiOldFilePtrLoc=0;
	UINT32	uiEndOfFilePtrLoc=0;
	UINT32	temp=0;

	GetLibraryAndFileIDFromLibraryFileHandle( hFile, &sLibraryID, &uiFileNum );

	//if its a real file, read the data from the file
	if( sLibraryID == REAL_FILE_LIBRARY_ID )
	{
		//Get the handle to the real file
		hRealFile = gFileDataBase.RealFiles.pRealFilesOpen[ uiFileNum ].hRealFileHandle;

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

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
#endif

#if 0
BOOLEAN GetFileManFileTime( HWFILE hFile, SGP_FILETIME	*pCreationTime, SGP_FILETIME *pLastAccessedTime, SGP_FILETIME *pLastWriteTime )
{
	HANDLE	hRealFile;
	INT16 sLibraryID;
	UINT32 uiFileNum;

	FILETIME	sCreationUtcFileTime;
	FILETIME	sLastAccessedUtcFileTime;
	FILETIME	sLastWriteUtcFileTime;

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

		//Gets the UTC file time for the 'real' file				
		GetFileTime( hRealFile, &sCreationUtcFileTime, &sLastAccessedUtcFileTime, &sLastWriteUtcFileTime );

		//converts the creation UTC file time to the current time used for the file
		FileTimeToLocalFileTime( &sCreationUtcFileTime, pCreationTime );
		
		//converts the accessed UTC file time to the current time used for the file
		FileTimeToLocalFileTime( &sLastAccessedUtcFileTime, pLastAccessedTime );

		//converts the write UTC file time to the current time used for the file
		FileTimeToLocalFileTime( &sLastWriteUtcFileTime, pLastWriteTime );
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
#endif

#if 0
INT32	CompareSGPFileTimes( SGP_FILETIME	*pFirstFileTime, SGP_FILETIME *pSecondFileTime )
{
#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	return( CompareFileTime( pFirstFileTime, pSecondFileTime ) );

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

// -------------------- End of Linux-specific stuff ------------------------
#endif	
}
#endif

UINT32 FileSize(STR strFilename)
{
UINT32 uiSize;

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

//**************************************************************************
//
// AddSubdirectoryToPath
//
//		Puts a subdirectory of the current working directory into the current 
// task's system path. 
//
// Parameter List :
// Return Value :
// Modification history :
//
//		10June98:DB		-> creation
//
//**************************************************************************

#if 0
BOOLEAN AddSubdirectoryToPath(CHAR8 *pDirectory)
{
CHAR8	*pSystemPath;
CHAR8 *pPath;
UINT32 uiPathLen;

	// Check for NULL
	if(!pDirectory)
		return(FALSE);

	// Check for zero length string
	if(!strlen(pDirectory))
		return(FALSE);

	if((pSystemPath=(CHAR8 *)MemAlloc(_MAX_PATH))==NULL)
		return(FALSE);

	memset(pSystemPath, 0, _MAX_PATH);

	if((pPath=(CHAR8 *)MemAlloc(_MAX_PATH))==NULL)
	{
		MemFree(pSystemPath);
		return(FALSE);
	}

	memset(pPath, 0, _MAX_PATH);

	// Builds a path to the directory with the SR DLL files.
	_getcwd(pPath, _MAX_PATH);
	uiPathLen=strlen(pPath);
	if(uiPathLen)
		uiPathLen--;
	if(pPath[uiPathLen]!='\\')
		strcat(pPath, "\\");
	
	strcat(pPath, pDirectory);

	// Appends it to the path for the current task
	if(GetEnvironmentVariable("PATH", pSystemPath, _MAX_PATH))
	{
		strcat(pSystemPath, ";");
		strcat(pSystemPath, pPath);
		SetEnvironmentVariable("PATH", pSystemPath);
		MemFree(pSystemPath);
		MemFree(pPath);
		return(TRUE);
	}
	else
	{
		MemFree(pSystemPath);
		MemFree(pPath);
		return(FALSE);
	}
	
}


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
