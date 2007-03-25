#ifndef _SGP_STR_H
#define _SGP_STR_H

#include "Types.h"
#include <stdarg.h>

INT32	WSTR_SPrintf (CHAR16 *pString, INT16 usMaxLen, const CHAR16 *pFormat, ...);
INT32	WSTR_VSPrintf(CHAR16 *pString, INT16 usMaxLen, const CHAR16 *pFormat, va_list arg);

INT32	STR_SPrintf(CHAR8 *pString, INT16 usMaxLen, const CHAR8 *pFormat, ...);

#endif
