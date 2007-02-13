/* $Id: sgp.c,v 1.4 2004/03/19 06:16:04 digicrab Exp $ */
//its test what doeas it do?
#ifdef JA2_PRECOMPILED_HEADERS
	#include "JA2 SGP ALL.H"
	#include "JA2 Splash.h"
	#include "utilities.h"
	#include "SDL.h"
#else
	#include "SDL.h"
	#include "types.h"
	#include <windows.h>
	#include <windowsx.h>
	#include <stdio.h>
	#include <stdarg.h>
	#include <string.h>
	#include "sgp.h"
	#include "RegInst.h"
	#include "vobject.h"
	#include "font.h"
	#include "local.h"
	#include "Fileman.h"
	#include "input.h"
	#include "Random.h"
	#include "gameloop.h"
	#include "soundman.h"
	#include "JA2 Splash.h"
	#include "Timer Control.h"
	#if !defined( JA2 ) && !defined( UTIL )
		#include "GameData.h"               // for MoveTimer() [Wizardry specific]
	#endif
#endif

	#include "input.h"
	#include "zmouse.h"


#include "ExceptionHandling.h"

#include "dbt.h"
#include "INIReader.h"
#include "BuildDefines.h"
#include "Intro.h"

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif


extern UINT32	MemDebugCounter;


extern	BOOLEAN	CheckIfGameCdromIsInCDromDrive();
extern  void    QueueEvent(UINT16 ubInputEvent, UINT32 usParam, UINT32 uiParam);

// Prototype Declarations

//INT32 FAR PASCAL WindowProcedure(HWND hWindow, UINT16 Message, WPARAM wParam, LPARAM lParam);
BOOLEAN          InitializeStandardGamingPlatform(HINSTANCE hInstance, int sCommandShow);
void             ShutdownStandardGamingPlatform(void);
void			 GetRuntimeSettings( );

//int PASCAL HandledWinMain(HINSTANCE hInstance,  HINSTANCE hPrevInstance, LPSTR pCommandLine, int sCommandShow);


#if !defined(JA2) && !defined(UTILS)
void	ProcessCommandLine(CHAR8 *pCommandLine);
BOOLEAN	RunSetup(void);

// Should the game immediately load the quick save at startup?
BOOLEAN						gfLoadAtStartup=FALSE;
BOOLEAN						gfUsingBoundsChecker=FALSE;
CHAR8						*gzStringDataOverride=NULL;
BOOLEAN						gfCapturingVideo = FALSE;

#endif

HINSTANCE					ghInstance;


// Global Variable Declarations
#ifdef WINDOWED_MODE
RECT	rcWindow;
#endif

// moved from header file: 24mar98:HJH
UINT32		giStartMem;

// GLOBAL RUN-TIME SETTINGS
UINT32		guiMouseWheelMsg;			// For mouse wheel messages

BOOLEAN gfApplicationActive;
BOOLEAN gfProgramIsRunning;
BOOLEAN gfGameInitialized = FALSE;
//UINT32	giStartMem; // redefintion... look up a few lines (jonathanl)
BOOLEAN	gfDontUseDDBlits	= FALSE;

// There were TWO of them??!?! -- DB
CHAR8		gzCommandLine[100];		// Command line given
INT32		giCLArgumentsCount = 0;	// Count of command line arguments
CHAR8		**gpCLArgument = NULL;	// Array of pointers to command line arguments

CHAR8		gzErrorMsg[2048]="";
BOOLEAN	gfIgnoreMessages=FALSE;

// GLOBAL VARIBLE, SET TO DEFAULT BUT CAN BE CHANGED BY THE GAME IF INIT FILE READ
UINT8		gbPixelDepth = PIXEL_DEPTH;

//INT32 FAR PASCAL WindowProcedure(HWND hWindow, UINT16 Message, WPARAM wParam, LPARAM lParam)
//{ 
//	static BOOLEAN fRestore = FALSE;
//
//  if(gfIgnoreMessages)
//		return(DefWindowProc(hWindow, Message, wParam, lParam));
//
//	// ATE: This is for older win95 or NT 3.51 to get MOUSE_WHEEL Messages
//	if ( Message == guiMouseWheelMsg )
//	{
//      QueueEvent(MOUSE_WHEEL, wParam, lParam);
//			return( 0L );
//	}
// 
//	switch(Message)
//  {
//		case WM_MOUSEWHEEL:
//			{
//				QueueEvent(MOUSE_WHEEL, wParam, lParam);
//				break;
//			}
//		
//#ifdef JA2
//#ifdef WINDOWED_MODE
//    case WM_MOVE:
//
//        GetClientRect(hWindow, &rcWindow);
//        ClientToScreen(hWindow, (LPPOINT)&rcWindow);
//        ClientToScreen(hWindow, (LPPOINT)&rcWindow+1);
//        break;
//#endif
//#else
//		case WM_MOUSEMOVE:
//			break;
//
//		case WM_SIZING:
//		{
//			LPRECT	lpWindow;
//			INT32		iWidth, iHeight, iX, iY;
//			BOOLEAN fWidthByHeight=FALSE, fHoldRight=FALSE;
//
//			lpWindow = (LPRECT) lParam;  
//
//			iWidth=lpWindow->right-lpWindow->left;
//			iHeight=lpWindow->bottom-lpWindow->top;
//			iX = (lpWindow->left + lpWindow->right)/2;
//			iY = (lpWindow->top + lpWindow->bottom)/2;
//
//			switch(wParam)
//			{
//				case WMSZ_BOTTOMLEFT:
//					fHoldRight=TRUE;
//				case WMSZ_BOTTOM:
//				case WMSZ_BOTTOMRIGHT:
//					if(iHeight < SCREEN_HEIGHT)
//					{
//						lpWindow->bottom=lpWindow->top+SCREEN_HEIGHT;
//						iHeight=SCREEN_HEIGHT;
//					}
//					fWidthByHeight=TRUE;
//				break;
//
//				case WMSZ_TOPLEFT:
//					fHoldRight=TRUE;
//				case WMSZ_TOP:
//				case WMSZ_TOPRIGHT:
//					if(iHeight < SCREEN_HEIGHT)
//					{
//						lpWindow->top=lpWindow->bottom-SCREEN_HEIGHT;
//						iHeight=SCREEN_HEIGHT;
//					}
//					fWidthByHeight=TRUE;
//					break;
//
//				case WMSZ_LEFT:
//					if(iWidth < SCREEN_WIDTH)
//					{
//						lpWindow->left=lpWindow->right-SCREEN_WIDTH;
//						iWidth = SCREEN_WIDTH;
//					}
//					break;
//
//				case WMSZ_RIGHT:
//					if(iWidth < SCREEN_WIDTH)
//					{
//						lpWindow->right=lpWindow->left+SCREEN_WIDTH;
//						iWidth = SCREEN_WIDTH;
//					}
//			}
//
//			// Calculate width as a factor of height
//			if(fWidthByHeight)
//			{
//				iWidth = iHeight * SCREEN_WIDTH / SCREEN_HEIGHT;
////				lpWindow->left = iX - iWidth/2;
////				lpWindow->right = iX + iWidth / 2;
//				if(fHoldRight)
//					lpWindow->left = lpWindow->right - iWidth;
//				else
//					lpWindow->right = lpWindow->left + iWidth;
//			}
//			else // Calculate height as a factor of width
//			{
//				iHeight = iWidth * SCREEN_HEIGHT / SCREEN_WIDTH;
////				lpWindow->top = iY - iHeight/2;
////				lpWindow->bottom = iY + iHeight/2;
//				lpWindow->bottom = lpWindow->top + iHeight;
//			}
//	
//
///*
//			switch(wParam)
//			{
//				case WMSZ_BOTTOM:
//				case WMSZ_BOTTOMLEFT:
//				case WMSZ_BOTTOMRIGHT:
//					if(iHeight < SCREEN_HEIGHT)
//						lpWindow->bottom=lpWindow->top+SCREEN_HEIGHT;
//			}
//
//			switch(wParam)
//			{
//				case WMSZ_TOP:
//				case WMSZ_TOPLEFT:
//				case WMSZ_TOPRIGHT:
//					if(iHeight < SCREEN_HEIGHT)
//						lpWindow->top=lpWindow->bottom-SCREEN_HEIGHT;
//			}
//
//			switch(wParam)
//			{
//				case WMSZ_BOTTOMLEFT:
//				case WMSZ_LEFT:
//				case WMSZ_TOPLEFT:
//					if(iWidth < SCREEN_WIDTH)
//						lpWindow->left=lpWindow->right-SCREEN_WIDTH;
//			}
//
//			switch(wParam)
//			{
//				case WMSZ_BOTTOMRIGHT:
//				case WMSZ_RIGHT:
//				case WMSZ_TOPRIGHT:
//					if(iWidth < SCREEN_WIDTH)
//						lpWindow->right=lpWindow->left+SCREEN_WIDTH;
//			}
//*/
//		}
//		break;
//
//    case WM_SIZE:
//		{
//			UINT16 nWidth = LOWORD(lParam);  // width of client area 
//			UINT16 nHeight = HIWORD(lParam); // height of client area 
//
//			if(nWidth && nHeight)
//			{
//				switch(wParam)
//				{
//					case SIZE_MAXIMIZED:
//						VideoFullScreen(TRUE);
//						break;
//
//					case SIZE_RESTORED:
//						VideoResizeWindow();
//						break;
//				}
//			}
//		}
//		break;
//
//    case WM_MOVE:
//		{
//			INT32 xPos = (INT32)LOWORD(lParam);    // horizontal position 
//			INT32 yPos = (INT32)HIWORD(lParam);    // vertical position 		
//		}
//		break;
//#endif
//
//	case WM_ACTIVATEAPP: 
//		switch(wParam)
//		{
//			case TRUE: // We are restarting DirectDraw
//				if (fRestore == TRUE)
//				{
//					RestoreVideoManager();
//					RestoreVideoSurfaces();	// Restore any video surfaces
//
//					// unpause the JA2 Global clock
//					if ( !gfPauseDueToPlayerGamePause )
//					{
//						PauseTime( FALSE );
//					}
//					gfApplicationActive = TRUE;
//				}
//				break;
//			case FALSE: // We are suspending direct draw
//#ifdef JA2
//						// pause the JA2 Global clock
//						PauseTime( TRUE );
//						SuspendVideoManager();
//#else
//#ifndef UTIL 
//						if(!VideoInspectorIsEnabled())
//							SuspendVideoManager();
//#endif
//#endif
//          // suspend movement timer, to prevent timer crash if delay becomes long
//          // * it doesn't matter whether the 3-D engine is actually running or not, or if it's even been initialized
//          // * restore is automatic, no need to do anything on reactivation
//#if !defined( JA2 ) && !defined( UTIL )
//          MoveTimer(TIMER_SUSPEND);
//#endif
//
//          gfApplicationActive = FALSE;
//          fRestore = TRUE;
//          break;
//      }
//      break;
//
//    case WM_CREATE: 
//			break;
//
//    case WM_DESTROY: 
//			ShutdownStandardGamingPlatform();
//      ShowCursor(TRUE);
//      PostQuitMessage(0);
//      break;
//
//		case WM_SETFOCUS:
//#if !defined( JA2 ) && !defined( UTIL )
//			if(!VideoInspectorIsEnabled())
//				RestoreVideoManager();
//			gfApplicationActive=TRUE;
////			RestrictMouseToXYXY(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
//#else
//      RestoreCursorClipRect( );
//#endif
//
//			break;
//
//		case WM_KILLFOCUS:
//#if !defined( JA2 ) && !defined( UTIL )
//			if(!VideoInspectorIsEnabled())
//				SuspendVideoManager();
//
//			gfApplicationActive=FALSE;
//			FreeMouseCursor();
//#endif
//			// Set a flag to restore surfaces once a WM_ACTIVEATEAPP is received
//			fRestore = TRUE;
//			break;
//
//    default
//    : return DefWindowProc(hWindow, Message, wParam, lParam);
//  }
//  return 0L;
//}
//
//

BOOLEAN InitializeStandardGamingPlatform(void)
{
	FontTranslationTable *pFontTable;
	//SDL_Surface *screen = SDL_SetVideoMode(800, 600, 16, SDL_SWSURFACE);

	// now required by all (even JA2) in order to call ShutdownSGP
	atexit(SGPExit);

	GetRuntimeSettings( );

	// Initialize the Debug Manager - success doesn't matter
	InitializeDebugManager();

	// Now start up everything else.
	RegisterDebugTopic(TOPIC_SGP, "Standard Gaming Platform");

	// this one needs to go ahead of all others (except Debug), for MemDebugCounter to work right...
	FastDebugMsg("Initializing Memory Manager");
	// Initialize the Memory Manager
	if (InitializeMemoryManager() == FALSE)
	{ // We were unable to initialize the memory manager
		fprintf(stderr, "Couldn't init memory manager\n");
		FastDebugMsg("FAILED : Initializing Memory Manager");
		return FALSE;
	}

	// removed cause of useless
	//FastDebugMsg("Initializing Mutex Manager");
	//// Initialize the Dirty Rectangle Manager
	//if (InitializeMutexManager() == FALSE)
	//{ // We were unable to initialize the game
	//	fprintf(stderr, "Couldn't init mutex manager\n");
	//	FastDebugMsg("FAILED : Initializing Mutex Manager");
	//	return FALSE;
	//}

	FastDebugMsg("Initializing File Manager");
	// Initialize the File Manager
	if (InitializeFileManager(NULL) == FALSE)
	{ // We were unable to initialize the file manager
		fprintf(stderr, "Couldn't init file manager\n");
		FastDebugMsg("FAILED : Initializing File Manager");
		return FALSE;
	}

	FastDebugMsg("Initializing Containers Manager");
	InitializeContainers();

	//FastDebugMsg("Initializing Input Manager");
	//// Initialize the Input Manager
	//if (InitializeInputManager() == FALSE)
	//{ // We were unable to initialize the input manager
	//	fprintf(stderr, "Couldn't init input manager\n");
	//	FastDebugMsg("FAILED : Initializing Input Manager");
	//	return FALSE;
	//}

	FastDebugMsg("Initializing Video Manager");
	// Initialize DirectDraw (DirectX 2)
	if (InitializeVideoManager( ) == FALSE)
	{ // We were unable to initialize the video manager
		fprintf(stderr, "Couldn't init video manager\n");
		FastDebugMsg("FAILED : Initializing Video Manager");
		return FALSE;
	}

	DoTester();

	// Initialize Video Object Manager
	FastDebugMsg("Initializing Video Object Manager");
	if ( !InitializeVideoObjectManager( ) )
	{ 
		FastDebugMsg("FAILED : Initializing Video Object Manager");
		return FALSE;
	}

	//// Initialize Video Surface Manager
	//FastDebugMsg("Initializing Video Surface Manager");
	//if ( !InitializeVideoSurfaceManager( ) )
	//{ 
	//	FastDebugMsg("FAILED : Initializing Video Surface Manager");
	//	return FALSE;
	//}

	// Snap: moved the following from InitJA2SplashScreen for clarity
	STRING512			CurrentDir;
	STRING512			DataDir;

	InitializeJA2Clock();
	//InitializeJA2TimerID();
	// Get Executable Directory
	GetExecutableDirectory( CurrentDir );

	// Adjust Current Dir
	sprintf( DataDir, "%s\\Data", CurrentDir );
	if ( !SetFileManCurrentDirectory( DataDir ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Could not find data directory, shutting down");
		return FALSE;
	}

	//Initialize the file database
	//InitializeFileDatabase( gGameLibaries, NUMBER_OF_LIBRARIES ); // ?!?! doesn't take parameters (jonathanl)
	InitializeFileDatabase();

	// Snap: Initialize the Data directory catalogue
	gDefaultDataCat.NewCat(DataDir);

	// Snap: Get the custom Data directory location from ja2.ini (if any)
	// and initialize the custom catalogue
	char customDataPath[MAX_PATH];
	if ( GetPrivateProfileString( "Ja2 Settings","CUSTOM_DATA_LOCATION", "", customDataPath, MAX_PATH, "..\\Ja2.ini" ) ) {
		gCustomDataCat.NewCat(std::string(CurrentDir) + '\\' + customDataPath);
	}

//	InitJA2SplashScreen();

	// useless
	// Make sure we start up our local clock (in milliseconds)
	// We don't need to check for a return value here since so far its always TRUE
	//InitializeClockManager();  // must initialize after VideoManager, 'cause it uses ghWindow

//	// Create font translation table (store in temp structure)
//	pFontTable = CreateEnglishTransTable( );
//	if ( pFontTable == NULL )
//	{
//		return( FALSE );
//	}
//
//	// Initialize Font Manager
//	FastDebugMsg("Initializing the Font Manager");
//	// Init the manager and copy the TransTable stuff into it.
//	if ( !InitializeFontManager( 8, pFontTable ) )
//	{ 
//		FastDebugMsg("FAILED : Initializing Font Manager");
//		return FALSE;
//	}
//	// Don't need this thing anymore, so get rid of it (but don't de-alloc the contents)
//	MemFree( pFontTable );
//
//	FastDebugMsg("Initializing Sound Manager");
//	// Initialize the Sound Manager (DirectSound)
//#ifndef UTIL
//	if (InitializeSoundManager() == FALSE)
//	{ // We were unable to initialize the sound manager
//		FastDebugMsg("FAILED : Initializing Sound Manager");
//		return FALSE;
//	}  
//#endif
//
//	FastDebugMsg("Initializing Random");
//	// Initialize random number generator
//	InitializeRandom(); // no Shutdown
//
//	FastDebugMsg("Initializing Game Manager");
//	// Initialize the Game
//	if (InitializeGame() == FALSE)
//	{ // We were unable to initialize the game
//		FastDebugMsg("FAILED : Initializing Game Manager");
//		return FALSE;
//	}
//
//	// Register mouse wheel message
//	guiMouseWheelMsg = RegisterWindowMessage( MSH_MOUSEWHEEL );

	gfGameInitialized = TRUE;
	
	return TRUE;
}


void ShutdownStandardGamingPlatform(void)
{
	//
	// Shut down the different components of the SGP
	//

	// TEST
	//SoundServiceStreams();

	//if (gfGameInitialized)
	//{
	//	ShutdownGame();  
	//}


	//ShutdownButtonSystem();
	//MSYS_Shutdown();

//#ifndef UTIL
//  ShutdownSoundManager();
//#endif

//	DestroyEnglishTransTable( );    // has to go before ShutdownFontManager()
//	ShutdownFontManager();
//
//	ShutdownClockManager();   // must shutdown before VideoManager, 'cause it uses ghWindow

#ifdef SGP_VIDEO_DEBUGGING
	PerformVideoInfoDumpIntoFile( "SGPVideoShutdownDump.txt", FALSE );
#endif

	//ShutdownVideoSurfaceManager();
	ShutdownVideoObjectManager();
	ShutdownVideoManager();
	//ShutdownInputManager();
	ShutdownContainers();
	ShutdownFileManager();
	//ShutdownMutexManager();  useless

#ifdef EXTREME_MEMORY_DEBUGGING
	DumpMemoryInfoIntoFile( (UINT8*)"ExtremeMemoryDump.txt", FALSE );
#endif

	ShutdownMemoryManager();  // must go last (except for Debug), for MemDebugCounter to work right...

	//
	// Make sure we unregister the last remaining debug topic before shutting
	// down the debugging layer
	UnRegisterDebugTopic(TOPIC_SGP, "Standard Gaming Platform");

	ShutdownDebugManager();
}

//
////int PASCAL WinMain(HINSTANCE hInstance,  HINSTANCE hPrevInstance, LPSTR pCommandLine, int sCommandShow)
////{
////
//////If we are to use exception handling
////#ifdef ENABLE_EXCEPTION_HANDLING
////	int Result = -1;
////
////
////	__try
////	{
////		Result = HandledWinMain(hInstance, hPrevInstance, pCommandLine, sCommandShow);
////	}
////	__except( RecordExceptionInfo( GetExceptionInformation() ))
////	{
////		// Do nothing here - RecordExceptionInfo() has already done
////		// everything that is needed. Actually this code won't even
////		// get called unless you return EXCEPTION_EXECUTE_HANDLER from
////		// the __except clause.
////
////
////	}
////	return Result;
////
////}
////
//////Do not place code in between WinMain and Handled WinMain
////
////
////
////int PASCAL HandledWinMain(HINSTANCE hInstance,  HINSTANCE hPrevInstance, LPSTR pCommandLine, int sCommandShow)
////{
//////DO NOT REMOVE, used for exception handing list above in WinMain
////#endif
////	MSG			Message;
////	HWND		hPrevInstanceWindow;
////	SDL_Event	event;
////
////	// Make sure that only one instance of this application is running at once
////	// // Look for prev instance by searching for the window
////	hPrevInstanceWindow = FindWindowEx( NULL, NULL, APPLICATION_NAME, APPLICATION_NAME );
////
////	// One is found, bring it up!
////	if ( hPrevInstanceWindow != NULL )
////	{
////		SetForegroundWindow( hPrevInstanceWindow );
////		ShowWindow( hPrevInstanceWindow, SW_RESTORE );
////		return( 0 );
////	}
////
////	ghInstance = hInstance;
////
////	// Copy commandline!
////	strncpy( gzCommandLine, pCommandLine, 100);
////	gzCommandLine[99]='\0';
////
////	//Process the command line BEFORE initialization
////	ProcessJa2CommandLineBeforeInitialization( pCommandLine );
////
////	// Mem Usage
////	giStartMem = MemGetFree(  ) / 1024;
////  
////	ShowCursor(FALSE);
////
////	// Inititialize the SGP
////	if (InitializeStandardGamingPlatform(hInstance, sCommandShow) == FALSE)
////	{ // We failed to initialize the SGP
////		return 0;
////	}
////
////#ifdef JA2
////	#ifdef ENGLISH
////		SetIntroType( INTRO_SPLASH );
////	#endif
////#endif
////
////	gfApplicationActive = TRUE;
////	gfProgramIsRunning = TRUE;
////
////	FastDebugMsg("Running Game");
////
////	// At this point the SGP is set up, which means all I/O, Memory, tools, etc... are available. All we need to do is 
////	// attend to the gaming mechanics themselves
////	while (gfProgramIsRunning)
////	{
////		if ( SDL_PollEvent( &event ) )
////		{
////			switch( event.type )
////			{
////			case SDL_QUIT:
////				ShutdownStandardGamingPlatform();
////				ShowCursor(TRUE);
////				PostQuitMessage(0);
////				break;
////			default:
////				break;
////			}
////		}
////		else
////		{ // Windows hasn't processed any messages, therefore we handle the rest
////			if (gfApplicationActive == FALSE)
////			{ // Well we got nothing to do but to wait for a message to activate
////				SDL_WaitEvent(NULL);
////			} 
////			else
////			{ // Well, the game is active, so we handle the game stuff        
////				//GameLoop();        
////
////				// After this frame, reset input given flag
////				gfSGPInputReceived  =  FALSE;			
////			}
////		}
////		//if (PeekMessage(&Message, NULL, 0, 0, PM_NOREMOVE))
////		//{ // We have a message on the WIN95 queue, let's get it
////		//	if (!GetMessage(&Message, NULL, 0, 0))
////		//	{ // It's quitting time
////		//		return Message.wParam;
////		//	}
////		//	// Ok, now that we have the message, let's handle it
////		//	TranslateMessage(&Message);
////		//	DispatchMessage(&Message);      
////		//}
////		//else
////		//{ // Windows hasn't processed any messages, therefore we handle the rest
////		//	if (gfApplicationActive == FALSE)
////		//	{ // Well we got nothing to do but to wait for a message to activate
////		//		WaitMessage();
////		//	} 
////		//	else
////		//	{ // Well, the game is active, so we handle the game stuff        
////		//		GameLoop();        
////
////		//		// After this frame, reset input given flag
////		//		gfSGPInputReceived  =  FALSE;			
////		//	}
////		//}
////	}
////
////	// This is the normal exit point
////	FastDebugMsg("Exiting Game");
////	PostQuitMessage(0);
////
////	// SGPExit() will be called next through the atexit() mechanism...  This way we correctly process both normal exits and
////	// emergency aborts (such as those caused by a failed assertion).
////
////	// return wParam of the last message received
////	return Message.wParam;
////}
////
////
//
void SGPExit(void)
{
	static BOOLEAN fAlreadyExiting = FALSE;
	BOOLEAN fUnloadScreens = TRUE;


	// helps prevent heap crashes when multiple assertions occur and call us
	if ( fAlreadyExiting )
	{
		return;
	}

	fAlreadyExiting = TRUE;
	gfProgramIsRunning = FALSE;

	ShutdownStandardGamingPlatform();
	SDL_Quit();
	ShowCursor(TRUE);
	if(strlen(gzErrorMsg))
	{
		MessageBox(NULL, gzErrorMsg, "Error", MB_OK | MB_ICONERROR  );
	}
}

void GetRuntimeSettings( )
{
	// Runtime settings - for now use INI file - later use registry
	STRING512		INIFile;		// Path to the ini file
	CHAR8			zScreenResolution[ 50 ];
	
	// Get Executable Directory
	GetExecutableDirectory( INIFile );

	strcat(INIFile, "\\Ja2.ini");

	iResolution = -1;

	if (GetPrivateProfileString( "Ja2 Settings","SCREEN_RESOLUTION", "", zScreenResolution, 50, INIFile ))
	{
		iResolution = atoi(zScreenResolution);
	}

#ifdef JA2EDITOR
		if (GetPrivateProfileString( "Ja2 Settings","EDITOR_SCREEN_RESOLUTION", "", zScreenResolution, 50, INIFile ))
		{
			iResolution = atoi(zScreenResolution);
		}
#endif


	int	iResX;
	int iResY;

	switch (iResolution)
	{
		// 640x480
		case 0:
			iResX = 640;
			iResY = 480;
			break;
		// 1024x768
		case 2:
			iResX = 1024;
			iResY = 768;
			break;
		// 800x600
		default:
			iResolution = 1;
			iResX = 800;
			iResY = 600;
			break;
	}

	gbPixelDepth = GetPrivateProfileInt( "SGP", "PIXEL_DEPTH", PIXEL_DEPTH, INIFile );

	SCREEN_WIDTH = GetPrivateProfileInt( "SGP", "WIDTH", iResX, INIFile );
	SCREEN_HEIGHT = GetPrivateProfileInt( "SGP", "HEIGHT", iResY, INIFile );

	iScreenWidthOffset = (SCREEN_WIDTH - 640) / 2;
	iScreenHeightOffset = (SCREEN_HEIGHT - 480) / 2;
}

void ShutdownWithErrorBox(CHAR8 *pcMessage)
{
	strncpy(gzErrorMsg, pcMessage, 255);
	gzErrorMsg[255]='\0';
	gfIgnoreMessages=TRUE;

	exit(0);
}

void ProcessJa2CommandLineBeforeInitialization(void)
{
	INT32	cnt = 1;

	while( cnt < giCLArgumentsCount )													
	{
		//if its the NO SOUND option
		if(!_strnicmp(gpCLArgument[cnt], "/NOSOUND", 8))
		{
			//disable the sound
			SoundEnableSound(FALSE);
		} else if(!_strnicmp(gpCLArgument[cnt], "/FULLSCREEN", 11))
		{
			gfFullScreen = TRUE;
		}
		cnt++;
	}
}