// Lesh: string formatting functions. 21/03/2007 22:31:29 

#include "platform.h"
#include "types.h"

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

// Wide string (UNICODE) functions
// ===============================

INT32	WSTR_SPrintf(CHAR16 *pString, INT16 usMaxLen, const CHAR16 *pFormat, ...)
{
	INT32 iRet;
	va_list argptr;

	va_start(argptr, pFormat);
	iRet = vswprintf(pString, usMaxLen - 1, pFormat, argptr);
	va_end(argptr);

	return( iRet );
}

INT32	WSTR_VSPrintf(CHAR16 *pString, INT16 usMaxLen, const CHAR16 *pFormat, va_list arg)
{
	return( vswprintf(pString, usMaxLen - 1, pFormat, arg) );
}

// Simple char functions
// =====================

INT32	STR_SPrintf(CHAR8 *pString, INT16 usMaxLen, const CHAR8 *pFormat, ...)
{
	INT32 iRet;
	va_list argptr;

	va_start(argptr, pFormat);
	iRet = vsnprintf(pString, usMaxLen - 1, pFormat, argptr);
	va_end(argptr);

	return( iRet );
}

#elif defined( JA2_WIN )

// ---------------
// WINDOWS section
// ---------------

#endif
