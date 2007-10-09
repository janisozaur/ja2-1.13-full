#ifndef _SOLDIER_ADD_H
#define _SOLDIER_ADD_H

#include "Soldier Control.h"


// Finds a gridno given a sweet spot
// Returns a good direction too!
INT32	FindGridNoFromSweetSpot( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, INT8 *pubDirection );

// Ensures a good path.....
INT32 FindGridNoFromSweetSpotThroughPeople( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, INT8 *pubDirection );

// Returns a good sweetspot but not the swetspot!
INT32 FindGridNoFromSweetSpotExcludingSweetSpot( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, INT8 *pubDirection );

INT32 FindGridNoFromSweetSpotExcludingSweetSpotInQuardent( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, INT8 *pubDirection, INT8 ubQuardentDir );

// Finds a gridno near a sweetspot but a random one!
INT32 FindRandomGridNoFromSweetSpot( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, INT8 *pubDirection );

// Finds a sweetspot but excluding this one!
INT32 FindRandomGridNoFromSweetSpotExcludingSweetSpot( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, INT8 *pubDirection );


// Adds a soldier ( already created in mercptrs[] array )! 
// Finds a good placement based on data in the loaded sector and if they are enemy's or not, etc...
BOOLEAN AddSoldierToSector( INT16 ubID );

BOOLEAN AddSoldierToSectorNoCalculateDirection( INT16 ubID );

BOOLEAN AddSoldierToSectorNoCalculateDirectionUseAnimation( INT16 ubID, UINT16 usAnimState, UINT16 usAnimCode );

// IsMercOnTeam() checks to see if the passed in Merc Profile ID is currently on the player's team
BOOLEAN IsMercOnTeam(INT16 ubMercID);
// requires non-intransit assignment, too
BOOLEAN IsMercOnTeamAndInOmertaAlready(INT16 ubMercID);
// ATE: Added for contract renewals
BOOLEAN IsMercOnTeamAndAlive(INT16 ubMercID);
// ATE: Added for contract renewals
BOOLEAN IsMercOnTeamAndInOmertaAlreadyAndAlive(INT16 ubMercID);



// GetSoldierIDFromMercID() Gets the Soldier ID from the Merc Profile ID, else returns -1
INT16 GetSoldierIDFromMercID(INT16 ubMercID);

INT32 FindGridNoFromSweetSpotWithStructData( SOLDIERTYPE *pSoldier, UINT16 usAnimState, INT32 sSweetGridNo, INT8 ubRadius, INT8 *pubDirection, BOOLEAN fClosestToMerc );

/*
void SoldierInSectorSleep( SOLDIERTYPE *pSoldier, INT32 sGridNo );
*/

INT32 FindGridNoFromSweetSpotWithStructDataFromSoldier( SOLDIERTYPE *pSoldier, UINT16 usAnimState, INT8 ubRadius, INT8 *pubDirection, BOOLEAN fClosestToMerc, SOLDIERTYPE *pSrcSoldier );

void SoldierInSectorPatient( SOLDIERTYPE *pSoldier, INT32 sGridNo );
void SoldierInSectorDoctor( SOLDIERTYPE *pSoldier, INT32 sGridNo );
void SoldierInSectorRepair( SOLDIERTYPE *pSoldier, INT32 sGridNo );

BOOLEAN CanSoldierReachGridNoInGivenTileLimit( SOLDIERTYPE *pSoldier, INT32 sGridNo, INT16 sMaxTiles, INT8 bLevel );


#endif