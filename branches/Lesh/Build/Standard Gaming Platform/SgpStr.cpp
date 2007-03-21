// Lesh: string formatting functions. 21/03/2007 22:31:29 

#include "Platform.h"
#include "Types.h"

/********************************************************************************
 *
 * This functions are wrappers to string formatting functions.
 * They are written to archieve cross-platform compiling and game independency
 * from the platform it is being run from.
 *
 ********************************************************************************
 */

#ifdef JA2_LINUX

// -------------
// LINUX section
// -------------

INT32	WSTR_SPrintf(CHAR16 *pString, INT16 usMaxLen, const CHAR16 *pFormat, ...)
{
	INT32 iRet;
	va_list argptr;

	va_start(argptr, pFormat);
	iRet = vswprintf(pString, usMaxLen, pFormat, argptr);
	va_end(argptr);

	return( iRet );
}

INT32	STR_SPrintf(CHAR8 *pString, INT16 usMaxLen, const CHAR8 *pFormat, ...)
{
	INT32 iRet;
	va_list argptr;

	va_start(argptr, pFormat);
	iRet = vsnprintf(pString, usMaxLen, pFormat, argptr);
	va_end(argptr);

	return( iRet );
}

#elif defined( JA2_WIN )

// ---------------
// WINDOWS section
// ---------------

#endif
