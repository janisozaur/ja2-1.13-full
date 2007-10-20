/* $Id: sgp.c,v 1.4 2004/03/19 06:16:04 digicrab Exp $ */
//its test what doeas it do?
#ifdef JA2_PRECOMPILED_HEADERS
	#include "JA2 SGP ALL.H"
	#include "JA2 Splash.h"
	#include "utilities.h"
#elif defined( WIZ8_PRECOMPILED_HEADERS )
	#include "WIZ8 SGP ALL.H"
#else
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
	#ifdef JA2
		#include "JA2 Splash.h"
		#include "Timer Control.h"
	#endif
	#if !defined( JA2 ) && !defined( UTIL )
		#include "GameData.h"               // for MoveTimer() [Wizardry specific]
	#endif
	#include "LibraryDataBase.h"
	#include "utilities.h"
#endif

	#include "input.h"
	#include "zmouse.h"


#include <iostream>

#include "ExceptionHandling.h"

#include "dbt.h"
#include "INIReader.h"
#include "Console.h"
#include "Lua Interpreter.h"

#ifdef JA2
	#include "BuildDefines.h"
	#include "Intro.h"
#endif

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif


extern UINT32 MemDebugCounter;
#ifdef JA2
extern BOOLEAN gfPauseDueToPlayerGamePause;
extern int     iScreenMode;
extern BOOL    bScreenModeCmdLine;
#endif

extern	BOOLEAN	CheckIfGameCdromIsInCDromDrive();
extern  void    QueueEvent(UINT16 ubInputEvent, UINT32 usParam, UINT32 uiParam);

// Prototype Declarations

INT32 FAR PASCAL WindowProcedure(HWND hWindow, UINT16 Message, WPARAM wParam, LPARAM lParam);
BOOLEAN          InitializeStandardGamingPlatform(HINSTANCE hInstance, int sCommandShow);
void             ShutdownStandardGamingPlatform(void);
void						 GetRuntimeSettings( );

int PASCAL HandledWinMain(HINSTANCE hInstance,  HINSTANCE hPrevInstance, LPSTR pCommandLine, int sCommandShow);

Console g_Console("", "", "Lua Console", "no");

#if !defined(JA2) && !defined(UTILS)
void							ProcessCommandLine(CHAR8 *pCommandLine);
BOOLEAN						RunSetup(void);

// Should the game immediately load the quick save at startup?
BOOLEAN						gfLoadAtStartup=FALSE;
BOOLEAN						gfUsingBoundsChecker=FALSE;
CHAR8						*gzStringDataOverride=NULL;
BOOLEAN						gfCapturingVideo = FALSE;

#endif

HINSTANCE					ghInstance;


#ifdef JA2
	void ProcessJa2CommandLineBeforeInitialization(CHAR8 *pCommandLine);
#endif

// Global Variable Declarations
RECT				rcWindow;
POINT               ptWindowSize;

// moved from header file: 24mar98:HJH
UINT32		giStartMem;
//UINT8			gbPixelDepth;		// redefintion... look down a few lines (jonathanl)
// GLOBAL RUN-TIME SETTINGS

UINT32		guiMouseWheelMsg;			// For mouse wheel messages

BOOLEAN gfApplicationActive;
BOOLEAN gfProgramIsRunning;
BOOLEAN gfGameInitialized = FALSE;
//UINT32	giStartMem; // redefintion... look up a few lines (jonathanl)
BOOLEAN	gfDontUseDDBlits	= FALSE;

// There were TWO of them??!?! -- DB
//CHAR8		gzCommandLine[ 100 ];
CHAR8		gzCommandLine[100];		// Command line given

CHAR8		gzErrorMsg[2048]="";
BOOLEAN	gfIgnoreMessages=FALSE;

// GLOBAL VARIBLE, SET TO DEFAULT BUT CAN BE CHANGED BY THE GAME IF INIT FILE READ
UINT8		gbPixelDepth = PIXEL_DEPTH;

INT32 FAR PASCAL WindowProcedure(HWND hWindow, UINT16 Message, WPARAM wParam, LPARAM lParam)
{ 
	static BOOLEAN fRestore = FALSE;

	if ( Message == WM_USER )
	{
		FreeConsole();
		return 0L;
	}
	BOOL visible = IsWindowVisible(hWindow);

	if(gfIgnoreMessages)
		return(DefWindowProc(hWindow, Message, wParam, lParam));

	// ATE: This is for older win95 or NT 3.51 to get MOUSE_WHEEL Messages
	if ( Message == guiMouseWheelMsg )
	{
      QueueEvent(MOUSE_WHEEL, wParam, lParam);
			return( 0L );
	}
 
	switch(Message)
  {
	case WM_CLOSE:
		PostQuitMessage(0);
		break;

		case WM_MOUSEWHEEL:
			{
				QueueEvent(MOUSE_WHEEL, wParam, lParam);
				break;
			}
		
#ifdef JA2
    case WM_MOVE:
//        if( 1==iScreenMode )
          {
          GetClientRect(hWindow, &rcWindow);
		  // Go ahead and clamp the client width and height
		  rcWindow.right = SCREEN_WIDTH;
		  rcWindow.bottom = SCREEN_HEIGHT;
          ClientToScreen(hWindow, (LPPOINT)&rcWindow);
          ClientToScreen(hWindow, (LPPOINT)&rcWindow+1);
			int xPos = (int)(short) LOWORD(lParam); 
			int yPos = (int)(short) HIWORD(lParam);
			BOOL needchange = FALSE;
			if (xPos < 0)
			{
				xPos = 0;
				needchange = TRUE;
			}
			if (yPos < 0)
			{
				yPos = 0;
				needchange = TRUE;
			}
			if (needchange)
			{
				SetWindowPos( hWindow, NULL, xPos, yPos, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);
			}

          }
        break;
	case WM_GETMINMAXINFO:
		{
			MINMAXINFO *mmi = (MINMAXINFO*)lParam;

			mmi->ptMaxSize = ptWindowSize;
			mmi->ptMaxTrackSize = mmi->ptMaxSize;
			mmi->ptMinTrackSize = mmi->ptMaxSize;
			break;
		}
#else
		case WM_MOUSEMOVE:
			break;

		case WM_SIZING:
		{
			LPRECT	lpWindow;
			INT32		iWidth, iHeight, iX, iY;
			BOOLEAN fWidthByHeight=FALSE, fHoldRight=FALSE;

			lpWindow = (LPRECT) lParam;  

			iWidth=lpWindow->right-lpWindow->left;
			iHeight=lpWindow->bottom-lpWindow->top;
			iX = (lpWindow->left + lpWindow->right)/2;
			iY = (lpWindow->top + lpWindow->bottom)/2;

			switch(wParam)
			{
				case WMSZ_BOTTOMLEFT:
					fHoldRight=TRUE;
				case WMSZ_BOTTOM:
				case WMSZ_BOTTOMRIGHT:
					if(iHeight < SCREEN_HEIGHT)
					{
						lpWindow->bottom=lpWindow->top+SCREEN_HEIGHT;
						iHeight=SCREEN_HEIGHT;
					}
					fWidthByHeight=TRUE;
				break;

				case WMSZ_TOPLEFT:
					fHoldRight=TRUE;
				case WMSZ_TOP:
				case WMSZ_TOPRIGHT:
					if(iHeight < SCREEN_HEIGHT)
					{
						lpWindow->top=lpWindow->bottom-SCREEN_HEIGHT;
						iHeight=SCREEN_HEIGHT;
					}
					fWidthByHeight=TRUE;
					break;

				case WMSZ_LEFT:
					if(iWidth < SCREEN_WIDTH)
					{
						lpWindow->left=lpWindow->right-SCREEN_WIDTH;
						iWidth = SCREEN_WIDTH;
					}
					break;

				case WMSZ_RIGHT:
					if(iWidth < SCREEN_WIDTH)
					{
						lpWindow->right=lpWindow->left+SCREEN_WIDTH;
						iWidth = SCREEN_WIDTH;
					}
			}

			// Calculate width as a factor of height
			if(fWidthByHeight)
			{
				iWidth = iHeight * SCREEN_WIDTH / SCREEN_HEIGHT;
//				lpWindow->left = iX - iWidth/2;
//				lpWindow->right = iX + iWidth / 2;
				if(fHoldRight)
					lpWindow->left = lpWindow->right - iWidth;
				else
					lpWindow->right = lpWindow->left + iWidth;
			}
			else // Calculate height as a factor of width
			{
				iHeight = iWidth * SCREEN_HEIGHT / SCREEN_WIDTH;
//				lpWindow->top = iY - iHeight/2;
//				lpWindow->bottom = iY + iHeight/2;
				lpWindow->bottom = lpWindow->top + iHeight;
			}
	

/*
			switch(wParam)
			{
				case WMSZ_BOTTOM:
				case WMSZ_BOTTOMLEFT:
				case WMSZ_BOTTOMRIGHT:
					if(iHeight < SCREEN_HEIGHT)
						lpWindow->bottom=lpWindow->top+SCREEN_HEIGHT;
			}

			switch(wParam)
			{
				case WMSZ_TOP:
				case WMSZ_TOPLEFT:
				case WMSZ_TOPRIGHT:
					if(iHeight < SCREEN_HEIGHT)
						lpWindow->top=lpWindow->bottom-SCREEN_HEIGHT;
			}

			switch(wParam)
			{
				case WMSZ_BOTTOMLEFT:
				case WMSZ_LEFT:
				case WMSZ_TOPLEFT:
					if(iWidth < SCREEN_WIDTH)
						lpWindow->left=lpWindow->right-SCREEN_WIDTH;
			}

			switch(wParam)
			{
				case WMSZ_BOTTOMRIGHT:
				case WMSZ_RIGHT:
				case WMSZ_TOPRIGHT:
					if(iWidth < SCREEN_WIDTH)
						lpWindow->right=lpWindow->left+SCREEN_WIDTH;
			}
*/
		}
		break;

    case WM_SIZE:
		{
			UINT16 nWidth = LOWORD(lParam);  // width of client area 
			UINT16 nHeight = HIWORD(lParam); // height of client area 

			if(nWidth && nHeight)
			{
				switch(wParam)
				{
					case SIZE_MAXIMIZED:
						VideoFullScreen(TRUE);
						break;

					case SIZE_RESTORED:
						VideoResizeWindow();
						break;
				}
			}
		}
		break;

    case WM_MOVE:
		{
			INT32 xPos = (INT32)LOWORD(lParam);    // horizontal position 
			INT32 yPos = (INT32)HIWORD(lParam);    // vertical position 		
		}
		break;
#endif
	case WM_SETCURSOR:
		SetCursor( NULL);
		return TRUE;

	case WM_TIMER:
#ifdef LUACONSOLE
		PollConsole( );
#endif

      if (gfApplicationActive)
      {
        GameLoop();        
      } 
	  break;

    case WM_ACTIVATEAPP: 
      switch(wParam)
      {
        case TRUE: // We are restarting DirectDraw
          if (fRestore == TRUE)
          {
#ifdef JA2
	          RestoreVideoManager();
		        RestoreVideoSurfaces();	// Restore any video surfaces

						// unpause the JA2 Global clock
            if ( !gfPauseDueToPlayerGamePause )
            {
						  PauseTime( FALSE );
            }
#else
						if(!VideoInspectorIsEnabled())
						{
	            RestoreVideoManager();
		          RestoreVideoSurfaces();	// Restore any video surfaces
						}

	          MoveTimer(TIMER_RESUME);
#endif
            gfApplicationActive = TRUE;
          }
          break;
        case FALSE: // We are suspending direct draw
#ifdef JA2
						// pause the JA2 Global clock
						PauseTime( TRUE );
						SuspendVideoManager();
#else
#ifndef UTIL 
						if(!VideoInspectorIsEnabled())
							SuspendVideoManager();
#endif
#endif
          // suspend movement timer, to prevent timer crash if delay becomes long
          // * it doesn't matter whether the 3-D engine is actually running or not, or if it's even been initialized
          // * restore is automatic, no need to do anything on reactivation
#if !defined( JA2 ) && !defined( UTIL )
          MoveTimer(TIMER_SUSPEND);
#endif

          gfApplicationActive = FALSE;
          fRestore = TRUE;
          break;
      }
      break;

    case WM_CREATE: 
			break;

    case WM_DESTROY: 
			ShutdownStandardGamingPlatform();
//      ShowCursor(TRUE);
      PostQuitMessage(0);
      break;

		case WM_SETFOCUS:
#if !defined( JA2 ) && !defined( UTIL )
			if(!VideoInspectorIsEnabled())
				RestoreVideoManager();
			gfApplicationActive=TRUE;
//			RestrictMouseToXYXY(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
#else
      RestoreCursorClipRect( );
#endif

			break;

		case WM_KILLFOCUS:
#if !defined( JA2 ) && !defined( UTIL )
			if(!VideoInspectorIsEnabled())
				SuspendVideoManager();

			gfApplicationActive=FALSE;
			FreeMouseCursor();
#endif
			// Set a flag to restore surfaces once a WM_ACTIVEATEAPP is received
			fRestore = TRUE;
			break;


#if defined( JA2 )
		case  WM_DEVICECHANGE:
			{
				//DEV_BROADCAST_HDR  *pHeader = (DEV_BROADCAST_HDR  *)lParam;

				////if a device has been removed
				//if( wParam == DBT_DEVICEREMOVECOMPLETE )
				//{
				//	//if its  a disk
				//	if( pHeader->dbch_devicetype == DBT_DEVTYP_VOLUME )
				//	{
				//		//check to see if the play cd is still in the cdrom
				//		if( !CheckIfGameCdromIsInCDromDrive() )
				//		{
				//		}
				//	}
				//}
			}
			break;
#endif

		case WM_SYSKEYUP:
		case WM_KEYUP:
		    KeyUp(wParam, lParam);
			break;

		case WM_SYSKEYDOWN:
		case WM_KEYDOWN:
		    KeyDown(wParam, lParam);
			gfSGPInputReceived =  TRUE;
			break;

		case WM_CHAR:
			if (wParam == '\\' &&
				lParam && KF_ALTDOWN)
			{
				g_Console.Create(ghWindow);
				cout << "LUA console ready" << endl;
				cout << "> ";
			}
			break;

		case WM_INPUTREADY:
			{
				wstring *tstr = (wstring*) lParam;
				if (EvalLua( tstr->c_str()))
				{
					tstr->erase();
				}
				else
				{
					cout << ">";
				}

				cout << "> ";
			}
			break;
    default
    : return DefWindowProc(hWindow, Message, wParam, lParam);
  }
  return 0L;
}



BOOLEAN InitializeStandardGamingPlatform(HINSTANCE hInstance, int sCommandShow)
{
	FontTranslationTable *pFontTable;

	// now required by all (even JA2) in order to call ShutdownSGP
	atexit(SGPExit);

	// First, initialize the registry keys.
	InitializeRegistryKeys( "Wizardry8", "Wizardry8key" );

	// For rendering DLLs etc.
#ifndef JA2
	AddSubdirectoryToPath("DLL");
#endif

	// Second, read in settings
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
		FastDebugMsg("FAILED : Initializing Memory Manager");
		return FALSE;
	}

#ifdef JA2
  FastDebugMsg("Initializing Mutex Manager");
	// Initialize the Dirty Rectangle Manager
	if (InitializeMutexManager() == FALSE)
	{ // We were unable to initialize the game
		FastDebugMsg("FAILED : Initializing Mutex Manager");
		return FALSE;
	}
#endif

	FastDebugMsg("Initializing File Manager");
	// Initialize the File Manager
	if (InitializeFileManager(NULL) == FALSE)
	{ // We were unable to initialize the file manager
		FastDebugMsg("FAILED : Initializing File Manager");
		return FALSE;
	}

	FastDebugMsg("Initializing Containers Manager");
  InitializeContainers();

	FastDebugMsg("Initializing Input Manager");
	// Initialize the Input Manager
	if (InitializeInputManager() == FALSE)
	{ // We were unable to initialize the input manager
		FastDebugMsg("FAILED : Initializing Input Manager");
		return FALSE;
	}

	FastDebugMsg("Initializing Video Manager");
	// Initialize DirectDraw (DirectX 2)
	if (InitializeVideoManager(hInstance, (UINT16) sCommandShow, (void *) WindowProcedure) == FALSE)
	{ // We were unable to initialize the video manager
		FastDebugMsg("FAILED : Initializing Video Manager");
		return FALSE;
	}

	// Initialize Video Object Manager
	FastDebugMsg("Initializing Video Object Manager");
	if ( !InitializeVideoObjectManager( ) )
	{ 
		FastDebugMsg("FAILED : Initializing Video Object Manager");
		return FALSE;
	}

	// Initialize Video Surface Manager
	FastDebugMsg("Initializing Video Surface Manager");
	if ( !InitializeVideoSurfaceManager( ) )
	{ 
		FastDebugMsg("FAILED : Initializing Video Surface Manager");
		return FALSE;
	}

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

	//#ifdef JA2
		InitJA2SplashScreen();
	//#endif

  // Make sure we start up our local clock (in milliseconds)
  // We don't need to check for a return value here since so far its always TRUE
  InitializeClockManager();  // must initialize after VideoManager, 'cause it uses ghWindow

	// Create font translation table (store in temp structure)
	pFontTable = CreateEnglishTransTable( );
	if ( pFontTable == NULL )
	{
		return( FALSE );
	}

	// Initialize Font Manager
	FastDebugMsg("Initializing the Font Manager");
	// Init the manager and copy the TransTable stuff into it.
	if ( !InitializeFontManager( 8, pFontTable ) )
	{ 
		FastDebugMsg("FAILED : Initializing Font Manager");
		return FALSE;
	}
	// Don't need this thing anymore, so get rid of it (but don't de-alloc the contents)
	MemFree( pFontTable );

	FastDebugMsg("Initializing Sound Manager");
	// Initialize the Sound Manager (DirectSound)
#ifndef UTIL
	if (InitializeSoundManager() == FALSE)
	{ // We were unable to initialize the sound manager
		FastDebugMsg("FAILED : Initializing Sound Manager");
		return FALSE;
	}  
#endif

	FastDebugMsg("Initializing Random");
  // Initialize random number generator
  InitializeRandom(); // no Shutdown

	FastDebugMsg("Initializing Game Manager");
	// Initialize the Game
	if (InitializeGame() == FALSE)
	{ // We were unable to initialize the game
		FastDebugMsg("FAILED : Initializing Game Manager");
		return FALSE;
	}

	// Register mouse wheel message
	guiMouseWheelMsg = RegisterWindowMessage( MSH_MOUSEWHEEL );

	gfGameInitialized = TRUE;
	
	return TRUE;
}


void ShutdownStandardGamingPlatform(void)
{
#ifndef JA2
	static BOOLEAN Reenter = FALSE;

	//
	// Prevent multiple reentry into this function
	//

	if (Reenter == FALSE)
	{
		Reenter = TRUE;
	}
	else
	{
		return;
	}
#endif

	//
	// Shut down the different components of the SGP
	//

	// TEST
	SoundServiceStreams();

	if (gfGameInitialized)
	{
		ShutdownGame();  
	}


	ShutdownButtonSystem();
	MSYS_Shutdown();

#ifndef UTIL
  ShutdownSoundManager();
#endif

	DestroyEnglishTransTable( );    // has to go before ShutdownFontManager()
  ShutdownFontManager();

  ShutdownClockManager();   // must shutdown before VideoManager, 'cause it uses ghWindow

#ifdef SGP_VIDEO_DEBUGGING
	PerformVideoInfoDumpIntoFile( "SGPVideoShutdownDump.txt", FALSE );
#endif

	ShutdownVideoSurfaceManager();
  ShutdownVideoObjectManager();
  ShutdownVideoManager();

  ShutdownInputManager();
  ShutdownContainers();
  ShutdownFileManager();
#ifdef JA2
  ShutdownMutexManager();
#endif

#ifdef EXTREME_MEMORY_DEBUGGING
	DumpMemoryInfoIntoFile( "ExtremeMemoryDump.txt", FALSE );
#endif

  ShutdownMemoryManager();  // must go last (except for Debug), for MemDebugCounter to work right...

	//
  // Make sure we unregister the last remaining debug topic before shutting
  // down the debugging layer
  UnRegisterDebugTopic(TOPIC_SGP, "Standard Gaming Platform");

  ShutdownDebugManager();
}


int PASCAL WinMain(HINSTANCE hInstance,  HINSTANCE hPrevInstance, LPSTR pCommandLine, int sCommandShow)
{

//If we are to use exception handling
#ifdef ENABLE_EXCEPTION_HANDLING
	int Result = -1;


	__try
	{
		Result = HandledWinMain(hInstance, hPrevInstance, pCommandLine, sCommandShow);
	}
	__except( RecordExceptionInfo( GetExceptionInformation() ))
	{
		// Do nothing here - RecordExceptionInfo() has already done
		// everything that is needed. Actually this code won't even
		// get called unless you return EXCEPTION_EXECUTE_HANDLER from
		// the __except clause.


	}
	return Result;

}

//Do not place code in between WinMain and Handled WinMain



int PASCAL HandledWinMain(HINSTANCE hInstance,  HINSTANCE hPrevInstance, LPSTR pCommandLine, int sCommandShow)
{
//DO NOT REMOVE, used for exception handing list above in WinMain
#endif
  MSG				Message;
	HWND			hPrevInstanceWindow;
	UINT32          uiTimer = 0;

	// Make sure that only one instance of this application is running at once
	// // Look for prev instance by searching for the window
	hPrevInstanceWindow = FindWindowEx( NULL, NULL, APPLICATION_NAME, APPLICATION_NAME );

	// One is found, bring it up!
	if ( hPrevInstanceWindow != NULL )
	{
		SetForegroundWindow( hPrevInstanceWindow );
		ShowWindow( hPrevInstanceWindow, SW_RESTORE );
		return( 0 );
	}

	//rain
	//NSLoadSettings();
	//NSSaveSettings();
	//InitResolution();

	//EmergencyExitButtonInit();
	//end rain

#ifdef _DEBUG
	// Use this one ONLY if you're having memory corruption issues that can be repeated in a short time
	// Otherwise it will just run out of memory.
	//_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_DELAY_FREE_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_EVERY_1024_DF);
#endif

	ghInstance = hInstance;

		// Copy commandline!
#ifdef JA2
	strncpy( gzCommandLine, pCommandLine, 100);
	gzCommandLine[99]='\0';

	//Process the command line BEFORE initialization
	ProcessJa2CommandLineBeforeInitialization( pCommandLine );
#else
	ProcessCommandLine(pCommandLine);
#endif

	// Mem Usage
	giStartMem = MemGetFree(  ) / 1024;
  

#ifdef JA2
	// Handle Check for CD
	if ( !HandleJA2CDCheck( ) )
	{
		return( 0 );
	}
#else

	if(!RunSetup())
		return(0);

#endif

//  ShowCursor(FALSE);

  // Inititialize the SGP
  if (InitializeStandardGamingPlatform(hInstance, sCommandShow) == FALSE)
  { // We failed to initialize the SGP
    return 0;
  }

#ifdef LUACONSOLE
  if (1==iScreenMode)
  {
	  CreateConsole();
  }
#endif

#ifdef JA2
	#ifdef ENGLISH
		SetIntroType( INTRO_SPLASH );
	#endif
#endif

  gfApplicationActive = TRUE;
  gfProgramIsRunning = TRUE;

  FastDebugMsg("Running Game");

  // 0verhaul:  Use the smallest available timer to make sure all animation updates happen at the speed they're supposed to
  SetTimer( ghWindow, uiTimer, 1, NULL);

  // At this point the SGP is set up, which means all I/O, Memory, tools, etc... are available. All we need to do is 
  // attend to the gaming mechanics themselves
  Message.wParam = 0;

  while (gfProgramIsRunning)
  {
//    if (PeekMessage(&Message, NULL, 0, 0, PM_NOREMOVE))
//    { // We have a message on the WIN95 queue, let's get it
      if (!GetMessage(&Message, NULL, 0, 0))
      { // It's quitting time
        return Message.wParam;
      }
      // Ok, now that we have the message, let's handle it
      TranslateMessage(&Message);
      DispatchMessage(&Message);      
    }
#if 0
    else
    { // Windows hasn't processed any messages, therefore we handle the rest
#ifdef LUACONSOLE
		PollConsole( );
#endif

      if (gfApplicationActive == FALSE)
      { // Well we got nothing to do but to wait for a message to activate
        WaitMessage();
      } 
      else
      { // Well, the game is active, so we handle the game stuff        
        GameLoop();        

				// After this frame, reset input given flag
	      gfSGPInputReceived  =  FALSE;
      }
    }
  }
#endif

  KillTimer( ghWindow, uiTimer);

  // This is the normal exit point
  FastDebugMsg("Exiting Game");
  PostQuitMessage(0);

	// SGPExit() will be called next through the atexit() mechanism...  This way we correctly process both normal exits and
	// emergency aborts (such as those caused by a failed assertion).

	// return wParam of the last message received
	return Message.wParam;
}



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

// Wizardry only
#if !defined( JA2 ) && !defined( UTIL )
	if (gfGameInitialized)
	{
// ARM: if in DEBUG mode & we've ShutdownWithErrorBox, don't unload screens and release data structs to permit easier debugging
#ifdef _DEBUG
		if (gfIgnoreMessages)
		{
			fUnloadScreens = FALSE;
		}
#endif
		GameloopExit(fUnloadScreens);
	}
#endif

	ShutdownStandardGamingPlatform();
//  ShowCursor(TRUE);
	if(strlen(gzErrorMsg))
  {
		MessageBox(NULL, gzErrorMsg, "Error", MB_OK | MB_ICONERROR  );
  }

#ifndef JA2
	VideoDumpMemoryLeaks();
#endif

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

  /* Sergeant_Kolja. 2007-02-20: runtime Windowed mode instead of compile-time */
  /* 1 for Windowed, 0 for Fullscreen */
  if( !bScreenModeCmdLine )
    iScreenMode = (int) GetPrivateProfileInt( "Ja2 Settings","SCREEN_MODE_WINDOWED", iScreenMode, INIFile );
}

void ShutdownWithErrorBox(CHAR8 *pcMessage)
{
	strncpy(gzErrorMsg, pcMessage, 255);
	gzErrorMsg[255]='\0';
	gfIgnoreMessages=TRUE;

	exit(0);
}

#if !defined(JA2) && !defined(UTILS)

void ProcessCommandLine(CHAR8 *pCommandLine)
{
CHAR8 cSeparators[]="\t =";
CHAR8	*pCopy=NULL, *pToken;

	pCopy=(CHAR8 *)MemAlloc(strlen(pCommandLine) + 1);

	Assert(pCopy);
	if(!pCopy)
		return;

	memcpy(pCopy, pCommandLine, strlen(pCommandLine)+1);

	pToken=strtok(pCopy, cSeparators);
	while(pToken)													
	{
		if(!_strnicmp(pToken, "/NOSOUND", 8))
		{
			SoundEnableSound(FALSE);
		}
		else if(!_strnicmp(pToken, "/INSPECTOR", 10))
		{
			VideoInspectorEnable();
		}
		else if(!_strnicmp(pToken, "/VIDEOCFG", 9))
		{
			pToken=strtok(NULL, cSeparators);
			VideoSetConfigFile(pToken);
		}
		else if(!_strnicmp(pToken, "/LOAD", 5))
		{
			gfLoadAtStartup=TRUE;
		}
		else if(!_strnicmp(pToken, "/WINDOW", 7))
		{
			VideoFullScreen(FALSE);
		}
		else if(!_strnicmp(pToken, "/BC", 7))
		{
			gfUsingBoundsChecker = TRUE;
		}
		else if(!_strnicmp(pToken, "/CAPTURE", 7))
		{
			gfCapturingVideo = TRUE;
		}
		else if(!_strnicmp(pToken, "/NOOCT", 6))
		{
			NoOct();
		}
		else if(!_strnicmp(pToken, "/STRINGDATA", 11))
		{
			pToken=strtok(NULL, cSeparators);
			gzStringDataOverride = (CHAR8 *)MemAlloc(strlen(pToken) + 1);
			strcpy(gzStringDataOverride, pToken);
		}

		pToken=strtok(NULL, cSeparators);
	}

	MemFree(pCopy);
}

BOOLEAN RunSetup(void)
{
	if(!FileExists(VideoGetConfigFile()))
		_spawnl(_P_WAIT, "3DSetup.EXE", "3DSetup.EXE", VideoGetConfigFile(), NULL);

	return(FileExists(VideoGetConfigFile()));
}

#endif



void ProcessJa2CommandLineBeforeInitialization(CHAR8 *pCommandLine)
{
	CHAR8 cSeparators[]="\t =";
	CHAR8	*pCopy=NULL, *pToken;

	pCopy=(CHAR8 *)MemAlloc(strlen(pCommandLine) + 1);

	Assert(pCopy);
	if(!pCopy)
		return;

	memcpy(pCopy, pCommandLine, strlen(pCommandLine)+1);

	pToken=strtok(pCopy, cSeparators);
	while(pToken)													
	{
		//if its the NO SOUND option
		if(!_strnicmp(pToken, "/NOSOUND", 8))
		{
			//disable the sound
			SoundEnableSound(FALSE);
		}
		else if(!_strnicmp(pToken, "/FULLSCREEN", 11))
		{
			//overwrite Graphic setting from JA2_settings.ini
			iScreenMode=0; /* 1 for Windowed, 0 for Fullscreen */
      bScreenModeCmdLine = TRUE; /* if set TRUE, INI is no longer evaluated */
      /* no resolution read from Args. Still from INI, but could be added here, too...*/
		}
		else if(!_strnicmp(pToken, "/WINDOW", 7))
		{
			//overwrite Graphic setting from JA2_settings.ini
			iScreenMode=1; /* 1 for Windowed, 0 for Fullscreen */
      bScreenModeCmdLine = TRUE; /* if set TRUE, INI is no longer evaluated */
      /* no resolution read from Args. Still from INI, but could be added here, too...*/
		}

    //get the next token
		pToken=strtok(NULL, cSeparators);
	}

	MemFree(pCopy);
}
