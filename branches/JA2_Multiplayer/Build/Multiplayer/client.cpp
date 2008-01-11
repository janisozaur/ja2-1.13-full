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

#include "map edgepoints.h"

#include "fresh_header.h"
#include "network.h"

#include "tactical placement gui.h"
#include "prebattle interface.h"

unsigned char GetPacketIdentifier(Packet *p);
unsigned char packetIdentifier;

#include "messagebox.h"

#pragma pack(1)

#include "new.h"
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
#include "test_space.h"

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

		SOLDIERCREATE_STRUCT standard_data;

		OBJECTTYPE slot0;
		OBJECTTYPE slot1;
		OBJECTTYPE slot2;
		OBJECTTYPE slot3;
		OBJECTTYPE slot4;
		OBJECTTYPE slot5;
		OBJECTTYPE slot6;
		OBJECTTYPE slot7;
		OBJECTTYPE slot8;
		OBJECTTYPE slot9;
		OBJECTTYPE slot10;
		OBJECTTYPE slot11;
		OBJECTTYPE slot12;
		OBJECTTYPE slot13;
		OBJECTTYPE slot14;
		OBJECTTYPE slot15;
		OBJECTTYPE slot16;
		OBJECTTYPE slot17;
		OBJECTTYPE slot18;
	} AI_STRUCT;

typedef struct
{
	BULLET net_bullet;
	UINT16 usHandItem;

}netb_struct;

typedef struct
{
	INT8 ubID;
	INT8 bTeam;
	UINT8 gubOutOfTurnPersons;
	UINT8 gubOutOfTurnOrder[MAXMERCS];
} INT_STRUCT;

typedef struct
{
	UINT8 client_num;
	bool status;
	UINT8 ready_stage;

} ready_struct;

typedef struct
{
	INT32 remote_id;
	INT32 local_id;

}bullets_table;

bullets_table bTable[11][50];

char client_names[4][30];

INT32 MAX_MERCS;

int OP_TEAM_1;
int OP_TEAM_2;
int OP_TEAM_3;
int OP_TEAM_4;

UINT8 netbTeam;
UINT8	ubID_prefix;

 bool is_connected=false;
 bool is_connecting=false;
 bool is_client=false;
 bool is_server=false;
 bool is_networked=false;


 int TESTING;
 bool allowlaptop=false;

 bool recieved_settings=false;

 char CLIENT_NUM[30];
 char SECT_EDGE[30];

 char CLIENT_NAME[30];

 char SERVER_IP[30] ;
 char SERVER_PORT[30];

 int ENEMY_ENABLED;
 int CREATURE_ENABLED;
 int MILITIA_ENABLED;
 int CIV_ENABLED;

 int ALLOW_EQUIP;

 int SAME_MERC;
 int PLAYER_BSIDE;

int START_TEAM_TURN;

 bool goahead = 0;
 int numready = 0;
 int readystage = 0;
 bool status = 0;

 bool lockedgui = 0;

FLOAT DAMAGE_MULTIPLIER;

int INTERRUPTS;
int MAX_CLIENTS;

UINT16 crate_usMapPos;	

INT16	crate_sGridX, crate_sGridY;

void overide_callback( UINT8 ubResult );
//*****************
//RPC sends and recieves:
//********************


void send_path (  SOLDIERTYPE *pSoldier, UINT16 sDestGridNo, UINT16 usMovementAnim, BOOLEAN fFromUI, BOOLEAN fForceRestartAnim  )
{
	if(pSoldier->ubID < 120)
	{
		//ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"Sending RPC..." );
		EV_S_GETNEWPATH	SGetNewPath;


		

		if(pSoldier->ubID < 20)
			SGetNewPath.usSoldierID = (pSoldier->ubID)+ubID_prefix;
		else
			SGetNewPath.usSoldierID = pSoldier->ubID;

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
	if(pSoldier->ubID < 120)
	{
		EV_S_CHANGESTANCE			SChangeStance;

		SChangeStance.ubNewStance   = ubDesiredStance;
		//SChangeStance.usSoldierID  = pSoldier->ubID;
		

		if(pSoldier->ubID < 20)
			SChangeStance.usSoldierID = (pSoldier->ubID)+ubID_prefix;
		else
			SChangeStance.usSoldierID = pSoldier->ubID;


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
	if((is_server && pSoldier->ubID < 120) || (!is_server && pSoldier->ubID < 20))
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
	if(pSoldier->ubID < 120)
	{
	
	EV_S_BEGINFIREWEAPON SBeginFireWeapon;


		

	if(pSoldier->ubID < 20)
		SBeginFireWeapon.usSoldierID = (pSoldier->ubID)+ubID_prefix;
	else
		SBeginFireWeapon.usSoldierID = pSoldier->ubID;


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



void send_hit(  EV_S_WEAPONHIT *SWeaponHit  )
{

		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"sendHIT" );
		//EV_S_WEAPONHIT* pWeaponHit =  (EV_S_WEAPONHIT*)pEventData;
		//SOLDIERTYPE *pSoldier = MercPtrs[ usSoldierID ];

		EV_S_WEAPONHIT weaphit_struct;				
				
		memcpy( &weaphit_struct , SWeaponHit, sizeof( EV_S_WEAPONHIT ));
		
		UINT16  usSoldierID=weaphit_struct.usSoldierID;

		if(SWeaponHit->usSoldierID < 20)weaphit_struct.usSoldierID = weaphit_struct.usSoldierID+ubID_prefix;
		if(SWeaponHit->ubAttackerID < 20)weaphit_struct.ubAttackerID = weaphit_struct.ubAttackerID+ubID_prefix;
			

		client->RPC("sendHIT",(const char*)&weaphit_struct, (int)sizeof(EV_S_WEAPONHIT)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

}

void recieveHIT(RPCParameters *rpcParameters)
{
		
		EV_S_WEAPONHIT* SWeaponHit = (EV_S_WEAPONHIT*)rpcParameters->input;
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"recieveHIT" );

		SOLDIERTYPE *pSoldier = MercPtrs[ SWeaponHit->usSoldierID ];
					UINT16 usSoldierID;
					UINT8 ubAttackerID;

					if((SWeaponHit->usSoldierID >= ubID_prefix) && (SWeaponHit->usSoldierID < (ubID_prefix+7))) // within our netbTeam range...
						usSoldierID = (SWeaponHit->usSoldierID - ubID_prefix);
					else
						usSoldierID = SWeaponHit->usSoldierID;

					if((SWeaponHit->ubAttackerID >= ubID_prefix) && (SWeaponHit->ubAttackerID < (ubID_prefix+7)))
						ubAttackerID = (SWeaponHit->ubAttackerID - ubID_prefix);
					else
						ubAttackerID = SWeaponHit->ubAttackerID;
	

	
		//AddGameEvent( S_WEAPONHIT, 0, &SWeaponHit );
		WeaponHit( usSoldierID, SWeaponHit->usWeaponIndex, SWeaponHit->sDamage, SWeaponHit->sBreathLoss, SWeaponHit->usDirection, SWeaponHit->sXPos, SWeaponHit->sYPos, SWeaponHit->sZPos, SWeaponHit->sRange, ubAttackerID, SWeaponHit->fHit, SWeaponHit->ubSpecial, SWeaponHit->ubLocation );

		if(SWeaponHit->fStopped)
		{
			INT8 bTeam=pSoldier->bTeam;
			INT32 iBullet = bTable[bTeam][SWeaponHit->iBullet].local_id;
					
			RemoveBullet(iBullet);
			//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"removed bullet" );

		
		}
		
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
			
			AddCharacterToAnySquad( pSoldier );
			AddSoldierToSector( pSoldier->ubID ); //add g\hired merc to sector so can access sector inv.
			//pSoldier->bSide=3;
			
				if (pSoldier->ubID==0)
				{
							///create crate apon first hire
				crate_usMapPos = MAPROWCOLTOPOS( crate_sGridY, crate_sGridX );

				//crate_usMapPos	= ChooseMapEdgepoint(atoi(SECT_EDGE));
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
	pSoldier->bSide=0; //default coop
	if(PLAYER_BSIDE==0)//allow opposition
	{
	if(MercCreateStruct.bTeam==6)pSoldier->bSide=OP_TEAM_1;
		if(MercCreateStruct.bTeam==7)pSoldier->bSide=OP_TEAM_2;
			if(MercCreateStruct.bTeam==8)pSoldier->bSide=OP_TEAM_3;
		if(MercCreateStruct.bTeam==9)pSoldier->bSide=OP_TEAM_4;
	}
	


	AddSoldierToSector( iNewIndex );
	

	ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"Client #%d: '%S' has hired '%s'",MercCreateStruct.bTeam-5,client_names[MercCreateStruct.bTeam-6],pSoldier->name );

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

	
	for ( cnt=120 ; cnt <= 155; cnt++ )
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

		//SOLDIERCREATE_STRUCT aaa = *pCreateStruct;
	
		AI_STRUCT send_inv;

		send_inv.standard_data = *pCreateStruct;

		send_inv.slot0 = pCreateStruct->Inv[ 0 ];
		send_inv.slot1 = pCreateStruct->Inv[ 1 ];
		send_inv.slot2 = pCreateStruct->Inv[ 2];
		send_inv.slot3 = pCreateStruct->Inv[ 3 ];
		send_inv.slot4 = pCreateStruct->Inv[ 4 ];
		send_inv.slot5 = pCreateStruct->Inv[ 5 ];
		send_inv.slot6 = pCreateStruct->Inv[ 6 ];
		send_inv.slot7 = pCreateStruct->Inv[ 7 ];
		send_inv.slot8 = pCreateStruct->Inv[ 8 ];
		send_inv.slot9 = pCreateStruct->Inv[ 9 ];
		send_inv.slot10 = pCreateStruct->Inv[ 10 ];
		send_inv.slot11 = pCreateStruct->Inv[ 11 ];
		send_inv.slot12 = pCreateStruct->Inv[ 12 ];
		send_inv.slot13 = pCreateStruct->Inv[ 13 ];
		send_inv.slot14 = pCreateStruct->Inv[ 14 ];
		send_inv.slot15 = pCreateStruct->Inv[ 15 ];
		send_inv.slot16 = pCreateStruct->Inv[ 16 ];
		send_inv.slot17 = pCreateStruct->Inv[ 17 ];
		send_inv.slot18 = pCreateStruct->Inv[ 18 ];

		
		
		client->RPC("sendAI",(const char*)&send_inv, (int)sizeof(AI_STRUCT)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

}

void recieveAI (RPCParameters *rpcParameters)
{
		UINT8 iNewIndex;
		SOLDIERTYPE *pSoldier;


		AI_STRUCT* send_inv = (AI_STRUCT*)rpcParameters->input;

		SOLDIERCREATE_STRUCT new_standard_data; //as originally my soldiercreate_struct would get its INV mangled through the RPC, so i send it packaged with it, then create a new struct and add in the bits that are still ok from the original struct, along with the packaged INV components...
	

		new_standard_data.bAgility = send_inv->standard_data.bAgility;
		new_standard_data.bAIMorale = send_inv->standard_data.bAIMorale;
		new_standard_data.bAttitude = send_inv->standard_data.bAttitude;
		new_standard_data.bBodyType = send_inv->standard_data.bBodyType;
		new_standard_data.bDexterity = send_inv->standard_data.bDexterity;
		new_standard_data.bDirection = send_inv->standard_data.bDirection;
		new_standard_data.bExpLevel = send_inv->standard_data.bExpLevel;
		new_standard_data.bExplosive = send_inv->standard_data.bExplosive;
		new_standard_data.bLeadership = send_inv->standard_data.bLeadership;
		new_standard_data.bLife = send_inv->standard_data.bLife;
		new_standard_data.bLifeMax = send_inv->standard_data.bLifeMax;
		new_standard_data.bMarksmanship = send_inv->standard_data.bMarksmanship;
		new_standard_data.bMechanical = send_inv->standard_data.bMechanical;
		new_standard_data.bMedical = send_inv->standard_data.bMedical;
		new_standard_data.bMorale = send_inv->standard_data.bMorale;
		new_standard_data.bOrders = send_inv->standard_data.bOrders;
		memcpy( new_standard_data.bPadding, send_inv->standard_data.bPadding, sizeof( INT8 ) * 115 );
		new_standard_data.bPatrolCnt = send_inv->standard_data.bPatrolCnt;
		new_standard_data.bSectorZ = send_inv->standard_data.bSectorZ;
		new_standard_data.bStrength = send_inv->standard_data.bStrength;
		new_standard_data.bTeam = send_inv->standard_data.bTeam;
		new_standard_data.bUseGivenVehicleID = send_inv->standard_data.bUseGivenVehicleID;
		new_standard_data.bWisdom = send_inv->standard_data.bWisdom;
		new_standard_data.ef1 = send_inv->standard_data.ef1;
		new_standard_data.ef2 = send_inv->standard_data.ef2;
		new_standard_data.endOfPOD = send_inv->standard_data.endOfPOD;
		new_standard_data.fCopyProfileItemsOver = send_inv->standard_data.fCopyProfileItemsOver;
		new_standard_data.fHasKeys = send_inv->standard_data.fHasKeys;
		new_standard_data.fKillSlotIfOwnerDies = send_inv->standard_data.fKillSlotIfOwnerDies;
		new_standard_data.fOnRoof = send_inv->standard_data.fOnRoof;
		new_standard_data.fPlayerMerc = send_inv->standard_data.fPlayerMerc;
		new_standard_data.fPlayerPlan = send_inv->standard_data.fPlayerPlan;
		new_standard_data.fStatic = send_inv->standard_data.fStatic;
		new_standard_data.fUseExistingSoldier = send_inv->standard_data.fUseExistingSoldier;
		new_standard_data.fUseGivenVehicle = send_inv->standard_data.fUseGivenVehicle;
		new_standard_data.fVisible = send_inv->standard_data.fVisible;
		memcpy( new_standard_data.HeadPal , send_inv->standard_data.HeadPal, sizeof( PaletteRepID ));
		memcpy( new_standard_data.MiscPal , send_inv->standard_data.MiscPal, sizeof( PaletteRepID ));
		memcpy( new_standard_data.name , send_inv->standard_data.name, sizeof( CHAR16 ) * 10 );
		memcpy( new_standard_data.PantsPal , send_inv->standard_data.PantsPal, sizeof( PaletteRepID ));
		new_standard_data.pExistingSoldier = send_inv->standard_data.pExistingSoldier;
		new_standard_data.sInsertionGridNo = send_inv->standard_data.sInsertionGridNo;
		memcpy( new_standard_data.SkinPal , send_inv->standard_data.SkinPal, sizeof( PaletteRepID ));
		memcpy( new_standard_data.sPatrolGrid, send_inv->standard_data.sPatrolGrid, sizeof( INT16 ) * MAXPATROLGRIDS );
		new_standard_data.sSectorX = send_inv->standard_data.sSectorX;
		new_standard_data.sSectorY = send_inv->standard_data.sSectorY;
		new_standard_data.ubCivilianGroup = send_inv->standard_data.ubCivilianGroup;
		new_standard_data.ubProfile = send_inv->standard_data.ubProfile;
		new_standard_data.ubScheduleID = send_inv->standard_data.ubScheduleID;
		new_standard_data.ubSoldierClass = send_inv->standard_data.ubSoldierClass;
		memcpy( new_standard_data.VestPal , send_inv->standard_data.VestPal, sizeof( PaletteRepID ));
				

		new_standard_data.Inv[0] = send_inv->slot0;
		new_standard_data.Inv[1] = send_inv->slot1;
		new_standard_data.Inv[2] = send_inv->slot2;
		new_standard_data.Inv[3] = send_inv->slot3;
		new_standard_data.Inv[4] = send_inv->slot4;
		new_standard_data.Inv[5] = send_inv->slot5;
		new_standard_data.Inv[6] = send_inv->slot6;
		new_standard_data.Inv[7] = send_inv->slot7;
		new_standard_data.Inv[8] = send_inv->slot8;
		new_standard_data.Inv[9] = send_inv->slot9;
		new_standard_data.Inv[10] = send_inv->slot10;
		new_standard_data.Inv[11] = send_inv->slot11;
		new_standard_data.Inv[12] = send_inv->slot12;
		new_standard_data.Inv[13] = send_inv->slot13;
		new_standard_data.Inv[14] = send_inv->slot14;
		new_standard_data.Inv[15] = send_inv->slot15;
		new_standard_data.Inv[16] = send_inv->slot16;
		new_standard_data.Inv[17] = send_inv->slot17;
		new_standard_data.Inv[18] = send_inv->slot18;

		new_standard_data.fPlayerPlan=1;

	
    	TacticalCreateSoldier( &new_standard_data, &iNewIndex );
		pSoldier = &Menptr[iNewIndex];
		pSoldier->uiStatusFlags |= SOLDIER_PC;

		AddSoldierToSector( iNewIndex );
		//ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, L"recieveAI" );
	

	
}

void send_ready ( void )
{
				ready_struct info;
				info.client_num = atoi(CLIENT_NUM);
		
				if(readystage==0)
				{
					
					info.ready_stage = 0;
					if(status==0)
					{
					info.status = 1; 
					status=1;
					numready = numready+1;
					ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"You are ready - Total ready = %d/%d",numready,MAX_CLIENTS );
					}
					else
					{
				    info.status = 0; 
					status=0;
					numready = numready-1;
					ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"You are no longer ready - Total ready = %d/%d",numready,MAX_CLIENTS );
					}	
				}
				
				
		
					if(is_server && numready == MAX_CLIENTS) //all ready. and server tells all to load...
					{
					ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"** beginning battle ..." );

					goahead=1;
					readystage=1;

					info.ready_stage = 1;
					info.status = 1; 					
					}	
			
				
			
		client->RPC("sendREADY",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

		if(is_server && numready == MAX_CLIENTS)
		{
			
			status=0;//reset
			numready=0;
			start_battle();//server loads
		}
}

void recieveREADY (RPCParameters *rpcParameters)
{
	ready_struct* info = (ready_struct*)rpcParameters->input;

	if(info->ready_stage==1)//recived ok for go ahead from server for level load
	{
		numready++;
		ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"Client #%d is ready - Total ready = %d/%d", info->client_num,numready,MAX_CLIENTS );
		ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"** beginning battle ..." );
		status=0;//reset
		numready=0;
		goahead=1;
		start_battle();
	}
	else if (info->ready_stage != 36) // recieved status update from client
	{
		if (info->status==1)
		{
		numready = numready+1;
		ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"Client #%d is ready - Total ready = %d/%d", info->client_num,numready,MAX_CLIENTS );
		
		}
		else
		{
		numready = numready-1;
		ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"Client #%d is no longer ready - Total ready = %d/%d", info->client_num,numready,MAX_CLIENTS );
		
		}
		
		if(is_server && numready == MAX_CLIENTS) //all ready. and server tells all to load...and loads himself
		{
			goahead=1;
			readystage=1;
			send_ready();
			start_battle();
			
		}
	}
	else if(info->ready_stage==36)//server allows laptop access
	{
		ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"The Server has unlocked the laptop... begin hiring !" );
		allowlaptop=1;
	}

}


void send_loaded (void)
{
		ready_struct info;
		info.client_num = atoi(CLIENT_NUM);
		info.ready_stage = 1;//done loading level
		info.status=1;

		numready++;
		if(numready==MAX_CLIENTS && is_server)
		{
			lockui(1);//unlock ui
			readystage=0;
			numready=0;

					info.ready_stage = 2;//done placing mercs
					info.status=1;

					//client->RPC("sendGUI",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}

		client->RPC("sendGUI",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

}
void send_donegui ( UINT8 ubResult )
{
	if(ubResult==1 && status==0)return;//avoid double remove callback response from final message box removal
		ready_struct info;
		info.client_num = atoi(CLIENT_NUM);
		if(ubResult==1)DialogRemoved(1);//cleanup msgbox after not ready
		if(status==0)//now ready
		{
			status=1;
			numready++;
		info.ready_stage = 3;//done placing mercs
		info.status=1;
			
			SGPRect CenterRect = { 100, 100, SCREEN_WIDTH - 100, 300 };
			DoMessageBox( MSG_BOX_BASIC_STYLE, L"You Are Ready, Awaiting other Clients... ok goes not ready.",  guiCurrentScreen, MSG_BOX_FLAG_OK | MSG_BOX_FLAG_USE_CENTERING_RECT, send_donegui,  &CenterRect );


			if(numready==MAX_CLIENTS && is_server)//all done
			{
				
				numready=0;
				status=0;
				info.ready_stage = 4;//done placing mercs
				info.status=1;
				gMsgBox.bHandled = MSG_BOX_RETURN_OK;
				KillTacticalPlacementGUI(); //send and kill
				ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"... Let the games begin !");
			}
		}
		else if(status==1)//was ready
		{
			status=0;
			numready--;
			info.ready_stage = 3;//not done placing mercs
			info.status=0;
		}

		

		client->RPC("sendGUI",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void recieveGUI (RPCParameters *rpcParameters)
{


	ready_struct* info = (ready_struct*)rpcParameters->input;

	if(info->ready_stage==1 && info->status==1)
	{
		numready++;
		if(numready==MAX_CLIENTS && is_server)
		{
			lockui(1);//unlock ui
			readystage=0;
			numready=0;

					ready_struct info;
					info.client_num = atoi(CLIENT_NUM);
					info.ready_stage = 2;//done placing mercs
					info.status=1;

					client->RPC("sendGUI",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}

	}
	if(info->ready_stage==2 && info->status ==1)
	{
		lockui(1);//unlock ui
		readystage=0;
		numready=0;
		
	}

	if(info->ready_stage==3 && info->status==1)//recieved client done placement
	{
		numready++;
		if(numready==MAX_CLIENTS && is_server)//all done
		{
						numready=0;

					ready_struct info;
					info.client_num = atoi(CLIENT_NUM);
					info.ready_stage = 4;//all done placing mercs, kill all
					info.status=1;
					gMsgBox.bHandled = MSG_BOX_RETURN_OK;
					status=0;
					KillTacticalPlacementGUI();
					ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"... Let the games begin !");

					client->RPC("sendGUI",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}
	}


	if(info->ready_stage==3 && info->status==0)//recieved client retracted place ready...
	{
		numready--;
	}

	if(info->ready_stage==4 && info->status==1)
	{
		gMsgBox.bHandled = MSG_BOX_RETURN_OK;
		KillTacticalPlacementGUI();
		ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"... Let the games begin !");
		numready=0;
		status=0;
	}

}
void allowlaptop_callback ( UINT8 ubResult )
{

	if(ubResult==2)
	{
	ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"The Server has unlocked the laptop... begin hiring !" );
	allowlaptop=1;

				ready_struct info;
				info.client_num = atoi(CLIENT_NUM);
				info.ready_stage=36;
				info.status=1;

	client->RPC("sendREADY",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

	}
}


void start_battle ( void )
{ 
if(!is_client)
{
ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"** a client must be running for game start" );
}
else if(!allowlaptop && is_server)
{
			SGPRect CenterRect = { 100, 100, SCREEN_WIDTH - 100, 300 };
			DoMessageBox( MSG_BOX_BASIC_STYLE, L"Unlock Laptop for Hiring ?  (are all clients connected?)",  guiCurrentScreen, MSG_BOX_FLAG_YESNO | MSG_BOX_FLAG_USE_CENTERING_RECT, allowlaptop_callback,  &CenterRect );

}
else if(allowlaptop)
{
	
	if ( NumberOfMercsOnPlayerTeam() ==0)
	{
	ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"** Game cannot start, No mercs hired ..." );
	}
	else if(goahead==1)
	{	
	goahead=0;
	status=0;//reset
	numready=0;
	SOLDIERTYPE *pSoldier = MercPtrs[ 0 ];
	UINT8 ubGroupID = pSoldier->ubGroupID;

	GROUP *pGroup;
	pGroup = GetGroup( ubGroupID ); 
	gpBattleGroup = pGroup;
		gubPBSectorX = gpBattleGroup->ubSectorX;
		gubPBSectorY = gpBattleGroup->ubSectorY;
		gubPBSectorZ = gpBattleGroup->ubSectorZ;

	gfEnterTacticalPlacementGUI = 1;

	UINT32	i;
	for(i=0; i<4;i++)
	{	
		CHAR16 name[255];
		int nm = mbstowcs( name, client_names[i], sizeof (char)*30 );
		//copy in client specified name for the player turn bar :)
		if(nm)
		{
			CHAR16 string[255];
			memcpy(string,TeamTurnString[ (i+6) ], sizeof( CHAR16) * 255 );
		

			CHAR16 full[255];
			swprintf(full, L"%s - '%s'",string,name);

			memcpy( TeamTurnString[ (i+6) ] , full, sizeof( CHAR16) * 255 );
		}
	}

	SetCurrentWorldSector( gubPBSectorX, gubPBSectorY, gubPBSectorZ );
	}
	else
	{
	send_ready();
	}
}
else if(!allowlaptop && is_client && !is_server)
{
   ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"awaiting ok from server for laptop unlock..." );
}
	
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
		//SetOpenableStructureToClosed( crate_usMapPos, 0 );
	}
	else
	{
		//ChangeStatusOfOpenableStructInUnloadedSector( gsMercArriveSectorX, gsMercArriveSectorY, 0, crate_usMapPos, FALSE );
		SetSectorFlag( gsMercArriveSectorX, gsMercArriveSectorY, 0, SF_ALREADY_VISITED);//allows update of item count
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
				AddItemToPool( crate_usMapPos, &Object, 1, 0, WOLRD_ITEM_FIND_SWEETSPOT_FROM_GRIDNO | WORLD_ITEM_REACHABLE, 0 );
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
		if( !AddItemsToUnLoadedSector( gsMercArriveSectorX, gsMercArriveSectorY, 0, crate_usMapPos, uiCount, pObject, 0, WOLRD_ITEM_FIND_SWEETSPOT_FROM_GRIDNO | WORLD_ITEM_REACHABLE, 0, 1, FALSE ) )
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
	if(SStopMerc->usSoldierID < 120)
	{
		//ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, L"send_stop" );
		if(SStopMerc->usSoldierID < 20)
		{
			stop_struct.usSoldierID = (SStopMerc->usSoldierID)+ubID_prefix;
		}
		else
			stop_struct.usSoldierID = SStopMerc->usSoldierID;

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
	//EVENT_StopMerc( pSoldier, SStopMerc->sGridNo, SStopMerc->bDirection );
	AdjustNoAPToFinishMove( pSoldier, TRUE );
	//pSoldier->ubReasonCantFinishMove = REASON_STOPPED_SIGHT;
	//	pSoldier->usPendingAnimation = NO_PENDING_ANIMATION;
	//	pSoldier->usPendingAnimation2 = NO_PENDING_ANIMATION;		
	//	pSoldier->fContinueMoveAfterStanceChange = FALSE;
	//	pSoldier->bTurningFromUI = FALSE;
		pSoldier->fTurningFromPronePosition = FALSE;
		
}

void send_interrupt (SOLDIERTYPE *pSoldier)
{

	
	INT_STRUCT INT;

	INT.ubID = pSoldier->ubID;
	INT.bTeam = pSoldier->bTeam;
	if(INT.bTeam==0)
	{
		INT.bTeam=netbTeam;
		INT.ubID=INT.ubID+ubID_prefix;
	}

	memcpy(INT.gubOutOfTurnOrder, gubOutOfTurnOrder, sizeof(UINT8) * MAXMERCS);
	INT.gubOutOfTurnPersons = gubOutOfTurnPersons;
	
	//if(gubOutOfTurnPersons>0)ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Sent Int: bTeam-%d ubID-%d", INT.bTeam,INT.ubID );
	
	
	client->RPC("sendINTERRUPT",(const char*)&INT, (int)sizeof(INT_STRUCT)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);


}

void recieveINTERRUPT (RPCParameters *rpcParameters)
{
	INT_STRUCT* INT = (INT_STRUCT*)rpcParameters->input;
	

		if(INT->bTeam==netbTeam)
		{
			INT->bTeam=0;
			INT->ubID=INT->ubID - ubID_prefix;
		}

			if(	INT->bTeam !=0)
			{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Interrupted" );
			//stop moving merc who was interrupted and init UI bar
			SOLDIERTYPE* pMerc = MercPtrs[ gusSelectedSoldier ];
			AdjustNoAPToFinishMove( pMerc, TRUE );	
			pMerc->fTurningFromPronePosition = FALSE;// hmmm ??
			FreezeInterfaceForEnemyTurn();
			InitEnemyUIBar( 0, 0 );
			fInterfacePanelDirty = DIRTYLEVEL2;
			AddTopMessage( COMPUTER_TURN_MESSAGE, TeamTurnString[ INT->bTeam ] );
			gTacticalStatus.fInterruptOccurred = TRUE;
			
			}
			else
			{
				//it for us ! :)
				if(INT->gubOutOfTurnPersons==0)//indicates finished interrupt maybe can just call end interrupt
				{
					//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"finish from interrupt" );
					ClearIntList();
					guiPendingOverrideEvent = LU_ENDUILOCK;
					guiPendingOverrideEvent = LA_ENDUIOUTURNLOCK;
					HandleTacticalUI( );
					if ( gusSelectedSoldier != NO_SOLDIER )
					{
					SlideTo( NOWHERE, gusSelectedSoldier, NOBODY ,SETLOCATOR);
					DoMercBattleSound( MercPtrs[ gusSelectedSoldier ], BATTLE_SOUND_ATTN1 );
					}
					InitPlayerUIBar( 2 );
				}
				else //start our interrupt
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Interruptor" );
					//StartInterrupt();
				}
			}
	
		//if(INT->gubOutOfTurnPersons > 0)ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Recieved Int: bTeam-%d ubID-%d", INT->bTeam,INT->ubID );
}

void grid_display ( void ) //print mouse coordinates, helpfull for crate placement.
{
	INT16	sGridX, sGridY;
	UINT16 usMapPos;

	GetMouseXY( &sGridX, &sGridY );
	usMapPos = MAPROWCOLTOPOS( sGridY, sGridX );
	
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Mouse Grid Coordinates:" );
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"X: %d, Y: %d", sGridX, sGridY );
	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Grid Number: %d", usMapPos );
}

void mp_help(void)
{
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L" ");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L"MP controls: (from map screen)");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L"** first set up Ja2_mp.ini **");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L" ");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L"'1' - start server, '2' - connect to server");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L"'3' - ready for match, also unlock laptop if server");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L"'4' - quit server and client");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L"'5' - display mouse co-ords (from tactical view)");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L"'7' - popup server overide panel");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L"'F2' - display secondary help");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L" ");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L"see readme.html for further details");
	


}

void mp_help2(void)
{
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L" ");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L"Tips: (assuming ja2_mp.ini is set up)");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L" ");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L"* make sure all clients have unique CLIENT_NUM ");

	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L"* game save doesn't record bobby rays order ,");
	
	
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L"* avoid placing opposed mercs in immediate sight,");

	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L" ");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L"'F1' - display primary help");
	ScreenMsg( MSG_FONT_WHITE, MSG_CHAT, L"see readme.html for further details");

}

void manual_overide (void)//now bound to "7"
{
	if(is_server)
	{
	//manual overide command for server
		const STR16 msg = L"Choose Server Manual Overide Stage:  ( 1 - Enable laptop/hiring )  ( 2 - Launch/load level )  ( 3 - Unlock Ui )  ( 4 - Finish placement ) ";

			SGPRect CenterRect = { 100, 100, SCREEN_WIDTH - 100, 300 };
			DoMessageBox( MSG_BOX_BASIC_STYLE, msg,  guiCurrentScreen, MSG_BOX_FLAG_FOUR_NUMBERED_BUTTONS | MSG_BOX_FLAG_USE_CENTERING_RECT, overide_callback,  &CenterRect );

	}
	else	ScreenMsg( FONT_LTGREEN, MSG_INTERFACE, L"server only feature" );
	
}

void overide_callback( UINT8 ubResult )
{
	if(is_server)
	{
			if(ubResult==1)
			{
					allowlaptop=true;
			}
			if(ubResult==2)//overide stop #1 (awaiting client ready for launch/load)
			
			
				{	
					goahead=0;
					status=0;//reset
					numready=0;
					SOLDIERTYPE *pSoldier = MercPtrs[ 0 ];
					UINT8 ubGroupID = pSoldier->ubGroupID;

					GROUP *pGroup;
					pGroup = GetGroup( ubGroupID ); 
					gpBattleGroup = pGroup;
						gubPBSectorX = gpBattleGroup->ubSectorX;
						gubPBSectorY = gpBattleGroup->ubSectorY;
						gubPBSectorZ = gpBattleGroup->ubSectorZ;

					gfEnterTacticalPlacementGUI = 1;
				
				

					goahead=1;
					readystage=1;
					ready_struct info; //send
					info.client_num = atoi(CLIENT_NUM);
					info.ready_stage = 1;
					info.status = 1; 		
					
			
				
			
					client->RPC("sendREADY",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
			
					status=0;//reset
					numready=0;
				
					SetCurrentWorldSector( gubPBSectorX, gubPBSectorY, gubPBSectorZ ); //load
				}
			
			
			if(ubResult==3)
			{
			lockui(1);//unlock ui paused while wainting for loaders

			ready_struct info;
			info.client_num = atoi(CLIENT_NUM);
			readystage=0;
			numready=0;
			info.ready_stage = 2;//done placing mercs
			info.status=1;
			client->RPC("sendGUI",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
			
			}
			if(ubResult==4) //overide waiting on merc placement
			{
					numready=0;

					ready_struct info; //send
					info.client_num = atoi(CLIENT_NUM);
					info.ready_stage = 4;//all done placing mercs, kill all
					info.status=1;
					gMsgBox.bHandled = MSG_BOX_RETURN_OK;
					status=0;
					KillTacticalPlacementGUI(); //kill
					ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"... Let the games begin !");

					client->RPC("sendGUI",(const char*)&info, (int)sizeof(ready_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

			}
		

		goahead = 0;
		numready = 0;
		readystage = 0;
		status = 0;
	
	

	}
}

void requestSETTINGS(void)
{
	client_info cl_name;
	cl_name.client_num=atoi(CLIENT_NUM);
	strcpy(cl_name.client_name , CLIENT_NAME);
	cl_name.cl_ops[0]=OP_TEAM_1;
	cl_name.cl_ops[1]=OP_TEAM_2;
	cl_name.cl_ops[2]=OP_TEAM_3;
	cl_name.cl_ops[3]=OP_TEAM_4;

	client->RPC("requestSETTINGS",(const char*)&cl_name, (int)sizeof(client_info)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

}




void recieveSETTINGS (RPCParameters *rpcParameters) //recive settings from server
{

	settings_struct* cl_lan = (settings_struct*)rpcParameters->input;

				char szDefault[30];
				sprintf(szDefault, "%s",cl_lan->client_name);

		if(!recieved_settings)
		{
			recieved_settings=1;

			memcpy( client_names , cl_lan->client_names, sizeof( char ) * 4 * 30 );
			
			strcpy(client_names[cl_lan->client_num-1],szDefault);

		
			ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"Recieved Game Settings:" );
			

			MAX_CLIENTS=cl_lan->max_clients;
			INTERRUPTS=cl_lan->interrupts;
			DAMAGE_MULTIPLIER=cl_lan->damage_multiplier;
			SAME_MERC=cl_lan->same_merc;
			

			gsMercArriveSectorX=cl_lan->gsMercArriveSectorX;
			gsMercArriveSectorY=cl_lan->gsMercArriveSectorY;

			PLAYER_BSIDE=cl_lan->gsPLAYER_BSIDE;

			ChangeSelectedMapSector( gsMercArriveSectorX, gsMercArriveSectorY, 0 );
			CHAR16 str[128];
			GetSectorIDString( gsMercArriveSectorX, gsMercArriveSectorY, 0, str, TRUE );
			//new  ---------
			gGameOptions.fTurnTimeLimit=cl_lan->sofTurnTimeLimit;
			INT32 secs_per_tick=cl_lan->secs_per_tick;
			PLAYER_TEAM_TIMER_SEC_PER_TICKS=secs_per_tick;

			INT32 clstarting_balance=cl_lan->starting_balance;//set starting balance

			if(LaptopSaveInfo.iCurrentBalance<clstarting_balance)
			{
			AddTransactionToPlayersBook( ANONYMOUS_DEPOSIT, 0, GetWorldTotalMin(), clstarting_balance-LaptopSaveInfo.iCurrentBalance );
			}
			else
			{
			AddTransactionToPlayersBook( TRANSACTION_FEE, 0, GetWorldTotalMin(), clstarting_balance-LaptopSaveInfo.iCurrentBalance );
			}

			gGameOptions.fGunNut=cl_lan->sofGunNut;	
			gGameOptions.ubGameStyle=cl_lan->soubGameStyle;
			gGameOptions.ubDifficultyLevel=cl_lan->soubDifficultyLevel;
			
			gGameOptions.fIronManMode=cl_lan->sofIronManMode;
			gGameOptions.ubBobbyRay=cl_lan->soubBobbyRay;

			if(!cl_lan->soDis_Bobby)LaptopSaveInfo.fBobbyRSiteCanBeAccessed = TRUE;

			if(!cl_lan->soDis_Equip)ALLOW_EQUIP=TRUE;

			MAX_MERCS=cl_lan->gsMAX_MERCS;
			TESTING=cl_lan->TESTING;

			ScreenMsg( FONT_YELLOW, MSG_CHAT, L"Sector= %s, Max Clients= %d, Max Mercs= %d, Co-Op= %d Interrupts= %d, Same Merc= %d, Damage Multiplier= %f,  Enemy= %d, Creature= %d, Militia= %d, Civilian= %d, Timed Turns= %d, Secs/Tic= %d, Starting Cash= $%d, Tons of Guns= %d, Sci-Fi= %d, Difficulty= %d, Iron-Man= %d, BobbyRays Range= %d, Disable BobbyRays= %d, Disable Aim/Merc Equip= %d, Testing= %d.",str,MAX_CLIENTS,MAX_MERCS,PLAYER_BSIDE,INTERRUPTS,SAME_MERC,DAMAGE_MULTIPLIER,cl_lan->ENEMY_ENABLED,cl_lan->CREATURE_ENABLED,cl_lan->MILITIA_ENABLED,cl_lan->CIV_ENABLED,gGameOptions.fTurnTimeLimit,secs_per_tick,clstarting_balance,gGameOptions.fGunNut,gGameOptions.ubGameStyle,gGameOptions.ubDifficultyLevel,gGameOptions.fIronManMode,gGameOptions.ubBobbyRay,cl_lan->soDis_Bobby,cl_lan->soDis_Equip,TESTING);
			if(TESTING)	ScreenMsg( FONT_WHITE, MSG_CHAT, L"TESTING AND CHEAT FUNCTION '9' IS ENABLED" );
		}
		else 
		{
			
			
		ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"Client #%d: '%S' has connected,",cl_lan->client_num,szDefault );
		if(PLAYER_BSIDE==0)ScreenMsg(FONT_LTGREEN,MSG_CHAT,L"-> Opposed with Client: #1=%d, #2=%d, #3=%d, #4=%d.",cl_lan->cl_ops[0],cl_lan->cl_ops[1],cl_lan->cl_ops[2],cl_lan->cl_ops[3]);
	
				strcpy(client_names[cl_lan->client_num-1],szDefault);				

		}

}


void send_bullet(  BULLET * pBullet,UINT16 usHandItem )
{	
	netb_struct netb;
	netb.net_bullet=*pBullet;
	netb.usHandItem=usHandItem;

	//ScreenMsg( FONT_YELLOW, MSG_CHAT, L"Sent Bullet Id: %d",pBullet->iBullet);

	if(pBullet->ubTargetID < 20)netb.net_bullet.ubTargetID = netb.net_bullet.ubTargetID+ubID_prefix;
	if(pBullet->ubFirerID < 20)netb.net_bullet.ubFirerID = netb.net_bullet.ubFirerID+ubID_prefix;
				
	
	client->RPC("sendBULLET",(const char*)&netb, (int)sizeof(netb_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

}

void recieveBULLET(RPCParameters *rpcParameters)
{
		netb_struct* netb = (netb_struct*)rpcParameters->input;

		INT32 net_iBullet=netb->net_bullet.iBullet;

		SOLDIERTYPE * pFirer=MercPtrs[ netb->net_bullet.ubFirerID ];

		INT8 bTeam=pFirer->bTeam;

		INT32 iBullet;
		BULLET * pBullet;

		iBullet = CreateBullet( netb->net_bullet.ubFirerID, 0, netb->net_bullet.usFlags,netb->usHandItem );
		if (iBullet == -1)
		{
			ScreenMsg( FONT_YELLOW, MSG_CHAT, L"Failed to create bullet");		
		}
//add bullet to bullet table for translation

		bTable[bTeam][net_iBullet].remote_id = net_iBullet;
		bTable[bTeam][net_iBullet].local_id = iBullet;

		pBullet = GetBulletPtr( iBullet );

		//ScreenMsg( FONT_YELLOW, MSG_CHAT, L"Created Bullet Id: %d",iBullet);		

		pBullet->fCheckForRoof=netb->net_bullet.fCheckForRoof;
		pBullet->qIncrX=netb->net_bullet.qIncrX;
		pBullet->qIncrY=netb->net_bullet.qIncrY;
		pBullet->qIncrZ=netb->net_bullet.qIncrZ;
		pBullet->sHitBy=netb->net_bullet.sHitBy;
		pBullet->ddHorizAngle=netb->net_bullet.ddHorizAngle;
		pBullet->fAimed=netb->net_bullet.fAimed;
		pBullet->ubItemStatus=netb->net_bullet.ubItemStatus;
		pBullet->qCurrX=netb->net_bullet.qCurrX;
		pBullet->qCurrY=netb->net_bullet.qCurrY;
		pBullet->qCurrZ=netb->net_bullet.qCurrZ;
		pBullet->iImpact=netb->net_bullet.iImpact;
		pBullet->iRange=netb->net_bullet.iRange;
		pBullet->sTargetGridNo=netb->net_bullet.sTargetGridNo;
		pBullet->bStartCubesAboveLevelZ=netb->net_bullet.bStartCubesAboveLevelZ;
		pBullet->bEndCubesAboveLevelZ=netb->net_bullet.bEndCubesAboveLevelZ;
		pBullet->iDistanceLimit=netb->net_bullet.iDistanceLimit;

		FireBullet( pFirer, pBullet, FALSE );

}

void send_changestate (EV_S_CHANGESTATE * SChangeState)
{
	EV_S_CHANGESTATE	new_state;

	memcpy( &new_state , SChangeState, sizeof( EV_S_CHANGESTATE ));

		if(new_state.usSoldierID < 20)new_state.usSoldierID = new_state.usSoldierID+ubID_prefix;
	
			

	client->RPC("sendSTATE",(const char*)&new_state, (int)sizeof(EV_S_CHANGESTATE)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	//ScreenMsg( FONT_YELLOW, MSG_CHAT, L"sent state");		


}

void recieveSTATE(RPCParameters *rpcParameters)
{

	EV_S_CHANGESTATE*	new_state = (EV_S_CHANGESTATE*)rpcParameters->input;


	//ScreenMsg( FONT_YELLOW, MSG_CHAT, L"recieved state");	
	

	SOLDIERTYPE * pSoldier=MercPtrs[ new_state->usSoldierID ];

	if(pSoldier)EVENT_InitNewSoldierAnim( pSoldier, new_state->usNewState, new_state->usStartingAniCode, new_state->fForce );
//AddGameEvent( S_CHANGESTATE, 0, &SChangeState );
	//someday ;)
}

void send_death( SOLDIERTYPE *pSoldier )
{
	death_struct nDeath;
	nDeath.soldier_id=pSoldier->ubID;
	nDeath.attacker_id=pSoldier->ubAttackerID;
		if(pSoldier->ubAttackerID==NULL)nDeath.attacker_id=pSoldier->ubPreviousAttackerID;
		if(pSoldier->ubID<20)nDeath.soldier_id=nDeath.soldier_id+ubID_prefix;
		
	client->RPC("sendDEATH",(const char*)&nDeath, (int)sizeof(death_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		
	SOLDIERTYPE * pAttacker=MercPtrs[ nDeath.attacker_id ];
	INT8 pA_bTeam=pAttacker->bTeam;
	INT8 pS_bTeam=pSoldier->bTeam;

	

	//ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"'%s' (Client #%d: '%S') was killed by '%s' (Client #%d: '%S') ! ",pSoldier->name,pS_bTeam-5,client_names[pS_bTeam-6],pAttacker->name,pS_bTeam-5,client_names[pS_bTeam-6] );
	ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"'%s' #%d: '%S' was killed by '%s' #%d: '%S' ! ",pSoldier->name,(atoi(CLIENT_NUM)),client_names[(atoi(CLIENT_NUM)-1)],pAttacker->name,(pA_bTeam-5),client_names[(pA_bTeam-6)] );

}

void recieveDEATH (RPCParameters *rpcParameters)
{

	death_struct* nDeath = (death_struct*)rpcParameters->input;
	SOLDIERTYPE * pSoldier=MercPtrs[ nDeath->soldier_id ];

	UINT16 ubAttackerID;
					if((nDeath->attacker_id >= ubID_prefix) && (nDeath->attacker_id < (ubID_prefix+5)))
						ubAttackerID = (nDeath->attacker_id - ubID_prefix);
					else
						ubAttackerID = nDeath->attacker_id;

	SOLDIERTYPE * pAttacker=MercPtrs[ ubAttackerID ];

	INT8 pA_bTeam=pAttacker->bTeam;
	INT8 pS_bTeam=pSoldier->bTeam;
	if(pA_bTeam>5)pA_bTeam=pA_bTeam-5;
	if(pA_bTeam==0)pA_bTeam=atoi(CLIENT_NUM);
		

	if(pSoldier->bLife!=0)
	{
		pSoldier->usAnimState=50;
		ScreenMsg( FONT_YELLOW, MSG_CHAT, L"made corpse/dead from remote client");	
		TurnSoldierIntoCorpse( pSoldier, TRUE, TRUE );
		ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"'%s' #%d: '%S' was killed by '%s' #%d: '%S' ! ",pSoldier->name,(pS_bTeam-5),client_names[(pS_bTeam-6)],pAttacker->name,(pA_bTeam),client_names[(pA_bTeam-1)] );

	}

}

void send_hitstruct(EV_S_STRUCTUREHIT	*	SStructureHit)
{
	EV_S_STRUCTUREHIT struct_hit;
	memcpy( &struct_hit , SStructureHit, sizeof( EV_S_STRUCTUREHIT ));
	if(SStructureHit->ubAttackerID <20)struct_hit.ubAttackerID = struct_hit.ubAttackerID+ubID_prefix;
			

	client->RPC("sendhitSTRUCT",(const char*)&struct_hit, (int)sizeof(EV_S_STRUCTUREHIT)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

}

void send_hitwindow(EV_S_WINDOWHIT * SWindowHit)
{
		EV_S_WINDOWHIT window_hit;
	memcpy( &window_hit , SWindowHit, sizeof( EV_S_WINDOWHIT ));
	if(SWindowHit->ubAttackerID <20)window_hit.ubAttackerID = window_hit.ubAttackerID+ubID_prefix;
			

	client->RPC("sendhitWINDOW",(const char*)&window_hit, (int)sizeof(EV_S_WINDOWHIT)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void send_miss(EV_S_MISS * SMiss)
{

		EV_S_MISS shot_miss;
	memcpy( &shot_miss , SMiss, sizeof( EV_S_MISS ));
	if(SMiss->ubAttackerID <20)shot_miss.ubAttackerID = shot_miss.ubAttackerID+ubID_prefix;
			

	client->RPC("sendMISS",(const char*)&shot_miss, (int)sizeof(EV_S_MISS)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void recievehitSTRUCT  (RPCParameters *rpcParameters)
{
	EV_S_STRUCTUREHIT* struct_hit = (EV_S_STRUCTUREHIT*)rpcParameters->input;
		
		SOLDIERTYPE *pSoldier = MercPtrs[ struct_hit->ubAttackerID ];
		INT8 bTeam=pSoldier->bTeam;
		INT32 iBullet = bTable[bTeam][struct_hit->iBullet].local_id;

	StructureHit( iBullet, struct_hit->usWeaponIndex, struct_hit->bWeaponStatus, struct_hit->ubAttackerID, struct_hit->sXPos, struct_hit->sYPos, struct_hit->sZPos, struct_hit->usStructureID, struct_hit->iImpact, struct_hit->fStopped );
	if(struct_hit->fStopped)RemoveBullet(iBullet);
	//ScreenMsg( FONT_YELLOW, MSG_CHAT, L"recieved structure hit");
}
void recievehitWINDOW  (RPCParameters *rpcParameters)
{
	EV_S_WINDOWHIT* window_hit = (EV_S_WINDOWHIT*)rpcParameters->input;



	WindowHit( window_hit->sGridNo, window_hit->usStructureID, window_hit->fBlowWindowSouth, window_hit->fLargeForce );

	//ScreenMsg( FONT_YELLOW, MSG_CHAT, L"recieved window hit");
}
void recieveMISS  (RPCParameters *rpcParameters)
{
	EV_S_MISS* shot_miss = (EV_S_MISS*)rpcParameters->input;

		SOLDIERTYPE *pSoldier = MercPtrs[ shot_miss->ubAttackerID ];
		INT8 bTeam=pSoldier->bTeam;
		INT32 iBullet = bTable[bTeam][shot_miss->iBullet].local_id;


	ShotMiss( shot_miss->ubAttackerID, iBullet );
	//ScreenMsg( FONT_YELLOW, MSG_CHAT, L"recieved shot miss");	
}

void cheat_func(void)
{
	if(TESTING)
	{
	ScreenMsg( FONT_YELLOW, MSG_CHAT, L"gTacticalStatus.uiFlags |= SHOW_ALL_MERCS");	
	gTacticalStatus.uiFlags |= SHOW_ALL_MERCS;
	}
}

void start_tt(void)
{
	if(is_server)
	{
		ScreenMsg( FONT_YELLOW, MSG_CHAT, L"manually starting turnbased mode...");	

		gTacticalStatus.uiFlags |= INCOMBAT;
		EndTurn (START_TEAM_TURN);
	}
    
}

void unlock (void)
{

	ScreenMsg( FONT_YELLOW, MSG_CHAT, L"unlocking ui...");	
		guiPendingOverrideEvent = LU_ENDUILOCK;
					guiPendingOverrideEvent = LA_ENDUIOUTURNLOCK;
	

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
			REGISTER_STATIC_RPC(client, recieveINTERRUPT);
			REGISTER_STATIC_RPC(client, recieveREADY);
			REGISTER_STATIC_RPC(client, recieveGUI);
			REGISTER_STATIC_RPC(client, recieveSETTINGS);
			REGISTER_STATIC_RPC(client, recieveBULLET);
			REGISTER_STATIC_RPC(client, recieveSTATE);
			REGISTER_STATIC_RPC(client, recieveDEATH);
			REGISTER_STATIC_RPC(client, recievehitSTRUCT);
			REGISTER_STATIC_RPC(client, recievehitWINDOW);
			REGISTER_STATIC_RPC(client, recieveMISS);
			

			
		
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

			recieved_settings=0;
			goahead = 0;
			numready = 0;
			readystage = 0;
			status = 0;
			gTacticalStatus.uiFlags&= (~SHOW_ALL_MERCS );

			//retrieve settings from Ja2_mp.ini
			char ip[30];
			char port[30];
			char client_number[30];
			char sector_edge[30];

			


			char c_x[30];
			char c_y[30];
		
			char clname[30];



			MAX_CLIENTS=0;//reset server only set settings.
			INTERRUPTS=0;
			DAMAGE_MULTIPLIER=0;
			SAME_MERC=0;

			GetPrivateProfileString( "Ja2_mp Settings","SERVER_IP", "", ip, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","SERVER_PORT", "", port, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","CLIENT_NUM", "", client_number, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","SECTOR_EDGE", "", sector_edge, MAX_PATH, "..\\Ja2_mp.ini" );


			GetPrivateProfileString( "Ja2_mp Settings","CRATE_X", "", c_x, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","CRATE_Y", "", c_y, MAX_PATH, "..\\Ja2_mp.ini" );

			GetPrivateProfileString( "Ja2_mp Settings","CLIENT_NAME", "", clname, MAX_PATH, "..\\Ja2_mp.ini" );

		

	

			char op1[30];
			char op2[30];
			char op3[30];
			char op4[30];

			GetPrivateProfileString( "Ja2_mp Settings","OP_TEAM_1", "", op1, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","OP_TEAM_2", "", op2, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","OP_TEAM_3", "", op3, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","OP_TEAM_4", "", op4, MAX_PATH, "..\\Ja2_mp.ini" );
char stt[30];
			GetPrivateProfileString( "Ja2_mp Settings","START_TEAM_TURN", "", stt, MAX_PATH, "..\\Ja2_mp.ini" );


			OP_TEAM_1=atoi(op1);
				OP_TEAM_2=atoi(op2);
					OP_TEAM_3=atoi(op3);
						OP_TEAM_4=atoi(op4);

			strcpy( CLIENT_NAME, clname);

			strcpy( SERVER_IP , ip );
			strcpy( SERVER_PORT, port );
			strcpy( CLIENT_NUM, client_number );
			strcpy( SECT_EDGE, sector_edge);
			
			START_TEAM_TURN=atoi(stt);
			crate_sGridX = atoi(c_x);
			crate_sGridY = atoi(c_y);

			netbTeam = (atoi(CLIENT_NUM))+5;
			ubID_prefix = gTacticalStatus.Team[ netbTeam ].bFirstID;




			//**********************
			//here some nifty little tweaks

				LaptopSaveInfo.guiNumberOfMercPaymentsInDays += 20;
				LaptopSaveInfo.gubLastMercIndex = NUMBER_MERCS_AFTER_FOURTH_MERC_ARRIVES;
				
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
				requestSETTINGS();
				//request_settings();//ask server for game settings...
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
	
	allowlaptop=false;

			

	// We're done with the network
	RakNetworkFactory::DestroyRakPeerInterface(client);
	ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"client disconnected and shutdown");
	}
	else
	{
	ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"client is not running");
	}
}