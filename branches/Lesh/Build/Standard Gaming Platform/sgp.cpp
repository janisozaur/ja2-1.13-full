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
	#include <stdio.h>
	#include <stdarg.h>
	#include <string.h>
	#include "sgp.h"
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

BOOLEAN          InitializeStandardGamingPlatform(void);
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
BOOLEAN fSGPClosed = FALSE;

// There were TWO of them??!?! -- DB
CHAR8		gzCommandLine[100];		// Command line given
INT32		giCLArgumentsCount = 0;	// Count of command line arguments
CHAR8		**gpCLArgument = NULL;	// Array of pointers to command line arguments

CHAR8		gzErrorMsg[2048]="";
BOOLEAN	gfIgnoreMessages=FALSE;

// GLOBAL VARIBLE, SET TO DEFAULT BUT CAN BE CHANGED BY THE GAME IF INIT FILE READ
UINT8		gbPixelDepth = PIXEL_DEPTH;


BOOLEAN InitializeStandardGamingPlatform(void)
{
	FontTranslationTable *pFontTable;

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

	FastDebugMsg("Initializing Video Manager");
	// Initialize DirectDraw (DirectX 2)
	if (InitializeVideoManager( ) == FALSE)
	{ // We were unable to initialize the video manager
		fprintf(stderr, "Couldn't init video manager\n");
		FastDebugMsg("FAILED : Initializing Video Manager");
		return FALSE;
	}

	FastDebugMsg("Initializing Input Manager");
	// Initialize the Input Manager
	if (InitializeInputManager() == FALSE)
	{ // We were unable to initialize the input manager
		fprintf(stderr, "Couldn't init input manager\n");
		FastDebugMsg("FAILED : Initializing Input Manager");
		return FALSE;
	}

	DoTester();

	// Initialize Video Surface Manager
	FastDebugMsg("Initializing Video Surface Manager");
	if ( !InitializeVideoSurfaceManager( ) )
	{ 
		fprintf(stderr, "Couldn't init video surface manager\n");
		FastDebugMsg("FAILED : Initializing Video Surface Manager");
		return FALSE;
	}

	// Initialize Video Object Manager
	FastDebugMsg("Initializing Video Object Manager");
	if ( !InitializeVideoObjectManager( ) )
	{ 
		fprintf(stderr, "Couldn't init video object manager\n");
		FastDebugMsg("FAILED : Initializing Video Object Manager");
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

	//InitJA2SplashScreen();

	// Create font translation table (store in temp structure)
	pFontTable = CreateEnglishTransTable( );
	if ( pFontTable == NULL )
	{
		fprintf(stderr, "Couldn't create font translation table\n");
		return( FALSE );
	}

	// Initialize Font Manager
	FastDebugMsg("Initializing the Font Manager");
	// Init the manager and copy the TransTable stuff into it.
	if ( !InitializeFontManager( 8, pFontTable ) )
	{ 
		fprintf(stderr, "Couldn't init font manager\n");
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
		fprintf(stderr, "Couldn't init sound manager\n");
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
		fprintf(stderr, "Couldn't init game manager\n");
		FastDebugMsg("FAILED : Initializing Game Manager");
		return FALSE;
	}
//
//	// Register mouse wheel message
//	guiMouseWheelMsg = RegisterWindowMessage( MSH_MOUSEWHEEL );

	gfGameInitialized = TRUE;
	fSGPClosed = FALSE;
	
	return TRUE;
}


void ShutdownStandardGamingPlatform(void)
{
	//
	// Shut down the different components of the SGP
	//

	// TEST
	SoundServiceStreams();

	if (gfGameInitialized)
	{
		ShutdownGame();  
	}


#ifndef UTIL
  ShutdownSoundManager();
#endif

	DestroyEnglishTransTable( );    // has to go before ShutdownFontManager()
	ShutdownFontManager();

#ifdef SGP_VIDEO_DEBUGGING
	PerformVideoInfoDumpIntoFile( "SGPVideoShutdownDump.txt", FALSE );
#endif

	ShutdownVideoObjectManager();
	ShutdownVideoSurfaceManager();
	ShutdownInputManager();
	ShutdownVideoManager();
	ShutdownContainers();
	ShutdownFileManager();

#ifdef EXTREME_MEMORY_DEBUGGING
	DumpMemoryInfoIntoFile( (UINT8*)"ExtremeMemoryDump.txt", FALSE );
#endif

	ShutdownMemoryManager();  // must go last (except for Debug), for MemDebugCounter to work right...

	//
	// Make sure we unregister the last remaining debug topic before shutting
	// down the debugging layer
	UnRegisterDebugTopic(TOPIC_SGP, "Standard Gaming Platform");

	ShutdownDebugManager();
	fSGPClosed = TRUE;
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

	if ( fSGPClosed )
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