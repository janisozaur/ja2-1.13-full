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


unsigned char GetPacketIdentifier(Packet *p);
unsigned char packetIdentifier;



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

#include "laptop.h"


extern INT8 SquadMovementGroups[ ];
RakPeerInterface *client;

typedef struct
{
	UINT8		ubProfileID;
	INT16		iTotalContractLength;
	BOOLEAN fCopyProfileItemsOver;
	INT8 bTeam;

} send_hire_struct;

typedef struct
{
	UINT8		usSoldierID;
	FLOAT dNewXPos;
	FLOAT dNewYPos;

} gui_pos;

typedef struct
{
	UINT8		usSoldierID;
	INT16	usNewDirection;

} gui_dir;

typedef struct
{	
	UINT8 tsnetbTeam;
	UINT8 tsubNextTeam;
} turn_struct;

typedef struct
		{	
		UINT8 aaa;
		BOOLEAN bbb;
		} temp;


UINT8 netbTeam;
UINT8	ubID_prefix;

 bool is_connected=false;
 bool is_connecting=false;
 bool is_client=false;
 bool is_server=false;
 bool is_networked=false;

 char CLIENT_NUM[30];
 char SECT_EDGE[30];

 char SERVER_IP[30] ;
 char SERVER_PORT[30];

 int ENEMY_ENABLED;
 int CREATURE_ENABLED;
 int MILITIA_ENABLED;
 int CIV_ENABLED;

 int SAME_MERC;
 int PLAYER_BSIDE;

INT16 NET_DIVISOR;

UINT16 crate_usMapPos;	

INT16	crate_sGridX, crate_sGridY;


//*****************
//RPC sends and recieves:
//********************


void send_path (  SOLDIERTYPE *pSoldier, UINT16 sDestGridNo, UINT16 usMovementAnim, BOOLEAN fFromUI, BOOLEAN fForceRestartAnim  )
{
	if(pSoldier->ubID < 124)
	{
		//ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"Sending RPC..." );
		EV_S_GETNEWPATH	SGetNewPath;


		

			SGetNewPath.usSoldierID = (pSoldier->ubID)+ubID_prefix;
		//SGetNewPath.usSoldierID				= pSoldier->ubID;
		SGetNewPath.sDestGridNo				= sDestGridNo;
		SGetNewPath.usMovementAnim		= usMovementAnim;
		SGetNewPath.uiUniqueId = pSoldier -> uiUniqueSoldierIdValue;

		
		client->RPC("sendPATH",(const char*)&SGetNewPath, (int)sizeof(EV_S_GETNEWPATH)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}

}

void recievePATH(RPCParameters *rpcParameters)
{

		//ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"Recieving RPC..." );
					
		EV_S_GETNEWPATH* SGetNewPath = (EV_S_GETNEWPATH*)rpcParameters->input;

		SOLDIERTYPE *pSoldier = MercPtrs[ SGetNewPath->usSoldierID ];
	
		SendGetNewSoldierPathEvent( pSoldier, SGetNewPath->sDestGridNo, SGetNewPath->usMovementAnim );	


		//********* implemented using event pump system ... :)
}


void send_stance ( SOLDIERTYPE *pSoldier, UINT8 ubDesiredStance )

{
	if(pSoldier->ubID < 124)
	{
		EV_S_CHANGESTANCE			SChangeStance;

		SChangeStance.ubNewStance   = ubDesiredStance;
		//SChangeStance.usSoldierID  = pSoldier->ubID;
		

			SChangeStance.usSoldierID = (pSoldier->ubID)+ubID_prefix;

		SChangeStance.sXPos				= pSoldier->sX;
		SChangeStance.sYPos				= pSoldier->sY;
		SChangeStance.uiUniqueId = pSoldier -> uiUniqueSoldierIdValue;

	
		client->RPC("sendSTANCE",(const char*)&SChangeStance, (int)sizeof(EV_S_CHANGESTANCE)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
}


void recieveSTANCE(RPCParameters *rpcParameters)
{


		EV_S_CHANGESTANCE* SChangeStance = (EV_S_CHANGESTANCE*)rpcParameters->input;
	
		SOLDIERTYPE *pSoldier = MercPtrs[ SChangeStance->usSoldierID ];
	
		ChangeSoldierStance( pSoldier, SChangeStance->ubNewStance );

		//SendChangeSoldierStanceEvent( pSoldier, SChangeStance->ubNewStance );
		//AddGameEvent( S_CHANGESTANCE, 0, &SChangeStance );
		//********* implemented using event pump system ... :)
}



void send_dir ( SOLDIERTYPE *pSoldier, UINT16 usDesiredDirection )

{
	if((is_server && pSoldier->ubID < 124) || (!is_server && pSoldier->ubID < 20))
	{

		EV_S_SETDESIREDDIRECTION	SSetDesiredDirection;

		//SSetDesiredDirection.usSoldierID = pSoldier->ubID;
			

		if(pSoldier->ubID < 20)
			SSetDesiredDirection.usSoldierID = (pSoldier->ubID)+ubID_prefix;
		else
			SSetDesiredDirection.usSoldierID = pSoldier->ubID;

	

		SSetDesiredDirection.usDesiredDirection = usDesiredDirection;
		SSetDesiredDirection.uiUniqueId = pSoldier -> uiUniqueSoldierIdValue;

		client->RPC("sendDIR",(const char*)&SSetDesiredDirection, (int)sizeof(EV_S_SETDESIREDDIRECTION)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
}


void recieveDIR(RPCParameters *rpcParameters)
{

		EV_S_SETDESIREDDIRECTION* SSetDesiredDirection = (EV_S_SETDESIREDDIRECTION*)rpcParameters->input;			
			

		SOLDIERTYPE *pSoldier = MercPtrs[ SSetDesiredDirection->usSoldierID ];

		EVENT_SetSoldierDesiredDirection( pSoldier, SSetDesiredDirection->usDesiredDirection );

		//AddGameEvent( S_SETDESIREDDIRECTION, 0, &SSetDesiredDirection );
		//********* implemented using event pump system ... :)
}

void send_fire( SOLDIERTYPE *pSoldier, INT16 sTargetGridNo )
{
	if(pSoldier->ubID < 124)
	{
	
	EV_S_BEGINFIREWEAPON SBeginFireWeapon;


		

		SBeginFireWeapon.usSoldierID = (pSoldier->ubID)+ubID_prefix;

	SBeginFireWeapon.sTargetGridNo = sTargetGridNo;
	SBeginFireWeapon.bTargetLevel = pSoldier->bTargetLevel;
	SBeginFireWeapon.bTargetCubeLevel = pSoldier->bTargetCubeLevel;
	SBeginFireWeapon.uiUniqueId = pSoldier->usAttackingWeapon;
		

	client->RPC("sendFIRE",(const char*)&SBeginFireWeapon, (int)sizeof(EV_S_BEGINFIREWEAPON)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
}

void recieveFIRE(RPCParameters *rpcParameters)
{
		
		EV_S_BEGINFIREWEAPON* SBeginFireWeapon = (EV_S_BEGINFIREWEAPON*)rpcParameters->input;
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"SendBeginFireWeaponEvent" );

		SOLDIERTYPE *pSoldier = MercPtrs[ SBeginFireWeapon->usSoldierID ];

		pSoldier->sTargetGridNo = SBeginFireWeapon->sTargetGridNo;
		pSoldier->bTargetLevel = SBeginFireWeapon->bTargetLevel;
		pSoldier->bTargetCubeLevel = SBeginFireWeapon->bTargetCubeLevel;
		pSoldier->usAttackingWeapon = SBeginFireWeapon->uiUniqueId; //cheap hack to pass wep id. 
					
		
		SendBeginFireWeaponEvent( pSoldier, SBeginFireWeapon->sTargetGridNo );
}



void send_hit( UINT16 usSoldierID, UINT16 usWeaponIndex, INT16 sDamage, INT16 sBreathLoss, UINT16 usDirection, INT16 sXPos, INT16 sYPos, INT16 sZPos, INT16 sRange , UINT8 ubAttackerID, BOOLEAN fHit, UINT8 ubSpecial, UINT8 ubHitLocation )
{

	if((!is_server && ubAttackerID < 20) || is_server) 
		
	{
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"sendHIT" );
		/*EV_S_WEAPONHIT* pWeaponHit =  (EV_S_WEAPONHIT*)pEventData;*/

		SOLDIERTYPE *pSoldier = MercPtrs[ usSoldierID ];

		EV_S_WEAPONHIT SWeaponHit;

					
		
	
		
		if(usSoldierID < 20)
			SWeaponHit.usSoldierID = usSoldierID+ubID_prefix;
		else
			SWeaponHit.usSoldierID = usSoldierID;

		if(ubAttackerID < 20)
			SWeaponHit.ubAttackerID = ubAttackerID+ubID_prefix;
		else
			SWeaponHit.ubAttackerID = ubAttackerID;

		SWeaponHit.uiUniqueId = pSoldier->uiUniqueSoldierIdValue;
		SWeaponHit.usWeaponIndex = usWeaponIndex;
        SWeaponHit.sDamage = sDamage;
		SWeaponHit.sBreathLoss = sBreathLoss;
		SWeaponHit.usDirection = usDirection;
		SWeaponHit.sXPos = sXPos;
		SWeaponHit.sYPos = sYPos;
		SWeaponHit.sZPos = sZPos;
		SWeaponHit.sRange = sRange;
		
		SWeaponHit.fHit = fHit;
		SWeaponHit.ubSpecial = ubSpecial;
		SWeaponHit.ubLocation = ubHitLocation;

		


		client->RPC("sendHIT",(const char*)&SWeaponHit, (int)sizeof(EV_S_WEAPONHIT)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
}

void recieveHIT(RPCParameters *rpcParameters)
{
		
		EV_S_WEAPONHIT* SWeaponHit = (EV_S_WEAPONHIT*)rpcParameters->input;
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"recieveHIT" );

		SOLDIERTYPE *pSoldier = MercPtrs[ SWeaponHit->usSoldierID ];
					UINT16 usSoldierID;
					UINT8 ubAttackerID;

					if((SWeaponHit->usSoldierID >= ubID_prefix) && (SWeaponHit->usSoldierID < (ubID_prefix+5))) // within our netbTeam range...
						usSoldierID = (SWeaponHit->usSoldierID - ubID_prefix);
					else
						usSoldierID = SWeaponHit->usSoldierID;

					if((SWeaponHit->ubAttackerID >= ubID_prefix) && (SWeaponHit->ubAttackerID < (ubID_prefix+5)))
						ubAttackerID = (SWeaponHit->ubAttackerID - ubID_prefix);
					else
						ubAttackerID = SWeaponHit->ubAttackerID;

		
		//SWeaponHit->uiUniqueId = pSoldier->uiUniqueSoldierIdValue;
	
		//AddGameEvent( S_WEAPONHIT, 0, &SWeaponHit );
		WeaponHit( usSoldierID, SWeaponHit->usWeaponIndex, SWeaponHit->sDamage, SWeaponHit->sBreathLoss, SWeaponHit->usDirection, SWeaponHit->sXPos, SWeaponHit->sYPos, SWeaponHit->sZPos, SWeaponHit->sRange, ubAttackerID, SWeaponHit->fHit, SWeaponHit->ubSpecial, SWeaponHit->ubLocation );

		
}

void send_hire( UINT8 iNewIndex, UINT8 ubCurrentSoldier, INT16 iTotalContractLength, BOOLEAN fCopyProfileItemsOver)
{
		
		{
		
			send_hire_struct sHireMerc;

			sHireMerc.ubProfileID=ubCurrentSoldier;
			sHireMerc.iTotalContractLength=iTotalContractLength;
			sHireMerc.fCopyProfileItemsOver=fCopyProfileItemsOver;

			
			sHireMerc.bTeam=netbTeam;

			SOLDIERTYPE *pSoldier = MercPtrs[ iNewIndex ];
			pSoldier->ubStrategicInsertionCode=(atoi(SECT_EDGE)); // this sets the param read from the ini for your starting sector edge...
			

			
				if (pSoldier->ubID==0)
				{
							///create crate apon first hire
				crate_usMapPos = MAPROWCOLTOPOS( crate_sGridY, crate_sGridX );
				AddStructToUnLoadedMapTempFile( crate_usMapPos, SECONDOSTRUCT3, gsMercArriveSectorX, gsMercArriveSectorY, 0 );
				}
				
	

	

			client->RPC("sendHIRE",(const char*)&sHireMerc, (int)sizeof(send_hire_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}
}

void recieveHIRE(RPCParameters *rpcParameters)
{
	
	send_hire_struct* sHireMerc = (send_hire_struct*)rpcParameters->input;

	

	SOLDIERTYPE	*pSoldier;
	UINT8		iNewIndex;
	UINT8		ubCount=0;

	
	SOLDIERCREATE_STRUCT		MercCreateStruct;
	BOOLEAN fReturn = FALSE;
	
	

	memset( &MercCreateStruct, 0, sizeof( MercCreateStruct ) );
	MercCreateStruct.ubProfile						= sHireMerc->ubProfileID;
	MercCreateStruct.fPlayerMerc					= 0;
	MercCreateStruct.sSectorX							= gsMercArriveSectorX;
	MercCreateStruct.sSectorY							= gsMercArriveSectorY;
	MercCreateStruct.bSectorZ							= 0;
	MercCreateStruct.bTeam								= sHireMerc->bTeam;
	MercCreateStruct.fCopyProfileItemsOver			= sHireMerc->fCopyProfileItemsOver;
	

	TacticalCreateSoldier( &MercCreateStruct, &iNewIndex ) ;

	pSoldier = &Menptr[iNewIndex];
	pSoldier->uiStatusFlags |= SOLDIER_PC;
	if(!SAME_MERC)gMercProfiles[ pSoldier->ubProfile ].bMercStatus = MERC_WORKING_ELSEWHERE;
	
	pSoldier->bSide=PLAYER_BSIDE;



	AddSoldierToSector( iNewIndex );
	

	ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"A client has hired a Merc");

}

void send_gui_pos(SOLDIERTYPE *pSoldier,  FLOAT dNewXPos, FLOAT dNewYPos)
{
		
		{
			
			gui_pos gnPOS;

		
	

			gnPOS.usSoldierID = (pSoldier->ubID)+ubID_prefix;
			
			gnPOS.dNewXPos = dNewXPos;
			gnPOS.dNewYPos = dNewYPos;

		

			client->RPC("sendguiPOS",(const char*)&gnPOS, (int)sizeof(gui_pos)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}
	
}
void recieveguiPOS(RPCParameters *rpcParameters)
{
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"recieveguiPOS" );
		gui_pos* gnPOS = (gui_pos*)rpcParameters->input;

		SOLDIERTYPE *pSoldier = MercPtrs[ gnPOS->usSoldierID ];

		EVENT_SetSoldierPosition( pSoldier, (FLOAT)gnPOS->dNewXPos, (FLOAT)gnPOS->dNewYPos );
		
}

void send_gui_dir(SOLDIERTYPE *pSoldier, UINT16	usNewDirection)
{
		
		{
			
			gui_dir gnDIR;

		

			gnDIR.usSoldierID = (pSoldier->ubID)+ubID_prefix;
			gnDIR.usNewDirection = usNewDirection;
			


			client->RPC("sendguiDIR",(const char*)&gnDIR, (int)sizeof(gui_dir)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}
	
}

void recieveguiDIR(RPCParameters *rpcParameters)
{
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"recieveguiDIR" );
		gui_dir* gnDIR = (gui_dir*)rpcParameters->input;

		SOLDIERTYPE *pSoldier = MercPtrs[ gnDIR->usSoldierID ];

		EVENT_SetSoldierDirection( pSoldier, gnDIR->usNewDirection );
}




void send_EndTurn( UINT8 ubNextTeam )
{
		
		{
			//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"sendEndTurn" );
		
			if(ubNextTeam==0)
			{
				ubNextTeam=netbTeam;
			}
		
	
		turn_struct tStruct;

		tStruct.tsnetbTeam = netbTeam;
		tStruct.tsubNextTeam = ubNextTeam;

		client->RPC("sendEndTurn",(const char*)&tStruct, (int)sizeof(turn_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}


}

void recieveEndTurn(RPCParameters *rpcParameters)
{
		turn_struct* tStruct = (turn_struct*)rpcParameters->input;
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"recieveEndTurn" );
		UINT8 sender_bTeam;
		UINT8 ubTeam;
		sender_bTeam=tStruct->tsnetbTeam;
		ubTeam=tStruct->tsubNextTeam;
	
	
		if (!is_server && !(gTacticalStatus.uiFlags & INCOMBAT))
		{
			EnterCombatMode(0); 
		}

	
		if(ubTeam==netbTeam)ubTeam=0;
		{
			if(!is_server && is_client)EndTurnEvents();
		}
		if(!is_server && is_client)EndTurn( ubTeam );
		
		BeginTeamTurn( ubTeam );

}


UINT8 numenemyLAN( UINT8 ubSectorX, UINT8 ubSectorY )
{

	SOLDIERTYPE *pSoldier;
	UINT8				cnt ; //first posible lan player
	UINT8				ubNumEnemies = 0;

	
	for ( cnt=125 ; cnt <= 155; cnt++ )
	{
		pSoldier = MercPtrs[cnt];
		if ( pSoldier->bActive && pSoldier->bInSector && pSoldier->bLife > 0 )
		{
			
			if ( !pSoldier->bNeutral && (pSoldier->bSide != 0 ) )
			{
				ubNumEnemies++;
			}
		}	

	}
	return ubNumEnemies;

}

void send_AI( SOLDIERCREATE_STRUCT *pCreateStruct, UINT8 *pubID )
{
		//ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"send_AI" );

		SOLDIERCREATE_STRUCT aaa = *pCreateStruct;
		
		client->RPC("sendAI",(const char*)&aaa, (int)sizeof(SOLDIERCREATE_STRUCT)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

}

void recieveAI (RPCParameters *rpcParameters)
{
		UINT8 iNewIndex;
		SOLDIERTYPE *pSoldier;

		SOLDIERCREATE_STRUCT* aaa = (SOLDIERCREATE_STRUCT*)rpcParameters->input;
		
		SOLDIERCREATE_STRUCT bbb = *aaa;

		bbb.fPlayerPlan=1;

		TacticalCreateSoldier( &bbb, &iNewIndex );
		pSoldier = &Menptr[iNewIndex];
		pSoldier->uiStatusFlags |= SOLDIER_PC;
		if(!pSoldier->bActive)
		{
			ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, L"dud AI" );
		}
		else
		{
		AddSoldierToSector( iNewIndex );
		ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, L"recieveAI" );
		}

	
}

void start_battle ( void )
{ // add all merc to sector and init prebattle interface

	UINT8 i;

	for ( i=0; i < 20; i++)
	{
		SOLDIERTYPE *pSoldier = MercPtrs[ i ];
		if( pSoldier->bActive )
		{
			AddCharacterToAnySquad( pSoldier );
			AddSoldierToSector( i );
		}
	}
		
				

	GROUP *pGroup;
	pGroup = GetGroup( 30 ); // hmmm 30 ?

	CheckConditionsForBattle( pGroup );
}

void DropOffItemsInSector( UINT8 ubOrderNum )
{
	BOOLEAN	fSectorLoaded = FALSE;
	OBJECTTYPE		Object;
	UINT32	uiCount = 0;
	OBJECTTYPE	*pObject=NULL;
	UINT16	usNumberOfItems=0, usItem;
	UINT8		ubItemsDelivered, ubTempNumItems;
	UINT32	i;

	

	// determine if the sector is loaded
	if( ( gWorldSectorX == gsMercArriveSectorX ) && ( gWorldSectorY == gsMercArriveSectorY ) && ( gbWorldSectorZ == 0 ) )
		fSectorLoaded = TRUE;
	else
		fSectorLoaded = FALSE;

	// set crate to closed!
	if ( fSectorLoaded )
	{
		SetOpenableStructureToClosed( crate_usMapPos, 0 );
	}
	else
	{
		ChangeStatusOfOpenableStructInUnloadedSector( gsMercArriveSectorX, gsMercArriveSectorY, 0, crate_usMapPos, FALSE );
	}

	
	for(i=0; i<gpNewBobbyrShipments[ ubOrderNum ].ubNumberPurchases; i++)
	{
		// Count how many items were purchased
		usNumberOfItems += gpNewBobbyrShipments[ ubOrderNum ].BobbyRayPurchase[i].ubNumberPurchased;
	}

	//if we are NOT currently in the right sector
	if( !fSectorLoaded )
	{
		//build an array of objects to be added
		pObject = (OBJECTTYPE *) MemAlloc( sizeof( OBJECTTYPE ) * usNumberOfItems );
		if( pObject == NULL )
			return;
		memset( pObject, 0, sizeof( OBJECTTYPE ) * usNumberOfItems );
	}


	uiCount = 0;

	//loop through the number of purchases
	for( i=0; i< gpNewBobbyrShipments->ubNumberPurchases; i++)
	{
		ubItemsDelivered = gpNewBobbyrShipments[ ubOrderNum ].BobbyRayPurchase[i].ubNumberPurchased;
		usItem = gpNewBobbyrShipments[ ubOrderNum ].BobbyRayPurchase[i].usItemIndex;

		while ( ubItemsDelivered )
		{
			// treat 0s as 1s :-)
			ubTempNumItems = __min( ubItemsDelivered, __max( 1, Item[ usItem ].ubPerPocket ) );
			CreateItems( usItem, gpNewBobbyrShipments[ ubOrderNum ].BobbyRayPurchase[i].bItemQuality, ubTempNumItems, &Object );

			// stack as many as possible
			if( fSectorLoaded )
			{
				AddItemToPool( crate_usMapPos, &Object, -1, 0, 0, 0 );
			}
			else
			{
				memcpy( &pObject[ uiCount ], &Object, sizeof( OBJECTTYPE ) );
				uiCount++;
			}

			ubItemsDelivered -= ubTempNumItems;
		}
	}

	//if the sector WASNT loaded
	if( !fSectorLoaded )
	{
		//add all the items from the array that was built above

		//The item are to be added to the Top part of Drassen, grid loc's  10112, 9950
		if( !AddItemsToUnLoadedSector( gsMercArriveSectorX, gsMercArriveSectorY, 0, crate_usMapPos, uiCount, pObject, 0, 0, 0, -1, FALSE ) )
		{
			//error
			Assert( 0 );
		}
		MemFree( pObject );
		pObject = NULL;
	}

	//mark that the shipment has arrived
	gpNewBobbyrShipments[ ubOrderNum ].fActive = FALSE;
}


void send_stop (EV_S_STOP_MERC *SStopMerc) // used to stop a merc when he spots new enemies...
{
	EV_S_STOP_MERC stop_struct;
	if(SStopMerc->usSoldierID < 124)
	{
		//ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, L"send_stop" );
		if(SStopMerc->usSoldierID < 20)
		{
			stop_struct.usSoldierID = (SStopMerc->usSoldierID)+ubID_prefix;
		}
		stop_struct.sGridNo=SStopMerc->sGridNo;
		stop_struct.bDirection=SStopMerc->bDirection;
		client->RPC("sendSTOP",(const char*)&stop_struct, (int)sizeof(EV_S_STOP_MERC)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
}

void recieveSTOP (RPCParameters *rpcParameters)
{
	//ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, L"recieve_stop" );
	
	EV_S_STOP_MERC* SStopMerc =(EV_S_STOP_MERC*)rpcParameters->input;
	
	SOLDIERTYPE *pSoldier = MercPtrs[ SStopMerc->usSoldierID ];
	EVENT_StopMerc( pSoldier, SStopMerc->sGridNo, SStopMerc->bDirection );
}


//***************************
//*** client connection*****
//*************************

void connect_client ( void )
{
	
		if(!is_client)
		{
			ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"Initiating RakNet Client..." );

			
							
		
			client=RakNetworkFactory::GetRakPeerInterface();
			bool b = client->Startup(1,30,&SocketDescriptor(), 1);

			//RPC's
			REGISTER_STATIC_RPC(client, recievePATH);
			REGISTER_STATIC_RPC(client, recieveSTANCE);
			REGISTER_STATIC_RPC(client, recieveDIR);
			REGISTER_STATIC_RPC(client, recieveFIRE);
			REGISTER_STATIC_RPC(client, recieveHIT);
			REGISTER_STATIC_RPC(client, recieveHIRE);
			REGISTER_STATIC_RPC(client, recieveguiPOS);
			REGISTER_STATIC_RPC(client, recieveguiDIR);
			REGISTER_STATIC_RPC(client, recieveEndTurn);
			REGISTER_STATIC_RPC(client, recieveAI);
			REGISTER_STATIC_RPC(client, recieveSTOP);

			
		
			//***
			
		if (b)
		{
			//ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"Client started, waiting for connections...");
			is_client=true;
			/*repo=0;*/
		}
		else
		{ 
			ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"Client failed to start.  Terminating.");
			
		}
			
		is_connected=false;

	
		
			
			
			
		}	
		
		//reconnect/connect
		if( !is_connected && !is_connecting)
		{

			//retrieve settings from Ja2_mp.ini
			char ip[30];
			char port[30];
			char client_number[30];
			char sector_edge[30];

			
			char bteam1_enabled[30];
			char bteam2_enabled[30];
			char bteam3_enabled[30];
			char bteam4_enabled[30];

			char hire_same_merc[30];
			char player_bside[30];
			char net_div[30];

			char c_x[30];
			char c_y[30];

			GetPrivateProfileString( "Ja2_mp Settings","SERVER_IP", "", ip, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","SERVER_PORT", "", port, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","CLIENT_NUM", "", client_number, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","SECTOR_EDGE", "", sector_edge, MAX_PATH, "..\\Ja2_mp.ini" );

			GetPrivateProfileString( "Ja2_mp Settings","ENEMY_ENABLED", "", bteam1_enabled, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","CREATURE_ENABLED", "", bteam2_enabled, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","MILITIA_ENABLED", "", bteam3_enabled, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","CIV_ENABLED", "", bteam4_enabled, MAX_PATH, "..\\Ja2_mp.ini" );

			GetPrivateProfileString( "Ja2_mp Settings","SAME_MERC", "", hire_same_merc, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","PLAYER_BSIDE", "", player_bside, MAX_PATH, "..\\Ja2_mp.ini" );

			GetPrivateProfileString( "Ja2_mp Settings","NET_DIVISOR", "", net_div, MAX_PATH, "..\\Ja2_mp.ini" );

			GetPrivateProfileString( "Ja2_mp Settings","CRATE_X", "", c_x, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","CRATE_Y", "", c_y, MAX_PATH, "..\\Ja2_mp.ini" );


			strcpy( SERVER_IP , ip );
			strcpy( SERVER_PORT, port );
			strcpy( CLIENT_NUM, client_number );
			strcpy( SECT_EDGE, sector_edge);
			
			ENEMY_ENABLED =atoi(bteam1_enabled);
			CREATURE_ENABLED =atoi(bteam2_enabled);
			MILITIA_ENABLED =atoi(bteam3_enabled);
			CIV_ENABLED =atoi(bteam4_enabled);

			SAME_MERC = atoi(hire_same_merc);
			PLAYER_BSIDE = atoi(player_bside);

			NET_DIVISOR = atoi(net_div);

			crate_sGridX = atoi(c_x);
			crate_sGridY = atoi(c_y);

			netbTeam = (atoi(CLIENT_NUM))+5;
			ubID_prefix = gTacticalStatus.Team[ netbTeam ].bFirstID;

			//**********************
			//here some nifty little tweaks

				LaptopSaveInfo.guiNumberOfMercPaymentsInDays += 20;
				LaptopSaveInfo.gubLastMercIndex = NUMBER_MERCS_AFTER_FOURTH_MERC_ARRIVES;
				LaptopSaveInfo.fBobbyRSiteCanBeAccessed = TRUE;
				extern BOOLEAN gfTemporaryDisablingOfLoadPendingFlag;
				gfTemporaryDisablingOfLoadPendingFlag = TRUE;
				SetBookMark( AIM_BOOKMARK );
				SetBookMark( BOBBYR_BOOKMARK );
				//SetBookMark( IMP_BOOKMARK );
				SetBookMark( MERC_BOOKMARK );

							
			//**********************
			
			
			ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"Connecting to specified IP...");
			client->Connect(SERVER_IP, atoi(SERVER_PORT), 0,0);
			is_connecting=true;
			
		}
	
		else if (is_connecting)
		{
			ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"already connecting..." );
		}
		else if (is_connected)
		{
			ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"already connected" );
		}

}

void client_packet ( void )
{	
	
	Packet* p;

	
	if (is_client)
	{

	p = client->Receive();

	while(p)
	{
			//continue; // Didn't get any packets

		// We got a packet, get the identifier with our handy function
		packetIdentifier = GetPacketIdentifier(p);
		//ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"packet recieved");
		// Check if this is a network message packet
		switch (packetIdentifier)
		{
			case ID_DISCONNECTION_NOTIFICATION:
				  // Connection lost normally
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_DISCONNECTION_NOTIFICATION");
				is_connected=false;
				break;
			case ID_ALREADY_CONNECTED:
				// Connection lost normally
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_ALREADY_CONNECTED");
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_REMOTE_DISCONNECTION_NOTIFICATION");
				break;
			case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_REMOTE_CONNECTION_LOST");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_REMOTE_NEW_INCOMING_CONNECTION");
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_CONNECTION_ATTEMPT_FAILED");
				is_connected=false;
				is_connecting=false;
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				 // Sorry, the server is full.  I don't do anything here but
				// A real app should tell the user
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_NO_FREE_INCOMING_CONNECTIONS");
				break;
			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_CONNECTION_LOST");
				is_connected=false;
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				// This tells the client they have connected
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_CONNECTION_REQUEST_ACCEPTED");
				is_connected=true;
				is_connecting=false;
				break;
				case ID_NEW_INCOMING_CONNECTION:
				//tells server client has connected
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_NEW_INCOMING_CONNECTION");
				break;
			case ID_MODIFIED_PACKET:
				// Cheater!
				ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"ID_MODIFIED_PACKET");
				break;
			default:
					
					ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"** a packet has been recieved for which i dont know what to do... **");
				break;
		}


		// We're done with the packet, get more :)
		client->DeallocatePacket(p);
		p = client->Receive();
	}
	}
}
// Copied from Multiplayer.cpp
// If the first byte is ID_TIMESTAMP, then we want the 5th byte
// Otherwise we want the 1st byte
unsigned char GetPacketIdentifier(Packet *p)
{
	if (p==0)
		return 255;

	if ((unsigned char)p->data[0] == ID_TIMESTAMP)
	{
		assert(p->length > sizeof(unsigned char) + sizeof(unsigned long));
		return (unsigned char) p->data[sizeof(unsigned char) + sizeof(unsigned long)];
	}
	else
		return (unsigned char) p->data[0];
}


void client_disconnect (void)
{
	if(is_client)
	{
	client->Shutdown(300);
	is_client = false;
	is_connected=false;
	is_connecting=false;
	// We're done with the network
	RakNetworkFactory::DestroyRakPeerInterface(client);
	ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"client disconnected and shutdown");
	}
	else
	{
	ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"client is not running");
	}
}