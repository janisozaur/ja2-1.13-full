#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "build_defines.h"

// Include common headers
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <string.h>
#include <time.h>
#include <wchar.h>
#include <stdarg.h>

// Include linux-specific headers
#ifdef JA2_LINUX
#	include <unistd.h>
#	include <dirent.h>
#	include <errno.h>
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <pwd.h>
#	include <fcntl.h>
#	include <glob.h>
#	include <fnmatch.h>
#	define SLASH '/'
#	define __min(a, b)	( ((a) < (b)) ? (a) : (b) )
#	define __max(a, b)	( ((a) > (b)) ? (a) : (b) )
#	define min(a, b)	( ((a) < (b)) ? (a) : (b) )
#	define max(a, b)	( ((a) > (b)) ? (a) : (b) )
#	define _stricmp	strcasecmp
#	define _strnicmp strncasecmp
#	define _access access
#	define MAX_PATH 512
#endif

// Include windows-specific headers
#ifdef JA2_WIN
#	include <windows.h>
#	include <direct.h>
#	define SLASH '\\'
#endif

#endif
