// WANNE 2 <changed some lines>
#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include <stdio.h>
	#include <stdarg.h>
	#include <time.h> 
	#include "sgp.h" 
	#include "gameloop.h"
	#include "himage.h"
	#include "vobject.h"
	#include "sysutil.h"
	#include "overhead.h"
	#include "mousesystem.h"
	#include "Button System.h"
	#include "interface.h"
	#include "vsurface.h"
	#include "wcheck.h"
	#include "input.h"
	#include "Handle UI.h"
	#include "renderworld.h"
	#include "sys globals.h"
	#include "cursors.h"
	#include "radar screen.h"
	#include "worldman.h"
	#include "Font Control.h"
	#include "render dirty.h"
	#include "utilities.h"
	#include "Sound Control.h"
	#include "Interface Panels.h"
	#include "Animation Control.h"
	#include "Soldier Control.h"
	#include "pathai.h"
	#include "weapons.h"
	#include "lighting.h"
	#include "faces.h"
	#include "mapscreen.h" 
	#include "message.h"
	#include "text.h"
	#include "Interface Items.h"
	#include "interface control.h"
	#include "interface utils.h"
	#include "game clock.h"
	#include "mapscreen.h"
	#include "Soldier Macros.h"
	#include "strategicmap.h"
	#include "soldier functions.h"
	#include "gamescreen.h"
	#include "Assignments.h"
	#include "points.h"
	#include "Squads.h"
	#include "strategic.h"
	#include "Map Screen Interface Map.h"
	#include "overhead map.h"
	#include "Map Screen Interface.h"
	#include "Options Screen.h"
	#include "ShopKeeper Interface.h"
	#include "english.h"
	#include "keys.h"
	#include "Soldier Add.h"
	#include "vehicles.h"
	#include "gamesettings.h"
	#include "dialogue control.h"
	#include "Items.h"
	#include "drugs and alcohol.h"
	#include "los.h"
	#include "opplist.h"
	#include "vobject_blitters.h"
	#include "Finances.h"
	#include "LaptopSave.h"
	#include "Cursor Control.h"
	#include "MessageBoxScreen.h"
#endif

/* 
 *
 *								SINGLE MERC BAR COORDS
 *
 *
 *  My opinion about this bars and GUI
 *  i would be very happy if we had c++ classes then we could made somethis like modules
 *  and peoples could very easly make their ow panels, button, labels and other 
 *  i have got some ideas but it will tak some time to implement then.
 *  it will be hard and time consuming but maby i will show ya all direction how i would 
 *  do it and peoples will continue that path :) joker
 */

/*	Ok so this array values had a constant initializer INV_INTERFACE_START_Y wich was constant
 *	we need do load SCREEN_WIDTH _HEIGHT from file to make JA more customizable so if we 
 *  will make SCREEN_xxx viariable then xxx_INTERFACE_xxx will be also variable.
 *  When we declare static array like here (below) we need constant initializer but we need dynamic 
 *  array so i declare only array and i will initielize it in run-time.
 *  any questions?
 *  joker 
 */

INV_REGION_DESC gSMInvPocketXY[19];	// ARRAY FOR INV PANEL INTERFACE ITEM POSITIONS
INV_REGION_DESC gSMCamoXY;			// X, Y Location of cammo region

/*  So this coords buttons, bars etc. also ware declered here as constant and we also need them
 *  more customizable in future so i just declare them and i will initialize them later
 *  any questions? joker
 */
int SM_ITEMDESC_START_X;
int SM_ITEMDESC_START_Y;
int SM_ITEMDESC_HEIGHT;
int SM_ITEMDESC_WIDTH;

int SM_SELMERC_AP_X;
int SM_SELMERC_AP_Y;
int SM_SELMERC_AP_HEIGHT;
int SM_SELMERC_AP_WIDTH;
 
int SM_SELMERC_HEALTH_X;
int SM_SELMERC_HEALTH_Y;
int SM_SELMERC_HEALTH_WIDTH;
int SM_SELMERC_HEALTH_HEIGHT;

int SM_SELMERC_BREATH_X;
int SM_SELMERC_BREATH_Y;
int SM_SELMERC_BREATH_WIDTH;
int SM_SELMERC_BREATH_HEIGHT;

int SM_SELMERC_MORALE_X;
int SM_SELMERC_MORALE_Y;
int SM_SELMERC_MORALE_WIDTH;
int SM_SELMERC_MORALE_HEIGHT;

int SM_SELMERC_BARS_TIP_X;
int SM_SELMERC_BARS_TIP_Y;
int SM_SELMERC_BARS_TIP_WIDTH;
int SM_SELMERC_BARS_TIP_HEIGHT;

int SM_SELMERCNAME_X;
int SM_SELMERCNAME_Y;
int SM_SELMERCNAME_WIDTH;
int SM_SELMERCNAME_HEIGHT;

int SM_SELMERC_FACE_X;
int SM_SELMERC_FACE_Y;
int SM_SELMERC_FACE_HEIGHT;
int SM_SELMERC_FACE_WIDTH;

int SM_SELMERC_PLATE_X;
int SM_SELMERC_PLATE_Y;
int SM_SELMERC_PLATE_HEIGHT;
int SM_SELMERC_PLATE_WIDTH;

int SM_BODYINV_X;
int SM_BODYINV_Y;

int STATS_TITLE_FONT_COLOR;
int STATS_TEXT_FONT_COLOR;

int SM_TALKB_X;
int SM_TALKB_Y;
int SM_MUTEB_X;
int SM_MUTEB_Y;
int SM_STANCEUPB_X;
int SM_STANCEUPB_Y;
int SM_UPDOWNB_X;
int SM_UPDOWNB_Y;
int SM_CLIMBB_X;
int SM_CLIMBB_Y;
int SM_STANCEDOWNB_X;
int SM_STANCEDOWNB_Y;
int SM_HANDCURSORB_X;
int SM_HANDCURSORB_Y;
int SM_PREVMERCB_X;
int SM_PREVMERCB_Y;
int SM_NEXTMERCB_X;
int SM_NEXTMERCB_Y;
int SM_OPTIONSB_X;
int SM_OPTIONSB_Y;
int SM_BURSTMODEB_X;
int SM_BURSTMODEB_Y;
int SM_LOOKB_X;
int SM_LOOKB_Y;
int SM_STEALTHMODE_X;
int SM_STEALTHMODE_Y;
int SM_DONE_X;
int SM_DONE_Y;
int SM_MAPSCREEN_X;
int SM_MAPSCREEN_Y;

int SM_POSITIONB_X;
int SM_POSITIONB_Y;
int SM_POSITIONB_WIDTH;
int SM_POSITIONB_HEIGHT;

int SM_PERCENT_WIDTH;
int SM_PERCENT_HEIGHT;
int SM_ARMOR_X;
int SM_ARMOR_Y;
int SM_ARMOR_LABEL_X;
int SM_ARMOR_LABEL_Y;
int SM_ARMOR_PERCENT_X;
int SM_ARMOR_PERCENT_Y;

int SM_WEIGHT_LABEL_X;
int SM_WEIGHT_LABEL_Y;
int SM_WEIGHT_PERCENT_X;
int SM_WEIGHT_PERCENT_Y;
int SM_WEIGHT_X;
int SM_WEIGHT_Y;

int SM_CAMMO_LABEL_X;
int SM_CAMMO_LABEL_Y;
int SM_CAMMO_PERCENT_X;
int SM_CAMMO_PERCENT_Y;
int SM_CAMMO_X;
int SM_CAMMO_Y;

int SM_STATS_WIDTH;
int SM_STATS_HEIGHT;
int SM_AGI_X;
int SM_AGI_Y;
int SM_DEX_X;
int SM_DEX_Y;
int SM_STR_X;
int SM_STR_Y;
int SM_CHAR_X;
int SM_CHAR_Y;
int SM_WIS_X;
int SM_WIS_Y;
int SM_EXPLVL_X;
int SM_EXPLVL_Y;
int SM_MRKM_X;
int SM_MRKM_Y;
int SM_EXPL_X;
int SM_EXPL_Y;
int SM_MECH_X;
int SM_MECH_Y;
int SM_MED_X;
int SM_MED_Y;

int MONEY_X;
int MONEY_Y;
int MONEY_WIDTH;
int MONEY_HEIGHT;

/* 
 *
 *								TEAM BAR COORDS
 * 
 */

/*
 *  Now we have only 6 mercs in team but maby we will make it CUSTOMIZABLE hehe i like that word
 *  why we declare array with 12 items? coz we got 2 valuse x and y maby we can work it out 
 *  and change INT16 to INV_REGION_DESC for ex.
 */
INT16 sTEAMAPPanelXY[12];
INT16 sTEAMFacesXY[12];
INT16 sTEAMNamesXY[12];
INT16 sTEAMFaceHighlXY[12];
INT16 sTEAMLifeXY[12]; 
INT16 sTEAMBreathXY[12];
INT16 sTEAMMoraleXY[12];
INT16 sTEAMApXY[12];
INT16 sTEAMBarsXY[12];
INT16 sTEAMHandInvXY[12];

int TM_FACE_WIDTH;
int TM_FACE_HEIGHT;
int TM_APPANEL_HEIGHT;
int TM_APPANEL_WIDTH;

int TM_ENDTURN_X;
int TM_ENDTURN_Y;
int TM_ROSTERMODE_X;
int TM_ROSTERMODE_Y;
int TM_DISK_X;
int TM_DISK_Y;

int TM_NAME_WIDTH;
int TM_NAME_HEIGHT;
int TM_LIFEBAR_WIDTH;
int TM_LIFEBAR_HEIGHT;
int TM_FACEHIGHTL_WIDTH;
int TM_FACEHIGHTL_HEIGHT;
int TM_AP_HEIGHT;
int TM_AP_WIDTH;

int TM_INV_WIDTH;
int TM_INV_HEIGHT;
int TM_INV_HAND1STARTX;
int TM_INV_HAND1STARTY;
int TM_INV_HAND2STARTX;
int TM_INV_HAND2STARTY;
int TM_INV_HAND_SEP;
int TM_INV_HAND_SEPY;

int TM_BARS_REGION_HEIGHT;
int TM_BARS_REGION_WIDTH;

int INDICATOR_BOX_WIDTH;
int INDICATOR_BOX_HEIGHT;

MOUSE_REGION	gSMPanelRegion;
INT8		gbNewItem[ NUM_INV_SLOTS ];
TEAM_PANEL_SLOTS_TYPE	gTeamPanel[ NUM_TEAM_SLOTS ];
/*  
 *
 *							VARIOUS PANELS COORDS
 *
 *	now its only clock and location name we will also make them ??? 
 *  yes yes yes MORE CUSTOMIZABLE :P for moders 
 *  i will work with radar screen to allow moving it. or maby someone else will do it
 *  any questions? joker
 */
int	INTERFACE_CLOCK_X;
int	INTERFACE_CLOCK_Y;
int	LOCATION_NAME_X;
int	LOCATION_NAME_Y;


typedef enum
{
	STANCEUP_IMAGES = 0,
	UPDOWN_IMAGES,
	CLIMB_IMAGES,
	STANCEDOWN_IMAGES,
	HANDCURSOR_IMAGES,
	PREVMERC_IMAGES,
	NEXTMERC_IMAGES,
	OPTIONS_IMAGES,
//	BURSTMODE_IMAGES,
	LOOK_IMAGES,
	TALK_IMAGES,
	MUTE_IMAGES,
	STANCE_IMAGES,
	DONE_IMAGES,
	MAPSCREEN_IMAGES,
	NUM_SM_BUTTON_IMAGES
};

typedef enum
{
	ENDTURN_IMAGES = 0,
	ROSTERMODE_IMAGES,
	DISK_IMAGES,
	NUM_TEAM_BUTTON_IMAGES
};

INT32		iSMPanelImages		[ NUM_SM_BUTTON_IMAGES ];
INT32		iBurstButtonImages	[ NUM_WEAPON_MODES ];
INT32		iTEAMPanelImages	[ NUM_TEAM_BUTTON_IMAGES ];

INT32		giSMStealthImages	= -1;
INT32		giSMStealthButton	= -1;

BOOLEAN		gfSwitchPanel		= FALSE;
UINT8		gbNewPanel			= SM_PANEL;
UINT8		gubNewPanelParam	= 0;

BOOLEAN		gfUIStanceDifferent						= FALSE;
BOOLEAN		gfAllDisabled							= FALSE;
BOOLEAN		gfSMDisableForItems						= FALSE;
BOOLEAN		gfDisableTacticalPanelButtons			= FALSE;
BOOLEAN		gfAddingMoneyToMercFromPlayersAccount	= FALSE;

BOOLEAN		gfCheckForMouseOverItem				= FALSE;
UINT32		guiMouseOverItemTime				= 0;
INT8		gbCheckForMouseOverItemPos			= 0;
UINT8		gubSelectSMPanelToMerc				= NOBODY;
BOOLEAN		gfReEvaluateDisabledINVPanelButtons = FALSE;

UINT32		guiBrownBackgroundForTeamPanel;
UINT32		guiGoldBackgroundForTeamPanel;

extern	BOOLEAN		gfRerenderInterfaceFromHelpText;
extern	BOOLEAN		gfInKeyRingPopup;
extern	UINT32		guiVEHINV;
extern	INT32		giMapInvDoneButton;
extern	BOOLEAN		gfBeginEndTurn;
extern	SOLDIERTYPE	*gpItemDescSoldier;
extern	BOOLEAN		gfInItemPickupMenu;

extern	void HandleAnyMercInSquadHasCompatibleStuff( UINT8 ubSquad, OBJECTTYPE *pObject, BOOLEAN fReset );
extern	void SetNewItem( SOLDIERTYPE *pSoldier, UINT8 ubInvPos, BOOLEAN fNewItem );
extern	void CleanUpStack( OBJECTTYPE * pObj, OBJECTTYPE * pCursorObj );
extern	BOOLEAN	InternalInitItemDescriptionBox( OBJECTTYPE *pObject, INT16 sX, INT16 sY, UINT8 ubStatusIndex, SOLDIERTYPE *pSoldier );
extern	BOOLEAN InternalHandleCompatibleAmmoUI( SOLDIERTYPE *pSoldier, OBJECTTYPE *pTestObject, BOOLEAN fOn  );

BOOLEAN IsMouseInRegion( MOUSE_REGION *pRegion );
void HandleMouseOverSoldierFaceForContMove( SOLDIERTYPE *pSoldier, BOOLEAN fOn );
void HandlePlayerTeamMemberDeathAfterSkullAnimation( SOLDIERTYPE *pSoldier );
void EnableButtonsForInItemBox( BOOLEAN fDisable );
void ConfirmationToDepositMoneyToPlayersAccount( UINT8 ubExitValue );
void MergeMessageBoxCallBack( UINT8 ubExitValue );

UINT8	gubHandPos;
UINT16	gusOldItemIndex;
UINT16	gusNewItemIndex;
BOOLEAN gfDeductPoints;






INT32	iSMPanelButtons[ NUM_SM_BUTTONS ];
INT32	iTEAMPanelButtons[ NUM_TEAM_BUTTONS ];

// Video Surface for Single Merc Panel
UINT32	guiSMPanel;
UINT32	guiSMObjects;
UINT32	guiSMObjects2;
UINT32	guiSecItemHiddenVO;
UINT32	guiTEAMPanel;
UINT32	guiTEAMObjects;

// Globals for various mouse regions
MOUSE_REGION	gSM_SELMERCPanelRegion;
MOUSE_REGION	gSM_SELMERCBarsRegion;
MOUSE_REGION	gSM_SELMERCMoneyRegion;
MOUSE_REGION	gSM_SELMERCEnemyIndicatorRegion;
MOUSE_REGION	gTEAM_PanelRegion;
MOUSE_REGION	gTEAM_FaceRegions[ 6 ];
MOUSE_REGION	gTEAM_BarsRegions[ 6 ];
MOUSE_REGION	gTEAM_LeftBarsRegions[ 6 ];
MOUSE_REGION	gTEAM_FirstHandInv[ 6 ];
MOUSE_REGION	gTEAM_SecondHandInv[ 6 ];
MOUSE_REGION	gTEAM_EnemyIndicator[ 6 ];

BOOLEAN			gfTEAM_HandInvDispText[ 6 ][ NUM_INV_SLOTS ];
BOOLEAN			gfSM_HandInvDispText[ NUM_INV_SLOTS ];

void HelpTextDoneCallback( void );

// Globals - for one - the current merc here
INT8				gbSMCurStanceObj;
UINT16				gusSMCurrentMerc = 0;
SOLDIERTYPE			*gpSMCurrentMerc = NULL;
extern	INT8		gbCompatibleApplyItem; 
extern	SOLDIERTYPE *gpItemPopupSoldier;

INT8		gbStanceButPos[2][3][3] =
{
	// NON-STEALTH
	16,		14,		15,
	10,		8,		9,
	22,		20,		21,
	// STEALTH MODE
	13,		11,		12,
	7,		5,		6,	
	19,		17,		18
};


// Mouse button and region callbacks
//void BtnPositionCallback( GUI_BUTTON *btn, INT32 reason );
//void BtnMovementCallback( GUI_BUTTON *btn, INT32 reason );
void SelectedMercButtonCallback( MOUSE_REGION * pRegion, INT32 iReason );
void SelectedMercButtonMoveCallback( MOUSE_REGION *pRegion, INT32 iReason );
void SelectedMercEnemyIndicatorCallback( MOUSE_REGION * pRegion, INT32 iReason );
void SMInvMoveCallback( MOUSE_REGION * pRegion, INT32 iReason );
void SMInvClickCallback( MOUSE_REGION * pRegion, INT32 iReason );
void SMInvClickCamoCallback( MOUSE_REGION * pRegion, INT32 iReason );
void SMInvMoveCammoCallback( MOUSE_REGION * pRegion, INT32 iReason );
void SMInvMoneyButtonCallback( MOUSE_REGION * pRegion, INT32 iReason );

// SINGLE MERC PANEL BUTTON CALLBACKS
void BtnStealthModeCallback(GUI_BUTTON *btn,INT32 reason);
void BtnStanceUpCallback(GUI_BUTTON *btn,INT32 reason);
void BtnUpdownCallback(GUI_BUTTON *btn,INT32 reason);
void BtnClimbCallback(GUI_BUTTON *btn,INT32 reason);
void BtnStanceDownCallback(GUI_BUTTON *btn,INT32 reason);
void BtnHandCursorCallback(GUI_BUTTON *btn,INT32 reason);
void BtnTalkCallback(GUI_BUTTON *btn,INT32 reason);
void BtnMuteCallback(GUI_BUTTON *btn,INT32 reason);
void BtnSMDoneCallback(GUI_BUTTON *btn,INT32 reason);
void BtnMapScreenCallback(GUI_BUTTON *btn,INT32 reason);

void BtnPrevMercCallback(GUI_BUTTON *btn,INT32 reason);
void BtnNextMercCallback(GUI_BUTTON *btn,INT32 reason);
void BtnOptionsCallback(GUI_BUTTON *btn,INT32 reason);
void BtnBurstModeCallback(GUI_BUTTON *btn,INT32 reason);
void BtnLookCallback(GUI_BUTTON *btn,INT32 reason);
void BtnPositionShowCallback(GUI_BUTTON *btn,INT32 reason);
void InvPanelButtonClickCallback( MOUSE_REGION * pRegion, INT32 iReason );

// TEAM PANEL BUTTON CALLBACKS
void BtnEndTurnCallback(GUI_BUTTON *btn,INT32 reason);
void BtnRostermodeCallback(GUI_BUTTON *btn,INT32 reason);
void BtnSquadCallback(GUI_BUTTON *btn,INT32 reason);
void MercFacePanelCallback( MOUSE_REGION * pRegion, INT32 iReason );
void MercFacePanelMoveCallback( MOUSE_REGION * pRegion, INT32 iReason );
void TMFirstHandInvCallback( MOUSE_REGION * pRegion, INT32 iReason );
void TMClickFirstHandInvCallback( MOUSE_REGION * pRegion, INT32 iReason );
void TMClickSecondHandInvCallback( MOUSE_REGION * pRegion, INT32 iReason );
void EnemyIndicatorClickCallback( MOUSE_REGION * pRegion, INT32 iReason );

void RenderSoldierTeamInv( SOLDIERTYPE *pSoldier, INT16 sX, INT16 sY, UINT8 ubPanelNum, BOOLEAN fDirty );

// keyring stuff
void KeyRingItemPanelButtonCallback( MOUSE_REGION * pRegion, INT32 iReason );

void UpdateSelectedSoldier( UINT16 usSoldierID, BOOLEAN fSelect );

void CheckForFacePanelStartAnims( SOLDIERTYPE *pSoldier, INT16 sPanelX, INT16 sPanelY );
void HandleSoldierFaceFlash( SOLDIERTYPE *pSoldier, INT16 sFaceX, INT16 sFaceY );
BOOLEAN PlayerExistsInSlot( UINT8 ubID );
void UpdateStatColor( UINT32 uiTimer, BOOLEAN fUpdate );

extern void UpdateItemHatches();

extern SOLDIERTYPE *FindNextActiveSquad( SOLDIERTYPE *pSoldier );

// Wraps up check for AP-s get from a different soldier for in a vehicle...
INT8 GetUIApsToDisplay( SOLDIERTYPE *pSoldier )
{
	SOLDIERTYPE *pVehicle;

	if ( pSoldier->uiStatusFlags & SOLDIER_DRIVER )
	{
		pVehicle = GetSoldierStructureForVehicle( pSoldier->iVehicleId );

		if ( pVehicle != NULL )
		{
			return( pVehicle->bActionPoints );
		}
		else
		{
			return( 0 );
		}
	}
	else
	{
		return ( pSoldier->bActionPoints );
	}
}

void CheckForDisabledForGiveItem( )
{
	INT16			sDist;
	INT16			sDistVisible;
	INT16			sDestGridNo;
	INT8			bDestLevel;
	INT32			cnt;
	SOLDIERTYPE		*pSoldier;
	UINT8			ubSrcSoldier;

	Assert( gpSMCurrentMerc != NULL);

	if ( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
	{
		gfSMDisableForItems = !CanMercInteractWithSelectedShopkeeper( gpSMCurrentMerc );
		return;
	}

	// Default to true
	gfSMDisableForItems = TRUE;

	// ATE: Is the current merc unconscious.....
	if ( gpSMCurrentMerc->bLife < OKLIFE && gpItemPointer != NULL )
	{
		// Go through each merc and see if there is one closeby....
		cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
		for ( pSoldier = MercPtrs[ cnt ]; cnt <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt++,pSoldier++)
		{	
			if ( pSoldier->bActive && pSoldier->bLife >= OKLIFE && !( pSoldier->uiStatusFlags & SOLDIER_VEHICLE ) && !AM_A_ROBOT( pSoldier ) && pSoldier->bInSector && IsMercOnCurrentSquad( pSoldier ) )
			{
				sDist = PythSpacesAway( gpSMCurrentMerc->sGridNo, pSoldier->sGridNo );

				sDistVisible = DistanceVisible( pSoldier, DIRECTION_IRRELEVANT, DIRECTION_IRRELEVANT, gpSMCurrentMerc->sGridNo, gpSMCurrentMerc->bLevel, gpSMCurrentMerc );

				// Check LOS....
				if ( SoldierTo3DLocationLineOfSightTest( pSoldier, gpSMCurrentMerc->sGridNo,  gpSMCurrentMerc->bLevel, 3, (UINT8) sDistVisible, TRUE ) )
				{
					if ( sDist <= PASSING_ITEM_DISTANCE_NOTOKLIFE )
					{
						gfSMDisableForItems = FALSE;
						break;	// found one, no need to keep looking
					}
				}
			}
		}
	}
	else
	{
		ubSrcSoldier = (UINT8)gusSelectedSoldier;

		if ( gpItemPointer != NULL )
		{
			ubSrcSoldier = gpItemPointerSoldier->ubID;
		}

		// OK buddy, check our currently selected merc and disable/enable if not close enough...
		if ( ubSrcSoldier != NOBODY )
		{
			if ( gusSMCurrentMerc != ubSrcSoldier )
			{
				sDestGridNo = MercPtrs[ gusSMCurrentMerc ]->sGridNo;
				bDestLevel	= MercPtrs[ gusSMCurrentMerc ]->bLevel;

				// Get distance....
				sDist = PythSpacesAway( MercPtrs[ ubSrcSoldier ]->sGridNo, sDestGridNo );

				// is he close enough to see that gridno if he turns his head?
				sDistVisible = DistanceVisible( MercPtrs[ ubSrcSoldier ], DIRECTION_IRRELEVANT, DIRECTION_IRRELEVANT, sDestGridNo, bDestLevel, MercPtrs[ ubSrcSoldier ] );

				// Check LOS....
				if ( SoldierTo3DLocationLineOfSightTest( MercPtrs[ ubSrcSoldier ], sDestGridNo,  bDestLevel, 3, (UINT8) sDistVisible, TRUE )  )
				{
					// UNCONSCIOUS GUYS ONLY 1 tile AWAY
					if ( MercPtrs[ gusSMCurrentMerc ]->bLife < CONSCIOUSNESS )
					{
						if ( sDist <= PASSING_ITEM_DISTANCE_NOTOKLIFE )
						{
							gfSMDisableForItems = FALSE;
						}
					}
					else if ( sDist <= PASSING_ITEM_DISTANCE_OKLIFE )
					{
						gfSMDisableForItems = FALSE;
					}
				}
			}
			else
			{
				gfSMDisableForItems = FALSE;
			}
		}
		else
		{
			gfSMDisableForItems = FALSE;
		}
	}
}

void SetSMPanelCurrentMerc( UINT8 ubNewID )
{
	gubSelectSMPanelToMerc = NOBODY;

	gusSMCurrentMerc = ubNewID;

	gpSMCurrentMerc = MercPtrs[ ubNewID ];

	// Set to current guy's interface level
	//if ( gsInterfaceLevel != gpSMCurrentMerc->bUIInterfaceLevel )
	//{
	//	SetRenderFlags(RENDER_FLAG_FULL);
	//	ErasePath(FALSE);
	//	gsInterfaceLevel = gpSMCurrentMerc->bUIInterfaceLevel;
	//}

	// Disable all faces
	SetAllAutoFacesInactive( );

	// Turn off compat ammo....
	if ( gpItemPointer == NULL )
	{
		HandleCompatibleAmmoUI( gpSMCurrentMerc, (INT8)HANDPOS, FALSE );
		gfCheckForMouseOverItem = FALSE;
	}
	else
	{
		// Turn it all false first....
		InternalHandleCompatibleAmmoUI( gpSMCurrentMerc, gpItemPointer, FALSE );
		InternalHandleCompatibleAmmoUI( gpSMCurrentMerc, gpItemPointer, TRUE );
	}

	// Remove item desc panel if one up....
	if ( gfInItemDescBox )
	{
		DeleteItemDescriptionBox( );
	}

	if ( gfInItemPickupMenu )
	{
		gfSMDisableForItems = TRUE;
	}
	else
	{
		if ( ( gpItemPointer != NULL || guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE || gpSMCurrentMerc->bLife < OKLIFE ) )
		{
			CheckForDisabledForGiveItem( );
		}
		else
		{
			gfSMDisableForItems = FALSE;
		}
	}

	if ( gpItemPointer != NULL )
	{
		ReevaluateItemHatches( gpSMCurrentMerc, FALSE );
	}


	DisableInvRegions( gfSMDisableForItems );

	fInterfacePanelDirty = DIRTYLEVEL2;

	gfUIStanceDifferent = TRUE;

	UpdateSMPanel( );

}


void UpdateForContOverPortrait( SOLDIERTYPE *pSoldier, BOOLEAN fOn )
{
	INT32 cnt;

	if ( gsCurInterfacePanel == SM_PANEL )
	{
		if ( gpSMCurrentMerc != NULL )
		{
			// Check if mouse is in region and if so, adjust...
			if ( IsMouseInRegion( &gSM_SELMERCPanelRegion ) )
			{
				HandleMouseOverSoldierFaceForContMove( gpSMCurrentMerc, fOn );
			}
		}
	}
	else
	{
		for ( cnt = 0; cnt < 6; cnt++ )
		{
			if ( gTeamPanel[ cnt ].ubID == pSoldier->ubID )
			{
				if ( IsMouseInRegion( &gTEAM_FaceRegions[ cnt ] ) )
				{
					HandleMouseOverSoldierFaceForContMove( pSoldier, fOn );
				}
			}
		}
	}
}


void UpdateSMPanel( )
{
	BOOLEAN						fNearHeigherLevel;
	BOOLEAN						fNearLowerLevel;
	INT8							bDirection;
	UINT8							ubStanceState;

	if ( gpSMCurrentMerc->sGridNo == NOWHERE )
	{
		return;
	}

	// Stance
	ubStanceState = gpSMCurrentMerc->ubDesiredHeight;

	if ( ubStanceState == NO_DESIRED_HEIGHT )
	{
		ubStanceState = gAnimControl[ gpSMCurrentMerc->usAnimState ].ubEndHeight;
	}


	switch( ubStanceState )
	{
		case ANIM_STAND:

			gbSMCurStanceObj = 0;
			DisableButton( iSMPanelButtons[ STANCEUP_BUTTON ] );
			EnableButton( iSMPanelButtons[ STANCEDOWN_BUTTON ] );

			// Disable if we cannot do this!
			if ( !IsValidStance( gpSMCurrentMerc, ANIM_CROUCH ) )
			{
				DisableButton( iSMPanelButtons[ STANCEDOWN_BUTTON ] );				
			}
			break;

		case ANIM_PRONE:
			gbSMCurStanceObj = 2;
			DisableButton( iSMPanelButtons[ STANCEDOWN_BUTTON ] );
			EnableButton( iSMPanelButtons[ STANCEUP_BUTTON ] );
			break;

		case ANIM_CROUCH:
			gbSMCurStanceObj = 1;
			EnableButton( iSMPanelButtons[ STANCEUP_BUTTON ] );
			EnableButton( iSMPanelButtons[ STANCEDOWN_BUTTON ] );

			// Disable if we cannot do this!
			if ( !IsValidStance( gpSMCurrentMerc, ANIM_PRONE ) )
			{
				DisableButton( iSMPanelButtons[ STANCEDOWN_BUTTON ] );				
			}
			break;
	}
	
	// Stance button done wether we're disabled or not...
	if ( gfUIStanceDifferent )
	{
		//Remove old
		if ( giSMStealthButton != -1 )
		{
				RemoveButton( giSMStealthButton );
		}
		if ( giSMStealthImages != -1 )
		{
			UnloadButtonImage( giSMStealthImages );
		}

		// Make new
		giSMStealthImages = UseLoadedButtonImage( iSMPanelImages[ STANCE_IMAGES  ] ,gbStanceButPos[ gpSMCurrentMerc->bStealthMode ][gbSMCurStanceObj][0] ,gbStanceButPos[ gpSMCurrentMerc->bStealthMode ][gbSMCurStanceObj][1],-1,gbStanceButPos[ gpSMCurrentMerc->bStealthMode ][gbSMCurStanceObj][2],-1 );

		giSMStealthButton = QuickCreateButton( giSMStealthImages, SM_STEALTHMODE_X, SM_STEALTHMODE_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnStealthModeCallback );

		SetButtonFastHelpText( giSMStealthButton, TacticalStr[ TOGGLE_STEALTH_MODE_POPUPTEXT ] );

		gfUIStanceDifferent = FALSE;

		if ( gfAllDisabled )
		{
			if ( giSMStealthButton != -1 )
			{
					DisableButton( giSMStealthButton );
			}
		}
	}

	if ( gfAllDisabled )
	{
		return;
	}


	CheckForReEvaluateDisabledINVPanelButtons( );

	// Check for any newly added items we need.....
	if ( gpSMCurrentMerc->fCheckForNewlyAddedItems )
	{
		// Startup any newly added items....
		CheckForAnyNewlyAddedItems( gpSMCurrentMerc );

		gpSMCurrentMerc->fCheckForNewlyAddedItems = FALSE;
	}



	// Set Disable /Enable UI based on buddy's stats
	if (ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->ImageNum != (UINT32) iBurstButtonImages[ gpSMCurrentMerc->bWeaponMode ] )
	{
		ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->ImageNum = iBurstButtonImages[ gpSMCurrentMerc->bWeaponMode ];
		ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->uiFlags |= BUTTON_DIRTY;
	}

	/*
	if ( gpSMCurrentMerc->bDoBurst )
	{
		if ( !ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
	else
	{
		if ( !ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->uiFlags &= ( ~BUTTON_CLICKED_ON );
		}
	}
	*/


	// Toggle MUTE button...
	if ( gpSMCurrentMerc->uiStatusFlags & SOLDIER_MUTE )
	{
		if ( !ButtonList[ iSMPanelButtons[ MUTE_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ MUTE_BUTTON ] ]->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
	else
	{
		if ( !ButtonList[ iSMPanelButtons[ MUTE_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ MUTE_BUTTON ] ]->uiFlags &= ( ~BUTTON_CLICKED_ON );
		}
	}


	DisableButton( iSMPanelButtons[ CLIMB_BUTTON ] );		

	GetMercClimbDirection( gpSMCurrentMerc->ubID, &fNearLowerLevel, &fNearHeigherLevel );

	if ( fNearLowerLevel || fNearHeigherLevel )
	{
		if ( fNearLowerLevel )
		{
			if ( EnoughPoints( gpSMCurrentMerc, GetAPsToClimbRoof( gpSMCurrentMerc, TRUE ), 0, FALSE ) )
			{
				EnableButton( iSMPanelButtons[ CLIMB_BUTTON ] );		
			}
		}

		if ( fNearHeigherLevel )
		{
			if ( EnoughPoints( gpSMCurrentMerc, GetAPsToClimbRoof( gpSMCurrentMerc, FALSE ), 0, FALSE ) )
			{
				EnableButton( iSMPanelButtons[ CLIMB_BUTTON ] );		
			}
		}
	}
	
	if ( FindFenceJumpDirection( gpSMCurrentMerc, gpSMCurrentMerc->sGridNo, gpSMCurrentMerc->bDirection, &bDirection ) )
	{
		EnableButton( iSMPanelButtons[ CLIMB_BUTTON ] );		
	}

	if ( (gTacticalStatus.ubCurrentTeam != gbPlayerNum) || (gTacticalStatus.uiFlags & REALTIME ) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		DisableButton( iSMPanelButtons[ SM_DONE_BUTTON ] );
	}
	else if ( !gfAllDisabled )
	{
		EnableButton( iSMPanelButtons[ SM_DONE_BUTTON ] );
	}

//	if ( gpSMCurrentMerc->bUIInterfaceLevel > 0 )
	if ( gsInterfaceLevel > 0 )
	{
		if ( !ButtonList[ iSMPanelButtons[ UPDOWN_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ UPDOWN_BUTTON ] ]->uiFlags |= (BUTTON_CLICKED_ON);
		}
	}
	else
	{
		if ( !ButtonList[ iSMPanelButtons[ UPDOWN_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ UPDOWN_BUTTON ] ]->uiFlags &= ( ~BUTTON_CLICKED_ON );
		}
	}

	if ( gCurrentUIMode == HANDCURSOR_MODE )
	{
		if ( !ButtonList[ iSMPanelButtons[ HANDCURSOR_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ HANDCURSOR_BUTTON ] ]->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
	else
	{
		if ( !ButtonList[ iSMPanelButtons[ HANDCURSOR_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ HANDCURSOR_BUTTON ] ]->uiFlags &= ( ~BUTTON_CLICKED_ON );
		}
	}

	if ( gCurrentUIMode == TALKCURSOR_MODE )
	{
		if ( !ButtonList[ iSMPanelButtons[ TALK_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ TALK_BUTTON ] ]->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
	else
	{
		if ( !ButtonList[ iSMPanelButtons[ TALK_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ TALK_BUTTON ] ]->uiFlags &= ( ~BUTTON_CLICKED_ON );
		}
	}

	if ( gCurrentUIMode == LOOKCURSOR_MODE )
	{
		if ( !ButtonList[ iSMPanelButtons[ LOOK_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ LOOK_BUTTON ] ]->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
	else
	{
		if ( !ButtonList[ iSMPanelButtons[ LOOK_BUTTON ] ]->ubToggleButtonActivated )
		{
			ButtonList[ iSMPanelButtons[ LOOK_BUTTON ] ]->uiFlags &= ( ~BUTTON_CLICKED_ON );
		}
	}

	// If not selected ( or dead ), disable/gray some buttons
	if ( gusSelectedSoldier != gpSMCurrentMerc->ubID || ( gpSMCurrentMerc->bLife < OKLIFE ) || (gTacticalStatus.ubCurrentTeam != gbPlayerNum) || gfSMDisableForItems )
	{
		DisableButton( iSMPanelButtons[ CLIMB_BUTTON ] );		
		DisableButton( iSMPanelButtons[ BURSTMODE_BUTTON ] );		
		DisableButton( iSMPanelButtons[ STANCEUP_BUTTON ] );
		DisableButton( iSMPanelButtons[ STANCEDOWN_BUTTON ] );
		DisableButton( iSMPanelButtons[ LOOK_BUTTON ] );
		DisableButton( iSMPanelButtons[ UPDOWN_BUTTON ] );
		DisableButton( iSMPanelButtons[ HANDCURSOR_BUTTON ] );
		if ( giSMStealthButton != -1 )
		{
				DisableButton( giSMStealthButton );
		}
	}
	else
	{
		// Enable some buttons!
		if ( ((IsGunAutofireCapable( gpSMCurrentMerc, HANDPOS ) || IsGunBurstCapable( gpSMCurrentMerc, HANDPOS , FALSE )) && !Weapon[gpSMCurrentMerc->inv[HANDPOS].usItem].NoSemiAuto ) || IsGrenadeLauncherAttached ( &(gpSMCurrentMerc->inv[HANDPOS]) ) )
		{
			EnableButton( iSMPanelButtons[ BURSTMODE_BUTTON ] );		
		}
		else
		{
			DisableButton( iSMPanelButtons[ BURSTMODE_BUTTON ] );		
		}
		EnableButton( iSMPanelButtons[ LOOK_BUTTON ] );
		EnableButton( iSMPanelButtons[ UPDOWN_BUTTON ] );
		EnableButton( iSMPanelButtons[ HANDCURSOR_BUTTON ] );

		if ( giSMStealthButton != -1 )
		{
				EnableButton( giSMStealthButton );
		}
	}

	// CJC Dec 4 2002: or if item pickup menu is up
	if ( (gTacticalStatus.uiFlags & ENGAGED_IN_CONV) || gfInItemPickupMenu)
	{
		DisableButton( iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ] );
	}
	else
	{
		EnableButton( iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ] );
	}

}

extern BOOLEAN CanItemFitInPosition( SOLDIERTYPE *pSoldier, OBJECTTYPE *pObj, INT8 bPos, BOOLEAN fDoingPlacement );
extern INT8		 gbInvalidPlacementSlot[ NUM_INV_SLOTS ]; 


void ReevaluateItemHatches( SOLDIERTYPE *pSoldier, BOOLEAN fAllValid )
{
	INT32	cnt;

	// if there's an item in the cursor and we're not supposed to just make them all valid
	if ( ( gpItemPointer != NULL ) && !fAllValid )
	{
		// check all inventory positions and mark the ones where cursor item won't fit as invalid 
		for ( cnt = 0; cnt < NUM_INV_SLOTS; cnt++ )
		{
			gbInvalidPlacementSlot[ cnt ] = !CanItemFitInPosition( pSoldier, gpItemPointer, (INT8)cnt, FALSE );

			// !!! ATTACHING/MERGING ITEMS IN MAP SCREEN IS NOT SUPPORTED !!!
			// CJC: seems to be supported now...
			//if( guiCurrentScreen != MAP_SCREEN )
			{
				// Check attachments, override to valid placement if valid merge...
				if ( ValidAttachment( gpItemPointer->usItem, pSoldier->inv[ cnt ].usItem ) )
				{
					gbInvalidPlacementSlot[ cnt ] = FALSE;
				}

				if ( ValidMerge( gpItemPointer->usItem, pSoldier->inv[ cnt ].usItem ) )
				{
					gbInvalidPlacementSlot[ cnt ] = FALSE;
				}
			}
		}
	}
	else
	{
		// mark all inventory positions as valid
		for ( cnt = 0; cnt < NUM_INV_SLOTS; cnt++ )
		{
			gbInvalidPlacementSlot[ cnt ] = FALSE;
		}
	}

	fInterfacePanelDirty = DIRTYLEVEL2;
}


void EnableSMPanelButtons( BOOLEAN fEnable , BOOLEAN fFromItemPickup )
{
	if ( fFromItemPickup )
	{
		// If we have the item pointer up...
		// CJC Dec 4 2002: or if item pickup menu is up
		//if ( gpItemPointer != NULL )
		if ( gpItemPointer != NULL || gfInItemPickupMenu )
		{
			DisableTacticalTeamPanelButtons( TRUE );
		}
		else
		{
			DisableTacticalTeamPanelButtons( FALSE );
		}

		fInterfacePanelDirty = DIRTYLEVEL2;
	}


	if ( gsCurInterfacePanel == SM_PANEL )
	{
		if ( fFromItemPickup )
		{
			// If we have the item pointer up...
			if ( gpItemPointer != NULL )
			{
				ReevaluateItemHatches( gpSMCurrentMerc, fEnable );

				// Turn it all false first....
				InternalHandleCompatibleAmmoUI( gpSMCurrentMerc, gpItemPointer, FALSE );
				InternalHandleCompatibleAmmoUI( gpSMCurrentMerc, gpItemPointer, TRUE );

				gfCheckForMouseOverItem = FALSE;

				// Highlight guys ....
				HandleAnyMercInSquadHasCompatibleStuff( (INT8)CurrentSquad( ), gpItemPointer, FALSE );

			}
			else
			{
				//InternalHandleCompatibleAmmoUI( gpSMCurrentMerc, gpItemPointer, FALSE );
				gfCheckForMouseOverItem = FALSE;

				HandleAnyMercInSquadHasCompatibleStuff( (INT8) CurrentSquad( ), NULL, TRUE );
			}

			if ( fEnable )
			{
				ReevaluateItemHatches( gpSMCurrentMerc, fEnable );
			}

			fInterfacePanelDirty = DIRTYLEVEL2;
		}

		if ( fEnable )
		{
			// only enable the following if NOT in shopkeeper's interface
			if ( !(guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE ) )
			{
				EnableButton( iSMPanelButtons[ CLIMB_BUTTON ] );		
				EnableButton( iSMPanelButtons[ BURSTMODE_BUTTON ] );		
				EnableButton( iSMPanelButtons[ STANCEUP_BUTTON ] );
				EnableButton( iSMPanelButtons[ STANCEDOWN_BUTTON ] );
				EnableButton( iSMPanelButtons[ LOOK_BUTTON ] );
				EnableButton( iSMPanelButtons[ UPDOWN_BUTTON ] );
				EnableButton( iSMPanelButtons[ HANDCURSOR_BUTTON ] );
				if ( giSMStealthButton != -1 )
				{
					EnableButton( giSMStealthButton );
				}

				if ( gfDisableTacticalPanelButtons )
				{
					DisableButton( iSMPanelButtons[ OPTIONS_BUTTON ] );
					DisableButton( iSMPanelButtons[ SM_DONE_BUTTON ] );
					DisableButton( iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ] );
				}
				else
				{
					EnableButton( iSMPanelButtons[ OPTIONS_BUTTON ] );
					EnableButton( iSMPanelButtons[ SM_DONE_BUTTON ] );
					EnableButton( iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ] );
				}


				//enable the radar map region
				MSYS_EnableRegion(&gRadarRegion);

				gfSMDisableForItems = FALSE;

      	DisableInvRegions( gfSMDisableForItems );
			}

			if ( !fFromItemPickup )
			{
				EnableButton( iSMPanelButtons[ NEXTMERC_BUTTON ] );
				EnableButton( iSMPanelButtons[ PREVMERC_BUTTON ] );
			}
		}
		else
		{
			DisableButton( iSMPanelButtons[ CLIMB_BUTTON ] );		
			DisableButton( iSMPanelButtons[ BURSTMODE_BUTTON ] );		
			DisableButton( iSMPanelButtons[ STANCEUP_BUTTON ] );
			DisableButton( iSMPanelButtons[ STANCEDOWN_BUTTON ] );
			DisableButton( iSMPanelButtons[ LOOK_BUTTON ] );
			DisableButton( iSMPanelButtons[ UPDOWN_BUTTON ] );
			DisableButton( iSMPanelButtons[ HANDCURSOR_BUTTON ] );
			if ( giSMStealthButton != -1 )
			{
					DisableButton( giSMStealthButton );
			}

			if ( !fFromItemPickup )
			{
				DisableButton( iSMPanelButtons[ NEXTMERC_BUTTON ] );
				DisableButton( iSMPanelButtons[ PREVMERC_BUTTON ] );
			}

			DisableButton( iSMPanelButtons[ OPTIONS_BUTTON ] );
			DisableButton( iSMPanelButtons[ SM_DONE_BUTTON ] );
			DisableButton( iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ] );

			//disable the radar map
			MSYS_DisableRegion(&gRadarRegion);
		}

		gfAllDisabled = !fEnable;

	}
}

UINT16 GetSMPanelCurrentMerc( )
{
	return( gusSMCurrentMerc );
}


BOOLEAN InitializeSMPanelCoords( )
{
	// int i; // unused (jonathanl)
	/* 
	 * now i use standard positions and i add to them starting coords of panel but 
	 * we can add here function that will read coord in panel from file
	 * that will be phase2 of making GUI more customizable
	 * any questions? joker
	 */
	// Inventory slots
	gSMInvPocketXY[0].sX = INTERFACE_START_X + 344;		gSMInvPocketXY[0].sY = INV_INTERFACE_START_Y + 7;	// HELMETPOS
	gSMInvPocketXY[1].sX = INTERFACE_START_X + 344;		gSMInvPocketXY[1].sY = INV_INTERFACE_START_Y + 36;	// VESTPOS
	gSMInvPocketXY[2].sX = INTERFACE_START_X + 344;		gSMInvPocketXY[2].sY = INV_INTERFACE_START_Y + 96;	// LEGPOS,
	gSMInvPocketXY[3].sX = INTERFACE_START_X + 226;		gSMInvPocketXY[3].sY = INV_INTERFACE_START_Y + 7;	// HEAD1POS
	gSMInvPocketXY[4].sX = INTERFACE_START_X + 226;		gSMInvPocketXY[4].sY = INV_INTERFACE_START_Y + 31;	// HEAD2POS
	gSMInvPocketXY[5].sX = INTERFACE_START_X + 226;		gSMInvPocketXY[5].sY = INV_INTERFACE_START_Y + 84;	// HANDPOS,
	gSMInvPocketXY[6].sX = INTERFACE_START_X + 226;		gSMInvPocketXY[6].sY = INV_INTERFACE_START_Y + 108;	// SECONDHANDPOS
	gSMInvPocketXY[7].sX = INTERFACE_START_X + 468;		gSMInvPocketXY[7].sY = INV_INTERFACE_START_Y + 6;	// BIGPOCK1
	gSMInvPocketXY[8].sX = INTERFACE_START_X + 468;		gSMInvPocketXY[8].sY = INV_INTERFACE_START_Y + 30;	// BIGPOCK2
	gSMInvPocketXY[9].sX = INTERFACE_START_X + 468;		gSMInvPocketXY[9].sY = INV_INTERFACE_START_Y + 54;	// BIGPOCK3
	gSMInvPocketXY[10].sX = INTERFACE_START_X + 468;	gSMInvPocketXY[10].sY = INV_INTERFACE_START_Y + 78;	// BIGPOCK4
	gSMInvPocketXY[11].sX = INTERFACE_START_X + 396;	gSMInvPocketXY[11].sY = INV_INTERFACE_START_Y + 6;	// SMALLPOCK1
	gSMInvPocketXY[12].sX = INTERFACE_START_X + 396;	gSMInvPocketXY[12].sY = INV_INTERFACE_START_Y + 30;	// SMALLPOCK2
	gSMInvPocketXY[13].sX = INTERFACE_START_X + 396;	gSMInvPocketXY[13].sY = INV_INTERFACE_START_Y + 54;	// SMALLPOCK3
	gSMInvPocketXY[14].sX = INTERFACE_START_X + 396;	gSMInvPocketXY[14].sY = INV_INTERFACE_START_Y + 78;	// SMALLPOCK4
	gSMInvPocketXY[15].sX = INTERFACE_START_X + 432;	gSMInvPocketXY[15].sY = INV_INTERFACE_START_Y + 6;	// SMALLPOCK5
	gSMInvPocketXY[16].sX = INTERFACE_START_X + 432;	gSMInvPocketXY[16].sY = INV_INTERFACE_START_Y + 30;	// SMALLPOCK6
	gSMInvPocketXY[17].sX = INTERFACE_START_X + 432;	gSMInvPocketXY[17].sY = INV_INTERFACE_START_Y + 54;	// SMALLPOCK7
	gSMInvPocketXY[18].sX = INTERFACE_START_X + 432;	gSMInvPocketXY[18].sY = INV_INTERFACE_START_Y + 78;	// SMALLPOCK8
	
	SM_ITEMDESC_START_X		= ( 214 + INTERFACE_START_X );
	SM_ITEMDESC_START_Y		= ( 1 +   INV_INTERFACE_START_Y );
	SM_ITEMDESC_HEIGHT		= 128;
	SM_ITEMDESC_WIDTH		= 358;

	SM_SELMERC_AP_X			= ( 70 + INTERFACE_START_X );
	SM_SELMERC_AP_Y			= ( 53 + INV_INTERFACE_START_Y );
	SM_SELMERC_AP_HEIGHT	= 12;
	SM_SELMERC_AP_WIDTH		= 11;
 
	SM_SELMERC_HEALTH_X		= ( 69 + INTERFACE_START_X );
	SM_SELMERC_HEALTH_Y		= ( 47 + INV_INTERFACE_START_Y );
	SM_SELMERC_HEALTH_WIDTH = 3;
	SM_SELMERC_HEALTH_HEIGHT = 42;

	SM_SELMERC_BREATH_X		= ( 75 + INTERFACE_START_X );
	SM_SELMERC_BREATH_Y		= ( 47 + INV_INTERFACE_START_Y );
	SM_SELMERC_BREATH_WIDTH = 3;
	SM_SELMERC_BREATH_HEIGHT = 42;

	SM_SELMERC_MORALE_X		= ( 81 + INTERFACE_START_X );
	SM_SELMERC_MORALE_Y		= ( 47 + INV_INTERFACE_START_Y );
	SM_SELMERC_MORALE_WIDTH = 3;
	SM_SELMERC_MORALE_HEIGHT = 42;

	SM_SELMERC_BARS_TIP_X	= (	SM_SELMERC_HEALTH_X - SM_SELMERC_HEALTH_WIDTH );
	SM_SELMERC_BARS_TIP_Y	= ( SM_SELMERC_HEALTH_Y - SM_SELMERC_HEALTH_HEIGHT );
	SM_SELMERC_BARS_TIP_WIDTH = ((SM_SELMERC_BREATH_WIDTH + 4) *3);
	SM_SELMERC_BARS_TIP_HEIGHT= ( SM_SELMERC_HEALTH_HEIGHT );

	SM_SELMERCNAME_X		= ( 7 + INTERFACE_START_X );
	SM_SELMERCNAME_Y		= ( 55 + INV_INTERFACE_START_Y );
	SM_SELMERCNAME_WIDTH	= 53;
	SM_SELMERCNAME_HEIGHT	= 9;

	SM_SELMERC_FACE_X		= ( 13 + INTERFACE_START_X );
	SM_SELMERC_FACE_Y		= ( 6 + INV_INTERFACE_START_Y );
	SM_SELMERC_FACE_HEIGHT	= 42;
	SM_SELMERC_FACE_WIDTH	= 48;

	SM_SELMERC_PLATE_X		= ( 4 + INTERFACE_START_X );
	SM_SELMERC_PLATE_Y		= ( 2 + INV_INTERFACE_START_Y );
	SM_SELMERC_PLATE_HEIGHT = 65;
	SM_SELMERC_PLATE_WIDTH	= 83;

	SM_BODYINV_X			= ( 244 + INTERFACE_START_X );
	SM_BODYINV_Y			= ( 6 + INV_INTERFACE_START_Y );

	// Cammo
	gSMCamoXY.sX = SM_BODYINV_X; gSMCamoXY.sY = SM_BODYINV_Y;

	SM_TALKB_X				= ( 155 + INTERFACE_START_X );
	SM_TALKB_Y				= ( 108 + INV_INTERFACE_START_Y );
	SM_MUTEB_X				= ( 91 + INTERFACE_START_X );
	SM_MUTEB_Y				= ( 108 + INV_INTERFACE_START_Y );
	SM_STANCEUPB_X			= ( 187 + INTERFACE_START_X );
	SM_STANCEUPB_Y			= ( 40 + INV_INTERFACE_START_Y );
	SM_UPDOWNB_X			= ( 91 + INTERFACE_START_X );
	SM_UPDOWNB_Y			= ( 73 + INV_INTERFACE_START_Y );
	SM_CLIMBB_X				= ( 187 + INTERFACE_START_X );
	SM_CLIMBB_Y				= ( 8 + INV_INTERFACE_START_Y );
	SM_STANCEDOWNB_X		= ( 187 + INTERFACE_START_X );
	SM_STANCEDOWNB_Y		= ( 108 + INV_INTERFACE_START_Y );
	SM_HANDCURSORB_X		= ( 123 + INTERFACE_START_X );
	SM_HANDCURSORB_Y		= ( 73 + INV_INTERFACE_START_Y );
	SM_PREVMERCB_X			= ( 9 + INTERFACE_START_X );
	SM_PREVMERCB_Y			= ( 70 + INV_INTERFACE_START_Y );
	SM_NEXTMERCB_X			= ( 51 + INTERFACE_START_X );
	SM_NEXTMERCB_Y			= ( 70 + INV_INTERFACE_START_Y );
	SM_OPTIONSB_X			= ( 9 + INTERFACE_START_X );
	SM_OPTIONSB_Y			= ( 105 + INV_INTERFACE_START_Y );
	SM_BURSTMODEB_X			= ( 155 + INTERFACE_START_X );
	SM_BURSTMODEB_Y			= ( 73 + INV_INTERFACE_START_Y );
	SM_LOOKB_X				= ( 123 + INTERFACE_START_X );
	SM_LOOKB_Y				= ( 108 + INV_INTERFACE_START_Y );
	SM_STEALTHMODE_X		= ( 187 + INTERFACE_START_X );
	SM_STEALTHMODE_Y		= ( 73 + INV_INTERFACE_START_Y );

	// WANNE 2
	SM_DONE_X				=  (SCREEN_WIDTH - 97);//( 543 + INTERFACE_START_X );
	SM_DONE_Y				= ( 4 + INV_INTERFACE_START_Y );
	SM_MAPSCREEN_X			=  (SCREEN_WIDTH - 51);//( 589 + INTERFACE_START_X );
	SM_MAPSCREEN_Y			= ( 4 + INV_INTERFACE_START_Y );

	SM_POSITIONB_X			= ( 106 + INTERFACE_START_X );
	SM_POSITIONB_Y			= ( 34 + INV_INTERFACE_START_Y );
	SM_POSITIONB_WIDTH		= 19;
	SM_POSITIONB_HEIGHT		= 24;

	SM_PERCENT_WIDTH		= 20;
	SM_PERCENT_HEIGHT		= 10;
	SM_ARMOR_X				= ( 347 + INTERFACE_START_X );
	SM_ARMOR_Y				= ( 79 + INV_INTERFACE_START_Y );
	SM_ARMOR_LABEL_X		= ( 363 + INTERFACE_START_X );
	SM_ARMOR_LABEL_Y		= ( 69 + INV_INTERFACE_START_Y );
	SM_ARMOR_PERCENT_X		= ( 368 + INTERFACE_START_X );
	SM_ARMOR_PERCENT_Y		= ( 79 + INV_INTERFACE_START_Y );

	SM_WEIGHT_LABEL_X		= ( 430 + INTERFACE_START_X );
	SM_WEIGHT_LABEL_Y		= ( 107 + INV_INTERFACE_START_Y );
	SM_WEIGHT_PERCENT_X		= ( 449 + INTERFACE_START_X );
	SM_WEIGHT_PERCENT_Y		= ( 107 + INV_INTERFACE_START_Y );
	SM_WEIGHT_X				= ( 428 + INTERFACE_START_X );
	SM_WEIGHT_Y				= ( 106 + INV_INTERFACE_START_Y );

	SM_CAMMO_LABEL_X		= ( 430 + INTERFACE_START_X );
	SM_CAMMO_LABEL_Y		= ( 122 + INV_INTERFACE_START_Y );
	SM_CAMMO_PERCENT_X		= ( 449 + INTERFACE_START_X );
	SM_CAMMO_PERCENT_Y		= ( 121 + INV_INTERFACE_START_Y );
	SM_CAMMO_X				= ( 428 + INTERFACE_START_X );
	SM_CAMMO_Y				= ( 121 + INV_INTERFACE_START_Y );

	SM_STATS_WIDTH			= 30;
	SM_STATS_HEIGHT			= 8 ;
	SM_AGI_X				= ( 99 + INTERFACE_START_X );
	SM_AGI_Y				= ( 7 + INV_INTERFACE_START_Y );
	SM_DEX_X				= ( 99 + INTERFACE_START_X );
	SM_DEX_Y				= ( 17 + INV_INTERFACE_START_Y );
	SM_STR_X				= ( 99 + INTERFACE_START_X );
	SM_STR_Y				= ( 27 + INV_INTERFACE_START_Y );
	SM_CHAR_X				= ( 99 + INTERFACE_START_X );
	SM_CHAR_Y				= ( 37 + INV_INTERFACE_START_Y );
	SM_WIS_X				= ( 99 + INTERFACE_START_X );
	SM_WIS_Y				= ( 47 + INV_INTERFACE_START_Y );
	SM_EXPLVL_X				= ( 148 + INTERFACE_START_X );
	SM_EXPLVL_Y				= ( 7 + INV_INTERFACE_START_Y );
	SM_MRKM_X				= ( 148 + INTERFACE_START_X );
	SM_MRKM_Y				= ( 17 + INV_INTERFACE_START_Y );
	SM_EXPL_X				= ( 148 + INTERFACE_START_X );
	SM_EXPL_Y				= ( 27 + INV_INTERFACE_START_Y );
	SM_MECH_X				= ( 148 + INTERFACE_START_X );
	SM_MECH_Y				= ( 37 + INV_INTERFACE_START_Y );
	SM_MED_X				= ( 148 + INTERFACE_START_X );
	SM_MED_Y				= ( 47 + INV_INTERFACE_START_Y );

	MONEY_X					= ( 460 + INTERFACE_START_X );
	MONEY_Y					= ( 105 + INV_INTERFACE_START_Y );
	MONEY_WIDTH				= 30;
	MONEY_HEIGHT			= 22;
	
	// ow ye font color they can also be customizable :P
	STATS_TITLE_FONT_COLOR	= 6;
	STATS_TEXT_FONT_COLOR	= 5;

	// ow and te clock and location i will put it here 
	INTERFACE_CLOCK_X	=  	(SCREEN_WIDTH - 86);				//( 554	+ INTERFACE_START_X		);
	INTERFACE_CLOCK_Y	= ( 119	+ INV_INTERFACE_START_Y );
	LOCATION_NAME_X		=	(SCREEN_WIDTH - 92);				//( 548	+ INTERFACE_START_X		);
	LOCATION_NAME_Y		= ( 65	+ INTERFACE_START_Y		);

	// so we got everything "dynamic" now we just return TRUE
	return ( TRUE );
}
BOOLEAN InitializeSMPanel(  )
{
  VOBJECT_DESC    VObjectDesc;

 /*  OK i need to initialize coords here
 *  Isnt it cool
 *  any questions? joker
 */
//	InitializeSMPanelCoords( );
	
	// failing the CHECKF after this will cause you to lose your mouse
	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;

	if (iResolution == 0)
	{
	strcpy( VObjectDesc.ImageFile, "INTERFACE\\inventory_bottom_panel.STI" );
	}
	else if (iResolution == 1)
	{
		strcpy( VObjectDesc.ImageFile, "INTERFACE\\inventory_bottom_panel_800x600.STI" );
	}
	else if (iResolution == 2)
	{
		strcpy( VObjectDesc.ImageFile, "INTERFACE\\inventory_bottom_panel_1024x768.STI" );
	}

	CHECKF( AddVideoObject( &VObjectDesc, &guiSMPanel) );

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\inventory_gold_front.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &guiSMObjects ) );

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\inv_frn.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &guiSMObjects2 ) );


	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\secondary_gun_hidden.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &guiSecItemHiddenVO ) );

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\Bars.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &guiBrownBackgroundForTeamPanel ) );

	
	// Clear inv display stuff
	memset( gfSM_HandInvDispText, 0, sizeof( gfSM_HandInvDispText ) );

	// INIT viewport region
	// Set global mouse regions for SM panel
	// Define region for viewport
	MSYS_DefineRegion( &gViewportRegion, 0, 0 ,gsVIEWPORT_END_X, gsVIEWPORT_WINDOW_END_Y, MSYS_PRIORITY_NORMAL, VIDEO_NO_CURSOR, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK );
	MSYS_AddRegion( &gViewportRegion );

	// Create buttons
	CHECKF( CreateSMPanelButtons( ) );

	// Set viewports
	// Define region for panel
	MSYS_DefineRegion( &gSMPanelRegion, 0, INV_INTERFACE_START_Y ,SCREEN_WIDTH, SCREEN_HEIGHT, MSYS_PRIORITY_NORMAL,
						 CURSOR_NORMAL, MSYS_NO_CALLBACK, InvPanelButtonClickCallback ); 
	// Add region
	MSYS_AddRegion( &gSMPanelRegion);

	//DEfine region for selected guy panel
	MSYS_DefineRegion( &gSM_SELMERCPanelRegion, SM_SELMERC_FACE_X, SM_SELMERC_FACE_Y, SM_SELMERC_FACE_X + SM_SELMERC_FACE_WIDTH, SM_SELMERC_FACE_Y + SM_SELMERC_FACE_HEIGHT, MSYS_PRIORITY_NORMAL,
						 MSYS_NO_CURSOR, SelectedMercButtonMoveCallback, SelectedMercButtonCallback ); 
	// Add region
	MSYS_AddRegion( &gSM_SELMERCPanelRegion );



	//DEfine region for selected guy panel
	MSYS_DefineRegion( &gSM_SELMERCEnemyIndicatorRegion, SM_SELMERC_FACE_X + 1, SM_SELMERC_FACE_Y + 1, SM_SELMERC_FACE_X + INDICATOR_BOX_WIDTH, SM_SELMERC_FACE_Y + INDICATOR_BOX_HEIGHT, MSYS_PRIORITY_NORMAL,
						 MSYS_NO_CURSOR, MSYS_NO_CALLBACK, SelectedMercEnemyIndicatorCallback ); 
	// Add region
	MSYS_AddRegion( &gSM_SELMERCEnemyIndicatorRegion );


	//DEfine region for money button
	MSYS_DefineRegion( &gSM_SELMERCMoneyRegion, MONEY_X, MONEY_Y, MONEY_X + MONEY_WIDTH, MONEY_Y + MONEY_HEIGHT, MSYS_PRIORITY_HIGH,
						 MSYS_NO_CURSOR, MSYS_NO_CALLBACK, SMInvMoneyButtonCallback ); 
	// Add region
	MSYS_AddRegion( &gSM_SELMERCMoneyRegion );

	SetRegionFastHelpText( &(gSM_SELMERCMoneyRegion), TacticalStr[ MONEY_BUTTON_HELP_TEXT ] );



	// Check if mouse is in region and if so, adjust...
	if ( IsMouseInRegion( &gSM_SELMERCPanelRegion ) )
	{
		HandleMouseOverSoldierFaceForContMove( gpSMCurrentMerc, TRUE );
	}


	//DEfine region for selected guy panel
	// THIS IS A REGION OF BAR TIPS
	MSYS_DefineRegion( &gSM_SELMERCBarsRegion, SM_SELMERC_BARS_TIP_X, SM_SELMERC_BARS_TIP_Y, 
					SM_SELMERC_BARS_TIP_X + SM_SELMERC_BARS_TIP_WIDTH, SM_SELMERC_BARS_TIP_Y + SM_SELMERC_BARS_TIP_HEIGHT, MSYS_PRIORITY_NORMAL,
						 MSYS_NO_CURSOR, MSYS_NO_CALLBACK, SelectedMercButtonCallback ); 
	MSYS_AddRegion( &gSM_SELMERCBarsRegion );

	
	InitInvSlotInterface( gSMInvPocketXY, &gSMCamoXY, SMInvMoveCallback, SMInvClickCallback, SMInvMoveCammoCallback, SMInvClickCamoCallback, FALSE );

	InitKeyRingInterface( KeyRingItemPanelButtonCallback );


	// this is important! It will disable buttons like SM_MAP_SCREEN_BUTTON when they're supposed to be disabled - the previous
	// disabled state is lost everytime panel is reinitialized, because all the buttons are created from scratch!
	if ( gpItemPointer == NULL )
	{
		// empty cursor - enable, not from item pickup
		EnableSMPanelButtons( TRUE, FALSE );
	}
	else
	{
		// full cursor - disable, from item pickup
		EnableSMPanelButtons( FALSE, TRUE );
	}

	return( TRUE );
}


BOOLEAN CreateSMPanelButtons( )
{
CHAR8 ubString[48];

	giSMStealthImages = -1;
	giSMStealthButton = -1;
	gfUIStanceDifferent = TRUE;
	gfAllDisabled	= FALSE;

	FilenameForBPP("INTERFACE\\inventory_buttons.sti", ubString);
	// Load button Graphics
	iSMPanelImages[ STANCEUP_IMAGES  ]			= LoadButtonImage(ubString,-1,0,-1,10,-1 );

	iSMPanelImages[ UPDOWN_IMAGES  ]				= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES  ] ,-1,9,-1,19,-1 );
	iSMPanelImages[ CLIMB_IMAGES  ]					= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES  ] ,-1,3,-1,13,-1 );
	iSMPanelImages[ STANCEDOWN_IMAGES  ]		= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES  ] ,-1,8,-1,18,-1 );
	iSMPanelImages[ HANDCURSOR_IMAGES  ]		= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES  ] ,-1,1,-1,11,-1 );
	iSMPanelImages[ PREVMERC_IMAGES  ]			= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES  ] ,-1,20,-1,22,-1 );
	iSMPanelImages[ NEXTMERC_IMAGES  ]			= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES  ] ,-1,21,-1,23,-1 );
	//iSMPanelImages[ BURSTMODE_IMAGES  ]			= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES  ] ,-1,7,-1,17,-1 );
	iSMPanelImages[ LOOK_IMAGES  ]					= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES  ] ,-1,2,-1,12,-1 );
	iSMPanelImages[ TALK_IMAGES  ]					= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES  ] ,-1,6,-1,16,-1 );
	iSMPanelImages[ MUTE_IMAGES  ]					= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES  ] ,-1,5,-1,15,-1 );
	iSMPanelImages[ OPTIONS_IMAGES  ]				= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES  ] ,-1,24,-1,25,-1 );

	iBurstButtonImages[ WM_NORMAL ]					= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES  ], -1, 7, -1, -1, -1 );
	iBurstButtonImages[ WM_BURST ]					= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES  ], -1, 17, -1, -1, -1 );
	iBurstButtonImages[ WM_AUTOFIRE ]				= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES  ], -1, 17, -1, -1, -1 );
	iBurstButtonImages[ WM_ATTACHED_GL ]				= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES  ], -1, 26, -1, -1, -1 );
	iBurstButtonImages[ WM_ATTACHED_GL_BURST ]					= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES  ], -1, 17, -1, -1, -1 );
	iBurstButtonImages[ WM_ATTACHED_GL_AUTO ]				= UseLoadedButtonImage( iSMPanelImages[ STANCEUP_IMAGES  ], -1, 17, -1, -1, -1 );
	

	FilenameForBPP("INTERFACE\\invadd-ons.sti", ubString);
	// Load button Graphics
	iSMPanelImages[ STANCE_IMAGES  ]				= LoadButtonImage(ubString,0,0,-1,2,-1 );

	FilenameForBPP("INTERFACE\\inventory_buttons_2.sti", ubString);
	// Load button Graphics
	iSMPanelImages[ DONE_IMAGES  ]					= LoadButtonImage(ubString,-1,1,-1,3,-1 );
	iSMPanelImages[ MAPSCREEN_IMAGES  ]			= UseLoadedButtonImage( iSMPanelImages[ DONE_IMAGES  ] ,-1,0,-1,2,-1 );


	// Create buttons

	// SET BUTTONS TO -1
	memset( iSMPanelButtons, -1, sizeof( iSMPanelButtons ) );

	iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ] = QuickCreateButton( iSMPanelImages[ MAPSCREEN_IMAGES ], SM_MAPSCREEN_X, SM_MAPSCREEN_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnMapScreenCallback );
	SetButtonFastHelpText( iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ], TacticalStr[ MAPSCREEN_POPUPTEXT ]);
	SetBtnHelpEndCallback( iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ], HelpTextDoneCallback );

	iSMPanelButtons[ SM_DONE_BUTTON ] = CreateIconAndTextButton( iSMPanelImages[ DONE_IMAGES ], L"", FONT12ARIAL,
													 FONT_MCOLOR_WHITE, DEFAULT_SHADOW ,
													 FONT_MCOLOR_WHITE, DEFAULT_SHADOW ,
													 TEXT_CJUSTIFIED, 
													 SM_DONE_X, SM_DONE_Y, BUTTON_TOGGLE ,MSYS_PRIORITY_HIGH-1,
													 DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnSMDoneCallback );
	SetButtonFastHelpText( iSMPanelButtons[ SM_DONE_BUTTON ], TacticalStr[ END_TURN_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ SM_DONE_BUTTON ], HelpTextDoneCallback );


	iSMPanelButtons[ TALK_BUTTON ] = QuickCreateButton( iSMPanelImages[ TALK_IMAGES ], SM_TALKB_X, SM_TALKB_Y,
										BUTTON_NEWTOGGLE, MSYS_PRIORITY_HIGH - 1,
										MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnTalkCallback );
	SetButtonFastHelpText( iSMPanelButtons[ TALK_BUTTON ], TacticalStr[ TALK_CURSOR_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ TALK_BUTTON ], HelpTextDoneCallback );



	iSMPanelButtons[ MUTE_BUTTON ] = QuickCreateButton( iSMPanelImages[ MUTE_IMAGES ], SM_MUTEB_X, SM_MUTEB_Y,
										BUTTON_NEWTOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnMuteCallback );
	SetButtonFastHelpText( iSMPanelButtons[ MUTE_BUTTON ], TacticalStr[ TOGGLE_MUTE_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ MUTE_BUTTON ], HelpTextDoneCallback );


	iSMPanelButtons[ STANCEUP_BUTTON ] = QuickCreateButton( iSMPanelImages[ STANCEUP_IMAGES ], SM_STANCEUPB_X, SM_STANCEUPB_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnStanceUpCallback );
	if ( iSMPanelButtons[ STANCEUP_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	SetButtonFastHelpText( iSMPanelButtons[ STANCEUP_BUTTON ], TacticalStr[ CHANGE_STANCE_UP_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ STANCEUP_BUTTON ], HelpTextDoneCallback );

	//SetButtonFastHelpText( iSMPanelButtons[ STANCEUP_BUTTON ],L"Change Stance Up");


	iSMPanelButtons[ UPDOWN_BUTTON ] = QuickCreateButton( iSMPanelImages[ UPDOWN_IMAGES ], SM_UPDOWNB_X, SM_UPDOWNB_Y,
										BUTTON_NEWTOGGLE, MSYS_PRIORITY_HIGH - 1,
										MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnUpdownCallback );
	if ( iSMPanelButtons[ UPDOWN_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	//SetButtonFastHelpText( iSMPanelButtons[ UPDOWN_BUTTON ],L"Whatever");
	SetButtonFastHelpText( iSMPanelButtons[ UPDOWN_BUTTON ], TacticalStr[ CURSOR_LEVEL_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ UPDOWN_BUTTON ], HelpTextDoneCallback );


	iSMPanelButtons[ CLIMB_BUTTON ] = QuickCreateButton( iSMPanelImages[ CLIMB_IMAGES ], SM_CLIMBB_X, SM_CLIMBB_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnClimbCallback );
	if ( iSMPanelButtons[ CLIMB_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	//SetButtonFastHelpText( iSMPanelButtons[ CLIMB_BUTTON ],L"Climb");
	SetButtonFastHelpText( iSMPanelButtons[ CLIMB_BUTTON ], TacticalStr[ JUMPCLIMB_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ CLIMB_BUTTON ], HelpTextDoneCallback );


	iSMPanelButtons[ STANCEDOWN_BUTTON ] = QuickCreateButton( iSMPanelImages[ STANCEDOWN_IMAGES ], SM_STANCEDOWNB_X, SM_STANCEDOWNB_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnStanceDownCallback );
	if ( iSMPanelButtons[ STANCEDOWN_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	//SetButtonFastHelpText( iSMPanelButtons[ STANCEDOWN_BUTTON ],L"Change Stance Down");
	SetButtonFastHelpText( iSMPanelButtons[ STANCEDOWN_BUTTON ], TacticalStr[ CHANGE_STANCE_DOWN_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ STANCEDOWN_BUTTON ], HelpTextDoneCallback );


	iSMPanelButtons[ HANDCURSOR_BUTTON ] = QuickCreateButton( iSMPanelImages[ HANDCURSOR_IMAGES ], SM_HANDCURSORB_X, SM_HANDCURSORB_Y,
										BUTTON_NEWTOGGLE, MSYS_PRIORITY_HIGH - 1,
										MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnHandCursorCallback );
	if ( iSMPanelButtons[ HANDCURSOR_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	//SetButtonFastHelpText( iSMPanelButtons[ HANDCURSOR_BUTTON ],L"Change Stance Down");
	SetButtonFastHelpText( iSMPanelButtons[ HANDCURSOR_BUTTON ], TacticalStr[ EXAMINE_CURSOR_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ HANDCURSOR_BUTTON ], HelpTextDoneCallback );


	iSMPanelButtons[ PREVMERC_BUTTON ] = QuickCreateButton( iSMPanelImages[ PREVMERC_IMAGES ], SM_PREVMERCB_X, SM_PREVMERCB_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnPrevMercCallback );
	if ( iSMPanelButtons[ PREVMERC_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	//SetButtonFastHelpText( iSMPanelButtons[ PREVMERC_BUTTON ],L"Change Stance Down");
	SetButtonFastHelpText( iSMPanelButtons[ PREVMERC_BUTTON ], TacticalStr[ PREV_MERC_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ PREVMERC_BUTTON ], HelpTextDoneCallback );


	iSMPanelButtons[ NEXTMERC_BUTTON ] = QuickCreateButton( iSMPanelImages[ NEXTMERC_IMAGES ], SM_NEXTMERCB_X, SM_NEXTMERCB_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnNextMercCallback );
	if ( iSMPanelButtons[ NEXTMERC_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	SetButtonFastHelpText( iSMPanelButtons[ NEXTMERC_BUTTON ], TacticalStr[ NEXT_MERC_POPUPTEXT ] );
	//SetButtonFastHelpText( iSMPanelButtons[ NEXTMERC_BUTTON ],L"Change Stance Down");
	SetBtnHelpEndCallback( iSMPanelButtons[ NEXTMERC_BUTTON ], HelpTextDoneCallback );


	iSMPanelButtons[ OPTIONS_BUTTON ] = QuickCreateButton( iSMPanelImages[ OPTIONS_IMAGES ], SM_OPTIONSB_X, SM_OPTIONSB_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnOptionsCallback );
	if ( iSMPanelButtons[ OPTIONS_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	//SetButtonFastHelpText( iSMPanelButtons[ OPTIONS_BUTTON ],L"Change Stance Down");
	SetButtonFastHelpText( iSMPanelButtons[ OPTIONS_BUTTON ], TacticalStr[ CHANGE_OPTIONS_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ OPTIONS_BUTTON ], HelpTextDoneCallback );


	/*iSMPanelButtons[ BURSTMODE_BUTTON ] = QuickCreateButton( iSMPanelImages[ BURSTMODE_IMAGES ], SM_BURSTMODEB_X, SM_BURSTMODEB_Y,
										BUTTON_NEWTOGGLE, MSYS_PRIORITY_HIGH - 1,
										MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnBurstModeCallback );*/
	iSMPanelButtons[ BURSTMODE_BUTTON ] = QuickCreateButton( iBurstButtonImages[ WM_NORMAL ], SM_BURSTMODEB_X, SM_BURSTMODEB_Y, BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH, MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnBurstModeCallback );

	if ( iSMPanelButtons[ BURSTMODE_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	SetButtonFastHelpText( iSMPanelButtons[ BURSTMODE_BUTTON ], TacticalStr[ TOGGLE_BURSTMODE_POPUPTEXT ] );
	//SetButtonFastHelpText( iSMPanelButtons[ BURSTMODE_BUTTON ],L"Change Stance Down");
	SetBtnHelpEndCallback( iSMPanelButtons[ BURSTMODE_BUTTON ], HelpTextDoneCallback );

	iSMPanelButtons[ LOOK_BUTTON ] = QuickCreateButton( iSMPanelImages[ LOOK_IMAGES ], SM_LOOKB_X, SM_LOOKB_Y,
										BUTTON_NEWTOGGLE, MSYS_PRIORITY_HIGH - 1,
										MSYS_NO_CALLBACK, (GUI_CALLBACK)BtnLookCallback );
	if ( iSMPanelButtons[ LOOK_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	//SetButtonFastHelpText( iSMPanelButtons[ LOOK_BUTTON ],L"Change Stance Down");
	SetButtonFastHelpText( iSMPanelButtons[ LOOK_BUTTON ], TacticalStr[ LOOK_CURSOR_POPUPTEXT ] );
	SetBtnHelpEndCallback( iSMPanelButtons[ LOOK_BUTTON ], HelpTextDoneCallback );

	return( TRUE );
}

void    RemoveSMPanelButtons( )
{
	UINT32 cnt;

	for ( cnt = 0; cnt < NUM_SM_BUTTONS; cnt++ )
	{
		if ( iSMPanelButtons[ cnt ] != -1 )
		{
			RemoveButton( iSMPanelButtons[ cnt ] );
		}
	}

	for ( cnt = 0; cnt < NUM_SM_BUTTON_IMAGES; cnt++ )
	{
		UnloadButtonImage( iSMPanelImages[ cnt ] );
	}

	if ( giSMStealthButton != -1 )
	{
		RemoveButton( giSMStealthButton );
	}

	if ( giSMStealthImages != -1 )
	{
		UnloadButtonImage( giSMStealthImages );
	}
	
	UnloadButtonImage( iBurstButtonImages[ WM_NORMAL ] );
	UnloadButtonImage( iBurstButtonImages[ WM_BURST ] );
	UnloadButtonImage( iBurstButtonImages[ WM_ATTACHED_GL ] );
	UnloadButtonImage( iBurstButtonImages[ WM_ATTACHED_GL_BURST ] );
	UnloadButtonImage( iBurstButtonImages[ WM_ATTACHED_GL_AUTO ] );

}




BOOLEAN ShutdownSMPanel( )
{

	// All buttons and regions and video objects and video surfaces will be deleted at shutddown of SGM
	// We may want to delete them at the interm as well, to free up room for other panels
	DeleteVideoObjectFromIndex( guiSMPanel );
	DeleteVideoObjectFromIndex( guiSMObjects );
	DeleteVideoObjectFromIndex( guiSMObjects2 );
	DeleteVideoObjectFromIndex( guiSecItemHiddenVO );
	DeleteVideoObjectFromIndex( guiBrownBackgroundForTeamPanel );

	gubSelectSMPanelToMerc = NOBODY;

	// CJC: delete key ring if open
	DeleteKeyRingPopup(); // function will abort if key ring is not up

	// ATE: Delete desc panel if it was open....
	if ( gfInItemDescBox )
	{
		DeleteItemDescriptionBox( );
	}


	// Shotdown item slot interface
	ShutdownInvSlotInterface( );

	// shutdown keyring interface
	ShutdownKeyRingInterface( );
	
	MSYS_RemoveRegion( &gSMPanelRegion );
	MSYS_RemoveRegion( &gSM_SELMERCPanelRegion );
	MSYS_RemoveRegion( &gSM_SELMERCBarsRegion );
	MSYS_RemoveRegion( &gSM_SELMERCMoneyRegion );
	MSYS_RemoveRegion( &gSM_SELMERCEnemyIndicatorRegion );

	HandleMouseOverSoldierFaceForContMove( gpSMCurrentMerc, FALSE );

	MSYS_RemoveRegion( &gViewportRegion );


	RemoveSMPanelButtons( );

	return( TRUE );
}


void RenderSMPanel( BOOLEAN *pfDirty )
{
	INT16 sFontX, sFontY;
	INT16	usX, usY;
	CHAR16 sString[9];
	UINT32	cnt;
	static CHAR16 pStr[ 200 ], pMoraleStr[ 20 ];

	if ( gubSelectSMPanelToMerc != NOBODY )
	{
		// Give him the panel!
		SetSMPanelCurrentMerc( gubSelectSMPanelToMerc );
	}


	// ATE: Don't do anything if we are in stack popup and are refreshing stuff....
	if ( ( InItemStackPopup( ) || ( InKeyRingPopup( ) ) ) && (*pfDirty) == DIRTYLEVEL1 )
	{
		return;
	}

	if ( gfCheckForMouseOverItem )
	{	
		if ( ( GetJA2Clock( ) - guiMouseOverItemTime ) > 100 )
		{
			if ( HandleCompatibleAmmoUI( gpSMCurrentMerc, (INT8)gbCheckForMouseOverItemPos, TRUE ) )
			{
				(*pfDirty) = DIRTYLEVEL2;
			}

			gfCheckForMouseOverItem = FALSE;
		}
	}

	HandleNewlyAddedItems( gpSMCurrentMerc, pfDirty );

	if ( InItemDescriptionBox( ) )
	{
		HandleItemDescriptionBox( pfDirty );
	}

	if ( *pfDirty == DIRTYLEVEL2 )
	{
		//if ( InItemStackPopup( ) )
		//{

		//}

		if ( InItemDescriptionBox( ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiSMPanel, 0, INTERFACE_START_X, INV_INTERFACE_START_Y, VO_BLT_SRCTRANSPARENCY, NULL );
			RenderSoldierFace( gpSMCurrentMerc, SM_SELMERC_FACE_X, SM_SELMERC_FACE_Y, TRUE );


			// ATE: Need these lines here to fix flash bug with face selection box
			if ( gfSMDisableForItems )
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiSMObjects2, 0, SM_SELMERC_PLATE_X, SM_SELMERC_PLATE_Y, VO_BLT_SRCTRANSPARENCY, NULL );
				RestoreExternBackgroundRect( SM_SELMERC_PLATE_X, SM_SELMERC_PLATE_Y, SM_SELMERC_PLATE_WIDTH , SM_SELMERC_PLATE_HEIGHT );
			}
			else
			{
				if ( gusSelectedSoldier == gpSMCurrentMerc->ubID && gTacticalStatus.ubCurrentTeam == OUR_TEAM && OK_INTERRUPT_MERC(  gpSMCurrentMerc ) )
				{
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiSMObjects, 0, SM_SELMERC_PLATE_X, SM_SELMERC_PLATE_Y, VO_BLT_SRCTRANSPARENCY, NULL );
					RestoreExternBackgroundRect( SM_SELMERC_PLATE_X, SM_SELMERC_PLATE_Y, SM_SELMERC_PLATE_WIDTH , SM_SELMERC_PLATE_HEIGHT );
				}
			}

			RenderItemDescriptionBox( );
		}
		else
		{

			BltVideoObjectFromIndex( guiSAVEBUFFER, guiSMPanel, 0, INTERFACE_START_X, INV_INTERFACE_START_Y, VO_BLT_SRCTRANSPARENCY, NULL );

			RenderInvBodyPanel( gpSMCurrentMerc, SM_BODYINV_X, SM_BODYINV_Y );		

			// CHECK FOR PANEL STUFF / DEATHS / CLOSURES
			CheckForFacePanelStartAnims( gpSMCurrentMerc, SM_SELMERC_FACE_X, SM_SELMERC_FACE_Y );

			// Hitlight

			if ( gfSMDisableForItems )
			{
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiSMObjects2, 0, SM_SELMERC_PLATE_X, SM_SELMERC_PLATE_Y, VO_BLT_SRCTRANSPARENCY, NULL );
				RestoreExternBackgroundRect( SM_SELMERC_PLATE_X, SM_SELMERC_PLATE_Y, SM_SELMERC_PLATE_WIDTH , SM_SELMERC_PLATE_HEIGHT );
			}
			else
			{
				if ( gusSelectedSoldier == gpSMCurrentMerc->ubID && gTacticalStatus.ubCurrentTeam == OUR_TEAM && OK_INTERRUPT_MERC(  gpSMCurrentMerc ) )
				{
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiSMObjects, 0, SM_SELMERC_PLATE_X, SM_SELMERC_PLATE_Y, VO_BLT_SRCTRANSPARENCY, NULL );
					RestoreExternBackgroundRect( SM_SELMERC_PLATE_X, SM_SELMERC_PLATE_Y, SM_SELMERC_PLATE_WIDTH , SM_SELMERC_PLATE_HEIGHT );
				}
			}

			// Render faceplate
			//BltVideoObjectFromIndex( guiSAVEBUFFER, guiSMObjects2, 1, SM_SELMERC_NAMEPLATE_X, SM_SELMERC_NAMEPLATE_Y, VO_BLT_SRCTRANSPARENCY, NULL );
			//RestoreExternBackgroundRect( SM_SELMERC_NAMEPLATE_X, SM_SELMERC_NAMEPLATE_Y, SM_SELMERC_NAMEPLATE_WIDTH, SM_SELMERC_NAMEPLATE_HEIGHT );

			// Blit position
			//if(gbPixelDepth==16)
			//{
				//BltVideoObjectFromIndex( guiSAVEBUFFER, guiSMObjects, gbSMCurStanceObj, SM_POSITIONB_X, SM_POSITIONB_Y, VO_BLT_SRCTRANSPARENCY, NULL );
			//}
			//RestoreExternBackgroundRect( SM_POSITIONB_X, SM_POSITIONB_Y, SM_POSITIONB_WIDTH , SM_POSITIONB_HEIGHT );


			SetFont( BLOCKFONT2 );


			// Render Values for stats!
			// Set font drawing to saved buffer
			SetFontDestBuffer( guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

			SetFontBackground( FONT_MCOLOR_BLACK );
			SetFontForeground( STATS_TITLE_FONT_COLOR );
			for( cnt = 0; cnt < 5; cnt++ )
			{
				mprintf( INTERFACE_START_X + 92, ( INT16 )( INV_INTERFACE_START_Y + 7 + cnt * 10 ), pShortAttributeStrings[ cnt  ] );
				mprintf( INTERFACE_START_X + 137, ( INT16 )( INV_INTERFACE_START_Y + 7 + cnt * 10 ), pShortAttributeStrings[ cnt + 5 ] );
			}

			mprintf( SM_ARMOR_LABEL_X - StringPixLength( pInvPanelTitleStrings[0], BLOCKFONT2 ) / 2, SM_ARMOR_LABEL_Y, pInvPanelTitleStrings[ 0 ] );
			mprintf( SM_ARMOR_PERCENT_X, SM_ARMOR_PERCENT_Y, L"%%" );

			mprintf( SM_WEIGHT_LABEL_X - StringPixLength( pInvPanelTitleStrings[1], BLOCKFONT2 ), SM_WEIGHT_LABEL_Y, pInvPanelTitleStrings[ 1 ] );
			mprintf( SM_WEIGHT_PERCENT_X, SM_WEIGHT_PERCENT_Y, L"%%" );
			
			mprintf( SM_CAMMO_LABEL_X - StringPixLength( pInvPanelTitleStrings[2], BLOCKFONT2 ), SM_CAMMO_LABEL_Y, pInvPanelTitleStrings[ 2 ] );
			mprintf( SM_CAMMO_PERCENT_X, SM_CAMMO_PERCENT_Y, L"%%" );

			UpdateStatColor( gpSMCurrentMerc-> uiChangeAgilityTime, ( BOOLEAN ) ( gpSMCurrentMerc->usValueGoneUp & AGIL_INCREASE? TRUE: FALSE ) );

			swprintf( sString, L"%2d", gpSMCurrentMerc->bAgility );
			FindFontRightCoordinates(SM_AGI_X, SM_AGI_Y ,SM_STATS_WIDTH ,SM_STATS_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString ); 

			UpdateStatColor( gpSMCurrentMerc-> uiChangeDexterityTime,( BOOLEAN ) ( gpSMCurrentMerc->usValueGoneUp & DEX_INCREASE? TRUE: FALSE )  );

			swprintf( sString, L"%2d", gpSMCurrentMerc->bDexterity );
			FindFontRightCoordinates(SM_DEX_X, SM_DEX_Y ,SM_STATS_WIDTH ,SM_STATS_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString ); 

			UpdateStatColor( gpSMCurrentMerc-> uiChangeStrengthTime, ( BOOLEAN )( gpSMCurrentMerc->usValueGoneUp & STRENGTH_INCREASE?TRUE: FALSE ) );

			swprintf( sString, L"%2d", gpSMCurrentMerc->bStrength );
			FindFontRightCoordinates(SM_STR_X, SM_STR_Y ,SM_STATS_WIDTH ,SM_STATS_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString ); 

			UpdateStatColor( gpSMCurrentMerc-> uiChangeLeadershipTime, ( BOOLEAN )( gpSMCurrentMerc->usValueGoneUp & LDR_INCREASE? TRUE: FALSE )  );

			swprintf( sString, L"%2d", gpSMCurrentMerc->bLeadership );
			FindFontRightCoordinates(SM_CHAR_X, SM_CHAR_Y ,SM_STATS_WIDTH ,SM_STATS_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString ); 

			UpdateStatColor( gpSMCurrentMerc-> uiChangeWisdomTime,( BOOLEAN ) ( gpSMCurrentMerc->usValueGoneUp & WIS_INCREASE? TRUE: FALSE )  );

			swprintf( sString, L"%2d", gpSMCurrentMerc->bWisdom );
			FindFontRightCoordinates(SM_WIS_X, SM_WIS_Y ,SM_STATS_WIDTH ,SM_STATS_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString ); 

			UpdateStatColor( gpSMCurrentMerc-> uiChangeLevelTime, ( BOOLEAN ) ( gpSMCurrentMerc->usValueGoneUp & LVL_INCREASE? TRUE: FALSE )  );

			swprintf( sString, L"%2d", gpSMCurrentMerc->bExpLevel );
			FindFontRightCoordinates(SM_EXPLVL_X, SM_EXPLVL_Y ,SM_STATS_WIDTH ,SM_STATS_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString ); 

			UpdateStatColor( gpSMCurrentMerc-> uiChangeMarksmanshipTime, ( BOOLEAN ) ( gpSMCurrentMerc->usValueGoneUp & MRK_INCREASE? TRUE: FALSE )  );

			swprintf( sString, L"%2d", gpSMCurrentMerc->bMarksmanship );
			FindFontRightCoordinates(SM_MRKM_X, SM_MRKM_Y ,SM_STATS_WIDTH ,SM_STATS_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString ); 

			UpdateStatColor( gpSMCurrentMerc-> uiChangeExplosivesTime, ( BOOLEAN ) ( gpSMCurrentMerc->usValueGoneUp & EXP_INCREASE? TRUE: FALSE )  );

			swprintf( sString, L"%2d", gpSMCurrentMerc->bExplosive );
			FindFontRightCoordinates(SM_EXPL_X, SM_EXPL_Y ,SM_STATS_WIDTH ,SM_STATS_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString ); 

			UpdateStatColor( gpSMCurrentMerc-> uiChangeMechanicalTime, ( BOOLEAN ) ( gpSMCurrentMerc->usValueGoneUp & MECH_INCREASE ? TRUE: FALSE )  );

			swprintf( sString, L"%2d", gpSMCurrentMerc->bMechanical );
			FindFontRightCoordinates(SM_MECH_X, SM_MECH_Y ,SM_STATS_WIDTH ,SM_STATS_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString ); 

			UpdateStatColor( gpSMCurrentMerc-> uiChangeMedicalTime, ( BOOLEAN ) ( gpSMCurrentMerc->usValueGoneUp & MED_INCREASE? TRUE: FALSE )  );

			swprintf( sString, L"%2d", gpSMCurrentMerc->bMedical );
			FindFontRightCoordinates(SM_MED_X, SM_MED_Y ,SM_STATS_WIDTH ,SM_STATS_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString ); 

			if ( gpSMCurrentMerc->bLife >= OKLIFE )
			{
				SetFontBackground( FONT_MCOLOR_BLACK );
				SetFontForeground( STATS_TEXT_FONT_COLOR );
			}
			else
			{
				SetFontBackground( FONT_MCOLOR_BLACK );
				SetFontForeground( FONT_MCOLOR_DKGRAY );
			}

			// Display armour value!
			swprintf( sString, L"%3d", ArmourPercent( gpSMCurrentMerc ) );
			FindFontRightCoordinates(SM_ARMOR_X, SM_ARMOR_Y ,SM_PERCENT_WIDTH ,SM_PERCENT_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString ); 

			// Display wieght value!
			swprintf( sString, L"%3d", CalculateCarriedWeight( gpSMCurrentMerc ) );
			FindFontRightCoordinates(SM_WEIGHT_X, SM_WEIGHT_Y ,SM_PERCENT_WIDTH ,SM_PERCENT_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString ); 

			ApplyEquipmentBonuses(gpSMCurrentMerc);

			// Display cammo value!
			swprintf( sString, L"%3d", max(0, min( (gpSMCurrentMerc->bCamo + gpSMCurrentMerc->wornCamo+gpSMCurrentMerc->urbanCamo+gpSMCurrentMerc->wornUrbanCamo+gpSMCurrentMerc->desertCamo+gpSMCurrentMerc->wornDesertCamo+gpSMCurrentMerc->snowCamo+gpSMCurrentMerc->wornSnowCamo),100 )) );
			FindFontRightCoordinates(SM_CAMMO_X, SM_CAMMO_Y ,SM_PERCENT_WIDTH ,SM_PERCENT_HEIGHT ,sString, BLOCKFONT2, &usX, &usY);
			mprintf( usX, usY , sString ); 


			// reset to frame buffer!
			SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

			RestoreExternBackgroundRect( INTERFACE_START_X, INV_INTERFACE_START_Y, SCREEN_WIDTH - INTERFACE_START_X , INV_INTERFACE_HEIGHT );


			RenderSoldierFace( gpSMCurrentMerc, SM_SELMERC_FACE_X, SM_SELMERC_FACE_Y, TRUE );

		}

		// Render Name!
		SetFont( BLOCKFONT2 );

		if ( gpSMCurrentMerc->bStealthMode )
		{
			SetFontBackground( FONT_MCOLOR_BLACK );
			SetFontForeground( FONT_MCOLOR_LTYELLOW );								
		}
		else
		{
			SetFontBackground( FONT_MCOLOR_BLACK );
			SetFontForeground( FONT_MCOLOR_LTGRAY );
		}

		RestoreExternBackgroundRect( SM_SELMERCNAME_X, SM_SELMERCNAME_Y, SM_SELMERCNAME_WIDTH, SM_SELMERCNAME_HEIGHT );
		VarFindFontCenterCoordinates( SM_SELMERCNAME_X, SM_SELMERCNAME_Y, SM_SELMERCNAME_WIDTH, SM_SELMERCNAME_HEIGHT, SMALLFONT1, &sFontX, &sFontY, L"%s", gpSMCurrentMerc->name );
		mprintf( sFontX + 5, sFontY, L"%s", gpSMCurrentMerc->name );

	}

	if ( *pfDirty != DIRTYLEVEL0 )
	{
		// UPdate stats!
		if ( gpSMCurrentMerc->bLife != 0 )
		{
      if ( gpSMCurrentMerc->uiStatusFlags & SOLDIER_VEHICLE )
      {
			  swprintf( pStr, TacticalStr[ VEHICLE_VITAL_STATS_POPUPTEXT ], gpSMCurrentMerc->bLife, gpSMCurrentMerc->bLifeMax, gpSMCurrentMerc->bBreath, gpSMCurrentMerc->bBreathMax );
			  SetRegionFastHelpText( &(gSM_SELMERCBarsRegion), pStr );
      }
      else if ( gpSMCurrentMerc->uiStatusFlags & SOLDIER_ROBOT )
      {
			  swprintf( pStr, gzLateLocalizedString[ 16 ], gpSMCurrentMerc->bLife, gpSMCurrentMerc->bLifeMax );
			  SetRegionFastHelpText( &(gTEAM_BarsRegions[ cnt ]), pStr );
      }
      else
      {
			  GetMoraleString( gpSMCurrentMerc, pMoraleStr );
			  swprintf( pStr, TacticalStr[ MERC_VITAL_STATS_POPUPTEXT ], gpSMCurrentMerc->bLife, gpSMCurrentMerc->bLifeMax, gpSMCurrentMerc->bBreath, gpSMCurrentMerc->bBreathMax, pMoraleStr );
			  SetRegionFastHelpText( &(gSM_SELMERCBarsRegion), pStr );
      }
		}
		else
		{
			SetRegionFastHelpText( &(gSM_SELMERCBarsRegion), L"" );
		}

		//if we are in the shop keeper interface
		if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
			SetRegionHelpEndCallback( &gSM_SELMERCBarsRegion, SkiHelpTextDoneCallBack );

		// display AP
		if ( !( gpSMCurrentMerc->uiStatusFlags & SOLDIER_DEAD ) )
		{
			if ( gTacticalStatus.uiFlags & TURNBASED && (gTacticalStatus.uiFlags & INCOMBAT ) && gpSMCurrentMerc->bLife >= OKLIFE )
			{
				SetFont( TINYFONT1 );
				//if ( gpSMCurrentMerc->sLastTarget != NOWHERE && !EnoughPoints( gpSMCurrentMerc, MinAPsToAttack( gpSMCurrentMerc, gpSMCurrentMerc->sLastTarget, FALSE ), 0, FALSE ) || GetUIApsToDisplay( gpSMCurrentMerc ) < 0 ) 
				if ( !EnoughPoints( gpSMCurrentMerc, MinAPsToAttack( gpSMCurrentMerc, gpSMCurrentMerc->sLastTarget, FALSE ), 0, FALSE ) || GetUIApsToDisplay( gpSMCurrentMerc ) < 0 ) 
				{
					SetFontBackground( FONT_MCOLOR_BLACK );
					SetFontForeground( FONT_MCOLOR_DKRED );
				}
				else
				{
					if ( MercUnderTheInfluence( gpSMCurrentMerc ) )
					{
						SetFontBackground( FONT_MCOLOR_BLACK );
						SetFontForeground( FONT_MCOLOR_LTBLUE );
					}
					else if ( gpSMCurrentMerc->bStealthMode )
					{
						SetFontBackground( FONT_MCOLOR_BLACK );
						SetFontForeground( FONT_MCOLOR_LTYELLOW );								
					}
					else
					{
						SetFontBackground( FONT_MCOLOR_BLACK );
						SetFontForeground( FONT_MCOLOR_LTGRAY );
					}
				}

				gprintfRestore( SM_SELMERC_AP_X, SM_SELMERC_AP_Y, L"%2d", GetUIApsToDisplay( gpSMCurrentMerc ) );
				VarFindFontCenterCoordinates( SM_SELMERC_AP_X, SM_SELMERC_AP_Y, SM_SELMERC_AP_WIDTH, SM_SELMERC_AP_HEIGHT, TINYFONT1, &sFontX, &sFontY, L"%d", GetUIApsToDisplay( gpSMCurrentMerc ) );
				mprintf( sFontX, SM_SELMERC_AP_Y, L"%d", GetUIApsToDisplay( gpSMCurrentMerc ) );
			}

			// Display bars
			DrawLifeUIBarEx( gpSMCurrentMerc, SM_SELMERC_HEALTH_X, SM_SELMERC_HEALTH_Y, SM_SELMERC_HEALTH_WIDTH, SM_SELMERC_HEALTH_HEIGHT, TRUE , FRAME_BUFFER );

      if ( !(gpSMCurrentMerc->uiStatusFlags & SOLDIER_ROBOT ) )
      {
			  DrawBreathUIBarEx( gpSMCurrentMerc, SM_SELMERC_BREATH_X, SM_SELMERC_BREATH_Y, SM_SELMERC_HEALTH_WIDTH, SM_SELMERC_HEALTH_HEIGHT, TRUE, FRAME_BUFFER );
			  DrawMoraleUIBarEx( gpSMCurrentMerc, SM_SELMERC_MORALE_X, SM_SELMERC_MORALE_Y, SM_SELMERC_MORALE_WIDTH, SM_SELMERC_MORALE_HEIGHT, TRUE, FRAME_BUFFER );
      }
		}

	}

/*  I added this
 *  any questions? joker
 */
	if( fRenderRadarScreen == TRUE )
	{
		// Render clock
		RenderClock( INTERFACE_CLOCK_X, INTERFACE_CLOCK_Y );
		CreateMouseRegionForPauseOfClock( INTERFACE_CLOCK_X, INTERFACE_CLOCK_Y );
		RenderTownIDString( );
	}
	else
	{
		RemoveMouseRegionForPauseOfClock( );
	}

	UpdateSMPanel( );

  
	//HandlePanelFaceAnimations( gpSMCurrentMerc );

	HandleSoldierFaceFlash( gpSMCurrentMerc, SM_SELMERC_FACE_X, SM_SELMERC_FACE_Y );

	// Render items in guy's hand!
	HandleRenderInvSlots( gpSMCurrentMerc, *pfDirty );

	if ( gfSMDisableForItems && (*pfDirty) != DIRTYLEVEL0 )
	{
		UINT8	 *pDestBuf;
		UINT32 uiDestPitchBYTES;
		SGPRect ClipRect;

		ClipRect.iLeft	 = 87;
		ClipRect.iRight  = 536;
		ClipRect.iTop		= INV_INTERFACE_START_Y;
		ClipRect.iBottom = SCREEN_HEIGHT;
		pDestBuf = LockVideoSurface( FRAME_BUFFER, &uiDestPitchBYTES );
		Blt16BPPBufferHatchRect( (UINT16*)pDestBuf, uiDestPitchBYTES, &ClipRect );
		UnLockVideoSurface( FRAME_BUFFER );
	}

}

void UpdateStatColor( UINT32 uiTimer, BOOLEAN fIncrease )
{
	if ( gpSMCurrentMerc->bLife >= OKLIFE )
	{
		if( ( GetJA2Clock()  < CHANGE_STAT_RECENTLY_DURATION + uiTimer) && ( uiTimer != 0 ) )
		{
			if( fIncrease )
			{
				SetFontForeground( FONT_LTGREEN );
			}
			else
			{
				SetFontForeground( FONT_RED );
			}
		}
		else
		{
			SetFontBackground( FONT_MCOLOR_BLACK );
			SetFontForeground( STATS_TEXT_FONT_COLOR );
		}
	}
	else
	{
		SetFontBackground( FONT_MCOLOR_BLACK );
		SetFontForeground( FONT_MCOLOR_DKGRAY );
	}
}



void SMInvMoveCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT32 uiHandPos;

	uiHandPos = MSYS_GetRegionUserData( pRegion, 0 );

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}

	if ( gpSMCurrentMerc->inv[ uiHandPos ].usItem == NOTHING )
		return;

	if (iReason == MSYS_CALLBACK_REASON_MOVE)
	{
	}
	else if (iReason == MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		if ( gpItemPointer == NULL )
		{
			// Setup a timer....
			guiMouseOverItemTime = GetJA2Clock( );
			gfCheckForMouseOverItem = TRUE;
			gbCheckForMouseOverItemPos = (INT8)uiHandPos;
		}
	}
	if (iReason == MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		//gfSM_HandInvDispText[ uiHandPos ] = 1;
		if ( gpItemPointer == NULL )
		{
			HandleCompatibleAmmoUI( gpSMCurrentMerc, (INT8)uiHandPos, FALSE );
			gfCheckForMouseOverItem = FALSE;
			fInterfacePanelDirty		= DIRTYLEVEL2;
		}
	}	
}

void InvPanelButtonClickCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN)
	{

	}
}


void SMInvMoveCammoCallback( MOUSE_REGION * pRegion, INT32 iReason )
{

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}
	else if (iReason == MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		// Setup a timer....
		guiMouseOverItemTime = GetJA2Clock( );
		gfCheckForMouseOverItem = TRUE;
		gbCheckForMouseOverItemPos = NO_SLOT;
	}
	if (iReason == MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		//gfSM_HandInvDispText[ uiHandPos ] = 1;
		HandleCompatibleAmmoUI( gpSMCurrentMerc, (INT8)NO_SLOT, FALSE );
		gfCheckForMouseOverItem = FALSE;
	}	
}


void SMInvClickCamoCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	//UINT16 usNewItemIndex;
	UINT8	 ubSrcID, ubDestID;
  BOOLEAN fGoodAPs;

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		// Apply camo ( if we have something in cursor... )
		// If we do not have an item in hand, start moving it

		if ( gpItemPointer != NULL )
		{
			// ATE: OK, get source, dest guy if different... check for and then charge appropriate APs
			ubSrcID  = gpSMCurrentMerc->ubID;
			ubDestID = gpItemPointerSoldier->ubID;

			//if ( ubSrcID == ubDestID )
			{
				// We are doing this ourselve, continue
				if ( gpSMCurrentMerc->bLife >= CONSCIOUSNESS )
				{
					//usNewItemIndex = gpItemPointer->usItem;

					// Try to apply camo....
					if ( ApplyCammo( gpSMCurrentMerc, gpItemPointer, &fGoodAPs ) )
					{
						if ( fGoodAPs )
						{
						  // Dirty 
						  fInterfacePanelDirty = DIRTYLEVEL2;

						  // Check if it's the same now!
						  if ( gpItemPointer->ubNumberOfObjects == 0 )
						  {
							  gbCompatibleApplyItem = FALSE;
							  EndItemPointer( );
						  }

						  // Say OK acknowledge....
						  DoMercBattleSound( gpSMCurrentMerc, BATTLE_SOUND_COOL1 );
            }
					}
					else if ( ApplyCanteen( gpSMCurrentMerc, gpItemPointer, &fGoodAPs ) )
					{
						// Dirty 
            if ( fGoodAPs )
            {
						  fInterfacePanelDirty = DIRTYLEVEL2;

						  // Check if it's the same now!
						  if ( gpItemPointer->ubNumberOfObjects == 0 )
						  {
							  gbCompatibleApplyItem = FALSE;
							  EndItemPointer( );
						  }
            }
					}
					else if ( ApplyElixir( gpSMCurrentMerc, gpItemPointer, &fGoodAPs ) )
					{
            if ( fGoodAPs )
            {
  						// Dirty 
						  fInterfacePanelDirty = DIRTYLEVEL2;

						  // Check if it's the same now!
						  if ( gpItemPointer->ubNumberOfObjects == 0 )
						  {
							  gbCompatibleApplyItem = FALSE;
							  EndItemPointer( );
						  }

						  // Say OK acknowledge....
   					  DoMercBattleSound( gpSMCurrentMerc, BATTLE_SOUND_COOL1 );
            }
					}
					else if ( ApplyDrugs( gpSMCurrentMerc, gpItemPointer ) )
					{
						// Dirty 
						fInterfacePanelDirty = DIRTYLEVEL2;

						// Check if it's the same now!
						if ( gpItemPointer->ubNumberOfObjects == 0 )
						{
							gbCompatibleApplyItem = FALSE;
							EndItemPointer( );
						}

						/*
						// COMMENTED OUT DUE TO POTENTIAL SERIOUS PROBLEMS WITH STRATEGIC EVENT SYSTEM

						if ( gpSMCurrentMerc->ubProfile == LARRY_NORMAL )
						{
							// Larry's taken something!
							gpSMCurrentMerc = SwapLarrysProfiles( gpSMCurrentMerc );
						}
						*/


						// Say OK acknowledge....
						DoMercBattleSound( gpSMCurrentMerc, BATTLE_SOUND_COOL1 );

					}
					else
					{
						// Send message
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_UI_FEEDBACK, TacticalStr[ CANNOT_DO_INV_STUFF_STR ] );
					}
				}
			} 
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN)
	{

	}
}


BOOLEAN HandleNailsVestFetish( SOLDIERTYPE *pSoldier, UINT32 uiHandPos, UINT16 usReplaceItem )
{
	BOOLEAN fRefuse = FALSE;

	// OK are we nails?
	if ( pSoldier->ubProfile == 34 )
	{
		// if this the VEST POS?
		if ( uiHandPos == VESTPOS )
		{
			// Are we trying to pick it up?
			if ( usReplaceItem == NOTHING )
			{
				fRefuse = TRUE;
			}
			else
			{
				// Do we have nothing or the leather vest or kevlar leather vest?
				if ( Item[usReplaceItem].leatherjacket ||
						 usReplaceItem == COMPOUND18 ||
						 usReplaceItem == JAR_QUEEN_CREATURE_BLOOD )
				{
					// This is good....
					fRefuse = FALSE;
				}
				else
				{
					fRefuse = TRUE;			
				}
			}

			if ( fRefuse )
			{
				// Say quote!
				TacticalCharacterDialogue( pSoldier, 61 );
				return( TRUE );
			}
		}
	}

	return( FALSE );
}

BOOLEAN UIHandleItemPlacement( UINT8 ubHandPos, UINT16 usOldItemIndex, UINT16 usNewItemIndex, BOOLEAN fDeductPoints )
{
	if ( _KeyDown(CTRL) )
	{
		CleanUpStack( &( gpSMCurrentMerc->inv[ ubHandPos ] ), gpItemPointer );
		if ( gpItemPointer->ubNumberOfObjects == 0 )
		{
			EndItemPointer( );
		}
		return( TRUE );
	}

	// Try to place here			
	if ( PlaceObject( gpSMCurrentMerc, ubHandPos, gpItemPointer ) )
	{
		if ( fDeductPoints )
		{
			// Deduct points
			if ( gpItemPointerSoldier->bLife >= CONSCIOUSNESS )
			{
				DeductPoints( gpItemPointerSoldier,  2, 0 );
			}
			if ( gpSMCurrentMerc->bLife >= CONSCIOUSNESS )
			{
				DeductPoints( gpSMCurrentMerc,  2, 0 );
			}
		}

		HandleTacticalEffectsOfEquipmentChange( gpSMCurrentMerc, ubHandPos, usOldItemIndex, usNewItemIndex );

		// Dirty 
		fInterfacePanelDirty = DIRTYLEVEL2;

		// Check if cursor is empty now
		if ( gpItemPointer->ubNumberOfObjects == 0 )
		{
			EndItemPointer( );
		}

		if ( gpItemPointerSoldier != gpSMCurrentMerc )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_ITEM_PASSED_TO_MERC ], ShortItemNames[ usNewItemIndex ], gpSMCurrentMerc->name );		
		}

		// UPDATE ITEM POINTER.....
		gpItemPointerSoldier = gpSMCurrentMerc;

		if ( gpItemPointer != NULL )
		{
			ReevaluateItemHatches( gpSMCurrentMerc, FALSE );
		}

		// Set cursor back to normal mode...
		guiPendingOverrideEvent = A_CHANGE_TO_MOVE;	

		return( TRUE );

	}
	else
	{
		return( FALSE );
	}
	
}

void SMInvClickCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT32 uiHandPos;
	// Copyies of values
	UINT16 usOldItemIndex, usNewItemIndex;
	UINT16 usItemPrevInItemPointer;
	BOOLEAN fNewItem = FALSE;
	static BOOLEAN	fRightDown = FALSE;
	static BOOLEAN	fLeftDown = FALSE;

	uiHandPos = MSYS_GetRegionUserData( pRegion, 0 );

	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		return;
	}

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}

	//if we are in the shop keeper interface
	if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
	{
		// and this inventory slot is hatched out
		if( ShouldSoldierDisplayHatchOnItem( gpSMCurrentMerc->ubProfile, (INT16)uiHandPos ) )
		{
			// it means that item is a copy of one in the player's offer area, so we treat it as if the slot was empty (ignore)
			// if the cursor has an item in it, we still ignore the click, because handling swaps in this situation would be
			// ugly, we'd have to the the swap, then make the bOwnerSlot of the item just picked up a -1 in its offer area spot.
			return;
		}
	}


	//if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	//{
	//	fLeftDown = TRUE;
	//}
	//else if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP && fLeftDown )
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		fLeftDown = FALSE;

		// If we do not have an item in hand, start moving it
		if ( gpItemPointer == NULL )
		{

			// Return if empty
			if ( gpSMCurrentMerc->inv[ uiHandPos ].usItem == NOTHING )
				return;

			if ( gpSMCurrentMerc->ubID != gusSelectedSoldier )
			{
				SelectSoldier( gpSMCurrentMerc->ubID, FALSE, FALSE );
			}

			// OK, check if this is Nails, and we're in the vest position , don't allow it to come off....
			if ( HandleNailsVestFetish( gpSMCurrentMerc, uiHandPos, NOTHING ) )
			{
				return;
			}

			if ( _KeyDown(CTRL) )
			{
				CleanUpStack( &( gpSMCurrentMerc->inv[ uiHandPos ] ), NULL );
				return;
			}
			
			// Turn off new item glow!
			gpSMCurrentMerc->bNewItemCount[ uiHandPos ] = 0;

			usOldItemIndex = gpSMCurrentMerc->inv[ uiHandPos ].usItem;

			// move item into the mouse cursor
			BeginItemPointer( gpSMCurrentMerc, (UINT8)uiHandPos );

			//if we are in the shopkeeper interface
			if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
			{
				// pick up item from regular inventory slot into cursor OR try to sell it ( unless CTRL is held down )
				BeginSkiItemPointer( PLAYERS_INVENTORY, ( INT8 ) uiHandPos, ( BOOLEAN )!gfKeyState[ CTRL ] );
			}

			HandleTacticalEffectsOfEquipmentChange( gpSMCurrentMerc, uiHandPos, usOldItemIndex, NOTHING );

			// HandleCompatibleAmmoUI( gpSMCurrentMerc, (INT8)uiHandPos, FALSE );
		}
		else	// item in cursor
		{
			UINT8			ubSrcID, ubDestID;
			BOOLEAN		fOKToGo = FALSE;
			BOOLEAN		fDeductPoints = FALSE;

			// ATE: OK, get source, dest guy if different... check for and then charge appropriate APs
			ubSrcID  = gpSMCurrentMerc->ubID;
			ubDestID = gpItemPointerSoldier->ubID;

			if ( ubSrcID == ubDestID )
			{
				// We are doing this ourselve, continue
				fOKToGo = TRUE;
			} 
			else
			{
				// These guys are different....
				fDeductPoints = TRUE;

				// First check points for src guy
				if ( gpItemPointerSoldier->bLife >= CONSCIOUSNESS )
				{
					if ( EnoughPoints( gpItemPointerSoldier, 3, 0, TRUE ) )
					{
						fOKToGo = TRUE;
					}
				}
				else
				{
					fOKToGo = TRUE;
				}

				// Should we go on?
				if ( fOKToGo )
				{
					if ( gpSMCurrentMerc->bLife >= CONSCIOUSNESS )
					{
						if ( EnoughPoints( gpSMCurrentMerc, 3, 0, TRUE ) )
						{
							fOKToGo = TRUE;
						}
						else
						{
							fOKToGo = FALSE;
						}
					}
				}
			}

			if ( fOKToGo )
			{
				// OK, check if this is Nails, and we're in the vest position , don't allow it to come off....
				if ( HandleNailsVestFetish( gpSMCurrentMerc, uiHandPos, gpItemPointer->usItem ) )
				{
					return;
				}

				usOldItemIndex = gpSMCurrentMerc->inv[ uiHandPos ].usItem;
				usNewItemIndex = gpItemPointer->usItem;

				if ( uiHandPos == HANDPOS || uiHandPos == SECONDHANDPOS || uiHandPos == HELMETPOS || uiHandPos == VESTPOS || uiHandPos == LEGPOS )
				{
					//if ( ValidAttachmentClass( usNewItemIndex, usOldItemIndex ) )
					if ( ValidAttachment( usNewItemIndex, usOldItemIndex ) )
					{
						// it's an attempt to attach; bring up the inventory panel
						if ( !InItemDescriptionBox( ) )
						{
							InitItemDescriptionBox( gpSMCurrentMerc, (UINT8)uiHandPos, SM_ITEMDESC_START_X, SM_ITEMDESC_START_Y, 0 );
						}
						return;
					}
					else if ( ValidMerge( usNewItemIndex, usOldItemIndex ) )
					{
						// bring up merge requestor
						gubHandPos = (UINT8) uiHandPos;
						gusOldItemIndex = usOldItemIndex;
						gusNewItemIndex = usNewItemIndex;
						gfDeductPoints = fDeductPoints;

						if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
						{
							//the only way to merge items is to pick them up.  In SKI when you pick up an item, the cursor is
							//locked in a region, free it up.
							FreeMouseCursor();

							DoMessageBox( MSG_BOX_BASIC_STYLE, Message[ STR_MERGE_ITEMS ], SHOPKEEPER_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, MergeMessageBoxCallBack, NULL );
						}
						else
							DoMessageBox( MSG_BOX_BASIC_STYLE, Message[ STR_MERGE_ITEMS ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, MergeMessageBoxCallBack, NULL );
						return;
					}
					// else handle normally
				}


				// remember the item type currently in the item pointer
				usItemPrevInItemPointer = gpItemPointer->usItem;

				if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
				{
					// If it's just been purchased or repaired, mark it as a "new item"
					fNewItem = ( BOOLEAN ) ( gMoveingItem.uiFlags & ( ARMS_INV_JUST_PURCHASED | ARMS_INV_ITEM_REPAIRED ) );
				}

				// try to place the item in the cursor into this inventory slot
				if ( UIHandleItemPlacement( (UINT8) uiHandPos, usOldItemIndex, usNewItemIndex, fDeductPoints ) )
				{
					// it worked!  if we're in the SKI...
					if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
					{
						SetNewItem( gpSMCurrentMerc, ( UINT8 ) uiHandPos, fNewItem );

						// and the cursor is now empty
						if( gpItemPointer == NULL )
						{
							// clean up
							memset( &gMoveingItem, 0, sizeof( INVENTORY_IN_SLOT ) );
							SetSkiCursor( CURSOR_NORMAL );
						}
						else
						{
							// if we're holding something else in the pointer now
							if ( usItemPrevInItemPointer != gpItemPointer->usItem )
							{
								// pick up item swapped out of inventory slot into cursor (don't try to sell)
								BeginSkiItemPointer( PLAYERS_INVENTORY, -1, FALSE );
							}
							else
							{
								// otherwise, leave the cursor as is, means more items were picked up at once than can be placed in this slot
								// we deal with this by leaving the remainder in the cursor, to be put down elsewhere using subsequent clicks
							}
						}
					}

					// Setup a timer....
					//guiMouseOverItemTime = GetJA2Clock( );
					//gfCheckForMouseOverItem = TRUE;
					//gbCheckForMouseOverItemPos = (INT8)uiHandPos;
				}

				/*
				// Try to place here			
				if ( PlaceObject( gpSMCurrentMerc, (UINT8)uiHandPos, gpItemPointer ) )
				{
				
					if ( fDeductPoints )
					{
						// Deduct points
						if ( gpItemPointerSoldier->bLife >= CONSCIOUSNESS )
						{
							DeductPoints( gpItemPointerSoldier,  2, 0 );
						}
						if ( gpSMCurrentMerc->bLife >= CONSCIOUSNESS )
						{
							DeductPoints( gpSMCurrentMerc,  2, 0 );
						}
					}

					HandleTacticalEffectsOfEquipmentChange( gpSMCurrentMerc, uiHandPos, usOldItemIndex, usNewItemIndex );

					// Dirty 
					fInterfacePanelDirty = DIRTYLEVEL2;

					// Check if it's the same now!
					if ( gpItemPointer->ubNumberOfObjects == 0 )
					{
						EndItemPointer( );
					}

					// Setup a timer....
					guiMouseOverItemTime = GetJA2Clock( );
					gfCheckForMouseOverItem = TRUE;
					gbCheckForMouseOverItemPos = (INT8)uiHandPos;

				}
				*/
			}
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN)
	{
		fRightDown = TRUE;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP && fRightDown )
	{
		fRightDown = FALSE;

		// Return if empty
		if ( gpSMCurrentMerc->inv[ uiHandPos ].usItem == NOTHING )
			return;

		// CJC: OK, get source, dest guy if different, don't allow panels to be brought up
		/*
		if ( gpItemPointer && gpSMCurrentMerc->ubID != gpItemPointerSoldier->ubID )
		{
			return;
		}
		*/

		// Turn off new item glow!
		gpSMCurrentMerc->bNewItemCount[ uiHandPos ] = 0;

		// Some global stuff here - for esc, etc
		// Check for # of slots in item
		if( ( gpSMCurrentMerc->inv[ uiHandPos ].ubNumberOfObjects > 1 && ItemSlotLimit( gpSMCurrentMerc->inv[ uiHandPos ].usItem, (UINT8)uiHandPos ) > 0 ) && ( guiCurrentScreen != MAP_SCREEN ) )
		{
			if ( !InItemStackPopup( )  )
			{

				//InitItemStackPopup( gpSMCurrentMerc, (UINT8)uiHandPos, SM_ITEMDESC_START_X, SM_ITEMDESC_START_Y, SM_ITEMDESC_WIDTH, SM_ITEMDESC_HEIGHT );
				InitItemStackPopup( gpSMCurrentMerc, (UINT8)uiHandPos, 216, INV_INTERFACE_START_Y, 314, ( SCREEN_HEIGHT - INV_INTERFACE_START_Y ) );

			}
		}
		else
		{
			if ( !InItemDescriptionBox( ) )
			{
				InitItemDescriptionBox( gpSMCurrentMerc, (UINT8)uiHandPos, SM_ITEMDESC_START_X, SM_ITEMDESC_START_Y, 0 );
			}
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		fRightDown = FALSE;
		fLeftDown = FALSE;
	}

}

void MergeMessageBoxCallBack( UINT8 ubExitValue )
{
	if ( ubExitValue == MSG_BOX_RETURN_YES )
	{
		AttachObject( gpItemPointerSoldier, &( gpSMCurrentMerc->inv[ gubHandPos ] ), gpItemPointer );

		// re-evaluate repairs
		gfReEvaluateEveryonesNothingToDo = TRUE;

		if (gpItemPointer->usItem == NOTHING)
		{
			// merge item consumed
			EndItemPointer();
			fInterfacePanelDirty = DIRTYLEVEL2;
		}
	}
	else
	{
		UIHandleItemPlacement( gubHandPos, gusOldItemIndex, gusNewItemIndex, gfDeductPoints );
	}
}

void HandleMouseOverSoldierFaceForContMove( SOLDIERTYPE *pSoldier, BOOLEAN fOn )
{
	FACETYPE *pFace;
	INT16			sGridNo;

	if ( pSoldier == NULL )
	{
		return;
	}

	if ( fOn )
	{
		// Check if we are waiting to continue move...
		if ( CheckForMercContMove( pSoldier ) )
		{
			// Display 'cont' on face....
			// Get face
			pFace = &gFacesData[ pSoldier->iFaceIndex ];
			
			pFace->fDisplayTextOver = FACE_DRAW_TEXT_OVER;
			wcscpy( pFace->zDisplayText, TacticalStr[ CONTINUE_OVER_FACE_STR ] );

			sGridNo = pSoldier->sFinalDestination;

			if ( pSoldier->bGoodContPath )
			{
				sGridNo = pSoldier->sContPathLocation;
			}

			// While our mouse is here, draw a path!
			PlotPath( pSoldier, sGridNo, NO_COPYROUTE, PLOT, TEMPORARY, (UINT16)pSoldier->usUIMovementMode, NOT_STEALTH, FORWARD, pSoldier->bActionPoints );
		}
	}
	else
	{
		// Remove 'cont' on face....
		// Get face
		pFace = &gFacesData[ pSoldier->iFaceIndex ];
		
		pFace->fDisplayTextOver = FACE_ERASE_TEXT_OVER;

		// Erase path!
		ErasePath( TRUE );
	}

	fInterfacePanelDirty = DIRTYLEVEL2;
}

void SelectedMercButtonMoveCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	if ( gpSMCurrentMerc == NULL )
	{
		return;
	}

	if (iReason & MSYS_CALLBACK_REASON_MOVE )
	{
		HandleMouseOverSoldierFaceForContMove( gpSMCurrentMerc, TRUE );
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		HandleMouseOverSoldierFaceForContMove( gpSMCurrentMerc, FALSE );
	}

}




void SelectedMercButtonCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	SOLDIERTYPE *pVehicle;

	if ( gpSMCurrentMerc == NULL )
	{
		return;
	}

	//if we are in the shop keeper interface
	if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
		return;

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		// ATE: Don't if this guy can't....
		if ( !gfSMDisableForItems )
		{
			if ( gpSMCurrentMerc->uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) )
			{
				pVehicle = GetSoldierStructureForVehicle( gpSMCurrentMerc->iVehicleId );

				HandleLocateSelectMerc( pVehicle->ubID, 0 );
			}
			else
			{
				if ( CheckForMercContMove( gpSMCurrentMerc ) )
				{
					// Continue
					ContinueMercMovement( gpSMCurrentMerc );
					ErasePath( TRUE );
				}
				else	
				{
					HandleLocateSelectMerc( gpSMCurrentMerc->ubID, 0 );
				}
			}
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN)
	{
		// ATE: Cannot get out by right clicking...
		//if ( gpItemPointer == NULL )
		{
			//if ( !gfSMDisableForItems || ( gfSMDisableForItems && gpItemPointer == NULL ) )
			{
				// Delete desc
				if ( InItemDescriptionBox( ) )
				{
					DeleteItemDescriptionBox( );
				}
			
				gfSwitchPanel = TRUE;
				gbNewPanel		= TEAM_PANEL;
			}
		}
	}
}


void SelectedMercEnemyIndicatorCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	if ( gpSMCurrentMerc == NULL )
	{
		return;
	}

	//if we are in the shop keeper interface
	if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
		return;

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		// ATE: Don't if this guy can't....
		if ( !gfSMDisableForItems )
		{
			if ( gpSMCurrentMerc->uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) )
			{
			}
			else
			{
				if ( gpSMCurrentMerc->bOppCnt > 0 )
				{
					CycleVisibleEnemies( gpSMCurrentMerc );
				}
				else
				{
					SelectedMercButtonCallback( pRegion, iReason );
				}
			}
		}
	}
}



void BtnStanceUpCallback(GUI_BUTTON *btn,INT32 reason)
{
	INT8 bNewStance;

	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		bNewStance = gAnimControl[ gpSMCurrentMerc->usAnimState ].ubEndHeight;

		if ( bNewStance == ANIM_CROUCH )
		{
			bNewStance = ANIM_STAND;
		}
		else if ( bNewStance == ANIM_PRONE )
		{
			bNewStance = ANIM_CROUCH;
		}

		UIHandleSoldierStanceChange( gpSMCurrentMerc->ubID, bNewStance );

	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{

		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}
	
}


void BtnUpdownCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		//gsInterfaceLevel = gpSMCurrentMerc->bUIInterfaceLevel;

		// Change interface level via HandleUI handler
		UIHandleChangeLevel( NULL );

		// Remember soldier's new value
		gpSMCurrentMerc->bUIInterfaceLevel = (INT8)gsInterfaceLevel;
	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}

}
	
void BtnClimbCallback(GUI_BUTTON *btn,INT32 reason)
{
	BOOLEAN						fNearHeigherLevel;
	BOOLEAN						fNearLowerLevel;
	INT8							bDirection;

	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		GetMercClimbDirection( gpSMCurrentMerc->ubID, &fNearLowerLevel, &fNearHeigherLevel );

		if ( fNearLowerLevel )
		{
			BeginSoldierClimbDownRoof( gpSMCurrentMerc );
		}
		if ( fNearHeigherLevel )
		{
			BeginSoldierClimbUpRoof( gpSMCurrentMerc );
		}

		if ( FindFenceJumpDirection( gpSMCurrentMerc, gpSMCurrentMerc->sGridNo, gpSMCurrentMerc->bDirection, &bDirection ) )
		{
			BeginSoldierClimbFence( gpSMCurrentMerc );
		}

	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}
	
}

void BtnStanceDownCallback(GUI_BUTTON *btn,INT32 reason)
{
	INT8 bNewStance;

	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		bNewStance = gAnimControl[ gpSMCurrentMerc->usAnimState ].ubEndHeight;

		if ( bNewStance == ANIM_STAND )
		{
			bNewStance = ANIM_CROUCH;
		}
		else if ( bNewStance == ANIM_CROUCH )
		{
			bNewStance = ANIM_PRONE;
		}

		UIHandleSoldierStanceChange( gpSMCurrentMerc->ubID, bNewStance );

	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}
	
}

void BtnStealthModeCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		gpSMCurrentMerc->bStealthMode = !	gpSMCurrentMerc->bStealthMode;
		gfUIStanceDifferent		= TRUE;
		gfPlotNewMovement			= TRUE;
		fInterfacePanelDirty	= DIRTYLEVEL2;
	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}

}


void BtnHandCursorCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		ToggleHandCursorMode( &guiCurrentEvent );
	}

}

void BtnTalkCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
			ToggleTalkCursorMode( &guiCurrentEvent );
	}

}

void BtnMuteCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if (reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if ( gpSMCurrentMerc->uiStatusFlags & SOLDIER_MUTE )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ MUTE_OFF_STR ], gpSMCurrentMerc->name );
			gpSMCurrentMerc->uiStatusFlags &= ( ~SOLDIER_MUTE );
		}
		else
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, TacticalStr[ MUTE_ON_STR ], gpSMCurrentMerc->name );
			gpSMCurrentMerc->uiStatusFlags |= ( SOLDIER_MUTE );
		}
	}

}


void BtnPrevMercCallback(GUI_BUTTON *btn,INT32 reason)
{
	INT16 sID;

	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		sID = FindPrevActiveAndAliveMerc( gpSMCurrentMerc, TRUE, TRUE );

		gubSelectSMPanelToMerc = (UINT8)sID;

		if ( !gfInItemPickupMenu )
		{
			//if we are in the shop keeper interface
			if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
			{
				LocateSoldier( sID, DONTSETLOCATOR );
				// refresh background for player slots (in case item values change due to Flo's discount)
				gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
			}
			else
				LocateSoldier( sID, SETLOCATOR );
		}

		//if the user is in the shop keeper interface and is in the item desc
		if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
		{
			if ( InItemDescriptionBox( ) )
			{
				DeleteItemDescriptionBox( );
			}
		}
	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}
	
}

void BtnNextMercCallback(GUI_BUTTON *btn,INT32 reason)
{
	INT16 sID;
	
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		sID = FindNextActiveAndAliveMerc( gpSMCurrentMerc, TRUE, TRUE );

		// Give him the panel!
		gubSelectSMPanelToMerc = (UINT8)sID;

		if ( !gfInItemPickupMenu )
		{
			//if we are in the shop keeper interface
			if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
			{
				LocateSoldier( sID, DONTSETLOCATOR );
				// refresh background for player slots (in case item values change due to Flo's discount)
				gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
			}
			else
				LocateSoldier( sID, SETLOCATOR );
		}

		//if the user is in the shop keeper interface and is in the item desc
		if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
		{
			if ( InItemDescriptionBox( ) )
			{
				DeleteItemDescriptionBox( );
			}
		}
	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}
	
}


void BtnOptionsCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		guiPreviousOptionScreen = guiCurrentScreen;
		LeaveTacticalScreen( OPTIONS_SCREEN );

	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}
	
}

void BtnSMDoneCallback(GUI_BUTTON *btn,INT32 reason)
{

	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		gfBeginEndTurn = TRUE;
	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}
	
}


void BtnMapScreenCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		// Enter mapscreen...
		//gfEnteringMapScreen = TRUE;
		GoToMapScreenFromTactical();
	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}
	
}

/*
uiTempVObject[0] = LoadButtonImage( "Interface\\InventoryButtons.sti", -1, 7, -1, -1, -1 );
uiTempVObject[1] = UseLoadedButtonImage( uiTempVObject[0], -1, 17, -1, -1, -1 );
uiTempVObject[2] = UseLoadedButtonImage( uiTempVObject[0], -1, 26, -1, -1, -1 );
iButtonID = QuickCreateButton( uiTempVObject[0], 0, 0, BUTTON_NO_TOGGLE, MSYS_PRIORITY_HIGH, MSYS_NO_CALLBACK, CycleCallback );

void CycleCallback( GUI_BUTTON *btn, INT32 reason )
{
	static INT32 buttonValue = 0;
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		buttonValue += buttonValue < 2 ? 1 : -2;
		btn->ImageNum = uiTempVObject[ buttonValue ];
		btn->uiFlags |= BUTTON_DIRTY;
	}
}
*/

void BtnBurstModeCallback(GUI_BUTTON *btn,INT32 reason)
{
	if (!(btn->uiFlags & BUTTON_ENABLED))
		return;

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		ChangeWeaponMode( gpSMCurrentMerc );
//		btn->ImageNum = iBurstButtonImages[ gpSMCurrentMerc->bWeaponMode ];
//		btn->uiFlags |= BUTTON_DIRTY;

	}
	
}

void BtnLookCallback(GUI_BUTTON *btn,INT32 reason)
{

	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		ToggleLookCursorMode( NULL );
		
	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}
	
}

void BtnPositionShowCallback(GUI_BUTTON *btn,INT32 reason)
{

}

// WANNE 2
BOOLEAN InitializeTEAMPanelCoords( )
{

	TM_FACE_WIDTH		= 48;
	TM_FACE_HEIGHT		= 43;
	TM_APPANEL_HEIGHT	= 56;
	TM_APPANEL_WIDTH	= 16;

	// WANNE 2
	TM_ENDTURN_X		=	(SCREEN_WIDTH - 133);		//( 507 + INTERFACE_START_X );
	TM_ENDTURN_Y		= ( 9 + INTERFACE_START_Y );
	TM_ROSTERMODE_X	=		(SCREEN_WIDTH - 133);		//( 507 + INTERFACE_START_X );
	TM_ROSTERMODE_Y	= ( 45 + INTERFACE_START_Y );
	TM_DISK_X			=	(SCREEN_WIDTH - 133);		//( 507 + INTERFACE_START_X );
	TM_DISK_Y			= ( 81 + INTERFACE_START_Y );

	TM_NAME_WIDTH		= 60;
	TM_NAME_HEIGHT		= 9;
	TM_LIFEBAR_WIDTH	= 3;
	TM_LIFEBAR_HEIGHT	= 42;
	TM_FACEHIGHTL_WIDTH = 84;
	TM_FACEHIGHTL_HEIGHT = 114;
	TM_AP_HEIGHT		= 10;
	TM_AP_WIDTH		= 15;

	TM_INV_WIDTH		= 58;
	TM_INV_HEIGHT		= 23;
	TM_INV_HAND1STARTX = ( 8 + INTERFACE_START_X );
	TM_INV_HAND1STARTY = ( 67 + INTERFACE_START_Y );
	TM_INV_HAND2STARTX = ( 8 + INTERFACE_START_X );
	TM_INV_HAND2STARTY = ( 93 + INTERFACE_START_Y );
	TM_INV_HAND_SEP	= 83;
	TM_INV_HAND_SEPY	= 24;

	TM_BARS_REGION_HEIGHT	= 47;
	TM_BARS_REGION_WIDTH	= 26;

	INDICATOR_BOX_WIDTH	 = 12;
	INDICATOR_BOX_HEIGHT = 10;

	sTEAMAPPanelXY[0] = ( 69  + INTERFACE_START_X );	sTEAMAPPanelXY[1] = ( 6 + INTERFACE_START_Y );
	sTEAMAPPanelXY[2] = ( 151 + INTERFACE_START_X );	sTEAMAPPanelXY[3] = ( 6 + INTERFACE_START_Y );
	sTEAMAPPanelXY[4] = ( 234 + INTERFACE_START_X );	sTEAMAPPanelXY[5] = ( 6 + INTERFACE_START_Y );
	sTEAMAPPanelXY[6] = ( 317 + INTERFACE_START_X );	sTEAMAPPanelXY[7] = ( 6 + INTERFACE_START_Y );
	sTEAMAPPanelXY[8] = ( 401 + INTERFACE_START_X );	sTEAMAPPanelXY[9] = ( 6 + INTERFACE_START_Y );
	sTEAMAPPanelXY[10] = ( 484 + INTERFACE_START_X );	sTEAMAPPanelXY[11] = ( 6 + INTERFACE_START_Y );

	sTEAMFacesXY[0] = ( 13 + INTERFACE_START_X );	sTEAMFacesXY[1] = ( 6 + INTERFACE_START_Y );
	sTEAMFacesXY[2] = ( 97 + INTERFACE_START_X );	sTEAMFacesXY[3] = ( 6 + INTERFACE_START_Y );
	sTEAMFacesXY[4] = ( 180 + INTERFACE_START_X );	sTEAMFacesXY[5] = ( 6 + INTERFACE_START_Y );
	sTEAMFacesXY[6] = ( 263 + INTERFACE_START_X );	sTEAMFacesXY[7] = ( 6 + INTERFACE_START_Y );
	sTEAMFacesXY[8] = ( 346 + INTERFACE_START_X );	sTEAMFacesXY[9] = ( 6 + INTERFACE_START_Y );
	sTEAMFacesXY[10] = ( 429 + INTERFACE_START_X );	sTEAMFacesXY[11] = ( 6 + INTERFACE_START_Y );

	sTEAMNamesXY[0] = ( 7 + INTERFACE_START_X );	sTEAMNamesXY[1] = ( 55 + INTERFACE_START_Y );
	sTEAMNamesXY[2] = ( 90 + INTERFACE_START_X );	sTEAMNamesXY[3] = ( 55 + INTERFACE_START_Y );
	sTEAMNamesXY[4] = ( 173 + INTERFACE_START_X );	sTEAMNamesXY[5] = ( 55 + INTERFACE_START_Y );
	sTEAMNamesXY[6] = ( 256 + INTERFACE_START_X );	sTEAMNamesXY[7] = ( 55 + INTERFACE_START_Y );
	sTEAMNamesXY[8] = ( 339 + INTERFACE_START_X );	sTEAMNamesXY[9] = ( 55 + INTERFACE_START_Y );
	sTEAMNamesXY[10] = ( 422 + INTERFACE_START_X );	sTEAMNamesXY[11] = ( 55 + INTERFACE_START_Y );

	sTEAMFaceHighlXY[0] = ( 4 + INTERFACE_START_X );	sTEAMFaceHighlXY[1] = ( 2 + INTERFACE_START_Y );
	sTEAMFaceHighlXY[2] = ( 87 + INTERFACE_START_X );	sTEAMFaceHighlXY[3] = ( 2 + INTERFACE_START_Y );
	sTEAMFaceHighlXY[4] = ( 170 + INTERFACE_START_X );	sTEAMFaceHighlXY[5] = ( 2 + INTERFACE_START_Y );
	sTEAMFaceHighlXY[6] = ( 253 + INTERFACE_START_X );	sTEAMFaceHighlXY[7] = ( 2 + INTERFACE_START_Y );
	sTEAMFaceHighlXY[8] = ( 336 + INTERFACE_START_X );	sTEAMFaceHighlXY[9] = ( 2 + INTERFACE_START_Y );
	sTEAMFaceHighlXY[10] = ( 419 + INTERFACE_START_X );	sTEAMFaceHighlXY[11] = ( 2 + INTERFACE_START_Y );

	sTEAMLifeXY[0] = ( 69 + INTERFACE_START_X );	sTEAMLifeXY[1] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMLifeXY[2] = ( 152 + INTERFACE_START_X );	sTEAMLifeXY[3] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMLifeXY[4] = ( 235 + INTERFACE_START_X );	sTEAMLifeXY[5] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMLifeXY[6] = ( 318 + INTERFACE_START_X );	sTEAMLifeXY[7] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMLifeXY[8] = ( 401 + INTERFACE_START_X );	sTEAMLifeXY[9] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMLifeXY[10] = ( 484 + INTERFACE_START_X );	sTEAMLifeXY[11] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );

	sTEAMBreathXY[0] = ( 75 + INTERFACE_START_X );	sTEAMBreathXY[1] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMBreathXY[2] = ( 158 + INTERFACE_START_X );	sTEAMBreathXY[3] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMBreathXY[4] = ( 241 + INTERFACE_START_X );	sTEAMBreathXY[5] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMBreathXY[6] = ( 324 + INTERFACE_START_X );	sTEAMBreathXY[7] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMBreathXY[8] = ( 407 + INTERFACE_START_X );	sTEAMBreathXY[9] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMBreathXY[10] = ( 490 + INTERFACE_START_X );sTEAMBreathXY[11] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );

	sTEAMMoraleXY[0] = ( 81 + INTERFACE_START_X );	sTEAMMoraleXY[1] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMMoraleXY[2] = ( 164 + INTERFACE_START_X );	sTEAMMoraleXY[3] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMMoraleXY[4] = ( 247 + INTERFACE_START_X );	sTEAMMoraleXY[5] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMMoraleXY[6] = ( 330 + INTERFACE_START_X );	sTEAMMoraleXY[7] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMMoraleXY[8] = ( 413 + INTERFACE_START_X );	sTEAMMoraleXY[9] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );
	sTEAMMoraleXY[10] = ( 496 + INTERFACE_START_X );sTEAMMoraleXY[11] = ( ( 5 + INTERFACE_START_Y ) + TM_LIFEBAR_HEIGHT );

	sTEAMApXY[0] = ( 68 + INTERFACE_START_X );	sTEAMApXY[1] = ( 52 + INTERFACE_START_Y );
	sTEAMApXY[2] = ( 152 + INTERFACE_START_X );	sTEAMApXY[3] = ( 52 + INTERFACE_START_Y );
	sTEAMApXY[4] = ( 233 + INTERFACE_START_X );	sTEAMApXY[5] = ( 52 + INTERFACE_START_Y );
	sTEAMApXY[6] = ( 317 + INTERFACE_START_X );	sTEAMApXY[7] = ( 52 + INTERFACE_START_Y );
	sTEAMApXY[8] = ( 400 + INTERFACE_START_X );	sTEAMApXY[9] = ( 52 + INTERFACE_START_Y );
	sTEAMApXY[10] = ( 483 + INTERFACE_START_X );sTEAMApXY[11] = ( 52 + INTERFACE_START_Y ); 

	sTEAMBarsXY[0] = ( 61 + INTERFACE_START_X );	sTEAMBarsXY[1] = ( 3 + INTERFACE_START_Y );
	sTEAMBarsXY[2] = ( 145 + INTERFACE_START_X );	sTEAMBarsXY[3] = ( 3 + INTERFACE_START_Y );
	sTEAMBarsXY[4] = ( 228 + INTERFACE_START_X );	sTEAMBarsXY[5] = ( 3 + INTERFACE_START_Y );
	sTEAMBarsXY[6] = ( 311 + INTERFACE_START_X );	sTEAMBarsXY[7] = ( 3 + INTERFACE_START_Y );
	sTEAMBarsXY[8] = ( 394 + INTERFACE_START_X );	sTEAMBarsXY[9] = ( 3 + INTERFACE_START_Y );
	sTEAMBarsXY[10] = ( 477 + INTERFACE_START_X );	sTEAMBarsXY[11] = ( 3 + INTERFACE_START_Y ); 

	sTEAMHandInvXY[0] = ( TM_INV_HAND1STARTX + ( 0 * TM_INV_HAND_SEP ));	sTEAMHandInvXY[1] = TM_INV_HAND1STARTY;
	sTEAMHandInvXY[2] = ( TM_INV_HAND1STARTX + ( 1 * TM_INV_HAND_SEP ));	sTEAMHandInvXY[3] = TM_INV_HAND1STARTY;
	sTEAMHandInvXY[4] = ( TM_INV_HAND1STARTX + ( 2 * TM_INV_HAND_SEP ));	sTEAMHandInvXY[5] = TM_INV_HAND1STARTY;
	sTEAMHandInvXY[6] = ( TM_INV_HAND1STARTX + ( 3 * TM_INV_HAND_SEP ));	sTEAMHandInvXY[7] = TM_INV_HAND1STARTY;
	sTEAMHandInvXY[8] = ( TM_INV_HAND1STARTX + ( 4 * TM_INV_HAND_SEP ));	sTEAMHandInvXY[9] = TM_INV_HAND1STARTY;
	sTEAMHandInvXY[10] = ( TM_INV_HAND1STARTX + ( 5 * TM_INV_HAND_SEP ));	sTEAMHandInvXY[11] = TM_INV_HAND1STARTY;
	// ufff to much copy&paste :D

	return ( TRUE );
}
// TEAM PANEL!!!!!!!!!!!!!!
BOOLEAN InitializeTEAMPanel(  )
{
	VSURFACE_DESC		vs_desc;
  VOBJECT_DESC    VObjectDesc;
	UINT32					cnt, posIndex;
	static BOOLEAN	fFirstTime = TRUE;

	// WANNE 2
	fDisplayOverheadMap = TRUE;

/*  OK i need to initialize coords here
 *  Isnt it cool
 *  any questions? joker
 *  i moved it
 */
//	InitializeTEAMPanelCoords( );
	// INit viewport region
	// Set global mouse regions
	// Define region for viewport
	MSYS_DefineRegion( &gViewportRegion, 0, 0 ,gsVIEWPORT_END_X, gsVIEWPORT_WINDOW_END_Y, MSYS_PRIORITY_NORMAL, VIDEO_NO_CURSOR, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK );
	MSYS_AddRegion( &gViewportRegion );
	
	// Load interface panels
	vs_desc.fCreateFlags = VSURFACE_CREATE_FROMFILE | VSURFACE_SYSTEM_MEM_USAGE;

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;

	if (iResolution == 0)
	{
	FilenameForBPP("INTERFACE\\bottom_bar.sti", VObjectDesc.ImageFile);
	}
	else if (iResolution == 1)
	{
		FilenameForBPP("INTERFACE\\bottom_bar_800x600.sti", VObjectDesc.ImageFile);
	}
	else if (iResolution == 2)
	{
		FilenameForBPP("INTERFACE\\bottom_bar_1024x768.sti", VObjectDesc.ImageFile);
	}

	CHECKF( AddVideoObject( &VObjectDesc, &guiTEAMPanel ) );

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\gold_front.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &guiTEAMObjects ) );

	VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("INTERFACE\\Bars.sti", VObjectDesc.ImageFile);
	CHECKF( AddVideoObject( &VObjectDesc, &guiBrownBackgroundForTeamPanel ) );

	// Clear inv display stuff
	memset( gfTEAM_HandInvDispText, 0, sizeof( gfTEAM_HandInvDispText ) );


	// Create buttons
	CHECKF( CreateTEAMPanelButtons( ) );

	// Set viewports
	// Define region for panel
	MSYS_DefineRegion( &gTEAM_PanelRegion, 0, gsVIEWPORT_END_Y ,SCREEN_WIDTH, SCREEN_HEIGHT, MSYS_PRIORITY_NORMAL,
						 CURSOR_NORMAL, MSYS_NO_CALLBACK, MSYS_NO_CALLBACK ); 
	// Add region
	MSYS_AddRegion( &gTEAM_PanelRegion);

	for ( posIndex = 0, cnt = 0; cnt < 6; cnt++, posIndex +=2 )
	{
		MSYS_DefineRegion( &gTEAM_FaceRegions[ cnt ], sTEAMFacesXY[ posIndex ], sTEAMFacesXY[ posIndex + 1 ] ,(INT16)(sTEAMFacesXY[ posIndex ] + TM_FACE_WIDTH ), (INT16)(sTEAMFacesXY[ posIndex + 1 ] + TM_FACE_HEIGHT), MSYS_PRIORITY_NORMAL,
							 MSYS_NO_CURSOR, MercFacePanelMoveCallback, MercFacePanelCallback ); 
		// Add region
		MSYS_AddRegion( &gTEAM_FaceRegions[ cnt ] );
		MSYS_SetRegionUserData( &gTEAM_FaceRegions[ cnt ], 0, cnt );


		MSYS_DefineRegion( &gTEAM_EnemyIndicator[ cnt ], (INT16)(sTEAMFacesXY[ posIndex ] + 1), (INT16)(sTEAMFacesXY[ posIndex + 1 ] + 1 ) ,(INT16)(sTEAMFacesXY[ posIndex ] + INDICATOR_BOX_WIDTH ), (INT16)(sTEAMFacesXY[ posIndex + 1 ] + INDICATOR_BOX_HEIGHT), MSYS_PRIORITY_NORMAL,
							 MSYS_NO_CURSOR, MSYS_NO_CALLBACK, EnemyIndicatorClickCallback ); 
		// Add region
		MSYS_AddRegion( &gTEAM_EnemyIndicator[ cnt ] );
		MSYS_SetRegionUserData( &gTEAM_EnemyIndicator[ cnt ], 0, cnt );



		if ( IsMouseInRegion( &gTEAM_FaceRegions[ cnt ] ) )
		{
			if ( gTeamPanel[ cnt ].ubID != NOBODY )
			{
				HandleMouseOverSoldierFaceForContMove( MercPtrs[ gTeamPanel[ cnt ].ubID ], TRUE );
			}
		}


		MSYS_DefineRegion( &gTEAM_BarsRegions[ cnt ], sTEAMBarsXY[ posIndex ], sTEAMBarsXY[ posIndex + 1 ] ,(INT16)(sTEAMBarsXY[ posIndex ] + TM_BARS_REGION_WIDTH ), (INT16)(sTEAMBarsXY[ posIndex + 1 ] + TM_BARS_REGION_HEIGHT ), MSYS_PRIORITY_NORMAL,
							 MSYS_NO_CURSOR, MSYS_NO_CALLBACK, MercFacePanelCallback ); 
		// Add region
		MSYS_AddRegion( &gTEAM_BarsRegions[ cnt ] );
		MSYS_SetRegionUserData( &gTEAM_BarsRegions[ cnt ], 0, cnt );

		MSYS_DefineRegion( &gTEAM_LeftBarsRegions[ cnt ], (INT16)(sTEAMFacesXY[ posIndex ] - 8 ), sTEAMFacesXY[ posIndex + 1 ] ,(INT16)(sTEAMFacesXY[ posIndex ] ), (INT16)(sTEAMFacesXY[ posIndex + 1 ] + TM_BARS_REGION_HEIGHT ), MSYS_PRIORITY_NORMAL,
							 MSYS_NO_CURSOR, MSYS_NO_CALLBACK, MercFacePanelCallback ); 
		// Add region
		MSYS_AddRegion( &gTEAM_LeftBarsRegions[ cnt ] );
		MSYS_SetRegionUserData( &gTEAM_LeftBarsRegions[ cnt ], 0, cnt );


		MSYS_DefineRegion( &gTEAM_FirstHandInv[ cnt ], sTEAMHandInvXY[ posIndex ], sTEAMHandInvXY[ posIndex + 1 ], (INT16)(sTEAMHandInvXY[ posIndex ] + TM_INV_WIDTH ) ,(INT16)(sTEAMHandInvXY[ posIndex + 1 ] + TM_INV_HEIGHT ), MSYS_PRIORITY_NORMAL,
							 MSYS_NO_CURSOR, MSYS_NO_CALLBACK, TMClickFirstHandInvCallback ); 
		// Add region
		MSYS_AddRegion( &gTEAM_FirstHandInv[ cnt ] );

		// Add user data
		MSYS_SetRegionUserData( &gTEAM_FirstHandInv[ cnt ], 0, cnt );



		MSYS_DefineRegion( &gTEAM_SecondHandInv[ cnt ], sTEAMHandInvXY[ posIndex ], (INT16)( sTEAMHandInvXY[ posIndex + 1 ] + 24 ), (INT16)(sTEAMHandInvXY[ posIndex ] + TM_INV_WIDTH) ,(INT16)(sTEAMHandInvXY[ posIndex + 1 ] + TM_INV_HEIGHT + 24 ), MSYS_PRIORITY_NORMAL,
							 MSYS_NO_CURSOR, MSYS_NO_CALLBACK, TMClickSecondHandInvCallback ); 
		// Add region
		MSYS_AddRegion( &gTEAM_SecondHandInv[ cnt ] );

		// Add user data
		MSYS_SetRegionUserData( &gTEAM_SecondHandInv[ cnt ], 0, cnt );
		
	}


	//DEfine region for selected guy panel
	//MSYS_DefineRegion( &gSM_SELMERCPanalRegion, SM_SELMERC_FACE_X, SM_SELMERC_FACE_Y, SM_SELMERC_FACE_X + SM_SELMERC_FACE_WIDTH, SM_SELMERC_FACE_Y + SM_SELMERC_FACE_HEIGHT, MSYS_PRIORITY_NORMAL,
	//					 CURSOR_NORMAL, MSYS_NO_CALLBACK, SelectedMercButtonCallback ); 
	// Add region
	//MSYS_AddRegion( &gSM_SELMERCPanalRegion );

	return( TRUE );
}

BOOLEAN ShutdownTEAMPanel( )
{
	UINT32					cnt;

	// All buttons and regions and video objects and video surfaces will be deleted at shutddown of SGM
	// We may want to delete them at the interm as well, to free up room for other panels
	// Delete video objects!
	DeleteVideoObjectFromIndex( guiTEAMPanel );
	DeleteVideoObjectFromIndex( guiTEAMObjects );
	DeleteVideoObjectFromIndex( guiBrownBackgroundForTeamPanel );


	MSYS_RemoveRegion( &gTEAM_PanelRegion);

	// Remove viewport
	MSYS_RemoveRegion( &gViewportRegion );

	for ( cnt = 0; cnt < 6; cnt++ )
	{
		MSYS_RemoveRegion( &gTEAM_EnemyIndicator[ cnt ] );
		MSYS_RemoveRegion( &gTEAM_FaceRegions[ cnt ] );
		MSYS_RemoveRegion( &gTEAM_BarsRegions[ cnt ] );
		MSYS_RemoveRegion( &gTEAM_LeftBarsRegions[ cnt ] );
		MSYS_RemoveRegion( &gTEAM_FirstHandInv[ cnt ] );
		MSYS_RemoveRegion( &gTEAM_SecondHandInv[ cnt ] );

		if ( gTeamPanel[ cnt ].ubID != NOBODY )
		{
			HandleMouseOverSoldierFaceForContMove( MercPtrs[ gTeamPanel[ cnt ].ubID ], FALSE );
		}

	}

	// Remove buttons
	RemoveTEAMPanelButtons( );

	// turn off the display of the squad list
	if( fRenderRadarScreen == FALSE )
	{
		// start rendering radar region again, 
	  fRenderRadarScreen = TRUE;

		// remove squad panel
		CreateDestroyMouseRegionsForSquadList( );
	}
	return( TRUE );
}


void RenderTEAMPanel( BOOLEAN fDirty )
{
	INT16 sFontX, sFontY;
	UINT32				cnt, posIndex;
	SOLDIERTYPE		*pSoldier;
	static				CHAR16		pStr[ 200 ], pMoraleStr[ 20 ];

	// WANNE 2 <new>
	if ( fDirty == DIRTYLEVEL2 )
	{
		MarkAButtonDirty( iTEAMPanelButtons[ TEAM_DONE_BUTTON ] );	
		MarkAButtonDirty( iTEAMPanelButtons[ TEAM_MAP_SCREEN_BUTTON ] );	
		MarkAButtonDirty( iTEAMPanelButtons[ CHANGE_SQUAD_BUTTON ] );	


		// Blit video surface
		BltVideoObjectFromIndex( guiSAVEBUFFER, guiTEAMPanel, 0, INTERFACE_START_X, INTERFACE_START_Y, VO_BLT_SRCTRANSPARENCY, NULL );
		RestoreExternBackgroundRect( INTERFACE_START_X, INTERFACE_START_Y, SCREEN_WIDTH - INTERFACE_START_X , INTERFACE_HEIGHT );
		
		// WANNE 2 <new> <begin>
		// LOOP THROUGH ALL MERCS ON TEAM PANEL
		for ( cnt = 0, posIndex = 0; cnt < NUM_TEAM_SLOTS; cnt++, posIndex+= 2 )
		{
			// GET SOLDIER
			if ( !gTeamPanel[ cnt ].fOccupied )
			{
				//BLIT CLOSE PANEL
				BltVideoObjectFromIndex( guiSAVEBUFFER, guiCLOSE, 5, sTEAMFacesXY[ posIndex ], sTEAMFacesXY[ posIndex + 1 ],  VO_BLT_SRCTRANSPARENCY, NULL );
				RestoreExternBackgroundRect( sTEAMFacesXY[ posIndex ], sTEAMFacesXY[ posIndex + 1 ], TM_FACE_WIDTH, TM_FACE_HEIGHT );

				// BLIT AP CLOSE PANEL
				//BltVideoObjectFromIndex( FRAME_BUFFER, guiTEAMObjects, 3, sTEAMAPPanelXY[ posIndex ], sTEAMAPPanelXY[ posIndex + 1 ], VO_BLT_SRCTRANSPARENCY, NULL );
				//RestoreExternBackgroundRect( (INT16)(sTEAMAPPanelXY[ posIndex ]), (INT16)(sTEAMAPPanelXY[ posIndex + 1 ]), TM_APPANEL_WIDTH, TM_APPANEL_HEIGHT );
				//InvalidateRegion( (INT16)(sTEAMAPPanelXY[ posIndex ]), (INT16)(sTEAMAPPanelXY[ posIndex + 1 ]), (INT16)(sTEAMAPPanelXY[ posIndex ] + TM_APPANEL_WIDTH), (INT16)(sTEAMAPPanelXY[ posIndex + 1 ] + TM_APPANEL_HEIGHT ) );

				if ( gTacticalStatus.ubCurrentTeam != OUR_TEAM || INTERRUPT_QUEUED )
				{
					// Hatch out...
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiTEAMObjects, 1, sTEAMFaceHighlXY[ posIndex ], sTEAMFaceHighlXY[ posIndex + 1 ], VO_BLT_SRCTRANSPARENCY, NULL );
					RestoreExternBackgroundRect( sTEAMFaceHighlXY[ posIndex ], sTEAMFaceHighlXY[ posIndex + 1 ], TM_FACEHIGHTL_WIDTH, TM_FACEHIGHTL_HEIGHT );						
				}

			}
			else
			{
				pSoldier = MercPtrs[ gTeamPanel[ cnt ].ubID ];

				if ( pSoldier->uiStatusFlags & ( SOLDIER_DRIVER ) )
				{
					// Get soldier pointer for vehicle.....
					SOLDIERTYPE *pVehicle;

					pVehicle = GetSoldierStructureForVehicle( pSoldier->iVehicleId );

					//OK, for each item, set dirty text if applicable!
					swprintf( pStr, TacticalStr[ DRIVER_POPUPTEXT ], pVehicle->bLife, pVehicle->bLifeMax, pVehicle->bBreath, pVehicle->bBreathMax );
					SetRegionFastHelpText( &(gTEAM_FirstHandInv[ cnt ]), pStr );					
				}
				// Add text for first hand popup
				else
				{
					GetHelpTextForItem( pStr, &( pSoldier->inv[ HANDPOS ] ), pSoldier );

					//OK, for each item, set dirty text if applicable!
					SetRegionFastHelpText( &(gTEAM_FirstHandInv[ cnt ]), pStr );
				}

				// Add text for seonc hand popup
				if ( pSoldier->uiStatusFlags & ( SOLDIER_PASSENGER | SOLDIER_DRIVER ) )
				{
					//OK, for each item, set dirty text if applicable!
					SetRegionFastHelpText( &(gTEAM_SecondHandInv[ cnt ]), TacticalStr[ EXIT_VEHICLE_POPUPTEXT ] );					
				}
				else
				{
					GetHelpTextForItem( pStr, &( pSoldier->inv[ SECONDHANDPOS ]  ), pSoldier );

					//OK, for each item, set dirty text if applicable!
					SetRegionFastHelpText( &(gTEAM_SecondHandInv[ cnt ]), pStr );
				}


				// Render Selected guy if selected
				if ( gusSelectedSoldier == pSoldier->ubID && gTacticalStatus.ubCurrentTeam == OUR_TEAM && OK_INTERRUPT_MERC( pSoldier ) )
				{
					//if(gbPixelDepth==16)
					//{
						BltVideoObjectFromIndex( guiSAVEBUFFER, guiTEAMObjects, 0, sTEAMFaceHighlXY[ posIndex ], sTEAMFaceHighlXY[ posIndex + 1 ], VO_BLT_SRCTRANSPARENCY, NULL );
						RestoreExternBackgroundRect( sTEAMFaceHighlXY[ posIndex ], sTEAMFaceHighlXY[ posIndex + 1 ], TM_FACEHIGHTL_WIDTH, TM_FACEHIGHTL_HEIGHT );						

						// Render faceplate
						//BltVideoObjectFromIndex( guiSAVEBUFFER, guiTEAMObjects, 1, sTEAMFacesXY[ posIndex ] + TM_FACEPLATE_OFFSET_X, sTEAMFacesXY[ posIndex + 1 ] + TM_FACEPLATE_OFFSET_Y, VO_BLT_SRCTRANSPARENCY, NULL );
						//RestoreExternBackgroundRect( (INT16)(sTEAMFacesXY[ posIndex ] + TM_FACEPLATE_OFFSET_X), (INT16)(sTEAMFacesXY[ posIndex + 1 ] + TM_FACEPLATE_OFFSET_Y ), TM_FACEPLATE_WIDTH, TM_FACEPLATE_HEIGHT );
					//}
				}
				else if ( gTacticalStatus.ubCurrentTeam != OUR_TEAM || !OK_INTERRUPT_MERC( pSoldier ) )
				{
					BltVideoObjectFromIndex( guiSAVEBUFFER, guiTEAMObjects, 1, sTEAMFaceHighlXY[ posIndex ], sTEAMFaceHighlXY[ posIndex + 1 ], VO_BLT_SRCTRANSPARENCY, NULL );
					RestoreExternBackgroundRect( sTEAMFaceHighlXY[ posIndex ], sTEAMFaceHighlXY[ posIndex + 1 ], TM_FACEHIGHTL_WIDTH, TM_FACEHIGHTL_HEIGHT );						
						// Render non-selection
						//BltVideoObjectFromIndex( guiSAVEBUFFER, guiTEAMObjects, 4, sTEAMFaceHighlXY[ posIndex ], sTEAMFaceHighlXY[ posIndex + 1 ], VO_BLT_SRCTRANSPARENCY, NULL );
						//RestoreExternBackgroundRect( sTEAMFaceHighlXY[ posIndex ], sTEAMFaceHighlXY[ posIndex + 1 ], TM_FACEHIGHTL_WIDTH, TM_FACEHIGHTL_HEIGHT );						

						// Render faceplate
						//BltVideoObjectFromIndex( guiSAVEBUFFER, guiTEAMObjects, 1, sTEAMFacesXY[ posIndex ] + TM_FACEPLATE_OFFSET_X, sTEAMFacesXY[ posIndex + 1 ] + TM_FACEPLATE_OFFSET_Y, VO_BLT_SRCTRANSPARENCY, NULL );
						//RestoreExternBackgroundRect( (INT16)(sTEAMFacesXY[ posIndex ] + TM_FACEPLATE_OFFSET_X), (INT16)(sTEAMFacesXY[ posIndex + 1 ] + TM_FACEPLATE_OFFSET_Y ), TM_FACEPLATE_WIDTH, TM_FACEPLATE_HEIGHT );


				}

				CheckForFacePanelStartAnims( pSoldier, sTEAMFacesXY[ posIndex ], sTEAMFacesXY[ posIndex + 1 ] );

				RenderSoldierFace( pSoldier, sTEAMFacesXY[ posIndex ], sTEAMFacesXY[ posIndex + 1 ], TRUE );

				// Restore AP/LIFE POSIITONS

				// Render name!
				SetFont( BLOCKFONT2 );

				if ( pSoldier->bStealthMode )
				{
					SetFontBackground( FONT_MCOLOR_BLACK );
					SetFontForeground( FONT_MCOLOR_LTYELLOW );								
				}
				else
				{
					SetFontBackground( FONT_MCOLOR_BLACK );
					SetFontForeground( FONT_MCOLOR_LTGRAY );
				}

				// RENDER ON SAVE BUFFER!
				SetFontDestBuffer( guiSAVEBUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
				VarFindFontCenterCoordinates( (INT16)(sTEAMNamesXY[ posIndex ] + 2 ), (INT16)(sTEAMNamesXY[ posIndex + 1 ] ), TM_NAME_WIDTH, TM_NAME_HEIGHT, BLOCKFONT2, &sFontX, &sFontY, L"%s", pSoldier->name );
				mprintf( sFontX, sFontY, L"%s", pSoldier->name );
				gprintfRestore( sFontX, sFontY, L"%s", pSoldier->name );
				// reset to frame buffer!
				SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

			}
		}

	}

	// Loop through all mercs and make go
	for ( cnt = 0, posIndex = 0; cnt < NUM_TEAM_SLOTS; cnt++, posIndex+= 2 )
	{
		// GET SOLDIER
		if ( gTeamPanel[ cnt ].fOccupied )
		{
			pSoldier = MercPtrs[ gTeamPanel[ cnt ].ubID ];

			// Update animations....
			if ( pSoldier->fClosePanel || pSoldier->fClosePanelToDie )
			{
				pSoldier->sPanelFaceX = gFacesData[ pSoldier->iFaceIndex ].usFaceX;
				pSoldier->sPanelFaceY = gFacesData[ pSoldier->iFaceIndex ].usFaceY;
			}


			if ( fDirty != DIRTYLEVEL0 )
			{
				// UPdate stats!
        if ( fDirty == DIRTYLEVEL2 )
        {
				  if ( pSoldier->bLife != 0 )
				  {
            if ( pSoldier->uiStatusFlags & SOLDIER_VEHICLE )
            {
			        swprintf( pStr, TacticalStr[ VEHICLE_VITAL_STATS_POPUPTEXT ], pSoldier->bLife, pSoldier->bLifeMax, pSoldier->bBreath, pSoldier->bBreathMax );
			        SetRegionFastHelpText( &(gTEAM_BarsRegions[ cnt ]), pStr );
            }
            else if ( pSoldier->uiStatusFlags & SOLDIER_ROBOT )
            {
			        swprintf( pStr, gzLateLocalizedString[ 16 ], pSoldier->bLife, pSoldier->bLifeMax );
			        SetRegionFastHelpText( &(gTEAM_BarsRegions[ cnt ]), pStr );
            }
            else
            {
					    GetMoraleString( pSoldier, pMoraleStr );
					    swprintf( pStr, TacticalStr[ MERC_VITAL_STATS_POPUPTEXT ], pSoldier->bLife, pSoldier->bLifeMax, pSoldier->bBreath, pSoldier->bBreathMax, pMoraleStr );
					    SetRegionFastHelpText( &(gTEAM_BarsRegions[ cnt ]), pStr );
            }
				  }
				  else
				  {
					  SetRegionFastHelpText( &(gTEAM_BarsRegions[ cnt ]), L"" );
				  }
        }

				if ( !( pSoldier->uiStatusFlags & SOLDIER_DEAD ) )
				{
					DrawLifeUIBarEx( pSoldier, sTEAMLifeXY[ posIndex ], sTEAMLifeXY[ posIndex + 1 ], TM_LIFEBAR_WIDTH, TM_LIFEBAR_HEIGHT, TRUE, FRAME_BUFFER );

          if ( !( pSoldier->uiStatusFlags & SOLDIER_ROBOT ) )
          {
					  DrawBreathUIBarEx( pSoldier, sTEAMBreathXY[ posIndex ], sTEAMBreathXY[ posIndex + 1 ], TM_LIFEBAR_WIDTH, TM_LIFEBAR_HEIGHT, TRUE, FRAME_BUFFER );
					  DrawMoraleUIBarEx( pSoldier, sTEAMMoraleXY[ posIndex ], sTEAMMoraleXY[ posIndex + 1 ], TM_LIFEBAR_WIDTH, TM_LIFEBAR_HEIGHT, TRUE, FRAME_BUFFER );
          }

					if ( gTacticalStatus.uiFlags & TURNBASED && pSoldier->bLife >= OKLIFE )
					{
						// Render APs
						SetFont( TINYFONT1 );

						//if ( pSoldier->sLastTarget != NOWHERE && !EnoughPoints( pSoldier, MinAPsToAttack( pSoldier, pSoldier->sLastTarget, TRUE ), 0, FALSE ) || GetUIApsToDisplay( pSoldier ) < 0 )
						if ( !EnoughPoints( pSoldier, MinAPsToAttack( pSoldier, pSoldier->sLastTarget, TRUE ), 0, FALSE ) || GetUIApsToDisplay( pSoldier ) < 0 )
						{
							SetFontBackground( FONT_MCOLOR_BLACK );
							SetFontForeground( FONT_MCOLOR_DKRED );
						}
						else
						{
							if ( MercUnderTheInfluence( pSoldier ) )
							{
								SetFontBackground( FONT_MCOLOR_BLACK );
								SetFontForeground( FONT_MCOLOR_LTBLUE );
							}
							else if ( pSoldier->bStealthMode )
							{
								SetFontBackground( FONT_MCOLOR_BLACK );
								SetFontForeground( FONT_MCOLOR_LTYELLOW );								
							}
							else
							{
								SetFontBackground( FONT_MCOLOR_BLACK );
								SetFontForeground( FONT_MCOLOR_LTGRAY );
							}
						}
						RestoreExternBackgroundRect( sTEAMApXY[ posIndex ], sTEAMApXY[ posIndex + 1 ], TM_AP_WIDTH, TM_AP_HEIGHT );

						if (gTacticalStatus.uiFlags & INCOMBAT )
						{
							VarFindFontCenterCoordinates( sTEAMApXY[ posIndex ], sTEAMApXY[ posIndex + 1], TM_AP_WIDTH, TM_AP_HEIGHT, TINYFONT1, &sFontX, &sFontY, L"%d", GetUIApsToDisplay( pSoldier ) );
							mprintf( sFontX, sTEAMApXY[ posIndex + 1], L"%d", GetUIApsToDisplay( pSoldier ) );
						}
					}
				}
				else
				{
					//Erase!
					//DrawUIBar( pSoldier->bBreath, sTEAMBreathXY[ posIndex ], sTEAMBreathXY[ posIndex + 1 ], TM_LIFEBAR_WIDTH, TM_LIFEBAR_HEIGHT, DRAW_ERASE_BAR );
					//DrawUIBar( pSoldier->bLife, sTEAMLifeXY[ posIndex ], sTEAMLifeXY[ posIndex + 1 ], TM_LIFEBAR_WIDTH, TM_LIFEBAR_HEIGHT, DRAW_ERASE_BAR );

					// Erase APs
					RestoreExternBackgroundRect( sTEAMApXY[ posIndex ], sTEAMApXY[ posIndex + 1 ], TM_AP_WIDTH, TM_AP_HEIGHT );

				}

			}

			RenderSoldierTeamInv( pSoldier, sTEAMHandInvXY[ posIndex ], sTEAMHandInvXY[ posIndex + 1 ], (UINT8)cnt, fDirty );

			HandleSoldierFaceFlash( pSoldier, sTEAMFacesXY[ posIndex ], sTEAMFacesXY[ posIndex + 1 ] );


			if ( !(guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
			{
				//HandlePanelFaceAnimations( pSoldier );
			}

		}
	}
	
	UpdateTEAMPanel( );
	
	if( fRenderRadarScreen == TRUE )
	{
		// Render clock
		RenderClock( INTERFACE_CLOCK_X, INTERFACE_CLOCK_Y );
		CreateMouseRegionForPauseOfClock( INTERFACE_CLOCK_X, INTERFACE_CLOCK_Y );
		RenderTownIDString( );
	}
	else
	{
		RemoveMouseRegionForPauseOfClock( );
	}
}

BOOLEAN CreateTEAMPanelButtons( )
{
CHAR8	ubString[48];

	FilenameForBPP("INTERFACE\\bottom_bar_buttons.sti", ubString);

	// Load button Graphics
	iTEAMPanelImages[ ENDTURN_IMAGES  ]			= LoadButtonImage(ubString,-1,0,-1, 3,-1 );
	iTEAMPanelImages[ ROSTERMODE_IMAGES  ]	= UseLoadedButtonImage( iTEAMPanelImages[ ENDTURN_IMAGES  ] ,-1, 1,-1, 4,-1 );
	iTEAMPanelImages[ DISK_IMAGES  ]				= UseLoadedButtonImage( iTEAMPanelImages[ ENDTURN_IMAGES  ] ,-1, 2,-1, 5, -1 );
	
	// Create buttons
	iTEAMPanelButtons[ TEAM_DONE_BUTTON ] = QuickCreateButton( iTEAMPanelImages[ ENDTURN_IMAGES ], TM_ENDTURN_X, TM_ENDTURN_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnEndTurnCallback );
	if ( iTEAMPanelButtons[ TEAM_DONE_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	SetButtonFastHelpText( iTEAMPanelButtons[ TEAM_DONE_BUTTON ], TacticalStr[ END_TURN_POPUPTEXT ] );



	iTEAMPanelButtons[ TEAM_MAP_SCREEN_BUTTON ] = QuickCreateButton( iTEAMPanelImages[ ROSTERMODE_IMAGES ], TM_ROSTERMODE_X, TM_ROSTERMODE_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnRostermodeCallback );
	if ( iTEAMPanelButtons[ TEAM_MAP_SCREEN_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	SetButtonFastHelpText( iTEAMPanelButtons[ TEAM_MAP_SCREEN_BUTTON ], TacticalStr[ MAPSCREEN_POPUPTEXT ] );

	iTEAMPanelButtons[ CHANGE_SQUAD_BUTTON ] = QuickCreateButton( iTEAMPanelImages[ DISK_IMAGES ], TM_DISK_X, TM_DISK_Y,
										BUTTON_TOGGLE, MSYS_PRIORITY_HIGH - 1,
										DEFAULT_MOVE_CALLBACK, (GUI_CALLBACK)BtnSquadCallback );
	if ( iTEAMPanelButtons[ CHANGE_SQUAD_BUTTON ] == -1 )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "Cannot create Interface button" );
		return( FALSE );
	}
	SetButtonFastHelpText( iTEAMPanelButtons[ CHANGE_SQUAD_BUTTON ], TacticalStr[ CHANGE_SQUAD_POPUPTEXT ] );

	return( TRUE );
}

void    RemoveTEAMPanelButtons( )
{
	UINT32 cnt;

	for ( cnt = 0; cnt < NUM_TEAM_BUTTONS; cnt++ )
	{
		RemoveButton( iTEAMPanelButtons[ cnt ] );
	}

	for ( cnt = 0; cnt < NUM_TEAM_BUTTON_IMAGES; cnt++ )
	{
		UnloadButtonImage( iTEAMPanelImages[ cnt ] );
	}

}

void BtnEndTurnCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		// END TURN
		UIHandleEndTurn( NULL );

	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}
	
}

void BtnRostermodeCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		if ( guiCurrentScreen == GAME_SCREEN )
		{
			GoToMapScreenFromTactical();
			EnableRadarScreenRender( );
		}

	}
	
}


// callback to handle squad switching callback
void BtnSquadCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
	}
	else if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );

		// ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Not Implemented Yet" );
		ToggleRadarScreenRender( );

	}
	else if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
	}
	
}

void SetTEAMPanelCurrentMerc( UINT8 ubNewID )
{
	// Ignore ther ID given!
	// ALWYAS USE CURRENT MERC!

	fInterfacePanelDirty = DIRTYLEVEL2;

	UpdateTEAMPanel( );

}

void UpdateTEAMPanel( )
{
	INT32		cnt;

	if ( (gTacticalStatus.ubCurrentTeam != gbPlayerNum) || (gTacticalStatus.uiFlags & REALTIME ) || !(gTacticalStatus.uiFlags & INCOMBAT ) )
	{
		DisableButton( iTEAMPanelButtons[ TEAM_DONE_BUTTON ] );
	}
	else
	{
		EnableButton( iTEAMPanelButtons[ TEAM_DONE_BUTTON ] );
	}

	if ( gTacticalStatus.uiFlags & ENGAGED_IN_CONV )
	{
		DisableButton( iTEAMPanelButtons[ TEAM_MAP_SCREEN_BUTTON ] );
	}
	else
	{
		EnableButton( iTEAMPanelButtons[ TEAM_MAP_SCREEN_BUTTON ] );
	}


	if ( gfDisableTacticalPanelButtons )
	{
		DisableButton( iTEAMPanelButtons[ TEAM_DONE_BUTTON ] );
		DisableButton( iTEAMPanelButtons[ TEAM_MAP_SCREEN_BUTTON ] );
		DisableButton( iTEAMPanelButtons[ CHANGE_SQUAD_BUTTON ] );

		// OK, disable item regions.......
		for ( cnt = 0; cnt < 6; cnt++ )
		{
			MSYS_DisableRegion( &gTEAM_EnemyIndicator[ cnt ] );

			MSYS_DisableRegion( &gTEAM_FirstHandInv[ cnt ] );

			MSYS_DisableRegion( &gTEAM_SecondHandInv[ cnt ] );			
		}

		//disable the radar map region
		// If NOT in overhead map
		if ( !InOverheadMap( ) )
		{
			MSYS_DisableRegion(&gRadarRegion);
		}

	}
	else
	{
		EnableButton( iTEAMPanelButtons[ CHANGE_SQUAD_BUTTON ] );

		for ( cnt = 0; cnt < 6; cnt++ )
		{
			MSYS_EnableRegion( &gTEAM_EnemyIndicator[ cnt ] );

			MSYS_EnableRegion( &gTEAM_FirstHandInv[ cnt ] );

			MSYS_EnableRegion( &gTEAM_SecondHandInv[ cnt ] );			
		}

		//Enable the radar map region
		MSYS_EnableRegion(&gRadarRegion);

	}


}


void HandleMouseOverTeamFaceForContMove( BOOLEAN fOn )
{
	FACETYPE *pFace;

	if ( fOn )
	{
		// Check if we are waiting to continue move...
		if ( CheckForMercContMove( gpSMCurrentMerc ) )
		{
			// Display 'cont' on face....
			// Get face
			pFace = &gFacesData[ gpSMCurrentMerc->iFaceIndex ];
			
			pFace->fDisplayTextOver = FACE_DRAW_TEXT_OVER;
			wcscpy( pFace->zDisplayText, TacticalStr[ CONTINUE_OVER_FACE_STR ] );

			// While our mouse is here, draw a path!
			PlotPath( gpSMCurrentMerc, gpSMCurrentMerc->sFinalDestination, NO_COPYROUTE, PLOT, TEMPORARY, (UINT16)gpSMCurrentMerc->usUIMovementMode, NOT_STEALTH, FORWARD, gpSMCurrentMerc->bActionPoints );
		}
	}
	else
	{
		// Remove 'cont' on face....
		// Get face
		pFace = &gFacesData[ gpSMCurrentMerc->iFaceIndex ];
		
		pFace->fDisplayTextOver = FACE_ERASE_TEXT_OVER;

		// Erase path!
		ErasePath( TRUE );
	}
}


void MercFacePanelMoveCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT8 ubID, ubSoldierID;
	SOLDIERTYPE	*pSoldier;

	ubID = (UINT8) MSYS_GetRegionUserData( pRegion, 0 );

	// If our flags are set to do this, gofoit!
	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		return;
	}

	if ( ubID == NOBODY )
	{
		return;
	}

	// Now use soldier ID values
	ubSoldierID = gTeamPanel[ ubID ].ubID;

	if ( ubSoldierID == NOBODY )
	{
		return;
	}

	pSoldier = MercPtrs[ ubSoldierID ];

	if ( !pSoldier->bActive )
	{
		return;
	}


	if (iReason & MSYS_CALLBACK_REASON_MOVE )
	{
		HandleMouseOverSoldierFaceForContMove( pSoldier, TRUE );
	}
	else if ( iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		HandleMouseOverSoldierFaceForContMove( pSoldier, FALSE );
	}

}


void EnemyIndicatorClickCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT8 ubID, ubSoldierID;

	ubID = (UINT8) MSYS_GetRegionUserData( pRegion, 0 );

	if ( ubID == NOBODY )
	{
		return;
	}

	// Now use soldier ID values
	ubSoldierID = gTeamPanel[ ubID ].ubID;

	if ( ubSoldierID == NOBODY )
	{
		return;
	}


	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}

	if ( !MercPtrs[ ubSoldierID ]->bActive )
	{
		return;
	}


	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if ( MercPtrs[ ubSoldierID ]->uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) )
		{
		}
		else
		{
			SOLDIERTYPE *pSoldier;

			pSoldier = MercPtrs[ ubSoldierID ];

			if ( pSoldier->bOppCnt > 0 )
			{	// Cycle....
				CycleVisibleEnemies( pSoldier );
			}
			else
			{
				MercFacePanelCallback( pRegion, iReason );
			}
		}
	}
}


void MercFacePanelCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT8 ubID, ubSoldierID;
	SOLDIERTYPE *pVehicle;

	ubID = (UINT8) MSYS_GetRegionUserData( pRegion, 0 );

	// If our flags are set to do this, gofoit!
	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
		{
			SetInfoChar( ubID );
		}
		return;
	}

	if ( ubID == NOBODY )
	{
		return;
	}


	// Now use soldier ID values
	ubSoldierID = gTeamPanel[ ubID ].ubID;

	if ( ubSoldierID == NOBODY )
	{
		return;
	}


	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}

	if ( !MercPtrs[ ubSoldierID ]->bActive )
	{
		return;
	}


	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		if ( !gfInItemPickupMenu && gpItemPointer == NULL )
		{
			if ( MercPtrs[ ubSoldierID ]->uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) )
			{
				pVehicle = GetSoldierStructureForVehicle( MercPtrs[ ubSoldierID ]->iVehicleId );

				HandleLocateSelectMerc( pVehicle->ubID, 0 );
			}
			else
			{
				if ( !InOverheadMap( ) )
				{
					// If we can continue a move, do so!
					if ( CheckForMercContMove( MercPtrs[ ubSoldierID ] ) )
					{
						// Continue
						ContinueMercMovement( MercPtrs[ ubSoldierID ] );
						ErasePath( TRUE );
					}
					else
					{
						HandleLocateSelectMerc( ubSoldierID, 0 );
					}
				}
				else
				{
					SelectSoldier( ubSoldierID, TRUE, FALSE );
				}
			}
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN)
	{
		if ( !InOverheadMap( ) )
		{
			// Only if guy is not dead!
			if ( !( MercPtrs[ ubSoldierID ]->uiStatusFlags & SOLDIER_DEAD ) && !AM_AN_EPC( MercPtrs[ ubSoldierID ] ) && !( MercPtrs[ ubSoldierID ]->uiStatusFlags & ( SOLDIER_DRIVER | SOLDIER_PASSENGER ) ) )
			{
				gfSwitchPanel = TRUE;
				gbNewPanel = SM_PANEL;
				gubNewPanelParam = (UINT8)ubSoldierID;
			}
		}
	}

}

extern void InternalSelectSoldier( UINT16 usSoldierID, BOOLEAN fAcknowledge, BOOLEAN fForceReselect, BOOLEAN fFromUI );

void HandleLocateSelectMerc( UINT8 ubID, INT8 bFlag  )
{
	BOOLEAN fSelect = FALSE;

	if( !MercPtrs[ ubID ]->bActive )
	{
		return;
	}

	if ( gpItemPointer != NULL )
	{
		// ATE: Disable for feel purposes....
		//return;
	}


	// ATE: No matter what we do... if below OKLIFE, just locate....
	if ( MercPtrs[ ubID ]->bLife < OKLIFE )
	{
		LocateSoldier( ubID, SETLOCATOR );
		return;
	}

	if ( _KeyDown( ALT ) )
	{
		if ( gGameSettings.fOptions[ TOPTION_OLD_SELECTION_METHOD ] )
		{
			// Select merc
			InternalSelectSoldier( ubID, TRUE, FALSE, TRUE);
			MercPtrs[ ubID ]->fFlashLocator = FALSE;
			ResetMultiSelection( );
		}
		else
		{
			// Just locate....
			LocateSoldier( ubID, SETLOCATOR );
		}
	}
	else
	{
		if ( MercPtrs[ ubID ]->fFlashLocator == FALSE )
		{
			if ( gGameSettings.fOptions[ TOPTION_OLD_SELECTION_METHOD ] )
			{
				// If we are currently selected, slide to location
				if ( ubID == gusSelectedSoldier )
				{
					SlideTo( NOWHERE, gusSelectedSoldier, NOBODY ,SETLOCATOR);
				}
				else
				{
					LocateSoldier( ubID, SETLOCATOR );
				}
			}
			else
			{
				if ( ubID == gusSelectedSoldier )
				{
					SlideTo( NOWHERE, gusSelectedSoldier, NOBODY ,SETLOCATOR );
				}
				else
				{
					fSelect = TRUE;
					//ShowRadioLocator( ubID, SHOW_LOCATOR_NORMAL );
				}
			}
		}
		else 
		{
			if ( gGameSettings.fOptions[ TOPTION_OLD_SELECTION_METHOD ] )
			{
				// If we are currently selected, slide to location
				if ( ubID == gusSelectedSoldier )
				{
					SlideTo( NOWHERE, gusSelectedSoldier, NOBODY ,DONTSETLOCATOR);
				}
				else
				{
					LocateSoldier( ubID, DONTSETLOCATOR );
				}

				fSelect = TRUE;
			}
			else
			{
				if ( ubID == gusSelectedSoldier )
				{
					LocateSoldier( ubID, DONTSETLOCATOR );
				}
				else
				{
					fSelect = TRUE;
				}
			}
		}

		if ( bFlag == LOCATEANDSELECT_MERC )
		{
			fSelect = TRUE;
		}

		if ( fSelect )
		{
			// Select merc, only if alive!
			if ( !( MercPtrs[ ubID ]->uiStatusFlags & SOLDIER_DEAD ) )
			{
				InternalSelectSoldier( ubID, TRUE, FALSE, TRUE );
			}
		}
	}

	ResetMultiSelection( );

	// Handle locate select merc....
	HandleMouseOverSoldierFaceForContMove( MercPtrs[ ubID ], TRUE );

}



void ShowRadioLocator( UINT8 ubID, UINT8 ubLocatorSpeed )
{
	RESETTIMECOUNTER( MercPtrs[ ubID ]->FlashSelCounter, FLASH_SELECTOR_DELAY );

	//LocateSoldier( ubID, FALSE );	// IC - this is already being done outside of this function :)
	MercPtrs[ ubID ]->fFlashLocator = TRUE;
	//gbPanelSelectedGuy = ubID;	IC - had to move this outside to make this function versatile
	MercPtrs[ ubID ]->sLocatorFrame = 0;

	if ( ubLocatorSpeed == SHOW_LOCATOR_NORMAL )
	{
		// If we are an AI guy, and we have the baton, make lower...
		// ( MercPtrs[ ubID ]->uiStatusFlags & SOLDIER_UNDERAICONTROL && MercPtrs[ ubID ]->bTeam != gbPlayerNum )
		//
		//ercPtrs[ ubID ]->ubNumLocateCycles = 3;
		//
		//se
		//
			MercPtrs[ ubID ]->ubNumLocateCycles = 5;
		//
	}
	else
	{
		MercPtrs[ ubID ]->ubNumLocateCycles = 3;
	}
}

void EndRadioLocator( UINT8 ubID )
{
	MercPtrs[ ubID ]->fFlashLocator = FALSE;
	MercPtrs[ ubID ]->fShowLocator = FALSE;  
}



void CheckForFacePanelStartAnims( SOLDIERTYPE *pSoldier, INT16 sPanelX, INT16 sPanelY )
{

	if ( !pSoldier->bActive )
	{
		return;
	}


	if ( pSoldier->fUIdeadMerc  )
	{
//		pSoldier->sPanelFaceX	= sPanelX;
//		pSoldier->sPanelFaceY	= sPanelY;
	}


}


void FinishAnySkullPanelAnimations( )
{
	SOLDIERTYPE *pTeamSoldier;
	INT32		cnt2;

	cnt2 = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;	

	// run through list
	for ( pTeamSoldier = MercPtrs[ cnt2 ]; cnt2 <= gTacticalStatus.Team[ gbPlayerNum ].bLastID; cnt2++,pTeamSoldier++ )
	{
		if ( pTeamSoldier->bActive && pTeamSoldier->bLife == 0 )
		{
			if ( pTeamSoldier->fUIdeadMerc || pTeamSoldier->fClosePanelToDie )
			{
				HandlePlayerTeamMemberDeathAfterSkullAnimation( pTeamSoldier );
				
				pTeamSoldier->fUIdeadMerc = FALSE;
				pTeamSoldier->fClosePanelToDie = FALSE;
			}
		}
	}

}

void HandlePanelFaceAnimations( SOLDIERTYPE *pSoldier )
{
	if ( pSoldier->bTeam != gbPlayerNum )
	{
		return;
	}


	if ( !pSoldier->bActive )
	{
		return;
	}

	if ( pSoldier->uiStatusFlags & SOLDIER_VEHICLE )
	{
		// Don't do this for a vehice.
		return;
	}

	// If this is the first time we are active, open panel!
	if ( pSoldier->fUInewMerc )
	{
		pSoldier->fUInewMerc = FALSE;
	}

	if ( pSoldier->fUICloseMerc )
	{
		pSoldier->fUICloseMerc = FALSE;
	}

	if ( pSoldier->fUIdeadMerc  )
	{
		pSoldier->sPanelFaceX = gFacesData[ pSoldier->iFaceIndex ].usFaceX;
		pSoldier->sPanelFaceY = gFacesData[ pSoldier->iFaceIndex ].usFaceY;

		pSoldier->fUIdeadMerc = FALSE;		
		pSoldier->fClosePanel		= TRUE;
		pSoldier->fClosePanelToDie = TRUE;
		pSoldier->ubClosePanelFrame = 0;
		pSoldier->ubDeadPanelFrame = 0;
		RESETTIMECOUNTER( pSoldier->PanelAnimateCounter, 160 );
	}

	if ( pSoldier->fClosePanel )
	{
		 if ( TIMECOUNTERDONE( pSoldier->PanelAnimateCounter, 160 ) )
		 {
				pSoldier->ubClosePanelFrame++;

				if ( pSoldier->ubClosePanelFrame > 5 )
				{
					pSoldier->fClosePanel = FALSE;
					pSoldier->ubClosePanelFrame = 5;

					if ( pSoldier->fClosePanelToDie )
					{
						pSoldier->fDeadPanel = TRUE;
						//PlayJA2Sample( (UINT8)HEADCR_1, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
					}
					else
					{
						if ( !gFacesData[ pSoldier->iFaceIndex ].fDisabled )
						{
							RestoreExternBackgroundRect( pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, TM_FACE_WIDTH, TM_FACE_HEIGHT );
						}
					}
				}
				RESETTIMECOUNTER( pSoldier->PanelAnimateCounter, 160 );
		 }
	}

	if ( pSoldier->fClosePanel )
	{
		 // Render panel!
		 //if(gbPixelDepth==16)
		 //{
			if ( !gFacesData[ pSoldier->iFaceIndex ].fDisabled )
			{
				 RestoreExternBackgroundRect( pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, TM_FACE_WIDTH, TM_FACE_HEIGHT );
				 BltVideoObjectFromIndex( FRAME_BUFFER, guiCLOSE, pSoldier->ubClosePanelFrame, pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, VO_BLT_SRCTRANSPARENCY, NULL );
				 InvalidateRegion( pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, pSoldier->sPanelFaceX + TM_FACE_WIDTH, pSoldier->sPanelFaceY + TM_FACE_HEIGHT );			 
			}
		 //}
	}


	if ( pSoldier->fDeadPanel )
	{
		 if ( TIMECOUNTERDONE(  pSoldier->PanelAnimateCounter, 160 ) )
		 {
				pSoldier->ubDeadPanelFrame++;

				if ( pSoldier->ubDeadPanelFrame == 4 )
				{
					ScreenMsg( FONT_RED, MSG_SKULL_UI_FEEDBACK, pMercDeadString[ 0 ], pSoldier->name );

					PlayJA2Sample( (UINT8)DOORCR_1, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
					PlayJA2Sample( (UINT8)HEADCR_1, RATE_11025, HIGHVOLUME, 1, MIDDLEPAN );
				}

				if ( pSoldier->ubDeadPanelFrame > 5 )
				{
					pSoldier->fDeadPanel = FALSE;
					pSoldier->ubDeadPanelFrame = 5;
					pSoldier->fClosePanelToDie = FALSE;

					// Finish!
					if ( !gFacesData[ pSoldier->iFaceIndex ].fDisabled )
					{
						BltVideoObjectFromIndex( guiSAVEBUFFER, guiDEAD, pSoldier->ubDeadPanelFrame, pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, VO_BLT_SRCTRANSPARENCY, NULL );

						// Blit hatch!
						BltVideoObjectFromIndex( guiSAVEBUFFER, guiHATCH, 0, pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, VO_BLT_SRCTRANSPARENCY, NULL );

						RestoreExternBackgroundRect( pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, TM_FACE_WIDTH, TM_FACE_HEIGHT );
					}
					HandlePlayerTeamMemberDeathAfterSkullAnimation( pSoldier );

				}
				RESETTIMECOUNTER( pSoldier->PanelAnimateCounter, 160 );
		 }
	}

	if ( pSoldier->fDeadPanel )
	{
		 // Render panel!
		 //if(gbPixelDepth==16)
		 //{
				if ( !gFacesData[ pSoldier->iFaceIndex ].fDisabled )
				{
					 BltVideoObjectFromIndex( FRAME_BUFFER, guiDEAD, pSoldier->ubDeadPanelFrame, pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, VO_BLT_SRCTRANSPARENCY, NULL );

					 // Blit hatch!
					 BltVideoObjectFromIndex( guiSAVEBUFFER, guiHATCH, 0, pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, VO_BLT_SRCTRANSPARENCY, NULL );

					 InvalidateRegion( pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, pSoldier->sPanelFaceX + TM_FACE_WIDTH, pSoldier->sPanelFaceY + TM_FACE_HEIGHT );			 
				}
		 //}
	}


	if ( pSoldier->fOpenPanel )
	{
		 if ( TIMECOUNTERDONE( pSoldier->PanelAnimateCounter, 160 ) )
		 {
				pSoldier->bOpenPanelFrame--;

				if ( pSoldier->bOpenPanelFrame < 0 )
				{
					pSoldier->fOpenPanel = FALSE;
					pSoldier->bOpenPanelFrame = 0;

					if ( !gFacesData[ pSoldier->iFaceIndex ].fDisabled )
					{
						RestoreExternBackgroundRect( pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, TM_FACE_WIDTH, TM_FACE_HEIGHT );
					}
				}
				RESETTIMECOUNTER( pSoldier->PanelAnimateCounter, 160 );
		 }
	}

	if ( pSoldier->fOpenPanel )
	{
		 // Render panel!
		 //if(gbPixelDepth==16)
		 //{
				if ( !gFacesData[ pSoldier->iFaceIndex ].fDisabled )
				{
					RestoreExternBackgroundRect( pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, TM_FACE_WIDTH, TM_FACE_HEIGHT );
					BltVideoObjectFromIndex( FRAME_BUFFER, guiCLOSE, pSoldier->bOpenPanelFrame, pSoldier->sPanelFaceX, pSoldier->sPanelFaceY, VO_BLT_SRCTRANSPARENCY, NULL );
					//InvalidateRegion( sTEAMFacesXY[ ubOpenPanelID ], sTEAMFacesXY[ ubOpenPanelID + 1 ], sTEAMFacesXY[ ubOpenPanelID ] + TM_FACE_WIDTH, sTEAMFacesXY[ ubOpenPanelID + 1 ] + TM_FACE_HEIGHT );			 
				}
		 //}
	}
	
}



void HandleSoldierFaceFlash( SOLDIERTYPE *pSoldier, INT16 sFaceX, INT16 sFaceY )
{

}



void RenderSoldierTeamInv( SOLDIERTYPE *pSoldier, INT16 sX, INT16 sY, UINT8 ubPanelNum, BOOLEAN fDirty )
{
	if ( pSoldier->bActive && !(pSoldier->uiStatusFlags & SOLDIER_DEAD ) )
	{
		if ( pSoldier->uiStatusFlags & SOLDIER_DRIVER )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiVEHINV, 0, sX, sY, VO_BLT_SRCTRANSPARENCY, NULL );
			RestoreExternBackgroundRect( sX, sY, (INT16)( TM_INV_WIDTH ) , (INT16)( TM_INV_HEIGHT ) );
		}
		else
		{
			// Look in primary hand
			INVRenderItem( guiSAVEBUFFER, pSoldier, &(pSoldier->inv[ HANDPOS ]), sX, sY, TM_INV_WIDTH, TM_INV_HEIGHT, fDirty, &(gfTEAM_HandInvDispText[ ubPanelNum ][ HANDPOS ] ), 0 , FALSE, 0);
		}

		if ( pSoldier->uiStatusFlags & ( SOLDIER_PASSENGER | SOLDIER_DRIVER ) )
		{
			BltVideoObjectFromIndex( guiSAVEBUFFER, guiVEHINV, 1, sX, (INT16)(sY + TM_INV_HAND_SEPY), VO_BLT_SRCTRANSPARENCY, NULL );
			RestoreExternBackgroundRect( sX, (INT16)(sY + TM_INV_HAND_SEPY), (INT16)(TM_INV_WIDTH ) , (INT16)( TM_INV_HEIGHT ) );
		}
		else
		{
			// Do secondary hand
			INVRenderItem( guiSAVEBUFFER, pSoldier, &(pSoldier->inv[ SECONDHANDPOS ]), sX, (INT16)(sY + TM_INV_HAND_SEPY), TM_INV_WIDTH, TM_INV_HEIGHT, fDirty, &(gfTEAM_HandInvDispText[ ubPanelNum ][ SECONDHANDPOS ] ), 0 , FALSE, 0);
		}

	}
}


void TMFirstHandInvCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT8 ubID, ubSoldierID;

	ubID = (UINT8) MSYS_GetRegionUserData( pRegion, 0 );

	// If our flags are set to do this, gofoit!
	if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
	{
		return;
	}

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}

	if ( ubID == NOBODY )
		return;


	// Now use soldier ID values
	ubSoldierID = gTeamPanel[ ubID ].ubID;
	

	if ( !MercPtrs[ ubSoldierID ]->bActive )
	{
		return;
	}


	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN)
	{
		//OK FOR NOW, DROP ITEM WHERE BUDDY IS
		{
		//	SOLDIERTYPE *pSoldier;

		//	if(	GetSoldier( &pSoldier, ubSoldierID ) )
		//	{
		//	}
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN)
	{

	}

}


void TMClickFirstHandInvCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT8		ubID, ubSoldierID;
	UINT16	usOldHandItem;

	ubID = (UINT8) MSYS_GetRegionUserData( pRegion, 0 );


	if ( ubID == NOBODY )
		return;
	
	// Now use soldier ID values
	ubSoldierID = gTeamPanel[ ubID ].ubID;

	if ( ubSoldierID == NOBODY )
		return;

	if (iReason == MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		// Change to use cursor mode...
		guiPendingOverrideEvent = A_ON_TERRAIN;
	}

	if (iReason == MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
		if ( !AM_A_ROBOT( MercPtrs[ ubSoldierID ] ) )
		{
			usOldHandItem = MercPtrs[ ubSoldierID ]->inv[HANDPOS].usItem;
			//SwapOutHandItem( MercPtrs[ ubSoldierID ] );
			SwapHandItems( MercPtrs[ ubSoldierID ] );
			ReLoadSoldierAnimationDueToHandItemChange( MercPtrs[ ubSoldierID ], usOldHandItem, MercPtrs[ ubSoldierID ]->inv[HANDPOS].usItem );
			fInterfacePanelDirty = DIRTYLEVEL2;
		}
	}
}


void TMClickSecondHandInvCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT8 ubID, ubSoldierID;
	UINT16	usOldHandItem;

	ubID = (UINT8) MSYS_GetRegionUserData( pRegion, 0 );

	if ( ubID == NOBODY )
		return;
	
	// Now use soldier ID values
	ubSoldierID = gTeamPanel[ ubID ].ubID;

	if ( ubSoldierID == NOBODY )
		return;

	if (iReason == MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if ( MercPtrs[ ubSoldierID ]->uiStatusFlags & ( SOLDIER_PASSENGER | SOLDIER_DRIVER ) )
		{
			ExitVehicle( MercPtrs[ ubSoldierID ] );
		}
	}

	if (iReason == MSYS_CALLBACK_REASON_RBUTTON_UP )
	{
		if ( MercPtrs[ ubSoldierID ]->uiStatusFlags & ( SOLDIER_PASSENGER | SOLDIER_DRIVER ) )
		{
		}
		else
		{
			if ( !AM_A_ROBOT( MercPtrs[ ubSoldierID ] ) )
			{
				usOldHandItem = MercPtrs[ ubSoldierID ]->inv[HANDPOS].usItem;
				SwapHandItems( MercPtrs[ ubSoldierID ] );
				ReLoadSoldierAnimationDueToHandItemChange( MercPtrs[ ubSoldierID ], usOldHandItem, MercPtrs[ ubSoldierID ]->inv[HANDPOS].usItem );
				fInterfacePanelDirty = DIRTYLEVEL2;
			}
		}
	}
}


BOOLEAN PlayerExistsInSlot( UINT8 ubID )
{
	INT32 cnt;

	for ( cnt = 0; cnt < NUM_TEAM_SLOTS; cnt++ )
	{
		if ( gTeamPanel[ cnt ].fOccupied	)
		{
			if ( gTeamPanel[ cnt ].ubID == ubID )
			{
				return( TRUE );
			}
		}
	}

	return( FALSE );
}


INT8 GetTeamSlotFromPlayerID( UINT8 ubID )
{
	INT8 cnt;

	for ( cnt = 0; cnt < NUM_TEAM_SLOTS; cnt++ )
	{
		if ( gTeamPanel[ cnt ].fOccupied	)
		{
			if ( gTeamPanel[ cnt ].ubID == ubID )
			{
				return( cnt );
			}
		}
	}

	return( -1 );
}


BOOLEAN RemovePlayerFromTeamSlotGivenMercID( UINT8 ubMercID )
{
	INT32 cnt;

	for ( cnt = 0; cnt < NUM_TEAM_SLOTS; cnt++ )
	{
		if ( gTeamPanel[ cnt ].fOccupied	)
		{
			if ( gTeamPanel[ cnt ].ubID == ubMercID )
			{
				RemovePlayerFromInterfaceTeamSlot( (UINT8)cnt );
				return( TRUE );
			}
		}
	}

	return( FALSE );
}


void AddPlayerToInterfaceTeamSlot( UINT8 ubID )
{
	INT32  cnt;

	// If we are a vehicle don't ever add.....
	if ( MercPtrs[ ubID ]->uiStatusFlags & SOLDIER_VEHICLE )
	{
		return;
	}

	if ( !PlayerExistsInSlot( ubID ) )
	{
		// Find a free slot
		for ( cnt = 0; cnt < NUM_TEAM_SLOTS; cnt++ )
		{
			if ( !gTeamPanel[ cnt ].fOccupied	)
			{
				gTeamPanel[ cnt ].fOccupied = TRUE;
				gTeamPanel[ cnt ].ubID			= ubID;

				MSYS_SetRegionUserData( &gTEAM_FirstHandInv[ cnt ], 0, cnt );
				MSYS_SetRegionUserData( &gTEAM_FaceRegions[ cnt ], 0, cnt );

				// DIRTY INTERFACE
				fInterfacePanelDirty = DIRTYLEVEL2;

				// Set ID to do open anim
				MercPtrs[ ubID ]->fUInewMerc						= TRUE;

				break;
			}
		}
	}
}

BOOLEAN InitTEAMSlots( )
{
	INT32 cnt;

	for ( cnt = 0; cnt < NUM_TEAM_SLOTS; cnt++ )
	{
		gTeamPanel[ cnt ].fOccupied = FALSE;
		gTeamPanel[ cnt ].ubID			= NOBODY;

	}

	return( TRUE );
}


BOOLEAN GetPlayerIDFromInterfaceTeamSlot( UINT8 ubPanelSlot, UINT8 *pubID )
{
	if ( ubPanelSlot >= NUM_TEAM_SLOTS )
	{
		return( FALSE );
	}

	if ( gTeamPanel[ ubPanelSlot ].fOccupied )
	{
		*pubID = gTeamPanel[ ubPanelSlot ].ubID;
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}

void RemoveAllPlayersFromSlot( )
{
	int cnt;
	
	for ( cnt = 0; cnt < NUM_TEAM_SLOTS; cnt++ )
	{
		RemovePlayerFromInterfaceTeamSlot( (UINT8)cnt );
	}
}


BOOLEAN RemovePlayerFromInterfaceTeamSlot( UINT8 ubPanelSlot )
{
	if ( ubPanelSlot >= NUM_TEAM_SLOTS )
	{
		return( FALSE );
	}

	if ( gTeamPanel[ ubPanelSlot ].fOccupied )
	{
		if ( !( MercPtrs[ gTeamPanel[ ubPanelSlot ].ubID ]->uiStatusFlags & SOLDIER_DEAD ) )
    {
		  // Set Id to close
		  MercPtrs[ gTeamPanel[ ubPanelSlot ].ubID ]->fUICloseMerc		= TRUE;
    }

		// Set face to inactive...
		SetAutoFaceInActive( MercPtrs[ gTeamPanel[ ubPanelSlot ].ubID ]->iFaceIndex );


		gTeamPanel[ ubPanelSlot ].fOccupied = FALSE;
		gTeamPanel[ ubPanelSlot ].ubID = NOBODY;

		MSYS_SetRegionUserData( &gTEAM_FirstHandInv[ ubPanelSlot ], 0, NOBODY );
		MSYS_SetRegionUserData( &gTEAM_FaceRegions[ ubPanelSlot ],	0, NOBODY );

		// DIRTY INTERFACE
		fInterfacePanelDirty = DIRTYLEVEL2;

		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}


void RenderTownIDString( )
{
	CHAR16	zTownIDString[80];
	INT16 sFontX, sFontY;

	// Render town, position
	SetFont( COMPFONT );
	SetFontForeground( 183 );
	SetFontBackground( FONT_BLACK );

	GetSectorIDString( gWorldSectorX, gWorldSectorY, gbWorldSectorZ, zTownIDString, TRUE );
	ReduceStringLength( zTownIDString, 80, COMPFONT );
	VarFindFontCenterCoordinates( LOCATION_NAME_X , LOCATION_NAME_Y, 80, 16, COMPFONT, &sFontX, &sFontY, zTownIDString );
	mprintf( sFontX, sFontY, L"%s", zTownIDString );
}


void CheckForAndAddMercToTeamPanel( SOLDIERTYPE *pSoldier )
{

	if ( pSoldier->bActive  )
	{
		// Add to interface if the are ours
		if ( pSoldier->bTeam == gbPlayerNum )
		{
			// Are we in the loaded sector?
			if ( pSoldier->sSectorX == gWorldSectorX && pSoldier->sSectorY == gWorldSectorY && pSoldier->bSectorZ == gbWorldSectorZ && !pSoldier->fBetweenSectors && pSoldier->bInSector )
			{
				// IF on duty....
				if( ( pSoldier->bAssignment ==  CurrentSquad( ) )|| ( SoldierIsDeadAndWasOnSquad( pSoldier, ( INT8 )( CurrentSquad( ) ) ) ) )
				{

					if( pSoldier->bAssignment == ASSIGNMENT_DEAD )
					{
						pSoldier->fUICloseMerc = FALSE;
					}
					// ATE: ALrighty, if we have the insertion code of helicopter..... don't add just yet!
					/// ( will add in heli code )
					if ( pSoldier->ubStrategicInsertionCode != INSERTION_CODE_CHOPPER )
					{
						AddPlayerToInterfaceTeamSlot( pSoldier->ubID );
					}

					// ARE WE A VEHICLE.....
					if ( pSoldier->uiStatusFlags & SOLDIER_VEHICLE )
					{
						AddPassangersToTeamPanel( pSoldier->bVehicleID );
					}
				}	
			}
			else
			{
				// Make sure we are NOT in this world!
				// Set gridno to one that's not visib;l
				RemoveSoldierFromGridNo( pSoldier );

				// Remove slot from tactical
				RemoveMercSlot( pSoldier );

			}
		}
	}
}


UINT8 FindNextMercInTeamPanel( SOLDIERTYPE *pSoldier, BOOLEAN fGoodForLessOKLife, BOOLEAN fOnlyRegularMercs )
{
	INT32 cnt;
	INT32 bFirstID;
	SOLDIERTYPE *pTeamSoldier;


	bFirstID = GetTeamSlotFromPlayerID( pSoldier->ubID );

	if ( bFirstID == -1 )
	{
		return( pSoldier->ubID );
	}

	for ( cnt = ( bFirstID + 1 ); cnt < NUM_TEAM_SLOTS; cnt++ )
	{
		if ( gTeamPanel[ cnt ].fOccupied )
		{
			// Set Id to close
			pTeamSoldier = MercPtrs[ gTeamPanel[ cnt ].ubID ];

			if ( fOnlyRegularMercs )
			{
				if ( pTeamSoldier->bActive && ( AM_AN_EPC( pTeamSoldier ) || AM_A_ROBOT( pTeamSoldier ) ) )
				{
					continue;
				}
			}

			if ( fGoodForLessOKLife )
			{
				if ( pTeamSoldier->bLife > 0 && pTeamSoldier->bActive && pTeamSoldier->bInSector && pTeamSoldier->bTeam == gbPlayerNum && pTeamSoldier->bAssignment < ON_DUTY  && OK_INTERRUPT_MERC( pTeamSoldier ) && pSoldier->bAssignment == pTeamSoldier->bAssignment )
				{
					return( (UINT8)gTeamPanel[ cnt ].ubID );
				}
			}
			else
			{
				if ( OK_CONTROLLABLE_MERC( pTeamSoldier) && OK_INTERRUPT_MERC( pTeamSoldier ) && pSoldier->bAssignment == pTeamSoldier->bAssignment )
				{
					return( (UINT8)gTeamPanel[ cnt ].ubID );
				}
			}
		}
	}


	if ( ( gusSelectedSoldier != NO_SOLDIER ) && ( gGameSettings.fOptions[ TOPTION_SPACE_SELECTS_NEXT_SQUAD ] ) )
	{ 
		// only allow if nothing in hand and if in SM panel, the Change Squad button must be enabled
		if ( ( ( gsCurInterfacePanel != TEAM_PANEL ) || ( ButtonList[ iTEAMPanelButtons[ CHANGE_SQUAD_BUTTON ] ]->uiFlags & BUTTON_ENABLED ) ) )
		{
			SOLDIERTYPE *pNewSoldier;
			INT32		iCurrentSquad;

			//Select next squad
			iCurrentSquad = CurrentSquad( );

			pNewSoldier = FindNextActiveSquad( MercPtrs[ gusSelectedSoldier ] );

			if ( pNewSoldier->bAssignment != iCurrentSquad )
			{
				ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, pMessageStrings[ MSG_SQUAD_ACTIVE ], ( CurrentSquad( ) + 1 ) );

				return( pNewSoldier->ubID );
			}
		}
	}

	// none found,
	// Now loop back
	for ( cnt = 0; cnt < bFirstID; cnt++ )
	{
		if ( gTeamPanel[ cnt ].fOccupied )
		{
			pTeamSoldier = MercPtrs[ gTeamPanel[ cnt ].ubID ];

			if ( fOnlyRegularMercs )
			{
				if ( pTeamSoldier->bActive && ( AM_AN_EPC( pTeamSoldier ) || AM_A_ROBOT( pTeamSoldier ) ) )
				{
					continue;
				}
			}

			if ( fGoodForLessOKLife )
			{
				if ( pTeamSoldier->bLife > 0 && pTeamSoldier->bActive && pTeamSoldier->bInSector && pTeamSoldier->bTeam == gbPlayerNum && pTeamSoldier->bAssignment < ON_DUTY  && OK_INTERRUPT_MERC( pTeamSoldier ) && pSoldier->bAssignment == pTeamSoldier->bAssignment )
				{
					return( (UINT8)gTeamPanel[ cnt ].ubID );
				}
			}
			else
			{
				if ( OK_CONTROLLABLE_MERC( pTeamSoldier) && OK_INTERRUPT_MERC( pTeamSoldier ) && pSoldier->bAssignment == pTeamSoldier->bAssignment )
				{
					return( (UINT8)gTeamPanel[ cnt ].ubID );
				}
			}
		}
	}

	// IF we are here, keep as we always were!
	return( pSoldier->ubID );
}


void HelpTextDoneCallback( void )
{
	gfRerenderInterfaceFromHelpText = TRUE;

	//To redraw the Store Keeper Interface
	gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;
}


void DisableTacticalTeamPanelButtons( BOOLEAN fDisable )
{
	gfDisableTacticalPanelButtons = fDisable;
}


void BeginKeyPanelFromKeyShortcut( )
{
	SOLDIERTYPE *pSoldier = NULL;
	INT16 sStartYPosition = 0;
	INT16 sWidth = 0, sHeight = 0;

	if ( gsCurInterfacePanel != SM_PANEL )
	{
		return;
	}

	if ( gpSMCurrentMerc == NULL )
	{
		return;
	}

	if ( gfInKeyRingPopup )
	{
		return;
	}


	sStartYPosition = INV_INTERFACE_START_Y;
	sWidth = SCREEN_WIDTH;
	sHeight = ( SCREEN_HEIGHT - ( INV_INTERFACE_START_Y ) );
	pSoldier = gpSMCurrentMerc;

	//if we are in the shop keeper interface
	if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
		return;

	InitKeyRingPopup( pSoldier, 0, sStartYPosition, sWidth, sHeight ); 
}


void KeyRingItemPanelButtonCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	SOLDIERTYPE *pSoldier = NULL;
	INT16 sStartYPosition = 0;
	INT16 sWidth = 0, sHeight = 0;

	if( guiCurrentScreen == MAP_SCREEN )
	{
		if( bSelectedInfoChar == -1 )
		{
			return;
		}

		if( gCharactersList[ bSelectedInfoChar ].fValid == FALSE )
		{
			return;
		}

		
		

		pSoldier = &( Menptr[ gCharactersList[ bSelectedInfoChar ].usSolID ] );
		sStartYPosition = MAP_START_KEYRING_Y;
		sWidth = 261;
		sHeight = ( 359 - 107 );
	}
	else
	{
		if ( gpSMCurrentMerc == NULL )
		{
			return;
		}

		sStartYPosition = INV_INTERFACE_START_Y;
		sWidth = SCREEN_WIDTH;
		sHeight = ( SCREEN_HEIGHT - ( INV_INTERFACE_START_Y ) );
		pSoldier = gpSMCurrentMerc;
	}

	//if we are in the shop keeper interface
	if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
		return;

	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}

	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		if( guiCurrentScreen == MAP_SCREEN )
		{
			// want the inv done button shutdown and the region behind the keyring shaded
			//ForceButtonUnDirty( giMapInvDoneButton );
			// shade the background
			ShadowVideoSurfaceRect( FRAME_BUFFER , 0, 107, 261, 359 );
			InvalidateRegion( 0, 107, 261, 359 );
		}

		InitKeyRingPopup( pSoldier, 0, sStartYPosition, sWidth, sHeight ); 
	}
}

void KeyRingSlotInvClickCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	UINT32 uiKeyRing;
	// Copyies of values
	UINT16	usOldItemIndex;
	static BOOLEAN	fRightDown = FALSE;
	static BOOLEAN	fLeftDown = FALSE;
	INT32 iNumberOfKeysTaken = 0;

	uiKeyRing = MSYS_GetRegionUserData( pRegion, 0 );

	
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}

	//if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	//{
	//	fLeftDown = TRUE;
	//}
	//else if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP && fLeftDown )
	if (iReason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		fLeftDown = FALSE;

		//if we are in the shop keeper interface
		if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
		{
			INVENTORY_IN_SLOT InvSlot;

			if( gMoveingItem.sItemIndex == 0 )
			{
				//Delete the contents of the item cursor
				memset( &gMoveingItem, 0, sizeof( INVENTORY_IN_SLOT ) );
			}
			else
			{
				memset( &InvSlot, 0, sizeof( INVENTORY_IN_SLOT ) );

				// Return if empty
				//if ( gpSMCurrentMerc->inv[ uiHandPos ].usItem == NOTHING )
				//	return;


				// Fill out the inv slot for the item
				//InvSlot.sItemIndex = gpSMCurrentMerc->inv[ uiHandPos ].usItem;
//			InvSlot.ubNumberOfItems = gpSMCurrentMerc->inv[ uiHandPos ].ubNumberOfObjects;
//			InvSlot.ubItemQuality = gpSMCurrentMerc->inv[ uiHandPos ].bGunStatus;
				//memcpy( &InvSlot.ItemObject, &gpSMCurrentMerc->inv[ uiHandPos ], sizeof( OBJECTTYPE ) );
				//InvSlot.ubLocationOfObject = PLAYERS_INVENTORY;

				//InvSlot.ubIdOfMercWhoOwnsTheItem = gpSMCurrentMerc->ubProfile;


				//Add the item to the Players Offer Area
				//AddItemToPlayersOfferArea( gpSMCurrentMerc->ubProfile, &InvSlot, (UINT8)uiHandPos );

				// Dirty 
				fInterfacePanelDirty = DIRTYLEVEL2;
			}
			return;
		}

		// If we do not have an item in hand, start moving it
		if ( gpItemPointer == NULL )
		{
			// Return if empty
			if( ( gpItemPopupSoldier->pKeyRing[ uiKeyRing ].ubKeyID == INVALID_KEY_NUMBER ) || ( gpItemPopupSoldier->pKeyRing[ uiKeyRing ].ubNumber == 0 ) )
				return;

			// If our flags are set to do this, gofoit!
			if ( (guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN ) )
			{
			}
			else
			{
				if ( gpItemPopupSoldier->ubID != gusSelectedSoldier )
				{
					SelectSoldier( gpItemPopupSoldier->ubID, FALSE, FALSE );
				}
			}

			usOldItemIndex =  ( UINT16 )uiKeyRing ;

			BeginKeyRingItemPointer( gpItemPopupSoldier, (UINT8)usOldItemIndex );
			//BeginItemPointer( gpSMCurrentMerc, (UINT8)uiHandPos );

		}
		else
		{
			UINT8			ubSrcID, ubDestID;
			BOOLEAN		fOKToGo = FALSE;
			BOOLEAN		fDeductPoints = FALSE;

			if( ( gpItemPointer ->usItem < FIRST_KEY ) || ( gpItemPointer ->usItem > KEY_32 ) )
			{
				return;
			}

			// ATE: OK, get source, dest guy if different... check for and then charge appropriate APs
			ubSrcID  = ( UINT8 )gCharactersList[ bSelectedInfoChar ].usSolID;
			if ( gpItemPointerSoldier )
			{
				ubDestID = gpItemPointerSoldier->ubID;
			}
			else
			{
				ubDestID = ubSrcID;
			}

			if ( ubSrcID == ubDestID )
			{
				// We are doing this ourselve, continue
				fOKToGo = TRUE;
			} 
			else
			{
				// These guys are different....
				fDeductPoints = TRUE;

				// First check points for src guy
				if ( gpItemPointerSoldier->bLife >= CONSCIOUSNESS )
				{
					if ( EnoughPoints( gpItemPointerSoldier, 2, 0, TRUE ) )
					{
						fOKToGo = TRUE;
					}
				}
				else
				{
					fOKToGo = TRUE;
				}

				// Should we go on?
				if ( fOKToGo )
				{
					if ( gpSMCurrentMerc->bLife >= CONSCIOUSNESS )
					{
						if ( EnoughPoints( gpSMCurrentMerc, 2, 0, TRUE ) )
						{
							fOKToGo = TRUE;
						}
						else
						{
							fOKToGo = FALSE;
						}
					}
				}
			}

			if ( fOKToGo )
			{
				//usOldItemIndex = gpSMCurrentMerc->inv[ uiHandPos ].usItem;
				//usNewItemIndex = gpItemPointer->usItem;

				if ( gpItemPopupSoldier->pKeyRing[ uiKeyRing ].ubKeyID == INVALID_KEY_NUMBER || gpItemPopupSoldier->pKeyRing[ uiKeyRing ].ubKeyID == gpItemPointer->ubKeyID)
				{
					// Try to place here
					if ( ( iNumberOfKeysTaken = AddKeysToSlot( gpItemPopupSoldier, ( INT8 )uiKeyRing, gpItemPointer ) ) )
					{
					
						if ( fDeductPoints )
						{
							// Deduct points
							if ( gpItemPointerSoldier->bLife >= CONSCIOUSNESS )
							{
								DeductPoints( gpItemPointerSoldier,  2, 0 );
							}
							if ( gpItemPopupSoldier->bLife >= CONSCIOUSNESS )
							{
								DeductPoints( gpItemPopupSoldier,  2, 0 );
							}
						}
						
						// Dirty 
						fInterfacePanelDirty = DIRTYLEVEL2;

						gpItemPointer->ubNumberOfObjects -= ( UINT8 )iNumberOfKeysTaken;

						// Check if it's the same now!
						if ( gpItemPointer->ubNumberOfObjects == 0 )
						{
							if ( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN )
							{
								MAPEndItemPointer();
							}
							else
							{
								EndItemPointer();
							}
						}

						// Setup a timer....
						//guiMouseOverItemTime = GetJA2Clock( );
						//gfCheckForMouseOverItem = TRUE;
						//gbCheckForMouseOverItemPos = (INT8)uiHandPos;

					}
				}
				else
				{
					// Swap!
					SwapKeysToSlot( gpItemPopupSoldier, (INT8) uiKeyRing, gpItemPointer );

					if ( fDeductPoints )
					{
						// Deduct points
						if ( gpItemPointerSoldier && gpItemPointerSoldier->bLife >= CONSCIOUSNESS )
						{
							DeductPoints( gpItemPointerSoldier,  2, 0 );
						}
						if ( gpSMCurrentMerc->bLife >= CONSCIOUSNESS )
						{
							DeductPoints( gpSMCurrentMerc,  2, 0 );
						}
					}
					
					// Dirty 
					fInterfacePanelDirty = DIRTYLEVEL2;

				}
			}
		}

		UpdateItemHatches();
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_DWN)
	{
		fRightDown = TRUE;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP && fRightDown )
	{
		fRightDown = FALSE;

		// Return if empty
		if( ( gpItemPopupSoldier->pKeyRing[ uiKeyRing ].ubKeyID == INVALID_KEY_NUMBER ) || ( gpItemPopupSoldier->pKeyRing[ uiKeyRing ].ubNumber == 0 ) )
		{
			DeleteKeyRingPopup( );
			fTeamPanelDirty = TRUE;
			return;
		}
		// Some global stuff here - for esc, etc
		// Check for # of slots in item
		if ( !InItemDescriptionBox( ) )
		{
			if ( guiTacticalInterfaceFlags & INTERFACE_MAPSCREEN )
			{
				//InitKeyItemDescriptionBox( gpItemPopupSoldier, (UINT8)uiKeyRing, MAP_ITEMDESC_START_X, MAP_ITEMDESC_START_Y, 0 );			
      }
      else
      {
				InitKeyItemDescriptionBox( gpItemPopupSoldier, (UINT8)uiKeyRing, SM_ITEMDESC_START_X, SM_ITEMDESC_START_Y, 0 );			
      }
		}
	}
	else if (iReason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		fRightDown = FALSE;
		fLeftDown = FALSE;
	}

}



void DisableSMPpanelButtonsWhenInShopKeeperInterface( BOOLEAN fDontDrawButtons )
{
	//Go through the buttons that will be under the ShopKeepers ATM panel and disable them
	DisableButton( iSMPanelButtons[ STANCEUP_BUTTON ] );
	DisableButton( iSMPanelButtons[ UPDOWN_BUTTON ] );
	DisableButton( iSMPanelButtons[ CLIMB_BUTTON ] );
	DisableButton( iSMPanelButtons[ STANCEDOWN_BUTTON ] );
	DisableButton( iSMPanelButtons[ HANDCURSOR_BUTTON ] );
	DisableButton( iSMPanelButtons[ BURSTMODE_BUTTON ] );
	DisableButton( iSMPanelButtons[ LOOK_BUTTON ] );
	DisableButton( iSMPanelButtons[ TALK_BUTTON ] );
	DisableButton( iSMPanelButtons[ MUTE_BUTTON ] );

	DisableButton( giSMStealthButton );

	//Make sure the options button is disabled
	DisableButton( iSMPanelButtons[ OPTIONS_BUTTON ] );

	//Make sure the mapscreen button is disabled
	DisableButton( iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ] );

	DisableButton( iSMPanelButtons[ STANCEUP_BUTTON ] );
	DisableButton( iSMPanelButtons[ UPDOWN_BUTTON ] );
	DisableButton( iSMPanelButtons[ CLIMB_BUTTON ] );
	DisableButton( iSMPanelButtons[ STANCEDOWN_BUTTON ] );
	DisableButton( iSMPanelButtons[ HANDCURSOR_BUTTON ] );
	DisableButton( iSMPanelButtons[ BURSTMODE_BUTTON ] );
	DisableButton( iSMPanelButtons[ LOOK_BUTTON ] );
	DisableButton( iSMPanelButtons[ TALK_BUTTON ] );
	DisableButton( iSMPanelButtons[ MUTE_BUTTON ] );

	DisableButton( giSMStealthButton );




	if( fDontDrawButtons )
	{
//ATM:

		//Go through the buttons that will be under the ShopKeepers ATM panel and disable them
		ButtonList[ iSMPanelButtons[ STANCEUP_BUTTON ] ]->uiFlags &= ~BUTTON_DIRTY;
		ButtonList[ iSMPanelButtons[ UPDOWN_BUTTON ] ]->uiFlags &= ~BUTTON_DIRTY;
		ButtonList[ iSMPanelButtons[ CLIMB_BUTTON ] ]->uiFlags &= ~BUTTON_DIRTY;
		ButtonList[ iSMPanelButtons[ STANCEDOWN_BUTTON ] ]->uiFlags &= ~BUTTON_DIRTY;
		ButtonList[ iSMPanelButtons[ HANDCURSOR_BUTTON ] ]->uiFlags &= ~BUTTON_DIRTY;
		ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->uiFlags &= ~BUTTON_DIRTY;
		ButtonList[ iSMPanelButtons[ LOOK_BUTTON ] ]->uiFlags &= ~BUTTON_DIRTY;
		ButtonList[ iSMPanelButtons[ TALK_BUTTON ] ]->uiFlags &= ~BUTTON_DIRTY;
		ButtonList[ iSMPanelButtons[ MUTE_BUTTON ] ]->uiFlags &= ~BUTTON_DIRTY;

		ButtonList[ giSMStealthButton ]->uiFlags &= ~BUTTON_DIRTY;

		//Make sure the options button is disabled
		ButtonList[ iSMPanelButtons[ OPTIONS_BUTTON ] ]->uiFlags &= ~BUTTON_ENABLED;

		//Make sure the mapscreen button is disabled
		ButtonList[ iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ] ]->uiFlags &= ~BUTTON_ENABLED;

		ButtonList[ iSMPanelButtons[ STANCEUP_BUTTON ] ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		ButtonList[ iSMPanelButtons[ UPDOWN_BUTTON ] ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		ButtonList[ iSMPanelButtons[ CLIMB_BUTTON ] ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		ButtonList[ iSMPanelButtons[ STANCEDOWN_BUTTON ] ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		ButtonList[ iSMPanelButtons[ HANDCURSOR_BUTTON ] ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		ButtonList[ iSMPanelButtons[ BURSTMODE_BUTTON ] ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		ButtonList[ iSMPanelButtons[ LOOK_BUTTON ] ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		ButtonList[ iSMPanelButtons[ TALK_BUTTON ] ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
		ButtonList[ iSMPanelButtons[ MUTE_BUTTON ] ]->uiFlags |= BUTTON_FORCE_UNDIRTY;

		ButtonList[ giSMStealthButton ]->uiFlags |= BUTTON_FORCE_UNDIRTY;
	}
}


BOOLEAN IsMouseInRegion( MOUSE_REGION *pRegion )
{
	if ( (gusMouseXPos >= pRegion->RegionTopLeftX ) && (gusMouseXPos <= pRegion->RegionBottomRightX ) && (gusMouseYPos >= pRegion->RegionTopLeftY ) && (gusMouseYPos <= pRegion->RegionBottomRightY ) )
	{
		return( TRUE );
	}
	else
	{
		return( FALSE );
	}
}


void EnableButtonsForInItemBox( BOOLEAN fEnable )
{
	if ( !fEnable )
	{
		DisableButton( iSMPanelButtons[ NEXTMERC_BUTTON ] );
		DisableButton( iSMPanelButtons[ PREVMERC_BUTTON ] );
		DisableButton( iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ] );
	}
	else
	{
		EnableButton( iSMPanelButtons[ NEXTMERC_BUTTON ] );
		EnableButton( iSMPanelButtons[ PREVMERC_BUTTON ] );

		if ( !gfDisableTacticalPanelButtons )
		{
			EnableButton( iSMPanelButtons[ SM_MAP_SCREEN_BUTTON ] );
		}
	}
}	


void SMInvMoneyButtonCallback( MOUSE_REGION * pRegion, INT32 iReason )
{
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
		return;
	}
	else if (iReason == MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		//If the current merc is to far away, dont allow anything to be done
		if( gfSMDisableForItems )
			return;

		//if the player has an item in his hand, 
		if( gpItemPointer != NULL )
		{
			//and the item is money
			if( Item[ gpItemPointer->usItem ].usItemClass == IC_MONEY )
			{
				CHAR16	zText[512];
				CHAR16	zMoney[64];

        // Make sure we go back to movement mode...
		    guiPendingOverrideEvent = A_CHANGE_TO_MOVE;
		    HandleTacticalUI( );

				swprintf( zMoney, L"%d", gpItemPointer->uiMoneyAmount );

				InsertCommasForDollarFigure( zMoney );
				InsertDollarSignInToString( zMoney );

				//ask the user if they are sure they want to deposit the money
				swprintf( zText, gzMoneyWithdrawMessageText[ CONFIRMATION_TO_DEPOSIT_MONEY_TO_ACCOUNT ], zMoney );

				if( guiCurrentScreen == SHOPKEEPER_SCREEN )
				{
					//if we are in the shop keeper interface, free the cursor
					if( guiTacticalInterfaceFlags & INTERFACE_SHOPKEEP_INTERFACE )
					{
						FreeMouseCursor();
					}

					DoMessageBox( MSG_BOX_BASIC_STYLE, zText, SHOPKEEPER_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, ConfirmationToDepositMoneyToPlayersAccount, NULL );
				}
				else
					DoMessageBox( MSG_BOX_BASIC_STYLE, zText, GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, ConfirmationToDepositMoneyToPlayersAccount, NULL );
			}
		}

		//else bring up the money item description box to remove money from the players account
		else
		{
			//set the flag indicating we are removing money from the players account
			gfAddingMoneyToMercFromPlayersAccount = TRUE;

			//create the temp object from the players account balance
//			if( LaptopSaveInfo.iCurrentBalance > MAX_MONEY_PER_SLOT )
//				CreateMoney( MAX_MONEY_PER_SLOT, &gItemPointer );
//			else
				CreateMoney( LaptopSaveInfo.iCurrentBalance, &gItemPointer );

			InternalInitItemDescriptionBox( &gItemPointer, SM_ITEMDESC_START_X, SM_ITEMDESC_START_Y, 0, gpSMCurrentMerc );
		}
	}
}

void ConfirmationToDepositMoneyToPlayersAccount( UINT8 ubExitValue )
{
	if ( ubExitValue == MSG_BOX_RETURN_YES )
	{
		//add the money to the players account
		AddTransactionToPlayersBook( MERC_DEPOSITED_MONEY_TO_PLAYER_ACCOUNT, gpSMCurrentMerc->ubProfile, GetWorldTotalMin(), gpItemPointer->uiMoneyAmount );

		// dirty shopkeeper
		gubSkiDirtyLevel = SKI_DIRTY_LEVEL2;

		EndItemPointer( );
	}
}


void ReEvaluateDisabledINVPanelButtons( )
{
	gfReEvaluateDisabledINVPanelButtons = TRUE;
}

void CheckForReEvaluateDisabledINVPanelButtons( )
{
	// OK, if we currently have an item pointer up....
	if ( gfReEvaluateDisabledINVPanelButtons )
	{
		if ( gpItemPointer || gfInItemPickupMenu )
		{
			EnableSMPanelButtons( FALSE, TRUE );
		}
		else
		{
			EnableSMPanelButtons( TRUE, TRUE );
		}

		gfReEvaluateDisabledINVPanelButtons = FALSE;
	}
}


void AbandonBoxingCallback( UINT8 ubExitValue )
{
	if ( ubExitValue == MSG_BOX_RETURN_YES )
	{
		// ok, proceed!
		SetBoxingState( NOT_BOXING );
		gfEnteringMapScreen = TRUE;
	}
	// otherwise do nothing
}

void GoToMapScreenFromTactical( void )
{
	if ( gTacticalStatus.bBoxingState != NOT_BOXING )
	{
		// pop up dialogue asking whether the player wants to abandon the fight
		DoMessageBox( MSG_BOX_BASIC_STYLE, Message[ STR_ABANDON_FIGHT ], GAME_SCREEN, ( UINT8 )MSG_BOX_FLAG_YESNO, AbandonBoxingCallback, NULL );		
		return;
	}
	// ok, proceed!
	gfEnteringMapScreen = TRUE;
}


void HandleTacticalEffectsOfEquipmentChange( SOLDIERTYPE *pSoldier, UINT32 uiInvPos, UINT16 usOldItem, UINT16 usNewItem )
{
	// if in attached weapon mode and don't have weapon with GL attached in hand, reset weapon mode
	if ( (pSoldier->bWeaponMode == WM_ATTACHED_GL || pSoldier->bWeaponMode == WM_ATTACHED_GL_BURST || pSoldier->bWeaponMode == WM_ATTACHED_GL_AUTO )&& !IsGrenadeLauncherAttached( &(pSoldier->inv[ HANDPOS ]) ) )
	{
		if ( !Weapon[pSoldier->inv[ HANDPOS ].usItem].NoSemiAuto )
		{
			pSoldier->bWeaponMode = WM_NORMAL;
			pSoldier->bDoBurst = FALSE;
			pSoldier->bDoAutofire = 0;
		}
		else
		{
			pSoldier->bWeaponMode = WM_AUTOFIRE;
			pSoldier->bDoBurst = TRUE;
			pSoldier->bDoAutofire = 1;
		}
	}

	// if he is loaded tactically
	if ( pSoldier->bInSector )
	{
		// If this is our main hand
		if ( uiInvPos == HANDPOS || uiInvPos == SECONDHANDPOS )
		{
			// check if we need to change animation!
			ReLoadSoldierAnimationDueToHandItemChange( pSoldier, usOldItem, usNewItem );
		}

		// if this is head gear
		if ( uiInvPos == HEAD1POS || uiInvPos == HEAD2POS )
		{
			// Could be because of GOGGLES change...  Re-create light...
			DeleteSoldierLight( pSoldier );
			PositionSoldierLight( pSoldier );
		}
	}
	else
	{
		// as a minimum
		if ( (Item[ pSoldier->inv[ HANDPOS ].usItem ].usItemClass & IC_WEAPON) && GetShotsPerBurst(&pSoldier->inv[ HANDPOS ]) == 0 )
		{
			if ( !Weapon[pSoldier->inv[ HANDPOS ].usItem].NoSemiAuto )
			{
				pSoldier->bWeaponMode = WM_NORMAL;
				pSoldier->bDoBurst = FALSE;
				pSoldier->bDoAutofire = 0;
			}
			else
			{
				pSoldier->bWeaponMode = WM_AUTOFIRE;
				pSoldier->bDoAutofire = 1;
				pSoldier->bDoBurst = TRUE;
			}
		}
	}
}