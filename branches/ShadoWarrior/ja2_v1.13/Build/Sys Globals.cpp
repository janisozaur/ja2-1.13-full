#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
#else
	#include <stdarg.h>
	#include <stdio.h>
	#include <wchar.h>
	#include "types.h"
	#include "screenids.h"
	#include "Sys Globals.h"
	#include "gameloop.h"
#endif


// External globals
extern UINT8	gubFilename[ 200 ]; // symbol already declared globally in editscreen.cpp (jonathanl)
BOOLEAN			gfEditMode = FALSE;
extern INT8		gDebugStr[128]; // symbol already defined in overhead.cpp (jonathanl)
INT8			gSystemDebugStr[128];
INT8			gbFPSDisplay = SHOW_MIN_FPS;
BOOLEAN			gfResetInputCheck = FALSE;
BOOLEAN			gfGlobalError			= FALSE;

UINT32	guiGameCycleCounter = 0;
UINT8	gubErrorText[ 200 ];

BOOLEAN  SET_ERROR( const char *String, ...)
{
  va_list  ArgPtr;

  va_start(ArgPtr, String);
  vsprintf( (char *) gubErrorText, String, ArgPtr);
  va_end(ArgPtr);

	SetPendingNewScreen( ERROR_SCREEN );

	gfGlobalError = TRUE; 
	
	return( FALSE );	
}


