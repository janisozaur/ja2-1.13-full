#pragma once

//time for a new header :)


typedef struct
{
	UINT16 soldier_id;
	UINT16 attacker_id;
}death_struct;


void send_changestate (EV_S_CHANGESTATE * SChangeState);

void send_death( SOLDIERTYPE *pSoldier);

void send_hitstruct(EV_S_STRUCTUREHIT	*	SStructureHit);
void send_hitwindow(EV_S_WINDOWHIT * SWindowHit);
void send_miss(EV_S_MISS * SMiss);
					
void cheat_func(void);
//void start_tt(void);
void unlock (void);

void UpdateSoldierToNetwork ( SOLDIERTYPE *pSoldier );

extern BOOLEAN gfUIInterfaceSetBusy;

void kick_player (void);
void overide_turn (void);