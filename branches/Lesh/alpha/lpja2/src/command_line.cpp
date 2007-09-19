#include "command_line.h"
#include "platform.h"

// command line parameters structure
COMMAND_LINE_PARAMS		gCmdLineParams;

//**********************************************************
//	ProcessCommandLine - parses the command line
//
//	[in]	argc - number of arguments
//	[in]	argv - string list of arguments
//
//**********************************************************
void	ProcessCommandLine( INT32 argc, CHAR8 *argv[] )
{
	INT32	cnt = 1;

	// first - write default values to cmd line params
	gCmdLineParams.fHelp		= FALSE;
	gCmdLineParams.eScreenMode	= SCR_NOT_SPECIFIED;
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
			gCmdLineParams.eScreenMode	= SCR_FULLSCREEN;

		if ( !strcmp( argv[cnt], "--windowed"  ) )
			gCmdLineParams.eScreenMode	= SCR_WINDOWED;

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

//**********************************************************
//	PrintCommandLineHelp - prints help for command line
//	arguments
//**********************************************************
void	PrintCommandLineHelp( void )
{
	printf("Jagged Alliance 2 v1.13 linux port\n");
	printf("\nCommand line switches:\n");
	printf("\t--help\t\thelp info (you are looking at it)\n");
	printf("\t--fullscreen\trun game at fullscreen\n");
	printf("\t--windowed\trun game in window\n");
	printf("\t--editor\trun editor (debug version only)\n");
	printf("\t--editor-auto\trun editor and load sector A9  (debug version only)\n");
	printf("\t--nosound\trun game without sound\n\n");
}
