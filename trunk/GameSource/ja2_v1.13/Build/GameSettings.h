#ifndef _GAME_SETTINGS__H_
#define _GAME_SETTINGS__H_


//If you add any options, MAKE sure you add the corresponding string to the Options Screen string array
enum
{
	TOPTION_SPEECH,
	TOPTION_MUTE_CONFIRMATIONS,
	TOPTION_SUBTITLES,
	TOPTION_KEY_ADVANCE_SPEECH,
	TOPTION_ANIMATE_SMOKE,
//	TOPTION_HIDE_BULLETS,
//	TOPTION_CONFIRM_MOVE,
	TOPTION_BLOOD_N_GORE,
	TOPTION_DONT_MOVE_MOUSE,
	TOPTION_OLD_SELECTION_METHOD,
	TOPTION_ALWAYS_SHOW_MOVEMENT_PATH,


//	TOPTION_TIME_LIMIT_TURNS,			//moved to the game init screen

	TOPTION_SHOW_MISSES,

	TOPTION_RTCONFIRM,


//	TOPTION_DISPLAY_ENEMY_INDICATOR,		//Displays the number of enemies seen by the merc, ontop of their portrait
	TOPTION_SLEEPWAKE_NOTIFICATION,

	TOPTION_USE_METRIC_SYSTEM,		//If set, uses the metric system

	TOPTION_MERC_ALWAYS_LIGHT_UP,

	TOPTION_SMART_CURSOR,

	TOPTION_SNAP_CURSOR_TO_DOOR,

	TOPTION_GLOW_ITEMS,
	TOPTION_TOGGLE_TREE_TOPS,
	TOPTION_TOGGLE_WIREFRAME,
	TOPTION_3D_CURSOR,
	TOPTION_CTH_CURSOR,
	TOPTION_GL_BURST_CURSOR,
	TOPTION_DROP_ALL,
	NUM_GAME_OPTIONS,				//Toggle up this will be able to be Toggled by the player

	//These options will NOT be toggable by the Player
	TOPTION_MERC_CASTS_LIGHT = NUM_GAME_OPTIONS,
	TOPTION_HIDE_BULLETS,
	TOPTION_TRACKING_MODE,

	NUM_ALL_GAME_OPTIONS,
};	


typedef struct
{
	INT8				bLastSavedGameSlot;							// The last saved game number goes in here	

	UINT8				ubMusicVolumeSetting;
	UINT8				ubSoundEffectsVolume;
	UINT8				ubSpeechVolume;

	//The following are set from the status of the toggle boxes in the Options Screen
	UINT8				fOptions[ NUM_ALL_GAME_OPTIONS ];

	CHAR8				zVersionNumber[14];

	UINT32			uiSettingsVersionNumber;
	UINT32			uiMeanwhileScenesSeenFlags;

	BOOLEAN			fHideHelpInAllScreens;

	BOOLEAN				fUNUSEDPlayerFinishedTheGame; // JA2Gold: for UB compatibility
	UINT8				ubSizeOfDisplayCover;
	UINT8				ubSizeOfLOS;

	UINT8		ubFiller[17];

}	GAME_SETTINGS;



//Enums for the difficulty levels
enum
{
	DIF_LEVEL_ZERO,
	DIF_LEVEL_EASY,
	DIF_LEVEL_MEDIUM,
	DIF_LEVEL_HARD,
	DIF_LEVEL_INSANE,
	DIF_LEVEL_FIVE,
};

//Enums for the difficulty levels
enum
{
	STYLE_REALISTIC = 0, //the specific indices are there to keep compartibility wiht old saves (back when there were only 2 styles)
	STYLE_SCIFI		= 1,
	STYLE_PLATINUM	= 2,
};

enum
{
	BR_GOOD = 1, 
	BR_GREAT = 2,
	BR_EXCELLENT = 4,
	BR_AWESOME = 10,
};

typedef struct
{
	BOOLEAN fGunNut;
	BOOLEAN fAirStrikes;//Madd
	UINT8	ubGameStyle;
	UINT8	ubDifficultyLevel;
	BOOLEAN	fTurnTimeLimit;
	BOOLEAN	fIronManMode;
	UINT8	ubBobbyRay;
	UINT8		ubFiller[7];

} GAME_OPTIONS;

// Snap: Options read from an INI file in the default of custom Data directory
typedef struct
{
	INT32	iStartingCashNovice;
	INT32	iStartingCashExperienced;
	INT32	iStartingCashExpert;
	INT32	iStartingCashInsane;

	BOOLEAN fSellAll;
	INT16 iPriceModifier;

	INT32 iMinAttribute;
	INT32 iMaxAttribute;
	INT32 iImpAttributePoints;
	INT32 iMaxZeroBonus;
	INT32 iStartAttribute;

	INT32 iMaxMilitiaPerSector;
	INT32 iTrainingSquadSize;
	INT32 iMilitiaTrainingCost;
	INT32 iMinLoyaltyToTrain;

	INT32 iMaxEnemyGroupSize;

	INT32 iReinforcementPoolNovice;
	INT32 iReinforcementPoolExperienced;
	INT32 iReinforcementPoolExpert;
	INT32 iReinforcementPoolInsane;

	INT32 iForcePercentageNovice;
	INT32 iForcePercentageExperienced;
	INT32 iForcePercentageExpert;
	INT32 iForcePercentageInsane;

	INT32 iPercentElitesBonusExpert;
	INT32 iPercentElitesBonusInsane;

	UINT8 ubMinEnemyGroupSizeNovice;
	UINT8 ubMinEnemyGroupSizeExperienced;
	UINT8 ubMinEnemyGroupSizeExpert;
	UINT8 ubMinEnemyGroupSizeInsane;

	BOOLEAN fMercDayOne;
	BOOLEAN fAllMercsAvailable;

	BOOLEAN fPers_att;

	INT8 iCustomPersonality;
	INT8 iCustomAttitude;

	INT8 iEasyAPBonus;
	INT8 iExperiencedAPBonus;
	INT8 iExpertAPBonus;
	INT8 iInsaneAPBonus;



} GAME_EXTERNAL_OPTIONS;

//This structure will contain general Ja2 settings  NOT individual game settings.
extern GAME_SETTINGS		gGameSettings;

//This structure will contain the Game options set at the beginning of the game.
extern GAME_OPTIONS		gGameOptions;

// Snap: Options read from an INI file in the default of custom Data directory
extern GAME_EXTERNAL_OPTIONS gGameExternalOptions; 

BOOLEAN	SaveGameSettings();
BOOLEAN LoadGameSettings();
// Snap: Read options from an INI file in the default of custom Data directory
void LoadGameExternalOptions();

void InitGameOptions();


BOOLEAN GetCDLocation( );

void DisplayGameSettings( );

BOOLEAN MeanwhileSceneSeen( UINT8 ubMeanwhile );

BOOLEAN SetMeanwhileSceneSeen( UINT8 ubMeanwhile );

BOOLEAN	CanGameBeSaved();

#endif