#include "SDL.h"
#include "sgp.h"
#include "Timer Control.h"
#include "Game Clock.h"
#include "ExceptionHandling.h"
#include "gameloop.h"

static BOOLEAN fRestore = FALSE;

static void GameQuit( void )
{
	SDL_Quit();
	exit(1);
}

int main(int argc, char *argv[])
{
	SDL_Event	event;

	printf("JA2 1.13 startup\n");

	// Copy commandline!
	giCLArgumentsCount = argc;
	gpCLArgument       = argv;
	ProcessJa2CommandLineBeforeInitialization();

	giStartMem = MemGetFree(  ) / 1024;

	if ( SDL_Init( 0 ) < 0 )
	{
		fprintf(stderr,	"Couldn't initialize SDL: %s\n", SDL_GetError());
		GameQuit();
		return FALSE;
	}

	SDL_EventState(SDL_QUIT, SDL_ENABLE);
	printf("SDL initialized\n");

//	__try
	{
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
		gfProgramIsRunning = TRUE;
		SDL_ShowCursor( SDL_DISABLE );

		FastDebugMsg("Running Game");

		// At this point the SGP is set up, which means all I/O, Memory, tools, etc... are available. All we need to do is 
		// attend to the gaming mechanics themselves
		while (gfProgramIsRunning)
		{
			fflush(stdout);
			fflush(stderr);

			if ( SDL_PollEvent( &event ) )
			{
				switch( event.type )
				{
				case SDL_QUIT:
					gfProgramIsRunning = FALSE;
					break;

				case SDL_KEYDOWN:
				case SDL_KEYUP:
					KeyboardHandler(&event);
					break;

				case SDL_MOUSEMOTION:
				case SDL_MOUSEBUTTONUP:
				case SDL_MOUSEBUTTONDOWN:
					MouseHandler(&event);
					break;

				case SDL_VIDEORESIZE:
					break;

				case SDL_VIDEOEXPOSE:
					// Redraw screen
					break;

				case SDL_ACTIVEEVENT:
					if ( event.active.state & SDL_APPINPUTFOCUS )
					{
						gfApplicationActive = event.active.gain;
					}

					if ( event.active.state & SDL_APPACTIVE )
					{
						if (event.active.gain)
						{
							// the application was restored (deiconified)
							if (fRestore == TRUE)
							{
								//RestoreVideoManager();
								//RestoreVideoSurfaces();	// Restore any video surfaces

								// unpause the JA2 Global clock
								if ( !gfPauseDueToPlayerGamePause )
								{
									PauseTime( FALSE );
								}
								gfApplicationActive = TRUE;
							}
						}
						else
						{
							//// application is iconified
							//// pause the JA2 Global clock
							PauseTime( TRUE );
							//SuspendVideoManager();
							gfApplicationActive = FALSE;
							fRestore = TRUE;
						}
					}

					if ( event.active.state & SDL_APPMOUSEFOCUS )
					{
						//if ( gfApplicationActive )
						//	SDL_ShowCursor( (event.active.gain) ? SDL_DISABLE : SDL_ENABLE );
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
					GameLoop();        

					// After this frame, reset input given flag
					//gfSGPInputReceived  =  FALSE;			
				}
			}
		}

		ShutdownStandardGamingPlatform();
		printf("SGP closed\n");
	}
//	__except( RecordExceptionInfo( GetExceptionInformation() ))
//	{
//	}
	SDL_Quit();
	printf("SDL closed\n");

	return(255);
}
