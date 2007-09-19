#ifdef PRECOMPILEDHEADERS
	#include "ja2_all.h"
	#include "help_screen.h"
	#include "multilingual_text_code_generator.h"
	#include "ini_reader.h"
#else
	#include "sgp.h"
	#include "game_loop.h"
	#include "screens.h"
	#include "wcheck.h"
	#include "ini_reader.h"
	#include "vobject_blitters.h"
	#include "render_world.h"
	#include "mouse_system.h"
	#include "sysutil.h"
	#include "world_def.h"
	#include "animation_data.h"
	#include "overhead.h"
	#include "font_control.h"
	#include "button_system.h"
	#include "timer_control.h"
	#include "utilities.h"
	#include "radar_screen.h"
	#include "render_dirty.h"
	#include "cursors.h"
	#include "sound_control.h"
	#include "event_pump.h"
	#include "lighting.h"
	#include "cursor_control.h"
	#include "music_control.h"
	#include "video.h"
	#include "sys_globals.h"
	#include "map_screen.h"
	#include "interface_items.h"
	#include "map_utility.h"
	#include "strategic.h"
	#include "dialogue_control.h"
	#include "text.h"
	#include "laptop.h"
	#include "npc.h"
	#include "merc_text_box.h"
	#include "tile_cache.h"
	#include "strategic_map.h"
	#include "map_information.h"
	#include "shade_table_util.h"
	#include "exit_grids.h"
	#include "summary_info.h" 
	#include "game_settings.h"
	#include "game_init.h"
	#include "init.h"
	#include "ja_screens.h"
	#include "xml.h"
	#include "save_load_game.h"
	#include "edit_screen.h"
	#include "vehicles.h"
	#include "weapons.h"
	#include "multilingual_text_code_generator.h"
	#include "command_line.h"
	
#endif

//extern BOOLEAN GetCDromDriveLetter( STR8	pString );

// The InitializeGame function is responsible for setting up all data and Gaming Engine
// tasks which will run the game

#ifdef JA2BETAVERSION
extern	BOOLEAN	gfUseConsecutiveQuickSaveSlots;
#endif

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
	printf("Initialize JA2\n");

	gfWorldLoaded = FALSE;

	//Load external game mechanic data
	printf("Loading external data\n");
	if ( !LoadExternalGameplayData(TABLEDATA_DIRECTORY))
	{
		return( ERROR_SCREEN );
	}
	
	// Load external text
	printf("Loading external text\n");
	LoadAllExternalText();

	// Init JA2 sounds
	printf("Initializing JA2 sound\n");
	InitJA2Sound( );

	gsRenderCenterX = 805;
	gsRenderCenterY = 805;
 

	// Init data
	printf("Initializing system video objects\n");
	InitializeSystemVideoObjects( );

	// Init animation system
	printf("Initializing animation system\n");
	if ( !InitAnimationSystem( ) )
	{
		return( ERROR_SCREEN );
	}

	// Init lighting system
	printf("Initializing light system\n");
	InitLightingSystem();

	// Init dialog queue system
	printf("Initializing dialogue control\n");
	InitalizeDialogueControl();

	printf("Initializing strategic engine\n");
	if ( !InitStrategicEngine( ) )
	{
		return( ERROR_SCREEN );
	}
	
	//needs to be called here to init the SectorInfo struct
	printf("Initializing strategic movement costs\n");
	if ( !InitStrategicMovementCosts( ) )
	{
		return( ERROR_SCREEN );
	}

	// Init tactical engine
	printf("Initializing tactical engine\n");
	if ( !InitTacticalEngine( ) )
	{
		return( ERROR_SCREEN );
	}

	// Init timer system
	//Moved to the splash screen code.
	//InitializeJA2Clock( );

	// INit shade tables
	printf("Building shade table\n");
	BuildShadeTable( );

	// INit intensity tables
	printf("Building intensity table\n");
	BuildIntensityTable( );
	
	// Init Event Manager
	printf("Initializing event manager\n");
	if ( !InitializeEventManager( ) )
	{
		return( ERROR_SCREEN );
	}

	// Initailize World
	printf("Initializing world\n");
	if ( !InitializeWorld( ) )
	{
		return( ERROR_SCREEN );
	}

	printf("Initializing tile cache\n");
	InitTileCache( );

	printf("Initializing merc popup box\n");
	InitMercPopupBox( );

	// Set global volume
	MusicSetVolume( gGameSettings.ubMusicVolumeSetting );

	DetermineRGBDistributionSettings();

	// Snap: Init save game directory
	printf("Initializing savegame directory\n");
	if ( !InitSaveDir() )
	{
		return( ERROR_SCREEN );
	}

	printf("Game ready!\n");

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
	// CHECK COMMANDLINE FOR SPECIAL UTILITY
	if ( gCmdLineParams.fDoMaps )
	{
		return( MAPUTILITY_SCREEN );
	}
#endif

#ifdef JA2BETAVERSION
	//This allows the QuickSave Slots to be autoincremented, ie everytime the user saves, there will be a new quick save file
	if ( gCmdLineParams.fQuickSave )
	{
		gfUseConsecutiveQuickSaveSlots = TRUE;
	}
#endif

//#ifdef JA2BETAVERSION
	#ifdef JA2EDITOR
		// CHECK COMMANDLINE FOR SPECIAL UTILITY
		if( gCmdLineParams.fEditorAuto )
		{
//			OutputDebugString( "Beginning JA2 using -EDITORAUTO commandline argument...\n" );
			//For editor purposes, need to know the default map file.
			sprintf( (char *)gubFilename, "none");
			//also set the sector
			gWorldSectorX = 0;
			gWorldSectorY = 0;
			gfAutoLoadA9 = TRUE;
			gfIntendOnEnteringEditor = TRUE;
			gGameOptions.fGunNut = TRUE;
			gGameOptions.fAirStrikes = FALSE;
			return( GAME_SCREEN );
		}
		if ( gCmdLineParams.fEditor )
		{
//			OutputDebugString( "Beginning JA2 using -EDITOR commandline argument...\n" );
			//For editor purposes, need to know the default map file.
			sprintf( (char *)gubFilename, "none");
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
//#endif

	return( INIT_SCREEN );
}


void ShutdownJA2(void)
{ 
  UINT32 uiIndex;

	printf("Closing JA2...\n");
	// Clear screen....
	//ColorFillVideoSurfaceArea( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
	//InvalidateScreen( );
	// Remove cursor....
	SetCurrentCursorFromDatabase( VIDEO_NO_CURSOR );

	RefreshScreen( NULL );
		
	printf("Closing strategic layer\n");
	ShutdownStrategicLayer();

	// remove temp files built by laptop
	printf("Clearing laptop files\n");
	ClearOutTempLaptopFiles( );

	// Shutdown queue system
	printf("Closing dialogue control\n");
	ShutdownDialogueControl();

  // Shutdown Screens
	printf("Closing screens\n");
  	for (uiIndex = 0; uiIndex < MAX_SCREENS; uiIndex++)
  	{ 
    	(*(GameScreens[uiIndex].ShutdownScreen))();
  	}


	// Shutdown animation system
	printf("Closing animation system\n");
	DeInitAnimationSystem( );

	printf("Closing lighting system\n");
	ShutdownLightingSystem();

	CursorDatabaseClear();

	printf("Closing tactical engine\n");
	ShutdownTacticalEngine( );

	// Shutdown Overhead
	printf("Closing overhead\n");
	ShutdownOverhead( );

	printf("Closing world\n");
	DeinitializeWorld( );

	printf("Deleting tile cache\n");
	DeleteTileCache( );

	printf("Closing ja2 clock\n");
	ShutdownJA2Clock( );

	printf("Closing fonts\n");
	ShutdownFonts();

	printf("Closing sounds\n");
	ShutdownJA2Sound( );

	printf("Closing event manager\n");
	ShutdownEventManager( );

	printf("Closing dirty rectangle queue\n");
	ShutdownBaseDirtyRectQueue( );

	// Unload any text box images!
	printf("Closing text merc popup boxes\n");
	RemoveTextMercPopupImages( );

	printf("Clearing vehicle list\n");
	ClearOutVehicleList();
	printf("Closing JA2...done!\n");
}

/*
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

    ShowCursor(TRUE);
    ShowCursor(TRUE);
		ShutdownWithErrorBox( zString );
	}
}

#endif
*/
