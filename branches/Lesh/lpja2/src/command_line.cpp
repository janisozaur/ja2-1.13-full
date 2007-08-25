#include "command_line.h"
#include "platform.h"

// command line parameters structure
COMMAND_LINE_PARAMS		gCmdLineParams;

void	ProcessCommandLine( INT32 argc, CHAR8 *argv[] )
{
	INT32	cnt = 1;

	// first - write default values to cmd line params
	gCmdLineParams.fHelp		= FALSE;
	gCmdLineParams.fFullScreen	= FALSE;
	gCmdLineParams.fNoSound	= FALSE;
	gCmdLineParams.fDoMaps		= FALSE;
	gCmdLineParams.fQuickSave	= FALSE;
	gCmdLineParams.fEditorAuto	= FALSE;
	gCmdLineParams.fEditor		= FALSE;

	// second - handle input
	while( cnt < argc )													
	{
		if ( !strcmp( argv[cnt], "--help"        ) )
			gCmdLineParams.fHelp		= TRUE;

		if ( !strcmp( argv[cnt], "--fullscreen"  ) )
			gCmdLineParams.fFullScreen	= TRUE;

		if ( !strcmp( argv[cnt], "--nosound"     ) )
			gCmdLineParams.fNoSound	= TRUE;

		if ( !strcmp( argv[cnt], "--domaps"      ) )
			gCmdLineParams.fDoMaps		= TRUE;

		if ( !strcmp( argv[cnt], "--quicksave"   ) )
			gCmdLineParams.fQuickSave	= TRUE;

		if ( !strcmp( argv[cnt], "--editor-auto" ) )
			gCmdLineParams.fEditorAuto	= TRUE;

		if ( !strcmp( argv[cnt], "--editor"      ) )
			gCmdLineParams.fEditor		= TRUE;

		cnt++;
	}
}

void	PrintCommandLineHelp( void )
{
	printf("Jagged Alliance 2 v1.13 linux port\n");
	printf("\nCommand line switches:\n");
	printf("\t--help\t\thelp info (you are looking at it)\n");
	printf("\t--fullscreen\trun game at fullscreen\n");
	printf("\t--nosound\trun game without sound\n\n");
}
