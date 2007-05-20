/* $Id: sgp.c,v 1.4 2004/03/19 06:16:04 digicrab Exp $ */
//its test what doeas it do?
#ifdef JA2_PRECOMPILED_HEADERS
	#include "ja2_sgp_all.h"
	#include "ja2_splash.h"
	#include "utilities.h"
	#include "SDL.h"
#else
	#include "platform.h"
	#include "SDL.h"
	#include "types.h"
	#include "sgp.h"
	#include "vobject.h"
	#include "font.h"
	#include "local.h"
	#include "file_man.h"
	#include "input.h"
	#include "random.h"
	#include "game_loop.h"
	#include "sound_man.h"
	#include "ja2_splash.h"
	#include "timer_control.h"
	#include "input.h"
	#include "build_defines.h"
	#include "intro.h"
	#include "library_database.h"
	#include "vfs.hpp"
	#include "io_layer.h"
		
#endif


//#include "exception_handling.h"

#include "ini_reader.h"

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif


extern UINT32	MemDebugCounter;


//extern	BOOLEAN	CheckIfGameCdromIsInCDromDrive();
extern  void    QueueEvent(UINT16 ubInputEvent, UINT32 usParam, UINT32 uiParam);

// Prototype Declarations

BOOLEAN          InitializeStandardGamingPlatform(void);
void             ShutdownStandardGamingPlatform(void);
void			 GetRuntimeSettings( );
void 			 TestIO( void );

#if !defined(JA2) && !defined(UTILS)
void	ProcessCommandLine(CHAR8 *pCommandLine);
BOOLEAN	RunSetup(void);

// Should the game immediately load the quick save at startup?
BOOLEAN						gfLoadAtStartup=FALSE;
BOOLEAN						gfUsingBoundsChecker=FALSE;
CHAR8						*gzStringDataOverride=NULL;
BOOLEAN						gfCapturingVideo = FALSE;

#endif


// Global Variable Declarations

// moved from header file: 24mar98:HJH
UINT32		giStartMem;

// GLOBAL RUN-TIME SETTINGS
//UINT32		guiMouseWheelMsg;			// For mouse wheel messages

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

	TestIO();	// $$$

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

	GetRuntimeSettings( );

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
	printf("Data directory is %s\n", DataDir);
	if ( !SetFileManCurrentDirectory( DataDir ) )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Could not find data directory, shutting down");
		return FALSE;
	}

	//Initialize the file database
	//InitializeFileDatabase( gGameLibaries, NUMBER_OF_LIBRARIES ); // ?!?! doesn't take parameters (jonathanl)
	if ( !InitializeFileDatabase() )
	{
		fprintf(stderr, "Couldn't init library database\n");
		return FALSE;
	}

	// Snap: Initialize the Data directory catalogue
	gDefaultDataCat.NewCat(DataDir);

	// Snap: Get the custom Data directory location from ja2.ini (if any)
	// and initialize the custom catalogue
	CHAR8 customDataPath[MAX_PATH];
	const CHAR8 *pCustomDir;
	CIniReader	customDP;

	customDP.Open("..\\Ja2.ini", TRUE);
	pCustomDir = customDP.ReadString("Ja2 Settings", "CUSTOM_DATA_LOCATION", NULL);
	if ( !pCustomDir )
		printf("Custom dir isn't specified\n");
	else
	{
		sprintf(customDataPath, "%s\\%s", CurrentDir, pCustomDir);
		printf("Custom dir = %s\n", customDataPath);
		gCustomDataCat.NewCat(customDataPath);
	}
	customDP.Close();

	if ( !VFS.AddContainerByIndex(0) )
		printf("AddContainer failed!\n");

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
//	ShowCursor(TRUE);
//	if(strlen(gzErrorMsg))
//	{
//		MessageBox(NULL, gzErrorMsg, "Error", MB_OK | MB_ICONERROR  );
//	}
}

void GetRuntimeSettings( )
{
	// Runtime settings - for now use INI file - later use registry
	STRING512		INIFile;		// Path to the ini file
	CIniReader		rtIni;
	
	// Get Executable Directory
	GetHomeDirectory( INIFile );

	strcat(INIFile, "\\Ja2.ini");

	printf("Reading run-time settings from: %s\n", INIFile);
	if ( !rtIni.Open( INIFile, TRUE ) )
	{
		printf("Failed to open run-time ini\n");
	}

	iResolution = -1;

#ifndef JA2EDITOR
	iResolution = rtIni.ReadInteger("Ja2 Settings", "SCREEN_RESOLUTION", 1);
#else
	iResolution = rtIni.ReadInteger("Ja2 Settings", "EDITOR_SCREEN_RESOLUTION", 1);
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

	printf("Specified resolution #%d: (%d, %d)\n", iResolution, iResX, iResY);

	SCREEN_WIDTH  = iResX;
	SCREEN_HEIGHT = iResY;

#if 0
	//gbPixelDepth = rtIni.ReadInteger("SGP", "PIXEL_DEPTH", 1);
	gbPixelDepth = GetPrivateProfileInt( "SGP", "PIXEL_DEPTH", PIXEL_DEPTH, INIFile );

	SCREEN_WIDTH = GetPrivateProfileInt( "SGP", "WIDTH", iResX, INIFile );
	SCREEN_HEIGHT = GetPrivateProfileInt( "SGP", "HEIGHT", iResY, INIFile );
#endif

	rtIni.Close();

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

void TestIO( void )
{
//	const STR8	name = "/home/lesh/ja2_113/test1.txt";
//	HWFILE	file;

//	file = IO_File_Open( name, IO_ACCESS_READ );
//	printf("File opened: %d\n", file );
//	printf("File size  : %d\n", IO_File_GetSize( file ) );
//	printf("File closed: %d\n", IO_File_Close( file ) );

	char	buffer[256];
	const	STR8 pattern = "/home/lesh/ja2_113/*";

	IO_Dir_SetCurrentDirectory( "/home/lesh/ja2_113/" );
	IO_File_GetFirst( pattern, buffer, 256 );
	do
	{
		printf("%s\n", buffer);
	} while ( IO_File_GetNext( buffer, 256 ) );
	IO_File_GetClose();
}
