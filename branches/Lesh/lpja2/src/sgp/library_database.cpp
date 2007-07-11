#ifdef JA2_PRECOMPILED_HEADERS
	#include "ja2_sgp_all.h"
#else
	#include "platform.h"
	#include "types.h"
	#include "file_man.h"
	#include "library_database.h"
	#include "mem_man.h"
	#include "wcheck.h"
	#include "debug.h"
	#include "video.h"
	#include "game_settings.h"
	#include "io_layer.h"
	#include "ini_reader.h"
	
	// need string and string array types
	#include "vfs_types.hpp"
	#include "vfs.hpp"
	#include "SDL_config_lib.h"
	#include "io_layer.h"
#endif

// This file still needs refactoring to be more understandable
//
// Lesh: June 2007 - customizable game resource sources
// There is two game resource sources: standard and additional.
// Old slf-files (only!) are standard resources, that were with this game all this time.
// New additional game resources are all that files in Data/ and Data-1.13/ directories
// and, maybe some new slf-files, or even zip-files (in future). Additional resources
// override original resources of the game as well, as override themselves. Resource
// sources are added to resource map consequently, so next added source (slf or dir)
// can override previously added resources.
//
// Resource sources will be read from cfg-file.
//
vfsStringArray	GameSource;
UINT16			NumberOfContainers;	// to know, how many slf-files are there

//used when doing the binary search of the libraries
INT16	gsCurrentLibrary = -1;

INT32		CompareFileNames( CHAR8 **arg1, FileHeaderStruct **arg2 );
BOOLEAN		GetFileHeaderFromLibrary( INT16 sLibraryID, const CHAR8 *pstrFileName, FileHeaderStruct **pFileHeader );
void		AddSlashToPath( STR pName );
HWFILE		CreateLibraryFileHandle( INT16 sLibraryID, UINT32 uiFileNum );
BOOLEAN 	CheckIfFileIsAlreadyOpen( const CHAR8 *pFileName, INT16 sLibraryID );
BOOLEAN		ReadGameSources( const CHAR8 *pCfgName );
BOOLEAN		ReadMultiValueEntry( const CHAR8 *pKey, const vfsString& DefaultPath );

INT32 CompareDirEntryFileNames( CHAR8 *arg1[], DIRENTRY **arg2 );


//************************************************************************
//
//	 InitializeFileDatabase():  Call this function to initialize the file
//	database.  It will use the gGameLibaries[] array for the list of libraries
//	and the define NUMBER_OF_LIBRARIES for the number of libraries.  The gGameLibaries
//	array is an array of structure, one of the fields determines if the library
//	will be initialized and game start.
//
//************************************************************************
BOOLEAN InitializeFileDatabase( )
{
	INT16		i, iCurrentLibIndex;
	UINT32		uiSize;
	BOOLEAN		fLibraryInited = FALSE;
	STRING512	zConfigName;
	STRING512	zGameHomeDir;

	printf("\nInitializing main libraries...\n");

	GetHomeDirectory( zGameHomeDir );
	sprintf( zConfigName, "%s%s", zGameHomeDir, "resources.cfg");

	NumberOfContainers = 0;
	GameSource.clear();
	// read all game sources

	if ( !ReadGameSources( zConfigName ) )
		return FALSE;

	//if all the libraries exist, set them up
	gFileDataBase.usNumberOfLibraries = NumberOfContainers;

	//allocate memory for the each of the library headers
	uiSize = NumberOfContainers * sizeof( LibraryHeaderStruct );
	if( uiSize )
	{ 
		gFileDataBase.pLibraries = (LibraryHeaderStruct *) MemAlloc( uiSize );
		CHECKF( gFileDataBase.pLibraries );

		//set all the memrory to 0
		memset( gFileDataBase.pLibraries, 0, uiSize );
		iCurrentLibIndex = 0;

		//Load up each library
		for( i = 0; i < GameSource.size(); i++ )
		{
			if ( IO_IsDirectory( GameSource[i].c_str() ) )
			{
				if ( !VFS.AddDirectoryContents(GameSource[i].c_str(), FALSE) )
					printf("AddDirectoryContents(%s) failed!\n", GameSource[i].c_str() );
				printf("  + %s ...success!\n", GameSource[i].c_str() );
			}
			//if the library exists
			else if( OpenLibrary( iCurrentLibIndex ) )
			{
				fLibraryInited = TRUE;
				if ( !VFS.AddContainerByIndex(iCurrentLibIndex) )
					printf("AddContainer(%s) failed!\n", GameSource[i].c_str());
				iCurrentLibIndex++;
				printf("  + %s ...success!\n", GameSource[i].c_str() );
			}
			//else the library doesnt exist
			else
			{
				printf("  + %s ...FAILED!\n", GameSource[i].c_str() );
				//gFileDataBase.pLibraries[ i ].fLibraryOpen = FALSE;
				return (FALSE);
			}
		}
		//signify that the database has been initialized ( only if there was a library loaded )
		gFileDataBase.fInitialized = fLibraryInited;
	}

	//allocate memory for the handles of the 'real files' that will be open
	//This is needed because the the code wouldnt be able to tell the difference between a 'real' handle and a made up one
	uiSize = INITIAL_NUM_HANDLES * sizeof( RealFileOpenStruct );
	gFileDataBase.RealFiles.pRealFilesOpen = (RealFileOpenStruct *) MemAlloc( uiSize );
	CHECKF( gFileDataBase.RealFiles.pRealFilesOpen );

	//clear the memory
	memset( gFileDataBase.RealFiles.pRealFilesOpen, 0, uiSize);

	//set the initial number how many files can be opened at the one time
	gFileDataBase.RealFiles.iSizeOfOpenFileArray = INITIAL_NUM_HANDLES;

	printf("----------> main libraries... done!\n\n");
	return(TRUE);
}


//************************************************************************
//
//	 ShutDownFileDatabase():  Call this function to close down the file
//	database.
//
//************************************************************************

BOOLEAN ShutDownFileDatabase( )
{
	UINT16 sLoop1;

	// Free up the memory used for each library
	for(sLoop1=0; sLoop1 < gFileDataBase.usNumberOfLibraries; sLoop1++)
		CloseLibrary( sLoop1 );

	//Free up the memory used for all the library headers
	if( gFileDataBase.pLibraries )
	{
		MemFree( gFileDataBase.pLibraries );
		gFileDataBase.pLibraries = NULL;
	}

	//loop through all the 'opened files' ( there should be no files open )
	for( sLoop1=0; sLoop1< gFileDataBase.RealFiles.iNumFilesOpen; sLoop1++)
	{
		FastDebugMsg( String("ShutDownFileDatabase( ):  ERROR:  real file id still exists, wasnt closed") );
		if ( !IO_File_Close( gFileDataBase.RealFiles.pRealFilesOpen[ sLoop1 ].hRealFileHandle ) )
		{
			fprintf(stderr, "Error closing library %d: errno=%d\n",
				gFileDataBase.RealFiles.pRealFilesOpen[ sLoop1 ].hRealFileHandle, errno);
		}
	}

	//Free up the memory used for the real files array for the opened files
	if( gFileDataBase.RealFiles.pRealFilesOpen )
	{
		MemFree( gFileDataBase.RealFiles.pRealFilesOpen );
		gFileDataBase.RealFiles.pRealFilesOpen = NULL;
	}

	GameSource.clear();

	return( TRUE );
}




BOOLEAN CheckForLibraryExistence( const CHAR8 *pLibraryName )
{
	return( IO_IsRegularFile( pLibraryName ) );
}




BOOLEAN InitializeLibrary( const CHAR8 *pLibraryName, LibraryHeaderStruct *pLibHeader )
{
	HANDLE	hFile;
	UINT16	usNumEntries=0;
	UINT32	uiNumBytesRead;
	UINT32	uiLoop;
	DIRENTRY DirEntry;
	LIBHEADER	LibFileHeader;
	UINT32	uiCount=0;	
	CHAR8		zTempPath[ SGPFILENAME_LEN ];

	hFile = IO_File_Open( pLibraryName, IO_ACCESS_READ );
	if ( hFile == -1 )
	{
		fprintf(stderr, "Error opening library %s: errno=%d\n", pLibraryName, errno);
		return( FALSE );
	}

	uiNumBytesRead = IO_File_Read( hFile, &LibFileHeader, sizeof( LIBHEADER ) );

	if( uiNumBytesRead != sizeof( LIBHEADER ) )
	{
		fprintf(stderr, "Error reading library header %d: errno=%d\n",
			hFile, errno);
		//Error Reading the file database header.
		return( FALSE );
	}

	if ( !IO_File_Seek( hFile, -( LibFileHeader.iEntries * (INT32)sizeof(DIRENTRY) ), IO_SEEK_FROM_END ) )
	{
		fprintf(stderr, "Error seeking library %d: errno=%d\n",
			hFile, errno);
		return(FALSE);
	}

	//loop through the library and determine the number of files that are FILE_OK
	//ie.  so we dont load the old or deleted files
	usNumEntries = 0;
	for( uiLoop=0; uiLoop<(UINT32)LibFileHeader.iEntries; uiLoop++ )
	{
		uiNumBytesRead = IO_File_Read( hFile, &DirEntry, sizeof( DIRENTRY ) );
		if( uiNumBytesRead != sizeof( DIRENTRY ) )
		{
			fprintf(stderr, "Error reading library entry %d: errno=%d\n",
				hFile, errno);
			//Error Reading the file database header.
			return( FALSE );
		}

		if( DirEntry.ubState == FILE_OK )
			usNumEntries++;
	}


	//Allocate enough memory for the library header
	pLibHeader->pFileHeader = (FileHeaderStruct *) MemAlloc( sizeof( FileHeaderStruct ) * usNumEntries );

	#ifdef JA2TESTVERSION
		pLibHeader->uiTotalMemoryAllocatedForLibrary = sizeof( FileHeaderStruct ) * usNumEntries;
	#endif

	if ( !IO_File_Seek( hFile, -( LibFileHeader.iEntries * (INT32)sizeof(DIRENTRY) ), IO_SEEK_FROM_END ) )
	{
		fprintf(stderr, "Error seeking library at 2nd time %d: errno=%d\n",
			hFile, errno);
		return(FALSE);
	}

	//loop through all the entries
	uiCount=0;
	for( uiLoop=0; uiLoop<(UINT32)LibFileHeader.iEntries; uiLoop++ )
	{
		uiNumBytesRead = IO_File_Read( hFile, &DirEntry, sizeof( DIRENTRY ) );
		if( uiNumBytesRead != sizeof( DIRENTRY ) )
		{
			fprintf(stderr, "Error reading library entry %d: errno=%d\n",
				hFile, errno);
			//Error Reading the file database header.
			return( FALSE );
		}

		if( DirEntry.ubState == FILE_OK )
		{
			//Check to see if the file is not longer then it should be
			if( ( strlen( DirEntry.sFileName ) + 1 ) >= FILENAME_SIZE )
				FastDebugMsg(String("\n*******InitializeLibrary():  Warning!:  '%s' from the library '%s' has name whose size (%d) is bigger then it should be (%s)", DirEntry.sFileName, pLibHeader->sLibraryPath, ( strlen( DirEntry.sFileName ) + 1 ), FILENAME_SIZE ) );


			//allocate memory for the files name
			pLibHeader->pFileHeader[ uiCount ].pFileName = (STR) MemAlloc( strlen( DirEntry.sFileName ) + 1 );

			//if we couldnt allocate memory
			if( !pLibHeader->pFileHeader[ uiCount ].pFileName )
			{
				//report an error
				return(FALSE);
			}


			#ifdef JA2TESTVERSION
				pLibHeader->uiTotalMemoryAllocatedForLibrary += strlen( DirEntry.sFileName ) + 1;
			#endif


			//copy the file name, offset and length into the header
			strcpy( pLibHeader->pFileHeader[ uiCount ].pFileName, DirEntry.sFileName);
			pLibHeader->pFileHeader[ uiCount ].uiFileOffset = DirEntry.uiOffset;
			pLibHeader->pFileHeader[ uiCount ].uiFileLength = DirEntry.uiLength;

			uiCount++;
		}
	}


	pLibHeader->usNumberOfEntries = usNumEntries;

	//allocate memory for the library path
//	if( strlen( LibFileHeader.sPathToLibrary ) == 0 )
	{
//		FastDebugMsg( String("The %s library file does not contain a path.  Use 'n' argument to name the library when you create it\n", LibFileHeader.sLibName ) );
//		Assert( 0 );
	}

	//if the library has a path
	if( strlen( LibFileHeader.sPathToLibrary ) != 0 )
	{
		pLibHeader->sLibraryPath = (STR) MemAlloc( strlen( LibFileHeader.sPathToLibrary ) + 1 );
		strcpy( pLibHeader->sLibraryPath, LibFileHeader.sPathToLibrary );
	}
	else
	{
		//else the library name does not contain a path ( most likely either an error or it is the default path )
		pLibHeader->sLibraryPath = (STR) MemAlloc( 1 );
		pLibHeader->sLibraryPath[0] = '\0';
	}


	#ifdef JA2TESTVERSION
		pLibHeader->uiTotalMemoryAllocatedForLibrary += strlen( LibFileHeader.sPathToLibrary ) + 1;
	#endif


	//allocate space for the open files array
	pLibHeader->pOpenFiles = (FileOpenStruct *) MemAlloc( INITIAL_NUM_HANDLES * sizeof( FileOpenStruct ) );
	if( !pLibHeader->pOpenFiles )
	{
			//report an error
			return(FALSE);
	}

	memset( pLibHeader->pOpenFiles, 0, INITIAL_NUM_HANDLES * sizeof( FileOpenStruct ) );

	#ifdef JA2TESTVERSION
		pLibHeader->uiTotalMemoryAllocatedForLibrary += INITIAL_NUM_HANDLES * sizeof( FileOpenStruct );
	#endif



	pLibHeader->hLibraryHandle = hFile;
	pLibHeader->usNumberOfEntries = usNumEntries;
	pLibHeader->fLibraryOpen = TRUE;
	pLibHeader->iNumFilesOpen = 0;
	pLibHeader->iSizeOfOpenFileArray = INITIAL_NUM_HANDLES;

	return( TRUE );
}




BOOLEAN LoadDataFromLibrary( INT16 sLibraryID, UINT32 uiFileNum, PTR pData, UINT32 uiBytesToRead, UINT32 *pBytesRead )
{
	UINT32	uiOffsetInLibrary, uiLength;
	HANDLE	hLibraryFile;
	UINT32	uiNumBytesRead;
	UINT32	uiCurPos;


	//get the offset into the library, the length and current position of the file pointer.
	uiOffsetInLibrary = gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].pFileHeader->uiFileOffset;
	uiLength          = gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].pFileHeader->uiFileLength;
	hLibraryFile      = gFileDataBase.pLibraries[ sLibraryID ].hLibraryHandle;
	uiCurPos          = gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].uiFilePosInFile;

	if ( !IO_File_Seek( hLibraryFile, ( uiOffsetInLibrary + uiCurPos ), IO_SEEK_FROM_START ) )
	{
		fprintf(stderr, "Error seeking library (load) %d: errno=%d\n",
			hLibraryFile, errno);
		return(FALSE);
	}

	//if we are trying to read more data then the size of the file, return an error
	if( uiBytesToRead + uiCurPos > uiLength )
	{
		*pBytesRead = 0;
		return( FALSE );
	}

	uiNumBytesRead = IO_File_Read( hLibraryFile, pData, uiBytesToRead );

	if( uiBytesToRead != uiNumBytesRead )
	{
		fprintf(stderr, "Error reading library entry %d: errno=%d\n",
			hLibraryFile, errno);
		return( FALSE );
	}

	gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].uiFilePosInFile += uiNumBytesRead;

	//	CloseHandle( hLibraryFile );

	*pBytesRead = uiNumBytesRead;

	return( TRUE );
}

//************************************************************************
//
// CheckIfFileExistInLibrary() determines if a file exists in a library.
//
//************************************************************************

BOOLEAN CheckIfFileExistInLibrary( const CHAR8 *pFileName )
{
	INT16 sLibraryID;
	FileHeaderStruct *pFileHeader;

	//get thelibrary that file is in
	sLibraryID = GetLibraryIDFromFileName( pFileName );
	if( sLibraryID == -1 )
	{
		//not in any library
		return( FALSE );
	}

	if( GetFileHeaderFromLibrary( sLibraryID, pFileName, &pFileHeader ) )
		return( TRUE );
	else
		return( FALSE );
}


//************************************************************************
//
//	This function finds out if the file CAN be in a library.  It determines
//	if the library that the file MAY be in is open.
//	( eg. File is  Laptop\Test.sti, if the Laptop\ library is open, it returns true
//
//************************************************************************
INT16 GetLibraryIDFromFileName( const CHAR8 *pFileName )
{
INT16 sLoop1, sBestMatch=-1;

	//loop through all the libraries to check which library the file is in
	for( sLoop1=0; sLoop1<gFileDataBase.usNumberOfLibraries; sLoop1++)
	{
		//if the library is not loaded, dont try to access the array
		if( IsLibraryOpened( sLoop1 ) )
		{
			//if the library path name is of size zero, ( the library is for the default path )
			if( strlen( gFileDataBase.pLibraries[ sLoop1 ].sLibraryPath ) == 0 )
			{
				//determine if there is a directory in the file name
				if( strchr( pFileName, '\\' ) == NULL && strchr( pFileName, '/' ) == NULL )
				{
					//There is no directory in the file name
					return( sLoop1 );
				}
			}

			//compare the library name to the file name that is passed in
			else 
			{
				// if the directory paths are the same, to the length of the lib's path
				if( _strnicmp( gFileDataBase.pLibraries[ sLoop1 ].sLibraryPath, pFileName, strlen( gFileDataBase.pLibraries[ sLoop1 ].sLibraryPath ) ) == 0 )
				{
					// if we've never matched, or this match's path is longer than the previous match (meaning it's more exact)
					if((sBestMatch==(-1)) || (strlen(gFileDataBase.pLibraries[ sLoop1 ].sLibraryPath) > strlen(gFileDataBase.pLibraries[ sBestMatch ].sLibraryPath)))
						sBestMatch = sLoop1;
				}
			}
		}
	}

	//no library was found, return an error
	return(sBestMatch);
}


//************************************************************************
//
//	GetFileHeaderFromLibrary() performsperforms a binary search of the
//	library.  It adds the libraries path to the file in the
//	library and then string compared that to the name that we are
//	searching for.
//
//************************************************************************

BOOLEAN	GetFileHeaderFromLibrary( INT16 sLibraryID, const CHAR8 *pstrFileName, FileHeaderStruct **pFileHeader )
{
	FileHeaderStruct **ppFileHeader;
	CHAR8		sFileNameWithPath[ FILENAME_SIZE ];

	//combine the library path to the file name (need it for the search of the library )
	strcpy( sFileNameWithPath, pstrFileName);

	gsCurrentLibrary = sLibraryID;

	 /* try to find the filename using a binary search algorithm: */
	 ppFileHeader = (FileHeaderStruct **) bsearch( (char *) &sFileNameWithPath, (FileHeaderStruct *) gFileDataBase.pLibraries[ sLibraryID ].pFileHeader, gFileDataBase.pLibraries[ sLibraryID ].usNumberOfEntries,
															sizeof( FileHeaderStruct ), (int (*)(const void*, const void*))CompareFileNames );

	 if( ppFileHeader )
	 {
			*pFileHeader = ( FileHeaderStruct * ) ppFileHeader;
			return( TRUE );
	 }
	 else
	 {
			pFileHeader = NULL;
			return( FALSE );
	 }
}


//************************************************************************
//
//	CompareFileNames() gets called by the binary search function.
//
//************************************************************************

INT32 CompareFileNames( CHAR8 *arg1[], FileHeaderStruct **arg2 )
{
	CHAR8		sSearchKey[ FILENAME_SIZE ];
	CHAR8		sFileNameWithPath[ FILENAME_SIZE ];
	FileHeaderStruct *TempFileHeader;

	TempFileHeader = ( FileHeaderStruct * ) arg2;

	sprintf( sSearchKey, "%s", arg1);

	sprintf( sFileNameWithPath, "%s%s", gFileDataBase.pLibraries[ gsCurrentLibrary ].sLibraryPath, TempFileHeader->pFileName );

   /* Compare all of both strings: */
   return _stricmp( sSearchKey, sFileNameWithPath );
}




void AddSlashToPath( STR pName )
{
	UINT32	uiLoop, uiCounter;
	BOOLEAN	fDone = FALSE;
	BOOLEAN fFound = FALSE;
	CHAR8		sNewName[ FILENAME_SIZE ];

	//find out if there is a '\' in the file name

	uiCounter=0;
	for( uiLoop=0; uiLoop < strlen( pName ) && !fDone; uiLoop++)
	{
		if( pName[ uiLoop ] == SLASH )
		{
			sNewName[ uiCounter ] = pName[ uiLoop ];
			uiCounter++;
			sNewName[ uiCounter ] = SLASH;
		}
		else
			sNewName[ uiCounter ] = pName[ uiLoop ];

		uiCounter++;
	}
	sNewName[ uiCounter ] = '\0';

	strcpy( pName, sNewName );
}



//************************************************************************
//
// This function will see if a file is in a library.  If it is, the file will be opened and a file
// handle will be created for it.
//
//************************************************************************

HWFILE OpenFileFromLibrary(const CHAR8 *pName )
{
	FileHeaderStruct *pFileHeader;
	HWFILE					hLibFile;
	INT16							sLibraryID;
	UINT16						uiLoop1;
	UINT32						uiFileNum=0;

	UINT32						uiNewFilePosition=0;


	//Check if the file can be contained from an open library ( the path to the file a library path )
	sLibraryID = GetLibraryIDFromFileName( pName );

	if( sLibraryID != -1 )
	{
		//Check if another file is already open in the library ( report a warning if so )

//		if( gFileDataBase.pLibraries[ sLibraryID ].fAnotherFileAlreadyOpenedLibrary )
		if( gFileDataBase.pLibraries[ sLibraryID ].uiIdOfOtherFileAlreadyOpenedLibrary != 0 )
		{
		}

		//check if the file is already open
		if( CheckIfFileIsAlreadyOpen( pName, sLibraryID ) )
			return( 0 );

		//if the file is in a library, get the file
		if( GetFileHeaderFromLibrary( sLibraryID, pName, &pFileHeader ) )
		{
			//increment the number of open files
			gFileDataBase.pLibraries[ sLibraryID ].iNumFilesOpen ++;

			//if there isnt enough space to put the file, realloc more space
			if( gFileDataBase.pLibraries[ sLibraryID ].iNumFilesOpen >= gFileDataBase.pLibraries[ sLibraryID ].iSizeOfOpenFileArray )
			{
				FileOpenStruct	*pOpenFiles;

				//reallocate more space for the array
				pOpenFiles = (FileOpenStruct *) MemRealloc( gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles,
								 gFileDataBase.pLibraries[ sLibraryID ].iSizeOfOpenFileArray + NUM_FILES_TO_ADD_AT_A_TIME );

				if( !pOpenFiles )
					return( 0 );

				//increment the number of open files that we can have open
				gFileDataBase.pLibraries[ sLibraryID ].iSizeOfOpenFileArray += NUM_FILES_TO_ADD_AT_A_TIME;


				gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles = pOpenFiles;
			}

			//loop through to find a new spot in the array
			uiFileNum = 0;
			for( uiLoop1=1; uiLoop1 < gFileDataBase.pLibraries[ sLibraryID ].iSizeOfOpenFileArray; uiLoop1++)
			{
				if( gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiLoop1 ].uiFileID == 0 )
				{
					uiFileNum = uiLoop1;
					break;
				}
			}

			//if for some reason we couldnt find a spot, return an error
			if( uiFileNum == 0 )
				return( 0 );

			//Create a library handle for the new file
			hLibFile = CreateLibraryFileHandle( sLibraryID, uiFileNum );

			//Set the current file data into the array of open files
			gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].uiFileID = hLibFile;
			gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].uiFilePosInFile = 0;
			gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].pFileHeader = pFileHeader;
			gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].uiActualPositionInLibrary = IO_File_GetPosition( gFileDataBase.pLibraries[ sLibraryID ].hLibraryHandle );

			if ( (uiNewFilePosition = IO_File_GetSize( gFileDataBase.pLibraries[ sLibraryID ].hLibraryHandle )) == -1 )
			{
				fprintf(stderr, "Error getting size of library %d: errno=%d\n",
					gFileDataBase.pLibraries[ sLibraryID ].hLibraryHandle, errno);
				return 0;
			}
		}
		else
		{
			// Failed to find the file in a library
			return( 0 );
		}
	}
	else
	{
		// Library is not open, or doesnt exist
		return( 0 );
	}

	//Set the fact the a file is currently open in the library
//	gFileDataBase.pLibraries[ sLibraryID ].fAnotherFileAlreadyOpenedLibrary = TRUE;
	gFileDataBase.pLibraries[ sLibraryID ].uiIdOfOtherFileAlreadyOpenedLibrary = uiFileNum;

	return( hLibFile );
}




HWFILE CreateLibraryFileHandle( INT16 sLibraryID, UINT32 uiFileNum )
{
	HWFILE hLibFile;


	hLibFile = uiFileNum;
	hLibFile |= DB_ADD_LIBRARY_ID( sLibraryID );

	return( hLibFile );
}


HWFILE CreateRealFileHandle( HANDLE hFile )
{
	HWFILE hLibFile;
	INT32	iLoop1;
	UINT32	uiFileNum=0;
	UINT32 uiSize;

	//if there isnt enough space to put the file, realloc more space
	if( gFileDataBase.RealFiles.iNumFilesOpen >= ( gFileDataBase.RealFiles.iSizeOfOpenFileArray -1 ) )
	{
		uiSize = ( gFileDataBase.RealFiles.iSizeOfOpenFileArray + NUM_FILES_TO_ADD_AT_A_TIME ) * sizeof( RealFileOpenStruct );

		gFileDataBase.RealFiles.pRealFilesOpen = (RealFileOpenStruct *) MemRealloc( gFileDataBase.RealFiles.pRealFilesOpen, uiSize );
		CHECKF( gFileDataBase.RealFiles.pRealFilesOpen );

		//Clear out the new part of the array
		memset( &gFileDataBase.RealFiles.pRealFilesOpen[ gFileDataBase.RealFiles.iSizeOfOpenFileArray ], 0, ( NUM_FILES_TO_ADD_AT_A_TIME * sizeof( RealFileOpenStruct ) ) );

		gFileDataBase.RealFiles.iSizeOfOpenFileArray += NUM_FILES_TO_ADD_AT_A_TIME;
	}


	//loop through to find a new spot in the array
	uiFileNum = 0;
	for( iLoop1=1; iLoop1 < gFileDataBase.RealFiles.iSizeOfOpenFileArray; iLoop1++)
	{
		if( gFileDataBase.RealFiles.pRealFilesOpen[ iLoop1 ].uiFileID == 0 )
		{
			uiFileNum = iLoop1;
			break;
		}
	}

	//if for some reason we couldnt find a spot, return an error
	if( uiFileNum == 0 )
		return( 0 );

	hLibFile = uiFileNum;
	hLibFile |= DB_ADD_LIBRARY_ID( REAL_FILE_LIBRARY_ID );

	gFileDataBase.RealFiles.pRealFilesOpen[ iLoop1 ].uiFileID = hLibFile;
	gFileDataBase.RealFiles.pRealFilesOpen[ iLoop1 ].hRealFileHandle = hFile;

	gFileDataBase.RealFiles.iNumFilesOpen++;

	return( hLibFile );
}




BOOLEAN GetLibraryAndFileIDFromLibraryFileHandle( HWFILE hlibFile, INT16 *pLibraryID, UINT32 *pFileNum )
{
	*pFileNum = DB_EXTRACT_FILE_ID( hlibFile );
	*pLibraryID = (UINT16)DB_EXTRACT_LIBRARY( hlibFile );

	return( TRUE );
}


//************************************************************************
//
//	Close an individual file that is contained in the library
//
//************************************************************************



BOOLEAN CloseLibraryFile( INT16 sLibraryID, UINT32 uiFileID )
{
	if( IsLibraryOpened( sLibraryID ) )
	{
		//if the uiFileID is invalid
		if( (uiFileID >= (UINT32)gFileDataBase.pLibraries[ sLibraryID ].iSizeOfOpenFileArray ) )
			return( FALSE );
			
		//if the file is not opened, dont close it
		if( gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileID ].uiFileID != 0 )
		{
			//reset the variables
			gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileID ].uiFileID = 0;
			gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileID ].uiFilePosInFile = 0;
			gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileID ].pFileHeader = NULL;

			IO_File_Seek( gFileDataBase.pLibraries[ sLibraryID ].hLibraryHandle, gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileID ].uiActualPositionInLibrary, IO_SEEK_FROM_START );

			//decrement the number of files that are open
			gFileDataBase.pLibraries[ sLibraryID ].iNumFilesOpen--;

			// Reset the fact that a file is accessing the library
//			gFileDataBase.pLibraries[ sLibraryID ].fAnotherFileAlreadyOpenedLibrary = FALSE;
			gFileDataBase.pLibraries[ sLibraryID ].uiIdOfOtherFileAlreadyOpenedLibrary = 0;
		}
	}

	return( TRUE );
}


BOOLEAN LibraryFileSeek( INT16 sLibraryID, UINT32 uiFileNum, UINT32 uiDistance, UINT8 uiHowToSeek )
{
	UINT32	uiCurPos, uiSize;

	//if the library is not open, return an error
	if( !IsLibraryOpened( sLibraryID ) )
		return( FALSE );

	uiCurPos = gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].uiFilePosInFile;
	uiSize = gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].pFileHeader->uiFileLength;


	if ( uiHowToSeek == FILE_SEEK_FROM_START )
		uiCurPos = uiDistance;
	else if ( uiHowToSeek == FILE_SEEK_FROM_END )
		uiCurPos = uiSize - uiDistance;
	else if ( uiHowToSeek == FILE_SEEK_FROM_CURRENT )
		uiCurPos += uiDistance;
	else
		return(FALSE);

	gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].uiFilePosInFile = uiCurPos;
	return( TRUE );
}




//************************************************************************
//
//	OpenLibrary() Opens a library from the 'array' of library names
//	that was passd in at game initialization.  Pass in an enum for the
//	library.
//
//************************************************************************

BOOLEAN OpenLibrary( INT16 sLibraryID )
{
	//if the library is already opened, report an error
	if( gFileDataBase.pLibraries[ sLibraryID ].fLibraryOpen )
		return( FALSE );

	//if we are trying to do something with an invalid library id
	if( sLibraryID >= gFileDataBase.usNumberOfLibraries )
		return( FALSE );

	//if we cant open the library
	if( !InitializeLibrary( GameSource.at( sLibraryID ).c_str(), &gFileDataBase.pLibraries[ sLibraryID ] ) )
		return( FALSE );

	return( TRUE );
}




BOOLEAN CloseLibrary( INT16 sLibraryID )
{
	UINT32	uiLoop1;

	//if the library isnt loaded, dont close it
	if( !IsLibraryOpened( sLibraryID ) )
		return( FALSE );

	#ifdef JA2TESTVERSION
		FastDebugMsg( String("ShutDownFileDatabase( ): %d bytes of ram used for the Library #%3d, path '%s',  in the File Database System\n", gFileDataBase.pLibraries[ sLibraryID ].uiTotalMemoryAllocatedForLibrary, sLibraryID, gFileDataBase.pLibraries[ sLibraryID ].sLibraryPath ));
		gFileDataBase.pLibraries[ sLibraryID ].uiTotalMemoryAllocatedForLibrary = 0;
	#endif

	//if there are any open files, loop through the library and close down whatever file is still open
	if( gFileDataBase.pLibraries[ sLibraryID ].iNumFilesOpen )
	{
		//loop though the array of open files to see if any are still open
		for( uiLoop1=0; uiLoop1<( UINT32 )gFileDataBase.pLibraries[ sLibraryID ].usNumberOfEntries; uiLoop1++)
		{
			if( CheckIfFileIsAlreadyOpen( gFileDataBase.pLibraries[ sLibraryID ].pFileHeader[ uiLoop1 ].pFileName, sLibraryID ) )
			{
				FastDebugMsg( String("CloseLibrary():  ERROR:  %s library file id still exists, wasnt closed, closing now.", gFileDataBase.pLibraries[ sLibraryID ].pFileHeader[ uiLoop1 ].pFileName ) );
				CloseLibraryFile( sLibraryID, uiLoop1 );

				//	Removed because the memory gets freed in the next for loop.  Would only enter here if files were still open
				//	gFileDataBase.pLibraries[ sLibraryID ].pFileHeader[ uiLoop1 ].pFileName = NULL;
			}
		}
	}

	//Free up the memory used for each file name
	for( uiLoop1=0; uiLoop1<gFileDataBase.pLibraries[ sLibraryID ].usNumberOfEntries; uiLoop1++)
	{
		MemFree( gFileDataBase.pLibraries[ sLibraryID ].pFileHeader[ uiLoop1 ].pFileName );
		gFileDataBase.pLibraries[ sLibraryID ].pFileHeader[ uiLoop1 ].pFileName = NULL;
	}

	//Free up the memory needed for the Library File Headers
	if( gFileDataBase.pLibraries[ sLibraryID ].pFileHeader )
	{
		MemFree( gFileDataBase.pLibraries[ sLibraryID ].pFileHeader );
		gFileDataBase.pLibraries[ sLibraryID ].pFileHeader = NULL;
	}

	//Free up the memory used for the library name
	if( gFileDataBase.pLibraries[ sLibraryID ].sLibraryPath )
	{
		MemFree( gFileDataBase.pLibraries[ sLibraryID ].sLibraryPath );
		gFileDataBase.pLibraries[ sLibraryID ].sLibraryPath = NULL;
	}

	//Free up the space requiered for the open files array
	if( gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles )
	{
		MemFree( gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles );
		gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles = NULL;
	}

	//set that the library isnt open
	gFileDataBase.pLibraries[ sLibraryID ].fLibraryOpen = FALSE;

	if ( !IO_File_Close( gFileDataBase.pLibraries[ sLibraryID ].hLibraryHandle ) )
	{
		fprintf(stderr, "Error closing library %d: errno=%d\n",
			gFileDataBase.pLibraries[ sLibraryID ].hLibraryHandle, errno);
	}


	return( TRUE );
}

BOOLEAN IsLibraryOpened( INT16 sLibraryID )
{
	//if the database is not initialized
	if( !gFileDataBase.fInitialized )
		return( FALSE );

	//if we are trying to do something with an invalid library id
	if( sLibraryID >= gFileDataBase.usNumberOfLibraries )
		return( FALSE );

	//if the library is opened
	if( gFileDataBase.pLibraries[ sLibraryID ].fLibraryOpen )
		return( TRUE );
	else
		return( FALSE );
}




BOOLEAN CheckIfFileIsAlreadyOpen( const CHAR8 *pFileName, INT16 sLibraryID )
{
	UINT16 usLoop1=0;
	CHAR8 *pName;
	CHAR8 *pFilenameWithoutPath;

	pFilenameWithoutPath = (CHAR8*) pFileName;
	pName = (CHAR8*) pFileName;

	while ((pFilenameWithoutPath = strpbrk(pName, "/\\")) != NULL)
	{
		pName = pFilenameWithoutPath + 1;
	}

	//loop through all the open files to see if 'new' file to open is already open
	for( usLoop1=1; usLoop1 < gFileDataBase.pLibraries[ sLibraryID ].iSizeOfOpenFileArray ; usLoop1++ )
	{
		//check if the file is open
		if( gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ usLoop1].uiFileID != 0 )
		{
			//Check if the file already exists
			if( _stricmp( pFilenameWithoutPath, gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ usLoop1].pFileHeader->pFileName ) == 0 )
				return( TRUE );
		}
	}
	return( FALSE );
}


BOOLEAN GetLibraryFileTime( INT16 sLibraryID, UINT32 uiFileNum, SGP_FILETIME	*pLastWriteTime )
{
	UINT16	usNumEntries=0;
	UINT32	uiNumBytesRead;
	DIRENTRY *pDirEntry;
	LIBHEADER	LibFileHeader;
	BOOLEAN fDone = FALSE;
//	UINT32	cnt;
	INT32	iFilePos=0;

	DIRENTRY **ppDirEntry;

	DIRENTRY *pAllEntries;

	memset( pLastWriteTime, 0, sizeof( SGP_FILETIME ) );

	IO_File_Seek( gFileDataBase.pLibraries[ sLibraryID ].hLibraryHandle, 0, IO_SEEK_FROM_START );
	uiNumBytesRead = IO_File_Read( gFileDataBase.pLibraries[ sLibraryID ].hLibraryHandle, &LibFileHeader, sizeof( LIBHEADER ));

	if( uiNumBytesRead != sizeof( LIBHEADER ) )
	{
		fprintf(stderr, "Error reading library %d: errno=%d\n",
			gFileDataBase.pLibraries[ sLibraryID ].hLibraryHandle, errno);
		//Error Reading the file database header.
		return( FALSE );
	}


	//If the file number is greater then the number in the lirary, return false
	if( uiFileNum >= (UINT32)LibFileHeader.iEntries )
		return( FALSE );

	pAllEntries = (DIRENTRY *) MemAlloc( sizeof( DIRENTRY ) * LibFileHeader.iEntries );
	if( pAllEntries == NULL )
		return( FALSE );
	memset( pAllEntries, 0, sizeof( DIRENTRY ) );

	iFilePos = -( LibFileHeader.iEntries * (INT32)sizeof(DIRENTRY) );
	IO_File_Seek( gFileDataBase.pLibraries[ sLibraryID ].hLibraryHandle, iFilePos, IO_SEEK_FROM_END );
	uiNumBytesRead = IO_File_Read( gFileDataBase.pLibraries[ sLibraryID ].hLibraryHandle, pAllEntries, ( sizeof( DIRENTRY ) * LibFileHeader.iEntries ));

	if( uiNumBytesRead != ( sizeof( DIRENTRY ) * LibFileHeader.iEntries ) )
	{
		fprintf(stderr, "Error reading library %d: errno=%d\n",
			gFileDataBase.pLibraries[ sLibraryID ].hLibraryHandle, errno);
		//Error Reading the file database header.
		return( FALSE );
	}

	 /* try to find the filename using a binary search algorithm: */
	 ppDirEntry = (DIRENTRY **) bsearch( gFileDataBase.pLibraries[ sLibraryID ].pOpenFiles[ uiFileNum ].pFileHeader->pFileName, 
																			(DIRENTRY *) pAllEntries, 
																			LibFileHeader.iEntries,
																			sizeof( DIRENTRY ), (int (*)(const void*, const void*))CompareDirEntryFileNames );

	 if( ppDirEntry )
		pDirEntry = ( DIRENTRY * ) ppDirEntry;
	 else
		return( FALSE );

	//Copy the dir entry time over to the passed in time
	memcpy( pLastWriteTime, &pDirEntry->sFileTime, sizeof( SGP_FILETIME ) );


	MemFree( pAllEntries );
	pAllEntries = NULL;

	return( TRUE );
}



//************************************************************************
//
//	CompareFileNames() gets called by the binary search function.
//
//************************************************************************

INT32 CompareDirEntryFileNames( CHAR8 *arg1[], DIRENTRY **arg2 )
{
	CHAR8				sSearchKey[ FILENAME_SIZE ];
	CHAR8				sFileNameWithPath[ FILENAME_SIZE ];
	DIRENTRY		*TempDirEntry;

	TempDirEntry = ( DIRENTRY * ) arg2;

	sprintf( sSearchKey, "%s", arg1);

	sprintf( sFileNameWithPath, "%s", TempDirEntry->sFileName );

   /* Compare all of both strings: */
   return _stricmp( sSearchKey, sFileNameWithPath );
}


//************************************************************************
//
//	ReadGameSources -  reads game sources names and counts number of
//	slf-files among them.
//
//	in	pCfgName:	name of the config file with paths and filenames
//					as game sources.
//
//	return:	TRUE, if all game sources were successfully added,
//			FALSE, otherwise
//
//	Game sources config file template:
//
//	[StandardSources]
//	StdBaseDir = "/usr/share/ja2/"
//	StdSources = "data.slf", "bigitems.slf", ...
//
//	[AdditionalSources]
//	AddSources = "Data/", "Data-1.13/"
//
//************************************************************************
BOOLEAN		ReadGameSources( const CHAR8 *pCfgName )
{
	CFG_File	cfgResources;
	vfsString	StandardSourcesDir;
	STRING512	zGameHomeDir;

	GetHomeDirectory( zGameHomeDir );

	// Open config-file with name pCfgName, report if error.
	// Then select group [StandardSources], which is describes
	// path and filenames of the standard sources
	if ( CFG_OpenFile( pCfgName, &cfgResources ) != CFG_OK )
	{
		fprintf(stderr, "ReadGameResources config: Failed to open %s\n", pCfgName);
		return FALSE;
	}
	if ( CFG_SelectGroup( "StandardSources", FALSE ) != CFG_OK )
	{
		fprintf(stderr, "ReadGameResources config: Failed to select standard sources group\n");
		CFG_CloseFile( &cfgResources );
		return FALSE;
	}

	// After selecting group, if all was successful, read the path,
	// where standard sources are exist. Then start reading sources
	// filenames from the entry StdSources, which contains multiple
	// string values.
	StandardSourcesDir = CFG_ReadText( "StdBaseDir", "/usr/share/ja2/" );
	if ( StandardSourcesDir[ StandardSourcesDir.length() - 1 ] != SLASH )
		StandardSourcesDir += SLASH;
	ReadMultiValueEntry( "StdSources", StandardSourcesDir );

	// Here the standard sources section ends and additional sources section begins
	// Try to select group [AdditionalSources]. Do not report, if section is absent.
	// This is not error situation (assume, that standard section has everything in
	// order to run the game. After section selecting, read source filenames from
	// multivalue entry AddSources.
	if ( CFG_SelectGroup( "AdditionalSources", FALSE ) == CFG_OK )
	{
		ReadMultiValueEntry( "AddSources", zGameHomeDir );
	}

	CFG_CloseFile( &cfgResources );
	return TRUE;
}

//************************************************************************
//
//	ReadMultiValueEntry -  reads values from a multi-value entry from
//	config file .
//
//	in	pKey:			name of the multi-value entry
//	in	DefaultPath:	if value isn't full root path, use this path
//						to make it full
//
//	return:	TRUE always
//
//************************************************************************
BOOLEAN		ReadMultiValueEntry( const CHAR8 *pKey, const vfsString& DefaultPath )
{
	vfsString	SourceName, FullSourceName;

	// handle each value from multivalue entry
	for ( CFG_StartMultiValueEntryIteration( pKey ); !CFG_IsLastMultiValueEntry(); CFG_SelectNextMultiValueEntry() )
	{
		SourceName = CFG_ReadText( CFG_ENTRY_ITERATION, "error" );

		// Allow fully qualified path and path within specified DefaultPath directory.
		if ( IO_IsRootPath( SourceName.c_str() ) )
			FullSourceName = SourceName;
		else
			FullSourceName = DefaultPath + SourceName;

		// If it is a file, consider him as container (slf, zip, etc...)
		// and increase container counter. Remember path.
		// If it is a directory, add slash to it's end (if needed), and
		// remember it.
		// If it is not a file, nor a directory, i.e. filename doesn't
		// exist, report about it.
		if ( CheckForLibraryExistence( FullSourceName.c_str() ) )
		{
			NumberOfContainers++;
			GameSource.push_back( FullSourceName );
		}
		else if ( IO_IsDirectory( FullSourceName.c_str() ) )
		{
			if ( FullSourceName[ FullSourceName.length() - 1 ] != SLASH )
				FullSourceName += SLASH;
			GameSource.push_back( FullSourceName );
		}
		else
			fprintf(stderr, "ReadGameResources config: Detected non-existing game source %s\n", SourceName.c_str());
	}
	return TRUE;
}
