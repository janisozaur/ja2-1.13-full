#ifndef __RENDER_FUN_H
#define __RENDER_FUN_H

#include <stdio.h>
#include "worlddef.h"

#define		NO_ROOM			0
#define   MAX_ROOMS		250


UINT8						gubWorldRoomHidden[ MAX_ROOMS ];
UINT8						gubWorldRoomInfo[ WORLD_MAX ];


BOOLEAN InitRoomDatabase( );
void ShutdownRoomDatabase( );

void SetTileRoomNum( INT32 sGridNo, UINT8 ubRoomNum );
void SetTileRangeRoomNum( SGPRect *pSelectRegion, UINT8 ubRoomNum );

void RemoveRoomRoof( UINT32 sGridNo, UINT8 bRoomNum, SOLDIERTYPE *pSoldier );
BOOLEAN InARoom( UINT32 sGridNo, UINT8 *pubRoomNo );
BOOLEAN InAHiddenRoom( UINT32 sGridNo, UINT8 *pubRoomNo );

void SetGridNoRevealedFlag( UINT32 sGridNo );

void ExamineGridNoForSlantRoofExtraGraphic( UINT32 sCheckGridNo );

void SetRecalculateWireFrameFlagRadius(INT16 sX, INT16 sY, INT16 sRadius);


#endif