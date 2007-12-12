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

extern INT8 SquadMovementGroups[ ];
RakPeerInterface *client;

typedef struct
{
	UINT8		ubProfileID;
	INT16		iTotalContractLength;
	BOOLEAN fCopyProfileItemsOver;

	INT8 bTeam;

} send_hire_struct;


//*****************
//RPC sends and recieves:
//********************
void test_function ( void )
{
	
}


//void test_function ( void )
//{
//
//	// Create guy # X //taken from function: QuickCreateProfileMerc ()
//	SOLDIERCREATE_STRUCT		MercCreateStruct;
//	INT16										sWorldX, sWorldY, sSectorX, sSectorY, sGridX, sGridY;
//	UINT8									ubID;
//	UINT16 usMapPos;
//	INT8 bTeam = CIV_TEAM;
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
//			}
//		}
//	}
//}

//void test_function ( void )
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
//	MercCreateStruct.fPlayerMerc					= TRUE;
//	MercCreateStruct.sSectorX							= 9;
//	MercCreateStruct.sSectorY							= 1;
//	MercCreateStruct.bSectorZ							= 0;
//	MercCreateStruct.bTeam								= 0;
//	MercCreateStruct.fCopyProfileItemsOver= 1;
//
//	TacticalCreateSoldier( &MercCreateStruct, &iNewIndex ) ;
//
//	pSoldier = &Menptr[iNewIndex];
//
//	ChangeSoldiersAssignment( pSoldier, 0 );
//	AddPlayerToGroup( SquadMovementGroups[ 0 ], pSoldier  );
//
//}

void send_path (  SOLDIERTYPE *pSoldier, UINT16 sDestGridNo, UINT16 usMovementAnim, BOOLEAN fFromUI, BOOLEAN fForceRestartAnim  )
{
	if(is_client)
	{
		//ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"Sending RPC..." );
		EV_S_GETNEWPATH	SGetNewPath;

		SGetNewPath.usSoldierID				= pSoldier->ubID;
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
	if(is_client)
	{
		EV_S_CHANGESTANCE			SChangeStance;

		SChangeStance.ubNewStance   = ubDesiredStance;
		SChangeStance.usSoldierID  = pSoldier->ubID;
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
	if(is_client)
	{

		EV_S_SETDESIREDDIRECTION	SSetDesiredDirection;

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
	if(is_client)
	{
	
	EV_S_BEGINFIREWEAPON SBeginFireWeapon;

	SBeginFireWeapon.usSoldierID = pSoldier->ubID;
	SBeginFireWeapon.sTargetGridNo = sTargetGridNo;
	SBeginFireWeapon.bTargetLevel = pSoldier->bTargetLevel;
	SBeginFireWeapon.bTargetCubeLevel = pSoldier->bTargetCubeLevel;
	SBeginFireWeapon.uiUniqueId = pSoldier->uiUniqueSoldierIdValue;
	repo=false;	

	client->RPC("sendFIRE",(const char*)&SBeginFireWeapon, (int)sizeof(EV_S_BEGINFIREWEAPON)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
	}
}

void recieveFIRE(RPCParameters *rpcParameters)
{
		
		EV_S_BEGINFIREWEAPON* SBeginFireWeapon = (EV_S_BEGINFIREWEAPON*)rpcParameters->input;
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"SendBeginFireWeaponEvent" );

		SOLDIERTYPE *pSoldier = MercPtrs[ SBeginFireWeapon->usSoldierID ];
					
		repo=true;		
		SendBeginFireWeaponEvent( pSoldier, SBeginFireWeapon->sTargetGridNo );
}



void send_hit( UINT16 usSoldierID, UINT16 usWeaponIndex, INT16 sDamage, INT16 sBreathLoss, UINT16 usDirection, INT16 sXPos, INT16 sYPos, INT16 sZPos, INT16 sRange , UINT8 ubAttackerID, BOOLEAN fHit, UINT8 ubSpecial, UINT8 ubHitLocation )
{

	if (!repo)
	{
		//AddGameEvent( S_WEAPONHIT, 0, &SWeaponHit );
		WeaponHit( usSoldierID, usWeaponIndex, sDamage, sBreathLoss, usDirection, sXPos, sYPos, sZPos, sRange, ubAttackerID, fHit, ubSpecial, ubHitLocation );

	}
	if(is_client && !repo)
		
	{
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"send_hit" );
		/*EV_S_WEAPONHIT* pWeaponHit =  (EV_S_WEAPONHIT*)pEventData;*/

		SOLDIERTYPE *pSoldier = MercPtrs[ usSoldierID ];

		EV_S_WEAPONHIT SWeaponHit;

		SWeaponHit.usSoldierID = usSoldierID;
		SWeaponHit.uiUniqueId = pSoldier->uiUniqueSoldierIdValue;
		SWeaponHit.usWeaponIndex = usWeaponIndex;
        SWeaponHit.sDamage = sDamage;
		SWeaponHit.sBreathLoss = sBreathLoss;
		SWeaponHit.usDirection = usDirection;
		SWeaponHit.sXPos = sXPos;
		SWeaponHit.sYPos = sYPos;
		SWeaponHit.sZPos = sZPos;
		SWeaponHit.sRange = sRange;
		SWeaponHit.ubAttackerID = ubAttackerID;
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
					
		//repo=false;	
		//SWeaponHit->uiUniqueId = pSoldier->uiUniqueSoldierIdValue;
	
		//AddGameEvent( S_WEAPONHIT, 0, &SWeaponHit );
		WeaponHit( SWeaponHit->usSoldierID, SWeaponHit->usWeaponIndex, SWeaponHit->sDamage, SWeaponHit->sBreathLoss, SWeaponHit->usDirection, SWeaponHit->sXPos, SWeaponHit->sYPos, SWeaponHit->sZPos, SWeaponHit->sRange, SWeaponHit->ubAttackerID, SWeaponHit->fHit, SWeaponHit->ubSpecial, SWeaponHit->ubLocation );

		
}

void send_hire( UINT8 ubCurrentSoldier, INT16 iTotalContractLength, BOOLEAN fCopyProfileItemsOver)
{
		if(is_client)
		{
			//attempt at implementing merc setup
			send_hire_struct sHireMerc;

			sHireMerc.ubProfileID=ubCurrentSoldier;
			sHireMerc.iTotalContractLength=iTotalContractLength;
			sHireMerc.fCopyProfileItemsOver=fCopyProfileItemsOver;
			sHireMerc.bTeam=5;


			client->RPC("sendHIRE",(const char*)&sHireMerc, (int)sizeof(send_hire_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);
		}
}

void recieveHIRE(RPCParameters *rpcParameters)
{
	
	send_hire_struct* sHireMerc = (send_hire_struct*)rpcParameters->input;

	//attempt at implementing merc setup

	ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"recieveHIRE" );

}

//***************************
//*** client connection*****
//*************************

void connect_client ( void )
{
	
		if(!is_client && !is_server)
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

			//retrieve settings from .ini
			char ip[30];
			char port[30];
			char SERVER_IP[30] ;
			char SERVER_PORT[30];
			   


			GetPrivateProfileString( "Ja2_mp Settings","SERVER_IP", "", ip, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","SERVER_PORT", "", port, MAX_PATH, "..\\Ja2_mp.ini" );
			strcpy( SERVER_IP , ip );
			strcpy( SERVER_PORT, port );
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
	// We're done with the network
	RakNetworkFactory::DestroyRakPeerInterface(client);
	ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"client disconnected and shutdown");
	}
	else
	{
	ScreenMsg( FONT_LTGREEN, MSG_CHAT, L"client is not running");
	}
}