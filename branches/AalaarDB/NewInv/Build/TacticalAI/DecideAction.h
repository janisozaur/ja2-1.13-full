#ifndef DECIDEACTION_H
#define DECIDEACTION_H
#pragma once

#include "Soldier Control.h"

INT8	DecideAction(SOLDIERTYPE *pSoldier);
INT8 DecideActionBlack(SOLDIERTYPE *pSoldier);
INT8 DecideActionEscort(SOLDIERTYPE *pSoldier);
INT8 DecideActionGreen(SOLDIERTYPE *pSoldier);
INT8 DecideActionRed(SOLDIERTYPE *pSoldier, UINT8 ubUnconsciousOK);
INT8 DecideActionYellow(SOLDIERTYPE *pSoldier);
void DecideAlertStatus( SOLDIERTYPE *pSoldier );


#endif