//
// Snap: Implementation of the TReadDir class
// This class reads the contents of a directory file-by-file
//
#include "read_dir.h"
#include "file_man.h"

TReadDir::TReadDir(char const* searchPattern) 
{
	char zPath[512];

	strncpy(zPath, searchPattern, 512);
	BACKSLASH(zPath);

#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------
	fSearchHandle = FindFirstFile(zPath, &fFileInfo);
	fFirstRequest = true;
// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	dirHandler = opendir(zPath);
	if ( !dirHandler )
	{
		printf("Failed to open dir for listing: %s\n", zPath);
	}

// -------------------- End of Linux-specific stuff ------------------------
#endif
}


bool TReadDir::NextFile(char* fileName)
{
#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------
	if (fSearchHandle == INVALID_HANDLE_VALUE)
		return false;
	
	if (fFirstRequest)
		fFirstRequest = false;
	else
		if ( !FindNextFile(fSearchHandle, &fFileInfo) )
			return false;

	strncpy( fileName, fFileInfo.cFileName, SGPFILENAME_LEN );
	isDir = fFileInfo.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY;
// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	if ( !dirHandler )
		return false;

	dirEntry = readdir(dirHandler);
	if ( !dirEntry )
		return false;

	strncpy( fileName, dirEntry->d_name, SGPFILENAME_LEN );

// -------------------- End of Linux-specific stuff ------------------------
#endif
	return true;
}

void TReadDir::Close( void )
{
#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	FindClose(fSearchHandle);

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	closedir(dirHandler);

// -------------------- End of Linux-specific stuff ------------------------
#endif
}
