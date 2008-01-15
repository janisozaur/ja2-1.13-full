#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
	#include "HelpScreen.h"
	#include "Multilingual Text Code Generator.h"
	#include "INIReader.h"
	
#else
	#include "builddefines.h"
	#include <stdio.h>
	#include "sgp.h"
	#include "Gameloop.h"
	#include "Screens.h"
	#include "Wcheck.h"
	#include "INIReader.h"
	#include "vobject_blitters.h"
	#include "renderworld.h"
	#include "mousesystem.h"
	#include "sysutil.h"
	#include "worlddef.h"
	#include "Animation Data.h"
	#include "overhead.h"
	#include "Font Control.h"
	#include "Button System.h"
	#include "Timer Control.h"
	#include "Utilities.h"
	#include "Radar Screen.h"
	#include "Render Dirty.h"
	#include "cursors.h"
	#include "Sound Control.h"
	#include "Event Pump.h"
	#include "lighting.h"
	#include "Cursor Control.h"
	#include "music control.h"
	#include "video.h"
	#include "sys globals.h"
	#include "mapscreen.h"
	#include "interface items.h"
	#include "Maputility.h"
	#include "strategic.h"
	#include "dialogue control.h"
	#include "Text.h"
	#include "laptop.h"
	#include "NPC.h"
	#include "MercTextBox.h"
	#include "tile cache.h"
	#include "strategicmap.h"
	#include "Map Information.h"
	#include "laptop.h"
	#include "Shade Table Util.h"
	#include "Exit Grids.h"
	#include "Summary Info.h" 
	#include "GameSettings.h"
	#include "Game Init.h"
	#include "Init.h"
	#include "jascreens.h"
	#include "XML.h"
	#include "SaveLoadGame.h"
	#include "weapons.h"
	#include "Strategic Movement.h"
	#include "Vehicles.h"
	#include "Multilingual Text Code Generator.h"
	#include "editscreen.h"
#endif
extern INT16 iFuneralConstants[];
extern INT16 iFloristConstants[];
extern INT16 iFloristGalleryConstants[];
extern INT16 iFloristCardsConstants[];
extern INT16 iFloristOrderFormConstants[];
extern BOOLEAN GetCDromDriveLetter( STR8	pString );

// The InitializeGame function is responsible for setting up all data and Gaming Engine
// tasks which will run the game

#ifdef JA2BETAVERSION
extern	BOOLEAN	gfUseConsecutiveQuickSaveSlots;
#endif

#if defined( GERMAN ) && !defined( _DEBUG )
	//#define LASERLOCK_ENABLED
#endif


#ifdef LASERLOCK_ENABLED
	int	LASERLOK_Init( HINSTANCE hInstance );
	int	LASERLOK_Run();
	int	LASERLOK_Check();

	BOOLEAN	PrepareLaserLockSystem();
	void HandleLaserLockResult( BOOLEAN fSuccess );
//	int	TestCall( int n);
#endif

extern	HINSTANCE					ghInstance;


BOOLEAN LoadExternalGameplayData(STR directoryName)
{
	char fileName[MAX_PATH];

	// WANNE: Enemy drops - begin
	strcpy(fileName, directoryName);
	strcat(fileName, ENEMYMISCDROPSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInEnemyMiscDropsStats(gEnemyMiscDrops, fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, ENEMYEXPLOSIVEDROPSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInEnemyExplosiveDropsStats(gEnemyExplosiveDrops, fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, ENEMYWEAPONDROPSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInEnemyWeaponDropsStats(gEnemyWeaponDrops, fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, ENEMYAMMODROPSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInEnemyAmmoDropsStats(gEnemyAmmoDrops, fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, ENEMYARMOURDROPSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInEnemyArmourDropsStats(gEnemyArmourDrops, fileName))
		return FALSE;
	// WANNE: Enemy drops - end
	
	// Gotthard: Loading external Florist Data Text [2007-10-16]

	strcpy(fileName, directoryName);
	strcat(fileName, LAPTOPFLORISTTEXTFILENAME);
	DebugMsg( TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInFloristText(fileName))
		return FALSE;

	// Gotthard: Florist Text - End

	// Gotthard: Loading external Funeral Data Text [2007-10-18]
	strcpy(fileName, directoryName);
	strcat(fileName, LAPTOPFUNERALTEXTFILENAME);
	DebugMsg( TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInFuneralText(fileName))
		return FALSE;

	// Gotthard: Funeral Data - End

	// Gotthard: Loading External Delivery Locations Text [2007-10-18]
	strcpy(fileName, directoryName);
	strcat(fileName, LAPTOPBOBBYRAYSTEXTFILENAME);
	DebugMsg( TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInBobbyRaysText(fileName))
		return FALSE;

	// Gotthard: External Delivery Locations Text - End

	// Gotthard: External Sector Names [2007-10-19]
	strcpy(fileName, directoryName);
	strcat(fileName, STRATEGICSECTORNAMETEXTFILENAME);
	DebugMsg( TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInSectorNamesText(fileName))
		return FALSE;

	// Gotthard: External Sector Names - End

	// Gotthard: Laptop Florist Locations [2007-11-03]
	SetupFloristPositionArrays();//This initializes the arrays so we don't have to use #defines anymore. Gotthard 1/15/08
	strcpy(fileName, directoryName);
	strcat(fileName, LAPTOPFLORISTLOCATIONFILENAME);
	DebugMsg( TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInFloristLocations(fileName))
		return FALSE;
	// Gotthard: Laptop Florist Locations - End

	// Gotthard: Laptop Funeral Locations [2007-11-03]
	SetupFuneralPositionArray();//This initializes the array so we don't have to use #defines anymore. Gotthard 1/15/08
	strcpy(fileName, directoryName);
	strcat(fileName, LAPTOPFUNERALLOCATIONFILENAME);
	DebugMsg( TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInFuneralLocations(fileName))
		return FALSE;
	// Gotthard: Laptop Funeral Locations - End

	// WANNE: Sector Loadscreens [2007-05-18]
	strcpy(fileName, directoryName);
	strcat(fileName, SECTORLOADSCREENSFILENAME);

	if ( FileExists(fileName) )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		if(!ReadInSectorLoadscreensStats(gSectorLoadscreens, fileName))
			return FALSE;
	}
	else
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("File does not exist, fileName = %s", fileName));
		// Reset ini-option
		gGameExternalOptions.gfUseExternalLoadscreens = FALSE;
	}

	strcpy(fileName, directoryName);
	strcat(fileName, AMMOTYPESFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInAmmoTypeStats(fileName))
		return FALSE;

//Madd: Simple Localization
//Read in the correct ammostring file for the given language
	strcpy(fileName, directoryName);
#ifdef GERMAN
	strcat(fileName, GERMAN_PREFIX); // add German. prefix to filename
#endif
#ifdef RUSSIAN
	strcat(fileName, RUSSIAN_PREFIX); // add Russian. prefix to filename
#endif
	strcat(fileName, AMMOFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInAmmoStats(fileName))
		return FALSE;


    // Lesh: added this, begin
	strcpy(fileName, directoryName);
	strcat(fileName, BURSTSOUNDSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInBurstSoundArray(fileName))
		return FALSE;
    // Lesh: end

	strcpy(fileName, directoryName);
	strcat(fileName, ITEMSFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
	if(!ReadInItemStats(fileName,FALSE))
		return FALSE;

//Madd: Simple localization
// The idea here is that we can have a separate xml file that's named differently
// but only contains the relevant tags that need to be localized
// then when the file is read in using the same xml reader code, it will only overwrite
// the tags that are contained in the localized file.  This only works for items.xml 
// since I tweaked the xml_items.cpp to make it work :p
// So for instance, the german file would be called German.Items.xml and would only contain
// the uiIndex (for reference), szItemName, szLongItemName, szItemDesc, szBRName, and szBRDesc tags
 
#ifdef GERMAN 
	strcpy(fileName, directoryName);
	strcat(fileName, GERMAN_PREFIX);
	strcat(fileName, ITEMSFILENAME);
	if ( FileExists(fileName) )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		if(!ReadInItemStats(fileName,TRUE))
			return FALSE;
	}
#endif
#ifdef RUSSIAN 
	strcpy(fileName, directoryName);
	strcat(fileName, RUSSIAN_PREFIX);
	strcat(fileName, ITEMSFILENAME);
	if ( FileExists(fileName) )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
		if(!ReadInItemStats(fileName,TRUE))
			return FALSE;
	}
#endif

	//if(!WriteItemStats())
	//	return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, SOUNDSFILENAME);
	if(!ReadInSoundArray(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, MAGAZINESFILENAME);
	if(!ReadInMagazineStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, ATTACHMENTSFILENAME);
	if(!ReadInAttachmentStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, ATTACHMENTINFOFILENAME);
	if(!ReadInAttachmentInfoStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, LAUNCHABLESFILENAME);
	if(!ReadInLaunchableStats(fileName))
		return FALSE;
	
	strcpy(fileName, directoryName);
	strcat(fileName, COMPATIBLEFACEITEMSFILENAME);
	if(!ReadInCompatibleFaceItemStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, MERGESFILENAME);
	if(!ReadInMergeStats(fileName))
		return FALSE;

	//if(!WriteMergeStats())
	//	return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, ATTACHMENTCOMBOMERGESFILENAME);
	if(!ReadInAttachmentComboMergeStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, EXPLOSIVESFILENAME);
	if(!ReadInExplosiveStats(fileName))
		return FALSE;
	
	strcpy(fileName, directoryName);
	strcat(fileName, ARMOURSFILENAME);
	if(!ReadInArmourStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, WEAPONSFILENAME);
	if(!ReadInWeaponStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, INCOMPATIBLEATTACHMENTSFILENAME);
	if(!ReadInIncompatibleAttachmentStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, ENEMYGUNCHOICESFILENAME);
	if(!ReadInExtendedArmyGunChoicesStats (fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, ENEMYITEMCHOICESFILENAME);
	if(!ReadInArmyItemChoicesStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, IMPITEMCHOICESFILENAME);
	if(!ReadInIMPItemChoicesStats(fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, TONYINVENTORYFILENAME);
	if(!ReadInInventoryStats(gTonyInventory,fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, DEVININVENTORYFILENAME);
	if(!ReadInInventoryStats(gDevinInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, FRANZINVENTORYFILENAME);
	if(!ReadInInventoryStats(gFranzInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, KEITHINVENTORYFILENAME);
	if(!ReadInInventoryStats(gKeithInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, SAMINVENTORYFILENAME);
	if(!ReadInInventoryStats(gSamInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, JAKEINVENTORYFILENAME);
	if(!ReadInInventoryStats(gJakeInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, HOWARDINVENTORYFILENAME);
	if(!ReadInInventoryStats(gHowardInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, GABBYINVENTORYFILENAME);
	if(!ReadInInventoryStats(gGabbyInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, FRANKINVENTORYFILENAME);
	if(!ReadInInventoryStats(gFrankInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, ELGININVENTORYFILENAME);
	if(!ReadInInventoryStats(gElginInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, MANNYINVENTORYFILENAME);
	if(!ReadInInventoryStats(gMannyInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, HERVEINVENTORYFILENAME);
	if(!ReadInInventoryStats(gHerveInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, PETERINVENTORYFILENAME);
	if(!ReadInInventoryStats(gPeterInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, ALBERTOINVENTORYFILENAME);
	if(!ReadInInventoryStats(gAlbertoInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, CARLOINVENTORYFILENAME);
	if(!ReadInInventoryStats(gCarloInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, MICKEYINVENTORYFILENAME);
	if(!ReadInInventoryStats(gMickyInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, ARNIEINVENTORYFILENAME);
	if(!ReadInInventoryStats(gArnieInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, PERKOINVENTORYFILENAME);
	if(!ReadInInventoryStats(gPerkoInventory,fileName))
		return FALSE;
	strcpy(fileName, directoryName);
	strcat(fileName, FREDOINVENTORYFILENAME);
	if(!ReadInInventoryStats(gFredoInventory,fileName))
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, CITYTABLEFILENAME);
	if(!ReadInMapStructure(fileName))
		return FALSE;
		
	// Lesh: Strategic movement costs will be read in Strategic\Strategic Movement Costs.cpp,
	//       function BOOLEAN InitStrategicMovementCosts();
	//       It is called several times from various places and acts after clearing SectorInfo array

	// Lesh: load altsectors list
	strcpy(fileName, directoryName);
	strcat(fileName, ALTSECTORSFILENAME);
	if ( !ReadInAltSectors(fileName) )
		return FALSE;

	// Lesh: load samsites - must be after cities.xml
	strcpy(fileName, directoryName);
	strcat(fileName, SAMSITESFILENAME);
	if ( !ReadInSAMInfo(fileName) )
		return FALSE;
	
	// Lesh: army externalization
	strcpy(fileName, directoryName);
	strcat(fileName, GARRISONFILENAME);
	if ( !ReadInGarrisonInfo(fileName) )
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, PATROLFILENAME);
	if ( !ReadInPatrolInfo(fileName) )
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, COMPOSITIONFILENAME);
	if ( !ReadInArmyCompositionInfo(fileName) )
		return FALSE;

	strcpy(fileName, directoryName);
	strcat(fileName, EXPLOSIONDATAFILENAME);
    if(!ReadInExplosionDataStats(fileName))
		return FALSE;

	// Kaiden: Read in Restricted Sectors for Mobile Militia
	strcpy(fileName, directoryName);
	strcat(fileName, ROAMINGMILITIAFILENAME);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("LoadExternalGameplayData, fileName = %s", fileName));
    if(!ReadInRoamingInfo(fileName))
		return FALSE;

	return TRUE;
}


UINT32 InitializeJA2(void)
{ 

#ifdef LASERLOCK_ENABLED
	HandleLaserLockResult( PrepareLaserLockSystem() );
#endif

  HandleJA2CDCheck( );

	gfWorldLoaded = FALSE;

	//Load external game mechanic data
	if ( !LoadExternalGameplayData(TABLEDATA_DIRECTORY))
	{
		return( ERROR_SCREEN );
	}
	
	// Load external text
	LoadAllExternalText();

	// Init JA2 sounds
	InitJA2Sound( );

	gsRenderCenterX = 805;
	gsRenderCenterY = 805;
 

	// Init data
	InitializeSystemVideoObjects( );

	// Init animation system
	if ( !InitAnimationSystem( ) )
	{
		return( ERROR_SCREEN );
	}

	// Init lighting system
	InitLightingSystem();

	// Init dialog queue system
	InitalizeDialogueControl();

	if ( !InitStrategicEngine( ) )
	{
		return( ERROR_SCREEN );
	}
	
	//needs to be called here to init the SectorInfo struct
	if ( !InitStrategicMovementCosts( ) )
	{
		return( ERROR_SCREEN );
	}

	// Init tactical engine
	if ( !InitTacticalEngine( ) )
	{
		return( ERROR_SCREEN );
	}

	// Init timer system
	//Moved to the splash screen code.
	//InitializeJA2Clock( );

	// INit shade tables
	BuildShadeTable( );

	// INit intensity tables
	BuildIntensityTable( );
	
	// Init Event Manager
	if ( !InitializeEventManager( ) )
	{
		return( ERROR_SCREEN );
	}

	// Initailize World
	if ( !InitializeWorld( ) )
	{
		return( ERROR_SCREEN );
	}

	InitTileCache( );

	InitMercPopupBox( );

	// Set global volume
	MusicSetVolume( gGameSettings.ubMusicVolumeSetting );

	DetermineRGBDistributionSettings();

	// Snap: Init save game directory
	if ( !InitSaveDir() )
	{
		return( ERROR_SCREEN );
	}

#ifdef JA2BETAVERSION
	#ifdef JA2EDITOR

	//UNCOMMENT NEXT LINE TO ALLOW FORCE UPDATES...
	//LoadGlobalSummary();
	if( gfMustForceUpdateAllMaps )
	{
		ApologizeOverrideAndForceUpdateEverything();
	}
	#endif
#endif
	
#ifdef JA2BETAVERSION
	if( ProcessIfMultilingualCmdLineArgDetected( gzCommandLine ) )
	{ //If the multilingual text code generator has activated, quit now.
		gfProgramIsRunning = FALSE;
		return( INIT_SCREEN );
	}
#endif

#ifdef JA2BETAVERSION
	// CHECK COMMANDLINE FOR SPECIAL UTILITY
	if ( strcmp( gzCommandLine, "-DOMAPS" ) == 0 )
	{
		return( MAPUTILITY_SCREEN );
	}
#endif

#ifdef JA2BETAVERSION
	//This allows the QuickSave Slots to be autoincremented, ie everytime the user saves, there will be a new quick save file
	if ( _stricmp( gzCommandLine, "-quicksave" ) == 0 )
	{
		gfUseConsecutiveQuickSaveSlots = TRUE;
	}
#endif

#ifdef JA2BETAVERSION
	#ifdef JA2EDITOR
		// CHECK COMMANDLINE FOR SPECIAL UTILITY
		if( !strcmp( gzCommandLine, "-EDITORAUTO" ) )
		{
			OutputDebugString( "Beginning JA2 using -EDITORAUTO commandline argument...\n" );
			//For editor purposes, need to know the default map file.
			sprintf( gubFilename, "none");
			//also set the sector
			gWorldSectorX = 0;
			gWorldSectorY = 0;
			gfAutoLoadA9 = TRUE;
			gfIntendOnEnteringEditor = TRUE;
			gGameOptions.fGunNut = TRUE;
			gGameOptions.fAirStrikes = FALSE;
			return( GAME_SCREEN );
		}
		if ( strcmp( gzCommandLine, "-EDITOR" ) == 0 )
		{
			OutputDebugString( "Beginning JA2 using -EDITOR commandline argument...\n" );
			//For editor purposes, need to know the default map file.
			sprintf( gubFilename, "none");
			//also set the sector
			gWorldSectorX = 0;
			gWorldSectorY = 0;
			gfAutoLoadA9 = FALSE;
			gfIntendOnEnteringEditor = TRUE;
			gGameOptions.fGunNut = TRUE;
			gGameOptions.fAirStrikes = FALSE;
			return( GAME_SCREEN );
		}
	#endif
#endif

	return( INIT_SCREEN );
}


void ShutdownJA2(void)
{ 
  UINT32 uiIndex;

	// Clear screen....
	ColorFillVideoSurfaceArea( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
	InvalidateScreen( );
	// Remove cursor....
	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );

	RefreshScreen( NULL );
		
	ShutdownStrategicLayer();

	// remove temp files built by laptop
	ClearOutTempLaptopFiles( );

	// Shutdown queue system
	ShutdownDialogueControl();

  // Shutdown Screens
  for (uiIndex = 0; uiIndex < MAX_SCREENS; uiIndex++)
  { 
    (*(GameScreens[uiIndex].ShutdownScreen))();
  }


	// Shutdown animation system
	DeInitAnimationSystem( );

	ShutdownLightingSystem();

	CursorDatabaseClear();

	ShutdownTacticalEngine( );

	// Shutdown Overhead
	ShutdownOverhead( );

	DeinitializeWorld( );

	DeleteTileCache( );

	ShutdownJA2Clock( );

	ShutdownFonts();

	ShutdownJA2Sound( );

	ShutdownEventManager( );

	ShutdownBaseDirtyRectQueue( );

	// Unload any text box images!
	RemoveTextMercPopupImages( );

	ClearOutVehicleList();
}


#ifdef LASERLOCK_ENABLED

BOOLEAN PrepareLaserLockSystem()
{
	INT32	iInitRetVal=0;
	INT32	iRunRetVal=0;
	INT32	iCheckRetVal=0;
	CHAR8 zDirectory[512];

	CHAR8		zCdLocation[ SGPFILENAME_LEN ];
	CHAR8		zCdFile[ SGPFILENAME_LEN ];

	//Get the "current" file directory
	GetFileManCurrentDirectory( zDirectory );

	if( GetCDromDriveLetter( zCdLocation ) )
	{
		// OK, build filename
		sprintf( zCdFile, "%s%s", zCdLocation, "Data" );
	}
	else
	{
		goto FAILED_LASERLOK;
	}

	//Go back to the root directory
	SetFileManCurrentDirectory( zCdFile );
	//Init the laser lock system
	iInitRetVal = LASERLOK_Init( ghInstance );
	if( iInitRetVal != 0 )
		goto FAILED_LASERLOK;

	//makes the verification of the laserlok system
	iRunRetVal = LASERLOK_Run();
	if( iRunRetVal != 0 )
		goto FAILED_LASERLOK;

	//checks the result of the laserlok run function
	iCheckRetVal = LASERLOK_Check();
	if( iCheckRetVal != 0 )
		goto FAILED_LASERLOK;
	
	//Restore back to the proper directory
	SetFileManCurrentDirectory( zDirectory );
	return( TRUE );

FAILED_LASERLOK:
	//Restore back to the proper directory
	SetFileManCurrentDirectory( zDirectory );
	return( FALSE );
}

void HandleLaserLockResult( BOOLEAN fSuccess )
{
	if( !fSuccess )
	{
		CHAR8	zString[512];

		sprintf( zString, "%S", gzLateLocalizedString[56] );

//    ShowCursor(TRUE);
//    ShowCursor(TRUE);
		ShutdownWithErrorBox( zString );
	}
}

#endif

//Not sure if this is in the right spot, but it initializes arrays of constants used by Florist.cpp, Florist Cards.cpp, Florist Order Form.cpp, and Florist Gallery.cpp so we don't have to use #defines anymore. Gotthard 1/14/08
void SetupFloristPositionArrays()
{
//These are from florist.cpp
iFloristConstants[0] = FONT12ARIAL;
iFloristConstants[1] = FONT_MCOLOR_WHITE;
iFloristConstants[2] = FONT14ARIAL;
iFloristConstants[3] = FONT_MCOLOR_WHITE;
iFloristConstants[4] = 125;
iFloristConstants[5] = 100;
iFloristConstants[6] = ((SCREEN_WIDTH - 640) / 2) + 224;
iFloristConstants[7] = ((SCREEN_HEIGHT - 480) / 2) + 46;
iFloristConstants[8] = ((SCREEN_WIDTH - 640) / 2) + 306;
iFloristConstants[9] = ((SCREEN_HEIGHT - 480) / 2) + 46;
iFloristConstants[10] = 100;
iFloristConstants[11] = 49;
iFloristConstants[12] = ((SCREEN_WIDTH - 640) / 2) + 116;
iFloristConstants[13] = ((SCREEN_HEIGHT - 480) / 2) + 181;
iFloristConstants[14] = 54;
iFloristConstants[15] = 4;
iFloristConstants[16] = ((SCREEN_WIDTH - 640) / 2) + 321;
iFloristConstants[17] = ((SCREEN_HEIGHT - 480) / 2) + 406;
iFloristConstants[18] = ((SCREEN_WIDTH - 640) / 2) + 164;
iFloristConstants[19] = 136;
iFloristConstants[20] = ((SCREEN_WIDTH - 640) / 2) + 311;
iFloristConstants[21] = 300;
iFloristConstants[22] = ((SCREEN_WIDTH - 640) / 2) + 228;
iFloristConstants[23] = 290;
iFloristConstants[24] = ((SCREEN_HEIGHT - 480) / 2) + 125;
iFloristConstants[25] = ((SCREEN_HEIGHT - 480) / 2) + 140;
iFloristConstants[26] = ((SCREEN_HEIGHT - 480) / 2) + 153;
iFloristConstants[27] = ((SCREEN_HEIGHT - 480) / 2) + 165;
//From the Florist.h file
iFloristConstants[28] = FONT14ARIAL;
iFloristConstants[29] = 2;
iFloristConstants[30] = 2;
iFloristConstants[31] = 128;

//These are from florist order form.cpp
iFloristOrderFormConstants[0] = FONT10ARIAL;
iFloristOrderFormConstants[1] = FONT12ARIAL;
iFloristOrderFormConstants[2] = FONT12ARIAL;
iFloristOrderFormConstants[3] = FONT_MCOLOR_WHITE;
iFloristOrderFormConstants[4] = FONT_MCOLOR_LTYELLOW;
iFloristOrderFormConstants[5] = FONT12ARIAL;
iFloristOrderFormConstants[6] = FONT_MCOLOR_WHITE;
iFloristOrderFormConstants[7] = 76;
iFloristOrderFormConstants[8] = ((SCREEN_WIDTH - 640) / 2) + 118;
iFloristOrderFormConstants[9] = ((SCREEN_HEIGHT - 480) / 2) + 109;
iFloristOrderFormConstants[10] = 75;
iFloristOrderFormConstants[11] = 100;
iFloristOrderFormConstants[12] = ((SCREEN_WIDTH - 640) / 2) + 125;
iFloristOrderFormConstants[13] = ((SCREEN_HEIGHT - 480) / 2) + 272;
iFloristOrderFormConstants[14] = 468;		
iFloristOrderFormConstants[15] = 27;
iFloristOrderFormConstants[16] = ((SCREEN_WIDTH - 640) / 2) + 171;
iFloristOrderFormConstants[17] = ((SCREEN_HEIGHT - 480) / 2) + 333;
iFloristOrderFormConstants[18] = 27;
iFloristOrderFormConstants[19] = ((SCREEN_WIDTH - 640) / 2) + 306;
iFloristOrderFormConstants[20] = ((SCREEN_HEIGHT - 480) / 2) + 189;
iFloristOrderFormConstants[21] = 252;
iFloristOrderFormConstants[22] = 20;
iFloristOrderFormConstants[23] = ((SCREEN_WIDTH - 640) / 2) + 119;
iFloristOrderFormConstants[24] = ((SCREEN_HEIGHT - 480) / 2) + 58;
iFloristOrderFormConstants[25] = ((SCREEN_WIDTH - 640) / 2) + 235;
iFloristOrderFormConstants[26] = ((SCREEN_HEIGHT - 480) / 2) + 410;
iFloristOrderFormConstants[27] = ((SCREEN_WIDTH - 640) / 2) + 326;
iFloristOrderFormConstants[28] = ((SCREEN_HEIGHT - 480) / 2) + 410;
iFloristOrderFormConstants[29] = ((SCREEN_WIDTH - 640) / 2) + 416;
iFloristOrderFormConstants[30] = ((SCREEN_HEIGHT - 480) / 2) + 410;
iFloristOrderFormConstants[31] = ((SCREEN_WIDTH - 640) / 2) + 205;
iFloristOrderFormConstants[32] = ((SCREEN_HEIGHT - 480) / 2) + 114;
iFloristOrderFormConstants[33] = ((SCREEN_WIDTH - 640) / 2) + 205;
iFloristOrderFormConstants[34] = ((SCREEN_HEIGHT - 480) / 2) + 129;
iFloristOrderFormConstants[35] = ((SCREEN_WIDTH - 640) / 2) + 205;
iFloristOrderFormConstants[36] = ((SCREEN_HEIGHT - 480) / 2) + 129;
iFloristOrderFormConstants[37] = ((SCREEN_WIDTH - 640) / 2) + 205;
iFloristOrderFormConstants[38] = ((SCREEN_HEIGHT - 480) / 2) + 172;
iFloristOrderFormConstants[39] = ((SCREEN_WIDTH - 640) / 2) + 205;
iFloristOrderFormConstants[40] = ((SCREEN_HEIGHT - 480) / 2) + 193;
iFloristOrderFormConstants[41] = ((SCREEN_WIDTH - 640) / 2) + 118;
iFloristOrderFormConstants[42] = ((SCREEN_HEIGHT - 480) / 2) + 213;
iFloristOrderFormConstants[43] = ((SCREEN_WIDTH - 640) / 2) + 118;
iFloristOrderFormConstants[44] = ((SCREEN_HEIGHT - 480) / 2) + 258;
iFloristOrderFormConstants[45] = ((SCREEN_WIDTH - 640) / 2) + 130;
iFloristOrderFormConstants[46] = ((SCREEN_HEIGHT - 480) / 2) + 277;
iFloristOrderFormConstants[47] = 457;
iFloristOrderFormConstants[48] = 17;
iFloristOrderFormConstants[49] = ((SCREEN_WIDTH - 640) / 2) + 118;
iFloristOrderFormConstants[50] = ((SCREEN_HEIGHT - 480) / 2) + 315;
iFloristOrderFormConstants[51] = ((SCREEN_WIDTH - 640) / 2) + 118;
iFloristOrderFormConstants[52] = ((SCREEN_HEIGHT - 480) / 2) + 337;
iFloristOrderFormConstants[53] = 50;
iFloristOrderFormConstants[54] = ((SCREEN_WIDTH - 640) / 2) + 174;
iFloristOrderFormConstants[55] = ((SCREEN_HEIGHT - 480) / 2) + 336;
iFloristOrderFormConstants[56] = 257;
iFloristOrderFormConstants[57] = 15;
iFloristOrderFormConstants[58] = 20;
iFloristOrderFormConstants[59] = 17;
iFloristOrderFormConstants[60] = ((SCREEN_WIDTH - 640) / 2) + 297;
iFloristOrderFormConstants[61] = ((SCREEN_HEIGHT - 480) / 2) + 169;
iFloristOrderFormConstants[62] = ((SCREEN_WIDTH - 640) / 2) + 381;
iFloristOrderFormConstants[63] = ((SCREEN_HEIGHT - 480) / 2) + 172;
iFloristOrderFormConstants[64] = ((SCREEN_WIDTH - 640) / 2) + 234;
iFloristOrderFormConstants[65] = ((SCREEN_HEIGHT - 480) / 2) + 213;
iFloristOrderFormConstants[66] = ((SCREEN_WIDTH - 640) / 2) + 380;
iFloristOrderFormConstants[67] = ((SCREEN_HEIGHT - 480) / 2) + 213;
iFloristOrderFormConstants[68] = ((SCREEN_WIDTH - 640) / 2) + 234;
iFloristOrderFormConstants[69] = ((SCREEN_HEIGHT - 480) / 2) + 238;
iFloristOrderFormConstants[70] = ((SCREEN_WIDTH - 640) / 2) + 234;
iFloristOrderFormConstants[71] = ((SCREEN_HEIGHT - 480) / 2) + 238;
iFloristOrderFormConstants[72] = ((SCREEN_WIDTH - 640) / 2) + 301;
iFloristOrderFormConstants[73] = ((SCREEN_HEIGHT - 480) / 2) + 303;
iFloristOrderFormConstants[74] = ((SCREEN_WIDTH - 640) / 2) + 316;
iFloristOrderFormConstants[75] = ((SCREEN_HEIGHT - 480) / 2) + 208;
iFloristOrderFormConstants[76] = 230;
iFloristOrderFormConstants[77] = ((SCREEN_WIDTH - 640) / 2) + 316;
iFloristOrderFormConstants[78] = ((SCREEN_HEIGHT - 480) / 2) + 192;
iFloristOrderFormConstants[79] = 75;
iFloristOrderFormConstants[80] = 35;
iFloristOrderFormConstants[81] = 17;

//These are from Florist Cards.cpp
iFloristCardsConstants[0] = FONT12ARIAL;
iFloristCardsConstants[1] = FONT_MCOLOR_WHITE;
iFloristCardsConstants[2] = ((SCREEN_WIDTH - 640) / 2) + 118;	
iFloristCardsConstants[3] = ((SCREEN_HEIGHT - 480) / 2) + 118;
iFloristCardsConstants[4] = 174;
iFloristCardsConstants[5] = 109;
iFloristCardsConstants[6] = 135;
iFloristCardsConstants[7] = 100;
iFloristCardsConstants[8] = 121;
iFloristCardsConstants[9] = 90;
iFloristCardsConstants[10] = ((SCREEN_WIDTH - 640) / 2) + 111;
iFloristCardsConstants[11] = ((SCREEN_HEIGHT - 480) / 2) + 99;
iFloristCardsConstants[12] = 502;
iFloristCardsConstants[13] = ((SCREEN_WIDTH - 640) / 2) + 119;
iFloristCardsConstants[14] = ((SCREEN_HEIGHT - 480) / 2) + 58;

//These are From Florist Gallery.cpp
iFloristGalleryConstants[0] = FONT10ARIAL;
iFloristGalleryConstants[1] = FONT_MCOLOR_WHITE;
iFloristGalleryConstants[2] = FONT14ARIAL;
iFloristGalleryConstants[3] = FONT_MCOLOR_WHITE;
iFloristGalleryConstants[4] = FONT12ARIAL;
iFloristGalleryConstants[5] = FONT_MCOLOR_WHITE;
iFloristGalleryConstants[6] = FONT12ARIAL;
iFloristGalleryConstants[7] = FONT_MCOLOR_WHITE;
iFloristGalleryConstants[8] = 3;
iFloristGalleryConstants[9] = 10;
iFloristGalleryConstants[10] = FONT12ARIAL;
iFloristGalleryConstants[11] = FONT_MCOLOR_WHITE;
iFloristGalleryConstants[12] = ((SCREEN_WIDTH - 640) / 2) + 119;
iFloristGalleryConstants[13] = ((SCREEN_HEIGHT - 480) / 2) + 58;
iFloristGalleryConstants[14] = ((SCREEN_WIDTH - 640) / 2) + 531;
iFloristGalleryConstants[15] = ((SCREEN_HEIGHT - 480) / 2) + 58;
iFloristGalleryConstants[16] = ((SCREEN_WIDTH - 640) / 2) + 118;
iFloristGalleryConstants[17] = ((SCREEN_HEIGHT - 480) / 2) + 120;
iFloristGalleryConstants[18] = 112;
iFloristGalleryConstants[19] = ((SCREEN_WIDTH - 640) / 2) + 111;
iFloristGalleryConstants[20] = ((SCREEN_HEIGHT - 480) / 2) + 94;
iFloristGalleryConstants[21] = 502;
iFloristGalleryConstants[22] = ((SCREEN_WIDTH - 640) / 2)+232;
iFloristGalleryConstants[23] = 390;
iFloristGalleryConstants[24] = 9;
iFloristGalleryConstants[25] = 26;
iFloristGalleryConstants[26] = 41;
}

//Not sure if this is in the right spot, but it initializes an array of constants used by funeral.cpp to place icons and text on screen. Gotthard 1/13/08
void SetupFuneralPositionArray()
{
iFuneralConstants[0] = FONT12ARIAL;
iFuneralConstants[1] = 2;
iFuneralConstants[2] = FONT_MCOLOR_WHITE;
iFuneralConstants[3] = FONT10ARIAL;
iFuneralConstants[4] = FONT14ARIAL;
iFuneralConstants[5] = FONT_MCOLOR_WHITE;//5
iFuneralConstants[6] = FONT_MCOLOR_DKWHITE;
iFuneralConstants[7] = FONT_MCOLOR_DKWHITE;
iFuneralConstants[8] = ((SCREEN_WIDTH - 640) / 2) + 203;
iFuneralConstants[9] = ((SCREEN_HEIGHT - 480) / 2) + 46;
iFuneralConstants[10] = ((SCREEN_WIDTH - 640) / 2) + 169;//10
iFuneralConstants[11] = ((SCREEN_HEIGHT - 480) / 2) + 89;
iFuneralConstants[12] = ((SCREEN_WIDTH - 640) / 2) + 111 + 0;
iFuneralConstants[13] = ((SCREEN_HEIGHT - 480) / 2) + 89;
iFuneralConstants[14] = ((SCREEN_WIDTH - 640) / 2) + 111 + 442;
iFuneralConstants[15] = ((SCREEN_HEIGHT - 480) / 2) + 89;//15
iFuneralConstants[16] = ((SCREEN_WIDTH - 640) / 2) + 148;
iFuneralConstants[17] = ((SCREEN_HEIGHT - 480) / 2) + 375;
iFuneralConstants[18] = 85;
iFuneralConstants[19] = 60;
iFuneralConstants[20] = 85;//20
iFuneralConstants[21] = 5;
iFuneralConstants[22] = 4;
iFuneralConstants[23] = 17;
iFuneralConstants[24] = 76;
iFuneralConstants[25] = 125;//25
iFuneralConstants[26] = 100;
iFuneralConstants[27] = 380;
iFuneralConstants[28] = ((SCREEN_WIDTH - 640) / 2) + 171;
iFuneralConstants[29] = ((SCREEN_HEIGHT - 480) / 2) + 210;
iFuneralConstants[30] = ((SCREEN_WIDTH - 640) / 2) + 171;//30
iFuneralConstants[31] = ((SCREEN_HEIGHT - 480) / 2) + 244;
iFuneralConstants[32] = ((SCREEN_WIDTH - 640) / 2) + 171;
iFuneralConstants[33] = ((SCREEN_HEIGHT - 480) / 2) + 273;
iFuneralConstants[34] = ((SCREEN_WIDTH - 640) / 2) + 171;
iFuneralConstants[35] = ((SCREEN_HEIGHT - 480) / 2) + 307;//35
iFuneralConstants[36] = ((SCREEN_WIDTH - 640) / 2) + 171;
iFuneralConstants[37] = ((SCREEN_HEIGHT - 480) / 2) + 349;
iFuneralConstants[38] = ((SCREEN_WIDTH - 640) / 2) + 183;
iFuneralConstants[39] = ((SCREEN_HEIGHT - 480) / 2) + 197;
iFuneralConstants[40] = 364;//40
iFuneralConstants[41] = 204;
iFuneralConstants[42] = 260;
iFuneralConstants[43] = ((SCREEN_WIDTH - 640) / 2) + 238;
iFuneralConstants[44] = ((SCREEN_HEIGHT - 480) / 2) + 295;
iFuneralConstants[45] = ((SCREEN_WIDTH - 640) / 2) + 238;//45
iFuneralConstants[46] = ((SCREEN_HEIGHT - 480) / 2) + 359;
return;
}