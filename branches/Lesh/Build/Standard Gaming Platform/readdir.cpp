//
// Snap: Implementation of the TReadDir class
// This class reads the contents of a directory file-by-file
//
#include "readdir.h"

TReadDir::TReadDir(char const* searchPattern) 
{
#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------
	fSearchHandle = FindFirstFile(searchPattern, &fFileInfo);
	fFirstRequest = true;
// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	dirHandler = opendir(searchPattern);
	if ( !dirHandler )
	{
		printf("Failed to open dir for listing: %s\n", searchPattern);
	}

// -------------------- End of Linux-specific stuff ------------------------
#endif
}


bool TReadDir::NextFile(char* fileName, bool &isDir)
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

	struct stat dir_stat;

	if ( !dirHandler )
		return false;

	dirEntry = readdir(dirHandler);
	if ( !dirEntry )
		return false;

    if (stat(dirEntry->d_name, &dir_stat) == -1)
		return false;

	strncpy( fileName, dirEntry->d_name, SGPFILENAME_LEN );
	isDir = S_ISDIR(dir_stat.st_mode);

// -------------------- End of Linux-specific stuff ------------------------
#endif
	return true;
}

TReadDir::~TReadDir()
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
