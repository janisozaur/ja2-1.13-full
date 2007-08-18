#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "items.h"
	#include "handle Items.h"
	#include "overhead.h"
	#include "weapons.h"
	#include "points.h"
	#include "tiledef.h"
	#include "worlddef.h"
	#include "worldman.h"
	#include "interface.h"
	#include "renderworld.h"
	#include "Animation Control.h"
	#include "font control.h"
	#include "render dirty.h"
	#include "World items.h"
	#include "debug.h"
	#include "Isometric utils.h"
	#include "sys globals.h"
	#include "Tactical Save.h"
	#include "strategicmap.h"
	#include "Campaign Types.h"
	#include "random.h"
	#include "Action Items.h"
	#include "pits.h"
	#include "GameSettings.h"
	#include "Quests.h"
	#include "Soldier Profile.h"
	#include "message.h"
#endif

//Global dynamic array of all of the items in a loaded map.
WORLDITEM *		gWorldItems = NULL;
UINT32				guiNumWorldItems = 0;

WORLDBOMB *		gWorldBombs = NULL;
UINT32				guiNumWorldBombs = 0;

void DeleteWorldItemsBelongingToTerroristsWhoAreNotThere( void );
void DeleteWorldItemsBelongingToQueenIfThere( void );

extern UINT16 StandardGunListAmmoReplacement( UINT16 usAmmo );
extern UINT8 GetDealerItemCategoryNumber( UINT16 usItemIndex );

bool WORLDITEM::operator<(const WORLDITEM& compare)
{
	PERFORMANCE_MARKER
	if ( this->fExists == false ) {
		return false;
	}
	if ( compare.fExists == false ) {
		return true;
	}


	UINT8		ubItem1Category = GetDealerItemCategoryNumber( this->object.usItem );
	UINT8		ubItem2Category = GetDealerItemCategoryNumber( compare.object.usItem );
	// lower category first
	if ( ubItem1Category < ubItem2Category ) {
		return( true );
	}
	else if ( ubItem1Category > ubItem2Category ) {
		return false;
	}


	// the same category 
	//Madd: sort by name (for now at least):
	int retVal = _stricmp(Item[this->object.usItem].szBRName,Item[compare.object.usItem].szBRName);
	if ( retVal < 0 ) {
		return true;
	}
	else if ( retVal > 0 ) {
		return false;
	}


	if (this->object.ubNumberOfObjects > compare.object.ubNumberOfObjects) {
		return true;
	}
	else if (this->object.ubNumberOfObjects < compare.object.ubNumberOfObjects) {
		return false;
	}

/*
	if (this->object[0]->attachments.size() > compare.object[0]->attachments.size()) {
		return true;
	}
	else if (this->object[0]->attachments.size() < compare.object[0]->attachments.size()) {
		return false;
	}
*/

	// higher quality first
	if ( this->object[0]->data.objectStatus > compare.object[0]->data.objectStatus ) {
		return( true );
	}
	else {
		// identical items!
		return( false );
	}
	return false;
}

void WORLDITEM::initialize()
{
	PERFORMANCE_MARKER
	this->fExists = 0;
	this->sGridNo = 0;
	this->ubLevel = 0;
	this->usFlags = 0;
	this->bRenderZHeightAboveLevel = 0;
	this->bVisible = 0;
	this->ubNonExistChance = 0;
	this->object.initialize();
}

WORLDITEM& WORLDITEM::operator=(OLD_WORLDITEM_101& src)
{
	PERFORMANCE_MARKER
	//the first conversion is simple enough that it can be done here
	this->fExists = src.fExists;
	this->sGridNo = src.sGridNo;
	this->ubLevel = src.ubLevel;
	this->usFlags = src.usFlags;
	this->bRenderZHeightAboveLevel = src.bRenderZHeightAboveLevel;
	this->bVisible = src.bVisible;
	this->ubNonExistChance = src.ubNonExistChance;

	//convert the OBJECTTYPE
	this->object = src.oldObject;
	return *this;
}

WORLDITEM& WORLDITEM::operator=(const WORLDITEM& src)
{
	PERFORMANCE_MARKER
	this->fExists = src.fExists;
	this->sGridNo = src.sGridNo;
	this->ubLevel = src.ubLevel;
	this->usFlags = src.usFlags;
	this->bRenderZHeightAboveLevel = src.bRenderZHeightAboveLevel;
	this->bVisible = src.bVisible;
	this->ubNonExistChance = src.ubNonExistChance;
	this->object = src.object;
	return *this;
}


INT32 GetFreeWorldBombIndex( void )
{
	PERFORMANCE_MARKER
	UINT32 uiCount;
	WORLDBOMB *newWorldBombs;
	UINT32	uiOldNumWorldBombs;

	for(uiCount=0; uiCount < guiNumWorldBombs; uiCount++)
	{
		if ( gWorldBombs[ uiCount ].fExists == FALSE )
			return( (INT32)uiCount );
	}

	uiOldNumWorldBombs = guiNumWorldBombs;
	guiNumWorldBombs += 10;
	//Allocate new table with max+10 items.
	newWorldBombs = (WORLDBOMB*)MemRealloc( gWorldBombs, sizeof( WORLDBOMB ) * guiNumWorldBombs );
	if (newWorldBombs == NULL)
	{
		return( -1 );
	}

	//Clear the rest of the new array
	memset( &newWorldBombs[ uiOldNumWorldBombs ], 0, 
		sizeof( WORLDBOMB ) * ( guiNumWorldBombs - uiOldNumWorldBombs ) );
	gWorldBombs = newWorldBombs;

	// Return uiCount.....
	return( uiCount );
}


UINT32 GetNumUsedWorldBombs( void )
{
	PERFORMANCE_MARKER
	UINT32 uiCount, uiNumItems;
	uiNumItems = 0;

	if ( guiNumWorldBombs == 0 )
	{
		return( 0 );
	}

	for( uiCount = 0; uiCount < guiNumWorldBombs; uiCount++ )
	{
		if( gWorldBombs[ uiCount ].fExists )
		{
			uiNumItems++;
		}
	}

	return( uiNumItems );
}



INT32 AddBombToWorld( INT32 iItemIndex )
{
	PERFORMANCE_MARKER
	UINT32	iBombIndex;

	iBombIndex = GetFreeWorldBombIndex( );

	//Add the new world item to the table.
	gWorldBombs[ iBombIndex ].fExists										= TRUE;
	gWorldBombs[ iBombIndex ].iItemIndex								= iItemIndex;

	return ( iBombIndex );
}

void RemoveBombFromWorld( INT32 iBombIndex )
{
	PERFORMANCE_MARKER
	//Remove the world bomb from the table.
	gWorldBombs[ iBombIndex ].fExists										= FALSE;
}

void RemoveBombFromWorldByItemIndex( INT32 iItemIndex )
{
	PERFORMANCE_MARKER
	// Find the world bomb which corresponds with a particular world item, then
	// remove the world bomb from the table.
	UINT32	uiBombIndex;

	for (uiBombIndex = 0; uiBombIndex < guiNumWorldBombs; uiBombIndex++)
	{
		if (gWorldBombs[ uiBombIndex ].fExists && gWorldBombs[ uiBombIndex].iItemIndex == iItemIndex )
		{
			RemoveBombFromWorld( uiBombIndex );
			return;
		}
	}
}

INT32 FindWorldItemForBombInGridNo( INT16 sGridNo, INT8 bLevel )
{
	PERFORMANCE_MARKER
	UINT32					uiBombIndex;

	for (uiBombIndex = 0; uiBombIndex < guiNumWorldBombs; uiBombIndex++)
	{
		if (gWorldBombs[ uiBombIndex ].fExists && gWorldItems[ gWorldBombs[ uiBombIndex ].iItemIndex ].sGridNo == sGridNo && gWorldItems[ gWorldBombs[ uiBombIndex ].iItemIndex ].ubLevel == bLevel )
		{
			return( gWorldBombs[ uiBombIndex ].iItemIndex );
		}
	}
	return( -1 );
}


void FindPanicBombsAndTriggers( void )
{
	PERFORMANCE_MARKER
	// This function searches the bomb table to find panic-trigger-tuned bombs and triggers

	UINT32			uiBombIndex;
	OBJECTTYPE *	pObj;
	STRUCTURE *		pSwitch;
	INT16			sGridNo = NOWHERE;
	INT8			bPanicIndex;
	for (uiBombIndex = 0; uiBombIndex < guiNumWorldBombs; uiBombIndex++)
	{
		if (gWorldBombs[ uiBombIndex ].fExists)
		{
			pObj = &(gWorldItems[ gWorldBombs[ uiBombIndex ].iItemIndex ].object);
			if ((*pObj)[0]->data.bombs.bFrequency == PANIC_FREQUENCY || (*pObj)[0]->data.bombs.bFrequency == PANIC_FREQUENCY_2 || (*pObj)[0]->data.bombs.bFrequency == PANIC_FREQUENCY_3 )
			{
				if (pObj->usItem == SWITCH)
				{
					sGridNo = gWorldItems[ gWorldBombs[ uiBombIndex ].iItemIndex ].sGridNo;
					switch( (*pObj)[0]->data.bombs.bFrequency )
					{
						case PANIC_FREQUENCY:
							bPanicIndex = 0;
							break;

						case PANIC_FREQUENCY_2:
							bPanicIndex = 1;
							break;

						case PANIC_FREQUENCY_3:
							bPanicIndex = 2;
							break;
						
						default:
							// augh!!!
							continue;
					}

					pSwitch = FindStructure( sGridNo, STRUCTURE_SWITCH );
					if (pSwitch)
					{
						switch( pSwitch->ubWallOrientation ) 
						{
							case INSIDE_TOP_LEFT:
							case OUTSIDE_TOP_LEFT:					
								sGridNo += DirectionInc( SOUTH );
								break;
							case INSIDE_TOP_RIGHT:
							case OUTSIDE_TOP_RIGHT:
								sGridNo += DirectionInc( EAST );
								break;
							default:
								break;
						}
					}

					gTacticalStatus.sPanicTriggerGridNo[ bPanicIndex ] = sGridNo;
					gTacticalStatus.ubPanicTolerance[ bPanicIndex ] = (*pObj)[0]->data.bombs.ubTolerance;
					if ((*pObj)[0]->data.fFlags & OBJECT_ALARM_TRIGGER)
					{
						gTacticalStatus.bPanicTriggerIsAlarm[ bPanicIndex ] = TRUE;
					}
					gTacticalStatus.fPanicFlags |= PANIC_TRIGGERS_HERE;
					bPanicIndex++;
					if (bPanicIndex == NUM_PANIC_TRIGGERS)
					{
						return;
					}
				}
				else
				{
					gTacticalStatus.fPanicFlags |= PANIC_BOMBS_HERE;
				}
			}
		}
	}
}

INT32 GetFreeWorldItemIndex( void )
{
	PERFORMANCE_MARKER
	UINT32 uiCount;
	WORLDITEM *newWorldItems;
	UINT32	uiOldNumWorldItems;

	for(uiCount=0; uiCount < guiNumWorldItems; uiCount++)
	{
		if ( gWorldItems[ uiCount ].fExists == FALSE )
			return( (INT32)uiCount );
	}

	uiOldNumWorldItems = guiNumWorldItems;
	guiNumWorldItems += 10;
	//Allocate new table with max+10 items.
	newWorldItems = new WORLDITEM [ guiNumWorldItems ];
	if (newWorldItems == NULL)
	{
		return( -1 );
	}

	if (gWorldItems)
	{
		for (unsigned int x = 0; x < uiOldNumWorldItems; ++x)
		{
			newWorldItems[x] = gWorldItems[x];
		}
		delete[] gWorldItems;
	}
	gWorldItems = newWorldItems;

	// Return uiCount.....
	return( uiCount );
}


UINT32 GetNumUsedWorldItems( void )
{
	PERFORMANCE_MARKER
	UINT32 uiCount, uiNumItems;
	uiNumItems = 0;

	for( uiCount = 0; uiCount < guiNumWorldItems; uiCount++ )
	{
		if( gWorldItems[ uiCount ].fExists )
		{
			uiNumItems++;
		}
	}

	return( uiNumItems );
}



INT32 AddItemToWorld( INT16 sGridNo, OBJECTTYPE *pObject, UINT8 ubLevel, UINT16 usFlags, INT8 bRenderZHeightAboveLevel, INT8 bVisible )
{
	PERFORMANCE_MARKER
	UINT32	iItemIndex;
	INT32		iReturn;

	// ATE: Check if the gridno is OK
	if ( (sGridNo) == NOWHERE )
	{
		// Display warning.....
		#ifdef JA2BETAVERSION
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Error: Item %d was given invalid grid location %d. Please report", pObject->usItem, sGridNo);
		#endif
		return( -1 );
	}

	iItemIndex = GetFreeWorldItemIndex( );

	//Add the new world item to the table.
	gWorldItems[ iItemIndex ].fExists										= TRUE;
	gWorldItems[ iItemIndex ].sGridNo										= sGridNo;
	gWorldItems[ iItemIndex ].ubLevel										= ubLevel;
	gWorldItems[ iItemIndex ].usFlags										= usFlags;
	gWorldItems[ iItemIndex ].bVisible									= bVisible;
	gWorldItems[ iItemIndex ].bRenderZHeightAboveLevel	= bRenderZHeightAboveLevel;

	gWorldItems[ iItemIndex ].object = *pObject;

	// Add a bomb reference if needed
	if (usFlags & WORLD_ITEM_ARMED_BOMB)
	{
		iReturn = AddBombToWorld( iItemIndex );
		if (iReturn == -1)
		{
			return( -1 );
		}
	}

	return ( iItemIndex );
}

void RemoveItemFromWorld( INT32 iItemIndex )
{
	PERFORMANCE_MARKER
	// Ensure the item still exists, then if it's a bomb,
	// remove the appropriate entry from the bomb table
	if (gWorldItems[ iItemIndex ].fExists)
	{
		if (gWorldItems[ iItemIndex ].usFlags & WORLD_ITEM_ARMED_BOMB)
		{
			RemoveBombFromWorldByItemIndex( iItemIndex );
		}
		gWorldItems[ iItemIndex ].fExists = FALSE;
	}
}

void TrashWorldItems()
{
	PERFORMANCE_MARKER
	UINT32 i;
	if( gWorldItems )
	{
		for( i = 0; i < guiNumWorldItems; i++ )
		{
			if( gWorldItems[ i ].fExists )
			{
				RemoveItemFromPool( gWorldItems[ i ].sGridNo, i, gWorldItems[ i ].ubLevel );
			}
		}
		delete[] gWorldItems;
		gWorldItems = NULL;
		guiNumWorldItems = 0;
	}
	if ( gWorldBombs )
	{
		MemFree( gWorldBombs );
		gWorldBombs = NULL;
		guiNumWorldBombs = 0;
	}
}


void SaveWorldItemsToMap( HWFILE fp )
{
	PERFORMANCE_MARKER
	UINT32 i, uiBytesWritten;
	UINT32		uiActualNumWorldItems;


	uiActualNumWorldItems = GetNumUsedWorldItems( );

	FileWrite( fp, &uiActualNumWorldItems, 4, &uiBytesWritten );

	for( i = 0; i < guiNumWorldItems; i++ )
	{
		if( gWorldItems[ i ].fExists )
			gWorldItems[ i ].Save(fp, TRUE);
	}
}


void LoadWorldItemsFromMap( INT8 **hBuffer, float dMajorMapVersion, int ubMinorMapVersion )
{
	PERFORMANCE_MARKER
	// Start loading itmes...
	
	UINT32			i;
	WORLDITEM		dummyItem;
	INT32				iItemIndex;
	UINT32			uiNumWorldItems;
	//If any world items exist, we must delete them now.
	TrashWorldItems();

	//Read the number of items that were saved in the map.
	LOADDATA( &uiNumWorldItems, *hBuffer, 4 );

	if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME && !gfEditMode )
	{ //The sector has already been visited.	The items are saved in a different format that will be 
		//loaded later on.	So, all we need to do is skip the data entirely.
		if (dMajorMapVersion >= 6.0 && ubMinorMapVersion > 26 ) {
			for (unsigned int x = 0; x < uiNumWorldItems; ++x)
			{
				//ADB WORLDITEM's size on disk is unknown
				dummyItem.Load(hBuffer, dMajorMapVersion, ubMinorMapVersion);
			}
		}
		else {
			*hBuffer += sizeof ( OLD_WORLDITEM_101 ) * uiNumWorldItems;
		}
		return;
	}
	else for ( i = 0; i < uiNumWorldItems; i++ )
	{	//Add all of the items to the world indirectly through AddItemToPool, but only if the chance
		//associated with them succeed.
		dummyItem.Load(hBuffer, dMajorMapVersion, ubMinorMapVersion);
		if( dummyItem.object.usItem == OWNERSHIP )
		{
			dummyItem.ubNonExistChance = 0;
		}
		if( gfEditMode || dummyItem.ubNonExistChance <= PreRandom( 100 ) )
		{
			if( !gfEditMode )
			{ 
				//check for matching item existance modes and only add if there is a match!
				//if we are in platinum mode, REALISTIC items are allowed, but not SCIFI items
				if( dummyItem.usFlags & WORLD_ITEM_SCIFI_ONLY && !(gGameOptions.ubGameStyle == STYLE_SCIFI) ||
					dummyItem.usFlags & WORLD_ITEM_REALISTIC_ONLY && (gGameOptions.ubGameStyle == STYLE_SCIFI) )
				{ //no match, so don't add item to world
					continue;
				}
/*
				if ( !gGameOptions.fGunNut )
				{
					UINT16	usReplacement;

					// do replacements?
					if ( Item[ dummyItem.object.usItem ].usItemClass == IC_GUN )
					{
						INT8		bAmmo, bNewAmmo;

						usReplacement = StandardGunListReplacement( dummyItem.object.usItem );
						if ( usReplacement )
						{
							// everything else can be the same? no.
							bAmmo = dummyItem.object[0]->data.gun.ubGunShotsLeft;
							bNewAmmo = (Weapon[ usReplacement ].ubMagSize * bAmmo) / Weapon[ dummyItem.object.usItem ].ubMagSize;
							if ( bAmmo > 0 && bNewAmmo == 0 )
							{
								bNewAmmo = 1;
							}

							dummyItem.object.usItem = usReplacement;
							dummyItem.object[0]->data.gun.ubGunShotsLeft = bNewAmmo;
						}
					}
					if ( Item[ dummyItem.object.usItem ].usItemClass == IC_AMMO )
					{
						usReplacement = StandardGunListAmmoReplacement( dummyItem.object.usItem );
						if ( usReplacement )
						{
							UINT8		ubLoop;

							// go through status values and scale up/down
							for ( ubLoop = 0; ubLoop < dummyItem.object.ubNumberOfObjects; ubLoop++ )
							{
								dummyItem.object.status.bStatus[ ubLoop ] = dummyItem.object.status.bStatus[ ubLoop ] * Magazine[ Item[ usReplacement ].ubClassIndex ].ubMagSize / Magazine[ Item[ dummyItem.object.usItem ].ubClassIndex ].ubMagSize; 
							}

							// then replace item #
							dummyItem.object.usItem = usReplacement;
						}
					}
				}

*/
			}
			if( dummyItem.object.usItem == ACTION_ITEM && gfLoadPitsWithoutArming )
			{ //if we are loading a pit, they are typically loaded without being armed.
				if( dummyItem.object[0]->data.bombs.bActionValue == ACTION_ITEM_SMALL_PIT || dummyItem.object[0]->data.bombs.bActionValue == ACTION_ITEM_LARGE_PIT )
				{
					dummyItem.usFlags &= ~WORLD_ITEM_ARMED_BOMB;
					dummyItem.bVisible = BURIED;
					dummyItem.object[0]->data.bombs.bDetonatorType = 0;
				}
			}
			
			else if ( dummyItem.bVisible == HIDDEN_ITEM && dummyItem.object.bTrap > 0 && ( Item[dummyItem.object.usItem].mine || dummyItem.object.usItem == TRIP_FLARE || dummyItem.object.usItem == TRIP_KLAXON) )
			{
				ArmBomb( &dummyItem.object, BOMB_PRESSURE );
				dummyItem.usFlags |= WORLD_ITEM_ARMED_BOMB;
				// this is coming from the map so the enemy must know about it.
				gpWorldLevelData[ dummyItem.sGridNo ].uiFlags |= MAPELEMENT_ENEMY_MINE_PRESENT;

			}

			if ( dummyItem.usFlags & WORLD_ITEM_ARMED_BOMB )
			{ //all armed bombs are buried
				dummyItem.bVisible = BURIED;
			}
			AddItemToPoolAndGetIndex( dummyItem.sGridNo, &dummyItem.object, dummyItem.bVisible, dummyItem.ubLevel, dummyItem.usFlags, dummyItem.bRenderZHeightAboveLevel, &iItemIndex );
			gWorldItems[ iItemIndex ].ubNonExistChance = dummyItem.ubNonExistChance;
		}
	}

	if ( !gfEditMode )
	{	
		DeleteWorldItemsBelongingToTerroristsWhoAreNotThere();
		if ( gWorldSectorX == 3 && gWorldSectorY == MAP_ROW_P && gbWorldSectorZ == 1 )
		{
			DeleteWorldItemsBelongingToQueenIfThere();
		}
	}
}

void DeleteWorldItemsBelongingToTerroristsWhoAreNotThere( void )
{
	PERFORMANCE_MARKER
	UINT32	uiLoop;
	UINT32	uiLoop2;
	INT16		sGridNo;
	UINT8		ubLevel;

	// only do this after Carmen has talked to player and terrorists have been placed
	//if ( CheckFact( FACT_CARMEN_EXPLAINED_DEAL, 0 ) == TRUE )
	{

		for ( uiLoop = 0; uiLoop < guiNumWorldItems; uiLoop++ )
		{
			// loop through all items, look for ownership
			if ( gWorldItems[ uiLoop ].fExists && gWorldItems[ uiLoop ].object.usItem == OWNERSHIP )
			{
				// if owner is a terrorist
				if ( IsProfileATerrorist( gWorldItems[ uiLoop ].object[0]->data.owner.ubOwnerProfile ) )
				{
					// and they were not set in the current sector
					if ( gMercProfiles[ gWorldItems[ uiLoop ].object[0]->data.owner.ubOwnerProfile ].sSectorX != gWorldSectorX ||
						gMercProfiles[ gWorldItems[ uiLoop ].object[0]->data.owner.ubOwnerProfile ].sSectorY != gWorldSectorY )
					{
						// then all items in this location should be deleted
						sGridNo = gWorldItems[ uiLoop ].sGridNo;
						ubLevel = gWorldItems[ uiLoop ].ubLevel;
						for ( uiLoop2 = 0; uiLoop2 < guiNumWorldItems; uiLoop2++ )
						{
							// loop through all items, look for ownership
							if ( gWorldItems[ uiLoop2 ].fExists && gWorldItems[ uiLoop2 ].sGridNo == sGridNo && gWorldItems[ uiLoop2 ].ubLevel == ubLevel )
							{
								RemoveItemFromPool( sGridNo, uiLoop2, ubLevel );
							}
						}
					}
				}
			}
		}
	}
	// else the terrorists haven't been placed yet!
}

void DeleteWorldItemsBelongingToQueenIfThere( void )
{
	PERFORMANCE_MARKER
	UINT32	uiLoop;
	UINT32	uiLoop2;
	INT16		sGridNo;
	UINT8		ubLevel;
	INT8		bSlot;

	if ( gMercProfiles[ QUEEN ].sSectorX == gWorldSectorX &&
		gMercProfiles[ QUEEN ].sSectorY == gWorldSectorY && 
		gMercProfiles[ QUEEN ].bSectorZ == gbWorldSectorZ )
	{

		for ( uiLoop = 0; uiLoop < guiNumWorldItems; uiLoop++ )
		{
			// loop through all items, look for ownership
			if ( gWorldItems[ uiLoop ].fExists && gWorldItems[ uiLoop ].object.usItem == OWNERSHIP )
			{
				// if owner is the Queen
				if ( gWorldItems[ uiLoop ].object[0]->data.owner.ubOwnerProfile == QUEEN )
				{
					// then all items in this location should be deleted
					sGridNo = gWorldItems[ uiLoop ].sGridNo;
					ubLevel = gWorldItems[ uiLoop ].ubLevel;
					for ( uiLoop2 = 0; uiLoop2 < guiNumWorldItems; uiLoop2++ )
					{
						// loop through all items, look for those in same tile
						if ( gWorldItems[ uiLoop2 ].fExists && gWorldItems[ uiLoop2 ].sGridNo == sGridNo && gWorldItems[ uiLoop2 ].ubLevel == ubLevel )
						{
							// upgrade equipment
							switch ( gWorldItems[ uiLoop2 ].object.usItem )
							{
								case AUTO_ROCKET_RIFLE:
									bSlot = FindObjectInSoldierProfile( QUEEN, ROCKET_RIFLE );
									if ( bSlot != NO_SLOT )
									{
										// give her auto rifle
										gMercProfiles[ QUEEN ].inv[ bSlot ] = AUTO_ROCKET_RIFLE;
									}
									break;
								case SPECTRA_HELMET_18:
									gMercProfiles[ QUEEN ].inv[ HELMETPOS ] = SPECTRA_HELMET_18;
									break;
								case SPECTRA_VEST_18:
									gMercProfiles[ QUEEN ].inv[ VESTPOS ] = SPECTRA_VEST_18;
									break;
								case SPECTRA_LEGGINGS_18:
									gMercProfiles[ QUEEN ].inv[ LEGPOS ] = SPECTRA_LEGGINGS_18;
									break;
								default:
									break;
							}
							RemoveItemFromPool( sGridNo, uiLoop2, ubLevel );
						}
					}
				}
			}
		}
	}
}


// Refresh item pools
void RefreshWorldItemsIntoItemPools( WORLDITEM * pItemList, INT32 iNumberOfItems )
{
	PERFORMANCE_MARKER
	INT32			i;

	for ( i = 0; i < iNumberOfItems; i++ )
	{	
		if( pItemList[ i ].fExists )
		{
			WORLDITEM& dummyItem = pItemList[ i ];

			AddItemToPool( dummyItem.sGridNo, &dummyItem.object, dummyItem.bVisible, dummyItem.ubLevel, dummyItem.usFlags, dummyItem.bRenderZHeightAboveLevel );
		}
	}

}