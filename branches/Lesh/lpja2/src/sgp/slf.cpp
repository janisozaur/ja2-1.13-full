#ifdef JA2_PRECOMPILED_HEADERS
	#include "ja2_sgp_all.h"
#else
	#include "platform.h"
	#include "types.h"
	#include "file_man.h"
	#include "slf.h"
	#include "mem_man.h"
	#include "wcheck.h"
	#include "debug.h"
#	include "path_utils.h"
#	include <physfs.h>
	
#endif

/************************************************************
**
**				Defines
**
************************************************************/

#define	FILENAME_SIZE					256

#define NUM_FILES_TO_ADD_AT_A_TIME		20
#define INITIAL_NUM_HANDLES				20

// these defines are used in slf containers
#define FILE_OK					0
#define FILE_DELETED			0xff
#define FILE_OLD				1
#define FILE_DOESNT_EXIST		0xfe

/************************************************************
**
**				Typedefs
**
************************************************************/


// this structs reflects inner structures of slf containers
typedef struct 
{
	CHAR8		sLibName[ FILENAME_SIZE ];
	CHAR8		sPathToLibrary[ FILENAME_SIZE ];
	INT32		iEntries;
	INT32		iUsed;
	UINT16		iSort;
	UINT16		iVersion;
	BOOLEAN		fContainsSubDirectories;
	INT32		iReserved;
} LIBHEADER;



typedef struct 
{
	CHAR8			sFileName[ FILENAME_SIZE ];
	UINT32			uiOffset;
	UINT32			uiLength;
	UINT8			ubState;
	UINT8			ubReserved[15];
} DIRENTRY;



// ------------ SLF_Handler --------------
typedef struct
{
	INT16	sUsed;			// usage flag
	INT32	iLibIndex;		// library index
	INT32	iFileIndex;		// file index in library file headers
	INT32	iFileOffset;	// current file offset
} SLF_Handler;



// ------------ SLF_FileHeader --------------
typedef struct
{
	STR		pFileName;		// filename
	INT32	uiFileLength;	// file length in bytes
	INT32	uiFileOffset;	// file offset in slf library
} SLF_FileHeader;



// ------------ SLF_Library --------------
typedef struct
{
	STR					sLibraryPath;		// files in slf will appear in this directory
	fsFile				hLibraryHandle;		// PhysFS handler to slf file
	UINT16				usNumberOfEntries;	// number of resources in slf
	SLF_FileHeader		*pFileHeader;		// resource entries
	BOOLEAN				fLibraryOpen;		// slf open flag
} SLF_Library;



// ------------ SLF_Manager --------------
typedef struct
{
	SLF_Library			*pLibraries;				// array of libraries
	INT16				sTotalNumberOfLibrarySlots;	// array of libraries size
	INT16				sUsedLibrarySlots;			// number of used cells in array of libraries
	SLF_Handler			*pHandlers;					// opened files handles array
	INT16				sTotalHandlers;				// opened files handles array size
	INT16				sUsedHandlers;				// number of used cells in opened files handles array
} SLF_Manager;



//The SLF Database
SLF_Manager	gSLF;

// Internal SLF functions

BOOLEAN SLF_AddLibraryInternal( const CHAR8 *pLibraryName, SLF_Library *pLibHeader );
void	SLF_FreeLibrary       ( SLF_Library *pLibHeader );
BOOLEAN SLF_Library_ReadHeader( fsFile hFile, LIBHEADER* pLibHeader );
BOOLEAN SLF_Library_ReadEntry ( fsFile hFile, DIRENTRY* pDirEntry   );
BOOLEAN SLF_FindFilenameInDatabase( const CHAR8 *pFilename, INT32 *pLibIndex, INT32 *pFileIndex );
BOOLEAN SLF_IsLibraryOpened( INT32 iLibIndex );
BOOLEAN SLF_IsAlreadyOpened   ( INT32 iLibIndex, INT32 iFileIndex );
INT32	SLF_GetFreeOpenedSlot ( void );




BOOLEAN	SLF_Init( )
{
	UINT32		uiSize;

	//if all the libraries exist, set them up
	gSLF.sTotalNumberOfLibrarySlots = 0;
	gSLF.sUsedLibrarySlots          = 0;

	//allocate memory for the handles of the slf-files that will be open
	uiSize = INITIAL_NUM_HANDLES * sizeof( SLF_Handler );
	gSLF.pHandlers = (SLF_Handler *) MemAlloc( uiSize );
	CHECKF( gSLF.pHandlers );

	//clear the memory
	memset( gSLF.pHandlers, 0, uiSize);

	//set the initial number how many files can be opened at the one time
	gSLF.sTotalHandlers = INITIAL_NUM_HANDLES;
	gSLF.sUsedHandlers  = 0;

	return(TRUE);
}




void SLF_Shutdown( )
{
	UINT16 sLoop1;

	// Free up the memory used for each library
	for(sLoop1=0; sLoop1 < gSLF.sTotalNumberOfLibrarySlots; sLoop1++)
		SLF_FreeLibrary( &gSLF.pLibraries[sLoop1] );

	//Free up the memory used for all the library headers
	if( gSLF.pLibraries )
	{
		MemFree( gSLF.pLibraries );
		gSLF.pLibraries = NULL;
	}

	//loop through all the 'opened files' ( there should be no files open )
	if( gSLF.pHandlers )
	{
		MemFree( gSLF.pHandlers );
		gSLF.pHandlers = NULL;
	}
}

BOOLEAN		SLF_AddLibrary( const CHAR8 *pLibName )
{
	INT16	sCurrentLib = gSLF.sUsedLibrarySlots;
	INT32	i;
	BOOLEAN	result, rejectLib;
	SLF_Library	slfLib;
	SLF_Library	*slfPtr;
	
	memset( &slfLib, 0, sizeof(SLF_Library) );
	
	rejectLib = FALSE;
	result = SLF_AddLibraryInternal( pLibName, &slfLib );
	if ( !result )
	{
		rejectLib = TRUE;
	}
	else
	{
		// accept results
		// allocate memory for new library
		slfPtr = (SLF_Library*)MemRealloc( gSLF.pLibraries, (gSLF.sUsedLibrarySlots + 1) * sizeof(SLF_Library) );
		if ( !slfPtr )
		{
			rejectLib = TRUE;
		}
		else
		{
			// copy library to database
			gSLF.pLibraries = slfPtr;
			gSLF.pLibraries[sCurrentLib] = slfLib;
			gSLF.sUsedLibrarySlots++;
			gSLF.sTotalNumberOfLibrarySlots++;
			rejectLib = FALSE;
		}
	}
	
	if ( rejectLib )
	{
		// reject library, release resources
		// free all memory, allocated for current library
		for ( i = 0; i < slfLib.usNumberOfEntries; i++ )
		{
			MemFree( slfLib.pFileHeader[i].pFileName );
		}
		if ( slfLib.pFileHeader  ) MemFree( slfLib.pFileHeader  );
		if ( slfLib.sLibraryPath ) MemFree( slfLib.sLibraryPath );
	}
	return result;
}


slfHandler	SLF_OpenResource ( const CHAR8 *pFilename )
{
	// 1. determine, what library pFilename belongs, and what is his index in library's pFileHeader
	// 2. check, if it is already opened. if opened - reject the access.
	// 3. allocate slot
	// 4. make virtual open file by filling slot
	// 5. return slot's pointer as file handler

	INT32	iLibIndex, iFileIndex;
	INT32	newFileSlot;

	// 1. determine, what library pFilename belongs, and what is his index in library's pFileHeader
	if ( !SLF_FindFilenameInDatabase( pFilename, &iLibIndex, &iFileIndex ) )
		return -1;
	
	// 2. check, if it is already opened. if opened - reject the access.
	if ( SLF_IsAlreadyOpened( iLibIndex, iFileIndex ) )
		return -1;
	
	// 3. allocate slot
	newFileSlot = SLF_GetFreeOpenedSlot( );
	if ( newFileSlot == -1 )
		return -1;
	
	// 4. make virtual open file by filling slot
	gSLF.pHandlers[ newFileSlot ].sUsed       = TRUE;	
	gSLF.pHandlers[ newFileSlot ].iLibIndex   = iLibIndex;
	gSLF.pHandlers[ newFileSlot ].iFileIndex  = iFileIndex;
	gSLF.pHandlers[ newFileSlot ].iFileOffset = 0;
	gSLF.sUsedHandlers++;
	
	// 5. return slot's pointer as file handler
	return newFileSlot;
}


BOOLEAN		SLF_CloseResource( slfHandler hResource )
{
	if ( hResource < 0 || hResource >= gSLF.sTotalHandlers )
		return FALSE;
	
	gSLF.pHandlers[ hResource ].sUsed = FALSE;
	gSLF.sUsedHandlers--;
	return TRUE;
}


fsSize		SLF_Read ( slfHandler hResource, void *pBuffer, INT32 iSize )
{
	SLF_FileHeader	*fileHeader;
	SLF_Handler		*handler;
	fsFile			slf;
	fsSize			fileRealPosition;
	fsSize			bytesRead;

	if ( hResource < 0 || hResource >= gSLF.sTotalHandlers )
		return 0;

	handler    = &gSLF.pHandlers[hResource];
	fileHeader = &gSLF.pLibraries[ handler->iLibIndex ].pFileHeader[ handler->iFileIndex ];
	slf        =  gSLF.pLibraries[ handler->iLibIndex ].hLibraryHandle;

	// first, seek the file position inside slf library
	fileRealPosition = fileHeader->uiFileOffset + handler->iFileOffset;
	if ( !PHYSFS_seek( slf, fileRealPosition ) )
	{
		return 0;
	}

	// then read the contents and move file pointer
	bytesRead = PHYSFS_read( slf, pBuffer, 1, iSize );
	handler->iFileOffset += bytesRead;
	
	return bytesRead;
}


BOOLEAN		SLF_Seek ( slfHandler hResource, INT32 Position, INT32 iMethod )
{
	SLF_FileHeader	*fileHeader;
	SLF_Handler		*handler;
	INT32			newPosition;

	if ( hResource < 0 || hResource >= gSLF.sTotalHandlers )
		return FALSE;

	handler    = &gSLF.pHandlers[hResource];
	fileHeader = &gSLF.pLibraries[ handler->iLibIndex ].pFileHeader[ handler->iFileIndex ];

	switch( iMethod )
	{
	case FILE_SEEK_FROM_START:
		newPosition = Position;
		break;
	
	case FILE_SEEK_FROM_END:
		newPosition = fileHeader->uiFileLength - Position;
		break;
	
	case FILE_SEEK_FROM_CURRENT:
		newPosition = handler->iFileOffset + Position;
		break;
	
	default:
		return FALSE;
	}
	
	handler->iFileOffset = newPosition;
	return TRUE;
}


fsSize		SLF_Tell ( slfHandler hResource )
{
	SLF_Handler		*handler;

	if ( hResource < 0 || hResource >= gSLF.sTotalHandlers )
		return 0;

	handler    = &gSLF.pHandlers[hResource];

	return handler->iFileOffset;
}


BOOLEAN		SLF_IsEOF( slfHandler hResource )
{
	SLF_FileHeader	*fileHeader;
	SLF_Handler		*handler;

	if ( hResource < 0 || hResource >= gSLF.sTotalHandlers )
		return TRUE;

	handler    = &gSLF.pHandlers[hResource];
	fileHeader = &gSLF.pLibraries[ handler->iLibIndex ].pFileHeader[ handler->iFileIndex ];
	
	return handler->iFileOffset >= fileHeader->uiFileLength;
}


fsSize		SLF_Size_Handler  ( slfHandler hResource )
{
	SLF_FileHeader	*fileHeader;
	SLF_Handler		*handler;

	if ( hResource < 0 || hResource >= gSLF.sTotalHandlers )
		return 0;

	handler    = &gSLF.pHandlers[hResource];
	fileHeader = &gSLF.pLibraries[ handler->iLibIndex ].pFileHeader[ handler->iFileIndex ];

	return fileHeader->uiFileLength;
}


fsSize		SLF_Size_Filename ( const CHAR8 *pFilename )
{
	INT32	iLibIndex, iFileIndex;
	SLF_FileHeader	*fileHeader;

	if ( !SLF_FindFilenameInDatabase( pFilename, &iLibIndex, &iFileIndex ) )
		return 0;

	fileHeader = &gSLF.pLibraries[ iLibIndex ].pFileHeader[ iFileIndex ];

	return fileHeader->uiFileLength;
}


BOOLEAN SLF_AddLibraryInternal( const CHAR8 *pLibraryName, SLF_Library *pLibHeader )
{
	fsFile		hFile;
	DIRENTRY 	DirEntry;
	LIBHEADER	LibFileHeader;
	UINT32		uiCount;	

	// open slf-file
	hFile = PHYSFS_openRead(pLibraryName);
	if ( !hFile )
	{
		fprintf(stderr, "Error opening library %s. Reason: %s\n", pLibraryName, PHYSFS_getLastError() );
		return( FALSE );
	}
	
	// read slf header
	if ( !SLF_Library_ReadHeader( hFile, &LibFileHeader ) )
	{
		fprintf(stderr, "Error reading header of library %s. Reason: %s\n", pLibraryName, PHYSFS_getLastError() );
		PHYSFS_close( hFile );
		return( FALSE );
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
	LowercaseAndChangeSlash( pLibHeader->sLibraryPath );
	pLibHeader->usNumberOfEntries = LibFileHeader.iUsed;
	pLibHeader->pFileHeader       = (SLF_FileHeader*) MemAlloc( sizeof(SLF_FileHeader) * LibFileHeader.iUsed );
	memset( pLibHeader->pFileHeader, 0, sizeof(SLF_FileHeader) * LibFileHeader.iUsed);
	
//	if ( LibFileHeader.iEntries != LibFileHeader.iUsed )
//		printf("iEntries != iUsed (%d vs %d) for %s\n", LibFileHeader.iEntries, LibFileHeader.iUsed, pLibraryName);
	
//	printf("sPathToLibrary=%s\n", LibFileHeader.sPathToLibrary);
//	printf("LibName=%s\n", LibFileHeader.sLibName);
//	printf("PathToLibrary=%s\n", LibFileHeader.sPathToLibrary);
//	printf("Entries=%d\n", LibFileHeader.iEntries);
//	printf("Used=%d\n", LibFileHeader.iUsed);

	// seek the file list
	if ( !PHYSFS_seek( hFile, PHYSFS_fileLength( hFile ) - LibFileHeader.iEntries * sizeof(DIRENTRY) ) )
	{
		fprintf(stderr, "Error seeking in library %s. Reason: %s\n", pLibraryName, PHYSFS_getLastError() );
		PHYSFS_close( hFile );
		return(FALSE);
	}

	// read file list from slf
	uiCount = 0;
	while( !PHYSFS_eof( hFile ) )
	{
		if ( !SLF_Library_ReadEntry ( hFile, &DirEntry ) )
		{
			fprintf(stderr, "Error reading filelist of library %s. Reason: %s\n", pLibraryName, PHYSFS_getLastError() );
			PHYSFS_close( hFile );
			return(FALSE);
		}

		// if file isn't marked as deleted, accept it
		if( DirEntry.ubState == FILE_OK )
		{
			if ( uiCount >= pLibHeader->usNumberOfEntries )
			{
				fprintf(stderr, "Not enough file slots for library %s.\n", pLibraryName );
				PHYSFS_close( hFile );
				return(FALSE);
			}

			//allocate memory for the files name
			pLibHeader->pFileHeader[ uiCount ].pFileName = (STR) MemAlloc( strlen( DirEntry.sFileName ) + 1 );

			//if we couldnt allocate memory
			if( !pLibHeader->pFileHeader[ uiCount ].pFileName )
			{
				//report an error
				PHYSFS_close( hFile );
				return(FALSE);
			}

			//copy the file name, offset and length into the header
			strcpy( pLibHeader->pFileHeader[ uiCount ].pFileName, DirEntry.sFileName);
			LowercaseAndChangeSlash( pLibHeader->pFileHeader[ uiCount ].pFileName );
			pLibHeader->pFileHeader[ uiCount ].uiFileOffset = DirEntry.uiOffset;
			pLibHeader->pFileHeader[ uiCount ].uiFileLength = DirEntry.uiLength;
			uiCount++;

//			printf("sFileName=%s\n", DirEntry.sFileName);
//			printf("uiOffset=%d\n",  DirEntry.uiOffset);
//			printf("uiLength=%d\n",  DirEntry.uiLength);
		}
	}

	pLibHeader->hLibraryHandle       = hFile;
	pLibHeader->fLibraryOpen         = TRUE;

	return( TRUE );
}

BOOLEAN SLF_Library_ReadHeader( fsFile hFile, LIBHEADER* pLibHeader )
{
	if (  PHYSFS_read     ( hFile,  pLibHeader->sLibName, sizeof(CHAR8), FILENAME_SIZE ) != FILENAME_SIZE ) return FALSE;
	if (  PHYSFS_read     ( hFile,  pLibHeader->sPathToLibrary, sizeof(CHAR8), FILENAME_SIZE ) != FILENAME_SIZE ) return FALSE;
	if ( !PHYSFS_readSLE32( hFile, &pLibHeader->iEntries) )	return FALSE;
	if ( !PHYSFS_readSLE32( hFile, &pLibHeader->iUsed)    ) return FALSE;
	if ( !PHYSFS_readULE16( hFile, &pLibHeader->iSort)    ) return FALSE;
	if ( !PHYSFS_readULE16( hFile, &pLibHeader->iVersion) )	return FALSE;
	if (  PHYSFS_read     ( hFile, &pLibHeader->fContainsSubDirectories, sizeof(CHAR8), 1) != 1 )	return FALSE;
	if ( !PHYSFS_readSLE32( hFile, &pLibHeader->iReserved) ) return FALSE;

	return TRUE;
}

BOOLEAN SLF_Library_ReadEntry( fsFile hFile, DIRENTRY* pDirEntry )
{
	if (  PHYSFS_read     ( hFile,  pDirEntry->sFileName, sizeof(CHAR8), FILENAME_SIZE ) != FILENAME_SIZE ) return FALSE;
	if ( !PHYSFS_readULE32( hFile, &pDirEntry->uiOffset) ) return FALSE;
	if ( !PHYSFS_readULE32( hFile, &pDirEntry->uiLength) ) return FALSE;
	if (  PHYSFS_read     ( hFile, &pDirEntry->ubState,    sizeof(UINT8), 1 ) != 1  )	return FALSE;
	if (  PHYSFS_read     ( hFile, &pDirEntry->ubReserved, sizeof(UINT8), 15) != 15 )	return FALSE;

	return TRUE;
}

void SLF_FreeLibrary( SLF_Library *pLibHeader )
{
	UINT16	i;

	if ( pLibHeader->fLibraryOpen )
	{
		PHYSFS_close( pLibHeader->hLibraryHandle );

		// free all memory, allocated for library
		for ( i = 0; i < pLibHeader->usNumberOfEntries; i++ )
		{
			MemFree( pLibHeader->pFileHeader[i].pFileName );
		}
		if ( pLibHeader->pFileHeader  ) MemFree( pLibHeader->pFileHeader  );
		if ( pLibHeader->sLibraryPath ) MemFree( pLibHeader->sLibraryPath );
		memset(pLibHeader, 0, sizeof(SLF_Library));
	}
}

BOOLEAN SLF_FindFilenameInDatabase( const CHAR8 *pFilename, INT32 *pLibIndex, INT32 *pFileIndex )
{
	INT32	iLibIndex, iFileIndex;
	INT16	cnt;
	CHAR8	*pResourceName;
	
	//loop through all the libraries to check which library the file is in
	cnt = 0;
	iLibIndex = -1;
	while ( (cnt < gSLF.sTotalNumberOfLibrarySlots) && (iLibIndex == -1) )
	{
		//if the library is not loaded, dont try to access the array
		if( SLF_IsLibraryOpened( cnt ) )
		{
			if( strlen( gSLF.pLibraries[ cnt ].sLibraryPath ) == 0 )
			{
				//determine if there is a directory in the file name
				if( strchr( pFilename, '/' ) == NULL )
				{
					//There is no directory in the file name
					iLibIndex = cnt;
				}
			}
			else //compare the library name to the file name that is passed in
			{
				// if the directory paths are the same, to the length of the lib's path
				if( strncmp( gSLF.pLibraries[cnt].sLibraryPath, pFilename, strlen( gSLF.pLibraries[cnt].sLibraryPath ) ) == 0 )
				{
					iLibIndex = cnt;
				}
			}
		}
		cnt++;
	}
	
	if ( iLibIndex == -1 )
		return FALSE;	// we failed to find filename

	// now browse through library filelist, searching pFilename,
	// but first cutoff first directory name
	pResourceName = strchr( pFilename, '/' );
	if ( !pResourceName ) pResourceName = (CHAR8*)pFilename;
	else pResourceName++;

	cnt = 0;
	iFileIndex = -1;
	while ( (cnt < gSLF.pLibraries[iLibIndex].usNumberOfEntries) && (iFileIndex == -1) )
	{
		if ( strncmp( pResourceName, gSLF.pLibraries[iLibIndex].pFileHeader[cnt].pFileName, FILENAME_SIZE) == 0 )
		{
			iFileIndex = cnt;
		}
		cnt++;
	}

	if ( iFileIndex == -1 )
		return FALSE;	// we failed to find filename

	// we've found it!
	if ( pLibIndex  ) *pLibIndex  = iLibIndex;
	if ( pFileIndex ) *pFileIndex = iFileIndex;
	return TRUE;
}

BOOLEAN SLF_IsLibraryOpened( INT32 iLibIndex )
{
	return gSLF.pLibraries[iLibIndex].fLibraryOpen;
}

void SLF_Dump( const CHAR8 *filename )
{
	FILE	*file;
	INT32	i, j;
	
	file = fopen( filename, "w" );
	fprintf(file, "SLF database dump\n");	
	fprintf(file, "total slf slots = %d\n", gSLF.sTotalNumberOfLibrarySlots);	
	fprintf(file, "used slf slots  = %d\n", gSLF.sUsedLibrarySlots);
	fprintf(file, "\n");	
	
	for ( i = 0; i < gSLF.sTotalNumberOfLibrarySlots; i++ )
	{
		fprintf(file, "library %d info:\n", i);	
		fprintf(file, "library is opened = %d\n", gSLF.pLibraries[i].fLibraryOpen);
		fprintf(file, "library path      = %s\n", gSLF.pLibraries[i].sLibraryPath);
		fprintf(file, "number of files   = %d\n", gSLF.pLibraries[i].usNumberOfEntries);
		fprintf(file, "\ncontents:\n");	
		for ( j = 0; j < gSLF.pLibraries[i].usNumberOfEntries; j++ )
		{
			fprintf(file, "(%08X, %08X) = %s\n",
				gSLF.pLibraries[i].pFileHeader[j].uiFileOffset,
				gSLF.pLibraries[i].pFileHeader[j].uiFileLength,
				gSLF.pLibraries[i].pFileHeader[j].pFileName);
		}
		fprintf(file, "\n");	
	}
	fclose( file );
}

INT32	SLF_GetFreeOpenedSlot ( void )
{
	INT32	cnt = 0;
	INT32	foundIndex = -1;
	SLF_Handler		*pNewHandlers;
	
	// search first free slot
	while ( (cnt < gSLF.sTotalHandlers) && (foundIndex == -1) )
	{
		if ( !gSLF.pHandlers[cnt].sUsed )
			foundIndex = cnt;
		cnt++;
	}
	
	// return it if found
	if ( foundIndex != -1 )
		return foundIndex;

	printf("SLF: taking more memory for handlers\n");
	
	// else allocate memory for new handlers
	pNewHandlers = (SLF_Handler *) MemRealloc( gSLF.pHandlers, (gSLF.sTotalHandlers + NUM_FILES_TO_ADD_AT_A_TIME) * sizeof(SLF_Handler) );

	if( !pNewHandlers )
		return(-1);

	foundIndex = gSLF.sTotalHandlers;

	// init new handlers
	gSLF.pHandlers = pNewHandlers;
	for (cnt = 0; cnt < NUM_FILES_TO_ADD_AT_A_TIME; cnt++)
	{
		gSLF.pHandlers[ gSLF.sTotalHandlers + cnt ].sUsed = FALSE;
	}
	
	//increment the number of open files that we can have open
	gSLF.sTotalHandlers += NUM_FILES_TO_ADD_AT_A_TIME;
	
	return foundIndex;
}

BOOLEAN SLF_IsAlreadyOpened( INT32 iLibIndex, INT32 iFileIndex )
{
	INT32	cnt;
	
	for (cnt = 0; cnt < gSLF.sTotalHandlers; cnt++)
	{
		if ( gSLF.pHandlers[cnt].sUsed
			&& gSLF.pHandlers[cnt].iLibIndex  == iLibIndex
			&& gSLF.pHandlers[cnt].iFileIndex == iFileIndex)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOLEAN	SLF_IsExist( const CHAR8 *pFilename )
{
	return SLF_FindFilenameInDatabase( pFilename, NULL, NULL );
}

BOOLEAN SLF_IsValidHandle( slfHandler hResource )
{
	return hResource != -1;
}
