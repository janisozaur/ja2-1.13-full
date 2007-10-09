#ifndef __CREATURE_SPREADING_H
#define __CREATURE_SPREADING_H

void InitCreatureQuest();
void SpreadCreatures();
void DecayCreatures();
void ChooseCreatureQuestStartDay();
void ClearCreatureQuest();
void DeleteCreatureDirectives();

BOOLEAN SaveCreatureDirectives( HWFILE hFile );
BOOLEAN LoadCreatureDirectives( HWFILE hFile, UINT32 uiSavedGameVersion );

UINT8 CreaturesInUndergroundSector( UINT8 ubSectorID, UINT8 ubSectorZ );
BOOLEAN PrepareCreaturesForBattle();
void CreatureNightPlanning();
void CreatureAttackTown( UINT8 ubSectorID, BOOLEAN fOverrideTest );

void CheckConditionsForTriggeringCreatureQuest( INT16 sSectorX, INT16 sSectorY, INT8 bSectorZ );

void ForceCreaturesToAvoidMineTemporarily( UINT8 ubMineIndex );

extern BOOLEAN gfUseCreatureMusic;

BOOLEAN MineClearOfMonsters( UINT8 ubMineIndex );

//Returns TRUE if valid and creature quest over, FALSE if creature quest active or not yet started
BOOLEAN GetWarpOutOfMineCodes( INT16 *psSectorX, INT16 *psSectorY, INT8 *pbSectorZ, INT32 *psInsertionGridNo );

extern INT16 gsCreatureInsertionCode;
extern INT32 gsCreatureInsertionGridNo;
extern UINT16 gubNumCreaturesAttackingTown;
extern UINT16 gubYoungMalesAttackingTown;
extern UINT16 gubYoungFemalesAttackingTown;
extern UINT16 gubAdultMalesAttackingTown;
extern UINT16 gubAdultFemalesAttackingTown;
extern UINT8 gubSectorIDOfCreatureAttack;
enum{
	CREATURE_BATTLE_CODE_NONE,
	CREATURE_BATTLE_CODE_TACTICALLYADD,
	CREATURE_BATTLE_CODE_TACTICALLYADD_WITHFOV,
	CREATURE_BATTLE_CODE_PREBATTLEINTERFACE,
	CREATURE_BATTLE_CODE_AUTORESOLVE,
};
extern UINT8 gubCreatureBattleCode;

void DetermineCreatureTownComposition( UINT16 ubNumCreatures, UINT16 *pubNumYoungMales, UINT16 *pubNumYoungFemales,	UINT16 *pubNumAdultMales, UINT16 *pubNumAdultFemales );																			

void DetermineCreatureTownCompositionBasedOnTacticalInformation( UINT16 *pubNumCreatures, UINT16 *pubNumYoungMales, UINT16 *pubNumYoungFemales, UINT16 *pubNumAdultMales, UINT16 *pubNumAdultFemales );																		


BOOLEAN PlayerGroupIsInACreatureInfestedMine();

#endif
