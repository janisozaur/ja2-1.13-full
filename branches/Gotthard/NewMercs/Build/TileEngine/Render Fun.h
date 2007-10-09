#ifndef __RENDER_FUN_H
#define __RENDER_FUN_H

#include <stdio.h>
#include "worlddef.h"

#define		NO_ROOM			-1
#define   MAX_ROOMS		2048


extern INT16	gubWorldRoomHidden[ MAX_ROOMS ];
//UINT8						gubWorldRoomInfo[ WORLD_MAX ];
extern INT16*						gubWorldRoomInfo;


BOOLEAN InitRoomDatabase( );
void ShutdownRoomDatabase( );

void SetTileRoomNum( INT32 sGridNo, INT16 ubRoomNum );
void SetTileRangeRoomNum( SGPRect *pSelectRegion, INT16 ubRoomNum );

void RemoveRoomRoof( INT32 sGridNo, INT16 bRoomNum, SOLDIERTYPE *pSoldier );
BOOLEAN InARoom( INT32 sGridNo, INT16 *pubRoomNo );
BOOLEAN InAHiddenRoom( INT32 sGridNo, INT16 *pubRoomNo );

void SetGridNoRevealedFlag( INT32 sGridNo );

void ExamineGridNoForSlantRoofExtraGraphic( INT32 sCheckGridNo );

void SetRecalculateWireFrameFlagRadius(INT16 sX, INT16 sY, INT16 sRadius);


#endif