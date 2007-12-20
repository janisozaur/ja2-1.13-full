#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
	#include "strategic.h"
#else
	#include "builddefines.h"
	#include <stdio.h>
	#include <string.h>
	#include "wcheck.h"
	#include "stdlib.h"
	#include "debug.h"
	#include "math.h"
	#include "worlddef.h"
	#include "worldman.h"
	#include "renderworld.h"
	#include "Assignments.h"
	#include "Soldier Control.h"
	#include "Animation Control.h"
	#include "Animation Data.h"
	#include "Isometric Utils.h"
	#include "Event Pump.h"
	#include "Timer Control.h"
	#include "Render Fun.h" 
	#include "Render Dirty.h"
	#include "mousesystem.h"
	#include "interface.h"
	#include "sysutil.h"
	#include "FileMan.h"
	#include "points.h"
	#include "Random.h"
	#include "ai.h"
	#include "Interactive Tiles.h"
	#include "soldier ani.h"
	#include "english.h"
	#include "overhead.h"
	#include "Soldier Profile.h"
	#include "Game Clock.h"
	#include "soldier create.h"
	#include "Merc Hiring.h"
	#include "Game Event Hook.h"
	#include "message.h"
	#include "strategicmap.h"
	#include "strategic.h"
	#include "items.h"
	#include "Soldier Add.h"
	#include "History.h"
	#include "Squads.h"
	#include "Strategic Merc Handler.h"
	#include "Dialogue Control.h"
	#include "Map Screen Interface.h"
	#include "Map Screen Interface Map.h"
	#include "screenids.h"
	#include "jascreens.h"
	#include "text.h"
	#include "Merc Contract.h"
	#include "LaptopSave.h"
	#include "personnel.h"
	#include "Auto Resolve.h"
	#include "Map Screen Interface Bottom.h"
	#include "Quests.h"
	#include "GameSettings.h"
#endif

#include "MessageIdentifiers.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>

#include "tactical placement gui.h"


#pragma pack(1)


#include "Types.h"
#include "connect.h"
#include "message.h"
#include "Event pump.h"
#include "Soldier Init List.h"
#include "Overhead.h"
#include "weapons.h"
#include "Merc Hiring.h"
#include "soldier profile.h"

#include"laptop.h"

extern INT8 SquadMovementGroups[ ];

// test_func2 issued by key "1" in map screen and "a" in tactical...

//void test_func2 ( void )
//{
//	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"test_func2" );
//
//	//ChangeSoldiersAssignment( pSoldier, 0 );
//	//AddPlayerToGroup( SquadMovementGroups[ 0 ], pSoldier  );
//	UINT8 i;
//	for ( i=0; i < 20; i++)
//	{
//	SOLDIERTYPE *pSoldier = MercPtrs[ i ];
//		if( pSoldier->bActive )
//		{
//			AddCharacterToAnySquad( pSoldier );
//			AddSoldierToSector( i );
//		}
//	}
//
//
//}

void test_func2 (void)//now bound to "5"
{
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"test_func2" );
}




//void test_func2 ( void )
//{
//	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"test_func2" );
//	INT8					cnt;
//	//INT16 cnt;
//	SOLDIERTYPE		*pSoldier;
//	INT16					bLastTeamID;
//	UINT8					ubCount=0;
//	UINT8  res;
//
//	gbPlayerNum=0;
//	gbPlayerNum=1;
//	gbPlayerNum=2;
//	gbPlayerNum=3;
//	gbPlayerNum=4;
//	gbPlayerNum=5;
//	gbPlayerNum=6;
//	gbPlayerNum=7;
//	gbPlayerNum=8;
//	gbPlayerNum=9;
//
//	// Set locator to first merc
//	cnt = gTacticalStatus.Team[ gbPlayerNum ].bFirstID;
//	bLastTeamID = gTacticalStatus.Team[ gbPlayerNum ].bLastID;
//
//  for ( pSoldier = MercPtrs[ cnt ]; cnt <= bLastTeamID; cnt++,pSoldier++)
//	{	
//		//if the is active, and is not a vehicle
//		if( pSoldier->bActive && !( pSoldier->uiStatusFlags & SOLDIER_VEHICLE ) )
//		{
//			ubCount++;
//		}
//	}
//
//	res= ubCount ;
//}


//void test_func2 ( void )
//{
//
//	// Create guy # X //taken from function: QuickCreateProfileMerc ()
//	SOLDIERTYPE	*pSoldier;
//	SOLDIERCREATE_STRUCT		MercCreateStruct;
//	INT16										sWorldX, sWorldY, sSectorX, sSectorY, sGridX, sGridY;
//	UINT8									ubID;
//	UINT16 usMapPos;
//	INT8 bTeam = LAN_TEAM_ONE;
//	UINT8 ubProfileID = 9;
//
//	if ( GetMouseXY( &sGridX, &sGridY ) )
//	{
//		usMapPos = MAPROWCOLTOPOS( sGridY, sGridX );
//		// Get Grid Coordinates of mouse
//		if ( GetMouseWorldCoordsInCenter( &sWorldX, &sWorldY ) )
//		{		 
//			GetCurrentWorldSector( &sSectorX, &sSectorY );
//
//
//			memset( &MercCreateStruct, 0, sizeof( MercCreateStruct ) );
//			MercCreateStruct.bTeam				= bTeam;
//			MercCreateStruct.ubProfile		= ubProfileID;
//			MercCreateStruct.sSectorX			= sSectorX;
//			MercCreateStruct.sSectorY			= sSectorY;
//			MercCreateStruct.bSectorZ			= gbWorldSectorZ;
//			MercCreateStruct.sInsertionGridNo		= usMapPos;
//
//			RandomizeNewSoldierStats( &MercCreateStruct );
//
//			if ( TacticalCreateSoldier( &MercCreateStruct, &ubID ) )
//			{
//				AddSoldierToSector( ubID );
//
//				// So we can see them!
//				AllTeamsLookForAll(NO_INTERRUPTS);
//				pSoldier = &Menptr[ubID];
//			}
//		}
//	}
//}

//void test_func2 ( void )
//{
//
//
//	SOLDIERTYPE	*pSoldier;
//	UINT8		iNewIndex;
//	UINT8		ubCount=0;
//
//	
//	SOLDIERCREATE_STRUCT		MercCreateStruct;
//	BOOLEAN fReturn = FALSE;
//	
//	
//
//	memset( &MercCreateStruct, 0, sizeof( MercCreateStruct ) );
//	MercCreateStruct.ubProfile						= 9;
//	MercCreateStruct.fPlayerMerc					= 0;
//	MercCreateStruct.sSectorX							= 9;
//	MercCreateStruct.sSectorY							= 1;
//	MercCreateStruct.bSectorZ							= 0;
//	MercCreateStruct.bTeam								= MILITIA_TEAM;
//	MercCreateStruct.fCopyProfileItemsOver= 1;
//
//	TacticalCreateSoldier( &MercCreateStruct, &iNewIndex ) ;
//
//		AddSoldierToSector( iNewIndex );
//
//				// So we can see them!
//		AllTeamsLookForAll(NO_INTERRUPTS);
//		pSoldier = &Menptr[iNewIndex];
//
//	/*ChangeSoldiersAssignment( pSoldier, 0 );*/
//	//AddPlayerToGroup( SquadMovementGroups[ 0 ], pSoldier  );
//
//}
//void test (void)
//{
//	SetLaptopExitScreen( MAP_SCREEN );
//	SetPendingNewScreen( MAP_SCREEN );
//}