#ifndef _SGP_STR_H
#define _SGP_STR_H

#include "types.h"
#include <stdarg.h>

#define STRLEN(x)	( sizeof(x) / sizeof(CHAR8)  )
#define WSTRLEN(x)	( sizeof(x) / sizeof(CHAR16) )

INT32	WSTR_SPrintf (CHAR16 *pString, INT16 usMaxLen, const CHAR16 *pFormat, ...);
INT32	WSTR_VSPrintf(CHAR16 *pString, INT16 usMaxLen, const CHAR16 *pFormat, va_list arg);

// Since windows has utf-16 encoding only ( sizeof(wchar_t)=2 ) and
// ja2 was developed with it, so some binary resources were saved
// in utf-16. It is merc profiles, an *.edt files.
// But in linux wchar_t is different! It is 32-bit, i.e. sizeof(wchar_t)=4
// This invalidates all current utf-16 resources and I had to convert
// strings on-the-fly, while reading specified above resource, in order
// to make game work.
INT32	UTF16toUTF32(UINT16 *pUTF16, CHAR16 *pUTF32, UINT16 usMaxLen);

// Use converter only in linux version
#ifdef JA2_LINUX
#	define ConvertUTF16to32(x,y,z)	UTF16toUTF32(x,y,z)
#elif defined( JA2_WIN )
#	define ConvertUTF16to32(x,y,z)	memcpy(y,x,z)
#endif

INT32	STR_SPrintf(CHAR8 *pString, INT16 usMaxLen, const CHAR8 *pFormat, ...);

#endif
