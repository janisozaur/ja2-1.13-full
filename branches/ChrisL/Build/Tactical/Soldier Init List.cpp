#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "builddefines.h"
	#include <stdio.h>
	#include <memory.h>

	#include "types.h"

	#include "strategicmap.h"
	#include "overhead.h"
	#include "isometric utils.h"

	#include "soldier add.h"
	#include "soldier create.h"
	#include "Soldier Init List.h"
	#include "debug.h"
	#include "Random.h"
	#include "items.h"

	#include "cursors.h"

	#include "Map Information.h"
	#include "soldier profile.h"
	#include "Sys Globals.h"
	#include "EditorMercs.h"
	#include "Animation Data.h"
	#include "message.h"
	#include "Font Control.h"
	#include "Sound Control.h"
	#include "Quests.h"
	#include "Render Fun.h"
	#include "meanwhile.h"
	#include "Map Screen Interface Map.h"
	#include "mapscreen.h"
	#include "Debug Control.h"
	#include "Inventory Choosing.h"
	#include "Strategic AI.h"
	#include "strategic.h"
	#include "Campaign Types.h"
	#include "ai.h"
	#include "NPC.h"
	#include "Scheduling.h"
	#include "opplist.h"
	#include "MessageBoxScreen.h"
	#include "screenids.h"
	#include "SaveLoadScreen.h"
	#include "Rotting Corpses.h"
#endif

#include "Map Edgepoints.h"
BOOLEAN gfOriginalList = TRUE;

SOLDIERINITNODE *gSoldierInitHead = NULL;
SOLDIERINITNODE *gSoldierInitTail = NULL;

SOLDIERINITNODE *gOriginalSoldierInitListHead = NULL;
SOLDIERINITNODE *gAlternateSoldierInitListHead = NULL;

#ifdef JA2BETAVERSION
	BOOLEAN ValidateSoldierInitLinks( UINT8 ubCode );
	BOOLEAN gfDoDialogOnceGameScreenFadesIn = FALSE;
#endif

UINT32 CountNumberOfNodesWithSoldiers()
{
	PERFORMANCE_MARKER
	SOLDIERINITNODE *curr;
	UINT32 num = 0;
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->pSoldier )
		{
			num++;
		}
		curr = curr->next;
	}
	return num;
}

void SortSoldierInitList();

void InitSoldierInitList()
{
	PERFORMANCE_MARKER
	if( gSoldierInitHead )
	KillSoldierInitList();
	gSoldierInitHead = NULL;
	gSoldierInitTail = NULL;
}

void KillSoldierInitList()
{
	PERFORMANCE_MARKER
	while( gSoldierInitHead )
		RemoveSoldierNodeFromInitList( gSoldierInitTail );
	if( gfOriginalList )
		gOriginalSoldierInitListHead = NULL;
	else
		gAlternateSoldierInitListHead = NULL;
}

SOLDIERINITNODE* AddBasicPlacementToSoldierInitList( BASIC_SOLDIERCREATE_STRUCT *pBasicPlacement )
{
	PERFORMANCE_MARKER
	SOLDIERINITNODE *curr;
	//Allocate memory for node
	curr = (SOLDIERINITNODE*)MemAlloc( sizeof (SOLDIERINITNODE ) );
	Assert( curr );
	memset( curr, 0, sizeof( SOLDIERINITNODE ) );

	//Allocate memory for basic placement
	curr->pBasicPlacement = ( BASIC_SOLDIERCREATE_STRUCT* )MemAlloc( sizeof( BASIC_SOLDIERCREATE_STRUCT ) );
	if( !curr->pBasicPlacement )
	{
		AssertMsg( 0, "Failed to allocate memory for AddBasicPlacementToSoldierInitList." );
		return NULL;
	}

	//Copy memory for basic placement
	memcpy( curr->pBasicPlacement, pBasicPlacement, sizeof( BASIC_SOLDIERCREATE_STRUCT ) );

	//It is impossible to set up detailed placement stuff now.	
	//If there is any detailed placement information during map load, it will be added
	//immediately after this function call.
	curr->pDetailedPlacement = NULL;
	curr->pSoldier = NULL;

	//Insert the new node in the list in its proper place.
	if( !gSoldierInitHead )
	{
		gSoldierInitHead = curr;
		if( gfOriginalList )
			gOriginalSoldierInitListHead = curr;
		else
			gAlternateSoldierInitListHead = curr;
		gSoldierInitTail = curr;
		gSoldierInitHead->next = NULL;
		gSoldierInitHead->prev = NULL;
	}
	else
	{
		//TEMP:	no sorting, just enemies
		curr->prev = gSoldierInitTail;
		curr->next = NULL;
		gSoldierInitTail->next = curr;
		gSoldierInitTail = gSoldierInitTail->next;
	}
	if( gfOriginalList )
		gMapInformation.ubNumIndividuals++;
	return curr;
}

void RemoveSoldierNodeFromInitList( SOLDIERINITNODE *pNode )
{
	PERFORMANCE_MARKER
	if( !pNode )
		return;
	if( gfOriginalList )
		gMapInformation.ubNumIndividuals--;
	if( pNode->pBasicPlacement )
	{
		MemFree( pNode->pBasicPlacement );
		pNode->pBasicPlacement = NULL;
	}
	if( pNode->pDetailedPlacement )
	{
		delete( pNode->pDetailedPlacement );
		pNode->pDetailedPlacement = NULL;
	}
	if( pNode->pSoldier )
	{
		if( pNode->pSoldier->ubID >= 20 )
		{
			TacticalRemoveSoldier( pNode->pSoldier->ubID );
		}
		else
		{
			//bug if gets here and we aren't exiting the game, maybe a bug even if we are exiting (but who cares)
			//DebugBreakpoint();
		}
	}
	if( pNode == gSoldierInitHead )
	{
		gSoldierInitHead = gSoldierInitHead->next;
		if( gSoldierInitHead )
			gSoldierInitHead->prev = NULL;
		if( gfOriginalList )
			gOriginalSoldierInitListHead = gSoldierInitHead;
		else
			gAlternateSoldierInitListHead = gSoldierInitHead;
	}
	else if( pNode == gSoldierInitTail )
	{
		gSoldierInitTail = gSoldierInitTail->prev;
		gSoldierInitTail->next = NULL;
	}
	else
	{
		pNode->prev->next = pNode->next;
		pNode->next->prev = pNode->prev;
	}
	MemFree( pNode );
}

//These serialization functions are assuming the passing of a valid file 
//pointer to the beginning of the save/load area, which is not necessarily at
//the beginning of the file.	This is just a part of the whole map serialization.
BOOLEAN SaveSoldiersToMap( HWFILE fp )
{
	PERFORMANCE_MARKER
	UINT32 i;
	UINT32 uiBytesWritten;
	SOLDIERINITNODE *curr;
	
	if( !fp )
		return FALSE;

	if( gMapInformation.ubNumIndividuals > MAX_INDIVIDUALS )	
		return FALSE;

	//If we are perhaps in the alternate version of the editor, we don't want bad things to
	//happen.	This is probably the only place I know where the user gets punished now.	If the 
	//person was in the alternate editor mode, then decided to save the game, the current mercs may
	//not be there.	This would be bad.	What we do is override any merc editing done while in this
	//mode, and kill them all, while replacing them with the proper ones.	Not only that, the alternate
	//editing mode is turned off, and if intentions are to play the game, the user will be facing many
	//enemies!
	#ifdef JA2EDITOR
		if( !gfOriginalList )
			ResetAllMercPositions();
	#endif
	
	curr = gSoldierInitHead;
	for( i=0; i < gMapInformation.ubNumIndividuals; i++ )
	{
		if( !curr )
			return FALSE;
		curr->ubNodeID = (UINT8)i;
		FileWrite( fp, curr->pBasicPlacement, sizeof( BASIC_SOLDIERCREATE_STRUCT ), &uiBytesWritten );

		if( curr->pBasicPlacement->fDetailedPlacement )
		{
			if( !curr->pDetailedPlacement )
				return FALSE;
			if ( !curr->pDetailedPlacement->Save(fp, TRUE) )
			{
				return FALSE;
			}
		}
		curr = curr->next;
	}
	return TRUE;
}



BOOLEAN LoadSoldiersFromMap( INT8 **hBuffer, float dMajorMapVersion, UINT8 ubMinorMapVersion )
{
	PERFORMANCE_MARKER
	UINT32 i;
	UINT8 ubNumIndividuals;
	BASIC_SOLDIERCREATE_STRUCT tempBasicPlacement;
	SOLDIERINITNODE *pNode;
	BOOLEAN fCowInSector = FALSE;

	ubNumIndividuals = gMapInformation.ubNumIndividuals;

	UseEditorAlternateList();
	KillSoldierInitList();
	UseEditorOriginalList();
	KillSoldierInitList();
	
	InitSoldierInitList();

	if( ubNumIndividuals > MAX_INDIVIDUALS )	
	{
		AssertMsg( 0, "Corrupt map check failed.	ubNumIndividuals is greater than MAX_INDIVIDUALS." );
		return FALSE; //too many mercs
	}
	if( !ubNumIndividuals )	
	{
		return TRUE; //no mercs
	}
	
	//Because we are loading the map, we needed to know how many
	//guys are being loaded, but when we add them to the list here, it
	//automatically increments that number, effectively doubling it, which
	//would be a problem.	Now that we know the number, we clear it here, so
	//it gets built again.
	gMapInformation.ubNumIndividuals = 0;		//MUST BE CLEARED HERE!!!

	SOLDIERCREATE_STRUCT tempDetailedPlacement;
	for( i=0; i < ubNumIndividuals; i++ )
	{
		LOADDATA( &tempBasicPlacement, *hBuffer, sizeof( BASIC_SOLDIERCREATE_STRUCT ) );
		pNode = AddBasicPlacementToSoldierInitList( &tempBasicPlacement );
		pNode->ubNodeID = (UINT8)i;
		if( !pNode )
		{
			AssertMsg( 0, "Failed to allocate memory for new basic placement in LoadSoldiersFromMap." );
			return FALSE;
		}
		if( tempBasicPlacement.fDetailedPlacement )
		{ //Add the static detailed placement information in the same newly created node as the basic placement.
			//read static detailed placement from file
			if ( !tempDetailedPlacement.Load(hBuffer, dMajorMapVersion, ubMinorMapVersion) )
			{
				return FALSE;
			}
			//allocate memory for new static detailed placement
			//copy the file information from temp var to node in list.
			pNode->pDetailedPlacement = new SOLDIERCREATE_STRUCT(tempDetailedPlacement);//(SOLDIERCREATE_STRUCT*)MemAlloc( SIZEOF_SOLDIERCREATE_STRUCT );
			if( !pNode->pDetailedPlacement )
			{
				AssertMsg( 0, "Failed to allocate memory for new detailed placement in LoadSoldiersFromMap." );
				return FALSE;
			}

			if( tempDetailedPlacement.ubProfile != NO_PROFILE )
			{
				pNode->pDetailedPlacement->ubCivilianGroup = gMercProfiles[ tempDetailedPlacement.ubProfile ].ubCivilianGroup;
				pNode->pBasicPlacement->ubCivilianGroup = gMercProfiles[ tempDetailedPlacement.ubProfile ].ubCivilianGroup;
			}
		}
		if( tempBasicPlacement.bBodyType == COW )
		{
			fCowInSector = TRUE;
		}
	}
	if( fCowInSector )
	{
		CHAR8 str[ 40 ];
		sprintf( str, "Sounds\\\\cowmoo%d.wav", Random( 3 ) + 1 );
		PlayJA2SampleFromFile(	str, RATE_11025, MIDVOLUME, 1, MIDDLEPAN );
	}
	return TRUE;
}

//Because soldiers, creatures, etc., maybe added to the game at anytime theoretically, the
//list will need to be sorted to reflect this.	It is quite likely that this won't be needed,
//but the flexibility is there just incase.	Now the list is sorted in the following manner:
//-1st priority:	Any nodes containing valid pointers to soldiers are moved to the end of the list.
//								We don't ever want to use two identical placements.
//-2nd priority:	Any nodes that have priority existance and detailed placement information are
//								put first in the list.
//-3rd priority:	Any nodes that have priority existance and no detailed placement information are used next.
//-4th priority:	Any nodes that have detailed placement and no priority existance information are used next.
//-5th priority:	The rest of the nodes are basic placements and are placed in the center of the list.	Of
//								these, they are randomly filled based on the number needed.
//NOTE:	This function is called by AddSoldierInitListTeamToWorld().	There is no other place it needs to
//			be called.
void SortSoldierInitList()
{
	PERFORMANCE_MARKER
	SOLDIERINITNODE *temp, *curr;

	BOOLEAN	fFredoAtStart = FALSE;
	BOOLEAN	fFredoAtEnd = FALSE;

	if( !gSoldierInitHead )
		return;

	curr = gSoldierInitHead;
	while( curr )
	{
		if ( curr->pDetailedPlacement && curr->pDetailedPlacement->ubProfile == FREDO )
		{
			fFredoAtStart = TRUE;
			break;
		}
		curr = curr->next;
	}

	//1st priority sort 
	curr = gSoldierInitTail;
	while( curr )
	{
		if( curr->pSoldier && curr != gSoldierInitTail )
		{	//This node has an existing soldier, so move to end of list.
			//copy node
			temp = curr;
			if( temp == gSoldierInitHead )
			{ //If we dealing with the head, we need to move it now.
				gSoldierInitHead = gSoldierInitHead->next;
				if( gfOriginalList )
					gOriginalSoldierInitListHead = gSoldierInitHead;
				else
					gAlternateSoldierInitListHead = gSoldierInitHead;
				gSoldierInitHead->prev = NULL;
				temp->next = NULL;
			}
			curr = curr->prev;
			//detach node from list
			if( temp->prev )
				temp->prev->next = temp->next;
			if( temp->next )
				temp->next->prev = temp->prev;
			//add node to end of list
			temp->prev = gSoldierInitTail;
			temp->next = NULL;
			gSoldierInitTail->next = temp;
			gSoldierInitTail = temp;
		}
		else
		{
			curr = curr->prev;
		}
	}
	//4th -- put to start
	curr = gSoldierInitHead;
	while( curr )
	{
		if( !curr->pSoldier && !curr->pBasicPlacement->fPriorityExistance && curr->pDetailedPlacement && curr != gSoldierInitHead )
		{	//Priority existance nodes without detailed placement info are moved to beginning of list
			//copy node
			temp = curr;
			if( temp == gSoldierInitTail )
			{ //If we dealing with the tail, we need to move it now.
				gSoldierInitTail = gSoldierInitTail->prev;
				gSoldierInitTail->next = NULL;
				temp->prev = NULL;
			}
			curr = curr->next;
			//detach node from list
			if( temp->prev )
				temp->prev->next = temp->next;
			if( temp->next )
				temp->next->prev = temp->prev;
			//add node to beginning of list
			temp->prev = NULL;
			temp->next = gSoldierInitHead;
			gSoldierInitHead->prev = temp;
			gSoldierInitHead = temp;
			if( gfOriginalList )
				gOriginalSoldierInitListHead = gSoldierInitHead;
			else
				gAlternateSoldierInitListHead = gSoldierInitHead;
		}
		else
		{	
			curr = curr->next;
		}
	}
	//3rd priority sort (see below for reason why we do 2nd after 3rd)
	curr = gSoldierInitHead;
	while( curr )
	{
		if( !curr->pSoldier && curr->pBasicPlacement->fPriorityExistance && !curr->pDetailedPlacement && curr != gSoldierInitHead )
		{	//Priority existance nodes without detailed placement info are moved to beginning of list
			//copy node
			temp = curr;
			if( temp == gSoldierInitTail )
			{ //If we dealing with the tail, we need to move it now.
				gSoldierInitTail = gSoldierInitTail->prev;
				gSoldierInitTail->next = NULL;
				temp->prev = NULL;
			}
			curr = curr->next;
			//detach node from list
			if( temp->prev )
				temp->prev->next = temp->next;
			if( temp->next )
				temp->next->prev = temp->prev;
			//add node to beginning of list
			temp->prev = NULL;
			temp->next = gSoldierInitHead;
			gSoldierInitHead->prev = temp;
			gSoldierInitHead = temp;
			if( gfOriginalList )
				gOriginalSoldierInitListHead = gSoldierInitHead;
			else
				gAlternateSoldierInitListHead = gSoldierInitHead;
		}
		else
		{	
			curr = curr->next;
		}
	}
	//2nd priority sort (by adding these to the front, it'll be before the 
	//3rd priority sort.	This is why we do it after.
	curr = gSoldierInitHead;
	while( curr )
	{
		if( !curr->pSoldier && curr->pBasicPlacement->fPriorityExistance && curr->pDetailedPlacement && curr != gSoldierInitHead )
		{	//Priority existance nodes are moved to beginning of list
			//copy node
			temp = curr;
			if( temp == gSoldierInitTail )
			{ //If we dealing with the tail, we need to move it now.
				gSoldierInitTail = gSoldierInitTail->prev;
				gSoldierInitTail->next = NULL;
				temp->prev = NULL;
			}
			curr = curr->next;
			//detach node from list
			if( temp->prev )
				temp->prev->next = temp->next;
			if( temp->next )
				temp->next->prev = temp->prev;
			//add node to beginning of list
			temp->prev = NULL;
			temp->next = gSoldierInitHead;
			gSoldierInitHead->prev = temp;
			gSoldierInitHead = temp;
			if( gfOriginalList )
				gOriginalSoldierInitListHead = gSoldierInitHead;
			else
				gAlternateSoldierInitListHead = gSoldierInitHead;
		}
		else
		{	
			curr = curr->next;
		}
	}
	//4th priority sort
	//Done!	If the soldier existing slots are at the end of the list and the
	//			priority placements are at the beginning of the list, then the
	//			basic placements are in the middle.

	curr = gSoldierInitHead;
	while( curr )
	{
		if ( curr->pDetailedPlacement && curr->pDetailedPlacement->ubProfile == FREDO )
		{
			fFredoAtEnd = TRUE;
			break;
		}
		curr = curr->next;
	}

}

BOOLEAN AddPlacementToWorld( SOLDIERINITNODE *curr, GROUP *pGroup = NULL )
{
	PERFORMANCE_MARKER
	UINT8 ubProfile;
	SOLDIERCREATE_STRUCT tempDetailedPlacement;
	SOLDIERTYPE *pSoldier;
	UINT8 ubID;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorld"));
	// First check if this guy has a profile and if so check his location such that it matches!
	// Get profile from placement info
	tempDetailedPlacement.initialize();

	if (curr->pBasicPlacement->bBodyType == TANK_NW ||
		curr->pBasicPlacement->bBodyType == TANK_NE)
	{
		while (1)
		{
			ROTTING_CORPSE *pCorpse = GetCorpseAtGridNo( curr->pBasicPlacement->sStartingGridNo, 0); // I assume we don't find tanks on the roof
			if (pCorpse)
			{
				// Assume this is a dead tank and have the replacement tank haul it away
				RemoveCorpse( pCorpse->iID);
			}
			else
			{
				break;
			}
		}
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorld: decide on placement"));
	if( curr->pDetailedPlacement )
	{
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorld: already have detailed placement"));
		ubProfile = curr->pDetailedPlacement->ubProfile;

		if( ubProfile != NO_PROFILE && !gfEditMode )
		{
			if( gMercProfiles[ ubProfile ].sSectorX != gWorldSectorX ||
				gMercProfiles[ ubProfile ].sSectorY != gWorldSectorY ||
					gMercProfiles[ ubProfile ].bSectorZ != gbWorldSectorZ ||
					gMercProfiles[ ubProfile ].ubMiscFlags & (PROFILE_MISC_FLAG_RECRUITED | PROFILE_MISC_FLAG_EPCACTIVE) ||
//				gMercProfiles[ ubProfile ].ubMiscFlags2 & PROFILE_MISC_FLAG2_DONT_ADD_TO_SECTOR ||
					!gMercProfiles[ ubProfile ].bLife	||
					gMercProfiles[ ubProfile ].fUseProfileInsertionInfo 
				)
			{
				return FALSE;
			}
		}
		//Special case code when adding icecream truck.
		if( !gfEditMode )
		{
			// CJC, August 18, 1999: don't do this code unless the ice cream truck is on our team
			if ( FindSoldierByProfileID( ICECREAMTRUCK, TRUE ) != NULL )
			{
				if( curr->pDetailedPlacement->bBodyType == ICECREAMTRUCK )
				{ //Check to see if Hamous is here and not recruited.	If so, add truck
					if( gMercProfiles[ HAMOUS ].sSectorX != gWorldSectorX ||
							gMercProfiles[ HAMOUS ].sSectorY != gWorldSectorY ||
							gMercProfiles[ HAMOUS ].bSectorZ )
					{ //not here, so don't add
						return TRUE;
					}
					//Hamous is here.	Check to make sure he isn't recruited.
					if( gMercProfiles[ HAMOUS ].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED )
					{
						return TRUE;
					}
				}
			}
		}
		CreateDetailedPlacementGivenStaticDetailedPlacementAndBasicPlacementInfo(
			&tempDetailedPlacement, curr->pDetailedPlacement, curr->pBasicPlacement );		
	}
	else
	{
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorld: create detailed placement"));
		CreateDetailedPlacementGivenBasicPlacementInfo( &tempDetailedPlacement, curr->pBasicPlacement );
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorld: if not edit mode"));
	if ( !gfEditMode )
	{
		if( tempDetailedPlacement.bTeam == CIV_TEAM )
		{
			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorld: set quest overrides"));
			// quest-related overrides
			if ( gWorldSectorX == 5 && gWorldSectorY == MAP_ROW_C )
			{
				UINT8	ubRoom;

				// Kinpin guys might be guarding Tony
				if ( tempDetailedPlacement.ubCivilianGroup == KINGPIN_CIV_GROUP && ( gTacticalStatus.fCivGroupHostile[ KINGPIN_CIV_GROUP ] == CIV_GROUP_WILL_BECOME_HOSTILE || ( (gubQuest[ QUEST_KINGPIN_MONEY ] == QUESTINPROGRESS) && (CheckFact( FACT_KINGPIN_CAN_SEND_ASSASSINS, KINGPIN )) ) ) )
				{
					if (tempDetailedPlacement.ubProfile == NO_PROFILE) 
					{
						// these guys should be guarding Tony!
						tempDetailedPlacement.sInsertionGridNo = 13531 +
							(INT16) ( PreRandom( 8 ) * ( PreRandom( 1 ) ? -1 : 1)
							+ PreRandom( 8 ) * ( PreRandom( 1 ) ? -1 : 1) * WORLD_ROWS );

						switch( PreRandom( 3 ) )
						{
							case 0:
								tempDetailedPlacement.bOrders = ONGUARD;
								break;
							case 1:
								tempDetailedPlacement.bOrders = CLOSEPATROL;
								break;
							case 2:
								tempDetailedPlacement.bOrders = ONCALL;
								break;

						}
					}
					else if (tempDetailedPlacement.ubProfile == BILLY ) 
					{
						// billy should now be able to roam around
						tempDetailedPlacement.sInsertionGridNo = 13531 +
							(INT16) ( PreRandom( 30 ) * ( PreRandom( 1 ) ? -1 : 1)
							+ PreRandom( 30 ) * ( PreRandom( 1 ) ? -1 : 1) * WORLD_ROWS );
						tempDetailedPlacement.bOrders = SEEKENEMY;
					}
					else if ( tempDetailedPlacement.ubProfile == MADAME )
					{
						// she shouldn't be here!
						return( TRUE );
					}
					else if ( tempDetailedPlacement.ubProfile == NO_PROFILE && InARoom( tempDetailedPlacement.sInsertionGridNo, &ubRoom ) && IN_BROTHEL( ubRoom ) )
					{					
						// must be a hooker, shouldn't be there
						return( TRUE );
					}
				}
			}
			else if ( !gfInMeanwhile && gWorldSectorX == 3 && gWorldSectorY == 16 && !gbWorldSectorZ )
			{ //Special civilian setup for queen's palace.
				if( gubFact[ FACT_QUEEN_DEAD ] ) 
				{
					if( tempDetailedPlacement.ubCivilianGroup == QUEENS_CIV_GROUP )
					{ //The queen's civs aren't added if queen is dead
						return TRUE;
					}
				}
				else
				{
					if( gfUseAlternateQueenPosition && tempDetailedPlacement.ubProfile == QUEEN )
					{
						tempDetailedPlacement.sInsertionGridNo = 11448;
					}
					if( tempDetailedPlacement.ubCivilianGroup != QUEENS_CIV_GROUP )
					{ //The free civilians aren't added if queen is alive
						return TRUE;
					}
				}
			}
			else if ( gWorldSectorX == TIXA_SECTOR_X && gWorldSectorY == TIXA_SECTOR_Y	&& gbWorldSectorZ == 0 )
			{
				// Tixa prison, once liberated, should not have any civs without profiles unless
				// they are kids
				if( !StrategicMap[ TIXA_SECTOR_X + TIXA_SECTOR_Y * MAP_WORLD_X ].fEnemyControlled && tempDetailedPlacement.ubProfile == NO_PROFILE && tempDetailedPlacement.bBodyType != HATKIDCIV && tempDetailedPlacement.bBodyType != KIDCIV )
				{
					// not there
					return( TRUE );
				}
			}
			else if ( gWorldSectorX == 13 && gWorldSectorY == MAP_ROW_C && gbWorldSectorZ == 0 )
			{
				if ( CheckFact( FACT_KIDS_ARE_FREE, 0 ) )
				{
					if ( tempDetailedPlacement.bBodyType == HATKIDCIV || tempDetailedPlacement.bBodyType == KIDCIV )
					{
						// not there any more!	kids have been freeeeed!
						return( TRUE );
					}
				}
			}
		}
		
		//SPECIAL!	Certain events in the game can cause profiled NPCs to become enemies.	The two cases are 
		//adding Mike and Iggy.	We will only add one NPC in any given combat and the conditions for setting
		//the associated facts are done elsewhere.	There is also another place where NPCs can get added, which
		//is in TacticalCreateElite() used for inserting offensive enemies.
		if( tempDetailedPlacement.bTeam == ENEMY_TEAM && tempDetailedPlacement.ubSoldierClass == SOLDIER_CLASS_ELITE )
		{
			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorld: set npcs to enemies"));
			OkayToUpgradeEliteToSpecialProfiledEnemy( &tempDetailedPlacement );
		}
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorld: create soldier"));
	pSoldier = TacticalCreateSoldier( &tempDetailedPlacement, &ubID );
	if( pSoldier )
	{
		curr->pSoldier = pSoldier;
		curr->ubSoldierID = ubID;
		if (pGroup)
		{
			pSoldier->ubGroupID = pGroup->ubGroupID;
			pSoldier->pGroup = pGroup;
		}

		AddSoldierToSectorNoCalculateDirection( ubID );

		if( pSoldier->bActive && pSoldier->bInSector && pSoldier->bTeam == ENEMY_TEAM && !pSoldier->inv[ HANDPOS ].usItem )
		{
			pSoldier = pSoldier;
		}
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorld: return true"));

		return TRUE;
	}
	else
	{
		DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("Failed to create soldier using TacticalCreateSoldier within AddPlacementToWorld"));
		LiveMessage( "Failed to create soldier using TacticalCreateSoldier within AddPlacementToWorld" );
	}

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorld: return false"));
	return FALSE;
}

void AddPlacementToWorldByProfileID( UINT8 ubProfile )
{
	PERFORMANCE_MARKER
	SOLDIERINITNODE * curr;
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddPlacementToWorldByProfileID"));

	curr = gSoldierInitHead;
	while( curr )
	{
		if ( curr->pDetailedPlacement && curr->pDetailedPlacement->ubProfile == ubProfile && !curr->pSoldier )
		{	
			//Matching profile, so add this placement.
			AddPlacementToWorld( curr );
			break;
		}
		curr = curr->next;
	}
}

UINT8 AddSoldierInitListTeamToWorld( INT8 bTeam, UINT8 ubMaxNum )
{
	PERFORMANCE_MARKER
	UINT8 ubNumAdded = 0;
	SOLDIERINITNODE *mark;
	UINT8 ubSlotsToFill;
	UINT8 ubSlotsAvailable;
	SOLDIERINITNODE *curr;
	
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddSoldierInitListTeamToWorld"));
	//Sort the list in the following manner:
	//-Priority placements first
	//-Basic placements next
	//-Any placements with existing soldiers last (overrides others)
	SortSoldierInitList();

	if( giCurrentTilesetID == 1 ) //cave/mine tileset only 
	{ //convert all civilians to miners which use uniforms and more masculine body types.
		curr = gSoldierInitHead;
		while( curr )
		{
			if( curr->pBasicPlacement->bTeam == CIV_TEAM && !curr->pDetailedPlacement )
			{
				curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_MINER;
				curr->pBasicPlacement->bBodyType = -1;
			}
			curr = curr->next;
		}
	}

	//Count the current number of soldiers of the specified team
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->pBasicPlacement->bTeam == bTeam && curr->pSoldier )
			ubNumAdded++;	//already one here!
		curr = curr->next;
	}

	curr = gSoldierInitHead;

	//First fill up all of the priority existance slots...
	while( curr && curr->pBasicPlacement->fPriorityExistance && ubNumAdded < ubMaxNum )
	{
		if( curr->pBasicPlacement->bTeam == bTeam )
		{	
			//Matching team, so add this placement.
			if( AddPlacementToWorld( curr ) )
			{
				ubNumAdded++;
			}
		}
		curr = curr->next;
	}
	if( ubNumAdded == ubMaxNum )
		return ubNumAdded;

	//Now count the number of nodes that are basic placements of desired team
	//This information will be used to randomly determine which of these placements
	//will be added based on the number of slots we can still add.
	mark = curr;
	ubSlotsAvailable = 0;
	ubSlotsToFill = ubMaxNum - ubNumAdded;
	while( curr && !curr->pSoldier && ubNumAdded < ubMaxNum )
	{
		if( curr->pBasicPlacement->bTeam == bTeam )
			ubSlotsAvailable++;
		curr = curr->next;
	}

	//we now have the number, so compared it to the num we can add, and determine how we will
	//randomly determine which nodes to add.
	if( !ubSlotsAvailable )
	{	//There aren't any basic placements of desired team, so exit.
		return ubNumAdded;
	}
	curr = mark;
	//while we have a list, with no active soldiers, the num added is less than the max num requested, and
	//we have slots available, process the list to add new soldiers.
	while( curr && !curr->pSoldier && ubNumAdded < ubMaxNum && ubSlotsAvailable )
	{
		if( curr->pBasicPlacement->bTeam == bTeam )
		{
			if( ubSlotsAvailable <= ubSlotsToFill || Random( ubSlotsAvailable ) < ubSlotsToFill )
			{
				//found matching team, so add this soldier to the game.
				if( AddPlacementToWorld( curr ) )
				{
					ubNumAdded++;
				}
				else
				{
					//if it fails to create the soldier, it is likely that it is because the slots in the tactical 
					//engine are already full.	Besides, the strategic AI shouldn't be trying to fill a map with
					//more than the maximum allowable soldiers of team.	All teams can have a max of 32 individuals,
					//except for the player which is 20.	Players aren't processed in this list anyway.
					return ubNumAdded;
				}
				ubSlotsToFill--;
			}
			ubSlotsAvailable--;
			//With the decrementing of the slot vars in this manner, the chances increase so that all slots
			//will be full by the time the end of the list comes up.
		}
		curr = curr->next;
	}
	return ubNumAdded;
}

void AddSoldierInitListEnemyDefenceSoldiers( UINT8 ubTotalAdmin, UINT8 ubTotalTroops, UINT8 ubTotalElite )
{
	PERFORMANCE_MARKER
	SOLDIERINITNODE *mark;
	SOLDIERINITNODE *curr;
	INT32 iRandom;
	UINT8 ubMaxNum;
 	UINT8 ubElitePDSlots = 0, ubEliteDSlots = 0, ubElitePSlots = 0, ubEliteBSlots = 0;
	UINT8 ubTroopPDSlots = 0, ubTroopDSlots = 0, ubTroopPSlots = 0, ubTroopBSlots = 0;
	UINT8 ubAdminPDSlots = 0, ubAdminDSlots = 0, ubAdminPSlots = 0, ubAdminBSlots = 0;
	UINT8 ubFreeSlots;
	UINT8 *pCurrSlots=NULL;
	UINT8 *pCurrTotal=NULL;
	UINT8 ubCurrClass;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddSoldierInitListEnemyDefenceSoldiers"));

	ResetMortarsOnTeamCount();

	//Specs call for only one profiled enemy can be in a sector at a time due to flavor reasons.
	gfProfiledEnemyAdded = FALSE;
	
	//Because the enemy defence forces work differently than the regular map placements, the numbers
	//of each type of enemy may not be the same.	Elites will choose the best placements, then army, then
	//administrators.

	ubMaxNum = ubTotalAdmin + ubTotalTroops + ubTotalElite;
	
	//Sort the list in the following manner:
	//-Priority placements first
	//-Basic placements next
	//-Any placements with existing soldiers last (overrides others)
	SortSoldierInitList();

	//Now count the number of nodes that are basic placements of desired team AND CLASS
	//This information will be used to randomly determine which of these placements
	//will be added based on the number of slots we can still add.
	curr = gSoldierInitHead;
	while( curr && !curr->pSoldier )
	{
		if( curr->pBasicPlacement->bTeam == ENEMY_TEAM )
		{
			switch( curr->pBasicPlacement->ubSoldierClass )
			{
				case SOLDIER_CLASS_ELITE:
					if( curr->pBasicPlacement->fPriorityExistance && curr->pDetailedPlacement )
						ubElitePDSlots++;	
					else if( curr->pBasicPlacement->fPriorityExistance )
						ubElitePSlots++;
					else if( curr->pDetailedPlacement )
						ubEliteDSlots++;
					else
						ubEliteBSlots++;
					break;
				case SOLDIER_CLASS_ADMINISTRATOR:		
					if( curr->pBasicPlacement->fPriorityExistance && curr->pDetailedPlacement )
						ubAdminPDSlots++;	
					else if( curr->pBasicPlacement->fPriorityExistance )
						ubAdminPSlots++;
					else if( curr->pDetailedPlacement )
						ubAdminDSlots++;
					else
						ubAdminBSlots++;
					break;
				case SOLDIER_CLASS_ARMY:						
					if( curr->pBasicPlacement->fPriorityExistance && curr->pDetailedPlacement )
						ubTroopPDSlots++;	
					else if( curr->pBasicPlacement->fPriorityExistance )
						ubTroopPSlots++;
					else if( curr->pDetailedPlacement )
						ubTroopDSlots++;
					else
						ubTroopBSlots++;
					break;
			}
		}
		curr = curr->next;
	}

	//ADD PLACEMENTS WITH PRIORITY EXISTANCE WITH DETAILED PLACEMENT INFORMATION FIRST
	//we now have the numbers of available slots for each soldier class, so loop through three times
	//and randomly choose some (or all) of the matching slots to fill.	This is done randomly.
	for( ubCurrClass = SOLDIER_CLASS_ADMINISTRATOR; ubCurrClass <= SOLDIER_CLASS_ARMY; ubCurrClass++ )
	{
		//First, prepare the counters.
		switch( ubCurrClass )
		{
			case SOLDIER_CLASS_ADMINISTRATOR:	
				pCurrSlots = &ubAdminPDSlots;
				pCurrTotal = &ubTotalAdmin;
				break;
			case SOLDIER_CLASS_ELITE:
				pCurrSlots = &ubElitePDSlots;
				pCurrTotal = &ubTotalElite;
				break;
			case SOLDIER_CLASS_ARMY:
				pCurrSlots = &ubTroopPDSlots;
				pCurrTotal = &ubTotalTroops;
				break;
		}
		//Now, loop through the priority existance and detailed placement section of the list.
		curr = gSoldierInitHead; 
		while( curr && ubMaxNum && *pCurrTotal && *pCurrSlots &&
			curr->pDetailedPlacement && curr->pBasicPlacement->fPriorityExistance )
		{
			if( !curr->pSoldier && curr->pBasicPlacement->bTeam == ENEMY_TEAM )
			{
				if( curr->pBasicPlacement->ubSoldierClass == ubCurrClass )
				{
					if( *pCurrSlots <= *pCurrTotal || Random( *pCurrSlots ) < *pCurrTotal )
					{
						//found matching team, so add this soldier to the game.
						if( AddPlacementToWorld( curr ) )
						{
							(*pCurrTotal)--;
							ubMaxNum--;
						}
						else
							return;
					}
					(*pCurrSlots)--;
					//With the decrementing of the slot vars in this manner, the chances increase so that all slots
					//will be full by the time the end of the list comes up.
				}
			}
			curr = curr->next;
		}
	}
	if( !ubMaxNum )
		return;
	curr = gSoldierInitHead;
	while( curr && curr->pDetailedPlacement && curr->pBasicPlacement->fPriorityExistance )
		curr = curr->next;
	mark = curr;

	//ADD PLACEMENTS WITH PRIORITY EXISTANCE AND NO DETAILED PLACEMENT INFORMATION SECOND
	//we now have the numbers of available slots for each soldier class, so loop through three times
	//and randomly choose some (or all) of the matching slots to fill.	This is done randomly.
	for( ubCurrClass = SOLDIER_CLASS_ADMINISTRATOR; ubCurrClass <= SOLDIER_CLASS_ARMY; ubCurrClass++ )
	{
		//First, prepare the counters.
		switch( ubCurrClass )
		{
			case SOLDIER_CLASS_ADMINISTRATOR:	
				pCurrSlots = &ubAdminPSlots;
				pCurrTotal = &ubTotalAdmin;
				break;
			case SOLDIER_CLASS_ELITE:
				pCurrSlots = &ubElitePSlots;
				pCurrTotal = &ubTotalElite;
				break;
			case SOLDIER_CLASS_ARMY:
				pCurrSlots = &ubTroopPSlots;
				pCurrTotal = &ubTotalTroops;
				break;
		}
		//Now, loop through the priority existance and non detailed placement section of the list.
		curr = mark;
		while( curr && ubMaxNum && *pCurrTotal && *pCurrSlots &&
			!curr->pDetailedPlacement && curr->pBasicPlacement->fPriorityExistance )
		{
			if( !curr->pSoldier && curr->pBasicPlacement->bTeam == ENEMY_TEAM )
			{
				if( curr->pBasicPlacement->ubSoldierClass == ubCurrClass )
				{
					if( *pCurrSlots <= *pCurrTotal || Random( *pCurrSlots ) < *pCurrTotal )
					{
						//found matching team, so add this soldier to the game.
						if( AddPlacementToWorld( curr ) )
						{
							(*pCurrTotal)--;
							ubMaxNum--;
						}
						else
							return;
					}
					(*pCurrSlots)--;
					//With the decrementing of the slot vars in this manner, the chances increase so that all slots
					//will be full by the time the end of the list comes up.
				}
			}
			curr = curr->next;
		}
	}
	if( !ubMaxNum )
		return;
	curr = mark;
	while( curr && !curr->pDetailedPlacement && curr->pBasicPlacement->fPriorityExistance )
		curr = curr->next;
	mark = curr;
	
	//ADD PLACEMENTS WITH NO DETAILED PLACEMENT AND PRIORITY EXISTANCE INFORMATION SECOND
	//we now have the numbers of available slots for each soldier class, so loop through three times
	//and randomly choose some (or all) of the matching slots to fill.	This is done randomly.
	for( ubCurrClass = SOLDIER_CLASS_ADMINISTRATOR; ubCurrClass <= SOLDIER_CLASS_ARMY; ubCurrClass++ )
	{
		//First, prepare the counters.
		switch( ubCurrClass )
		{
			case SOLDIER_CLASS_ADMINISTRATOR:	
				pCurrSlots = &ubAdminDSlots;
				pCurrTotal = &ubTotalAdmin;
				break;
			case SOLDIER_CLASS_ELITE:
				pCurrSlots = &ubEliteDSlots;
				pCurrTotal = &ubTotalElite;
				break;
			case SOLDIER_CLASS_ARMY:
				pCurrSlots = &ubTroopDSlots;
				pCurrTotal = &ubTotalTroops;
				break;
		}
		//Now, loop through the priority existance and detailed placement section of the list.
		curr = mark;
		while( curr && ubMaxNum && *pCurrTotal && *pCurrSlots &&
			curr->pDetailedPlacement && !curr->pBasicPlacement->fPriorityExistance )
		{
			if( !curr->pSoldier && curr->pBasicPlacement->bTeam == ENEMY_TEAM )
			{
				if( curr->pBasicPlacement->ubSoldierClass == ubCurrClass )
				{
					if( *pCurrSlots <= *pCurrTotal || Random( *pCurrSlots ) < *pCurrTotal )
					{
						//found matching team, so add this soldier to the game.
						if( AddPlacementToWorld( curr ) )
						{
							(*pCurrTotal)--;
							ubMaxNum--;
						}
						else
							return;
					}
					(*pCurrSlots)--;
					//With the decrementing of the slot vars in this manner, the chances increase so that all slots
					//will be full by the time the end of the list comes up.
				}
			}
			curr = curr->next;
		}
	}
	if( !ubMaxNum )
		return;
	curr = mark;
	while( curr && curr->pDetailedPlacement && !curr->pBasicPlacement->fPriorityExistance )
		curr = curr->next;
	mark = curr;

	//Kris: January 11, 2000 -- NEW!!!
	//PRIORITY EXISTANT SLOTS MUST BE FILLED
	//This must be done to ensure all priority existant slots are filled before ANY other slots are filled,
	//even if that means changing the class of the slot.	Also, assume that there are no matching fits left 
	//for priority existance slots.	All of the matches have been already assigned in the above passes.	
	//We'll have to convert the soldier type of the slot to match.
	curr = gSoldierInitHead; 
	while( curr && ubMaxNum && curr->pBasicPlacement->fPriorityExistance )
	{
		if( !curr->pSoldier && curr->pBasicPlacement->bTeam == ENEMY_TEAM )
		{
			//Choose which team to use.
			iRandom = Random( ubMaxNum );
			if( iRandom < ubTotalElite )
			{
				curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_ELITE;
				ubTotalElite--;
			}
			else if( iRandom < ubTotalElite + ubTotalTroops )
			{
				curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_ARMY;
				ubTotalTroops--;
			}
			else if( iRandom < ubTotalElite + ubTotalTroops + ubTotalAdmin )
			{
				curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_ADMINISTRATOR;
				ubTotalAdmin--;
			}
			else
				Assert(0);
			if( AddPlacementToWorld( curr ) )
			{
				ubMaxNum--;
			}
			else
				return;
		}
		curr = curr->next;
	}
	if( !ubMaxNum )
		return;
	
	//ADD REMAINING PLACEMENTS WITH BASIC PLACEMENT INFORMATION
	//we now have the numbers of available slots for each soldier class, so loop through three times
	//and randomly choose some (or all) of the matching slots to fill.	This is done randomly.
	for( ubCurrClass = SOLDIER_CLASS_ADMINISTRATOR; ubCurrClass <= SOLDIER_CLASS_ARMY; ubCurrClass++ )
	{
		//First, prepare the counters.
		switch( ubCurrClass )
		{
			case SOLDIER_CLASS_ADMINISTRATOR:	
				pCurrSlots = &ubAdminBSlots;
				pCurrTotal = &ubTotalAdmin;
				break;
			case SOLDIER_CLASS_ELITE:
				pCurrSlots = &ubEliteBSlots;
				pCurrTotal = &ubTotalElite;
				break;
			case SOLDIER_CLASS_ARMY:
				pCurrSlots = &ubTroopBSlots;
				pCurrTotal = &ubTotalTroops;
				break;
		}
		//Now, loop through the regular basic placements section of the list.
		curr = mark;
		while( curr && ubMaxNum && *pCurrTotal && *pCurrSlots )
		{
			if( !curr->pSoldier && curr->pBasicPlacement->bTeam == ENEMY_TEAM )
			{
				if( curr->pBasicPlacement->ubSoldierClass == ubCurrClass )
				{
					if( *pCurrSlots <= *pCurrTotal || Random( *pCurrSlots ) < *pCurrTotal )
					{
						//found matching team, so add this soldier to the game.
						if( AddPlacementToWorld( curr ) )
						{
							(*pCurrTotal)--;
							ubMaxNum--;
						}
						else
							return;
					}
					(*pCurrSlots)--;
					//With the decrementing of the slot vars in this manner, the chances increase so that all slots
					//will be full by the time the end of the list comes up.
				}
			}
			curr = curr->next;
		}
	}
	if( !ubMaxNum )
		return;

	//If we are at this point, that means that there are some compatibility issues.	This is fine.	An example 
	//would be a map containing 1 elite placement, and 31 troop placements.	If we had 3 elites move into this
	//sector, we would not have placements for two of them.	What we have to do is override the class information
	//contained in the list by choosing unused placements, and assign them to the elites.	This time, we will 
	//use all free slots including priority placement slots (ignoring the priority placement information).	

	//First, count up the total number of free slots.
	ubFreeSlots = 0;
	curr = gSoldierInitHead; 
	while( curr )
	{
		if( !curr->pSoldier && curr->pBasicPlacement->bTeam == ENEMY_TEAM )
			ubFreeSlots++;
		curr = curr->next;
	}

	//Now, loop through the entire list again, but for the last time.	All enemies will be inserted now ignoring
	//detailed placements and classes.
	curr = gSoldierInitHead; 
	while( curr && ubFreeSlots && ubMaxNum )
	{
		if( !curr->pSoldier && curr->pBasicPlacement->bTeam == ENEMY_TEAM )
		{
			//Randomly determine if we will use this slot; the more available slots in proportion to
			//the number of enemies, the lower the chance of accepting the slot.
			if( ubFreeSlots <= ubMaxNum || Random( ubFreeSlots ) < ubMaxNum )
			{
				//Choose which team to use.
				iRandom = Random( ubMaxNum );
				if( iRandom < ubTotalElite )
				{
					curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_ELITE;
					ubTotalElite--;
				}
				else if( iRandom < ubTotalElite + ubTotalTroops )
				{
					curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_ARMY;
					ubTotalTroops--;
				}
				else if( iRandom < ubTotalElite + ubTotalTroops + ubTotalAdmin )
				{
					curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_ADMINISTRATOR;
					ubTotalAdmin--;
				}
				else
					Assert(0);
				/* DISABLE THE OVERRIDE FOR NOW...
				if( curr->pDetailedPlacement )
				{ //delete the detailed placement information.
					MemFree( curr->pDetailedPlacement );
					curr->pDetailedPlacement = NULL;
					curr->pBasicPlacement->fDetailedPlacement = FALSE;
				}
				*/
				if( AddPlacementToWorld( curr ) )
				{
					ubMaxNum--;
				}
				else
					return;
			}
			ubFreeSlots--;
			//With the decrementing of the slot vars in this manner, the chances increase so that all slots
			//will be full by the time the end of the list comes up.
		}
		curr = curr->next;
	}
}

//If we are adding militia to our map, then we do a few things differently.
//First of all, they exist exclusively to the enemy troops, so if the militia exists in the
//sector, then they get to use the enemy placements.	However, we remove any orders from
//placements containing RNDPTPATROL or POINTPATROL orders, as well as remove any detailed
//placement information.
void AddSoldierInitListMilitia( UINT8 ubNumGreen, UINT8 ubNumRegs, UINT8 ubNumElites )
{
	PERFORMANCE_MARKER
	SOLDIERINITNODE *mark;
	SOLDIERINITNODE *curr;
	INT32 iRandom;
	UINT8 ubMaxNum;
	BOOLEAN fDoPlacement;
 	UINT8 ubEliteSlots = 0;
	UINT8 ubRegSlots = 0;
	UINT8 ubGreenSlots = 0;
	UINT8 ubFreeSlots;
	UINT8 *pCurrSlots=NULL;
	UINT8 *pCurrTotal=NULL;
	UINT8 ubCurrClass;

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddSoldierInitListMilitia"));

	ubMaxNum = ubNumGreen + ubNumRegs + ubNumElites;
	
	//Sort the list in the following manner:
	//-Priority placements first
	//-Basic placements next
	//-Any placements with existing soldiers last (overrides others)
	SortSoldierInitList();

	curr = gSoldierInitHead;

	//First fill up only the priority existance slots (as long as the availability and class are okay)
	while( curr && curr->pBasicPlacement->fPriorityExistance && ubMaxNum )
	{
		fDoPlacement = TRUE;

		if( curr->pBasicPlacement->bTeam == ENEMY_TEAM || curr->pBasicPlacement->bTeam == MILITIA_TEAM )
		{
			//Matching team (kindof), now check the soldier class...
			if( ubNumElites && curr->pBasicPlacement->ubSoldierClass == SOLDIER_CLASS_ELITE )
			{
				curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_ELITE_MILITIA;
				ubNumElites--;
			}
			else if( ubNumRegs && curr->pBasicPlacement->ubSoldierClass == SOLDIER_CLASS_ARMY )
			{
				curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_REG_MILITIA;
				ubNumRegs--;
			}
			else if( ubNumGreen && curr->pBasicPlacement->ubSoldierClass == SOLDIER_CLASS_ADMINISTRATOR )
			{
				curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_GREEN_MILITIA;
				ubNumGreen--;
			}
			else
				fDoPlacement = FALSE;

			if ( fDoPlacement )
			{
				curr->pBasicPlacement->bTeam = MILITIA_TEAM;
				curr->pBasicPlacement->bOrders = STATIONARY;
				curr->pBasicPlacement->bAttitude = (INT8) Random( MAXATTITUDES );
				if( curr->pDetailedPlacement )
				{ //delete the detailed placement information.
					MemFree( curr->pDetailedPlacement );
					curr->pDetailedPlacement = NULL;
					curr->pBasicPlacement->fDetailedPlacement = FALSE;
					RandomizeRelativeLevel( &( curr->pBasicPlacement->bRelativeAttributeLevel ), curr->pBasicPlacement->ubSoldierClass );
					RandomizeRelativeLevel( &( curr->pBasicPlacement->bRelativeEquipmentLevel ), curr->pBasicPlacement->ubSoldierClass );
				}
				if( AddPlacementToWorld( curr ) )
				{
					ubMaxNum--;
				}
				else
					return;
			}
		}
		curr = curr->next;
	}
	if( !ubMaxNum )
		return;
	//Now count the number of nodes that are basic placements of desired team AND CLASS
	//This information will be used to randomly determine which of these placements
	//will be added based on the number of slots we can still add.
	mark = curr;
	while( curr && !curr->pSoldier )
	{
		if( curr->pBasicPlacement->bTeam == ENEMY_TEAM || curr->pBasicPlacement->bTeam == MILITIA_TEAM )
		{
			switch( curr->pBasicPlacement->ubSoldierClass )
			{
				case SOLDIER_CLASS_ELITE:						ubEliteSlots++;	break;
				case SOLDIER_CLASS_ADMINISTRATOR:		ubGreenSlots++; break;
				case SOLDIER_CLASS_ARMY:						ubRegSlots++; break;
			}
		}
		curr = curr->next;
	}

	//we now have the numbers of available slots for each soldier class, so loop through three times
	//and randomly choose some (or all) of the matching slots to fill.	This is done randomly.
	for( ubCurrClass = SOLDIER_CLASS_ADMINISTRATOR; ubCurrClass <= SOLDIER_CLASS_ARMY; ubCurrClass++ )
	{
		//First, prepare the counters.
		switch( ubCurrClass )
		{
			case SOLDIER_CLASS_ADMINISTRATOR:	
				pCurrSlots = &ubGreenSlots;
				pCurrTotal = &ubNumGreen;
				break;
			case SOLDIER_CLASS_ELITE:
				pCurrSlots = &ubEliteSlots;
				pCurrTotal = &ubNumElites;
				break;
			case SOLDIER_CLASS_ARMY:
				pCurrSlots = &ubRegSlots;
				pCurrTotal = &ubNumRegs;
				break;
		}
		//Now, loop through the basic placement of the list.
		curr = mark; //mark is the marker where the basic placements start.
		while( curr && !curr->pSoldier && ubMaxNum && *pCurrTotal && *pCurrSlots )
		{
			if( curr->pBasicPlacement->bTeam == ENEMY_TEAM || curr->pBasicPlacement->bTeam == MILITIA_TEAM )
			{
				if( curr->pBasicPlacement->ubSoldierClass == ubCurrClass )
				{
					if( *pCurrSlots <= *pCurrTotal || Random( *pCurrSlots ) < *pCurrTotal )
					{
						curr->pBasicPlacement->bTeam = MILITIA_TEAM;
						curr->pBasicPlacement->bOrders = STATIONARY;
						switch( ubCurrClass )
						{
							case SOLDIER_CLASS_ADMINISTRATOR:	
								curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_GREEN_MILITIA;
								break;
							case SOLDIER_CLASS_ARMY:
								curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_REG_MILITIA;
								break;
							case SOLDIER_CLASS_ELITE:
								curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_ELITE_MILITIA;
								break;
						}
						//found matching team, so add this soldier to the game.
						if( AddPlacementToWorld( curr ) )
						{
							(*pCurrTotal)--;
							ubMaxNum--;
						}
						else
							return;
					}
					(*pCurrSlots)--;
					//With the decrementing of the slot vars in this manner, the chances increase so that all slots
					//will be full by the time the end of the list comes up.
				}
			}
			curr = curr->next;
		}
	}
	if( !ubMaxNum )
		return;
	//If we are at this point, that means that there are some compatibility issues.	This is fine.	An example 
	//would be a map containing 1 elite placement, and 31 troop placements.	If we had 3 elites move into this
	//sector, we would not have placements for two of them.	What we have to do is override the class information
	//contained in the list by choosing unused placements, and assign them to the elites.	This time, we will 
	//use all free slots including priority placement slots (ignoring the priority placement information).	

	//First, count up the total number of free slots.
	ubFreeSlots = 0;
	curr = gSoldierInitHead; 
	while( curr )
	{
		if( !curr->pSoldier && (curr->pBasicPlacement->bTeam == ENEMY_TEAM || curr->pBasicPlacement->bTeam == MILITIA_TEAM) )
			ubFreeSlots++;
		curr = curr->next;
	}

	//Now, loop through the entire list again, but for the last time.	All enemies will be inserted now ignoring
	//detailed placements and classes.
	curr = gSoldierInitHead; 
	while( curr && ubFreeSlots && ubMaxNum )
	{
		if( !curr->pSoldier && (curr->pBasicPlacement->bTeam == ENEMY_TEAM || curr->pBasicPlacement->bTeam == MILITIA_TEAM) )
		{
			//Randomly determine if we will use this slot; the more available slots in proportion to
			//the number of enemies, the lower the chance of accepting the slot.
			if( ubFreeSlots <= ubMaxNum || Random( ubFreeSlots ) < ubMaxNum )
			{
				//Choose which team to use.
				iRandom = Random( ubMaxNum );
				if( iRandom < ubNumElites )
				{
					curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_ELITE_MILITIA;
					ubNumElites--;
				}
				else if( iRandom < ubNumElites + ubNumRegs )
				{
					curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_REG_MILITIA;
					ubNumRegs--;
				}
				else if( iRandom < ubNumElites + ubNumRegs + ubNumGreen )
				{
					curr->pBasicPlacement->ubSoldierClass = SOLDIER_CLASS_GREEN_MILITIA;
					ubNumGreen--;
				}
				else
					Assert(0);
				curr->pBasicPlacement->bTeam = MILITIA_TEAM;
				curr->pBasicPlacement->bOrders = STATIONARY;
				curr->pBasicPlacement->bAttitude = (INT8) Random( MAXATTITUDES );
				if( curr->pDetailedPlacement )
				{ //delete the detailed placement information.
					MemFree( curr->pDetailedPlacement );
					curr->pDetailedPlacement = NULL;
					curr->pBasicPlacement->fDetailedPlacement = FALSE;
					RandomizeRelativeLevel( &( curr->pBasicPlacement->bRelativeAttributeLevel), curr->pBasicPlacement->ubSoldierClass );
					RandomizeRelativeLevel( &( curr->pBasicPlacement->bRelativeEquipmentLevel), curr->pBasicPlacement->ubSoldierClass );
				}
				if( AddPlacementToWorld( curr ) )
				{
					ubMaxNum--;
				}
				else
					return;
			}
			ubFreeSlots--;
			//With the decrementing of the slot vars in this manner, the chances increase so that all slots
			//will be full by the time the end of the list comes up.
		}
		curr = curr->next;
	}
}

void AddSoldierInitListCreatures( BOOLEAN fQueen, UINT8 ubNumLarvae, UINT8 ubNumInfants,	
																	UINT8 ubNumYoungMales, UINT8 ubNumYoungFemales, UINT8 ubNumAdultMales, 
																	UINT8 ubNumAdultFemales )
{
	PERFORMANCE_MARKER
	SOLDIERINITNODE *curr;
	INT32 iRandom;
	UINT8 ubFreeSlots;
	BOOLEAN fDoPlacement;
	UINT8 ubNumCreatures;
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddSoldierInitListCreatures"));

	SortSoldierInitList();

	//Okay, if we have a queen, place her first.	She MUST have a special placement, else
	//we can't use anything.
	ubNumCreatures = (UINT8)(ubNumLarvae + ubNumInfants + ubNumYoungMales + ubNumYoungFemales + ubNumAdultMales + ubNumAdultFemales);
	if( fQueen )
	{
		curr = gSoldierInitHead; 
		while( curr	)
		{
			if( !curr->pSoldier && curr->pBasicPlacement->bTeam == CREATURE_TEAM && curr->pBasicPlacement->bBodyType == QUEENMONSTER )
			{
				if( !AddPlacementToWorld( curr ) )
				{
					fQueen = FALSE;
					break;
				}
			}
			curr = curr->next;
		}
		if( !fQueen )
		{
			#ifdef JA2BETAVERSION
				ScreenMsg( FONT_RED, MSG_ERROR, L"Couldn't place the queen." );
			#endif
		}
	}

	//First fill up only the priority existance slots (as long as the availability and bodytypes match)
	curr = gSoldierInitHead; 
	while( curr && curr->pBasicPlacement->fPriorityExistance && ubNumCreatures )
	{
		fDoPlacement = TRUE;

		if( curr->pBasicPlacement->bTeam == CREATURE_TEAM )
		{
			//Matching team, now check the soldier class...
			if( ubNumLarvae && curr->pBasicPlacement->bBodyType == LARVAE_MONSTER )
				ubNumLarvae--;
			else if( ubNumInfants && curr->pBasicPlacement->bBodyType == INFANT_MONSTER )
				ubNumInfants--;
			else if( ubNumYoungMales && curr->pBasicPlacement->bBodyType == YAM_MONSTER )
				ubNumYoungMales--;
			else if( ubNumYoungFemales && curr->pBasicPlacement->bBodyType == YAF_MONSTER )
				ubNumYoungFemales--;
			else if( ubNumAdultMales && curr->pBasicPlacement->bBodyType == AM_MONSTER )
				ubNumAdultMales--;
			else if( ubNumAdultFemales && curr->pBasicPlacement->bBodyType == ADULTFEMALEMONSTER )
				ubNumAdultFemales--;
			else
				fDoPlacement = FALSE;
			if ( fDoPlacement )
			{
				if( AddPlacementToWorld( curr ) )
				{
					ubNumCreatures--;
				}
				else
					return;
			}
		}
		curr = curr->next;
	}
	if( !ubNumCreatures )
		return;

	//Count how many free creature slots are left.
	curr = gSoldierInitHead; 
	ubFreeSlots = 0;
	while( curr )
	{
		if( !curr->pSoldier && curr->pBasicPlacement->bTeam == CREATURE_TEAM )
			ubFreeSlots++;
		curr = curr->next;
	}
	//Now, if we still have creatures to place, do so completely randomly, overriding priority 
	//placements, etc.
	curr = gSoldierInitHead; 
	while( curr && ubFreeSlots && ubNumCreatures )
	{
		if( !curr->pSoldier && curr->pBasicPlacement->bTeam == CREATURE_TEAM )
		{
			//Randomly determine if we will use this slot; the more available slots in proportion to
			//the number of enemies, the lower the chance of accepting the slot.
			if( ubFreeSlots <= ubNumCreatures || Random( ubFreeSlots ) < ubNumCreatures )
			{
				//Choose which team to use.
				iRandom = Random( ubNumCreatures );
				
				if( ubNumLarvae && iRandom < ubNumLarvae )
				{
					ubNumLarvae--;
					curr->pBasicPlacement->bBodyType = LARVAE_MONSTER;
				}
				else if( ubNumInfants && iRandom < ubNumLarvae + ubNumInfants )
				{
					ubNumInfants--;
					curr->pBasicPlacement->bBodyType = INFANT_MONSTER;
				}
				else if( ubNumYoungMales && iRandom < ubNumLarvae + ubNumInfants + ubNumYoungMales )
				{
					ubNumYoungMales--;
					curr->pBasicPlacement->bBodyType = YAM_MONSTER;
				}
				else if( ubNumYoungFemales && iRandom < ubNumLarvae + ubNumInfants + ubNumYoungMales + ubNumYoungFemales )
				{
					ubNumYoungFemales--;
					curr->pBasicPlacement->bBodyType = YAF_MONSTER;
				}
				else if( ubNumAdultMales && iRandom < ubNumLarvae + ubNumInfants + ubNumYoungMales + ubNumYoungFemales + ubNumAdultMales )
				{
					ubNumAdultMales--;
					curr->pBasicPlacement->bBodyType = AM_MONSTER;
				}
				else if( ubNumAdultFemales && iRandom < ubNumLarvae + ubNumInfants + ubNumYoungMales + ubNumYoungFemales + ubNumAdultMales + ubNumAdultFemales )
				{
					ubNumAdultFemales--;
					curr->pBasicPlacement->bBodyType = ADULTFEMALEMONSTER;
				}
				else
					Assert(0);
				if( curr->pDetailedPlacement )
				{ //delete the detailed placement information.
					MemFree( curr->pDetailedPlacement );
					curr->pDetailedPlacement = NULL;
					curr->pBasicPlacement->fDetailedPlacement = FALSE;
				}
				if( AddPlacementToWorld( curr ) )
				{
					ubNumCreatures--;
				}
				else
				{
					return;
				}
			}
			ubFreeSlots--;
			//With the decrementing of the slot vars in this manner, the chances increase so that all slots
			//will be full by the time the end of the list comes up.
		}
		curr = curr->next;
	}
}



SOLDIERINITNODE* FindSoldierInitNodeWithProfileID( UINT16 usProfile );
SOLDIERINITNODE* FindSoldierInitNodeWithProfileID( UINT16 usProfile )
{
	PERFORMANCE_MARKER
	SOLDIERINITNODE *curr;
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->pDetailedPlacement && curr->pDetailedPlacement->ubProfile == usProfile )
			return curr;
		curr = curr->next;
	}
	return NULL;
}


SOLDIERINITNODE* FindSoldierInitNodeWithID( UINT16 usID )
{
	PERFORMANCE_MARKER
	SOLDIERINITNODE *curr;
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->pSoldier->ubID == usID )
			return curr;
		curr = curr->next;
	}
	return NULL;
}

void UseEditorOriginalList()
{
	PERFORMANCE_MARKER
	SOLDIERINITNODE *curr;
	gfOriginalList = TRUE;
	gSoldierInitHead = gOriginalSoldierInitListHead;
	curr = gSoldierInitHead;
	if( curr )
	{
		while( curr->next )
			curr = curr->next;
	}
	if( curr )
		gSoldierInitTail = curr;
}

void UseEditorAlternateList()
{
	PERFORMANCE_MARKER
	SOLDIERINITNODE *curr;
	gfOriginalList = FALSE;
	gSoldierInitHead = gAlternateSoldierInitListHead;
	curr = gSoldierInitHead;
	if( curr )
	{
		while( curr->next )
			curr = curr->next;
	}
	if( curr )
		gSoldierInitTail = curr;
}

//Any killed people that used detailed placement information must prevent that from occurring
//again in the future.	Otherwise, the sniper guy with 99 marksmanship could appear again
//if the map was loaded again!
void EvaluateDeathEffectsToSoldierInitList( SOLDIERTYPE *pSoldier )
{
	PERFORMANCE_MARKER
	SOLDIERINITNODE *curr;
	UINT8 ubNodeID;
	curr = gSoldierInitHead;
	ubNodeID = 0;
	if( pSoldier->bTeam == MILITIA_TEAM )
		return;
	while( curr )
	{
		if( curr->pSoldier == pSoldier )
		{ //Matching soldier found
			if( curr->pDetailedPlacement )
			{ //This soldier used detailed placement information, so we must save the
				//node ID into the temp file which signifies that the 

				//RECORD UBNODEID IN TEMP FILE.

				curr->pSoldier = NULL;
				MemFree( curr->pDetailedPlacement );
				curr->pDetailedPlacement = NULL;
				return;
			}
		}
		ubNodeID++;
		curr = curr->next;
	}
}

void RemoveDetailedPlacementInfo( UINT8 ubNodeID )
{
	PERFORMANCE_MARKER
	SOLDIERINITNODE *curr;
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->ubNodeID == ubNodeID )
		{
			if( curr->pDetailedPlacement )
			{
				MemFree( curr->pDetailedPlacement );
				curr->pDetailedPlacement = NULL;
				return;
			}
		}
		curr = curr->next;
	}
}

//For the purpose of keeping track of which soldier belongs to which placement within the game,
//the only way we can do this properly is to save the soldier ID from the list and reconnect the
//soldier pointer whenever we load the game.
BOOLEAN SaveSoldierInitListLinks( HWFILE hfile )
{
	PERFORMANCE_MARKER
	SOLDIERINITNODE *curr;
	UINT32 uiNumBytesWritten;
	UINT8 ubSlots = 0;

	//count the number of soldier init nodes...
	curr = gSoldierInitHead;
	while( curr )
	{
		ubSlots++;
		curr = curr->next;
	}
	//...and save it.
	FileWrite( hfile, &ubSlots, 1, &uiNumBytesWritten );
	if( uiNumBytesWritten != 1 )
	{
		return FALSE;
	}
	//Now, go through each node, and save just the ubSoldierID, if that soldier is alive.
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->pSoldier && !curr->pSoldier->bActive )
		{
			curr->ubSoldierID = 0;
		}
		FileWrite( hfile, &curr->ubNodeID, 1, &uiNumBytesWritten );
		if( uiNumBytesWritten != 1 )
		{
			return FALSE;
		}
		FileWrite( hfile, &curr->ubSoldierID, 1, &uiNumBytesWritten );
		if( uiNumBytesWritten != 1 )
		{
			return FALSE;
		}
		curr = curr->next;
	}
	return TRUE;
}

BOOLEAN LoadSoldierInitListLinks( HWFILE hfile )
{
	PERFORMANCE_MARKER
	UINT32 uiNumBytesRead;
	SOLDIERINITNODE *curr;
	UINT8 ubSlots, ubSoldierID, ubNodeID;
	
	FileRead( hfile, &ubSlots, 1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
	{
		return FALSE;
	}
	while( ubSlots-- )
	{
		FileRead( hfile, &ubNodeID, 1, &uiNumBytesRead );
		if( uiNumBytesRead != 1 )
		{
			return FALSE;
		}
		FileRead( hfile, &ubSoldierID, 1, &uiNumBytesRead );
		if( uiNumBytesRead != 1 )
		{
			return FALSE;
		}

		if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
		{
			curr = gSoldierInitHead;
			while( curr )
			{
				if( curr->ubNodeID == ubNodeID )
				{
					curr->ubSoldierID = ubSoldierID;
					if( ubSoldierID >= gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID && 
							ubSoldierID <= gTacticalStatus.Team[ CREATURE_TEAM ].bLastID ||
							ubSoldierID >= gTacticalStatus.Team[ CIV_TEAM ].bFirstID && 
							ubSoldierID <= gTacticalStatus.Team[ CIV_TEAM ].bLastID )
					{ //only enemies and creatures.
						curr->pSoldier = MercPtrs[ ubSoldierID ];
					}
				}
				curr = curr->next;
			}
		}
	}
	return TRUE;
}

void AddSoldierInitListBloodcats()
{
	PERFORMANCE_MARKER
	SECTORINFO *pSector;
	SOLDIERINITNODE *curr;
	UINT8 ubSectorID;
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddSoldierInitListBloodcats"));

	if( gbWorldSectorZ )
	{
		return; //no bloodcats underground.
	}

	ubSectorID = (UINT8)SECTOR( gWorldSectorX, gWorldSectorY );
	pSector = &SectorInfo[ ubSectorID ];

	if( !pSector->bBloodCatPlacements )
	{ //This map has no bloodcat placements, so don't waste CPU time.
		return;
	}
	
	if( pSector->bBloodCatPlacements )
	{ //We don't yet know the number of bloodcat placements in this sector so
		//count them now, and permanently record it.
		INT8 bBloodCatPlacements = 0;
		curr = gSoldierInitHead;
		while( curr )
		{
			if( curr->pBasicPlacement->bBodyType == BLOODCAT )
			{
				bBloodCatPlacements++;
			}
			curr = curr->next;
		}
		if( bBloodCatPlacements != pSector->bBloodCatPlacements &&
				ubSectorID != SEC_I16 && ubSectorID != SEC_N5 )
		{
			#ifdef JA2BETAVERSION
				CHAR16 str[200];
				swprintf( str, L"Table specifies that there are %d bloodcat placements in sector %c%d, but the map actually has %d bloodcat placements. Map value takes precedence. KM,LC:1",
									pSector->bBloodCatPlacements, gWorldSectorY + 'A' - 1, gWorldSectorX, bBloodCatPlacements );
				DoScreenIndependantMessageBox( str, MSG_BOX_FLAG_OK, NULL );
			#endif
			pSector->bBloodCatPlacements = bBloodCatPlacements;
			pSector->bBloodCats = -1;
			if( !bBloodCatPlacements )
			{
				return;
			}
		}
	}
	if( pSector->bBloodCats > 0 )
	{ //Add them to the world now...
		UINT8 ubNumAdded = 0;
		UINT8 ubMaxNum = (UINT8)pSector->bBloodCats;
		SOLDIERINITNODE *mark;
		UINT8 ubSlotsToFill;
		UINT8 ubSlotsAvailable;
		SOLDIERINITNODE *curr;
		
		//Sort the list in the following manner:
		//-Priority placements first
		//-Basic placements next
		//-Any placements with existing soldiers last (overrides others)
		SortSoldierInitList();

		//Count the current number of soldiers of the specified team
		curr = gSoldierInitHead;
		while( curr )
		{
			if( curr->pBasicPlacement->bBodyType == BLOODCAT && curr->pSoldier )
				ubNumAdded++;	//already one here!
			curr = curr->next;
		}

		curr = gSoldierInitHead;

		//First fill up all of the priority existance slots...
		while( curr && curr->pBasicPlacement->fPriorityExistance && ubNumAdded < ubMaxNum )
		{
			if( curr->pBasicPlacement->bBodyType == BLOODCAT )
			{	
				//Matching team, so add this placement.
				if( AddPlacementToWorld( curr ) )
				{
					ubNumAdded++;
				}
			}
			curr = curr->next;
		}
		if( ubNumAdded == ubMaxNum )
			return;

		//Now count the number of nodes that are basic placements of desired team
		//This information will be used to randomly determine which of these placements
		//will be added based on the number of slots we can still add.
		mark = curr;
		ubSlotsAvailable = 0;
		ubSlotsToFill = ubMaxNum - ubNumAdded;
		while( curr && !curr->pSoldier && ubNumAdded < ubMaxNum )
		{
			if( curr->pBasicPlacement->bBodyType == BLOODCAT )
				ubSlotsAvailable++;
			curr = curr->next;
		}

		//we now have the number, so compared it to the num we can add, and determine how we will
		//randomly determine which nodes to add.
		if( !ubSlotsAvailable )
		{	//There aren't any basic placements of desired team, so exit.
			return;
		}
		curr = mark;
		//while we have a list, with no active soldiers, the num added is less than the max num requested, and
		//we have slots available, process the list to add new soldiers.
		while( curr && !curr->pSoldier && ubNumAdded < ubMaxNum && ubSlotsAvailable )
		{
			if( curr->pBasicPlacement->bBodyType == BLOODCAT )
			{
				if( ubSlotsAvailable <= ubSlotsToFill || Random( ubSlotsAvailable ) < ubSlotsToFill )
				{
					//found matching team, so add this soldier to the game.
					if( AddPlacementToWorld( curr ) )
					{
						ubNumAdded++;
					}
					else
					{
						//if it fails to create the soldier, it is likely that it is because the slots in the tactical 
						//engine are already full.	Besides, the strategic AI shouldn't be trying to fill a map with
						//more than the maximum allowable soldiers of team.	All teams can have a max of 32 individuals,
						//except for the player which is 20.	Players aren't processed in this list anyway.
						return;
					}
					ubSlotsToFill--;
				}
				ubSlotsAvailable--;
				//With the decrementing of the slot vars in this manner, the chances increase so that all slots
				//will be full by the time the end of the list comes up.
			}
			curr = curr->next;
		}
		return;
	}
}


SOLDIERINITNODE * FindSoldierInitListNodeByProfile( UINT8 ubProfile )
{
	PERFORMANCE_MARKER
	SOLDIERINITNODE * curr;

	curr = gSoldierInitHead;

	while( curr )
	{
		if ( curr->pDetailedPlacement && curr->pDetailedPlacement->ubProfile == ubProfile )
		{
			return( curr );
		}
		curr = curr->next;
	}
	return( NULL );
}

//This is the code that loops through the profiles starting at the RPCs, and adds them using strategic insertion
//information, and not editor placements.	The key flag involved for doing it this way is the gMercProfiles[i].fUseProfileInsertionInfo.
void AddProfilesUsingProfileInsertionData()
{
	PERFORMANCE_MARKER
	INT32 i;
	SOLDIERTYPE *pSoldier;
	SOLDIERINITNODE * curr;

	for( i = FIRST_RPC; i < ( PROF_HUMMER ); i++ )
	{
		//Perform various checks to make sure the soldier is actually in the same sector, alive, and so on.
		//More importantly, the flag to use profile insertion data must be set.
		if( gMercProfiles[ i ].sSectorX != gWorldSectorX ||
				gMercProfiles[ i ].sSectorY != gWorldSectorY ||
				gMercProfiles[ i ].bSectorZ != gbWorldSectorZ ||
				gMercProfiles[ i ].ubMiscFlags & PROFILE_MISC_FLAG_RECRUITED ||
				gMercProfiles[ i ].ubMiscFlags & PROFILE_MISC_FLAG_EPCACTIVE ||
//			gMercProfiles[ i ].ubMiscFlags2 & PROFILE_MISC_FLAG2_DONT_ADD_TO_SECTOR ||
				!gMercProfiles[ i ].bLife ||
				!gMercProfiles[ i ].fUseProfileInsertionInfo 
			)
		{ //Don't add, so skip to the next soldier.
			continue;
		}
		pSoldier = FindSoldierByProfileID( (UINT8)i, FALSE );
		if( !pSoldier )
		{ //Create a new soldier, as this one doesn't exist
			SOLDIERCREATE_STRUCT		MercCreateStruct;
			UINT8									ubID;

			//Set up the create struct so that we can properly create the profile soldier.
			MercCreateStruct.initialize();
			MercCreateStruct.bTeam						= CIV_TEAM;
			MercCreateStruct.ubProfile				= (UINT8)i;
			MercCreateStruct.sSectorX					= gWorldSectorX;
			MercCreateStruct.sSectorY					= gWorldSectorY;
			MercCreateStruct.bSectorZ					= gbWorldSectorZ;

			pSoldier = TacticalCreateSoldier( &MercCreateStruct, &ubID );
		}
		if ( pSoldier )
		{ //Now, insert the soldier.
			pSoldier->ubStrategicInsertionCode = gMercProfiles[ i ].ubStrategicInsertionCode;
			pSoldier->usStrategicInsertionData = gMercProfiles[ i ].usStrategicInsertionData;
			UpdateMercInSector( pSoldier, gWorldSectorX, gWorldSectorY, gbWorldSectorZ );
			// CJC: Note well that unless an error occurs, UpdateMercInSector calls 
			// AddSoldierToSector
			// AddSoldierToSector( pSoldier->ubID );

			// check action ID values
			if ( gMercProfiles[ i ].ubQuoteRecord )
			{
				pSoldier->ubQuoteRecord = gMercProfiles[ i ].ubQuoteRecord;
				pSoldier->ubQuoteActionID = gMercProfiles[ i ].ubQuoteActionID;
				if ( pSoldier->ubQuoteActionID == QUOTE_ACTION_ID_CHECKFORDEST )
				{
					// gridno will have been changed to destination... so we're there...
					NPCReachedDestination( pSoldier, FALSE );
				}
			}

			// make sure this person's pointer is set properly in the init list
			curr = FindSoldierInitListNodeByProfile( pSoldier->ubProfile );
			if ( curr )
			{
				curr->pSoldier = pSoldier;
				curr->ubSoldierID = pSoldier->ubID;
				// also connect schedules here
				if ( curr->pDetailedPlacement->ubScheduleID != 0 )
				{
					SCHEDULENODE * pSchedule = GetSchedule( curr->pDetailedPlacement->ubScheduleID );
					if ( pSchedule )
					{
						pSchedule->ubSoldierID = pSoldier->ubID;
						pSoldier->ubScheduleID = curr->pDetailedPlacement->ubScheduleID;
					}				
				}
			}
		}
	}
}

void AddProfilesNotUsingProfileInsertionData()
{
	PERFORMANCE_MARKER
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("AddProfilesNotUsingProfileInsertionData"));
	SOLDIERINITNODE *curr;
	//Count the current number of soldiers of the specified team
	curr = gSoldierInitHead;
	while( curr )
	{
		if( !curr->pSoldier && 
				curr->pBasicPlacement->bTeam == CIV_TEAM && 
				curr->pDetailedPlacement &&
				curr->pDetailedPlacement->ubProfile != NO_PROFILE &&
				!gMercProfiles[ curr->pDetailedPlacement->ubProfile ].fUseProfileInsertionInfo &&
				gMercProfiles[ curr->pDetailedPlacement->ubProfile ].bLife )
		{
			AddPlacementToWorld( curr );
		}
		curr = curr->next;
	}
}

#ifdef JA2BETAVERSION
extern void ErrorDetectedInSaveCallback( UINT8 bValue );

BOOLEAN ValidateSoldierInitLinks( UINT8 ubCode )
{
	PERFORMANCE_MARKER
	SOLDIERINITNODE *curr;
	UINT32 uiNumInvalids = 0;
	CHAR16 str[512];
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->pSoldier )
		{
			if( curr->pSoldier->ubID < 20 && !MercPtrs[ curr->pSoldier->ubID ]->bActive )
			{
				uiNumInvalids++;
			}
		}
		curr = curr->next;
	}
	if( uiNumInvalids || ubCode == 4)
	{
		switch( ubCode )
		{
			case 1: //loading save
				swprintf( str, L"Error detected in save file WHILE LOADING.  Please send save and text files associated with save to Kris and Dave."
											L"  After doing so, go back into the game and immediately resave the game which will fix the problem."
											L"  This is the bug responsible for mercs disappearing.  Be prepared to answer lots of questions..." );
				DoSaveLoadMessageBox( MSG_BOX_BASIC_STYLE, str, SAVE_LOAD_SCREEN, MSG_BOX_FLAG_OK, ErrorDetectedInSaveCallback );
				break;
			case 2: //saving game
				//swprintf( str, L"Error detected WHILE SAVING file.  Please send save and text files associated with save to Kris and Dave."
				//							L"  After doing so, go back into the game and try reloading the new save and saving it again which *could* fix the problem."
				//							L"  This is the bug responsible for mercs disappearing.  Be prepared to answer lots of questions..." );
				//if( guiPreviousOptionScreen == MAP_SCREEN )
				//	DoMapMessageBox( MSG_BOX_BASIC_STYLE, str, MAP_SCREEN, MSG_BOX_FLAG_OK, NULL );
				//else
				//	DoMessageBox( MSG_BOX_BASIC_STYLE, str, GAME_SCREEN, MSG_BOX_FLAG_OK, NULL, NULL );
				break;
			case 3: //entering sector using temp files (before fade in)
				gfDoDialogOnceGameScreenFadesIn = TRUE;
				break;
			case 4: //after fade in
				gfDoDialogOnceGameScreenFadesIn = FALSE;
				swprintf( str, L"Error detected while entering sector USING TEMP FILES.  Please send previous save and text files associated with save to Kris and Dave."
											L"  After doing so, go back into the game and saving the game, reloading it, and saving it again *could* fix it."
											L"  This is the bug responsible for mercs disappearing.  Be prepared to answer lots of questions..." );
				DoMessageBox( MSG_BOX_BASIC_STYLE, str, GAME_SCREEN, MSG_BOX_FLAG_OK, NULL, NULL );
				break;
		}
		return FALSE;
	}
	return TRUE;
}
#endif //betaversion error checking functions


BOOLEAN NewWayOfLoadingEnemySoldierInitListLinks( HWFILE hfile )
{
	PERFORMANCE_MARKER
	UINT32 uiNumBytesRead;
	SOLDIERINITNODE *curr;
	UINT8 ubSlots, ubSoldierID, ubNodeID;
	
	FileRead( hfile, &ubSlots, 1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
	{
		return FALSE;
	}
	while( ubSlots-- )
	{
		FileRead( hfile, &ubNodeID, 1, &uiNumBytesRead );
		if( uiNumBytesRead != 1 )
		{
			return FALSE;
		}
		FileRead( hfile, &ubSoldierID, 1, &uiNumBytesRead );
		if( uiNumBytesRead != 1 )
		{
			return FALSE;
		}

		if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
		{
			curr = gSoldierInitHead;
			while( curr )
			{
				if( curr->ubNodeID == ubNodeID )
				{
					curr->ubSoldierID = ubSoldierID;
					if( ubSoldierID >= gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID && 
							ubSoldierID <= gTacticalStatus.Team[ CREATURE_TEAM ].bLastID )
					{ //only enemies and creatures.
						curr->pSoldier = MercPtrs[ ubSoldierID ];
					}
				}
				curr = curr->next;
			}
		}
	}
	return TRUE;
}


BOOLEAN NewWayOfLoadingCivilianInitListLinks( HWFILE hfile )
{
	PERFORMANCE_MARKER
	UINT32 uiNumBytesRead;
	SOLDIERINITNODE *curr;
	UINT8 ubSlots, ubSoldierID, ubNodeID;
	
	FileRead( hfile, &ubSlots, 1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
	{
		return FALSE;
	}
	while( ubSlots-- )
	{
		FileRead( hfile, &ubNodeID, 1, &uiNumBytesRead );
		if( uiNumBytesRead != 1 )
		{
			return FALSE;
		}
		FileRead( hfile, &ubSoldierID, 1, &uiNumBytesRead );
		if( uiNumBytesRead != 1 )
		{
			return FALSE;
		}

		if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
		{
			curr = gSoldierInitHead;
			while( curr )
			{
				if( curr->ubNodeID == ubNodeID )
				{
					curr->ubSoldierID = ubSoldierID;
					if( ubSoldierID >= gTacticalStatus.Team[ CIV_TEAM ].bFirstID && 
							ubSoldierID <= gTacticalStatus.Team[ CIV_TEAM ].bLastID )
					{ //only enemies and creatures.
						curr->pSoldier = MercPtrs[ ubSoldierID ];
					}
				}
				curr = curr->next;
			}
		}
	}
	return( TRUE );
}


BOOLEAN LookAtButDontProcessEnemySoldierInitListLinks( HWFILE hfile )
{
	PERFORMANCE_MARKER
	UINT32 uiNumBytesRead;
	SOLDIERINITNODE *curr;
	UINT8 ubSlots, ubSoldierID, ubNodeID;
	
	FileRead( hfile, &ubSlots, 1, &uiNumBytesRead );
	if( uiNumBytesRead != 1 )
	{
		return FALSE;
	}
	while( ubSlots-- )
	{
		FileRead( hfile, &ubNodeID, 1, &uiNumBytesRead );
		if( uiNumBytesRead != 1 )
		{
			return FALSE;
		}
		FileRead( hfile, &ubSoldierID, 1, &uiNumBytesRead );
		if( uiNumBytesRead != 1 )
		{
			return FALSE;
		}

		if( gTacticalStatus.uiFlags & LOADING_SAVED_GAME )
		{
			curr = gSoldierInitHead;
			while( curr )
			{
				if( curr->ubNodeID == ubNodeID )
				{
					curr->ubSoldierID = ubSoldierID;
					if( ubSoldierID >= gTacticalStatus.Team[ ENEMY_TEAM ].bFirstID && 
							ubSoldierID <= gTacticalStatus.Team[ CREATURE_TEAM ].bLastID )
					{ //only enemies and creatures.
						curr->pSoldier = MercPtrs[ ubSoldierID ];
					}
				}
				curr = curr->next;
			}
		}
	}
	return TRUE;
}

void StripEnemyDetailedPlacementsIfSectorWasPlayerLiberated()
{
	PERFORMANCE_MARKER
	SECTORINFO *pSector;
	SOLDIERINITNODE *curr;
	
	if( !gfWorldLoaded || gbWorldSectorZ )
	{ //No world loaded or underground.	Underground sectors don't matter
		//seeing enemies (not creatures) never rejuvenate underground.
		return;
	}

	pSector = &SectorInfo[ SECTOR( gWorldSectorX, gWorldSectorY ) ];

	if( !pSector->uiTimeLastPlayerLiberated )
	{ //The player has never owned the sector.
		return;
	}

	//The player has owned the sector at one point.	By stripping all of the detailed placements, only basic 
	//placements will remain.	This prevents tanks and "specially detailed" enemies from coming back.
	curr = gSoldierInitHead;
	while( curr )
	{
		if( curr->pDetailedPlacement )
		{
			if( curr->pBasicPlacement->bTeam == ENEMY_TEAM )
			{
				// pDetailedPlacement has been C++'d
				//MemFree( curr->pDetailedPlacement );
				delete curr->pDetailedPlacement;
				curr->pDetailedPlacement = NULL;
				curr->pBasicPlacement->fDetailedPlacement = FALSE;
				curr->pBasicPlacement->fPriorityExistance = FALSE;
				curr->pBasicPlacement->bBodyType = -1;
				RandomizeRelativeLevel( &( curr->pBasicPlacement->bRelativeAttributeLevel ), curr->pBasicPlacement->ubSoldierClass );
				RandomizeRelativeLevel( &( curr->pBasicPlacement->bRelativeEquipmentLevel ), curr->pBasicPlacement->ubSoldierClass );
			}
		}
		curr = curr->next;
	}

}

//////// 
////////	For militia squad attack!
////////

#define CENTRAL_GRIDNO 13202
#define CENTRAL_RADIUS 30

void AddSoldierInitListMilitiaOnEdge( UINT8 ubStrategicInsertionCode, UINT8 ubNumGreen, UINT8 ubNumReg, UINT8 ubNumElites )
{
	PERFORMANCE_MARKER
	SOLDIERTYPE *pSoldier;
	MAPEDGEPOINTINFO MapEdgepointInfo;
	UINT8 ubCurrSlot;
	UINT8 ubTotalSoldiers;
	UINT8 bDesiredDirection=0;

	ubTotalSoldiers = ubNumGreen + ubNumReg + ubNumElites;

	// WANNE: If we have no militia soldiers->exit!
	if (ubTotalSoldiers == 0)
		return;

	switch( ubStrategicInsertionCode )
	{
		case INSERTION_CODE_NORTH:	bDesiredDirection = SOUTHEAST;										break;
		case INSERTION_CODE_EAST:		bDesiredDirection = SOUTHWEST;										break;
		case INSERTION_CODE_SOUTH:	bDesiredDirection = NORTHWEST;										break;
		case INSERTION_CODE_WEST:		bDesiredDirection = NORTHEAST;										break;
		default:	AssertMsg( 0, "Illegal direction passed to AddSoldierInitListMilitiaOnEdge()" );	break;
	}
	#ifdef JA2TESTVERSION
		ScreenMsg( FONT_RED, MSG_INTERFACE, L"Militia reinforcements have arrived!	(%d admins, %d troops, %d elite)", ubNumGreen, ubNumReg, ubNumElites );
	#endif

	ChooseMapEdgepoints( &MapEdgepointInfo, ubStrategicInsertionCode, (UINT8)(ubNumGreen + ubNumReg + ubNumElites) );
	ubCurrSlot = 0;
	while( ubTotalSoldiers )
	{
		if( ubNumElites && Random( ubTotalSoldiers ) < ubNumElites )
		{
			ubNumElites--;
			ubTotalSoldiers--;
			pSoldier = TacticalCreateMilitia(SOLDIER_CLASS_ELITE_MILITIA);

			// Lesh: if pSoldier is NULL then no slot for a new men or other problems
			//		it better to leave this function is such case
			if ( !pSoldier )
				return;

			pSoldier->ubInsertionDirection = bDesiredDirection;

			// Lesh: militia came from another sector can't reach opposite side of map in case of battle there
			//		they are often stop at a half way. trying to fix this
			if ( gTacticalStatus.Team[ MILITIA_TEAM ].bAwareOfOpposition )
			{
				pSoldier->aiData.bOrders = SEEKENEMY;
				pSoldier->aiData.bAlertStatus = STATUS_RED;
			}
			else
			{
				pSoldier->aiData.bOrders = ONGUARD;
				pSoldier->aiData.bAlertStatus = STATUS_YELLOW;
				pSoldier->aiData.sNoiseGridno = (INT16)(CENTRAL_GRIDNO + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) * WORLD_COLS);
				pSoldier->aiData.ubNoiseVolume = MAX_MISC_NOISE_DURATION;
			}
			
			//Setup the position
			if( ubCurrSlot < MapEdgepointInfo.ubNumPoints )
			{ //using an edgepoint
				pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
				pSoldier->usStrategicInsertionData = MapEdgepointInfo.sGridNo[ ubCurrSlot++ ];
			}
			else 
			{ //no edgepoints left, so put him at the entrypoint.
				pSoldier->ubStrategicInsertionCode = ubStrategicInsertionCode;
			}
			UpdateMercInSector( pSoldier, gWorldSectorX, gWorldSectorY, 0 );
		}
		else if( ubNumReg && (UINT8)Random( ubTotalSoldiers ) < (UINT8)(ubNumElites + ubNumReg) )
		{
			ubNumReg--;
			ubTotalSoldiers--;
			pSoldier = TacticalCreateMilitia(SOLDIER_CLASS_REG_MILITIA);

			// Lesh: if pSoldier is NULL then no slot for a new men or other problems
			//		it better to leave this function is such case
			if ( !pSoldier )
				return;

			pSoldier->ubInsertionDirection = bDesiredDirection;

			// Lesh: militia came from another sector can't reach opposite side of map in case of battle there
			//		they are often stop at a half way. trying to fix this
			if ( gTacticalStatus.Team[ MILITIA_TEAM ].bAwareOfOpposition )
			{
				pSoldier->aiData.bOrders = SEEKENEMY;
				pSoldier->aiData.bAlertStatus = STATUS_RED;
			}
			else
			{
				pSoldier->aiData.bOrders = ONGUARD;
				pSoldier->aiData.bAlertStatus = STATUS_YELLOW;
				pSoldier->aiData.sNoiseGridno = (INT16)(CENTRAL_GRIDNO + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) * WORLD_COLS);
				pSoldier->aiData.ubNoiseVolume = MAX_MISC_NOISE_DURATION;
			}

			//Setup the position
			if( ubCurrSlot < MapEdgepointInfo.ubNumPoints )
			{ //using an edgepoint
				pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
				pSoldier->usStrategicInsertionData = MapEdgepointInfo.sGridNo[ ubCurrSlot++ ];
			}
			else 
			{ //no edgepoints left, so put him at the entrypoint.
				pSoldier->ubStrategicInsertionCode = ubStrategicInsertionCode;
			}
			UpdateMercInSector( pSoldier, gWorldSectorX, gWorldSectorY, 0 );
		}
		else if( ubNumGreen && (UINT8)Random( ubTotalSoldiers ) < (UINT8)(ubNumElites + ubNumReg + ubNumGreen) )
		{
			ubNumGreen--;
			ubTotalSoldiers--;
			pSoldier = TacticalCreateMilitia(SOLDIER_CLASS_GREEN_MILITIA);

			// Lesh: if pSoldier is NULL then no slot for a new men or other problems
			//		it better to leave this function is such case
			if ( !pSoldier )
				return;

			pSoldier->ubInsertionDirection = bDesiredDirection;

			// Lesh: militia came from another sector can't reach opposite side of map in case of battle there
			//		they are often stop at a half way. trying to fix this
			if ( gTacticalStatus.Team[ MILITIA_TEAM ].bAwareOfOpposition )
			{
				pSoldier->aiData.bOrders = SEEKENEMY;
				pSoldier->aiData.bAlertStatus = STATUS_RED;
			}
			else
			{
				pSoldier->aiData.bOrders = ONGUARD;
				pSoldier->aiData.bAlertStatus = STATUS_YELLOW;
				pSoldier->aiData.sNoiseGridno = (INT16)(CENTRAL_GRIDNO + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) + ( Random( CENTRAL_RADIUS * 2 + 1 ) - CENTRAL_RADIUS ) * WORLD_COLS);
				pSoldier->aiData.ubNoiseVolume = MAX_MISC_NOISE_DURATION;
			}

//			if ( GetTimeOfDayAmbientLightLevel() < NORMAL_LIGHTLEVEL_DAY + 2 )
//				gTacticalStatus.Team[ ENEMY_TEAM ].bAwareOfOpposition = TRUE;
//			gTacticalStatus.Team[ MILITIA_TEAM ].bAwareOfOpposition = TRUE;
			
			//Setup the position
			if( ubCurrSlot < MapEdgepointInfo.ubNumPoints )
			{ //using an edgepoint
				pSoldier->ubStrategicInsertionCode = INSERTION_CODE_GRIDNO;
				pSoldier->usStrategicInsertionData = MapEdgepointInfo.sGridNo[ ubCurrSlot++ ];
			}
			else 
			{ //no edgepoints left, so put him at the entrypoint.
				pSoldier->ubStrategicInsertionCode = ubStrategicInsertionCode;
			}
			UpdateMercInSector( pSoldier, gWorldSectorX, gWorldSectorY, 0 );
		}
	}
}
