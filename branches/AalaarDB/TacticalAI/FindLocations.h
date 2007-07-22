#ifndef FIND_LOCATIONS_H
#define FIND_LOCATIONS_H

#include "Soldier Control.h"

#define MINIMUM_REQUIRED_STATUS 70

UINT8 NumberOfTeamMatesAdjacent( SOLDIERTYPE * pSoldier, INT16 sGridNo );
INT8 CalcWorstCTGTForPosition( SOLDIERTYPE * pSoldier, UINT8 ubOppID, INT16 sOppGridNo, INT8 bLevel, INT32 iMyAPsLeft );
INT8 CalcBestCTGT( SOLDIERTYPE *pSoldier, UINT8 ubOppID, INT16 sOppGridNo, INT8 bLevel, INT32 iMyAPsLeft );
INT8 CalcAverageCTGTForPosition( SOLDIERTYPE * pSoldier, UINT8 ubOppID, INT16 sOppGridNo, INT8 bLevel, INT32 iMyAPsLeft );
INT32 CalcPercentBetter(INT32 iOldValue, INT32 iNewValue, INT32 iOldScale, INT32 iNewScale);
void AICenterXY( INT16 sGridNo, FLOAT * pdX, FLOAT * pdY );
INT8 SearchForItems( SOLDIERTYPE * pSoldier, INT8 bReason, UINT16 usItem );
BOOLEAN CanClimbFromHere (SOLDIERTYPE * pSoldier, BOOLEAN fUp );
INT32 CalcCoverValue(SOLDIERTYPE *pMe, INT16 sMyGridNo, INT32 iMyThreat, INT32 iMyAPsLeft,
					UINT32 uiThreatIndex, INT32 iRange, INT32 morale, INT32 *iTotalScale);


INT16 FindBestNearbyCover(SOLDIERTYPE *pSoldier, INT32 morale, INT32 *piPercentBetter);
INT16 FindBestNearbyCover(SOLDIERTYPE *pSoldier);
INT16 FindSpotMaxDistFromOpponents(SOLDIERTYPE *pSoldier);
INT16 FindNearestUngassedLand(SOLDIERTYPE *pSoldier);
INT16 FindNearbyDarkerSpot( SOLDIERTYPE *pSoldier );
INT16 FindClosestDoor( SOLDIERTYPE * pSoldier );
INT16 FindNearestEdgepointOnSpecifiedEdge( INT16 sGridNo, INT8 bEdgeCode );
INT16 FindNearestEdgePoint( INT16 sGridNo );
INT16 FindNearbyPointOnEdgeOfMap( SOLDIERTYPE * pSoldier, INT8 * pbDirection );
INT16 FindRouteBackOntoMap( SOLDIERTYPE * pSoldier, INT16 sDestGridNo );
INT16 FindClosestBoxingRingSpot( SOLDIERTYPE * pSoldier, BOOLEAN fInRing );
INT16 FindNearestOpenableNonDoor( INT16 sStartGridNo );
INT16 FindFlankingSpot(SOLDIERTYPE *pSoldier, INT16 sPos, INT8 bAction );
INT16 FindClosestClimbPoint (SOLDIERTYPE *pSoldier, BOOLEAN fClimbUp );
INT16 FindBestCoverNearTheGridNo(SOLDIERTYPE *pSoldier, INT16 sGridNo, UINT8 ubSearchRadius );
INT8  FindDirectionForClimbing( INT16 sGridNo );

#endif