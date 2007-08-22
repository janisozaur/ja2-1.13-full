#ifndef __MILITIA_CONTROL_H
#define __MILITIA_CONTROL_H

#include "types.h"
#include "soldier_control.h"
#include "mouse_system.h"
#include "strategic_movement.h"

void PrepareMilitiaForTactical();
void RemoveMilitiaFromTactical();
void ResetMilitia();
void HandleMilitiaPromotions();

extern BOOLEAN gfStrategicMilitiaChangesMade;

extern INT32 ghMilitiaControlBox;
//extern INT32 ghTalkToAllBox;

extern BOOLEAN fFirstClickInMilitiaControlScreenMask;

// figure out the assignment menu pop up box positions
void DetermineMilitiaControlBoxPositions( void );

// set x,y position in tactical 
void SetTacticalPopUpMilitiaControlBoxXY( SOLDIERTYPE *pSoldier );

BOOLEAN CreateDestroyMilitiaControlPopUpBoxes( void );

BOOLEAN CheckIfRadioIsEquipped( void );

#endif
