#ifndef _BUILDDEFINES_H
#define _BUILDDEFINES_H

#include "Language Defines.h"

#pragma warning (disable : 4005)//disables macro redefinition, I dunno why they are getting redefined (it's just JA2BETAVERSION and JA2TESTVERSION so it's fine)
#pragma warning (disable : 4512)//disables assignment operator could not be generated
#pragma warning (disable : 4100)//disables unreferenced formal parameter
#pragma warning (disable : 4201)//disables nonstandard extension used : nameless struct/union
#pragma warning (disable : 4244)//disables conversion from type to type, possible loss of data
#pragma warning (disable : 4245)//disables conversion from type to type, signed/unsigned mismatch
#pragma warning (disable : 4127)//disables conditional expression is constant
#pragma warning (disable : 4996)//disables unsafe functions (sprintf etc)
#pragma warning (disable : 4505)//disables unreferenced local function has been removed
#pragma warning (disable : 4702)//disables unreachable code
//#pragma warning (disable : 4189)//disables local variable is initialized but not referenced

// Remove comment to build Beta version (with lot of debug infos) including the Editor
//#define JA2BETAVERSION

// Normal test version
//#define JA2TESTVERSION

//#define JA2EDITOR

//#define SGP_DEBUG

//#define PROFILER_ENABLED

//#define DEBUG_ATTACKBUSY

//#define DEBUGDECISIONS

//#define ROBOT_ALWAYS_READY

//#ifdef _DEBUG
//	#ifndef JA2TESTVERSION
//		#define JA2TESTVERSION
//	#endif
//#endif

// Do combinations
//#ifdef JA2TESTVERSION
//	#define JA2BETAVERSION
//	#define JA2EDITOR
//#endif



#ifdef JA2BETAVERSION
	#ifndef JA2EDITOR
		#define SGP_DEBUG
		#define	FORCE_ASSERTS_ON
		#define SGP_VIDEO_DEBUGGING
	#endif
#endif

//#define CRIPPLED_VERSION

// Huge speed and memory hog, but thorough -- will work with release builds.
// #define EXTREME_MEMORY_DEBUGGING 

// 0verhaul
// These squelch warnings in VC2K5 about printf's and string operations being unsafe.	Maybe we should work on building a better
// system that gives buffer sizes for copies (which would prevent future problems like a stack corruption issue I fixed a while back),
// but for now, just tell the compiler to shut up so I can see the real errors.
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS

#endif

