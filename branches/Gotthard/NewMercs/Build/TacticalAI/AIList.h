#include "Types.h"

typedef struct AILIST
{
	INT16							ubID;
	INT8							bPriority;
	UINT8							ubUnused;
	struct AILIST *		pNext;
} AILIST;

#define MAX_AI_PRIORITY 100

extern void ClearAIList( void );
extern AILIST * CreateNewAIListEntry( INT16 ubNewEntry, INT16 ubID, INT8 bAlertStatus );
extern BOOLEAN InsertIntoAIList( INT16 ubID, INT8 bAlertStatus );
extern INT16 RemoveFirstAIListEntry( void );
extern BOOLEAN BuildAIListForTeam( UINT8 bTeam );
extern BOOLEAN MoveToFrontOfAIList( INT16 ubID );
