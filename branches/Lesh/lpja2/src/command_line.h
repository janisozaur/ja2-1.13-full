#ifndef _COMMAND_LINE_H_
#define _COMMAND_LINE_H_

#include "types.h"

enum ScreenMode
{
	SCR_NOT_SPECIFIED = 0,
	SCR_FULLSCREEN,
	SCR_WINDOWED
};

typedef struct
{
	BOOLEAN		fHelp;
	ScreenMode	eScreenMode;
	BOOLEAN		fNoSound;
	BOOLEAN		fDoMaps;
	BOOLEAN		fQuickSave;
	BOOLEAN		fEditorAuto;
	BOOLEAN		fEditor;
} COMMAND_LINE_PARAMS;

extern	COMMAND_LINE_PARAMS		gCmdLineParams;
extern	STRING512				argv0;			// needed for PhysicsFS

void	ProcessCommandLine( INT32 argc, CHAR8 *argv[] );
void	PrintCommandLineHelp( void );

#endif
