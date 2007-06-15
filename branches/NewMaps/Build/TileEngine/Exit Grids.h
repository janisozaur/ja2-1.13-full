#ifndef __EXIT_GRIDS_H
#define __EXIT_GRIDS_H

#include "Fileman.h"
#include "Worlddef.h"

typedef struct //for exit grids (object level)
{ //if an item pool is also in same gridno, then this would be a separate levelnode
	//in the object level list
	INT32 usGridNo; //sweet spot for placing mercs in new sector.
	UINT8 ubGotoSectorX;
	UINT8 ubGotoSectorY;
	UINT8 ubGotoSectorZ;
}EXITGRID;

BOOLEAN	ExitGridAtGridNo( UINT32 usMapIndex );
BOOLEAN	GetExitGridLevelNode( UINT32 usMapIndex, LEVELNODE **ppLevelNode );
BOOLEAN	GetExitGrid( UINT32 usMapIndex, EXITGRID *pExitGrid );

void AddExitGridToWorld( INT32 iMapIndex, EXITGRID *pExitGrid );
void RemoveExitGridFromWorld( INT32 iMapIndex );

void SaveExitGrids( HWFILE fp, UINT16 usNumExitGrids );
void LoadExitGrids( INT8 **hBuffer );

void AttemptToChangeFloorLevel( INT8 bRelativeZLevel );

extern EXITGRID gExitGrid;
extern BOOLEAN gfOverrideInsertionWithExitGrid;

// Finds closest ExitGrid of same type as is at gridno, within a radius.  Checks
// valid paths, destinations, etc.
INT32 FindGridNoFromSweetSpotCloseToExitGrid( SOLDIERTYPE *pSoldier, INT32 sSweetGridNo, INT8 ubRadius, UINT8 *pubDirection );

INT32 FindClosestExitGrid( SOLDIERTYPE *pSoldier, INT32 sSrcGridNo, INT8 ubRadius );

#endif
