#ifndef _COMMAND_LINE_H_
#define _COMMAND_LINE_H_

#include "types.h"

typedef struct
{
	BOOLEAN		fHelp;
	BOOLEAN		fFullScreen;
	BOOLEAN		fNoSound;
	BOOLEAN		fDoMaps;
	BOOLEAN		fQuickSave;
	BOOLEAN		fEditorAuto;
	BOOLEAN		fEditor;
} COMMAND_LINE_PARAMS;

extern	COMMAND_LINE_PARAMS		gCmdLineParams;

void	ProcessCommandLine( INT32 argc, CHAR8 *argv[] );
void	PrintCommandLineHelp( void );

#endif
