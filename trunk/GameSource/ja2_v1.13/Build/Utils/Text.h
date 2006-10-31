#ifndef __TEXT_H
#define __TEXT_H

#include "items.h"
#include "types.h"
#include "mapscreen.h"

#define STRING_LENGTH 255

extern UINT16 ShortItemNames[MAXITEMS][80];
extern UINT16 ItemNames[MAXITEMS][80];
extern UINT16 AmmoCaliber[MAXITEMS][20];
extern UINT16 BobbyRayAmmoCaliber[MAXITEMS][20];
extern UINT16 WeaponType[MAXITEMS][30];

extern UINT16 Message[][STRING_LENGTH];
extern UINT16 TeamTurnString[][STRING_LENGTH];
extern STR16 pMilitiaControlMenuStrings[]; //lal
//extern STR16 pTalkToAllMenuStrings[];
extern STR16 pAssignMenuStrings[];
extern STR16 pTrainingStrings[];
extern STR16 pTrainingMenuStrings[];
extern STR16 pAttributeMenuStrings[];
extern STR16 pVehicleStrings[];
extern STR16 pShortAttributeStrings[];
extern STR16 pLongAttributeStrings[];
extern STR16 pContractStrings[];
extern STR16 pAssignmentStrings[];
extern STR16 pConditionStrings[];
extern CHAR16 pTownNames[MAX_TOWNS][MAX_TOWN_NAME_LENGHT];	// Lesh: look mapscreen.h for definitions
extern STR16 pPersonnelScreenStrings[];
extern STR16 pPersonnelTitle[];
extern STR16 pUpperLeftMapScreenStrings[];
extern STR16 pTacticalPopupButtonStrings[];
extern STR16 pSquadMenuStrings[];
extern STR16 pDoorTrapStrings[];
extern STR16 pLongAssignmentStrings[];
extern STR16 pContractExtendStrings[];
extern STR16 pMapScreenMouseRegionHelpText[];
extern STR16 pPersonnelAssignmentStrings[];
extern STR16 pNoiseVolStr[];
extern STR16 pNoiseTypeStr[];
extern STR16 pDirectionStr[];
extern STR16 pRemoveMercStrings[];
extern STR16 sTimeStrings[];
extern STR16 pLandTypeStrings[];
extern STR16 pGuardMenuStrings[];
extern STR16 pOtherGuardMenuStrings[];
extern STR16 pInvPanelTitleStrings[];
extern STR16 pPOWStrings[];
extern STR16 pMilitiaString[];
extern STR16 pMilitiaButtonString[];
extern STR16 pEpcMenuStrings[];

extern STR16 pRepairStrings[];
extern STR16 sPreStatBuildString[];
extern STR16 sStatGainStrings[];
extern STR16 pHelicopterEtaStrings[];
extern STR16 sMapLevelString[];
extern STR16 gsLoyalString[];
extern STR16 pMapHeliErrorString[];
extern STR16 gsUndergroundString[];
extern STR16 gsTimeStrings[];
extern STR16 sFacilitiesStrings[];
extern STR16 pMapPopUpInventoryText[];
extern STR16 pwTownInfoStrings[];
extern STR16 pwMineStrings[];
extern STR16 pwMiscSectorStrings[];
extern STR16 pMapInventoryErrorString[];
extern STR16 pMapInventoryStrings[];
extern STR16 pMapScreenFastHelpTextList[];
extern STR16 pMovementMenuStrings[];
extern STR16 pUpdateMercStrings[];
extern STR16 pMapScreenBorderButtonHelpText[];
extern STR16 pMapScreenBottomFastHelp[];
extern STR16 pMapScreenBottomText[];
extern STR16 pMercDeadString[];
extern STR16 pSenderNameList[];
extern STR16 pTraverseStrings[];
extern STR16 pNewMailStrings[];
extern STR16 pDeleteMailStrings[];
extern STR16 pEmailHeaders[];
extern STR16 pEmailTitleText[];
extern STR16 pFinanceTitle[];
extern STR16 pFinanceSummary[];
extern STR16 pFinanceHeaders[];
extern STR16 pTransactionText[];
extern STR16 pTransactionAlternateText[];
extern STR16 pMoralStrings[];
extern STR16 pSkyriderText[];
extern STR16 pMercFellAsleepString[];
extern STR16 pLeftEquipmentString[]; 
extern STR16 pMapScreenStatusStrings[];
extern STR16 pMapScreenPrevNextCharButtonHelpText[];
extern STR16 pEtaString[];
extern STR16 pShortVehicleStrings[];
extern STR16 pTrashItemText[];
extern STR16 pMapErrorString[];
extern STR16 pMapPlotStrings[];
extern STR16 pMiscMapScreenMouseRegionHelpText[];
extern STR16 pMercHeLeaveString[];
extern STR16 pMercSheLeaveString[];
extern STR16 pImpPopUpStrings[];
extern STR16 pImpButtonText[];
extern STR16 pExtraIMPStrings[];
extern STR16 pFilesTitle[];
extern STR16 pFilesSenderList[];
extern STR16 pHistoryLocations[];
//extern STR16 pHistoryAlternateStrings[];
extern STR16 pHistoryStrings[];
extern STR16 pHistoryHeaders[];
extern STR16 pHistoryTitle[];
extern STR16 pShowBookmarkString[];
extern STR16 pWebPagesTitles[];
extern STR16 pWebTitle[ ];
extern STR16 pPersonnelString[];
extern STR16 pErrorStrings[];
extern STR16 pDownloadString[];
extern STR16 pBookmarkTitle[];
extern STR16 pBookMarkStrings[];
extern STR16 pLaptopIcons[];
extern STR16 sATMText[ ];
extern STR16 gsAtmStartButtonText[];
extern STR16 gsAtmSideButtonText[];
extern STR16 pDownloadString[];
extern STR16 pPersonnelTeamStatsStrings[];
extern STR16 pPersonnelCurrentTeamStatsStrings[];
extern STR16 pPersonelTeamStrings[];
extern STR16 pPersonnelDepartedStateStrings[];
extern STR16 pMapHortIndex[];
extern STR16 pMapVertIndex[];
extern STR16 pMapDepthIndex[];
//extern STR16 sCritLocationStrings[];
//extern STR16 sVehicleHit[ ];
extern STR16 pLaptopTitles[];
extern STR16 pDayStrings[];
extern STR16 pMercContractOverStrings[];
extern STR16 pMilitiaConfirmStrings[];
extern STR16 pDeliveryLocationStrings[];
extern STR16 pSkillAtZeroWarning[];
extern STR16 pIMPBeginScreenStrings[];
extern STR16 pIMPFinishButtonText[1];
extern STR16 pIMPFinishStrings[];
extern STR16 pIMPVoicesStrings[];
extern STR16 pDepartedMercPortraitStrings[];
extern STR16 pPersTitleText[];
extern STR16 pPausedGameText[];
extern STR16	zOptionsToggleText[];
extern STR16	zOptionsScreenHelpText[];
extern STR16 pDoctorWarningString[];
extern STR16 pMilitiaButtonsHelpText[];
extern STR16 pMapScreenJustStartedHelpText[];
extern STR16 pLandMarkInSectorString[];
extern STR16 gzMercSkillText[];
extern STR16 gzNonPersistantPBIText[];
extern STR16 gzMiscString[];

extern STR16 wMapScreenSortButtonHelpText[];
extern STR16 pNewNoiseStr[];
extern STR16 gzLateLocalizedString[];

extern STR16 gzCWStrings[];

extern STR16 gzTooltipStrings[];

enum
{
	ANTIHACKERSTR_EXITGAME,
};
extern STR16 pAntiHackerString[];

enum
{
	MSG_EXITGAME,
	MSG_OK,
	MSG_YES,
	MSG_NO,
	MSG_CANCEL,
	MSG_REHIRE,
	MSG_LIE,
	MSG_NODESC,
	MSG_SAVESUCCESS,
	MSG_SAVESLOTSUCCESS,
	MSG_QUICKSAVE_NAME,
	MSG_SAVE_NAME,
	MSG_SAVEEXTENSION,
	MSG_SAVEDIRECTORY,
	MSG_DAY,
	MSG_MERCS,
	MSG_EMPTYSLOT,
	MSG_DEMOWORD,
	MSG_DEBUGWORD,
	MSG_RELEASEWORD,
	MSG_RPM,
	MSG_MINUTE_ABBREVIATION,
	MSG_METER_ABBREVIATION,
	MSG_ROUNDS_ABBREVIATION,
	MSG_KILOGRAM_ABBREVIATION,
	MSG_POUND_ABBREVIATION,
	MSG_HOMEPAGE,
	MSG_USDOLLAR_ABBREVIATION,
	MSG_LOWERCASE_NA,
	MSG_MEANWHILE,
	MSG_ARRIVE,
	MSG_VERSION,
	MSG_EMPTY_QUICK_SAVE_SLOT,
	MSG_QUICK_SAVE_RESERVED_FOR_TACTICAL,
	MSG_OPENED,
	MSG_CLOSED,
	MSG_LOWDISKSPACE_WARNING,
	MSG_HIRED_MERC,
	MSG_MERC_CAUGHT_ITEM,
	MSG_MERC_TOOK_DRUG,
	MSG_MERC_HAS_NO_MEDSKILL,
	MSG_INTEGRITY_WARNING,
	MSG_CDROM_SAVE,
	MSG_CANT_FIRE_HERE,
	MSG_CANT_CHANGE_STANCE,
	MSG_DROP,
	MSG_THROW,
	MSG_PASS,
	MSG_ITEM_PASSED_TO_MERC,
	MSG_NO_ROOM_TO_PASS_ITEM,
	MSG_END_ATTACHMENT_LIST,
	MSG_CHEAT_LEVEL_ONE,
	MSG_CHEAT_LEVEL_TWO,
	MSG_SQUAD_ON_STEALTHMODE,
	MSG_SQUAD_OFF_STEALTHMODE,
	MSG_MERC_ON_STEALTHMODE,
	MSG_MERC_OFF_STEALTHMODE,
	MSG_WIREFRAMES_ADDED,
	MSG_WIREFRAMES_REMOVED,
	MSG_CANT_GO_UP,
	MSG_CANT_GO_DOWN,
	MSG_ENTERING_LEVEL,
	MSG_LEAVING_BASEMENT,
	MSG_DASH_S,				// the old 's
	MSG_TACKING_MODE_OFF,
	MSG_TACKING_MODE_ON,
	MSG_3DCURSOR_OFF,
	MSG_3DCURSOR_ON,
	MSG_SQUAD_ACTIVE,
	MSG_CANT_AFFORD_TO_PAY_NPC_DAILY_SALARY_MSG,
	MSG_SKIP,
	MSG_EPC_CANT_TRAVERSE,
	MSG_CDROM_SAVE_GAME,
	MSG_DRANK_SOME,
	MSG_PACKAGE_ARRIVES,
	MSG_JUST_HIRED_MERC_ARRIVAL_LOCATION_POPUP,
	MSG_HISTORY_UPDATED,
	MSG_GL_BURST_CURSOR_ON,
	MSG_GL_BURST_CURSOR_OFF,
	MSG_DROP_ALL_ON,
	MSG_DROP_ALL_OFF,
	MSG_GL_LOW_ANGLE,
	MSG_GL_HIGH_ANGLE,
#ifdef JA2BETAVERSION
	MSG_END_TURN_AUTO_SAVE,
#endif
};
extern STR16 pMessageStrings[];

extern UINT16 ItemPickupHelpPopup[][40];

enum
{
	STR_EMPTY,
	STR_LOSES_1_WISDOM,
	STR_LOSES_1_DEX,
	STR_LOSES_1_STRENGTH,
	STR_LOSES_1_AGIL,
	STR_LOSES_WISDOM,
	STR_LOSES_DEX,
	STR_LOSES_STRENGTH,
	STR_LOSES_AGIL,
	STR_INTERRUPT,
	STR_HEARS_NOISE_FROM,
	STR_PLAYER_REINFORCEMENTS,
	STR_PLAYER_RELOADS,
	STR_PLAYER_NOT_ENOUGH_APS,
	STR_IS_APPLYING_FIRST_AID,
	STR_ARE_APPLYING_FIRST_AID,
	STR_RELIABLE,
	STR_UNRELIABLE,
	STR_EASY_TO_REPAIR,
	STR_HARD_TO_REPAIR,
	STR_HIGH_DAMAGE,
	STR_LOW_DAMAGE,
	STR_QUICK_FIRING,
	STR_SLOW_FIRING,
	STR_LONG_RANGE,
	STR_SHORT_RANGE,
	STR_LIGHT,
	STR_HEAVY,
	STR_SMALL,
	STR_FAST_BURST,
	STR_NO_BURST,
	STR_LARGE_AMMO_CAPACITY,
	STR_SMALL_AMMO_CAPACITY,
	STR_CAMMO_WORN_OFF,
	STR_CAMMO_WASHED_OFF,
	STR_2ND_CLIP_DEPLETED,
	STR_STOLE_SOMETHING,
	STR_NOT_BURST_CAPABLE,
	STR_ATTACHMENT_ALREADY,
	STR_MERGE_ITEMS,
	STR_CANT_ATTACH,
	STR_NONE,
	STR_EJECT_AMMO,
	STR_ATTACHMENTS,
	STR_CANT_USE_TWO_ITEMS,
	STR_ATTACHMENT_HELP,
	STR_ATTACHMENT_INVALID_HELP,
	STR_SECTOR_NOT_CLEARED,
	STR_NEED_TO_GIVE_MONEY,
	STR_HEAD_HIT,
	STR_ABANDON_FIGHT,
	STR_PERMANENT_ATTACHMENT,
	STR_ENERGY_BOOST,
	STR_SLIPPED_MARBLES,
	STR_FAILED_TO_STEAL_SOMETHING,
	STR_REPAIRED,
	STR_INTERRUPT_FOR,
	STR_SURRENDER,
	STR_REFUSE_FIRSTAID,
	STR_REFUSE_FIRSTAID_FOR_CREATURE,
  STR_HOW_TO_USE_SKYRIDDER,
	STR_RELOAD_ONLY_ONE_GUN,
	STR_BLOODCATS_TURN,
	STR_AUTOFIRE,
	STR_NO_AUTOFIRE,
	STR_ACCURATE,
	STR_INACCURATE,
	STR_NO_SEMI_AUTO,
	STR_NO_MORE_ITEMS_TO_STEAL	// WANNE
};

// WANNE: Tooltips
enum
{
	STR_TT_CAT_LOCATION,
	STR_TT_CAT_BRIGHTNESS,
	STR_TT_CAT_RANGE_TO_TARGET,
	STR_TT_CAT_ID,
	STR_TT_CAT_ORDERS,
	STR_TT_CAT_ATTITUDE,
	STR_TT_CAT_CURRENT_APS,
	STR_TT_CAT_CURRENT_HEALTH,
	STR_TT_CAT_HELMET,
	STR_TT_CAT_VEST,
	STR_TT_CAT_LEGGINGS,
	STR_TT_CAT_ARMOR,
	STR_TT_HELMET,
	STR_TT_VEST,
	STR_TT_LEGGINGS,
	STR_TT_WORN,
	STR_TT_NO_ARMOR,
	STR_TT_CAT_NVG,
	STR_TT_NO_NVG,
	STR_TT_CAT_GAS_MASK,
	STR_TT_NO_MASK,
	STR_TT_CAT_HEAD_POS_1,
	STR_TT_CAT_HEAD_POS_2,
	STR_TT_IN_BACKPACK,
	STR_TT_CAT_WEAPON,
	STR_TT_NO_WEAPON,
	STR_TT_HANDGUN,
	STR_TT_SMG,
	STR_TT_RIFLE,
	STR_TT_MG,
	STR_TT_SHOTGUN,
	STR_TT_KNIFE,
	STR_TT_HEAVY_WEAPON,
	STR_TT_NO_HELMET,
	STR_TT_NO_VEST,
	STR_TT_NO_LEGGING,
	STR_TT_CAT_ARMOR_2,
};

#define LARGE_STRING_LENGTH			200
#define MED_STRING_LENGTH				80
#define	SMALL_STRING_LENGTH			20

extern UINT16 TacticalStr[][MED_STRING_LENGTH];
extern UINT16 LargeTacticalStr[][ LARGE_STRING_LENGTH ];


extern UINT16		zDialogActions[][ SMALL_STRING_LENGTH ];
extern UINT16		zDealerStrings[][ SMALL_STRING_LENGTH ];
extern UINT16		zTalkMenuStrings[][ SMALL_STRING_LENGTH ];
extern STR16		gzMoneyAmounts[6];
extern INT16		gzProsLabel[10];
extern INT16		gzConsLabel[10];
extern INT16		gMoneyStatsDesc[][ 13 ];
extern INT16		gWeaponStatsDesc[][ 14 ];
extern STR16		sKeyDescriptionStrings[2];
extern UINT16		zHealthStr[][13];
extern STR16		zVehicleName[ 6 ];

enum
{
	AIR_RAID_TURN_STR,
	BEGIN_AUTOBANDAGE_PROMPT_STR,
	NOTICING_MISSING_ITEMS_FROM_SHIPMENT_STR,
	DOOR_LOCK_DESCRIPTION_STR,
	DOOR_THERE_IS_NO_LOCK_STR,
	DOOR_LOCK_DESTROYED_STR,
	DOOR_LOCK_NOT_DESTROYED_STR,
	DOOR_LOCK_HAS_BEEN_PICKED_STR,
	DOOR_LOCK_HAS_NOT_BEEN_PICKED_STR,
	DOOR_LOCK_UNTRAPPED_STR,
	DOOR_LOCK_HAS_BEEN_UNLOCKED_STR,
	DOOR_NOT_PROPER_KEY_STR,
	DOOR_LOCK_HAS_BEEN_UNTRAPPED_STR,
	DOOR_LOCK_IS_NOT_TRAPPED_STR,
	DOOR_LOCK_HAS_BEEN_LOCKED_STR,
	DOOR_DOOR_MOUSE_DESCRIPTION,
	DOOR_TRAPPED_MOUSE_DESCRIPTION,
	DOOR_LOCKED_MOUSE_DESCRIPTION,
	DOOR_UNLOCKED_MOUSE_DESCRIPTION,
	DOOR_BROKEN_MOUSE_DESCRIPTION,
	ACTIVATE_SWITCH_PROMPT,
	DISARM_TRAP_PROMPT,
	ITEMPOOL_POPUP_PREV_STR,
	ITEMPOOL_POPUP_NEXT_STR,
	ITEMPOOL_POPUP_MORE_STR,
	ITEM_HAS_BEEN_PLACED_ON_GROUND_STR,
	ITEM_HAS_BEEN_GIVEN_TO_STR,
	GUY_HAS_BEEN_PAID_IN_FULL_STR,
	GUY_STILL_OWED_STR,
	CHOOSE_BOMB_FREQUENCY_STR,
	CHOOSE_TIMER_STR,
	CHOOSE_REMOTE_FREQUENCY_STR,
	DISARM_BOOBYTRAP_PROMPT,
	REMOVE_BLUE_FLAG_PROMPT,
	PLACE_BLUE_FLAG_PROMPT,
	ENDING_TURN,
	ATTACK_OWN_GUY_PROMPT,
	VEHICLES_NO_STANCE_CHANGE_STR,
	ROBOT_NO_STANCE_CHANGE_STR,
	CANNOT_STANCE_CHANGE_STR,
	CANNOT_DO_FIRST_AID_STR,
	CANNOT_NO_NEED_FIRST_AID_STR,
	CANT_MOVE_THERE_STR,
	CANNOT_RECRUIT_TEAM_FULL,
	HAS_BEEN_RECRUITED_STR,
	BALANCE_OWED_STR,
	ESCORT_PROMPT,
	HIRE_PROMPT,
	BOXING_PROMPT,
	BUY_VEST_PROMPT,
	NOW_BING_ESCORTED_STR,
	JAMMED_ITEM_STR,
	ROBOT_NEEDS_GIVEN_CALIBER_STR,
	CANNOT_THROW_TO_DEST_STR,
	TOGGLE_STEALTH_MODE_POPUPTEXT,
	MAPSCREEN_POPUPTEXT,
	END_TURN_POPUPTEXT,
	TALK_CURSOR_POPUPTEXT,
	TOGGLE_MUTE_POPUPTEXT,
	CHANGE_STANCE_UP_POPUPTEXT,
	CURSOR_LEVEL_POPUPTEXT,
	JUMPCLIMB_POPUPTEXT,
	CHANGE_STANCE_DOWN_POPUPTEXT,
	EXAMINE_CURSOR_POPUPTEXT,
	PREV_MERC_POPUPTEXT,
	NEXT_MERC_POPUPTEXT,
	CHANGE_OPTIONS_POPUPTEXT,
	TOGGLE_BURSTMODE_POPUPTEXT,
	LOOK_CURSOR_POPUPTEXT,
	MERC_VITAL_STATS_POPUPTEXT,
	CANNOT_DO_INV_STUFF_STR,
	CONTINUE_OVER_FACE_STR,
	MUTE_OFF_STR,
	MUTE_ON_STR,
	DRIVER_POPUPTEXT,
	EXIT_VEHICLE_POPUPTEXT,
	CHANGE_SQUAD_POPUPTEXT,
	DRIVE_POPUPTEXT,
	NOT_APPLICABLE_POPUPTEXT,
	USE_HANDTOHAND_POPUPTEXT,
	USE_FIREARM_POPUPTEXT,
	USE_BLADE_POPUPTEXT ,
	USE_EXPLOSIVE_POPUPTEXT,
	USE_MEDKIT_POPUPTEXT,
	CATCH_STR,
	RELOAD_STR,
	GIVE_STR,
	LOCK_TRAP_HAS_GONE_OFF_STR,
	MERC_HAS_ARRIVED_STR,
	GUY_HAS_RUN_OUT_OF_APS_STR,
	MERC_IS_UNAVAILABLE_STR,
	MERC_IS_ALL_BANDAGED_STR,
	MERC_IS_OUT_OF_BANDAGES_STR,
	ENEMY_IN_SECTOR_STR,
	NO_ENEMIES_IN_SIGHT_STR,
	NOT_ENOUGH_APS_STR,
	NOBODY_USING_REMOTE_STR,
	BURST_FIRE_DEPLETED_CLIP_STR,
	ENEMY_TEAM_MERC_NAME,
	CREATURE_TEAM_MERC_NAME,
	MILITIA_TEAM_MERC_NAME,
	CIV_TEAM_MERC_NAME,
	
	//The text for the 'exiting sector' gui
	EXIT_GUI_TITLE_STR,
	OK_BUTTON_TEXT_STR,
	CANCEL_BUTTON_TEXT_STR,
	EXIT_GUI_SELECTED_MERC_STR,
	EXIT_GUI_ALL_MERCS_IN_SQUAD_STR,
	EXIT_GUI_GOTO_SECTOR_STR,
	EXIT_GUI_GOTO_MAP_STR,
	CANNOT_LEAVE_SECTOR_FROM_SIDE_STR,
	MERC_IS_TOO_FAR_AWAY_STR,
	REMOVING_TREETOPS_STR,
	SHOWING_TREETOPS_STR,
	CROW_HIT_LOCATION_STR,
	NECK_HIT_LOCATION_STR,
	HEAD_HIT_LOCATION_STR,
	TORSO_HIT_LOCATION_STR,
	LEGS_HIT_LOCATION_STR,
	YESNOLIE_STR,
	GUN_GOT_FINGERPRINT,
	GUN_NOGOOD_FINGERPRINT,
	GUN_GOT_TARGET,
	NO_PATH,
	MONEY_BUTTON_HELP_TEXT,
	AUTOBANDAGE_NOT_NEEDED,
	SHORT_JAMMED_GUN,
	CANT_GET_THERE,
	EXCHANGE_PLACES_REQUESTER,
	REFUSE_EXCHANGE_PLACES,
	PAY_MONEY_PROMPT,
	FREE_MEDICAL_PROMPT,
	MARRY_DARYL_PROMPT,
	KEYRING_HELP_TEXT,
	EPC_CANNOT_DO_THAT,
	SPARE_KROTT_PROMPT,
	OUT_OF_RANGE_STRING,
	CIV_TEAM_MINER_NAME,
	VEHICLE_CANT_MOVE_IN_TACTICAL,
	CANT_AUTOBANDAGE_PROMPT,
	NO_PATH_FOR_MERC,
	POW_MERCS_ARE_HERE,
	LOCK_HAS_BEEN_HIT,
	LOCK_HAS_BEEN_DESTROYED,
	DOOR_IS_BUSY,
  VEHICLE_VITAL_STATS_POPUPTEXT,
  NO_LOS_TO_TALK_TARGET,
};

enum{
	EXIT_GUI_LOAD_ADJACENT_SECTOR_HELPTEXT,
	EXIT_GUI_GOTO_MAPSCREEN_HELPTEXT,
	EXIT_GUI_CANT_LEAVE_HOSTILE_SECTOR_HELPTEXT,
	EXIT_GUI_MUST_LOAD_ADJACENT_SECTOR_HELPTEXT,
	EXIT_GUI_MUST_GOTO_MAPSCREEN_HELPTEXT,
	EXIT_GUI_ESCORTED_CHARACTERS_MUST_BE_ESCORTED_HELPTEXT,
	EXIT_GUI_MERC_CANT_ISOLATE_EPC_HELPTEXT_MALE_SINGULAR,
	EXIT_GUI_MERC_CANT_ISOLATE_EPC_HELPTEXT_FEMALE_SINGULAR,
	EXIT_GUI_MERC_CANT_ISOLATE_EPC_HELPTEXT_MALE_PLURAL,
	EXIT_GUI_MERC_CANT_ISOLATE_EPC_HELPTEXT_FEMALE_PLURAL,
	EXIT_GUI_ALL_MERCS_MUST_BE_TOGETHER_TO_ALLOW_HELPTEXT,
	EXIT_GUI_EPC_NOT_ALLOWED_TO_LEAVE_ALONE_HELPTEXT,
	EXIT_GUI_SINGLE_TRAVERSAL_WILL_SEPARATE_SQUADS_HELPTEXT,
	EXIT_GUI_ALL_TRAVERSAL_WILL_MOVE_CURRENT_SQUAD_HELPTEXT,
	EXIT_GUI_ESCORTED_CHARACTERS_CANT_LEAVE_SECTOR_ALONE_STR,
};
extern STR16 pExitingSectorHelpText[];


enum
{
	LARGESTR_NOONE_LEFT_CAPABLE_OF_BATTLE_STR,
	LARGESTR_NOONE_LEFT_CAPABLE_OF_BATTLE_AGAINST_CREATURES_STR,
	LARGESTR_HAVE_BEEN_CAPTURED,
};


//Insurance Contract.c
enum
{
	INS_CONTRACT_PREVIOUS,
	INS_CONTRACT_NEXT,
	INS_CONTRACT_ACCEPT,
	INS_CONTRACT_CLEAR,
};
extern		STR16			InsContractText[];


//Insurance Info
enum
{
	INS_INFO_PREVIOUS,
	INS_INFO_NEXT,
};
extern		STR16		InsInfoText[];

//Merc Account.c
enum
{
	MERC_ACCOUNT_AUTHORIZE,
	MERC_ACCOUNT_HOME,
	MERC_ACCOUNT_ACCOUNT,
	MERC_ACCOUNT_MERC,
	MERC_ACCOUNT_DAYS,
	MERC_ACCOUNT_RATE,
	MERC_ACCOUNT_CHARGE,
	MERC_ACCOUNT_TOTAL,
	MERC_ACCOUNT_AUTHORIZE_CONFIRMATION,
	MERC_ACCOUNT_NOT_ENOUGH_MONEY,
};
extern		STR16			MercAccountText[];

// WANNE: The "Next" and "Prev" button text of the merc account page
extern	STR16 MercAccountPageText[];


//MercFile.c
enum
{
	MERC_FILES_HEALTH,
	MERC_FILES_AGILITY,
	MERC_FILES_DEXTERITY,
	MERC_FILES_STRENGTH,
	MERC_FILES_LEADERSHIP,
	MERC_FILES_WISDOM,
	MERC_FILES_EXPLEVEL,
	MERC_FILES_MARKSMANSHIP,
	MERC_FILES_MECHANICAL,
	MERC_FILES_EXPLOSIVE,
	MERC_FILES_MEDICAL,

	MERC_FILES_PREVIOUS,
	MERC_FILES_HIRE,
	MERC_FILES_NEXT,
	MERC_FILES_ADDITIONAL_INFO,
	MERC_FILES_HOME,
	MERC_FILES_ALREADY_HIRED,			//5
	MERC_FILES_SALARY,
	MERC_FILES_PER_DAY,
	MERC_FILES_MERC_IS_DEAD,

	MERC_FILES_HIRE_TO_MANY_PEOPLE_WARNING,

	MERC_FILES_MERC_UNAVAILABLE,
};
extern		STR16			MercInfo[];


//MercNoAccount.c
enum
{
	MERC_NO_ACC_OPEN_ACCOUNT,
	MERC_NO_ACC_CANCEL,
	MERC_NO_ACC_NO_ACCOUNT_OPEN_ONE,
};
extern		STR16			MercNoAccountText[];



//Merc HomePage
enum
{
	MERC_SPECK_OWNER,
	MERC_OPEN_ACCOUNT,
	MERC_VIEW_ACCOUNT,
	MERC_VIEW_FILES,
	MERC_SPECK_COM,
};
extern		STR16			MercHomePageText[];


//Funerl.c
enum
{
	FUNERAL_INTRO_1,
	FUNERAL_INTRO_2,
	FUNERAL_INTRO_3,
	FUNERAL_INTRO_4,
	FUNERAL_INTRO_5,
	FUNERAL_SEND_FLOWERS,			//5
	FUNERAL_CASKET_URN,
	FUNERAL_CREMATION,
	FUNERAL_PRE_FUNERAL,
	FUNERAL_FUNERAL_ETTIQUETTE,
	FUNERAL_OUR_CONDOLENCES,	//10
	FUNERAL_OUR_SYMPATHIES,
};
extern		STR16			sFuneralString[];


//Florist.c
enum
{
	FLORIST_GALLERY,
	FLORIST_DROP_ANYWHERE,
	FLORIST_PHONE_NUMBER,
	FLORIST_STREET_ADDRESS,
	FLORIST_WWW_ADDRESS,
	FLORIST_ADVERTISEMENT_1,
	FLORIST_ADVERTISEMENT_2,
	FLORIST_ADVERTISEMENT_3,
	FLORIST_ADVERTISEMENT_4,
	FLORIST_ADVERTISEMENT_5,
	FLORIST_ADVERTISEMENT_6,
	FLORIST_ADVERTISEMENT_7,
	FLORIST_ADVERTISEMENT_8,
};
extern		STR16			sFloristText[];


//Florist Order Form
enum
{
	FLORIST_ORDER_BACK,		
	FLORIST_ORDER_SEND,
	FLORIST_ORDER_CLEAR,
	FLORIST_ORDER_GALLERY,
	FLORIST_ORDER_NAME_BOUQUET,
	FLORIST_ORDER_PRICE,					//5
	FLORIST_ORDER_ORDER_NUMBER,
	FLORIST_ORDER_DELIVERY_DATE,
	FLORIST_ORDER_NEXT_DAY,
	FLORIST_ORDER_GETS_THERE,
	FLORIST_ORDER_DELIVERY_LOCATION,			//10
	FLORIST_ORDER_ADDITIONAL_CHARGES,
	FLORIST_ORDER_CRUSHED,
	FLORIST_ORDER_BLACK_ROSES,
	FLORIST_ORDER_WILTED,
	FLORIST_ORDER_FRUIT_CAKE,							//15
	FLORIST_ORDER_PERSONAL_SENTIMENTS,
	FLORIST_ORDER_CARD_LENGTH,
	FLORIST_ORDER_SELECT_FROM_OURS,
	FLORIST_ORDER_STANDARDIZED_CARDS,
	FLORIST_ORDER_BILLING_INFO,							//20
	FLORIST_ORDER_NAME,
};
extern		STR16			sOrderFormText[];



//Florist Gallery.c
enum
{
	FLORIST_GALLERY_PREV,
	FLORIST_GALLERY_NEXT,
	FLORIST_GALLERY_CLICK_TO_ORDER,
	FLORIST_GALLERY_ADDIFTIONAL_FEE,
	FLORIST_GALLERY_HOME,
};
extern		STR16		sFloristGalleryText[];


//Florist Cards
enum
{
	FLORIST_CARDS_CLICK_SELECTION,
	FLORIST_CARDS_BACK,
};
extern		STR16			sFloristCards[];

// Bobbyr Mail Order.c
enum
{
	BOBBYR_ORDER_FORM,
	BOBBYR_QTY,
	BOBBYR_WEIGHT,
	BOBBYR_NAME,
	BOBBYR_UNIT_PRICE,
	BOBBYR_TOTAL,
	BOBBYR_SUB_TOTAL,
	BOBBYR_S_H,
	BOBBYR_GRAND_TOTAL,
	BOBBYR_SHIPPING_LOCATION,
	BOBBYR_SHIPPING_SPEED,
	BOBBYR_COST,
	BOBBYR_OVERNIGHT_EXPRESS,
	BOBBYR_BUSINESS_DAYS,
	BOBBYR_STANDARD_SERVICE,
	BOBBYR_CLEAR_ORDER,
	BOBBYR_ACCEPT_ORDER,
	BOBBYR_BACK,
	BOBBYR_HOME,
	BOBBYR_USED_TEXT,
	BOBBYR_CANT_AFFORD_PURCHASE,
	BOBBYR_SELECT_DEST,
	BOBBYR_CONFIRM_DEST,
	BOBBYR_PACKAGE_WEIGHT,
	BOBBYR_MINIMUM_WEIGHT,
	BOBBYR_GOTOSHIPMENT_PAGE,
};
extern		STR16			BobbyROrderFormText[];

// WANNE
enum
{
	// Guns
	BOBBYR_FILTER_GUNS_HEAVY,
	BOBBYR_FILTER_GUNS_PISTOL,
	BOBBYR_FILTER_GUNS_M_PISTOL,
	BOBBYR_FILTER_GUNS_SMG,
	BOBBYR_FILTER_GUNS_RIFLE,
	BOBBYR_FILTER_GUNS_SN_RIFLE,
	BOBBYR_FILTER_GUNS_AS_RIFLE,
	BOBBYR_FILTER_GUNS_LMG,
	BOBBYR_FILTER_GUNS_SHOTGUN,
	// Ammo
	//BOBBYR_FILTER_AMMO_HEAVY,
	BOBBYR_FILTER_AMMO_PISTOL,
	BOBBYR_FILTER_AMMO_M_PISTOL,
	BOBBYR_FILTER_AMMO_SMG,
	BOBBYR_FILTER_AMMO_RIFLE,
	BOBBYR_FILTER_AMMO_SN_RIFLE,
	BOBBYR_FILTER_AMMO_AS_RIFLE,
	BOBBYR_FILTER_AMMO_LMG,
	BOBBYR_FILTER_AMMO_SHOTGUN,
	// Used
	BOBBYR_FILTER_USED_GUNS,
	//BOBBYR_FILTER_USED_AMMO,
	BOBBYR_FILTER_USED_ARMOR,
	BOBBYR_FILTER_USED_MISC,
};


//BobbyRGuns.c
enum
{
	BOBBYR_GUNS_TO_ORDER,
	BOBBYR_GUNS_CLICK_ON_ITEMS,
	BOBBYR_GUNS_PREVIOUS_ITEMS,
	BOBBYR_GUNS_GUNS,
	BOBBYR_GUNS_AMMO,
	BOBBYR_GUNS_ARMOR,		//5
	BOBBYR_GUNS_MISC,
	BOBBYR_GUNS_USED,
	BOBBYR_GUNS_MORE_ITEMS,
	BOBBYR_GUNS_ORDER_FORM,
	BOBBYR_GUNS_HOME,					//10
	BOBBYR_GUNS_NUM_GUNS_THAT_USE_AMMO_1,
	BOBBYR_GUNS_NUM_GUNS_THAT_USE_AMMO_2,

	BOBBYR_GUNS_WGHT,
	BOBBYR_GUNS_CALIBRE,
	BOBBYR_GUNS_MAGAZINE,
	BOBBYR_GUNS_RANGE,
	BOBBYR_GUNS_DAMAGE,
	BOBBYR_GUNS_ROF,				//5
	BOBBYR_GUNS_COST,
	BOBBYR_GUNS_IN_STOCK,
	BOBBYR_GUNS_QTY_ON_ORDER,
	BOBBYR_GUNS_DAMAGED,
	BOBBYR_GUNS_WEIGHT,				//10
	BOBBYR_GUNS_SUB_TOTAL,
	BOBBYR_GUNS_PERCENT_FUNCTIONAL,

	BOBBYR_MORE_THEN_10_PURCHASES,
	BOBBYR_MORE_NO_MORE_IN_STOCK,
	BOBBYR_NO_MORE_STOCK,

};

extern		STR16			BobbyRText[];

// WANNE
extern		STR16			BobbyRFilter[];


//BobbyR.c
enum
{
	BOBBYR_ADVERTISMENT_1,
	BOBBYR_ADVERTISMENT_2,
	BOBBYR_USED,
	BOBBYR_MISC,
	BOBBYR_GUNS,
	BOBBYR_AMMO,
	BOBBYR_ARMOR,
	BOBBYR_ADVERTISMENT_3,
	BOBBYR_UNDER_CONSTRUCTION,
};
extern		STR16			BobbyRaysFrontText[];

//Aim Sort.c
enum
{
	AIM_AIMMEMBERS,
	SORT_BY,
	PRICE,
	EXPERIENCE,
	AIMMARKSMANSHIP,
	AIMMEDICAL,
	EXPLOSIVES,
	AIMMECHANICAL,
	MUGSHOT_INDEX,
	MERCENARY_FILES,
	ALUMNI_GALLERY,
	ASCENDING,
	DESCENDING
};
extern		STR16			AimSortText[];

//Aim Policies.c
enum
{
	AIM_POLICIES_PREVIOUS,
	AIM_POLICIES_HOMEPAGE,
	AIM_POLICIES_POLICY,
	AIM_POLICIES_NEXT_PAGE,
	AIM_POLICIES_DISAGREE,
	AIM_POLICIES_AGREE,
};
extern		STR16		AimPolicyText[];




//Aim Member.c
enum
{
	AIM_MEMBER_CLICK_INSTRUCTIONS,
};
extern		STR16			AimMemberText[];



//Aim Member.c
enum
{
	AIM_MEMBER_HEALTH,
	AIM_MEMBER_AGILITY,
	AIM_MEMBER_DEXTERITY,
	AIM_MEMBER_STRENGTH,
	AIM_MEMBER_LEADERSHIP,
	AIM_MEMBER_WISDOM,				//5
	AIM_MEMBER_EXP_LEVEL,
	AIM_MEMBER_MARKSMANSHIP,
	AIM_MEMBER_MECHANICAL,
	AIM_MEMBER_EXPLOSIVE,
	AIM_MEMBER_MEDICAL,				//10
	AIM_MEMBER_FEE,
	AIM_MEMBER_CONTRACT,
	AIM_MEMBER_1_DAY,
	AIM_MEMBER_1_WEEK,
	AIM_MEMBER_2_WEEKS,				//15
	AIM_MEMBER_PREVIOUS,
	AIM_MEMBER_CONTACT,
	AIM_MEMBER_NEXT,
	AIM_MEMBER_ADDTNL_INFO,
	AIM_MEMBER_ACTIVE_MEMBERS,	//20
	AIM_MEMBER_OPTIONAL_GEAR,
	AIM_MEMBER_MEDICAL_DEPOSIT_REQ,
};
extern		STR16			CharacterInfo[];



//Aim Member.c
enum
{
	AIM_MEMBER_CONTRACT_CHARGE,
	AIM_MEMBER_ONE_DAY,
	AIM_MEMBER_ONE_WEEK,
	AIM_MEMBER_TWO_WEEKS,
	AIM_MEMBER_NO_EQUIPMENT,
	AIM_MEMBER_BUY_EQUIPMENT,		//5
	AIM_MEMBER_TRANSFER_FUNDS,
	AIM_MEMBER_CANCEL,
	AIM_MEMBER_HIRE,
	AIM_MEMBER_HANG_UP,
	AIM_MEMBER_OK,							//10
	AIM_MEMBER_LEAVE_MESSAGE,
	AIM_MEMBER_VIDEO_CONF_WITH,
	AIM_MEMBER_CONNECTING,
	AIM_MEMBER_WITH_MEDICAL,		//14
};
extern		STR16			VideoConfercingText[];

//Aim Member.c
enum
{
	AIM_MEMBER_FUNDS_TRANSFER_SUCCESFUL,
	AIM_MEMBER_FUNDS_TRANSFER_FAILED,
	AIM_MEMBER_NOT_ENOUGH_FUNDS,

	AIM_MEMBER_ON_ASSIGNMENT,
	AIM_MEMBER_LEAVE_MSG,
	AIM_MEMBER_DEAD,

	AIM_MEMBER_ALREADY_HAVE_20_MERCS,

	AIM_MEMBER_PRERECORDED_MESSAGE,
	AIM_MEMBER_MESSAGE_RECORDED,

};
extern		STR16			AimPopUpText[];

//AIM Link.c
enum
{
	AIM_LINK_TITLE,
};
extern		STR16			AimLinkText[];


//Aim History
enum
{
	AIM_HISTORY_TITLE,
	AIM_HISTORY_PREVIOUS,
	AIM_HISTORY_HOME,
	AIM_HISTORY_AIM_ALUMNI,
	AIM_HISTORY_NEXT,
};
extern		STR16			AimHistoryText[];



//Aim Facial Index
enum
{
	AIM_FI_PRICE,
	AIM_FI_EXP,
	AIM_FI_MARKSMANSHIP,
	AIM_FI_MEDICAL,
	AIM_FI_EXPLOSIVES,
	AIM_FI_MECHANICAL,		
	AIM_FI_AIM_MEMBERS_SORTED_ASCENDING,
	AIM_FI_AIM_MEMBERS_SORTED_DESCENDING,
	AIM_FI_LEFT_CLICK,
	AIM_FI_TO_SELECT,			
	AIM_FI_RIGHT_CLICK,
	AIM_FI_TO_ENTER_SORT_PAGE,
	AIM_FI_AWAY,
	AIM_FI_DEAD,					
};
extern		STR16			AimFiText[];


//AimArchives.
enum
{
	AIM_ALUMNI_PAGE_1,
	AIM_ALUMNI_PAGE_2,
	AIM_ALUMNI_PAGE_3,
	AIM_ALUMNI_ALUMNI,
	AIM_ALUMNI_DONE,
};
extern		STR16			AimAlumniText[];



//Aim Home Page
enum
{
//	AIM_INFO_1,
//	AIM_INFO_2,
//	AIM_POLICIES,
//	AIM_HISTORY,
//	AIM_LINKS,		//5
	AIM_INFO_3,
	AIM_INFO_4,
	AIM_INFO_5,
	AIM_INFO_6,
	AIM_INFO_7,			//9
	AIM_BOBBYR_ADD1,
	AIM_BOBBYR_ADD2,
	AIM_BOBBYR_ADD3,

};

extern		STR16			AimScreenText[];

//Aim Home Page
enum
{
	AIM_HOME,
	AIM_MEMBERS,
	AIM_ALUMNI,
	AIM_POLICIES,
	AIM_HISTORY,
	AIM_LINKS,
};

extern STR16			AimBottomMenuText[];



// MapScreen
enum
{
	MAP_SCREEN_MAP_LEVEL,
	MAP_SCREEN_NO_MILITIA_TEXT,
};
extern	STR16		zMarksMapScreenText[];



//Weapon Name and Description size
#define		ITEMSTRINGFILENAME				"BINARYDATA\\ITEMDESC.EDT"
#define   SIZE_ITEM_NAME						160
#define   SIZE_SHORT_ITEM_NAME			160
#define   SIZE_ITEM_INFO						480
#define   SIZE_ITEM_PROS						160
#define   SIZE_ITEM_CONS						160

BOOLEAN LoadItemInfo(UINT16 ubIndex, STR16 pNameString, STR16 pInfoString );
extern void LoadAllExternalText( void );
BOOLEAN LoadShortNameItemInfo(UINT16 ubIndex, STR16 pNameString );
BOOLEAN LoadItemProsAndCons( UINT16 usIndex, STR16 pProsString, STR16 pConsString );
BOOLEAN LoadBRDesc(UINT16 ubIndex, STR16 pDescString );
BOOLEAN LoadBRName(UINT16 ubIndex, STR16 pNameString );

enum
{
	//Coordinating simultaneous arrival dialog strings
	STR_DETECTED_SIMULTANEOUS_ARRIVAL,
	STR_DETECTED_SINGULAR,
	STR_DETECTED_PLURAL,
	STR_COORDINATE,
	//AutoResove Enemy capturing strings
	STR_ENEMY_SURRENDER_OFFER,
	STR_ENEMY_CAPTURED,
	//AutoResolve Text buttons
	STR_AR_RETREAT_BUTTON,
	STR_AR_DONE_BUTTON,
	//AutoResolve header text
	STR_AR_DEFEND_HEADER,
	STR_AR_ATTACK_HEADER,
	STR_AR_ENCOUNTER_HEADER,
	STR_AR_SECTOR_HEADER,
	//String for AutoResolve battle over conditions
	STR_AR_OVER_VICTORY,
	STR_AR_OVER_DEFEAT,
	STR_AR_OVER_SURRENDERED,
	STR_AR_OVER_CAPTURED,
	STR_AR_OVER_RETREATED,
	STR_AR_MILITIA_NAME,
	STR_AR_ELITE_NAME,
	STR_AR_TROOP_NAME,
	STR_AR_ADMINISTRATOR_NAME,
	STR_AR_CREATURE_NAME,
	STR_AR_TIME_ELAPSED,
	STR_AR_MERC_RETREATED,
	STR_AR_MERC_RETREATING,
	STR_AR_MERC_RETREAT,
	//Strings for prebattle interface
	STR_PB_AUTORESOLVE_BTN,
	STR_PB_GOTOSECTOR_BTN,
	STR_PB_RETREATMERCS_BTN,
	STR_PB_ENEMYENCOUNTER_HEADER,
	STR_PB_ENEMYINVASION_HEADER,
	STR_PB_ENEMYAMBUSH_HEADER,
	STR_PB_ENTERINGENEMYSECTOR_HEADER,
	STR_PB_CREATUREATTACK_HEADER,
	STR_PB_BLOODCATAMBUSH_HEADER,
	STR_PB_ENTERINGBLOODCATLAIR_HEADER,
	STR_PB_LOCATION,
	STR_PB_ENEMIES,
	STR_PB_MERCS,
	STR_PB_MILITIA,
	STR_PB_CREATURES,
	STR_PB_BLOODCATS,
	STR_PB_SECTOR,
	STR_PB_NONE,
	STR_PB_NOTAPPLICABLE_ABBREVIATION,
	STR_PB_DAYS_ABBREVIATION,
	STR_PB_HOURS_ABBREVIATION,
	//Strings for the tactical placement gui
	//The four buttons and it's help text.
	STR_TP_CLEAR,
	STR_TP_SPREAD,
	STR_TP_GROUP,
	STR_TP_DONE,
	STR_TP_CLEARHELP,
	STR_TP_SPREADHELP,
	STR_TP_GROUPHELP,
	STR_TP_DONEHELP,
	STR_TP_DISABLED_DONEHELP,
	//various strings.
	STR_TP_SECTOR,
	STR_TP_CHOOSEENTRYPOSITIONS,
	STR_TP_INACCESSIBLE_MESSAGE,
	STR_TP_INVALID_MESSAGE,
	STR_TP_NAME_HASARRIVEDINSECTOR_XX,
	STR_PB_AUTORESOLVE_FASTHELP,
	STR_PB_DISABLED_AUTORESOLVE_FASTHELP,
	STR_PB_GOTOSECTOR_FASTHELP,
	STR_BP_RETREATSINGLE_FASTHELP,
	STR_BP_RETREATPLURAL_FASTHELP,

	//various popup messages for battle,
	STR_DIALOG_ENEMIES_ATTACK_MILITIA,
	STR_DIALOG_CREATURES_ATTACK_MILITIA,
	STR_DIALOG_CREATURES_KILL_CIVILIANS,
	STR_DIALOG_ENEMIES_ATTACK_UNCONCIOUSMERCS,
	STR_DIALOG_CREATURES_ATTACK_UNCONCIOUSMERCS,
};

//Strings used in conjunction with above enumerations
extern STR16 gpStrategicString[];

enum
{
	STR_GAMECLOCK_DAY_NAME,
};
extern STR16 gpGameClockString[];

//enums for the Shopkeeper Interface
enum
{
	SKI_TEXT_MERCHADISE_IN_STOCK,
	SKI_TEXT_PAGE,
	SKI_TEXT_TOTAL_COST,
	SKI_TEXT_TOTAL_VALUE,
	SKI_TEXT_EVALUATE,
	SKI_TEXT_TRANSACTION,
	SKI_TEXT_DONE,
	SKI_TEXT_REPAIR_COST,
	SKI_TEXT_ONE_HOUR,
	SKI_TEXT_PLURAL_HOURS,
	SKI_TEXT_REPAIRED,
	SKI_TEXT_NO_MORE_ROOM_IN_PLAYER_OFFER_AREA,
	SKI_TEXT_MINUTES,
	SKI_TEXT_DROP_ITEM_TO_GROUND,
};
extern	STR16			SKI_Text[];

//ShopKeeper Interface
enum
{
	SKI_ATM_0,
	SKI_ATM_1,
	SKI_ATM_2,
	SKI_ATM_3,
	SKI_ATM_4,
	SKI_ATM_5,
	SKI_ATM_6,
	SKI_ATM_7,
	SKI_ATM_8,
	SKI_ATM_9,
	SKI_ATM_OK,
	SKI_ATM_TAKE,
	SKI_ATM_GIVE,
	SKI_ATM_CANCEL,
	SKI_ATM_CLEAR,

	NUM_SKI_ATM_BUTTONS
};
extern	STR16	SkiAtmText[];

//ShopKeeper Interface
enum
{
	SKI_ATM_MODE_TEXT_SELECT_MODE,
	SKI_ATM_MODE_TEXT_ENTER_AMOUNT,
	SKI_ATM_MODE_TEXT_SELECT_TO_MERC,
	SKI_ATM_MODE_TEXT_SELECT_FROM_MERC,
	SKI_ATM_MODE_TEXT_SELECT_INUSUFFICIENT_FUNDS,
	SKI_ATM_MODE_TEXT_BALANCE,
};
extern	STR16	gzSkiAtmText[];

//ShopKeeperInterface Message Box defines
enum
{
	SKI_QUESTION_TO_DEDUCT_MONEY_FROM_PLAYERS_ACCOUNT_TO_COVER_DIFFERENCE,
	SKI_SHORT_FUNDS_TEXT,
	SKI_QUESTION_TO_DEDUCT_MONEY_FROM_PLAYERS_ACCOUNT_TO_COVER_COST,

	SKI_TRANSACTION_BUTTON_HELP_TEXT,
	SKI_REPAIR_TRANSACTION_BUTTON_HELP_TEXT,
	SKI_DONE_BUTTON_HELP_TEXT,

	SKI_PLAYERS_CURRENT_BALANCE,
};

extern	STR16	SkiMessageBoxText[];


//enums for the above text
enum
{
	SLG_SAVE_GAME,
	SLG_LOAD_GAME,
	SLG_CANCEL,
	SLG_SAVE_SELECTED,
	SLG_LOAD_SELECTED,
	SLG_SAVE_GAME_OK,				//5
	SLG_SAVE_GAME_ERROR,
	SLG_LOAD_GAME_OK,
	SLG_LOAD_GAME_ERROR,
	SLG_GAME_VERSION_DIF,
	SLG_DELETE_ALL_SAVE_GAMES,	//10
	SLG_SAVED_GAME_VERSION_DIF,
	SLG_BOTH_GAME_AND_SAVED_GAME_DIF,
	SLG_CONFIRM_SAVE,
	SLG_CONFIRM_LOAD,
	SLG_NOT_ENOUGH_HARD_DRIVE_SPACE,		//15
	SLG_SAVING_GAME_MESSAGE,
	SLG_NORMAL_GUNS,
	SLG_ADDITIONAL_GUNS,
	SLG_REALISTIC,
	SLG_SCIFI,		//20

	SLG_DIFF,
	SLG_PLATINUM,

	SLG_BR_QUALITY_TEXT,
	SLG_BR_GOOD_TEXT,
	SLG_BR_GREAT_TEXT,
	SLG_BR_EXCELLENT_TEXT,
	SLG_BR_AWESOME_TEXT,
};
extern	STR16			zSaveLoadText[];



//OptionScreen.h
// defines used for the zOptionsText
enum
{
	OPT_SAVE_GAME,
	OPT_LOAD_GAME,
	OPT_MAIN_MENU,
	OPT_DONE,
	OPT_SOUND_FX,
	OPT_SPEECH,
	OPT_MUSIC,
	OPT_RETURN_TO_MAIN,
	OPT_NEED_AT_LEAST_SPEECH_OR_SUBTITLE_OPTION_ON,
};

extern	STR16	zOptionsText[];


//used with the gMoneyStatsDesc[]
enum
{
	MONEY_DESC_AMOUNT,
	MONEY_DESC_REMAINING,
	MONEY_DESC_AMOUNT_2_SPLIT,
	MONEY_DESC_TO_SPLIT,

	MONEY_DESC_PLAYERS,
	MONEY_DESC_BALANCE,
	MONEY_DESC_AMOUNT_2_WITHDRAW,
	MONEY_DESC_TO_WITHDRAW,

};


// used with gzMoneyWithdrawMessageText
enum
{
	MONEY_TEXT_WITHDRAW_MORE_THEN_MAXIMUM,
	CONFIRMATION_TO_DEPOSIT_MONEY_TO_ACCOUNT,
};



// Game init option screen
enum
{
	GIO_INITIAL_GAME_SETTINGS,

	GIO_GAME_STYLE_TEXT,
	GIO_REALISTIC_TEXT,
	GIO_SCI_FI_TEXT,
	GIO_PLATINUM_TEXT,

	GIO_GUN_OPTIONS_TEXT,
	GIO_GUN_NUT_TEXT,
	GIO_REDUCED_GUNS_TEXT,
	
	GIO_DIF_LEVEL_TEXT,
	GIO_EASY_TEXT,
	GIO_MEDIUM_TEXT,
	GIO_HARD_TEXT,
	GIO_INSANE_TEXT,

	GIO_OK_TEXT,
	GIO_CANCEL_TEXT,

	GIO_GAME_SAVE_STYLE_TEXT,
	GIO_SAVE_ANYWHERE_TEXT,
	GIO_IRON_MAN_TEXT,
	GIO_DISABLED_FOR_THE_DEMO_TEXT,

	GIO_BR_QUALITY_TEXT,
	GIO_BR_GOOD_TEXT,
	GIO_BR_GREAT_TEXT,
	GIO_BR_EXCELLENT_TEXT,
	GIO_BR_AWESOME_TEXT,
};
extern STR16	gzGIOScreenText[];



enum
{
	LAPTOP_BN_HLP_TXT_VIEW_EMAIL,
	LAPTOP_BN_HLP_TXT_BROWSE_VARIOUS_WEB_SITES,
	LAPTOP_BN_HLP_TXT_VIEW_FILES_AND_EMAIL_ATTACHMENTS,
	LAPTOP_BN_HLP_TXT_READ_LOG_OF_EVENTS,
	LAPTOP_BN_HLP_TXT_VIEW_TEAM_INFO,
	LAPTOP_BN_HLP_TXT_VIEW_FINANCIAL_SUMMARY_AND_HISTORY,
	LAPTOP_BN_HLP_TXT_CLOSE_LAPTOP,

	LAPTOP_BN_HLP_TXT_YOU_HAVE_NEW_MAIL,
	LAPTOP_BN_HLP_TXT_YOU_HAVE_NEW_FILE,


	BOOKMARK_TEXT_ASSOCIATION_OF_INTERNATION_MERCENARIES,
	BOOKMARK_TEXT_BOBBY_RAY_ONLINE_WEAPON_MAIL_ORDER,
	BOOKMARK_TEXT_INSTITUTE_OF_MERCENARY_PROFILING,
	BOOKMARK_TEXT_MORE_ECONOMIC_RECRUITING_CENTER,
	BOOKMARK_TEXT_MCGILLICUTTY_MORTUARY,
	BOOKMARK_TEXT_UNITED_FLORAL_SERVICE,
	BOOKMARK_TEXT_INSURANCE_BROKERS_FOR_AIM_CONTRACTS,

};


//enums for the help screen
enum
{
	HLP_SCRN_TXT__EXIT_SCREEN,
};
extern	STR16	gzHelpScreenText[];


extern	STR16 gzLaptopHelpText[];


extern	STR16	gzMoneyWithdrawMessageText[];
extern STR16 gzCopyrightText[];



//enums used for the mapscreen inventory messages
enum
{
	MAPINV_MERC_ISNT_CLOSE_ENOUGH,
	MAPINV_CANT_SELECT_MERC,
	MAPINV_NOT_IN_SECTOR_TO_TAKE,
	MAPINV_CANT_PICKUP_IN_COMBAT,
	MAPINV_CANT_DROP_IN_COMBAT,
	MAPINV_NOT_IN_SECTOR_TO_DROP,
};


//the laptop broken link site
enum
{
	BROKEN_LINK_TXT_ERROR_404,
	BROKEN_LINK_TXT_SITE_NOT_FOUND,
};
extern	STR16		BrokenLinkText[];

//Bobby rays page for recent shipments
enum
{
	BOBBYR_SHIPMENT__TITLE,
	BOBBYR_SHIPMENT__ORDER_NUM,
	BOBBYR_SHIPMENT__NUM_ITEMS,
	BOBBYR_SHIPMENT__ORDERED_ON,
};

extern STR16 gzBobbyRShipmentText[];


enum
{
	GIO_CFS_NOVICE,
	GIO_CFS_EXPERIENCED,
	GIO_CFS_EXPERT,
	GIO_CFS_INSANE,
};
extern STR16 zGioDifConfirmText[];


enum
{
	CRDT_CAMFIELD,
	CRDT_SHAWN,
	CRDT_KRIS,
	CRDT_IAN,
	CRDT_LINDA,
	CRDT_ERIC,
	CRDT_LYNN,
	CRDT_NORM,
	CRDT_GEORGE,
	CRDT_STACEY,
	CRDT_SCOTT,
	CRDT_EMMONS,
	CRDT_DAVE,
	CRDT_ALEX,
	CRDT_JOEY,

	NUM_PEOPLE_IN_CREDITS,
};

STR16	gzCreditNames[];
STR16	gzCreditNameTitle[];
STR16	gzCreditNameFunny[];


extern INT16 * GetWeightUnitString( void );
FLOAT GetWeightBasedOnMetricOption( UINT32 uiObjectWeight );






#endif