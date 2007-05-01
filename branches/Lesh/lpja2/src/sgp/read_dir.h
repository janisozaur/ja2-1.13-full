//
// Snap: Declaration of the TReadDir class
// This class reads the contents of a directory file-by-file
//
#ifndef READDIR_H
#define READDIR_H

#include "platform.h"
#include "types.h"

class TReadDir {
public:

	TReadDir(char const* searchPattern);

	bool NextFile(char* fileName);
	void Close( void );

private:
#ifdef JA2_WIN
// ---------------------- Windows-specific stuff ---------------------------

	HANDLE fSearchHandle;
	WIN32_FIND_DATA fFileInfo;
	bool fFirstRequest;

// ------------------- End of Windows-specific stuff -----------------------
#elif defined(JA2_LINUX)
// ----------------------- Linux-specific stuff ----------------------------

	DIR				*dirHandler;
	struct dirent	*dirEntry;

// -------------------- End of Linux-specific stuff ------------------------
#endif

};


#endif // #ifndef READDIR_H
