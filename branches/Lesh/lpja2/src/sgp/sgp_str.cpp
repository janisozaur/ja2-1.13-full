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
	iRet = vswprintf(pString, usMaxLen, pFormat, argptr);
	va_end(argptr);

	return( iRet );
}

INT32	WSTR_VSPrintf(CHAR16 *pString, INT16 usMaxLen, const CHAR16 *pFormat, va_list arg)
{
	return( vswprintf(pString, usMaxLen, pFormat, arg) );
}

INT32	UTF16toUTF32(UINT16 *pUTF16, CHAR16 *pUTF32, UINT16 usMaxLen)
{
	// let's be safe as possible
	UINT16	usCharsLeft = usMaxLen;
	UINT16	*utf16 = pUTF16;
	CHAR16	*utf32 = pUTF32;

	for (; usCharsLeft && (utf16 != 0); utf16++, utf32++, usCharsLeft--)
		*utf32 = *utf16;

	if ( usCharsLeft )
		*utf32 = 0;

	return usMaxLen - usCharsLeft;
}

// Simple char functions
// =====================

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
