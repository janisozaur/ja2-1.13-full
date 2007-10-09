#ifndef _QUEST_DEBUG_SYSTEM_H_
#define _QUEST_DEBUG_SYSTEM_H_

#include "Soldier Control.h"
#include "NPC.h"


extern	BOOLEAN			gfNpcLogButton;
extern	INT32				gusQdsEnteringGridNo;


void NpcRecordLoggingInit( INT16 ubNpcID, INT16 ubMercID, UINT8 ubQuoteNum, UINT8 ubApproach );
void NpcRecordLogging( UINT8 ubApproach, STR pStringA, ...);


#endif