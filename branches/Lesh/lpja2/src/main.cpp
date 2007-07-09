#include "SDL.h"
#include "sgp.h"
#include "platform.h"
#include "game.h"


static BOOLEAN fRestore = FALSE;

int main(int argc, char *argv[])
{
	printf("JA2 1.13 startup\n");

	// Copy commandline!
	giCLArgumentsCount = argc;
	gpCLArgument       = argv;
	ProcessJa2CommandLineBeforeInitialization();

	giStartMem = MemGetFree(  ) / 1024;

	if ( !GameInit() )
	{
		GameQuit();
		return FALSE;
	}

	fflush(stdout);
	fflush(stderr);
	if ( !InitializeStandardGamingPlatform() )
	{
		fprintf(stderr, "Couldn't init SGP\n");
		GameQuit();
		return FALSE;
	}

	printf("SGP initialized\n");
	gfApplicationActive = TRUE;
	gfProgramIsRunning  = TRUE;

	// At this point the SGP is set up, which means all I/O, Memory, tools, etc... are available. All we need to do is 
	// attend to the gaming mechanics themselves
	GameCycle();
	ShutdownStandardGamingPlatform();
	printf("SGP closed\n");
	GameQuit();
	printf("JA2 1.13 exited\n");

	return(255);
}
