//**************************************************************************
//
// Filename :	FileMan.h
//
//	Purpose :	prototypes for the file manager
//
// Modification history :
//
//		24sep96:HJH				- Creation
//
//**************************************************************************

#ifndef _FILEMAN_H
#define _FILEMAN_H

//**************************************************************************
//
//				Includes
//
//**************************************************************************

#include "types.h"

//**************************************************************************
//
//				Defines
//
//**************************************************************************

// Lesh: define this to test new revision of vfs
#define VFS2

#define MAX_FILENAME_LEN        48

#define FILE_ACCESS_READ	    0x01
#define FILE_ACCESS_WRITE	    0x02
#define FILE_ACCESS_READWRITE	0x03

#define FILE_CREATE_NEW			0x0010	// create new file. fail if exists
#define FILE_CREATE_ALWAYS		0x0020	// create new file. overwrite existing
#define FILE_OPEN_EXISTING		0x0040	// open a file. fail if doesn't exist
#define FILE_OPEN_ALWAYS		0x0080	// open a file, create if doesn't exist
#define FILE_TRUNCATE_EXISTING	0x0100	// open a file, truncate to size 0. fail if no exist

#define FILE_SEEK_FROM_START	0x01	// keep in sync with dbman.h
#define FILE_SEEK_FROM_END		0x02	// keep in sync with dbman.h
#define FILE_SEEK_FROM_CURRENT	0x04	// keep in sync with dbman.h

// Snap, Kaiden: This define duplicates a standard MFC define
// Added to resolve some intractable issue with MSVC6
#define INVALID_FILE_ATTRIBUTES ((UINT32)-1)

typedef UINT32				HANDLE;

//**************************************************************************
//
// Globals
//
//**************************************************************************


//**************************************************************************
//
//				Function Prototypes
//
//**************************************************************************
/*
#ifdef __cplusplus
extern "C" {
#endif
*/

extern BOOLEAN	InitializeFileManager(  STR strIndexFilename );

extern void		ShutdownFileManager( void );
extern void		FileDebug( BOOLEAN f );
extern BOOLEAN 	PathBackslash(STR path);

BOOLEAN	FileExists( STR strFilename );
extern BOOLEAN	FileExistsNoDB( STR strFilename );
extern BOOLEAN	FileDelete( const CHAR8 *strFilename );
extern HWFILE	FileOpen( STR strFilename, UINT32 uiOptions, BOOLEAN fDeleteOnClose );
extern void		FileClose( HWFILE );

extern BOOLEAN	FileRead( HWFILE hFile, PTR pDest, UINT32 iBytesToRead, UINT32 *piBytesRead );
extern BOOLEAN	FileWrite( HWFILE hFile, PTR pDest, UINT32 iBytesToWrite, UINT32 *piBytesWritten );

extern BOOLEAN  FilePrintf( HWFILE hFile, char * strFormatted, ... );

extern BOOLEAN	FileSeek( HWFILE, INT32 iDistance, UINT8 uiHow );
extern INT32	FileGetPos( HWFILE );

extern INT32	FileGetSize( HWFILE );
extern INT32	FileSize(STR strFilename);

#ifdef JA2_WIN
#	define BACKSLASH(x)
#elif defined(JA2_LINUX)
#	define BACKSLASH(x)		PathBackslash(x)

BOOLEAN PathBackslash(STR path);

#endif

BOOLEAN SetFileManCurrentDirectory( STR pcDirectory );
BOOLEAN GetFileManCurrentDirectory( STRING512 pcDirectory );
BOOLEAN GetExecutableDirectory( STRING512 pcDirectory );
BOOLEAN GetHomeDirectory( STRING512 pcDirectory );
BOOLEAN GetTempDirectory( STRING512 pcDirectory );
BOOLEAN GetWorkDirectory( STRING512 pcDirectory );

BOOLEAN DirectoryExists( STRING512 pcDirectory );	// not used in sources
BOOLEAN MakeFileManDirectory( STRING512 pcDirectory );

// WARNING: THESE DELETE ALL FILES IN THE DIRECTORY ( and all subdirectories if fRecursive is TRUE!! )
BOOLEAN RemoveFileManDirectory( STRING512 pcDirectory, BOOLEAN fRecursive);
BOOLEAN EraseDirectory( STRING512 pcDirectory);

typedef struct _GETFILESTRUCT_TAG {
	INT32	iFindHandle;
	CHAR8	zFileName[ 260 ];			// changed from UINT16, Alex Meduna, Mar-20'98
	BOOLEAN	fUseVFS;
//	UINT32 uiFileSize;
//	UINT32 uiFileAttribs;
} GETFILESTRUCT;

// File searching stuff
BOOLEAN GetFileFirst( CHAR8 * pSpec, GETFILESTRUCT *pGFStruct );
BOOLEAN GetFileNext( GETFILESTRUCT *pGFStruct );
void GetFileClose( GETFILESTRUCT *pGFStruct );

//returns true if at end of file, else false
BOOLEAN	FileCheckEndOfFile( HWFILE hFile );



BOOLEAN GetFileManFileTime( HWFILE hFile, SGP_FILETIME	*pCreationTime, SGP_FILETIME *pLastAccessedTime, SGP_FILETIME *pLastWriteTime );





// CompareSGPFileTimes() returns...
// -1 if the First file time is less than second file time. ( first file is older )
// 0 First file time is equal to second file time.
// +1 First file time is greater than second file time ( first file is newer ).
INT32	CompareSGPFileTimes( SGP_FILETIME	*pFirstFileTime, SGP_FILETIME *pSecondFileTime );

//	Pass in the Fileman file handle of an OPEN file and it will return..
//		if its a Real File, the return will be the handle of the REAL file
//		if its a LIBRARY file, the return will be the handle of the LIBRARY
HANDLE	GetRealFileHandleFromFileManFileHandle( HWFILE hFile );

/*
#ifdef __cplusplus
}
#endif
*/

#endif
