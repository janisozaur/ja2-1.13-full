#include "game.h"
#include "SDL.h"
#include "game_loop.h"
#include "platform.h"
#include "sgp.h"
#include "game_clock.h"
#include "timer_control.h"

enum
{
	EVENT_GAME_UPDATE = SDL_USEREVENT
};

void GameQuit( void )
{
	SDL_Quit();
//	printf("SDL closed\n");
}

BOOLEAN	GameInit( void )
{
	if ( SDL_Init( 0 ) < 0 )
	{
		fprintf(stderr,	"Couldn't initialize SDL: %s\n", SDL_GetError());
		return FALSE;
	}

	SDL_EventState( SDL_QUIT, SDL_ENABLE );
	printf("SDL initialized\n");
	return TRUE;
}

void	GameCycle( void )
{
	SDL_Event	event;

	while (gfProgramIsRunning)
	{
		fflush(stdout);
		fflush(stderr);

//		if ( SDL_WaitEvent( &event ) )
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
//				GameNeedUpdate();
				break;

			case SDL_MOUSEMOTION:
			case SDL_MOUSEBUTTONUP:
			case SDL_MOUSEBUTTONDOWN:
				MouseHandler(&event);
//				GameNeedUpdate();
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
//					if ( event.active.gain )
//						GameNeedUpdate();
				}

				if ( event.active.state & SDL_APPACTIVE )
				{
					if (event.active.gain)
					{
						// unpause the JA2 Global clock
						if ( !gfPauseDueToPlayerGamePause )
						{
							PauseTime( FALSE );
						}
						gfApplicationActive = TRUE;
//						GameNeedUpdate();
					}
					else
					{
						//// application is iconified
						//// pause the JA2 Global clock
						PauseTime( TRUE );
						gfApplicationActive = FALSE;
					}
				}

				if ( event.active.state & SDL_APPMOUSEFOCUS )
				{
//					if ( gfApplicationActive )
//						GameNeedUpdate();
					//	SDL_ShowCursor( (event.active.gain) ? SDL_DISABLE : SDL_ENABLE );
				}
				break;

//			case EVENT_GAME_UPDATE:
//				GameLoop();        
//				break;

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
			}
		}
	}
}

void GameNeedUpdate( void )
{
	SDL_Event	EventToPush;

	EventToPush.type = EVENT_GAME_UPDATE;
	SDL_PushEvent( &EventToPush );
}
