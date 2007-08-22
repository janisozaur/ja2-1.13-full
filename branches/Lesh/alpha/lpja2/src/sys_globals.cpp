#ifdef PRECOMPILEDHEADERS
	#include "ja2_all.h"
#else
	#include "types.h"
	#include "screen_ids.h"
	#include "sys_globals.h"
	#include "game_loop.h"
	#include "platform.h"
	
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


