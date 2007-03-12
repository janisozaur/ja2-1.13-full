#ifndef _BUILDDEFINES_H
#define _BUILDDEFINES_H

#ifdef __linux__
	#define JA2_LINUX
#elif __WIN32__
	#define JA2_WIN
#else
	#error "Unsupported operating system!"
#endif

#include "Language Defines.h"

// Remove comment to build Beta version & Editor
//#define	JA2BETAVERSION

// Normal test version
#define JA2TESTVERSION

// Remove comment to build the editor!
//#define JA2EDITOR

//#ifdef _DEBUG
//	#ifndef JA2TESTVERSION
//		#define JA2TESTVERSION
//	#endif
//#endif

// Do combinations
#ifdef JA2TESTVERSION
	#define JA2BETAVERSION
	#define JA2EDITOR
#endif



#ifdef JA2BETAVERSION
	#ifndef JA2EDITOR
		#define SGP_DEBUG
		#define	FORCE_ASSERTS_ON
		#define SGP_VIDEO_DEBUGGING
	#endif
#endif

//#define CRIPPLED_VERSION

// Huge speed and memory hog, but thorough -- will work with release builds.
 //#define EXTREME_MEMORY_DEBUGGING 

#endif
