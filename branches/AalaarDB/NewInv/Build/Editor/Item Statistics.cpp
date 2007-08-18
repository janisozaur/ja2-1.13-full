#ifdef PRECOMPILEDHEADERS
	#include "Editor All.h"
#else
	#include "builddefines.h"
#endif

#ifdef JA2EDITOR

#ifndef PRECOMPILEDHEADERS
	#include <memory.h>
	#include <stdio.h>
	#include "types.h"
	#include "mousesystem.h"
	#include "Button System.h"
	#include "font control.h"
	#include "EditorDefines.h"
	#include "Editor Taskbar Utils.h"
	#include "EditorMercs.h"
	#include "EditorItems.h"
	#include "Item Statistics.h"
	#include "Text Input.h"
	#include "Action Items.h"
	#include "Item types.h"
	#include "video.h"
	#include "Simple Render Utils.h"
	#include "Weapons.h"
	#include "Items.h"
	#include "editscreen.h"
	#include "Random.h"
	#include "Handle Items.h"
	#include "World Items.h"
	#include "PopupMenu.h"
	#include "pits.h"
#endif

INT32 giBothCheckboxButton = -1;
INT32 giRealisticCheckboxButton = -1;
INT32 giSciFiCheckboxButton = -1;
INT32 giAlarmTriggerButton = -1;
INT32 giOwnershipGroupButton = -1;

CHAR16 gszActionItemDesc[ NUM_ACTIONITEMS ][ 30 ] = 
{
	L"Klaxon Mine",
	L"Flare Mine",
	L"Teargas Explosion",
	L"Stun Explosion",
	L"Smoke Explosion",
	L"Mustard Gas",
	L"Land Mine",
	L"Open Door",
	L"Close Door",
	L"3x3 Hidden Pit",
	L"5x5 Hidden Pit",
	L"Small Explosion",
	L"Medium Explosion",
	L"Large Explosion",
	L"Toggle Door",
	L"Toggle Action1s",
	L"Toggle Action2s",
	L"Toggle Action3s",
	L"Toggle Action4s",
	L"Enter Brothel",
	L"Exit Brothel",
	L"Kingpin Alarm",
	L"Sex with Prostitute",
	L"Reveal Room",
	L"Local Alarm",
	L"Global Alarm",
	L"Klaxon Sound",
	L"Unlock door",
	L"Toggle lock",
	L"Untrap door",
	L"Tog pressure items",
	L"Museum alarm",
	L"Bloodcat alarm",
	L"Big teargas",
};

const STR16 GetActionItemName( OBJECTTYPE *pItem )
{
	PERFORMANCE_MARKER
	if( !pItem || pItem->usItem != ACTION_ITEM )
		return NULL;
	if( (*pItem)[0]->data.bombs.bActionValue != ACTION_ITEM_BLOW_UP )
	{
		switch( (*pItem)[0]->data.bombs.bActionValue )
		{
			case ACTION_ITEM_OPEN_DOOR:								return gszActionItemDesc[ ACTIONITEM_OPEN ];
			case ACTION_ITEM_CLOSE_DOOR:							return gszActionItemDesc[ ACTIONITEM_CLOSE ];
			case ACTION_ITEM_SMALL_PIT:								return gszActionItemDesc[ ACTIONITEM_SMPIT ];
			case ACTION_ITEM_LARGE_PIT:								return gszActionItemDesc[ ACTIONITEM_LGPIT ];
			case ACTION_ITEM_TOGGLE_DOOR:							return gszActionItemDesc[ ACTIONITEM_TOGGLE_DOOR ];
			case ACTION_ITEM_TOGGLE_ACTION1:					return gszActionItemDesc[ ACTIONITEM_TOGGLE_ACTION1 ];
			case ACTION_ITEM_TOGGLE_ACTION2:					return gszActionItemDesc[ ACTIONITEM_TOGGLE_ACTION2 ];
			case ACTION_ITEM_TOGGLE_ACTION3:					return gszActionItemDesc[ ACTIONITEM_TOGGLE_ACTION3 ];
			case ACTION_ITEM_TOGGLE_ACTION4:					return gszActionItemDesc[ ACTIONITEM_TOGGLE_ACTION4 ];
			case ACTION_ITEM_ENTER_BROTHEL:						return gszActionItemDesc[ ACTIONITEM_ENTER_BROTHEL ];
			case ACTION_ITEM_EXIT_BROTHEL:						return gszActionItemDesc[ ACTIONITEM_EXIT_BROTHEL ];
			case ACTION_ITEM_KINGPIN_ALARM:						return gszActionItemDesc[ ACTIONITEM_KINGPIN_ALARM ];
			case ACTION_ITEM_SEX:											return gszActionItemDesc[ ACTIONITEM_SEX ];
			case ACTION_ITEM_REVEAL_ROOM:							return gszActionItemDesc[ ACTIONITEM_REVEAL_ROOM ];
			case ACTION_ITEM_LOCAL_ALARM:							return gszActionItemDesc[ ACTIONITEM_LOCAL_ALARM ];
			case ACTION_ITEM_GLOBAL_ALARM:						return gszActionItemDesc[ ACTIONITEM_GLOBAL_ALARM ];
			case ACTION_ITEM_KLAXON:									return gszActionItemDesc[ ACTIONITEM_KLAXON ];
			case ACTION_ITEM_UNLOCK_DOOR:							return gszActionItemDesc[ ACTIONITEM_UNLOCK_DOOR ];
			case ACTION_ITEM_TOGGLE_LOCK:							return gszActionItemDesc[ ACTIONITEM_TOGGLE_LOCK ];
			case ACTION_ITEM_UNTRAP_DOOR:							return gszActionItemDesc[ ACTIONITEM_UNTRAP_DOOR ];
			case ACTION_ITEM_TOGGLE_PRESSURE_ITEMS:		return gszActionItemDesc[ ACTIONITEM_TOGGLE_PRESSURE_ITEMS ];
			case ACTION_ITEM_MUSEUM_ALARM:						return gszActionItemDesc[ ACTIONITEM_MUSEUM_ALARM ];
			case ACTION_ITEM_BLOODCAT_ALARM:					return gszActionItemDesc[ ACTIONITEM_BLOODCAT_ALARM ];
			default:																	return NULL;
		}
	}
	else switch( (*pItem)[0]->data.bombs.usBombItem )
	{
		case STUN_GRENADE:			return gszActionItemDesc[ ACTIONITEM_STUN ];
		case SMOKE_GRENADE:			return gszActionItemDesc[ ACTIONITEM_SMOKE ];
		case TEARGAS_GRENADE:		return gszActionItemDesc[ ACTIONITEM_TEARGAS ];
		case MUSTARD_GRENADE:		return gszActionItemDesc[ ACTIONITEM_MUSTARD ];
		case HAND_GRENADE:			return gszActionItemDesc[ ACTIONITEM_SMALL ];
		case TNT:								return gszActionItemDesc[ ACTIONITEM_MEDIUM ];
		case C4:								return gszActionItemDesc[ ACTIONITEM_LARGE ];
		case MINE:							return gszActionItemDesc[ ACTIONITEM_MINE ];
		case TRIP_FLARE:				return gszActionItemDesc[ ACTIONITEM_FLARE ];
		case TRIP_KLAXON:				return gszActionItemDesc[ ACTIONITEM_TRIP_KLAXON ];
		case BIG_TEAR_GAS:			return gszActionItemDesc[ ACTIONITEM_BIG_TEAR_GAS ];
		default:								return NULL;
	}
}

enum
{
	SILENCER_ATTACHMENT_BUTTON,
	SNIPERSCOPE_ATTACHMENT_BUTTON,
	LASERSCOPE_ATTACHMENT_BUTTON,
	BIPOD_ATTACHMENT_BUTTON,
	DUCKBILL_ATTACHMENT_BUTTON,
	GLAUNCHER_ATTACHMENT_BUTTON,
	NUM_ATTACHMENT_BUTTONS
};
UINT32 guiAttachmentButton[ NUM_ATTACHMENT_BUTTONS ];
BOOLEAN gfAttachment[ NUM_ATTACHMENT_BUTTONS ];
void ToggleAttachment( GUI_BUTTON *btn, INT32 reason );

UINT32 guiCeramicPlatesButton;
BOOLEAN gfCeramicPlates;
void ToggleCeramicPlates( GUI_BUTTON *btn, INT32 reason );

UINT32 guiDetonatorButton;
BOOLEAN gfDetonator;
void ToggleDetonator( GUI_BUTTON *btn, INT32 reason );

UINT32 guiActionItemButton;
void ActionItemCallback( GUI_BUTTON *btn, INT32 reason );
INT8 gbActionItemIndex = ACTIONITEM_MEDIUM;
INT8 gbDefaultBombTrapLevel = 9;

void RemoveBombFromWorldByItemIndex( INT32 iItemIndex );

enum
{
	EDITING_NOTHING,
	EDITING_NOT_YET_IMPLEMENTED,
	EDITING_DROPPABLE,
	EDITING_GUNS,
	EDITING_AMMO,
	EDITING_ARMOUR,
	EDITING_EQUIPMENT,
	EDITING_EXPLOSIVES,
	EDITING_MONEY,
	EDITING_ACTIONITEMS,
	EDITING_TRIGGERS,
	EDITING_KEYS,
	EDITING_OWNERSHIP,
};

INT8 gbEditingMode = EDITING_NOTHING;

OBJECTTYPE	*gpItem = NULL;
BOOLEAN			gfShowItemStatsPanel;
INT16				gsItemGridNo;

ITEM_POOL		*gpEditingItemPool = NULL;
extern ITEM_POOL *gpItemPool;

void SetupGameTypeFlags();
void RemoveGameTypeFlags();

void SetupGunGUI();
void ExtractAndUpdateGunGUI();
void RemoveGunGUI();

void SetupAmmoGUI();
void ExtractAndUpdateAmmoGUI();
void RemoveAmmoGUI();

void SetupArmourGUI();
void ExtractAndUpdateArmourGUI();
void RemoveArmourGUI();

void SetupEquipGUI();
void ExtractAndUpdateEquipGUI();
void RemoveEquipGUI();

void SetupExplosivesGUI();
void ExtractAndUpdateExplosivesGUI();
void RemoveExplosivesGUI();

void SetupTriggersGUI();
void ExtractAndUpdateTriggersGUI();
void RemoveTriggersGUI();

void SetupActionItemsGUI();
void ExtractAndUpdateActionItemsGUI();
void RemoveActionItemsGUI();

void SetupMoneyGUI();
void ExtractAndUpdateMoneyGUI();
void RemoveMoneyGUI();

void SetupOwnershipGUI();
void ExtractAndUpdateOwnershipGUI();
void RemoveOwnershipGUI();
void OwnershipGroupButtonCallback( GUI_BUTTON *btn, INT32 reason );

void SetupKeysGUI();
void ExtractAndUpdateKeysGUI();
void RemoveKeysGUI();


void ReEvaluateAttachmentStatii();

void ShowItemStatsPanel()
{
	PERFORMANCE_MARKER
	ShowEditorButtons( FIRST_ITEMSTATS_BUTTON, LAST_ITEMSTATS_BUTTON );
	if( iCurrentTaskbar == TASK_MERCS || !gpItem )
		HideEditorButton( ITEMSTATS_HIDDEN_BTN );
	gfShowItemStatsPanel = TRUE;
}

void HideItemStatsPanel()
{
	PERFORMANCE_MARKER
	HideEditorButtons( FIRST_ITEMSTATS_BUTTON, LAST_ITEMSTATS_BUTTON );
	SpecifyItemToEdit( NULL, -1 );
	gfShowItemStatsPanel = FALSE;
}

void EnableItemStatsPanel()
{
	PERFORMANCE_MARKER

}

void DisableItemStatsPanel()
{
	PERFORMANCE_MARKER

}

void ExecuteItemStatsCmd( UINT8 ubAction )
{
	PERFORMANCE_MARKER
	switch( ubAction )
	{
		case ITEMSTATS_APPLY:
			if( gpItem && gpItem->usItem == ACTION_ITEM )
			{
				ExtractAndUpdateActionItemsGUI();
			}
			else if( gpItem && gpItem->usItem == SWITCH )
			{
				ExtractAndUpdateTriggersGUI();
			}
			else if( gpItem && gpItem->usItem == OWNERSHIP )
			{
				ExtractAndUpdateOwnershipGUI();
			}
			else switch( gbEditingMode )
			{
				case EDITING_GUNS:				ExtractAndUpdateGunGUI();					break;
				case EDITING_AMMO:				ExtractAndUpdateAmmoGUI();				break;
				case EDITING_ARMOUR:			ExtractAndUpdateArmourGUI();			break;
				case EDITING_EQUIPMENT:		ExtractAndUpdateEquipGUI();				break;
				case EDITING_EXPLOSIVES:	ExtractAndUpdateExplosivesGUI();	break;
				case EDITING_MONEY:				ExtractAndUpdateMoneyGUI();				break;
				case EDITING_KEYS:				ExtractAndUpdateKeysGUI();				break;
			}
			SetActiveField( 0 );
			gfRenderTaskbar = TRUE;
			break;
		case ITEMSTATS_CANCEL:
			SpecifyItemToEdit( gpItem, gsItemGridNo );
			SetActiveField( 0 );
			break;
		case ITEMSTATS_DEFAULT:
			break;
		case ITEMSTATS_DELETE:
			DeleteSelectedItem();
			break;
		case ITEMSTATS_SHOW:
			ShowSelectedItem();
			break;
		case ITEMSTATS_HIDE:
			HideSelectedItem();
			break;
	}
}

void RemoveItemGUI()
{
	PERFORMANCE_MARKER
	if( !gpItem )
		return;
	if( TextInputMode() )
		KillTextInputMode();
	HideEditorButton( ITEMSTATS_HIDDEN_BTN );
	RemoveGameTypeFlags();
	if( gpItem && gpItem->usItem == ACTION_ITEM )
	{
		RemoveActionItemsGUI();
	}
	else if( gpItem && gpItem->usItem == SWITCH )
	{
		RemoveTriggersGUI();
	}
	else if( gpItem && gpItem->usItem == OWNERSHIP )
	{
		RemoveOwnershipGUI();
	}
	else switch( gbEditingMode )
	{
		case EDITING_GUNS:				RemoveGunGUI();					break;
		case EDITING_AMMO:				RemoveAmmoGUI();				break;
		case EDITING_ARMOUR:			RemoveArmourGUI();			break;
		case EDITING_EQUIPMENT:		RemoveEquipGUI();				break;
		case EDITING_EXPLOSIVES:	RemoveExplosivesGUI();	break;
		case EDITING_MONEY:				RemoveMoneyGUI();				break;
		case EDITING_KEYS:				RemoveKeysGUI();				break;
	}
}

void SpecifyItemToEdit( OBJECTTYPE *pItem, INT32 iMapIndex )
{
	PERFORMANCE_MARKER
	//Set the global item pointer to point to the new item
	if( gpItem == pItem )
		return;
	if( gpItem != pItem )
	{
		ExecuteItemStatsCmd( ITEMSTATS_APPLY );
		RemoveItemGUI();
		gpItem = pItem;
		if( gpItemPool )
		{
			gpEditingItemPool = gpItemPool;
		}
		gsItemGridNo = (INT16)iMapIndex;
	}
	else
		RemoveItemGUI();

	gfRenderTaskbar = TRUE;

	if( !gpItem )
	{
		//Hide all edit features.
		gbEditingMode = EDITING_NOTHING;
		gsItemGridNo = -1;
		return;
	}

	//Setup the text mode
	InitTextInputModeWithScheme( DEFAULT_SCHEME );

	if( iCurrentTaskbar == TASK_ITEMS )
		ShowEditorButton( ITEMSTATS_HIDDEN_BTN );

	//Determine the type of item so that we can determine the dynamic editing mode.
	AddUserInputField( NULL );

	SetupGameTypeFlags();

	if( Item[ gpItem->usItem ].usItemClass == IC_MONEY )
	{
		gbEditingMode = EDITING_MONEY;
		SetupMoneyGUI();
	}
	else if( gpItem->usItem == ACTION_ITEM )
	{
		gbEditingMode = EDITING_ACTIONITEMS;
		SetupActionItemsGUI();
		HideEditorButton( ITEMSTATS_HIDDEN_BTN );
	}
	else if( gpItem->usItem == SWITCH )
	{
		gbEditingMode = EDITING_TRIGGERS;
		SetupTriggersGUI();
		HideEditorButton( ITEMSTATS_HIDDEN_BTN );
	}
	else if( gpItem->usItem == OWNERSHIP )
	{
		gbEditingMode = EDITING_OWNERSHIP;
		SetupOwnershipGUI();
		HideEditorButton( ITEMSTATS_HIDDEN_BTN );
	}
	else switch( Item[ gpItem->usItem ].usItemClass )
	{
		case IC_GUN:
			gbEditingMode = EDITING_GUNS;
			SetupGunGUI();
			break;
		case IC_AMMO:
			gbEditingMode = EDITING_AMMO;
			SetupAmmoGUI();
			break;
		case IC_ARMOUR:
			gbEditingMode = EDITING_ARMOUR;
			SetupArmourGUI();
			break;
		case IC_MEDKIT:
		case IC_KIT:
		case IC_MISC:
		case IC_FACE:
		case IC_BLADE:
		case IC_LAUNCHER:
		case IC_THROWING_KNIFE:
		case IC_MONEY:
			gbEditingMode = EDITING_EQUIPMENT;
			SetupEquipGUI();
			break;
		case IC_THROWN:
		case IC_GRENADE:
		case IC_EXPLOSV:
		case IC_BOMB:
			gbEditingMode = EDITING_EXPLOSIVES;
			SetupExplosivesGUI();
			break;
		case IC_KEY:
			gbEditingMode = EDITING_KEYS;
			SetupKeysGUI();
			break;
		case IC_PUNCH:
			if ( gpItem->usItem != NOTHING)
			{
				gbEditingMode = EDITING_EQUIPMENT;
				SetupEquipGUI();
				break;
			}
			// else fall through and act as nothing
		case IC_NONE:
			gbEditingMode = EDITING_NOTHING;
			if( !(gpItem->fFlags & OBJECT_UNDROPPABLE) )
				gbEditingMode = EDITING_DROPPABLE;
			break;
		default:
			gbEditingMode = EDITING_NOT_YET_IMPLEMENTED;
			break;
	}
	if( gpItemPool )
	{
		if( gWorldItems[ gpItemPool->iItemIndex ].bVisible == INVISIBLE )
		{
			UnclickEditorButton( ITEMSTATS_HIDDEN_BTN );
			ShowSelectedItem();
		}
		else
		{
			ClickEditorButton( ITEMSTATS_HIDDEN_BTN );
			HideSelectedItem();
		}
	}
}

//called from the taskbar renderer.
void UpdateItemStatsPanel()
{
	PERFORMANCE_MARKER
	SetFont( FONT10ARIAL );
	SetFontForeground( FONT_GRAY2 );
	SetFontShadow( FONT_NEARBLACK );
	SetFontBackground( FONT_BLACK );
	if( gpItem && iCurrentTaskbar == TASK_ITEMS && 
			gbEditingMode != EDITING_TRIGGERS && gbEditingMode != EDITING_ACTIONITEMS )
	{
		mprintf( iScreenWidthOffset + 500, 2 * iScreenHeightOffset + 366, L"Toggle hide flag" );
	}
	SetFontForeground( FONT_YELLOW );
	switch( gbEditingMode )
	{
		case EDITING_NOTHING:
			if( iCurrentTaskbar == TASK_ITEMS )
				mprintf( iScreenWidthOffset + 520, 2 * iScreenHeightOffset + 400, L"No item selected." );
			else
			{
				mprintf( iScreenWidthOffset + 500, 2 * iScreenHeightOffset + 390, L"Slot available for" );
				mprintf( iScreenWidthOffset + 500, 2 * iScreenHeightOffset + 400, L"random generation." );
			}
			return;
		case EDITING_KEYS:
			if( !gpEditingItemPool )
			{
				mprintf( iScreenWidthOffset + 500, 2 * iScreenHeightOffset + 400, L"Keys not editable." );
				return;
			}
			break;
		case EDITING_OWNERSHIP:
			mprintf( iScreenWidthOffset + 512, 2 * iScreenHeightOffset + 384, L"ProfileID of owner" );
			return;
		case EDITING_NOT_YET_IMPLEMENTED:
			mprintf( iScreenWidthOffset + 500, 2 * iScreenHeightOffset + 400, L"Item class not implemented." );
			return;
		case EDITING_DROPPABLE:
			mprintf( iScreenWidthOffset + 500, 2 * iScreenHeightOffset + 400, L"Slot locked as empty.");
			return;
		case EDITING_GUNS:
			mprintf( iScreenWidthOffset + 512, 2 * iScreenHeightOffset + 384, L"Status" );
			mprintf( iScreenWidthOffset + 512, 2 * iScreenHeightOffset + 404, L"Rounds" );
			mprintf( iScreenWidthOffset + 512, 2 * iScreenHeightOffset + 424, L"Trap Level" );
			break;
		case EDITING_AMMO:
			mprintf( iScreenWidthOffset + 512, 2 * iScreenHeightOffset + 384, L"Quantity" );
			mprintf( iScreenWidthOffset + 512, 2 * iScreenHeightOffset + 404, L"Trap Level" );
			break;
		case EDITING_ARMOUR:
		case EDITING_EQUIPMENT:
			mprintf( iScreenWidthOffset + 512, 2 * iScreenHeightOffset + 384, L"Status" );
			mprintf( iScreenWidthOffset + 512, 2 * iScreenHeightOffset + 404, L"Trap Level" );
			break;
		case EDITING_EXPLOSIVES:
			mprintf( iScreenWidthOffset + 512, 2 * iScreenHeightOffset + 380, L"Status" );
			mprintf( iScreenWidthOffset + 512, 2 * iScreenHeightOffset + 404, L"Quantity" );
			mprintf( iScreenWidthOffset + 512, 2 * iScreenHeightOffset + 424, L"Trap Level" );
			break;
		case EDITING_MONEY:
			mprintf( iScreenWidthOffset + 532, 2 * iScreenHeightOffset + 384, L"Dollars" );
			break;
		case EDITING_ACTIONITEMS:
			mprintf( iScreenWidthOffset + 512, 2 * iScreenHeightOffset + 369, L"Status" );
			mprintf( iScreenWidthOffset + 512, 2 * iScreenHeightOffset + 389, L"Trap Level" );
			break;
		case EDITING_TRIGGERS:
			mprintf( iScreenWidthOffset + 512, 2 * iScreenHeightOffset + 369, L"Trap Level");
			mprintf( iScreenWidthOffset + 512, 2 * iScreenHeightOffset + 389, L"Tolerance" );
			if( gpEditingItemPool && (*gpItem)[0]->data.bombs.bFrequency >= PANIC_FREQUENCY_3 && (*gpItem)[0]->data.bombs.bFrequency <= PANIC_FREQUENCY )
				mprintf( iScreenWidthOffset + 500, 2 * iScreenHeightOffset + 407, L"Alarm Trigger" );
			break;
	}
	if( gpEditingItemPool )
	{
		INT32 iPercent = 100 - gWorldItems[ gpEditingItemPool->iItemIndex ].ubNonExistChance;
		if( iPercent == 100 )
			SetFontForeground( FONT_YELLOW );
		else if( iPercent >= 50 )
			SetFontForeground( FONT_ORANGE );
		else 
			SetFontForeground( FONT_RED );
		mprintf( iScreenWidthOffset + 512, 2 * iScreenHeightOffset + 444, L"Exist Chance" );
		mprintf( iScreenWidthOffset + 587, 2 * iScreenHeightOffset + 366, L"B" );
		mprintf( iScreenWidthOffset + 609, 2 * iScreenHeightOffset + 366, L"R" );
		mprintf( iScreenWidthOffset + 630, 2 * iScreenHeightOffset + 366, L"S" );
	}
	InvalidateRegion( iScreenWidthOffset + 477, 2 * iScreenHeightOffset + 362, 161, 97 );
}

void RealisticOnlyCheckboxCallback( GUI_BUTTON *btn, INT32 reason )
{
	PERFORMANCE_MARKER
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		ButtonList[ giRealisticCheckboxButton ]->uiFlags |= (BUTTON_CLICKED_ON | BUTTON_DIRTY);
		ButtonList[ giSciFiCheckboxButton ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ giSciFiCheckboxButton ]->uiFlags |= BUTTON_DIRTY;
		ButtonList[ giBothCheckboxButton ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ giBothCheckboxButton ]->uiFlags |= BUTTON_DIRTY;
		gWorldItems[ gpEditingItemPool->iItemIndex ].usFlags &= ~(WORLD_ITEM_REALISTIC_ONLY | WORLD_ITEM_SCIFI_ONLY);
		gWorldItems[ gpEditingItemPool->iItemIndex ].usFlags |= WORLD_ITEM_REALISTIC_ONLY;
	}
}

void SciFiOnlyCheckboxCallback( GUI_BUTTON *btn, INT32 reason )
{
	PERFORMANCE_MARKER
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		ButtonList[ giRealisticCheckboxButton ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ giRealisticCheckboxButton ]->uiFlags |= BUTTON_DIRTY;
		ButtonList[ giSciFiCheckboxButton ]->uiFlags |= (BUTTON_CLICKED_ON | BUTTON_DIRTY);
		ButtonList[ giBothCheckboxButton ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ giBothCheckboxButton ]->uiFlags |= BUTTON_DIRTY;
		gWorldItems[ gpEditingItemPool->iItemIndex ].usFlags &= ~(WORLD_ITEM_REALISTIC_ONLY | WORLD_ITEM_SCIFI_ONLY);
		gWorldItems[ gpEditingItemPool->iItemIndex ].usFlags |= WORLD_ITEM_SCIFI_ONLY;
	}
}

void BothModesCheckboxCallback( GUI_BUTTON *btn, INT32 reason )
{
	PERFORMANCE_MARKER
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		ButtonList[ giRealisticCheckboxButton ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ giRealisticCheckboxButton ]->uiFlags |= BUTTON_DIRTY;
		ButtonList[ giSciFiCheckboxButton ]->uiFlags &= ~BUTTON_CLICKED_ON;
		ButtonList[ giSciFiCheckboxButton ]->uiFlags |= BUTTON_DIRTY;
		ButtonList[ giBothCheckboxButton ]->uiFlags |= (BUTTON_CLICKED_ON | BUTTON_DIRTY);
		gWorldItems[ gpEditingItemPool->iItemIndex ].usFlags &= ~(WORLD_ITEM_REALISTIC_ONLY | WORLD_ITEM_SCIFI_ONLY);
	}
}

void SetupGameTypeFlags()
{
	PERFORMANCE_MARKER
	if( gpEditingItemPool )
	{
		giBothCheckboxButton = 
			CreateCheckBoxButton(	iScreenWidthOffset + 573, 2 * iScreenHeightOffset + 365, "EDITOR//radiobutton.sti", MSYS_PRIORITY_NORMAL, BothModesCheckboxCallback );
		SetButtonFastHelpText( giBothCheckboxButton, L"Item appears in both Sci-Fi and Realistic modes. (|B)" );
		giRealisticCheckboxButton = 
			CreateCheckBoxButton(	iScreenWidthOffset + 595, 2 * iScreenHeightOffset + 365, "EDITOR//radiobutton.sti", MSYS_PRIORITY_NORMAL, RealisticOnlyCheckboxCallback );
		SetButtonFastHelpText( giRealisticCheckboxButton, L"Item appears in |Realistic mode only." );
		giSciFiCheckboxButton = 
			CreateCheckBoxButton(	iScreenWidthOffset + 616, 2 * iScreenHeightOffset + 365, "EDITOR//radiobutton.sti", MSYS_PRIORITY_NORMAL, SciFiOnlyCheckboxCallback );
		SetButtonFastHelpText( giSciFiCheckboxButton, L"Item appears in |Sci-Fi mode only." );
		
		if( gWorldItems[ gpEditingItemPool->iItemIndex ].usFlags & WORLD_ITEM_REALISTIC_ONLY )
			ButtonList[ giRealisticCheckboxButton ]->uiFlags |= (BUTTON_CLICKED_ON | BUTTON_DIRTY);
		else if( gWorldItems[ gpEditingItemPool->iItemIndex ].usFlags & WORLD_ITEM_SCIFI_ONLY )
			ButtonList[ giSciFiCheckboxButton ]->uiFlags |= (BUTTON_CLICKED_ON | BUTTON_DIRTY);
		else
			ButtonList[ giBothCheckboxButton ]->uiFlags |= (BUTTON_CLICKED_ON | BUTTON_DIRTY);
	}
}

void RemoveGameTypeFlags()
{
	PERFORMANCE_MARKER
	if( giBothCheckboxButton != -1 )
	{
		RemoveButton( giBothCheckboxButton );
		giBothCheckboxButton = -1;
	}
	if( giRealisticCheckboxButton != -1 )
	{
		RemoveButton( giRealisticCheckboxButton );
		giRealisticCheckboxButton = -1;
	}
	if( giSciFiCheckboxButton != -1 )
	{
		RemoveButton( giSciFiCheckboxButton );
		giSciFiCheckboxButton = -1;
	}
}

void SetupGunGUI()
{
	PERFORMANCE_MARKER
	CHAR16 str[20];
	INT16 yp;
	memset( gfAttachment, 0, NUM_ATTACHMENT_BUTTONS );
	swprintf( str, L"%d", (*gpItem)[0]->data.gun.bGunStatus );
	AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 380, 25, 15, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	swprintf( str, L"%d", (*gpItem)[0]->data.gun.ubGunShotsLeft );
	AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 400, 25, 15, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	swprintf( str, L"%d", gpItem->bTrap );
	AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 420, 25, 15, MSYS_PRIORITY_NORMAL, str, 2, INPUTTYPE_NUMERICSTRICT );
	if( gpEditingItemPool )
	{
		swprintf( str, L"%d", 100 - gWorldItems[ gpEditingItemPool->iItemIndex ].ubNonExistChance );
		AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 440, 25, 15, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	}
	//Attachments are a dynamic part of guns.	None, some, or all attachments could be available
	//for a particular weapon.	Show only the ones that we can apply to this gun.
	yp = 2 * iScreenHeightOffset + 383;
	guiAttachmentButton[ SILENCER_ATTACHMENT_BUTTON ] = -1;
	if( ValidAttachment( SILENCER, gpItem->usItem ) )
	{
		guiAttachmentButton[ SILENCER_ATTACHMENT_BUTTON ] = 
			CreateTextButton( L"SILENCER", SMALLCOMPFONT, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
			iScreenWidthOffset + 570, yp, 60, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, ToggleAttachment );
		yp += 14;
		if( FindAttachment( gpItem, SILENCER ) )
		{
			ButtonList[ guiAttachmentButton[ SILENCER_ATTACHMENT_BUTTON ] ]->uiFlags |= BUTTON_CLICKED_ON;
			gfAttachment[0] = TRUE;
		}
	}
	guiAttachmentButton[ SNIPERSCOPE_ATTACHMENT_BUTTON ] = -1;
	if( ValidAttachment( SNIPERSCOPE, gpItem->usItem ) )
	{
		guiAttachmentButton[ SNIPERSCOPE_ATTACHMENT_BUTTON ] = 
			CreateTextButton( L"SNIPERSCOPE", SMALLCOMPFONT, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
			iScreenWidthOffset + 570, yp, 60, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, ToggleAttachment );
		yp += 14;
		if( FindAttachment( gpItem, SNIPERSCOPE ) )
		{
			ButtonList[ guiAttachmentButton[ SNIPERSCOPE_ATTACHMENT_BUTTON ] ]->uiFlags |= BUTTON_CLICKED_ON;
			gfAttachment[1] = TRUE;
		}
	}
	guiAttachmentButton[ LASERSCOPE_ATTACHMENT_BUTTON ] = -1;
	if( ValidAttachment( LASERSCOPE, gpItem->usItem ) )
	{
		guiAttachmentButton[ LASERSCOPE_ATTACHMENT_BUTTON ] = 
			CreateTextButton( L"LASERSCOPE", SMALLCOMPFONT, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
			iScreenWidthOffset + 570, yp, 60, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, ToggleAttachment );
		yp += 14;
		if( FindAttachment( gpItem, LASERSCOPE ) )
		{
			ButtonList[ guiAttachmentButton[ LASERSCOPE_ATTACHMENT_BUTTON ] ]->uiFlags |= BUTTON_CLICKED_ON;
			gfAttachment[2] = TRUE;
		}
	}
	guiAttachmentButton[ BIPOD_ATTACHMENT_BUTTON ] = -1;
	if( ValidAttachment( BIPOD, gpItem->usItem ) )
	{
		guiAttachmentButton[ BIPOD_ATTACHMENT_BUTTON ] = 
			CreateTextButton( L"BIPOD", SMALLCOMPFONT, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
			iScreenWidthOffset + 570, yp, 60, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, ToggleAttachment );
		yp += 14;
		if( FindAttachment( gpItem, BIPOD ) )
		{
			ButtonList[ guiAttachmentButton[ BIPOD_ATTACHMENT_BUTTON ] ]->uiFlags |= BUTTON_CLICKED_ON;
			gfAttachment[3] = TRUE;
		}
	}
	guiAttachmentButton[ DUCKBILL_ATTACHMENT_BUTTON ] = -1;
	if( ValidAttachment( DUCKBILL, gpItem->usItem ) )
	{
		guiAttachmentButton[ DUCKBILL_ATTACHMENT_BUTTON ] = 
			CreateTextButton( L"DUCKBILL", SMALLCOMPFONT, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
			iScreenWidthOffset + 570, yp, 60, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, ToggleAttachment );
		yp += 14;
		if( FindAttachment( gpItem, DUCKBILL ) )
		{
			ButtonList[ guiAttachmentButton[ DUCKBILL_ATTACHMENT_BUTTON ] ]->uiFlags |= BUTTON_CLICKED_ON;
			gfAttachment[4] = TRUE;
		}
	}
	guiAttachmentButton[ GLAUNCHER_ATTACHMENT_BUTTON ] = -1;
	if( ValidAttachment( UNDER_GLAUNCHER, gpItem->usItem ) )
	{
		guiAttachmentButton[ GLAUNCHER_ATTACHMENT_BUTTON ] = 
			CreateTextButton( L"G-LAUNCHER", SMALLCOMPFONT, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
			iScreenWidthOffset + 570, yp, 60, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, ToggleAttachment );
		yp += 14;
		if( FindAttachment( gpItem, UNDER_GLAUNCHER ) )
		{
			ButtonList[ guiAttachmentButton[ GLAUNCHER_ATTACHMENT_BUTTON ] ]->uiFlags |= BUTTON_CLICKED_ON;
			gfAttachment[5] = TRUE;
		}
	}
	ReEvaluateAttachmentStatii();
}

void RemoveGunGUI()
{
	PERFORMANCE_MARKER
	INT32 i;
	for( i = 0; i < NUM_ATTACHMENT_BUTTONS; i++ )
	{
		if( guiAttachmentButton[ i ] != -1 )
		{
			RemoveButton( guiAttachmentButton[ i ] );
			guiAttachmentButton[ i ] = -1;
		}
	}
}

void ExtractAndUpdateGunGUI()
{
	PERFORMANCE_MARKER
	INT32 i;
	//Update the gun status
	i = GetNumericStrictValueFromField( 1 );
	if( i == -1 )
		i = 20 + Random( 81 );
	else
		i = min( i, 100 );
	(*gpItem)[0]->data.gun.bGunStatus = (INT8)i;
	SetInputFieldStringWithNumericStrictValue( 1, i );
	//Update the ammo
	i = GetNumericStrictValueFromField( 2 );
	if( i == -1 )
		i = Random( 1 + Weapon[ gpItem->usItem ].ubMagSize );
	else
		i = min( i, Weapon[ gpItem->usItem ].ubMagSize );
	(*gpItem)[0]->data.gun.ubGunShotsLeft = (UINT8)i;
	SetInputFieldStringWithNumericStrictValue( 2, i );
	//Update the trap level
	i = GetNumericStrictValueFromField( 3 );
	i = ( i == -1 ) ? 0 : min( i, 20 );
	gpItem->bTrap = (INT8)i;
	SetInputFieldStringWithNumericStrictValue( 3, i );
	if( gpEditingItemPool )
	{
		giDefaultExistChance = GetNumericStrictValueFromField( 4 );
		giDefaultExistChance = ( giDefaultExistChance == -1 ) ? 100 : max( 1, min( giDefaultExistChance, 100 ) );
		gWorldItems[ gpEditingItemPool->iItemIndex ].ubNonExistChance = (UINT8)(100 - giDefaultExistChance );
		SetInputFieldStringWithNumericStrictValue( 4, giDefaultExistChance );
	}
}

void SetupAmmoGUI()
{
	PERFORMANCE_MARKER
	CHAR16 str[20];
	swprintf( str, L"%d", gpItem->ubNumberOfObjects );
	AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 380, 25, 15, MSYS_PRIORITY_NORMAL, str, 1, INPUTTYPE_NUMERICSTRICT );
	swprintf( str, L"%d", gpItem->bTrap );
	AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 400, 25, 15, MSYS_PRIORITY_NORMAL, str, 2, INPUTTYPE_NUMERICSTRICT );
	if( gpEditingItemPool )
	{
		swprintf( str, L"%d", 100 - gWorldItems[ gpEditingItemPool->iItemIndex ].ubNonExistChance );
		AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 440, 25, 15, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	}
}

void RemoveAmmoGUI()
{
	PERFORMANCE_MARKER
	//nothing to remove
}

void ExtractAndUpdateAmmoGUI()
{
	PERFORMANCE_MARKER
	INT32 i;
	//Update the number of clips
	i = GetNumericStrictValueFromField( 1 );
	if( i == -1 )
		i = 1 + Random( Item[ gpItem->usItem ].ubPerPocket );
	else
		i = max( 1, min( i, Item[ gpItem->usItem ].ubPerPocket ) );
	gpItem->ubNumberOfObjects = (UINT8)i;
	SetInputFieldStringWithNumericStrictValue( 1, i );
	CreateItems( gpItem->usItem, 100, gpItem->ubNumberOfObjects, gpItem );
	//Update the trap level
	i = GetNumericStrictValueFromField( 2 );
	i = ( i == -1 ) ? 0 : min( i, 20 );
	gpItem->bTrap = (INT8)i;
	SetInputFieldStringWithNumericStrictValue( 2, i );
	if( gpEditingItemPool )
	{
		giDefaultExistChance = GetNumericStrictValueFromField( 3 );
		giDefaultExistChance = ( giDefaultExistChance == -1 ) ? 100 : max( 1, min( giDefaultExistChance, 100 ) );
		gWorldItems[ gpEditingItemPool->iItemIndex ].ubNonExistChance = (UINT8)(100 - giDefaultExistChance );
		SetInputFieldStringWithNumericStrictValue( 3, giDefaultExistChance );
	}
}

void SetupArmourGUI()
{
	PERFORMANCE_MARKER
	CHAR16 str[20];
	swprintf( str, L"%d", (*gpItem)[0]->data.objectStatus );
	AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 380, 25, 15, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	swprintf( str, L"%d", gpItem->bTrap );
	AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 400, 25, 15, MSYS_PRIORITY_NORMAL, str, 2, INPUTTYPE_NUMERICSTRICT );
	if( gpEditingItemPool )
	{
		swprintf( str, L"%d", 100 - gWorldItems[ gpEditingItemPool->iItemIndex ].ubNonExistChance );
		AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 440, 25, 15, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	}

	guiCeramicPlatesButton = -1;
	if( ValidAttachment( CERAMIC_PLATES, gpItem->usItem ) )
	{
		guiCeramicPlatesButton = 
			CreateTextButton( L"CERAMIC PLATES", SMALLCOMPFONT, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
			iScreenWidthOffset + 558, 2 * iScreenHeightOffset + 375, 72, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, ToggleCeramicPlates );
		if( FindAttachment( gpItem, CERAMIC_PLATES ) )
		{
			ButtonList[ guiCeramicPlatesButton ]->uiFlags |= BUTTON_CLICKED_ON;
			gfCeramicPlates = TRUE;
		}
	}
}

void RemoveArmourGUI()
{
	PERFORMANCE_MARKER
	if( guiCeramicPlatesButton != -1 )
	{
		RemoveButton( guiCeramicPlatesButton );
		guiCeramicPlatesButton = -1;
	}
}

void ExtractAndUpdateArmourGUI()
{
	PERFORMANCE_MARKER
	INT32 i;
	//Update the armour status
	i = GetNumericStrictValueFromField( 1 );
	if( i == -1 )
		i = 20 + Random( 81 );
	else
		i = min( i, 100 );
	(*gpItem)[0]->data.objectStatus = (INT8)i;
	SetInputFieldStringWithNumericStrictValue( 1, i );
	//Update the trap level
	i = GetNumericStrictValueFromField( 2 );
	i = ( i == -1 ) ? 0 : min( i, 20 );
	gpItem->bTrap = (INT8)i;
	SetInputFieldStringWithNumericStrictValue( 2, i );
	if( gpEditingItemPool )
	{
		giDefaultExistChance = GetNumericStrictValueFromField( 3 );
		giDefaultExistChance = ( giDefaultExistChance == -1 ) ? 100 : max( 1, min( giDefaultExistChance, 100 ) );
		gWorldItems[ gpEditingItemPool->iItemIndex ].ubNonExistChance = (UINT8)(100 - giDefaultExistChance );
		SetInputFieldStringWithNumericStrictValue( 3, giDefaultExistChance );
	}
}

void SetupEquipGUI()
{
	PERFORMANCE_MARKER
	CHAR16 str[20];
	swprintf( str, L"%d", (*gpItem)[0]->data.objectStatus );
	AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 380, 25, 15, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	swprintf( str, L"%d", gpItem->bTrap );
	AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 400, 25, 15, MSYS_PRIORITY_NORMAL, str, 2, INPUTTYPE_NUMERICSTRICT );
	if( gpEditingItemPool )
	{
		swprintf( str, L"%d", 100 - gWorldItems[ gpEditingItemPool->iItemIndex ].ubNonExistChance );
		AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 440, 25, 15, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	}
}

void RemoveEquipGUI()
{
	PERFORMANCE_MARKER
	//nothing to remove
}

void ExtractAndUpdateEquipGUI()
{
	PERFORMANCE_MARKER
	INT32 i;
	//Update the equipment status
	i = GetNumericStrictValueFromField( 1 );
	if( i == -1 )
		i = 20 + Random( 81 );
	else
		i = min( i, 100 );
	(*gpItem)[0]->data.objectStatus = (INT8)i;
	SetInputFieldStringWithNumericStrictValue( 1, i );
	//Update the trap level
	i = GetNumericStrictValueFromField( 2 );
	i = ( i == -1 ) ? 0 : min( i, 20 );
	gpItem->bTrap = (INT8)i;
	SetInputFieldStringWithNumericStrictValue( 2, i );
	if( gpEditingItemPool )
	{
		giDefaultExistChance = GetNumericStrictValueFromField( 3 );
		giDefaultExistChance = ( giDefaultExistChance == -1 ) ? 100 : max( 1, min( giDefaultExistChance, 100 ) );
		gWorldItems[ gpEditingItemPool->iItemIndex ].ubNonExistChance = (UINT8)(100 - giDefaultExistChance );
		SetInputFieldStringWithNumericStrictValue( 3, giDefaultExistChance );
	}
}

void SetupExplosivesGUI()
{
	PERFORMANCE_MARKER
	CHAR16 str[20];
	INT16 yp;
	swprintf( str, L"%d", (*gpItem)[0]->data.objectStatus );
	AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 380, 25, 15, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	swprintf( str, L"%d", gpItem->ubNumberOfObjects );
	AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 400, 25, 15, MSYS_PRIORITY_NORMAL, str, 1, INPUTTYPE_NUMERICSTRICT );
	if( Item[ gpItem->usItem ].ubPerPocket == 1 )
	{
		DisableTextField( 2 );
	}
	swprintf( str, L"%d", gpItem->bTrap );
	AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 420, 25, 15, MSYS_PRIORITY_NORMAL, str, 2, INPUTTYPE_NUMERICSTRICT );
	if( gpEditingItemPool )
	{
		swprintf( str, L"%d", 100 - gWorldItems[ gpEditingItemPool->iItemIndex ].ubNonExistChance );
		AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 440, 25, 15, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	}
	yp = 375;
	gfDetonator = FALSE;
	guiDetonatorButton = -1;
	if( ValidAttachment( DETONATOR, gpItem->usItem ) )
	{
		guiDetonatorButton = 
			CreateTextButton( L"DETONATOR", SMALLCOMPFONT, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
			iScreenWidthOffset + 570, yp, 60, 12, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, ToggleDetonator );
		yp += 14;
		if( FindAttachment( gpItem, DETONATOR ) )
		{
			ButtonList[ guiDetonatorButton ]->uiFlags |= BUTTON_CLICKED_ON;
			gfDetonator = TRUE;
		}
	}
}

void RemoveExplosivesGUI()
{
	PERFORMANCE_MARKER
	if( guiDetonatorButton != -1 )
	{
		RemoveButton( guiDetonatorButton );
		guiDetonatorButton = -1;
	}
}

void ExtractAndUpdateExplosivesGUI()
{
	PERFORMANCE_MARKER
	INT32 i;
	//Update the explosives status
	i = GetNumericStrictValueFromField( 1 );
	if( i == -1 )
		i = 20 + Random( 81 );
	else
		i = min( i, 100 );
	(*gpItem)[0]->data.objectStatus = (INT8)i;
	SetInputFieldStringWithNumericStrictValue( 1, i );
	//Update the quantity
	if( Item[ gpItem->usItem ].ubPerPocket > 1 )
	{
		i = GetNumericStrictValueFromField( 2 );
		if( i == -1 )
			i = 1 + Random( Item[ gpItem->usItem ].ubPerPocket );
		else
			i = max( 1, min( i, Item[ gpItem->usItem ].ubPerPocket ) );
		gpItem->ubNumberOfObjects = (UINT8)i;
		SetInputFieldStringWithNumericStrictValue( 2, i );
		CreateItems( gpItem->usItem, (*gpItem)[0]->data.objectStatus, gpItem->ubNumberOfObjects, gpItem );
	}
	//Update the trap level
	i = GetNumericStrictValueFromField( 3 );
	i = ( i == -1 ) ? 0 : min( i, 20 );	
	gpItem->bTrap = (INT8)i;
	SetInputFieldStringWithNumericStrictValue( 3, i );
	if( gpEditingItemPool )
	{
		giDefaultExistChance = GetNumericStrictValueFromField( 4 );
		giDefaultExistChance = ( giDefaultExistChance == -1 ) ? 100 : max( 1, min( giDefaultExistChance, 100 ) );
		gWorldItems[ gpEditingItemPool->iItemIndex ].ubNonExistChance = (UINT8)(100 - giDefaultExistChance );
		SetInputFieldStringWithNumericStrictValue( 4, giDefaultExistChance );
	}
}

void SetupMoneyGUI()
{
	PERFORMANCE_MARKER
	CHAR16 str[20];
	swprintf( str, L"%d", (*gpItem)[0]->data.money.uiMoneyAmount );
	AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 380, 45, 15, MSYS_PRIORITY_NORMAL, str, 5, INPUTTYPE_NUMERICSTRICT );
	if( gpEditingItemPool )
	{
		swprintf( str, L"%d", 100 - gWorldItems[ gpEditingItemPool->iItemIndex ].ubNonExistChance );
		AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 440, 25, 15, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	}
}

void ExtractAndUpdateMoneyGUI()
{
	PERFORMANCE_MARKER
	INT32 i;
	//Update the amount of cash
	i = GetNumericStrictValueFromField( 1 );
	if( i == -1 )
		i = Random( 20000 );
	else
		i = max( 1, min( i, 20000 ) );
	(*gpItem)[0]->data.money.uiMoneyAmount = i;
	(*gpItem)[0]->data.objectStatus = 100;
	SetInputFieldStringWithNumericStrictValue( 1, i );
	if( gpEditingItemPool )
	{
		giDefaultExistChance = GetNumericStrictValueFromField( 2 );
		giDefaultExistChance = ( giDefaultExistChance == -1 ) ? 100 : max( 1, min( giDefaultExistChance, 100 ) );
		gWorldItems[ gpEditingItemPool->iItemIndex ].ubNonExistChance = (UINT8)(100 - giDefaultExistChance );
		SetInputFieldStringWithNumericStrictValue( 2, giDefaultExistChance );
	}
}

void RemoveMoneyGUI()
{
	PERFORMANCE_MARKER

}

void SetupOwnershipGUI()
{
	PERFORMANCE_MARKER
	CHAR16 str[20];
	swprintf( str, L"%d", (*gpItem)[0]->data.owner.ubOwnerProfile );
	AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 380, 25, 15, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	giOwnershipGroupButton = 
		CreateTextButton( gszCivGroupNames[ (*gpItem)[0]->data.owner.ubOwnerCivGroup ], SMALLCOMPFONT, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 415, 80, 25, BUTTON_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, OwnershipGroupButtonCallback );
}

void OwnershipGroupButtonCallback( GUI_BUTTON *btn, INT32 reason )
{
	PERFORMANCE_MARKER
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		InitPopupMenu( btn->IDNum, OWNERSHIPGROUP_POPUP, DIR_UPLEFT );
	}
}

void SetOwnershipGroup( UINT8 ubNewGroup )
{
	PERFORMANCE_MARKER
	(*gpItem)[0]->data.owner.ubOwnerCivGroup = ubNewGroup;
	SpecifyButtonText( giOwnershipGroupButton, gszCivGroupNames[ ubNewGroup ] );
}

void ExtractAndUpdateOwnershipGUI()
{
	PERFORMANCE_MARKER
	INT32 i;
	//Update the amount of cash
	i = GetNumericStrictValueFromField( 1 );
	if( i == -1 )
		i = Random( 0 );
	else
		i = max( 0, min( i, 255 ) );
	(*gpItem)[0]->data.owner.ubOwnerProfile = (UINT8)i;
	SetInputFieldStringWithNumericStrictValue( 1, i );
}

void RemoveOwnershipGUI()
{
	PERFORMANCE_MARKER
	if( giOwnershipGroupButton != -1 )
	{
		RemoveButton( giOwnershipGroupButton );
		giOwnershipGroupButton = -1;
	}
}

void SetupKeysGUI()
{
	PERFORMANCE_MARKER
	CHAR16 str[20];
	if( gpEditingItemPool )
	{
		swprintf( str, L"%d", 100 - gWorldItems[ gpEditingItemPool->iItemIndex ].ubNonExistChance );
		AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 440, 25, 15, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	}
}

void ExtractAndUpdateKeysGUI()
{
	PERFORMANCE_MARKER
	if( gpEditingItemPool )
	{
		giDefaultExistChance = GetNumericStrictValueFromField( 1 );
		giDefaultExistChance = ( giDefaultExistChance == -1 ) ? 100 : max( 1, min( giDefaultExistChance, 100 ) );
		gWorldItems[ gpEditingItemPool->iItemIndex ].ubNonExistChance = (UINT8)(100 - giDefaultExistChance );
		SetInputFieldStringWithNumericStrictValue( 1, giDefaultExistChance );
	}
}

void RemoveKeysGUI()
{
	PERFORMANCE_MARKER

}

void SetupActionItemsGUI()
{
	PERFORMANCE_MARKER
	CHAR16 str[4];
	STR16 pStr;
	swprintf( str, L"%d", (*gpItem)[0]->data.objectStatus );
	AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 365, 25, 15, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	swprintf( str, L"%d", gpItem->bTrap );
	AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 385, 25, 15, MSYS_PRIORITY_NORMAL, str, 2, INPUTTYPE_NUMERICSTRICT );
	if( gpEditingItemPool )
	{
		swprintf( str, L"%d", 100 - gWorldItems[ gpEditingItemPool->iItemIndex ].ubNonExistChance );
		AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 440, 25, 15, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	}
	pStr = GetActionItemName( gpItem );
	guiActionItemButton = 
		CreateTextButton( pStr, FONT10ARIAL, FONT_YELLOW, FONT_BLACK, BUTTON_USE_DEFAULT,
		iScreenWidthOffset + 510, 2 * iScreenHeightOffset + 410, 100, 20, BUTTON_NO_TOGGLE, MSYS_PRIORITY_NORMAL, DEFAULT_MOVE_CALLBACK, ActionItemCallback );
}

void ExtractAndUpdateActionItemsGUI()
{
	PERFORMANCE_MARKER
	INT32 i;
	//Update the equipment status
	i = GetNumericStrictValueFromField( 1 );
	if( i == -1 )
		i = 20 + Random( 81 );
	else
		i = min( i, 100 );
	(*gpItem)[0]->data.objectStatus = (INT8)i;
	SetInputFieldStringWithNumericStrictValue( 1, i );
	//Update the trap level
	i = GetNumericStrictValueFromField( 2 );
	i = ( i == -1 ) ? 0 : min( i, 20 );
	if( i != gpItem->bTrap )
		gbDefaultBombTrapLevel = (INT8)i;
	gpItem->bTrap = (INT8)i;
	SetInputFieldStringWithNumericStrictValue( 2, i );

	if( gpEditingItemPool )
	{
		giDefaultExistChance = GetNumericStrictValueFromField( 3 );
		giDefaultExistChance = ( giDefaultExistChance == -1 ) ? 100 : max( 1, min( giDefaultExistChance, 100 ) );
		gWorldItems[ gpEditingItemPool->iItemIndex ].ubNonExistChance = (UINT8)(100 - giDefaultExistChance );
		SetInputFieldStringWithNumericStrictValue( 3, giDefaultExistChance );
	}
}

void RemoveActionItemsGUI()
{
	PERFORMANCE_MARKER
	if( guiActionItemButton != -1 )
	{
		RemoveButton( guiActionItemButton );
		guiActionItemButton = -1;
	}
}

void AlarmTriggerCheckboxCallback( GUI_BUTTON *btn, INT32 reason )
{
	PERFORMANCE_MARKER
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if( btn->uiFlags & BUTTON_CLICKED_ON )
			gpItem->fFlags |= OBJECT_ALARM_TRIGGER;
		else
			gpItem->fFlags &= ~OBJECT_ALARM_TRIGGER;
	}
}

void SetupTriggersGUI()
{
	PERFORMANCE_MARKER
	CHAR16 str[4];
	swprintf( str, L"%d", gpItem->bTrap );
	AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 365, 25, 15, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	swprintf( str, L"%d", (*gpItem)[0]->data.bombs.ubTolerance );
	AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 385, 25, 15, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	if( gpEditingItemPool )
	{
		swprintf( str, L"%d", 100 - gWorldItems[ gpEditingItemPool->iItemIndex ].ubNonExistChance );
		AddTextInputField( iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 440, 25, 15, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
		if( (*gpItem)[0]->data.bombs.bFrequency <= PANIC_FREQUENCY && (*gpItem)[0]->data.bombs.bFrequency >= PANIC_FREQUENCY_3 )
		{
			giAlarmTriggerButton = 
				CreateCheckBoxButton(	iScreenWidthOffset + 485, 2 * iScreenHeightOffset + 405, "EDITOR//smCheckBox.sti", MSYS_PRIORITY_NORMAL, AlarmTriggerCheckboxCallback );
			SetButtonFastHelpText( giAlarmTriggerButton, L"If the panic trigger is an alarm trigger,\nenemies won't attempt to use it if they\nare already aware of your presence.");
			if( gpItem->fFlags & OBJECT_ALARM_TRIGGER )
				ButtonList[ giAlarmTriggerButton ]->uiFlags |= BUTTON_CLICKED_ON;
		}
	}
}

void ExtractAndUpdateTriggersGUI()
{
	PERFORMANCE_MARKER
	INT32 i;
	//Update the trap level
	i = GetNumericStrictValueFromField( 1 );
	i = ( i == -1 ) ? 0 : min( i, 20 );
	gpItem->bTrap = (INT8)i;
	SetInputFieldStringWithNumericStrictValue( 1, i );

	i = GetNumericStrictValueFromField( 2 );
	i = ( i == -1 ) ? 0 : max( 0, min( i, 99 ) );
	(*gpItem)[0]->data.bombs.ubTolerance = (UINT8)i;
	SetInputFieldStringWithNumericStrictValue( 2, i );

	if( gpEditingItemPool )
	{
		giDefaultExistChance = GetNumericStrictValueFromField( 3 );
		giDefaultExistChance = ( giDefaultExistChance == -1 ) ? 100 : max( 1, min( giDefaultExistChance, 100 ) );
		gWorldItems[ gpEditingItemPool->iItemIndex ].ubNonExistChance = (UINT8)(100 - giDefaultExistChance );
		SetInputFieldStringWithNumericStrictValue( 3, giDefaultExistChance );
	}
}

void RemoveTriggersGUI()
{
	PERFORMANCE_MARKER
	if( gpEditingItemPool && (*gpItem)[0]->data.bombs.bFrequency <= PANIC_FREQUENCY && (*gpItem)[0]->data.bombs.bFrequency >= PANIC_FREQUENCY_3 )
	{
		if( giAlarmTriggerButton != -1 )
		{
			RemoveButton( giAlarmTriggerButton );
			giAlarmTriggerButton = -1;
		}
	}
}

void ToggleAttachment( GUI_BUTTON *btn, INT32 reason )
{
	PERFORMANCE_MARKER
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		INT32 i;
		UINT16 usAttachment = 0;
		for( i = 0; i < NUM_ATTACHMENT_BUTTONS; i++ )
		{	//Loop through and find the button that was just modified
			switch( i )
			{
				case 0: usAttachment = SILENCER;				break;
				case 1: usAttachment = SNIPERSCOPE;			break;
				case 2: usAttachment = LASERSCOPE;			break;
				case 3:	usAttachment = BIPOD;						break;
				case 4: usAttachment = DUCKBILL;				break;
				case 5: usAttachment = UNDER_GLAUNCHER;	break;
				default: Assert(0); continue;
			}
			if( guiAttachmentButton[ i ] != -1 && btn == ButtonList[ guiAttachmentButton[ i ] ] )
			{	//Found it, now check the state of the button.
				if( !gfAttachment[ i ] )
				{
					gfAttachment[ i ] = TRUE;
					btn->uiFlags |= BUTTON_CLICKED_ON;
					CreateItem( usAttachment, (*gpItem)[0]->data.gun.bGunStatus, &gTempObject );
					AttachObject( NULL, gpItem, &gTempObject );
				}
				else
				{ //Button is out, so remove the attachment
					gfAttachment[ i ] = FALSE;
					btn->uiFlags &= ~BUTTON_CLICKED_ON;
					OBJECTTYPE* pAttachment = FindAttachment( gpItem, usAttachment );
					if( pAttachment )
						RemoveAttachment( gpItem, pAttachment );
				}
			}
		}
		ReEvaluateAttachmentStatii();
	}
}

void ToggleCeramicPlates( GUI_BUTTON *btn, INT32 reason )
{
	PERFORMANCE_MARKER
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		gfCeramicPlates ^= TRUE;
		if( gfCeramicPlates )
		{
			btn->uiFlags |= BUTTON_CLICKED_ON;
			CreateItem( CERAMIC_PLATES, (*gpItem)[0]->data.objectStatus, &gTempObject );
			AttachObject( NULL, gpItem, &gTempObject );
		}
		else
		{
			btn->uiFlags &= ~BUTTON_CLICKED_ON;
			OBJECTTYPE* pAttachment = FindAttachment( gpItem, CERAMIC_PLATES );
			if( pAttachment )
				RemoveAttachment( gpItem, pAttachment );
		}
	}
}

void ToggleDetonator( GUI_BUTTON *btn, INT32 reason )
{
	PERFORMANCE_MARKER
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if( !gfDetonator )
		{
			gfDetonator = TRUE;
			btn->uiFlags |= BUTTON_CLICKED_ON;
			CreateItem( DETONATOR, (*gpItem)[0]->data.objectStatus, &gTempObject );
			AttachObject( NULL, gpItem, &gTempObject );
		}
		else
		{ //Button is out, so remove the attachment
			gfDetonator = FALSE;
			btn->uiFlags &= ~BUTTON_CLICKED_ON;
			OBJECTTYPE* pAttachment = FindAttachment( gpItem, DETONATOR );
			if( pAttachment )
				RemoveAttachment( gpItem, pAttachment );
		}
	}
}

void ActionItemCallback( GUI_BUTTON *btn, INT32 reason )
{
	PERFORMANCE_MARKER
	if( reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		InitPopupMenu( guiActionItemButton, ACTIONITEM_POPUP, DIR_UPLEFT );
	}
}

void ChangeActionItem( OBJECTTYPE *pItem, INT8 bActionItemIndex )
{
	PERFORMANCE_MARKER
	pItem->usItem = ACTION_ITEM;
	(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_BLOW_UP;
	switch( bActionItemIndex )
	{
		case ACTIONITEM_TRIP_KLAXON:
			(*pItem)[0]->data.bombs.usBombItem = TRIP_KLAXON;
			break;
		case ACTIONITEM_FLARE:
			(*pItem)[0]->data.bombs.usBombItem = TRIP_FLARE;
			break;
		case ACTIONITEM_TEARGAS:
			(*pItem)[0]->data.bombs.usBombItem = TEARGAS_GRENADE;
			break;
		case ACTIONITEM_STUN:
			(*pItem)[0]->data.bombs.usBombItem = STUN_GRENADE;
			break;
		case ACTIONITEM_SMOKE:		
			(*pItem)[0]->data.bombs.usBombItem = SMOKE_GRENADE;
			break;
		case ACTIONITEM_MUSTARD:
			(*pItem)[0]->data.bombs.usBombItem = MUSTARD_GRENADE;
			break;
		case ACTIONITEM_MINE:
			(*pItem)[0]->data.bombs.usBombItem = MINE;
			break;
		case ACTIONITEM_OPEN:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_OPEN_DOOR;
			break;
		case ACTIONITEM_CLOSE:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_CLOSE_DOOR;
			break;
		case ACTIONITEM_UNLOCK_DOOR:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_UNLOCK_DOOR;
			break;
		case ACTIONITEM_TOGGLE_LOCK:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_TOGGLE_LOCK;
			break;
		case ACTIONITEM_UNTRAP_DOOR:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_UNTRAP_DOOR;
			break;
		case ACTIONITEM_TOGGLE_PRESSURE_ITEMS:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_TOGGLE_PRESSURE_ITEMS;
			break;
		case ACTIONITEM_SMPIT:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_SMALL_PIT;
			break;
		case ACTIONITEM_LGPIT:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_LARGE_PIT;
			break;
		case ACTIONITEM_TOGGLE_DOOR:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_TOGGLE_DOOR;
			break;
		case ACTIONITEM_TOGGLE_ACTION1:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_TOGGLE_ACTION1;
			break;
		case ACTIONITEM_TOGGLE_ACTION2:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_TOGGLE_ACTION2;
			break;
		case ACTIONITEM_TOGGLE_ACTION3:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_TOGGLE_ACTION3;
			break;
		case ACTIONITEM_TOGGLE_ACTION4:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_TOGGLE_ACTION4;
			break;
		case ACTIONITEM_ENTER_BROTHEL:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_ENTER_BROTHEL;
			break;
		case ACTIONITEM_EXIT_BROTHEL:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_EXIT_BROTHEL;
			break;
		case ACTIONITEM_KINGPIN_ALARM:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_KINGPIN_ALARM;
			break;
		case ACTIONITEM_SEX:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_SEX;
			break;
		case ACTIONITEM_REVEAL_ROOM:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_REVEAL_ROOM;
			break;
		case ACTIONITEM_LOCAL_ALARM:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_LOCAL_ALARM;
			break;
		case ACTIONITEM_GLOBAL_ALARM:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_GLOBAL_ALARM;
			break;
		case ACTIONITEM_KLAXON:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_KLAXON;
			break;
		case ACTIONITEM_SMALL:
			(*pItem)[0]->data.bombs.usBombItem = HAND_GRENADE;
			break;
		case ACTIONITEM_MEDIUM:
			(*pItem)[0]->data.bombs.usBombItem = TNT;
			break;
		case ACTIONITEM_LARGE:
			(*pItem)[0]->data.bombs.usBombItem = C4;
			break;
		case ACTIONITEM_MUSEUM_ALARM:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_MUSEUM_ALARM;
			break;
		case ACTIONITEM_BLOODCAT_ALARM:
			(*pItem)[0]->data.bombs.usBombItem = NOTHING;
			(*pItem)[0]->data.bombs.bActionValue = ACTION_ITEM_BLOODCAT_ALARM;
			break;
		case ACTIONITEM_BIG_TEAR_GAS:
			(*pItem)[0]->data.bombs.usBombItem = BIG_TEAR_GAS;
			break;

	}
}

void UpdateActionItem( INT8 bActionItemIndex )
{
	PERFORMANCE_MARKER
	gbActionItemIndex = bActionItemIndex; //used for future new actionitems as the default.

	if( !gpItemPool || !gpItem )
		return;

	//If the previous item was a pit, remove it before changing it
	if( gpItem->usItem == ACTION_ITEM )
	{
		if( (*gpItem)[0]->data.bombs.bActionValue == ACTION_ITEM_SMALL_PIT )
			Remove3X3Pit( gWorldItems[ gpItemPool->iItemIndex ].sGridNo );
		else if( (*gpItem)[0]->data.bombs.bActionValue == ACTION_ITEM_LARGE_PIT )
			Remove5X5Pit( gWorldItems[ gpItemPool->iItemIndex ].sGridNo );
	}

	ChangeActionItem( gpItem, gbActionItemIndex );
	SpecifyButtonText( guiActionItemButton,	GetActionItemName( gpItem ) );

	//If the new item is a pit, add it so we can see how it looks.
	if( gpItem->usItem == ACTION_ITEM )
	{
		if( (*gpItem)[0]->data.bombs.bActionValue == ACTION_ITEM_SMALL_PIT )
			Add3X3Pit( gWorldItems[ gpItemPool->iItemIndex ].sGridNo );
		else if( (*gpItem)[0]->data.bombs.bActionValue == ACTION_ITEM_LARGE_PIT )
			Add5X5Pit( gWorldItems[ gpItemPool->iItemIndex ].sGridNo );
	}
}

void ReEvaluateAttachmentStatii()
{
	PERFORMANCE_MARKER
	INT32 i;
	UINT16 usAttachment = 0;
	for( i = 0; i < NUM_ATTACHMENT_BUTTONS; i++ )
	{
		if( guiAttachmentButton[ i ] != -1 && !( ButtonList[ guiAttachmentButton[ i ] ]->uiFlags & BUTTON_CLICKED_ON ) )
		{ //if button exists and button isn't clicked
			switch( i )
			{
				case 0: usAttachment = SILENCER;				break;
				case 1: usAttachment = SNIPERSCOPE;			break;
				case 2: usAttachment = LASERSCOPE;			break;
				case 3:	usAttachment = BIPOD;						break;
				case 4: usAttachment = DUCKBILL;				break;
				case 5: usAttachment = UNDER_GLAUNCHER;	break;
				default: Assert(0); continue;
			}
			if( ValidItemAttachment( gpItem, usAttachment, TRUE ) )
				EnableButton( guiAttachmentButton[ i ] );
			else
				DisableButton( guiAttachmentButton[ i ] );
		}
	}
}


#endif