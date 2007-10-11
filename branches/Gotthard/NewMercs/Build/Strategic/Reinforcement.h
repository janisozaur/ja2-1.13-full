#ifndef __REINFORCEMENT_H__
#define __REINFORCEMENT_H__


#define IS_ONLY_IN_CITIES ( gGameExternalOptions.gfAllowReinforcementsOnlyInCity ? TRUE: FALSE )


//For Autoresolve (mostly)
void GetNumberOfEnemiesInFiveSectors( INT16 sSectorX, INT16 sSectorY, UINT16 *pubNumAdmins, UINT16 *pubNumTroops, UINT16 *pubNumElites );
BOOLEAN IsGroupInARightSectorToReinforce( GROUP *pGroup, INT16 sSectorX, INT16 sSectorY );
UINT8 GetAdjacentSectors( UINT8 pSectors[4], INT16 sSectorX, INT16 sSectorY );
UINT16 CountAllMilitiaInFiveSectors(INT16 sMapX, INT16 sMapY);
UINT16 MilitiaInFiveSectorsOfRank(INT16 sMapX, INT16 sMapY, UINT8 ubRank);
BOOLEAN ARRemoveMilitiaMan( INT16 sMapX, INT16 sMapY, UINT8 ubRank );
UINT16 NumEnemiesInFiveSectors( INT16 sMapX, INT16 sMapY );

//For Tactical
UINT16 DoReinforcementAsPendingEnemy( INT16 sMapX, INT16 sMapY );
void AddPossiblePendingMilitiaToBattle();
GROUP* GetEnemyGroupInSector( INT16 sMapX, INT16 sMapY );

#endif