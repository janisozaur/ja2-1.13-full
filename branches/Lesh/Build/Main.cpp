#include "SDL.h"
#include "sgp.h"
#include "Timer Control.h"
#include "Game Clock.h"

//static BOOLEAN fRestore = FALSE;

static void GameQuit( void )
{
	SDL_Quit();
	exit(1);
}

int main(int argc, char *argv[])
{
	SDL_Surface *pScreen;
	SDL_Event	event;

	// Copy commandline!
	giCLArgumentsCount = argc;
	gpCLArgument       = argv;
	ProcessJa2CommandLineBeforeInitialization();

	giStartMem = MemGetFree(  ) / 1024;

	if ( SDL_Init( 0 ) < 0 )
	{
		fprintf(stderr,	"Couldn't initialize SDL: %s\n", SDL_GetError());
		return FALSE;
	}

	SDL_WM_SetCaption("JA2 1.13", NULL);

	pScreen = SDL_SetVideoMode(800, 600, 32, SDL_SWSURFACE);
	if ( !pScreen ) {
		return FALSE;
	}

	if ( InitializeStandardGamingPlatform() )
	{
		fprintf(stderr, "Couldn't init SGP\n");
		return FALSE;
	}

	gfApplicationActive = TRUE;
	gfProgramIsRunning = TRUE;

	FastDebugMsg("Running Game");

	// At this point the SGP is set up, which means all I/O, Memory, tools, etc... are available. All we need to do is 
	// attend to the gaming mechanics themselves
	while (gfProgramIsRunning)
	{
		if ( SDL_PollEvent( &event ) )
		{
			switch( event.type )
			{
			case SDL_QUIT:
				gfProgramIsRunning = FALSE;
				//ShutdownStandardGamingPlatform();
				break;

			case SDL_VIDEORESIZE:
				break;

			case SDL_VIDEOEXPOSE:
				// Redraw screen
				break;

			case SDL_ACTIVEEVENT:
				switch(event.active.state)
				{
				case SDL_APPMOUSEFOCUS:
					break;

				case SDL_APPINPUTFOCUS:
					break;

				case SDL_APPACTIVE:
					if (event.active.gain)
					{
						// the application was restored (deiconified)
						//if (fRestore == TRUE)
						//{
							RestoreVideoManager();
							RestoreVideoSurfaces();	// Restore any video surfaces

							// unpause the JA2 Global clock
							if ( !gfPauseDueToPlayerGamePause )
							{
								PauseTime( FALSE );
							}
							gfApplicationActive = TRUE;
						//}
					}
					else
					{
						// application is iconified
						// pause the JA2 Global clock
						PauseTime( TRUE );
						SuspendVideoManager();
						gfApplicationActive = FALSE;
						//fRestore = TRUE;
					}
					break;
				}
				break;

			default:
				break;
			}
		}
		else
		{ // Windows hasn't processed any messages, therefore we handle the rest
			if (gfApplicationActive == FALSE)
			{ // Well we got nothing to do but to wait for a message to activate
				SDL_WaitEvent(NULL);
			} 
			else
			{ // Well, the game is active, so we handle the game stuff        
				//GameLoop();        

				// After this frame, reset input given flag
				//gfSGPInputReceived  =  FALSE;			
			}
		}
	}

	ShutdownStandardGamingPlatform();
	SDL_Quit();

	return(255);
}