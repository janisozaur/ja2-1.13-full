#ifndef __QUEEN_COMMAND_H
#define __QUEEN_COMMAND_H

#include "types.h"
#include "Campaign Types.h"
#include "Soldier Control.h"
#include "FileMan.h"
#include "Strategic Movement.h"



extern BOOLEAN gfPendingEnemies;


UINT16 NumFreeEnemySlots();

//Counts enemies and crepitus, but not bloodcats.
UINT16 NumHostilesInSector( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ );

UINT16 NumEnemiesInAnySector( INT16 sSectorX, INT16 sSectorY, INT16 sSectorZ );

UINT16 NumEnemiesInSector( INT16 sSectorX, INT16 sSectorY );
UINT16 NumStationaryEnemiesInSector( INT16 sSectorX, INT16 sSectorY );
UINT16 NumMobileEnemiesInSector( INT16 sSectorX, INT16 sSectorY );
void GetNumberOfMobileEnemiesInSector( INT16 sSectorX, INT16 sSectorY, UINT16 *pubNumAdmins, UINT16 *pubNumTroops, UINT16 *pubNumElites );
void GetNumberOfMobileEnemiesInSectorWithoutRoadBlock( INT16 sSectorX, INT16 sSectorY, UINT16 *pubNumAdmins, UINT16 *pubNumTroops, UINT16 *pubNumElites );
void GetNumberOfStationaryEnemiesInSector( INT16 sSectorX, INT16 sSectorY, UINT16 *pubNumAdmins, UINT16 *pubNumTroops, UINT16 *pubNumElites );
void GetNumberOfEnemiesInSector( INT16 sSectorX, INT16 sSectorY, UINT16 *pubNumAdmins, UINT16 *pubNumTroops, UINT16 *pubNumElites );

//Called when entering a sector so the campaign AI can automatically insert the
//correct number of troops of each type based on the current number in the sector
//in global focus (gWorldSectorX/Y)
BOOLEAN PrepareEnemyForSectorBattle();
BOOLEAN PrepareEnemyForUndergroundBattle();

void AddEnemiesToBattle( GROUP *pGroup, UINT8 ubStrategicInsertionCode, UINT16 ubNumAdmins, UINT16 ubNumTroops, UINT16 ubNumElites, BOOLEAN fMagicallyAppeared );
void AddPossiblePendingEnemiesToBattle();
void EndTacticalBattleForEnemy();

void ProcessQueenCmdImplicationsOfDeath( SOLDIERTYPE *pSoldier );

void HandleEnemyStatusInCurrentMapBeforeLoadingNewMap();
BOOLEAN SaveUnderGroundSectorInfoToSaveGame( HWFILE hFile );
BOOLEAN LoadUnderGroundSectorInfoFromSavedGame( HWFILE hFile );

//Finds and returns the specified underground structure ( DONT MODIFY IT ).  Else returns NULL
UNDERGROUND_SECTORINFO* FindUnderGroundSector( INT16 sMapX, INT16 sMapY, UINT8 bMapZ );

void EnemyCapturesPlayerSoldier( SOLDIERTYPE *pSoldier );
void BeginCaptureSquence( );
void EndCaptureSequence( );

BOOLEAN PlayerSectorDefended( UINT8 ubSectorID );

BOOLEAN OnlyHostileCivsInSector();

extern INT32 gsInterrogationGridNo[3];

#endif