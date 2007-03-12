#ifndef _PLATFORM_H_
#define _PLATFORM_H_

#include "builddefines.h"

// Include common headers
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <string.h>
#include <time.h>

// Include linux-specific headers
#ifdef JA2_LINUX
#	include <unistd.h>
#	include <dirent.h>
#	include <errno.h>
#	include <sys/types.h>
#	include <sys/stat.h>
#	include <pwd.h>
#	include <stdarg.h>
#	include <fcntl.h>
#	define SLASH '/'
#endif

// Include windows-specific headers
#ifdef JA2_WIN
#	include <windows.h>
#	include <direct.h>
#	define SLASH '\\'
#endif

#endif
