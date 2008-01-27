#pragma once


#include "Soldier Init List.h"
#include "Merc Hiring.h"
#include "event pump.h"

extern bool is_connected;
extern bool is_connecting;
extern bool is_client;
extern bool is_server;
extern bool is_networked;

extern PLAYER_TEAM_TIMER_SEC_PER_TICKS;

extern char CLIENT_NUM[30];

extern int ENEMY_ENABLED;
extern int CREATURE_ENABLED;
extern int MILITIA_ENABLED;
extern int CIV_ENABLED;

extern int MAX_CLIENTS;
extern int SAME_MERC;

extern bool allowlaptop;

extern UINT8 netbTeam;
extern UINT8 ubID_prefix;
extern FLOAT DAMAGE_MULTIPLIER;

extern UINT16 crate_usMapPos;

//extern int INTERRUPTS;

extern int ALLOW_EQUIP;

extern INT32 MAX_MERCS;

void start_battle ( void );
void DropOffItemsInSector( UINT8 ubOrderNum );

void test_func2 ( void );

void mp_help (void);
void mp_help2 (void);
void grid_display ( void);

void send_loaded (void);
void send_donegui ( UINT8 ubResult );
void lockui (bool unlock);

UINT8 numenemyLAN( UINT8 ubSectorX, UINT8 ubSectorY );

void connect_client ( void );
void start_server (void);
void client_packet ( void );
void server_packet ( void );

void server_disconnect (void);
void client_disconnect (void);

void DialogRemoved( UINT8 ubResult );
void manual_overide(void);

void send_path (  SOLDIERTYPE *pSoldier, UINT16 sDestGridNo, UINT16 usMovementAnim, BOOLEAN fFromUI, BOOLEAN fForceRestartAnim  );
void send_stance ( SOLDIERTYPE *pSoldier, UINT8 ubDesiredStance );
void send_dir ( SOLDIERTYPE *pSoldier, UINT16 usDesiredDirection );
void send_fire( SOLDIERTYPE *pSoldier, INT16 sTargetGridNo );
void send_hit(  EV_S_WEAPONHIT *SWeaponHit  );

void send_hire( UINT8 iNewIndex, UINT8 ubCurrentSoldier, INT16 iTotalContractLength, BOOLEAN fCopyProfileItemsOver);

void send_gui_pos(SOLDIERTYPE *pSoldier,  FLOAT dNewXPos, FLOAT dNewYPos);
void send_gui_dir(SOLDIERTYPE *pSoldier, UINT16	usNewDirection);

void send_EndTurn( UINT8 ubNextTeam );

void send_AI( SOLDIERCREATE_STRUCT *pCreateStruct, UINT8 *pubID );

void send_stop (EV_S_STOP_MERC *SStopMerc);

void send_interrupt(SOLDIERTYPE *pSoldier);

BOOLEAN CheckConditionsForBattle( GROUP *pGroup ); // this comes from strategic movement.cpp

extern char client_names[4][30];

void send_settings();
