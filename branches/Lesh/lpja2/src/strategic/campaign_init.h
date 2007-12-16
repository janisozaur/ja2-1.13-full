#ifndef __CAMPAIGN_INIT_H
#define __CAMPAIGN_INIT_H

#include "campaign_types.h"

extern BOOLEAN RandomSector[ TOTAL_SECTORS ];

extern void InitNewCampaign();

extern void BuildUndergroundSectorInfoList();
extern void TrashUndergroundSectorInfo();

#endif
