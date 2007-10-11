#ifndef _STRATEGIC_MERC_HANDLER_H_
#define _STRATEGIC_MERC_HANDLER_H_


void StrategicHandlePlayerTeamMercDeath( SOLDIERTYPE *pSoldier );
void MercDailyUpdate();
void MercsContractIsFinished( INT16 ubID );
void RPCWhineAboutNoPay( INT16	ubID );
void MercComplainAboutEquipment( INT16 ubProfileID );
BOOLEAN SoldierHasWorseEquipmentThanUsedTo( SOLDIERTYPE *pSoldier );
void UpdateBuddyAndHatedCounters( void );
void HourlyCamouflageUpdate( void );
#endif
