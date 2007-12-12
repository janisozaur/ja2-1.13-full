#pragma once


#include "Soldier Init List.h"
#include "Merc Hiring.h"

static bool is_connected;
static bool is_connecting;
static bool is_client;
static bool is_server;
static bool is_networked;

void test_function ( void );


void connect_client ( void );
void start_server (void);
void client_packet ( void );
void server_packet ( void );

void server_disconnect (void);
void client_disconnect (void);

void send_path (  SOLDIERTYPE *pSoldier, UINT16 sDestGridNo, UINT16 usMovementAnim, BOOLEAN fFromUI, BOOLEAN fForceRestartAnim  );
void send_stance ( SOLDIERTYPE *pSoldier, UINT8 ubDesiredStance );
void send_dir ( SOLDIERTYPE *pSoldier, UINT16 usDesiredDirection );
void send_fire( SOLDIERTYPE *pSoldier, INT16 sTargetGridNo );
void send_hit( UINT16 usSoldierID, UINT16 usWeaponIndex, INT16 sDamage, INT16 sBreathLoss, UINT16 usDirection, INT16 sXPos, INT16 sYPos, INT16 sZPos, INT16 sRange , UINT8 ubAttackerID, BOOLEAN fHit, UINT8 ubSpecial, UINT8 ubHitLocation );

void send_hire( UINT8 ubCurrentSoldier, INT16 iTotalContractLength, BOOLEAN fCopyProfileItemsOver);

static bool repo;

