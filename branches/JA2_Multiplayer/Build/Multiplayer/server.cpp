
#include "MessageIdentifiers.h"
#include "RakNetworkFactory.h"
#include "RakPeerInterface.h"
#include "RakNetStatistics.h"
#include "RakNetTypes.h"
#include "BitStream.h"
#include "RakSleep.h"
#include <assert.h>
#include <cstdio>
#include <cstring>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "types.h"
#include "gamesettings.h"
//#include "test_space.h"

char net_div[30];
INT32 gsMAX_MERCS;


#include "connect.h"
#include "message.h"
#include "network.h"

bool gsDis_Bobby;
bool gsDis_Equip;

int gsSAME_MERC;
float gsDAMAGE_MULTIPLIER;
int gsINTERRUPTS ;
int gsMAX_CLIENTS ;
int gsPLAYER_BSIDE;

INT32 gssecs_per_tick;
INT32 gsstarting_balance;

unsigned char SGetPacketIdentifier(Packet *p);
unsigned char SpacketIdentifier;

RakPeerInterface *server;

// use UNASSIGNED_SYSTEM_ADDRESS instead of rpcParameters->sender to send it back to yourself (the sender)
// there is very little in here dependant on the game engine and originally started out as an independant dedicated server .exe, and could if go ther again ... hayden.
//********* RPC SECTION ************

void sendPATH(RPCParameters *rpcParameters)
{
	server->RPC("recievePATH",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendSTANCE(RPCParameters *rpcParameters)
{
	server->RPC("recieveSTANCE",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendDIR(RPCParameters *rpcParameters)
{
	server->RPC("recieveDIR",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendFIRE(RPCParameters *rpcParameters)
{
	server->RPC("recieveFIRE",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendHIT(RPCParameters *rpcParameters)
{
	server->RPC("recieveHIT",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendHIRE(RPCParameters *rpcParameters)
{
	server->RPC("recieveHIRE",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendguiPOS(RPCParameters *rpcParameters)
{
	server->RPC("recieveguiPOS",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendguiDIR(RPCParameters *rpcParameters)
{
	server->RPC("recieveguiDIR",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendEndTurn(RPCParameters *rpcParameters)
{
	server->RPC("recieveEndTurn",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendAI(RPCParameters *rpcParameters)
{
	server->RPC("recieveAI",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendSTOP(RPCParameters *rpcParameters)
{
	server->RPC("recieveSTOP",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}
void sendINTERRUPT(RPCParameters *rpcParameters)
{
	server->RPC("recieveINTERRUPT",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}
void sendREADY(RPCParameters *rpcParameters)
{
	server->RPC("recieveREADY",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendGUI(RPCParameters *rpcParameters)
{
	server->RPC("recieveGUI",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendBULLET(RPCParameters *rpcParameters)
{
	server->RPC("recieveBULLET",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}

void sendSTATE(RPCParameters *rpcParameters)
{
	server->RPC("recieveSTATE",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}
void sendDEATH(RPCParameters *rpcParameters)
{
	server->RPC("recieveDEATH",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}
void sendhitSTRUCT(RPCParameters *rpcParameters)
{
	server->RPC("recievehitSTRUCT",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}
void sendhitWINDOW(RPCParameters *rpcParameters)
{
	server->RPC("recievehitWINDOW",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}
void sendMISS(RPCParameters *rpcParameters)
{
	server->RPC("recieveMISS",(const char*)rpcParameters->input, (*rpcParameters).numberOfBitsOfData, HIGH_PRIORITY, RELIABLE, 0, rpcParameters->sender, true, 0, UNASSIGNED_NETWORK_ID,0);
}
		

//************************* //UNASSIGNED_SYSTEM_ADDRESS
//START INTERNAL SERVER
//*************************
//void send_settings (void)//send server settings to client
void requestSETTINGS(RPCParameters *rpcParameters)
{

	client_info* clinf = (client_info*)rpcParameters->input;

		settings_struct lan;

		lan.client_num = clinf->client_num;
		strcpy(lan.client_name , clinf->client_name);

		lan.max_clients = gsMAX_CLIENTS;
		lan.interrupts = gsINTERRUPTS;
		lan.damage_multiplier = gsDAMAGE_MULTIPLIER;
		
		lan.same_merc = gsSAME_MERC;
		lan.gsMercArriveSectorX=gsMercArriveSectorX;
		lan.gsMercArriveSectorY=gsMercArriveSectorY;

		lan.ENEMY_ENABLED=ENEMY_ENABLED;
		lan.CREATURE_ENABLED=CREATURE_ENABLED;
		lan.MILITIA_ENABLED=MILITIA_ENABLED;
		lan.CIV_ENABLED=CIV_ENABLED;

		lan.gsPLAYER_BSIDE=gsPLAYER_BSIDE;

		
//something new
				lan.secs_per_tick=gssecs_per_tick;
				lan.soubBobbyRay=gGameOptions.ubBobbyRay;
				lan.sofGunNut=gGameOptions.fGunNut;	
				lan.soubGameStyle=gGameOptions.ubGameStyle;
				lan.soubDifficultyLevel=gGameOptions.ubDifficultyLevel;
				lan.sofTurnTimeLimit=gGameOptions.fTurnTimeLimit;
				lan.sofIronManMode=gGameOptions.fIronManMode;
				lan.starting_balance=gsstarting_balance;

				lan.soDis_Bobby=gsDis_Bobby;
				lan.soDis_Equip=gsDis_Equip;

				lan.gsMAX_MERCS=gsMAX_MERCS;
				
				memcpy( lan.client_names , client_names, sizeof( char ) * 4 * 30 );
				lan.cl_ops[0]=clinf->cl_ops[0];
				lan.cl_ops[1]=clinf->cl_ops[1];
				lan.cl_ops[2]=clinf->cl_ops[2];
				lan.cl_ops[3]=clinf->cl_ops[3];
				//memcpy(lan.cl_ops,clinf->cl_ops,sizeof(int)*4);
////


		server->RPC("recieveSETTINGS",(const char*)&lan, (int)sizeof(settings_struct)*8, HIGH_PRIORITY, RELIABLE, 0, UNASSIGNED_SYSTEM_ADDRESS, true, 0, UNASSIGNED_NETWORK_ID,0);

}


void start_server (void)
{
	if(!is_server)
	{	



			//retrieve settings from .ini


			//char maxclients[30];
			char port[30];
			char SERVER_PORT[30];
			//char MAX_CLIENTS[30] ;

		//	GetPrivateProfileString( "Ja2_mp Settings","MAX_CLIENTS", "", maxclients, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","SERVER_PORT", "", port, MAX_PATH, "..\\Ja2_mp.ini" );
		//	strcpy( MAX_CLIENTS , maxclients );
			strcpy( SERVER_PORT, port );


			char ints[30];
			char maxclients[30];
		
			char hire_same_merc[30];
			char bteam1_enabled[30];
			char bteam2_enabled[30];
			char bteam3_enabled[30];
			char bteam4_enabled[30];

			char player_bside[30];

			char sBalance[30];
			char time_div[30];


			GetPrivateProfileString( "Ja2_mp Settings","SAME_MERC", "", hire_same_merc, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","INTERRUPTS", "", ints, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","MAX_CLIENTS", "", maxclients, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","DAMAGE_MULTIPLIER", "", net_div, MAX_PATH, "..\\Ja2_mp.ini" );
			
			GetPrivateProfileString( "Ja2_mp Settings","ENEMY_ENABLED", "", bteam1_enabled, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","CREATURE_ENABLED", "", bteam2_enabled, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","MILITIA_ENABLED", "", bteam3_enabled, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","CIV_ENABLED", "", bteam4_enabled, MAX_PATH, "..\\Ja2_mp.ini" );

			GetPrivateProfileString( "Ja2_mp Settings","Co_Op", "", player_bside, MAX_PATH, "..\\Ja2_mp.ini" );


//something new

			
			GetPrivateProfileString( "Ja2_mp Settings","STARTING_BALANCE", "", sBalance, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","TIMED_TURN_SECS_PER_TICK", "", time_div, MAX_PATH, "..\\Ja2_mp.ini" );
			
			char dis_bob[30];
			char dis_equip[30];
			char max_merc[30];
			GetPrivateProfileString( "Ja2_mp Settings","DISABLE_BOBBY_RAYS", "", dis_bob, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","DISABLE_AIM_AND_MERC_EQUIP", "", dis_equip, MAX_PATH, "..\\Ja2_mp.ini" );
			GetPrivateProfileString( "Ja2_mp Settings","MAX_MERCS", "", max_merc, MAX_PATH, "..\\Ja2_mp.ini" );

			gsDis_Bobby=false;
			gsDis_Equip=false;

			 if(atoi(dis_bob)==1)gsDis_Bobby=true;
			 if(atoi(dis_equip)==1)gsDis_Equip=true;

			 gsMAX_MERCS=atoi(max_merc);

////
			ENEMY_ENABLED =atoi(bteam1_enabled);
			CREATURE_ENABLED =atoi(bteam2_enabled);
			MILITIA_ENABLED =atoi(bteam3_enabled);
			CIV_ENABLED =atoi(bteam4_enabled);

			gsPLAYER_BSIDE = atoi(player_bside);

			gsSAME_MERC = atoi(hire_same_merc);
			gsDAMAGE_MULTIPLIER =(FLOAT)atof(net_div);
			gsINTERRUPTS = atoi(ints);
			gsMAX_CLIENTS = atoi(maxclients);

			gssecs_per_tick=atoi(time_div) ;
			gsstarting_balance=atoi(sBalance);

			//**********************



		
		ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"Initiating RakNet Server..." );
		
		server=RakNetworkFactory::GetRakPeerInterface();
		bool b = server->Startup(gsMAX_CLIENTS, 30, &SocketDescriptor(atoi(SERVER_PORT),0), 1);
		
		server->SetMaximumIncomingConnections((gsMAX_CLIENTS));

		


	//RPC's
		REGISTER_STATIC_RPC(server, sendPATH);
		REGISTER_STATIC_RPC(server, sendSTANCE);
		REGISTER_STATIC_RPC(server, sendDIR);
		REGISTER_STATIC_RPC(server, sendFIRE);
		REGISTER_STATIC_RPC(server, sendHIT);
		REGISTER_STATIC_RPC(server, sendHIRE);
		REGISTER_STATIC_RPC(server, sendguiPOS);
		REGISTER_STATIC_RPC(server, sendguiDIR);
		REGISTER_STATIC_RPC(server, sendEndTurn);
		REGISTER_STATIC_RPC(server, sendAI);
		REGISTER_STATIC_RPC(server, sendSTOP);
		REGISTER_STATIC_RPC(server, sendINTERRUPT);
		REGISTER_STATIC_RPC(server, sendREADY);
		REGISTER_STATIC_RPC(server, sendGUI);
		REGISTER_STATIC_RPC(server, sendBULLET);
		REGISTER_STATIC_RPC(server, requestSETTINGS);
		REGISTER_STATIC_RPC(server, sendSTATE);
		REGISTER_STATIC_RPC(server, sendDEATH);
		REGISTER_STATIC_RPC(server, sendhitSTRUCT);
		REGISTER_STATIC_RPC(server, sendhitWINDOW);
		REGISTER_STATIC_RPC(server, sendMISS);
		
	//


		if (b)
		{
			ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"Server started, waiting for connections...");
			ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"You must now connect to yourself with the client to play ('2').");
			is_server = true;
		}
		else
		{ 
			ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"Server failed to start.  Terminating.");
		
		}
	}
	else
	{
		ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"Server is allready running.");
	}

		


}

// recieve and process server info packets

void server_packet ( void )
{	
	
	Packet* p;

	
	if (is_server)
	{

	p = server->Receive();

	while(p)
	{
			//continue; // Didn't get any packets

		// We got a packet, get the identifier with our handy function
		SpacketIdentifier = SGetPacketIdentifier(p);
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"packet recieved");
		// Check if this is a network message packet
		switch (SpacketIdentifier)
		{
			case ID_DISCONNECTION_NOTIFICATION:
				  // Connection lost normally
				ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"ID_DISCONNECTION_NOTIFICATION");
				break;
			case ID_ALREADY_CONNECTED:
				// Connection lost normally
				ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"ID_ALREADY_CONNECTED");
				break;
			case ID_REMOTE_DISCONNECTION_NOTIFICATION: // Server telling the clients of another client disconnecting gracefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"ID_REMOTE_DISCONNECTION_NOTIFICATION");
				break;
			case ID_REMOTE_CONNECTION_LOST: // Server telling the clients of another client disconnecting forcefully.  You can manually broadcast this in a peer to peer enviroment if you want.
				ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"ID_REMOTE_CONNECTION_LOST");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION: // Server telling the clients of another client connecting.  You can manually broadcast this in a peer to peer enviroment if you want.
				ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"ID_REMOT/MING_CONNECTION");
				break;
			case ID_CONNECTION_ATTEMPT_FAILED:
				ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"ID_CONNECTION_ATTEMPT_FAILED");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				 // Sorry, the server is full.  I don't do anything here but
				// A real app should tell the user
				ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"ID_NO_FREE_INCOMING_CONNECTIONS");
				break;
			case ID_CONNECTION_LOST:
				// Couldn't deliver a reliable packet - i.e. the other system was abnormally
				// terminated
				ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"ID_CONNECTION_LOST");
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				// This tells the client they have connected
				ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"ID_CONNECTION_REQUEST_ACCEPTED");
				break;
			case ID_NEW_INCOMING_CONNECTION:
				//tells server client has connected
				ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"ID_NEW_INCOMING_CONNECTION");
				//send_settings();//send off server set settings
				break;
			case ID_MODIFIED_PACKET:
				// Cheater!
				ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"ID_MODIFIED_PACKET");
				break;
			default:
					
				ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"** a packet has been recieved for which i dont know what to do... **");
				break;
		}


		// We're done with the packet, get more :)
		server->DeallocatePacket(p);
		p = server->Receive();
	}
	}
}
// Copied from Multiplayer.cpp
// If the first byte is ID_TIMESTAMP, then we want the 5th byte
// Otherwise we want the 1st byte
unsigned char SGetPacketIdentifier(Packet *p)
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

void server_disconnect (void)
{
	if(is_server)
	{
	server->Shutdown(300);
	is_server = false;
	// We're done with the network
	RakNetworkFactory::DestroyRakPeerInterface(server);
	ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"server disconnected and shutdown");
	}
	else
	{
	ScreenMsg( FONT_LTBLUE, MSG_CHAT, L"server is not running");
	}
}