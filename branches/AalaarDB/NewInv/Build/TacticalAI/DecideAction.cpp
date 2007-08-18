#ifdef PRECOMPILEDHEADERS
	#include "AI All.h"
#else
	#include "ai.h"
	#include "AIInternals.h"
	#include "Isometric utils.h"
	#include "Points.h"
	#include "overhead.h"
	#include "opplist.h"
	#include "items.h"
	#include "Weapons.h"
	#include "NPC.h"
	#include "Soldier Functions.h"
	#include "worldman.h"
	#include "Scheduling.h"
	#include "Message.h"
	#include "Structure Wrap.h"
	#include "Keys.h"
	#include "pathai.h"
	#include "Render Fun.h"
	#include "Boxing.h"
	//	#include "Air Raid.h"
	#include "Soldier Profile.h"
	#include "soldier profile type.h"
	#include "Soldier macros.h"
	#include "los.h"
	#include "Buildings.h"
	#include "strategicmap.h"
	#include "Quests.h"
	#include "Map Screen Interface Map.h"
	#include "soldier ani.h"
	#include "rotting corpses.h"
	#include "GameSettings.h"
	#include "Dialogue Control.h"
#endif

extern BOOLEAN InternalIsValidStance( SOLDIERTYPE *pSoldier, INT8 bDirection, INT8 bNewStance );
extern BOOLEAN gfHiddenInterrupt;
extern BOOLEAN gfUseAlternateQueenPosition;

// global status time counters to determine what takes the most time

#ifdef AI_TIMING_TESTS
UINT32 guiGreenTimeTotal = 0, guiYellowTimeTotal = 0, guiRedTimeTotal = 0, guiBlackTimeTotal = 0;
UINT32 guiGreenCounter = 0, guiYellowCounter = 0, guiRedCounter = 0, guiBlackCounter = 0;
UINT32 guiRedSeekTimeTotal = 0, guiRedHelpTimeTotal = 0, guiRedHideTimeTotal = 0;
UINT32 guiRedSeekCounter = 0, guiRedHelpCounter = 0; guiRedHideCounter = 0;
#endif

#define CENTER_OF_RING 11237

#define MAX_FLANKS_RED 15
#define MAX_FLANKS_YELLOW 25

#define MIN_FLANK_DIST_YELLOW 10 * STRAIGHT_RATIO
#define MAX_FLANK_DIST_YELLOW 50 * STRAIGHT_RATIO

#define MIN_FLANK_DIST_RED 10 * STRAIGHT_RATIO
#define MAX_FLANK_DIST_RED 40 * STRAIGHT_RATIO


void DoneScheduleAction( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	pSoldier->aiData.fAIFlags &= (~AI_CHECK_SCHEDULE);
	pSoldier->bAIScheduleProgress = 0;
	PostNextSchedule( pSoldier );
}

INT8 DO_ACTION_LOCKDOOR ( SOLDIERTYPE * pSoldier, INT32 iScheduleIndex )
{
	PERFORMANCE_MARKER
	SCHEDULENODE * pSchedule = GetSchedule( pSoldier->ubScheduleID );
	UINT16						usGridNo1, usGridNo2;
	INT8							bDirection;
	STRUCTURE *				pStructure;
	BOOLEAN						fDoUseDoor;
	DOOR_STATUS	*			pDoorStatus;

	usGridNo1 = pSchedule->usData1[ iScheduleIndex ];
	usGridNo2 = pSchedule->usData2[ iScheduleIndex ];

	//Uses first gridno for locking door, then second to move to after door is locked.
	//It is possible that the second gridno will border the edge of the map, meaning that
	//the individual will walk off of the map.
	//If this is a "merchant", make sure that nobody occupies the building/room.

	switch( pSoldier->bAIScheduleProgress )
	{
	case 0: // move to gridno specified
		if (pSoldier->sGridNo == usGridNo1)
		{
			pSoldier->bAIScheduleProgress++;
			// fall through
		}
		else
		{
			pSoldier->aiData.usActionData = usGridNo1;
			pSoldier->sAbsoluteFinalDestination = pSoldier->aiData.usActionData;
			return( AI_ACTION_SCHEDULE_MOVE );
		}
		// fall through
	case 1:
		// start the door open: find the door...
		usGridNo1 = FindDoorAtGridNoOrAdjacent( usGridNo1 );

		if ( usGridNo1 == NOWHERE )
		{
			// do nothing right now!
			return( AI_ACTION_NONE );
		}

		pDoorStatus = GetDoorStatus( usGridNo1 );
		if (pDoorStatus && pDoorStatus->ubFlags & DOOR_BUSY)
		{
			// do nothing right now!
			return( AI_ACTION_NONE );
		}

		pStructure = FindStructure( usGridNo1, STRUCTURE_ANYDOOR );
		if (pStructure == NULL)
		{
			fDoUseDoor = FALSE;
		}
		else
		{
			// action-specific tests to not handle the door
			fDoUseDoor = TRUE;

			if (pStructure->fFlags & STRUCTURE_OPEN)
			{
				// not only do we have to lock the door but
				// close it too!
				pSoldier->aiData.fAIFlags |= AI_LOCK_DOOR_INCLUDES_CLOSE;
			}
			else
			{
				DOOR * pDoor;

				pDoor = FindDoorInfoAtGridNo( usGridNo1 );
				if (pDoor)
				{
					if (pDoor->fLocked)
					{
						// door already locked!
						fDoUseDoor = FALSE;
					}
					else
					{
						pDoor->fLocked = TRUE;
					}
				}
				else
				{
					ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Schedule involved locked door at %d but there's no lock there!", usGridNo1 );
					fDoUseDoor = FALSE;
				}
			}
		}

		if (fDoUseDoor)
		{
			pSoldier->aiData.usActionData = usGridNo1;
			return( AI_ACTION_LOCK_DOOR );
		}

		// the door is already in the desired state, or it doesn't exist!
		pSoldier->bAIScheduleProgress++;
		// fall through

	case 2:
		if (pSoldier->sGridNo == usGridNo2 || pSoldier->sGridNo == NOWHERE)
		{
			// NOWHERE indicates we were supposed to go off map and have done so
			DoneScheduleAction( pSoldier );

			if ( pSoldier->sGridNo != NOWHERE )
			{
				pSoldier->aiData.usPatrolGrid[0] = pSoldier->sGridNo;
			}
		}
		else
		{
			if ( GridNoOnEdgeOfMap( usGridNo2, &bDirection ) )
			{
				// told to go to edge of map, so go off at that point!
				pSoldier->ubQuoteActionID = GetTraversalQuoteActionID( bDirection );
			}
			pSoldier->aiData.usActionData = usGridNo2;
			pSoldier->sAbsoluteFinalDestination = pSoldier->aiData.usActionData;
			return( AI_ACTION_SCHEDULE_MOVE );
		}
	}
	return( AI_ACTION_NONE );
}

INT8 DO_ACTION_DOOR ( SOLDIERTYPE * pSoldier, INT32 iScheduleIndex, UINT8 ubScheduleAction)
{
	PERFORMANCE_MARKER
	SCHEDULENODE * pSchedule = GetSchedule( pSoldier->ubScheduleID );
	UINT16						usGridNo1, usGridNo2;
	INT8							bDirection;
	STRUCTURE *				pStructure;
	BOOLEAN						fDoUseDoor;
	DOOR_STATUS	*			pDoorStatus;

	usGridNo1 = pSchedule->usData1[ iScheduleIndex ];
	usGridNo2 = pSchedule->usData2[ iScheduleIndex ];

	//Uses first gridno for opening/closing/unlocking door, then second to move to after door is opened.
	//It is possible that the second gridno will border the edge of the map, meaning that
	//the individual will walk off of the map.
	switch( pSoldier->bAIScheduleProgress )
	{
	case 0: // move to gridno specified
		if (pSoldier->sGridNo == usGridNo1)
		{
			pSoldier->bAIScheduleProgress++;
			// fall through
		}
		else
		{
			pSoldier->aiData.usActionData = usGridNo1;
			pSoldier->sAbsoluteFinalDestination = pSoldier->aiData.usActionData;
			return( AI_ACTION_SCHEDULE_MOVE );
		}
		// fall through
	case 1:
		// start the door open: find the door...
		usGridNo1 = FindDoorAtGridNoOrAdjacent( usGridNo1 );

		if ( usGridNo1 == NOWHERE )
		{
			// do nothing right now!
			return( AI_ACTION_NONE );
		}

		pDoorStatus = GetDoorStatus( usGridNo1 );
		if (pDoorStatus && pDoorStatus->ubFlags & DOOR_BUSY)
		{
			// do nothing right now!
			return( AI_ACTION_NONE );
		}

		pStructure = FindStructure( usGridNo1, STRUCTURE_ANYDOOR );
		if (pStructure == NULL)
		{
			fDoUseDoor = FALSE;
		}
		else
		{
			fDoUseDoor = TRUE;

			// action-specific tests to not handle the door
			switch( ubScheduleAction )
			{
			case SCHEDULE_ACTION_UNLOCKDOOR:
				if (pStructure->fFlags & STRUCTURE_OPEN)
				{
					// door is already open!
					fDoUseDoor = FALSE;
				}
				else
				{
					// set the door to unlocked
					DOOR * pDoor;

					pDoor = FindDoorInfoAtGridNo( usGridNo1 );
					if (pDoor)
					{
						if (pDoor->fLocked)
						{
							pDoor->fLocked = FALSE;
						}
						else
						{
							// door already unlocked!
							fDoUseDoor = FALSE;
						}
					}
					else
					{
						// WTF?  Warning time!
						ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Schedule involved locked door at %d but there's no lock there!", usGridNo1 );
						fDoUseDoor = FALSE;
					}
				}
				break;
			case SCHEDULE_ACTION_OPENDOOR:
				if (pStructure->fFlags & STRUCTURE_OPEN)
				{
					// door is already open!
					fDoUseDoor = FALSE;
				}
				break;
			case SCHEDULE_ACTION_CLOSEDOOR:
				if ( !(pStructure->fFlags & STRUCTURE_OPEN) )
				{
					// door is already closed!
					fDoUseDoor = FALSE;
				}
				break;
			default:
				break;
			}
		}

		if (fDoUseDoor)
		{
			pSoldier->aiData.usActionData = usGridNo1;
			if (ubScheduleAction == SCHEDULE_ACTION_UNLOCKDOOR)
			{
				return( AI_ACTION_UNLOCK_DOOR );
			}
			else
			{
				return( AI_ACTION_OPEN_OR_CLOSE_DOOR );
			}
		}

		// the door is already in the desired state, or it doesn't exist!
		pSoldier->bAIScheduleProgress++;
		// fall through

	case 2:
		if (pSoldier->sGridNo == usGridNo2 || pSoldier->sGridNo == NOWHERE)
		{
			// NOWHERE indicates we were supposed to go off map and have done so
			DoneScheduleAction( pSoldier );
			if ( pSoldier->sGridNo != NOWHERE )
			{
				pSoldier->aiData.usPatrolGrid[0] = pSoldier->sGridNo;
			}
		}
		else
		{
			if ( GridNoOnEdgeOfMap( usGridNo2, &bDirection ) )
			{
				// told to go to edge of map, so go off at that point!
				pSoldier->ubQuoteActionID = GetTraversalQuoteActionID( bDirection );
			}
			pSoldier->aiData.usActionData = usGridNo2;
			pSoldier->sAbsoluteFinalDestination = pSoldier->aiData.usActionData;
			return( AI_ACTION_SCHEDULE_MOVE );
		}
	}
	return( AI_ACTION_NONE );
}

INT8 DO_ACTION_GRIDNO ( SOLDIERTYPE * pSoldier, INT32 iScheduleIndex )
{
	PERFORMANCE_MARKER
	SCHEDULENODE * pSchedule = GetSchedule( pSoldier->ubScheduleID );
	UINT16						usGridNo1;

	usGridNo1 = pSchedule->usData1[ iScheduleIndex ];

	// Only uses the first gridno
	if ( pSoldier->sGridNo == usGridNo1 )
	{
		// done!
		DoneScheduleAction( pSoldier );
		if ( pSoldier->sGridNo != NOWHERE )
		{
			pSoldier->aiData.usPatrolGrid[0] = pSoldier->sGridNo;
		}
	}
	else
	{
		// move!
		pSoldier->aiData.usActionData = usGridNo1;
		pSoldier->sAbsoluteFinalDestination = pSoldier->aiData.usActionData;
		return( AI_ACTION_SCHEDULE_MOVE );
	}
	return( AI_ACTION_NONE );
}

INT8 DO_ACTION_LEAVESECTOR ( SOLDIERTYPE * pSoldier, INT32 iScheduleIndex )
{
	PERFORMANCE_MARKER
	INT8							bDirection;
	//Doesn't use any gridno flags
	switch( pSoldier->bAIScheduleProgress )
	{
	case 0: // start the action

		pSoldier->aiData.usActionData = FindNearestEdgePoint( pSoldier->sGridNo );

		if (pSoldier->aiData.usActionData == NOWHERE)
		{
#ifdef JA2BETAVERSION
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_BETAVERSION, L"Civilian could not find path to map edge!" );
#endif
			DoneScheduleAction( pSoldier );
			return( AI_ACTION_NONE );
		}

		if ( pSoldier->sGridNo == pSoldier->aiData.usActionData )
		{
			// time to go off the map
			pSoldier->bAIScheduleProgress++;
		}
		else
		{
			// move!
			pSoldier->sAbsoluteFinalDestination = pSoldier->aiData.usActionData;
			return( AI_ACTION_SCHEDULE_MOVE );
		}

		// fall through

	case 1: // near edge

		pSoldier->aiData.usActionData = FindNearbyPointOnEdgeOfMap( pSoldier, &bDirection );
		if (pSoldier->aiData.usActionData == NOWHERE)
		{
			// what the heck??
			// ABORT!
			DoneScheduleAction( pSoldier );
		}
		else
		{
			pSoldier->ubQuoteActionID = GetTraversalQuoteActionID( bDirection );
			pSoldier->bAIScheduleProgress++;
			pSoldier->sAbsoluteFinalDestination = pSoldier->aiData.usActionData;
			return( AI_ACTION_SCHEDULE_MOVE );
		}
		break;

	case 2: // should now be done!
		DoneScheduleAction( pSoldier );
		break;

	default:
		break;
	}


	return( AI_ACTION_NONE );
}
INT8 DO_ACTION_ENTERSECTOR ( SOLDIERTYPE * pSoldier, INT32 iScheduleIndex )
{
	PERFORMANCE_MARKER
	SCHEDULENODE * pSchedule = GetSchedule( pSoldier->ubScheduleID );
	UINT16						usGridNo1;
	INT16							sX, sY;

	usGridNo1 = pSchedule->usData1[ iScheduleIndex ];

	if ( pSoldier->ubProfile != NO_PROFILE && gMercProfiles[ pSoldier->ubProfile ].ubMiscFlags & PROFILE_MISC_FLAG2_DONT_ADD_TO_SECTOR )
	{
		// ignore.
		DoneScheduleAction( pSoldier );
		return( AI_ACTION_NONE );
	}
	switch( pSoldier->bAIScheduleProgress )
	{
	case 0:
		sX = CenterX( pSoldier->sOffWorldGridNo );
		sY = CenterY( pSoldier->sOffWorldGridNo );
		pSoldier->EVENT_SetSoldierPosition( sX, sY );
		pSoldier->bInSector = TRUE;
		MoveSoldierFromAwayToMercSlot( pSoldier );
		pSoldier->aiData.usActionData = usGridNo1;
		pSoldier->bAIScheduleProgress++;
		pSoldier->sAbsoluteFinalDestination = pSoldier->aiData.usActionData;
		return( AI_ACTION_SCHEDULE_MOVE );
	case 1:
		if (pSoldier->sGridNo == usGridNo1)
		{
			DoneScheduleAction( pSoldier );
			if ( pSoldier->sGridNo != NOWHERE )
			{
				pSoldier->aiData.usPatrolGrid[0] = pSoldier->sGridNo;
			}
		}
		else
		{
			pSoldier->aiData.usActionData = usGridNo1;
			pSoldier->sAbsoluteFinalDestination = pSoldier->aiData.usActionData;
			return( AI_ACTION_SCHEDULE_MOVE );
		}
		break;
	}

	return( AI_ACTION_NONE );
}
INT8 DO_ACTION_WAKE ( SOLDIERTYPE * pSoldier, INT32 iScheduleIndex )
{
	PERFORMANCE_MARKER
	// Go to this position
	if (pSoldier->sGridNo == pSoldier->sInitialGridNo)
	{
		// th-th-th-that's it!
		DoneScheduleAction( pSoldier );
		pSoldier->aiData.usPatrolGrid[0] = pSoldier->sGridNo;
	}
	else
	{
		pSoldier->aiData.usActionData = pSoldier->sInitialGridNo;
		pSoldier->sAbsoluteFinalDestination = pSoldier->aiData.usActionData;
		return( AI_ACTION_SCHEDULE_MOVE );
	}


	return( AI_ACTION_NONE );
}
INT8 DO_ACTION_SLEEP ( SOLDIERTYPE * pSoldier, INT32 iScheduleIndex )
{
	PERFORMANCE_MARKER
	SCHEDULENODE * pSchedule = GetSchedule( pSoldier->ubScheduleID );
	UINT16						usGridNo1;
	usGridNo1 = pSchedule->usData1[ iScheduleIndex ];

	// Go to this position
	if (pSoldier->sGridNo == usGridNo1)
	{
		// Sleep
		pSoldier->aiData.fAIFlags |= AI_ASLEEP;
		DoneScheduleAction( pSoldier );
		if ( pSoldier->sGridNo != NOWHERE )
		{
			pSoldier->aiData.usPatrolGrid[0] = pSoldier->sGridNo;
		}
	}
	else
	{
		pSoldier->aiData.usActionData = usGridNo1;
		pSoldier->sAbsoluteFinalDestination = pSoldier->aiData.usActionData;
		return( AI_ACTION_SCHEDULE_MOVE );
	}


	return( AI_ACTION_NONE );
}

INT8 DecideActionSchedule( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	SCHEDULENODE * pSchedule = GetSchedule( pSoldier->ubScheduleID );
	if (!pSchedule)
	{
		return( AI_ACTION_NONE );
	}

	INT32							iScheduleIndex;
	if (pSchedule->usFlags & SCHEDULE_FLAGS_ACTIVE1)
	{
		iScheduleIndex = 0;
	}
	else if (pSchedule->usFlags & SCHEDULE_FLAGS_ACTIVE2)
	{
		iScheduleIndex = 1;
	}
	else if (pSchedule->usFlags & SCHEDULE_FLAGS_ACTIVE3)
	{
		iScheduleIndex = 2;
	}
	else if (pSchedule->usFlags & SCHEDULE_FLAGS_ACTIVE4)
	{
		iScheduleIndex = 3;
	}
	else
	{
		// error!
		return( AI_ACTION_NONE );
	}

	// assume soldier is awake unless the action is a sleep
	pSoldier->aiData.fAIFlags &= ~(AI_ASLEEP);

	switch( pSchedule->ubAction[ iScheduleIndex ] )
	{
	case SCHEDULE_ACTION_LOCKDOOR:
		return DO_ACTION_LOCKDOOR(pSoldier, iScheduleIndex);

	case SCHEDULE_ACTION_UNLOCKDOOR:
	case SCHEDULE_ACTION_OPENDOOR:
	case SCHEDULE_ACTION_CLOSEDOOR:
		return DO_ACTION_DOOR(pSoldier, iScheduleIndex, pSchedule->ubAction[ iScheduleIndex ]);

	case SCHEDULE_ACTION_GRIDNO:
		return DO_ACTION_GRIDNO(pSoldier, iScheduleIndex);

	case SCHEDULE_ACTION_LEAVESECTOR:
		return DO_ACTION_LEAVESECTOR(pSoldier, iScheduleIndex);

	case SCHEDULE_ACTION_ENTERSECTOR:
		return DO_ACTION_ENTERSECTOR(pSoldier, iScheduleIndex);

	case SCHEDULE_ACTION_WAKE:
		return DO_ACTION_WAKE(pSoldier, iScheduleIndex);

	case SCHEDULE_ACTION_SLEEP:
		return DO_ACTION_SLEEP(pSoldier, iScheduleIndex);
	}

	return( AI_ACTION_NONE );
}

INT8 DecideActionBoxerEnteringRing(SOLDIERTYPE *pSoldier)
{
	PERFORMANCE_MARKER
	UINT8 ubRoom;
	INT16	sDesiredMercLoc;
	UINT8 ubDesiredMercDir;
#ifdef DEBUGDECISIONS
	std::string tempstr;
#endif
	// boxer, should move into ring!
	if ( InARoom( pSoldier->sGridNo, &ubRoom ))
	{
		if (ubRoom == BOXING_RING)
		{
			// look towards nearest player
			sDesiredMercLoc = ClosestPC( pSoldier, NULL );
			if ( sDesiredMercLoc != NOWHERE )
			{
				// see if we are facing this person
				ubDesiredMercDir = atan8(CenterX(pSoldier->sGridNo),CenterY(pSoldier->sGridNo),CenterX(sDesiredMercLoc),CenterY(sDesiredMercLoc));

				// if not already facing in that direction,
				if ( pSoldier->bDirection != ubDesiredMercDir && pSoldier->InternalSoldierReadyWeapon( ubDesiredMercDir, gAnimControl[ pSoldier->usAnimState ].ubEndHeight ) )
				{

					pSoldier->aiData.usActionData = ubDesiredMercDir;

#ifdef DEBUGDECISIONS
					tempstr = String("%s - TURNS TOWARDS CLOSEST PC to face direction %d",pSoldier->name,pSoldier->aiData.usActionData);
					DebugAI(tempstr);
#endif

					return( AI_ACTION_CHANGE_FACING );
				}
			}
			return( AI_ACTION_ABSOLUTELY_NONE );
		}
		else
		{
			// move to starting spot
			pSoldier->aiData.usActionData = FindClosestBoxingRingSpot( pSoldier, TRUE );
			return( AI_ACTION_GET_CLOSER );
		}
	}

	return( AI_ACTION_ABSOLUTELY_NONE );
}

INT8 DecideActionNamedNPC( SOLDIERTYPE * pSoldier )
{
	PERFORMANCE_MARKER
	INT16 sDesiredMercLoc;
	UINT8	ubDesiredMercDir;
	UINT8	ubDesiredMerc;
	INT16	sDesiredMercDist;
#ifdef DEBUGDECISIONS
	std::string tempstr;
#endif

	// if a quote record has been set and we're not doing movement, then
	// it means we have to wait until someone is nearby and then see
	// to do...

	// is this person close enough to trigger event?
	if (pSoldier->ubQuoteRecord && pSoldier->ubQuoteActionID == QUOTE_ACTION_ID_TURNTOWARDSPLAYER )
	{
		sDesiredMercLoc = ClosestPC( pSoldier, &sDesiredMercDist );
		if (sDesiredMercLoc != NOWHERE )
		{
			if ( sDesiredMercDist <= NPC_TALK_RADIUS * 2)
			{
				pSoldier->ubQuoteRecord = 0;
				// see if this triggers a conversation/NPC record
				PCsNearNPC( pSoldier->ubProfile );
				// clear "handle every frame" flag
				pSoldier->aiData.fAIFlags &= (~AI_HANDLE_EVERY_FRAME);
				return( AI_ACTION_ABSOLUTELY_NONE );
			}

			// see if we are facing this person
			ubDesiredMercDir = atan8(CenterX(pSoldier->sGridNo),CenterY(pSoldier->sGridNo),CenterX(sDesiredMercLoc),CenterY(sDesiredMercLoc));

			// if not already facing in that direction,
			if (pSoldier->bDirection != ubDesiredMercDir && pSoldier->InternalSoldierReadyWeapon( ubDesiredMercDir, gAnimControl[ pSoldier->usAnimState ].ubEndHeight ) )
			{

				pSoldier->aiData.usActionData = ubDesiredMercDir;

#ifdef DEBUGDECISIONS
				tempstr = String("%s - TURNS TOWARDS CLOSEST PC to face direction %d",pSoldier->name,pSoldier->aiData.usActionData);
				DebugAI(tempstr);
#endif

				return( AI_ACTION_CHANGE_FACING );
			}
		}

		// do nothing; we're looking at the PC or the NPC is far away
		return( AI_ACTION_ABSOLUTELY_NONE );

	}
	else
	{
		///////////////
		// CHECK TO SEE IF WE WANT TO GO UP TO PERSON AND SAY SOMETHING
		///////////////
		pSoldier->aiData.usActionData = NPCConsiderInitiatingConv( pSoldier, &ubDesiredMerc );
		if (pSoldier->aiData.usActionData != NOWHERE)
		{
			return( AI_ACTION_APPROACH_MERC );
		}
	}

	switch( pSoldier->ubProfile )
	{
	case JIM:
	case JACK:
	case OLAF:
	case RAY:
	case OLGA:
	case TYRONE:
		sDesiredMercLoc = ClosestPC( pSoldier, &sDesiredMercDist );
		if (sDesiredMercLoc != NOWHERE )
		{
			if ( sDesiredMercDist <= NPC_TALK_RADIUS * 2 )
			{
				AddToShouldBecomeHostileOrSayQuoteList( pSoldier->ubID );
				// now wait a bit!
				pSoldier->aiData.usActionData = 5000;
				return( AI_ACTION_WAIT );
			}
			else
			{
				pSoldier->aiData.usActionData = GoAsFarAsPossibleTowards( pSoldier, sDesiredMercLoc, AI_ACTION_APPROACH_MERC );
				if ( pSoldier->aiData.usActionData != NOWHERE )
				{
					return( AI_ACTION_APPROACH_MERC );
				}
			}
		}
		break;
	default:
		break;
	}

	return( AI_ACTION_NONE );
}

struct GreenAlertFlags
{
	INT8  bInWater,bInGas;
	BOOLEAN fCivilian;
	BOOLEAN fCivilianOrMilitia;
};

INT8 GreenAlert_TryToDoBoxing(SOLDIERTYPE* pSoldier, GreenAlertFlags& flags)
{
	PERFORMANCE_MARKER
	if (pSoldier->flags.uiStatusFlags & SOLDIER_BOXER)
	{
		if ( gTacticalStatus.bBoxingState == PRE_BOXING )
		{
			return( DecideActionBoxerEnteringRing( pSoldier ) );
		}
		else
		{
			UINT8	ubRoom;
			UINT8 ubLoop;

			// boxer... but since in status green, it's time to leave the ring!
			if ( InARoom( pSoldier->sGridNo, &ubRoom ))
			{
				if (ubRoom == BOXING_RING)
				{
					for ( ubLoop = 0; ubLoop < NUM_BOXERS; ubLoop++ )
					{
						if (pSoldier->ubID == gubBoxerID[ ubLoop ])
						{
							// we should go back where we started
							pSoldier->aiData.usActionData = gsBoxerGridNo[ ubLoop ];
							return( AI_ACTION_GET_CLOSER );
						}
					}
					pSoldier->aiData.usActionData = FindClosestBoxingRingSpot( pSoldier, FALSE );
					return( AI_ACTION_GET_CLOSER );
				}
				else
				{
					// done!
					pSoldier->flags.uiStatusFlags &= ~(SOLDIER_BOXER);
					if (pSoldier->bTeam == gbPlayerNum)
					{
						pSoldier->flags.uiStatusFlags &= (~SOLDIER_PCUNDERAICONTROL);
						TriggerEndOfBoxingRecord( pSoldier );
					}
					else if ( CountPeopleInBoxingRing() == 0 )
					{
						// Probably disqualified by jumping out of ring; the player
						// character then didn't trigger the end of boxing record
						// (and we know from the if statement above that we're
						// still in a boxing state of some sort...)
						TriggerEndOfBoxingRecord( NULL );

					}
				}
			}

			return( AI_ACTION_ABSOLUTELY_NONE );
		}
	}
	//else if ( (gTacticalStatus.bBoxingState == PRE_BOXING || gTacticalStatus.bBoxingState == BOXING) && ( PythSpacesAway( pSoldier->sGridNo, CENTER_OF_RING ) <= MaxDistanceVisible() ) )
	else if ( PythSpacesAway( pSoldier->sGridNo, CENTER_OF_RING ) <= MaxDistanceVisible() )
	{
		UINT8 ubRingDir;
		// face ring!

		ubRingDir = atan8(CenterX(pSoldier->sGridNo),CenterY(pSoldier->sGridNo),CenterX(CENTER_OF_RING),CenterY(CENTER_OF_RING));
		if ( gfTurnBasedAI || GetAPsToLook( pSoldier ) <= pSoldier->bActionPoints )
		{
			if ( pSoldier->bDirection != ubRingDir )
			{
				pSoldier->aiData.usActionData = ubRingDir;
				return( AI_ACTION_CHANGE_FACING );
			}
		}
		return( AI_ACTION_NONE );
	}
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 GreenAlert_TryToDoCivStuff(SOLDIERTYPE* pSoldier, GreenAlertFlags& flags)
{
	PERFORMANCE_MARKER
	// special stuff for civs

	if (pSoldier->flags.uiStatusFlags & SOLDIER_COWERING)
	{
		// everything's peaceful again, stop cowering!!
		pSoldier->aiData.usActionData = ANIM_STAND;
		return( AI_ACTION_STOP_COWERING );
	}

	if (!gfTurnBasedAI)
	{
		// ******************
		// REAL TIME NPC CODE
		// ******************
		if (pSoldier->aiData.fAIFlags & AI_CHECK_SCHEDULE)
		{
			pSoldier->aiData.bAction = DecideActionSchedule( pSoldier );
			if (pSoldier->aiData.bAction != AI_ACTION_NONE)
			{
				return( pSoldier->aiData.bAction );
			}
		}

		if (pSoldier->ubProfile != NO_PROFILE)
		{
			pSoldier->aiData.bAction = DecideActionNamedNPC( pSoldier );
			if ( pSoldier->aiData.bAction != AI_ACTION_NONE )
			{
				return( pSoldier->aiData.bAction );
			}
			// can we act again? not for a minute since we were last spoken to/triggered a record
			if ( pSoldier->uiTimeSinceLastSpoke && (GetJA2Clock() < pSoldier->uiTimeSinceLastSpoke + 60000) )
			{
				return( AI_ACTION_NONE );
			}
			// turn off counter so we don't check it again
			pSoldier->uiTimeSinceLastSpoke = 0;

		}

	}

	// if not in the way, do nothing most of the time
	// unless in water (could've started there), then we better swim to shore!

	if (!(flags.bInWater) && PreRandom( 5 ) )
	{
		// don't do nuttin!
		return( AI_ACTION_NONE );
	}
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 GreenAlert_TryToClimbABuilding(SOLDIERTYPE* pSoldier, GreenAlertFlags& flags)
{
	PERFORMANCE_MARKER
	INT32 iChance = 10 + pSoldier->aiData.bBypassToGreen;

	// set base chance and maximum seeking distance according to orders
	switch (pSoldier->aiData.bOrders)
	{
	case STATIONARY:     iChance = 0; break;
	case ONGUARD:        iChance += 10; break;
	case ONCALL:                         break;
	case CLOSEPATROL:    iChance += -20; break;
	case RNDPTPATROL:
	case POINTPATROL:    iChance  = -30; break;
	case FARPATROL:      iChance += -40; break;
	case SEEKENEMY:      iChance += -30; break;
	case SNIPER:		 iChance += 70; break;
	}

	// modify for attitude
	switch (pSoldier->aiData.bAttitude)
	{
	case DEFENSIVE:      iChance = (INT32) (iChance * 1.5);  break;
	case BRAVESOLO:      iChance /= 2;    break;
	case BRAVEAID:       iChance /= 2;   break;
	case CUNNINGSOLO:					break;
	case CUNNINGAID:					break;
	case AGGRESSIVE:     iChance /= 3;    break;
	case ATTACKSLAYONLY:									 break;
	}


	//hide those suicidal militia on the roofs for better defensive positions
	if ( pSoldier->bTeam == MILITIA_TEAM )
		iChance += 20;

	// reduce chance for any injury, less likely to hop up if hurt
	iChance -= (pSoldier->stats.bLifeMax - pSoldier->stats.bLife);

	// reduce chance if breath is down
	//iChance -= (100 - pSoldier->bBreath);         // don't care

	if ((INT16) PreRandom(100) < iChance)
	{
		if ( pSoldier->pathing.bLevel == 0 )
		{
			BOOLEAN fUp = TRUE;
			if (pSoldier->pathing.bLevel > 0 )
				fUp = FALSE;

			if ( CanClimbFromHere ( pSoldier, fUp ) )
			{
				DebugMsg ( TOPIC_JA2AI , DBG_LEVEL_3 , String("Soldier %d is climbing roof",pSoldier->ubID) );
				return( AI_ACTION_CLIMB_ROOF );
			}
			else
			{
				pSoldier->aiData.usActionData = FindClosestClimbPoint(pSoldier, fUp );
				// Added the check here because sniper militia who are locked inside of a building without keys
				// will still have a >100% chance to want to climb, which means an infinite loop.  In fact, any
				// time a move is desired, there is also probably be a need to check for a path.
				if ( pSoldier->aiData.usActionData != NOWHERE &&
					LegalNPCDestination(pSoldier,pSoldier->aiData.usActionData,ENSURE_PATH,WATEROK, 0 ))
				{
					return( AI_ACTION_MOVE_TO_CLIMB  );
				}
			}
		}
	}
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 GreenAlert_TryToDoRandomPatrol(SOLDIERTYPE* pSoldier, GreenAlertFlags& flags)
{
	PERFORMANCE_MARKER
	INT32 iChance = 25 + pSoldier->aiData.bBypassToGreen;
	//ADB iSneaky isn't used apparently
	//INT32 iSneaky = 10;

	// set base chance according to orders
	switch (pSoldier->aiData.bOrders)
	{
	case STATIONARY:     iChance += -20;  break;
	case ONGUARD:        iChance += -15;  break;
	case ONCALL:                          break;
	case CLOSEPATROL:    iChance += +15;  break;
	case RNDPTPATROL:
	case POINTPATROL:		iChance = 0; break;
		/*
		if ( !gfTurnBasedAI )
		{
		// realtime deadlock... increase chance!
		iChance = 110;// more than 100 in case person is defensive
		}
		else if ( pSoldier->bInitialActionPoints < pSoldier->bActionPoints ) // could be less because of carried-over points
		{
		// CJC: allow pt patrol guys to do a random move in case
		// of a deadlock provided they haven't done anything yet this turn
		iChance=   0;
		}
		break;
		*/
	case FARPATROL:      iChance += +25;  break;
	case SEEKENEMY:      iChance += -10;  break;
	case SNIPER:		iChance += -10;  break;
	}

	// modify chance of patrol (and whether it's a sneaky one) by attitude
	switch (pSoldier->aiData.bAttitude)
	{
	case DEFENSIVE:      iChance += -10;                 break;
	case BRAVESOLO:      iChance +=   5;                 break;
	case BRAVEAID:                                       break;
	case CUNNINGSOLO:    iChance +=   5;  /*iSneaky += 10;*/ break;
	case CUNNINGAID:                      /*iSneaky +=  5;*/ break;
	case AGGRESSIVE:     iChance +=  10;  /*iSneaky += -5;*/ break;
	case ATTACKSLAYONLY: iChance +=  10;  /*iSneaky += -5;*/ break;
	}

	// reduce chance for any injury, less likely to wander around when hurt
	iChance -= (pSoldier->stats.bLifeMax - pSoldier->stats.bLife);

	// reduce chance if breath is down, less likely to wander around when tired
	iChance -= (100 - pSoldier->bBreath);


	// if we're in water with land miles (> 25 tiles) away,
	// OR if we roll under the chance calculated
	if (flags.bInWater || ((INT16) PreRandom(100) < iChance))
	{
		pSoldier->aiData.usActionData = RandDestWithinRange(pSoldier);

		if (pSoldier->aiData.usActionData != NOWHERE)
		{
			pSoldier->aiData.usActionData = GoAsFarAsPossibleTowards( pSoldier, pSoldier->aiData.usActionData, AI_ACTION_RANDOM_PATROL );
		}

		if (pSoldier->aiData.usActionData != NOWHERE)
		{
#ifdef DEBUGDECISIONS
			std::string tempstr = String("%s - RANDOM PATROL to grid %d",pSoldier->name,pSoldier->aiData.usActionData);
			DebugAI(tempstr);
#endif

			if (!gfTurnBasedAI)
			{
				// wait after this...
				pSoldier->aiData.bNextAction = AI_ACTION_WAIT;
				pSoldier->aiData.usNextActionData = RealtimeDelay( pSoldier );
			}
			return(AI_ACTION_RANDOM_PATROL);
		}
	}
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 GreenAlert_TryToSeekFriend(SOLDIERTYPE* pSoldier, GreenAlertFlags& flags)
{
	PERFORMANCE_MARKER
	INT32 iChance = 25 + pSoldier->aiData.bBypassToGreen;

	// set base chance and maximum seeking distance according to orders
	switch (pSoldier->aiData.bOrders)
	{
	case STATIONARY:     iChance += -20; break;
	case ONGUARD:        iChance += -15; break;
	case ONCALL:                         break;
	case CLOSEPATROL:    iChance += +10; break;
	case RNDPTPATROL:
	case POINTPATROL:    iChance  = -10; break;
	case FARPATROL:      iChance += +20; break;
	case SEEKENEMY:      iChance += -10; break;
	case SNIPER:		  iChance += -10; break;
	}

	// modify for attitude
	switch (pSoldier->aiData.bAttitude)
	{
	case DEFENSIVE:                       break;
	case BRAVESOLO:      iChance /= 2;    break;  // loners
	case BRAVEAID:       iChance += 10;   break;  // friendly
	case CUNNINGSOLO:    iChance /= 2;    break;  // loners
	case CUNNINGAID:     iChance += 10;   break;  // friendly
	case AGGRESSIVE:                      break;
	case ATTACKSLAYONLY:									 break;
	}

	// reduce chance for any injury, less likely to wander around when hurt
	iChance -= (pSoldier->stats.bLifeMax - pSoldier->stats.bLife);

	// reduce chance if breath is down
	iChance -= (100 - pSoldier->bBreath);         // very likely to wait when exhausted


	if ((INT16) PreRandom(100) < iChance)
	{
		if (RandomFriendWithin(pSoldier))
		{
			if ( pSoldier->aiData.usActionData == GoAsFarAsPossibleTowards( pSoldier, pSoldier->aiData.usActionData, AI_ACTION_SEEK_FRIEND ) )
			{

#ifdef DEBUGDECISIONS
				std::string tempstr = String("%s - SEEK FRIEND at grid %d",pSoldier->name,pSoldier->aiData.usActionData);
				DebugAI(tempstr);
#endif

				if (flags.fCivilianOrMilitia && !gfTurnBasedAI)
				{
					// pause at the end of the walk!
					pSoldier->aiData.bNextAction = AI_ACTION_WAIT;
					pSoldier->aiData.usNextActionData = (UINT16) REALTIME_CIV_AI_DELAY;
				}

				return(AI_ACTION_SEEK_FRIEND);
			}
		}
	}
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 GreenAlert_TryToDoSniperStuff(SOLDIERTYPE* pSoldier, GreenAlertFlags& flags)
{
	PERFORMANCE_MARKER
	////////////////////////////////////////////////////////////////////////////
	// SNIPERS LIKE TO CROUCH (on roofs)
	////////////////////////////////////////////////////////////////////////////

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DecideActionGreen: Snipers like to crouch, sniper = %d",pSoldier->sniper));
	// if not in water and not already crouched, try to crouch down first
	if ( !PTR_CROUCHED && IsValidStance( pSoldier, ANIM_CROUCH ) && pSoldier->pathing.bLevel == 1 )
	{
		if (!gfTurnBasedAI || (GetAPsToChangeStance( pSoldier, ANIM_CROUCH ) <= pSoldier->bActionPoints))
		{
			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DecideActionGreen: Sniper is crouching"));
			pSoldier->aiData.usActionData = ANIM_CROUCH;
			pSoldier->sniper = 0;
			return(AI_ACTION_CHANGE_STANCE);
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// SNIPER - RAISE WEAPON TO SCAN AREA
	////////////////////////////////////////////////////////////////////////////

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DecideActionGreen: Snipers like to raise weapons, sniper = %d",pSoldier->sniper));
	if ( pSoldier->sniper == 0 && ( pSoldier->pathing.bLevel == 1 || Random(100) < 40 ) )
	{
		if (!gfTurnBasedAI || GetAPsToReadyWeapon( pSoldier, READY_RIFLE_CROUCH ) <= pSoldier->bActionPoints)
		{
			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DecideActionGreen: Sniper is raising weapon, soldier = %d, sniper = %d",pSoldier->ubID,pSoldier->sniper));
			pSoldier->sniper = 1;
			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DecideActionGreen: Sniper = %d",pSoldier->sniper));
			return(AI_ACTION_RAISE_GUN);
		}
	}
	//else if ( pSoldier->sniper == 1 )
	//{
	//	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DecideActionGreen: Sniper is lowering weapon, sniper = %d",pSoldier->sniper));
	//	pSoldier->sniper = 0;
	//	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DecideActionGreen: Sniper = %d",pSoldier->sniper));
	//	return(AI_ACTION_LOWER_GUN);
	//}
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 GreenAlert_TryToLookAround(SOLDIERTYPE* pSoldier, GreenAlertFlags& flags)
{
	PERFORMANCE_MARKER
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DecideActionGreen: Soldier deciding to turn"));
	// avoid 2 consecutive random turns in a row
	if (pSoldier->aiData.bLastAction != AI_ACTION_CHANGE_FACING)
	{
		INT32 iChance = 25 + pSoldier->aiData.bBypassToGreen;

		// set base chance according to orders
		if (pSoldier->aiData.bOrders == STATIONARY || pSoldier->aiData.bOrders == SNIPER)
			iChance += 25;

		if (pSoldier->aiData.bOrders == ONGUARD)
			iChance += 20;

		if (pSoldier->aiData.bAttitude == DEFENSIVE)
			iChance += 25;

		if ( pSoldier->aiData.bOrders == SNIPER && pSoldier->pathing.bLevel == 1)
			iChance += 35;

		if ((INT16)PreRandom(100) < iChance)
		{
			// roll random directions (stored in actionData) until different from current
			do
			{
				// if man has a LEGAL dominant facing, and isn't facing it, he will turn
				// back towards that facing 50% of the time here (normally just enemies)
				if ((pSoldier->aiData.bDominantDir >= 0) && (pSoldier->aiData.bDominantDir <= 8) &&
					(pSoldier->bDirection != pSoldier->aiData.bDominantDir) && PreRandom(2) && pSoldier->aiData.bOrders != SNIPER )
				{
					pSoldier->aiData.usActionData = pSoldier->aiData.bDominantDir;
				}
				else
				{
					INT32 iNoiseValue;
					BOOLEAN fClimb;
					BOOLEAN fReachable;
					INT16 sNoiseGridNo = MostImportantNoiseHeard(pSoldier,&iNoiseValue, &fClimb, &fReachable);
					UINT8 ubNoiseDir;

					if (sNoiseGridNo != NOWHERE)
						ubNoiseDir = atan8(CenterX(pSoldier->sGridNo),CenterY(pSoldier->sGridNo),CenterX(sNoiseGridNo),CenterY(sNoiseGridNo));

					if ( sNoiseGridNo != NOWHERE && pSoldier->bDirection != ubNoiseDir )
						pSoldier->aiData.usActionData = ubNoiseDir;
					else
						pSoldier->aiData.usActionData = (UINT16)PreRandom(8);
				}
			} while (pSoldier->aiData.usActionData == pSoldier->bDirection);


#ifdef DEBUGDECISIONS
			std::string tempstr = String("%s - TURNS to face direction %d",pSoldier->name,pSoldier->aiData.usActionData);
			DebugAI(tempstr);
#endif

			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DecideActionGreen: Trying to turn - checking stance validity, sniper = %d",pSoldier->sniper));
			if ( pSoldier->InternalSoldierReadyWeapon( (INT8) pSoldier->aiData.usActionData, gAnimControl[ pSoldier->usAnimState ].ubEndHeight ) )
			{

				if ( !gfTurnBasedAI )
				{
					// wait after this...
					pSoldier->aiData.bNextAction = AI_ACTION_WAIT;
					pSoldier->aiData.usNextActionData = RealtimeDelay( pSoldier );
				}

				DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DecideActionGreen: Soldier is turning"));
				return(AI_ACTION_CHANGE_FACING);
			}
		}
	}
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 DecideActionGreen(SOLDIERTYPE *pSoldier)
{
	PERFORMANCE_MARKER
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DecideActionGreen, orders = %d",pSoldier->aiData.bOrders));
	GreenAlertFlags flags;
#ifdef DEBUGDECISIONS
	std::string tempstr;
#endif


	INT8	bActionReturned;
	flags.fCivilian = (PTR_CIVILIAN && (pSoldier->ubCivilianGroup == NON_CIV_GROUP || pSoldier->aiData.bNeutral || (pSoldier->ubBodyType >= FATCIV && pSoldier->ubBodyType <= CRIPPLECIV) ) );
	flags.fCivilianOrMilitia = PTR_CIV_OR_MILITIA;

	gubNPCPathCount = 0;

	if ( gTacticalStatus.bBoxingState != NOT_BOXING )
	{
		bActionReturned = GreenAlert_TryToDoBoxing(pSoldier, flags);
		if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
			return(bActionReturned);
	}

	if ( TANK( pSoldier ) )
	{
		return( AI_ACTION_NONE );
	}


	flags.bInWater = Water( pSoldier->sGridNo );

	// check if standing in tear gas without a gas mask on, or in smoke
	flags.bInGas = InGasOrSmoke( pSoldier, pSoldier->sGridNo );

	// if real-time, and not in the way, do nothing 90% of the time (for GUARDS!)
	// unless in water (could've started there), then we better swim to shore!

	if (flags.fCivilian)
	{
		bActionReturned = GreenAlert_TryToDoCivStuff(pSoldier, flags);
		if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
			return(bActionReturned);
	}





	////////////////////////////////////////////////////////////////////////////
	// POINT PATROL: move towards next point unless getting a bit winded
	////////////////////////////////////////////////////////////////////////////

	// this takes priority over water/gas checks, so that point patrol WILL work
	// from island to island, and through gas covered areas, too
	if ((pSoldier->aiData.bOrders == POINTPATROL) && (pSoldier->bBreath >= 75))
	{
		if (PointPatrolAI(pSoldier))
		{
			if (!gfTurnBasedAI)
			{
				// wait after this...
				pSoldier->aiData.bNextAction = AI_ACTION_WAIT;
				pSoldier->aiData.usNextActionData = RealtimeDelay( pSoldier );
			}
			return(AI_ACTION_POINT_PATROL);
		}
		else
		{
			// Reset path count to avoid dedlok
			gubNPCPathCount = 0;
		}
	}

	if ((pSoldier->aiData.bOrders == RNDPTPATROL) && (pSoldier->bBreath >=75))
	{
		if (RandomPointPatrolAI(pSoldier))
		{
			if (!gfTurnBasedAI)
			{
				// wait after this...
				pSoldier->aiData.bNextAction = AI_ACTION_WAIT;
				pSoldier->aiData.usNextActionData = RealtimeDelay( pSoldier );
			}
			return(AI_ACTION_POINT_PATROL);
		}
		else
		{
			// Reset path count to avoid dedlok
			gubNPCPathCount = 0;
		}

	}

	////////////////////////////////////////////////////////////////////////////
	// WHEN LEFT IN WATER OR GAS, GO TO NEAREST REACHABLE SPOT OF UNGASSED LAND
	////////////////////////////////////////////////////////////////////////////

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DecideActionGreen: get out of water and gas"));

	if (flags.bInWater || flags.bInGas)
	{
		pSoldier->aiData.usActionData = FindNearestUngassedLand(pSoldier);

		if (pSoldier->aiData.usActionData != NOWHERE)
		{
#ifdef DEBUGDECISIONS
			tempstr = String("%s - SEEKING NEAREST UNGASSED LAND at grid %d",pSoldier->name,pSoldier->aiData.usActionData);
			DebugAI(tempstr);
#endif

			return(AI_ACTION_LEAVE_WATER_GAS);
		}
	}



	////////////////////////////////////////////////////////////////////////
	// REST IF RUNNING OUT OF BREATH
	////////////////////////////////////////////////////////////////////////

	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DecideActionGreen: rest if running out of breath"));
	// if our breath is running a bit low, and we're not in the way or in water
	if ((pSoldier->bBreath < 75) && !flags.bInWater)
	{
		// take a breather for gods sake!
		// for realtime, AI will use a standard wait set outside of here
		pSoldier->aiData.usActionData = NOWHERE;
		return(AI_ACTION_NONE);
	}


	////////////////////////////////////////////////////////////////////////////
	// CLIMB A BUILDING
	////////////////////////////////////////////////////////////////////////////
	if (!flags.fCivilian)
	{
		bActionReturned = GreenAlert_TryToClimbABuilding(pSoldier, flags);
		if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
			return(bActionReturned);
	}


	////////////////////////////////////////////////////////////////////////////
	// RANDOM PATROL:  determine % chance to start a new patrol route
	////////////////////////////////////////////////////////////////////////////
	if (!gubNPCPathCount) // try to limit pathing in Green AI
	{
		bActionReturned = GreenAlert_TryToDoRandomPatrol(pSoldier, flags);
		if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
			return(bActionReturned);
	}


	////////////////////////////////////////////////////////////////////////////
	// SEEK FRIEND: determine %chance for man to pay a friendly visit
	////////////////////////////////////////////////////////////////////////////
	if (!gubNPCPathCount) // try to limit pathing in Green AI
	{
		bActionReturned = GreenAlert_TryToSeekFriend(pSoldier, flags);
		if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
			return(bActionReturned);
	}


	////////////////////////////////////////////////////////////////////////////
	// DO SNIPER STUFF like crouch or raise weapon
	////////////////////////////////////////////////////////////////////////////
	if ( pSoldier->aiData.bOrders == SNIPER )
	{
		bActionReturned = GreenAlert_TryToDoSniperStuff(pSoldier, flags);
		if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
			return(bActionReturned);
	}


	////////////////////////////////////////////////////////////////////////////
	// LOOK AROUND: determine %chance for man to turn in place
	////////////////////////////////////////////////////////////////////////////
	if (!gfTurnBasedAI || GetAPsToLook( pSoldier ) <= pSoldier->bActionPoints)
	{
		bActionReturned = GreenAlert_TryToLookAround(pSoldier, flags);
		if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
			return(bActionReturned);
	}


	////////////////////////////////////////////////////////////////////////////
	// NONE:
	////////////////////////////////////////////////////////////////////////////

	// by default, if everything else fails, just stands in place without turning
	// for realtime, regular AI guys will use a standard wait set outside of here
	pSoldier->aiData.usActionData = NOWHERE;
	return(AI_ACTION_NONE);
}

struct YellowAlertFlags
{
	INT16 sNoiseGridNo;
	INT32 iNoiseValue;
	BOOLEAN fCivilian;
	BOOLEAN fClimb;
	BOOLEAN fReachable;
};

INT8 YellowAlert_TryToLookAround(SOLDIERTYPE* pSoldier, YellowAlertFlags& flags)
{
	PERFORMANCE_MARKER
	// determine direction from this soldier in which the noise lies
	UINT8 ubNoiseDir = atan8(CenterX(pSoldier->sGridNo),CenterY(pSoldier->sGridNo),CenterX(flags.sNoiseGridNo),CenterY(flags.sNoiseGridNo));

	// if soldier is not already facing in that direction,
	// and the noise source is close enough that it could possibly be seen
	if ( !gfTurnBasedAI || GetAPsToLook( pSoldier ) <= pSoldier->bActionPoints )
	{
		if ((pSoldier->bDirection != ubNoiseDir) && PythSpacesAway(pSoldier->sGridNo,flags.sNoiseGridNo) <= MaxDistanceVisible() )
		{
			INT32 iChance;
			// set base chance according to orders
			if ((pSoldier->aiData.bOrders == STATIONARY) || (pSoldier->aiData.bOrders == ONGUARD) )
				iChance = 50;
			else           // all other orders
				iChance = 25;

			if (pSoldier->aiData.bAttitude == DEFENSIVE)
				iChance += 15;


			if ((INT16)PreRandom(100) < iChance && pSoldier->InternalSoldierReadyWeapon( ubNoiseDir, gAnimControl[ pSoldier->usAnimState ].ubEndHeight ) )
			{
				pSoldier->aiData.usActionData = ubNoiseDir;
#ifdef DEBUGDECISIONS
				std::string tempstr = String("%s - TURNS TOWARDS NOISE to face direction %d",pSoldier->name,pSoldier->aiData.usActionData);
				DebugAI(tempstr);
#endif
				if ( pSoldier->aiData.bOrders == SNIPER )
				{
					if (!gfTurnBasedAI || GetAPsToReadyWeapon( pSoldier, READY_RIFLE_CROUCH ) <= pSoldier->bActionPoints)
					{
						pSoldier->aiData.bNextAction = AI_ACTION_RAISE_GUN;
					}
				}
				return(AI_ACTION_CHANGE_FACING);
			}
		}
	}
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 YellowAlert_TryToFlank(SOLDIERTYPE* pSoldier, YellowAlertFlags& flags)
{
	PERFORMANCE_MARKER
	//continue flanking
	INT16 tempGridNo;
	if ( flags.sNoiseGridNo == NOWHERE )
		tempGridNo = pSoldier->lastFlankSpot;
	else
		tempGridNo = flags.sNoiseGridNo;

	if ( pSoldier->numFlanks > 0 && pSoldier->numFlanks < MAX_FLANKS_YELLOW )
	{
		INT16 currDir = GetDirectionFromGridNo ( tempGridNo, pSoldier );
		INT16 origDir = pSoldier->origDir;
		pSoldier->numFlanks += 1;
		if ( pSoldier->flags.lastFlankLeft )
		{
			if ( origDir > currDir )
				origDir -= 8;

			if ( (currDir - origDir) >= 4 )
			{
				pSoldier->numFlanks = MAX_FLANKS_YELLOW;
			}
			else
			{
				pSoldier->aiData.usActionData = FindFlankingSpot (pSoldier, tempGridNo , AI_ACTION_FLANK_LEFT);
				if ( pSoldier->aiData.usActionData != NOWHERE ) //&& (currDir - origDir) < 2 )
					return AI_ACTION_FLANK_LEFT ;
				else
					pSoldier->numFlanks = MAX_FLANKS_YELLOW;
			}
		}
		else
		{
			if ( origDir < currDir )
				origDir += 8;

			if ( (origDir - currDir) >= 4 )
			{
				pSoldier->numFlanks = MAX_FLANKS_YELLOW;
			}
			else
			{
				pSoldier->aiData.usActionData = FindFlankingSpot (pSoldier, tempGridNo , AI_ACTION_FLANK_RIGHT);
				if ( pSoldier->aiData.usActionData != NOWHERE )//&& (origDir - currDir) < 2 )
					return AI_ACTION_FLANK_RIGHT ;
				else
					pSoldier->numFlanks = MAX_FLANKS_YELLOW;
			}
		}
	}

	if ( pSoldier->numFlanks == MAX_FLANKS_YELLOW )
	{
		pSoldier->numFlanks += 1;
		pSoldier->aiData.usActionData = GoAsFarAsPossibleTowards(pSoldier,tempGridNo,AI_ACTION_SEEK_NOISE);
		return AI_ACTION_SEEK_NOISE ;
	}

	return AI_ACTION_NOT_AN_ACTION;
}

INT8 YellowAlert_TryToRadioYellowAlert(SOLDIERTYPE* pSoldier, YellowAlertFlags& flags)
{
	PERFORMANCE_MARKER
	INT32 iChance;
	// if we have the action points remaining to RADIO
	// (we never want NPCs to choose to radio if they would have to wait a turn)
	if ( !flags.fCivilian && (pSoldier->bActionPoints >= AP_RADIO) &&
		(gTacticalStatus.Team[pSoldier->bTeam].bMenInSector > 1) )
	{
		// base chance depends on how much new info we have to radio to the others
		iChance = 5 * WhatIKnowThatPublicDont(pSoldier,FALSE);   // use 5 * for YELLOW alert

		// if I actually know something they don't and I ain't swimming (deep water)
		if (iChance && !DeepWater( pSoldier->sGridNo ))
		{

			// CJC: this addition allows for varying difficulty levels for soldier types
			iChance += gbDiff[ DIFF_RADIO_RED_ALERT ][ SoldierDifficultyLevel( pSoldier ) ] / 2;

			// Alex: this addition replaces the sectorValue/2 in original JA
			//iChance += gsDiff[DIFF_RADIO_RED_ALERT][GameOption[ENEMYDIFFICULTY]] / 2;

			// modify base chance according to orders
			switch (pSoldier->aiData.bOrders)
			{
			case STATIONARY: iChance +=  20;  break;
			case ONGUARD:    iChance +=  15;  break;
			case ONCALL:     iChance +=  10;  break;
			case CLOSEPATROL:                 break;
			case RNDPTPATROL:
			case POINTPATROL:                 break;
			case FARPATROL:  iChance += -10;  break;
			case SEEKENEMY:  iChance += -20;  break;
			case SNIPER:		iChance += -10; break; //Madd: sniper contacts are supposed to be automatically reported
			}

			// modify base chance according to attitude
			switch (pSoldier->aiData.bAttitude)
			{
			case DEFENSIVE:  iChance +=  20;  break;
			case BRAVESOLO:  iChance += -10;  break;
			case BRAVEAID:                    break;
			case CUNNINGSOLO:iChance +=  -5;  break;
			case CUNNINGAID:                  break;
			case AGGRESSIVE: iChance += -20;  break;
			case ATTACKSLAYONLY: iChance = 0; break;
			}

#ifdef DEBUGDECISIONS
			AINumMessage("Chance to radio yellow alert = ",iChance);
#endif

			if ((INT16)PreRandom(100) < iChance)
			{
#ifdef DEBUGDECISIONS
				AINameMessage(pSoldier,"decides to radio a YELLOW alert!",1000);
#endif

				return(AI_ACTION_YELLOW_ALERT);
			}
		}
	}
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 YellowAlert_TryToSeekNoise(SOLDIERTYPE* pSoldier, YellowAlertFlags& flags)
{
	PERFORMANCE_MARKER
	// remember that noise value is negative, and closer to 0 => more important!
	INT32 iChance = 95 + (flags.iNoiseValue / 3);
	INT32 iSneaky = 30;

	// increase

	// set base chance according to orders
	switch (pSoldier->aiData.bOrders)
	{
	case STATIONARY:     iChance += -20;  break;
	case ONGUARD:        iChance += -15;  break;
	case ONCALL:                          break;
	case CLOSEPATROL:    iChance += -10;  break;
	case RNDPTPATROL:
	case POINTPATROL:                     break;
	case FARPATROL:      iChance +=  10;  break;
	case SEEKENEMY:      iChance +=  25;  break;
	case SNIPER:		  iChance += -10; break;
	}

	// modify chance of patrol (and whether it's a sneaky one) by attitude
	switch (pSoldier->aiData.bAttitude)
	{
	case DEFENSIVE:      iChance += -10;  iSneaky +=  15;  break;
	case BRAVESOLO:      iChance +=  10;                   break;
	case BRAVEAID:       iChance +=   5;                   break;
	case CUNNINGSOLO:    iChance +=   5;  iSneaky +=  30;  break;
	case CUNNINGAID:                      iSneaky +=  30;  break;
	case AGGRESSIVE:     iChance +=  20;  iSneaky += -10;  break;
	case ATTACKSLAYONLY:	iChance +=  20;  iSneaky += -10;  break;
	}


	// reduce chance if breath is down, less likely to wander around when tired
	iChance -= (100 - pSoldier->bBreath);

	//Madd: make militia less likely to go running headlong into trouble
	if ( pSoldier->bTeam == MILITIA_TEAM )
		iChance -= 30;

	if ((INT16) PreRandom(100) < iChance  )
	{

		pSoldier->aiData.usActionData = GoAsFarAsPossibleTowards(pSoldier,flags.sNoiseGridNo,AI_ACTION_SEEK_NOISE);

		if (pSoldier->aiData.usActionData != NOWHERE)
		{
#ifdef DEBUGDECISIONS
			std::string tempstr = String("%s - INVESTIGATING NOISE at grid %d, moving to %d",
				pSoldier->name,flags.sNoiseGridNo,pSoldier->aiData.usActionData);
			DebugAI(tempstr);
#endif

			if ( flags.fClimb )//&& pSoldier->aiData.usActionData == flags.sNoiseGridNo)
			{
				// need to climb AND have enough APs to get there this turn
				BOOLEAN fUp = TRUE;
				if (pSoldier->pathing.bLevel > 0 )
					fUp = FALSE;

				if (!fUp)
					DebugMsg ( TOPIC_JA2AI , DBG_LEVEL_3 , String("Soldier %d, is climbing down",pSoldier->ubID) );

				if ( CanClimbFromHere ( pSoldier, fUp ) )
				{
					if (IsActionAffordable(pSoldier) && pSoldier->bActionPoints >= ( AP_CLIMBROOF + MinAPsToAttack( pSoldier, flags.sNoiseGridNo, ADDTURNCOST)))
					{
						return( AI_ACTION_CLIMB_ROOF );
					}
				}
				else
				{
					pSoldier->aiData.usActionData = FindClosestClimbPoint(pSoldier->sGridNo , flags.sNoiseGridNo , fUp );
					if ( pSoldier->aiData.usActionData != NOWHERE )
					{
						return( AI_ACTION_MOVE_TO_CLIMB  );
					}
				}
			}

			if ( ( pSoldier->aiData.bAttitude == CUNNINGAID || pSoldier->aiData.bAttitude == CUNNINGSOLO || pSoldier->aiData.bAttitude == BRAVESOLO ) )
			{
				INT8 action = AI_ACTION_SEEK_NOISE;
				INT16 dist = PythSpacesAway ( pSoldier->sGridNo, flags.sNoiseGridNo );
				if ( dist > MIN_FLANK_DIST_YELLOW && dist < MAX_FLANK_DIST_YELLOW  )
				{
					INT16 rdm = Random(6);

					switch (rdm)
					{
					case 1:
					case 2:
					case 3:
						if ( pSoldier->aiData.bLastAction != AI_ACTION_FLANK_LEFT && pSoldier->aiData.bLastAction != AI_ACTION_FLANK_RIGHT )
							action = AI_ACTION_FLANK_LEFT ;
						break;
					default:
						if ( pSoldier->aiData.bLastAction != AI_ACTION_FLANK_LEFT && pSoldier->aiData.bLastAction != AI_ACTION_FLANK_RIGHT )
							action = AI_ACTION_FLANK_RIGHT ;
						break;
					}
				}
				else
					return AI_ACTION_SEEK_NOISE ;

				pSoldier->aiData.usActionData = FindFlankingSpot (pSoldier, flags.sNoiseGridNo, action );

				if ( pSoldier->aiData.usActionData == NOWHERE || pSoldier->numFlanks >= MAX_FLANKS_YELLOW  )
				{
					pSoldier->aiData.usActionData = GoAsFarAsPossibleTowards(pSoldier,flags.sNoiseGridNo,AI_ACTION_SEEK_NOISE);
					//pSoldier->numFlanks = 0;
					return(AI_ACTION_SEEK_NOISE);
				}
				else
				{
					if ( action == AI_ACTION_FLANK_LEFT )
						pSoldier->flags.lastFlankLeft = TRUE;
					else
						pSoldier->flags.lastFlankLeft = FALSE;

					if ( pSoldier->lastFlankSpot != flags.sNoiseGridNo)
						pSoldier->numFlanks = 0;

					pSoldier->origDir = GetDirectionFromGridNo ( flags.sNoiseGridNo, pSoldier);
					pSoldier->lastFlankSpot = flags.sNoiseGridNo;
					pSoldier->numFlanks++;
					return(action);
				}
			}
			else
			{
				return(AI_ACTION_SEEK_NOISE);
			}

		}
	}
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 YellowAlert_TryToSeekFriend(SOLDIERTYPE* pSoldier, YellowAlertFlags& flags)
{
	PERFORMANCE_MARKER
	INT16 sClosestFriend = ClosestReachableFriendInTrouble(pSoldier, &flags.fClimb);

	// if there is a friend alive & reachable who last radioed in
	if (sClosestFriend != NOWHERE)
	{
		// there a chance enemy soldier choose to go "help" his friend
		INT32 iChance = 50 - SpacesAway(pSoldier->sGridNo,sClosestFriend);
		INT32 iSneaky = 10;

		// set base chance according to orders
		switch (pSoldier->aiData.bOrders)
		{
		case STATIONARY:     iChance += -20;  break;
		case ONGUARD:        iChance += -15;  break;
		case ONCALL:         iChance +=  20;  break;
		case CLOSEPATROL:    iChance += -10;  break;
		case RNDPTPATROL:
		case POINTPATROL:    iChance += -10;  break;
		case FARPATROL:                       break;
		case SEEKENEMY:      iChance +=  10;  break;
		case SNIPER:		  iChance += -10; break;
		}

		// modify chance of patrol (and whether it's a sneaky one) by attitude
		switch (pSoldier->aiData.bAttitude)
		{
		case DEFENSIVE:      iChance += -10;  iSneaky +=  15;        break;
		case BRAVESOLO:                                              break;
		case BRAVEAID:       iChance +=  20;  iSneaky += -10;        break;
		case CUNNINGSOLO:					   iSneaky +=  30;		  break;
		case CUNNINGAID:     iChance +=  20;  iSneaky +=  20;        break;
		case AGGRESSIVE:     iChance += -20;  iSneaky += -20;        break;
		case ATTACKSLAYONLY: iChance += -20;  iSneaky += -20;        break;
		}

		// reduce chance if breath is down, less likely to wander around when tired
		iChance -= (100 - pSoldier->bBreath);

		if ((INT16)PreRandom(100) < iChance)
		{
			pSoldier->aiData.usActionData = GoAsFarAsPossibleTowards(pSoldier,sClosestFriend,AI_ACTION_SEEK_FRIEND);

			if (pSoldier->aiData.usActionData != NOWHERE)
			{
#ifdef DEBUGDECISIONS
				std::string tempstr = String("%s - SEEKING FRIEND at %d, MOVING to %d",
					pSoldier->name,sClosestFriend,pSoldier->aiData.usActionData);
				DebugAI(tempstr);
#endif

				if ( flags.fClimb )//&& pSoldier->aiData.usActionData == sClosestFriend)
				{
					// need to climb AND have enough APs to get there this turn
					BOOLEAN fUp = TRUE;
					if (pSoldier->pathing.bLevel > 0 )
						fUp = FALSE;

					if (!fUp)
						DebugMsg ( TOPIC_JA2AI , DBG_LEVEL_3 , String("Soldier %d is climbing down",pSoldier->ubID) );

					if ( CanClimbFromHere ( pSoldier, fUp ) )
					{
						if (IsActionAffordable(pSoldier) )
						{
							return( AI_ACTION_CLIMB_ROOF );
						}
					}
					else
					{
						pSoldier->aiData.usActionData = FindClosestClimbPoint(pSoldier->sGridNo , sClosestFriend , fUp );
						if ( pSoldier->aiData.usActionData != NOWHERE )
						{
							return( AI_ACTION_MOVE_TO_CLIMB  );
						}
					}
				}

				//if (flags.fClimb && pSoldier->aiData.usActionData == sClosestFriend)
				//{
				//// need to climb AND have enough APs to get there this turn
				//return( AI_ACTION_CLIMB_ROOF );
				//}

				return(AI_ACTION_SEEK_FRIEND);
			}
		}
	}
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 YellowAlert_TryToTakeCover(SOLDIERTYPE* pSoldier, YellowAlertFlags& flags)
{
	PERFORMANCE_MARKER
	// remember that noise value is negative, and closer to 0 => more important!
	INT32 iChance = 25;
	INT32 iSneaky = 30;

	// set base chance according to orders
	switch (pSoldier->aiData.bOrders)
	{
	case STATIONARY:     iChance +=  20;  break;
	case ONGUARD:        iChance +=  15;  break;
	case ONCALL:                          break;
	case CLOSEPATROL:    iChance +=  10;  break;
	case RNDPTPATROL:
	case POINTPATROL:                     break;
	case FARPATROL:      iChance +=  -5;  break;
	case SEEKENEMY:      iChance += -20;  break;
	case SNIPER:		  iChance +=  20; break;
	}

	// modify chance (and whether it's sneaky) by attitude
	switch (pSoldier->aiData.bAttitude)
	{
	case DEFENSIVE:      iChance +=  10;  iSneaky +=  15;  break;
	case BRAVESOLO:      iChance += -15;  iSneaky += -20;  break;
	case BRAVEAID:       iChance += -20;  iSneaky += -20;  break;
	case CUNNINGSOLO:    iChance +=  20;  iSneaky +=  30;  break;
	case CUNNINGAID:     iChance +=  15;  iSneaky +=  30;  break;
	case AGGRESSIVE:     iChance += -10;  iSneaky += -10;  break;
	case ATTACKSLAYONLY: iChance += -10;  iSneaky += -10;  break;
	}


	//Madd: make militia more likely to take cover
	if ( pSoldier->bTeam == MILITIA_TEAM )
		iChance += 20;

	// reduce chance if breath is down, less likely to wander around when tired
	iChance -= (100 - pSoldier->bBreath);

	if ((INT16)PreRandom(100) < iChance)
	{
		pSoldier->aiData.bAIMorale = CalcMorale( pSoldier );
		pSoldier->aiData.usActionData = FindBestNearbyCover(pSoldier);

		if (pSoldier->aiData.usActionData != NOWHERE)
		{
#ifdef DEBUGDECISIONS
			std::string tempstr = String("%s - TAKING COVER at grid %d",pSoldier->name,pSoldier->aiData.usActionData);
			DebugAI(tempstr);
#endif

			return(AI_ACTION_TAKE_COVER);
		}
	}
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 DecideActionYellow(SOLDIERTYPE *pSoldier)
{
	PERFORMANCE_MARKER
	YellowAlertFlags flags;
	flags.fCivilian = (PTR_CIVILIAN && (pSoldier->ubCivilianGroup == NON_CIV_GROUP || pSoldier->aiData.bNeutral || (pSoldier->ubBodyType >= FATCIV && pSoldier->ubBodyType <= CRIPPLECIV) ) );
	INT8	bActionReturned;
#ifdef DEBUGDECISIONS
	std::string tempstr;
#endif


	if (flags.fCivilian)
	{
		if (pSoldier->flags.uiStatusFlags & SOLDIER_COWERING)
		{
			// everything's peaceful again, stop cowering!!
			pSoldier->aiData.usActionData = ANIM_STAND;
			return( AI_ACTION_STOP_COWERING );
		}
		if (!gfTurnBasedAI)
		{
			// ******************
			// REAL TIME NPC CODE
			// ******************
			if (pSoldier->ubProfile != NO_PROFILE)
			{
				pSoldier->aiData.bAction = DecideActionNamedNPC( pSoldier );
				if ( pSoldier->aiData.bAction != AI_ACTION_NONE )
				{
					return( pSoldier->aiData.bAction );
				}
			}
		}
	}

	// determine the most important noise heard, and its relative value
	flags.sNoiseGridNo = MostImportantNoiseHeard(pSoldier,&flags.iNoiseValue, &flags.fClimb, &flags.fReachable);
	//NumMessage("flags.iNoiseValue = ",flags.iNoiseValue);

	if (flags.sNoiseGridNo == NOWHERE)
	{
		// then we have no business being under YELLOW status any more!
#ifdef RECORDNET
		fprintf(NetDebugFile,"\nDecideActionYellow: ERROR - No important noise known by guynum %d\n\n",pSoldier->ubID);
#endif

#ifdef BETAVERSION
		NumMessage("DecideActionYellow: ERROR - No important noise known by guynum ",pSoldier->ubID);
#endif

		return(AI_ACTION_NONE);
	}



	////////////////////////////////////////////////////////////////////////////
	// LOOK AROUND TOWARD NOISE: determine %chance for man to turn towards noise
	////////////////////////////////////////////////////////////////////////////
	bActionReturned = YellowAlert_TryToLookAround(pSoldier, flags);
	if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
		return(bActionReturned);


	////////////////////////////////////////////////////////////////////////////
	// RADIO YELLOW ALERT: determine %chance to call others and report noise
	////////////////////////////////////////////////////////////////////////////
	bActionReturned = YellowAlert_TryToRadioYellowAlert(pSoldier, flags);
	if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
		return(bActionReturned);


	if ( TANK( pSoldier ) )
	{
		return( AI_ACTION_NONE );
	}

	////////////////////////////////////////////////////////////////////////
	// REST IF RUNNING OUT OF BREATH
	////////////////////////////////////////////////////////////////////////

	// if our breath is running a bit low, and we're not in water
	if ((pSoldier->bBreath < 25) && !pSoldier->MercInWater())
	{
		// take a breather for gods sake!
		pSoldier->aiData.usActionData = NOWHERE;
		return(AI_ACTION_NONE);
	}


	bActionReturned = YellowAlert_TryToFlank(pSoldier, flags);
	if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
		return(bActionReturned);


	if ( !( pSoldier->bTeam == CIV_TEAM && pSoldier->ubProfile != NO_PROFILE && pSoldier->ubProfile != ELDIN ) )
	{
		// IF WE ARE MILITIA/CIV IN REALTIME, CLOSE TO NOISE, AND CAN SEE THE SPOT WHERE THE NOISE CAME FROM, FORGET IT
		if ( flags.fReachable && !flags.fClimb && !gfTurnBasedAI && (pSoldier->bTeam == MILITIA_TEAM || pSoldier->bTeam == CIV_TEAM )&& PythSpacesAway( pSoldier->sGridNo, flags.sNoiseGridNo ) < 5 )
		{
			if ( SoldierTo3DLocationLineOfSightTest( pSoldier, flags.sNoiseGridNo, pSoldier->pathing.bLevel, 0, 6, TRUE )	)
			{
				// set reachable to false so we don't investigate
				flags.fReachable = FALSE;
				// forget about noise
				pSoldier->aiData.sNoiseGridno = NOWHERE;
				pSoldier->aiData.ubNoiseVolume = 0;
			}
		}

		////////////////////////////////////////////////////////////////////////////
		// SEEK NOISE
		////////////////////////////////////////////////////////////////////////////

		if ( flags.fReachable )
		{
			bActionReturned = YellowAlert_TryToSeekNoise(pSoldier, flags);
			if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
				return(bActionReturned);
		}


		////////////////////////////////////////////////////////////////////////////
		// SEEK FRIEND WHO LAST RADIOED IN TO REPORT NOISE
		////////////////////////////////////////////////////////////////////////////
		bActionReturned = YellowAlert_TryToSeekFriend(pSoldier, flags);
		if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
			return(bActionReturned);



		////////////////////////////////////////////////////////////////////////////
		// TAKE BEST NEARBY COVER FROM THE NOISE GENERATING GRIDNO
		////////////////////////////////////////////////////////////////////////////

		if (!SkipCoverCheck ) // && gfTurnBasedAI) // only do in turnbased
		{
			bActionReturned = YellowAlert_TryToTakeCover(pSoldier, flags);
			if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
				return(bActionReturned);
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// SWITCH TO GREEN: determine if soldier acts as if nothing at all was wrong
	////////////////////////////////////////////////////////////////////////////
	if ((INT16)PreRandom(100) < 50 )
	{
#ifdef RECORDNET
		fprintf(NetDebugFile,"\tDecideActionYellow: guynum %d ignores noise, switching to GREEN AI...\n",pSoldier->ubID);
#endif

#ifdef DEBUGDECISIONS
		AINameMessage(pSoldier,"ignores noise completely and BYPASSES to GREEN!",1000);
#endif

		// Skip YELLOW until new situation, 15% extra chance to do GREEN actions
		pSoldier->aiData.bBypassToGreen = 15;
		return(DecideActionGreen(pSoldier));
	}


	////////////////////////////////////////////////////////////////////////////
	// CROUCH IF NOT CROUCHING ALREADY
	////////////////////////////////////////////////////////////////////////////

	// if not in water and not already crouched, try to crouch down first
	if (!flags.fCivilian && !PTR_CROUCHED && IsValidStance( pSoldier, ANIM_CROUCH ) )
	{
#ifdef DEBUGDECISIONS
		tempstr = String("%s CROUCHES (STATUS YELLOW)",pSoldier->name);
		DebugAI(tempstr);
#endif

		if (!gfTurnBasedAI || GetAPsToChangeStance( pSoldier, ANIM_CROUCH ) <= pSoldier->bActionPoints)
		{
			pSoldier->aiData.usActionData = ANIM_CROUCH;
			return(AI_ACTION_CHANGE_STANCE);
		}
	}


	////////////////////////////////////////////////////////////////////////////
	// DO NOTHING: Not enough points left to move, so save them for next turn
	////////////////////////////////////////////////////////////////////////////

#ifdef DEBUGDECISIONS
	AINameMessage(pSoldier,"- DOES NOTHING (YELLOW)",1000);
#endif

	// by default, if everything else fails, just stands in place without turning
	pSoldier->aiData.usActionData = NOWHERE;
	return(AI_ACTION_NONE);
}

struct RedAlertFlags
{
	UINT8 ubUnconsciousOK, ubCanMove;
	INT8 bInWater, bInDeepWater, bInGas;
	BOOLEAN fCivilian;
};

INT8 RedAlert_TryToCower(SOLDIERTYPE *pSoldier, RedAlertFlags& flags)
{
	PERFORMANCE_MARKER
	if ( FindAIUsableObjClass( pSoldier, IC_WEAPON ) == ITEM_NOT_FOUND )
	{
		// cower in fear!!
		if ( pSoldier->flags.uiStatusFlags & SOLDIER_COWERING )
		{
			if ( gfTurnBasedAI || gTacticalStatus.fEnemyInSector ) // battle!
			{
				// in battle!
				if ( pSoldier->aiData.bLastAction == AI_ACTION_COWER )
				{
					// do nothing
					pSoldier->aiData.usActionData = NOWHERE;
					return( AI_ACTION_NONE );
				}
				else
				{
					// set up next action to run away
					pSoldier->aiData.usNextActionData = FindSpotMaxDistFromOpponents( pSoldier );
					if ( pSoldier->aiData.usNextActionData != NOWHERE )
					{
						pSoldier->aiData.bNextAction = AI_ACTION_RUN_AWAY;
						pSoldier->aiData.usActionData = ANIM_STAND;
						return( AI_ACTION_STOP_COWERING );
					}
					else
					{
						return( AI_ACTION_NONE );
					}
				}
			}
			else
			{
				if ( pSoldier->aiData.bNewSituation == NOT_NEW_SITUATION )
				{
					// stop cowering, not in battle, timer expired
					// we have to turn off whatever is necessary to stop status red...
					pSoldier->aiData.bAlertStatus = STATUS_GREEN;
					return( AI_ACTION_STOP_COWERING );
				}
				else
				{
					return( AI_ACTION_NONE );
				}
			}
		}
		else
		{
			if ( gfTurnBasedAI || gTacticalStatus.fEnemyInSector )
			{
				// battle - cower!!!
				pSoldier->aiData.usActionData = ANIM_CROUCH;
				return( AI_ACTION_COWER );
			}
			else // not in battle, cower for a certain length of time
			{
				pSoldier->aiData.bNextAction = AI_ACTION_WAIT;
				pSoldier->aiData.usNextActionData = (UINT16) REALTIME_CIV_AI_DELAY;
				pSoldier->aiData.usActionData = ANIM_CROUCH;
				return( AI_ACTION_COWER );
			}
		}
	}
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 RedAlert_TryLongRangeWeapons(SOLDIERTYPE *pSoldier, RedAlertFlags& flags)
{
	PERFORMANCE_MARKER
	ATTACKTYPE BestThrow, BestShot;
	UINT8 ubOpponentDir;
	INT16 sCheckGridNo;
	BestThrow.ubPossible = FALSE;    // by default, assume Throwing isn't possible
	CheckIfTossPossible(pSoldier,&BestThrow);

	if (BestThrow.ubPossible)
	{
		// if firing mortar make sure we have room
		if ( Item[pSoldier->inv[ BestThrow.bWeaponIn ].usItem].mortar )
		{
			ubOpponentDir = (UINT8)GetDirectionFromGridNo( BestThrow.sTarget, pSoldier );

			// Get new gridno!
			sCheckGridNo = NewGridNo( (UINT16)pSoldier->sGridNo, (UINT16)DirectionInc( ubOpponentDir ) );

			if ( OKFallDirection( pSoldier, sCheckGridNo, pSoldier->pathing.bLevel, ubOpponentDir, pSoldier->usAnimState ) )
			{

				// then do it!  The functions have already made sure that we have a
				// pair of worthy opponents, etc., so we're not just wasting our time

				// if necessary, swap the usItem from holster into the hand position
				AssureItemIsInHandPos(pSoldier, BestThrow.bWeaponIn, FOREVER);

				pSoldier->aiData.usActionData = BestThrow.sTarget;
				pSoldier->aiData.bAimTime			= BestThrow.ubAimTime;

				return(AI_ACTION_TOSS_PROJECTILE);
			}
			else
			{					// can't fire!
				BestThrow.ubPossible = FALSE;

				// try behind us, see if there's room to move back
				sCheckGridNo = NewGridNo( (UINT16)pSoldier->sGridNo, (UINT16)DirectionInc( gOppositeDirection[ ubOpponentDir ] ) );
				if ( OKFallDirection( pSoldier, sCheckGridNo, pSoldier->pathing.bLevel, gOppositeDirection[ ubOpponentDir ], pSoldier->usAnimState ) )
				{
					pSoldier->aiData.usActionData = sCheckGridNo;

					return( AI_ACTION_GET_CLOSER );
				}
			}
		}
	}
	else		// toss/throw/launch not possible
	{
 // WDS - Fix problem when there is no "best thrown" weapon (i.e., BestThrow.bWeaponIn == NO_SLOT)
		// if this dude has a longe-range weapon on him (longer than normal
		// sight range), and there's at least one other team-mate around, and
		// spotters haven't already been called for, then DO SO!

		if ( (BestThrow.bWeaponIn != NO_SLOT) &&
			(CalcMaxTossRange( pSoldier, pSoldier->inv[BestThrow.bWeaponIn].usItem, TRUE ) > MaxDistanceVisible() ) &&
			(gTacticalStatus.Team[pSoldier->bTeam].bMenInSector > 1) &&
			(gTacticalStatus.ubSpottersCalledForBy == NOBODY))
		{
			// then call for spotters!  Uses up the rest of his turn (whatever
			// that may be), but from now on, BLACK AI NPC may radio sightings!
			gTacticalStatus.ubSpottersCalledForBy = pSoldier->ubID;
			pSoldier->bActionPoints = 0;

#ifdef DEBUGDECISIONS
			AINameMessage(pSoldier,"calls for spotters!",1000);
#endif

			pSoldier->aiData.usActionData = NOWHERE;
			return(AI_ACTION_NONE);
		}
	}




	// SNIPER!
	CheckIfShotPossible(pSoldier,&BestShot,FALSE);
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("decideactionred: is sniper shot possible? = %d, CTH = %d",BestShot.ubPossible,BestShot.ubChanceToReallyHit));

	if (BestShot.ubPossible && BestShot.ubChanceToReallyHit > 50 )
	{
		// then do it!  The functions have already made sure that we have a
		// pair of worthy opponents, etc., so we're not just wasting our time

		// if necessary, swap the usItem from holster into the hand position
		AssureItemIsInHandPos(pSoldier, BestShot.bWeaponIn, FOREVER);

		pSoldier->aiData.usActionData = BestShot.sTarget;
		pSoldier->aiData.bAimTime			= BestShot.ubAimTime;
		ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Sniper!" );
		return(AI_ACTION_FIRE_GUN );
	}
	else		// snipe not possible
	{
		// if this dude has a longe-range weapon on him (longer than normal
		// sight range), and there's at least one other team-mate around, and
		// spotters haven't already been called for, then DO SO!

		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: sniper shot not possible");
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("decideactionred: weapon in slot #%d",BestShot.bWeaponIn));
	// WDS - Fix problem when there is no "best shot" weapon (i.e., BestShot.bWeaponIn == NO_SLOT)
		if (BestShot.bWeaponIn != NO_SLOT) {
			OBJECTTYPE * gun = &pSoldier->inv[BestShot.bWeaponIn];
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("decideactionred: men in sector %d, ubspotters called by %d, nobody %d",gTacticalStatus.Team[pSoldier->bTeam].bMenInSector,gTacticalStatus.ubSpottersCalledForBy,NOBODY ));

			if ( ( ( IsScoped(gun) && GunRange(gun) > MaxDistanceVisible() ) || pSoldier->aiData.bOrders == SNIPER ) &&
				(gTacticalStatus.Team[pSoldier->bTeam].bMenInSector > 1) &&
				(gTacticalStatus.ubSpottersCalledForBy == NOBODY))

			{
				// then call for spotters!  Uses up the rest of his turn (whatever
				// that may be), but from now on, BLACK AI NPC may radio sightings!
				gTacticalStatus.ubSpottersCalledForBy = pSoldier->ubID;
				pSoldier->bActionPoints = 0;

				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: calling for sniper spotters");

				pSoldier->aiData.usActionData = NOWHERE;
				return(AI_ACTION_NONE);
			}
		}
	}

	//SUPPRESSION FIRE

	CheckIfShotPossible(pSoldier,&BestShot,TRUE);

	//must have a small chance to hit and the opponent must be on the ground (can't suppress guys on the roof)
	if ( BestShot.ubPossible && BestShot.ubChanceToReallyHit < 50 && Menptr[BestShot.ubOpponent].pathing.bLevel == 0 && pSoldier->aiData.bOrders != SNIPER )
	{
		// then do it!

		// if necessary, swap the usItem from holster into the hand position
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: suppression fire possible!");

		AssureItemIsInHandPos(pSoldier, BestShot.bWeaponIn, FOREVER);

		pSoldier->aiData.usActionData = BestShot.sTarget;
		//pSoldier->aiData.bAimTime			= BestShot.ubAimTime;
		UINT8 ubBurstAPs = 0;

		pSoldier->bDoAutofire = 0;
		do
		{
			pSoldier->bDoAutofire++;
			ubBurstAPs = CalcAPsToAutofire( pSoldier->CalcActionPoints( ), &(pSoldier->inv[BestShot.bWeaponIn]), pSoldier->bDoAutofire );
		}
		while(	pSoldier->bActionPoints >= BestShot.ubAPCost + ubBurstAPs &&
			pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft >= pSoldier->bDoAutofire &&
			GetAutoPenalty(&pSoldier->inv[ pSoldier->ubAttackingHand ], gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE)*pSoldier->bDoAutofire <= 80 );


		pSoldier->bDoAutofire--;

		// Make sure we decided to fire at least one shot!
		ubBurstAPs = CalcAPsToAutofire( pSoldier->CalcActionPoints( ), &(pSoldier->inv[BestShot.bWeaponIn]), pSoldier->bDoAutofire );

		// minimum 10 bullets
		// Hmmm, automatic suppression?  Howcome we don't get this?
		if (pSoldier->bDoAutofire >= 10 &&  pSoldier->bActionPoints >= BestShot.ubAPCost + ubBurstAPs )
		{
			pSoldier->aiData.bAimTime			= 0;
			pSoldier->bDoBurst			= 1;

			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Suppression Fire!" );
			Menptr[BestShot.ubOpponent].ubSuppressionPoints += pSoldier->bDoAutofire;
			Menptr[BestShot.ubOpponent].ubSuppressorID = pSoldier->ubID;
			return( AI_ACTION_FIRE_GUN );
		}
		else
		{
			pSoldier->aiData.bAimTime			= 0;
			pSoldier->bDoBurst			= 0;
			pSoldier->bDoAutofire		= 0;
			// not enough aps - do somthing else
		}
	}
	// suppression not possible, do something else
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 RedAlert_TryToRest(SOLDIERTYPE* pSoldier, RedAlertFlags& flags)
{
	PERFORMANCE_MARKER
	// if not already crouched, try to crouch down first
	if (!flags.fCivilian && !PTR_CROUCHED && IsValidStance( pSoldier, ANIM_CROUCH ) && gAnimControl[ pSoldier->usAnimState ].ubHeight != ANIM_PRONE)
	{
#ifdef DEBUGDECISIONS
		std::string tempstr = String("%s CROUCHES, NEEDING REST (STATUS RED), breath = %d",pSoldier->name,pSoldier->bBreath);
		DebugAI(tempstr);
#endif

		if (!gfTurnBasedAI || GetAPsToChangeStance( pSoldier, ANIM_CROUCH ) <= pSoldier->bActionPoints)
		{
			pSoldier->aiData.usActionData = ANIM_CROUCH;

			return(AI_ACTION_CHANGE_STANCE);
		}
	}

#ifdef DEBUGDECISIONS
	std::string tempstr = String("%s RESTS (STATUS RED), breath = %d",pSoldier->name,pSoldier->bBreath);
	DebugAI(tempstr);
#endif

	pSoldier->aiData.usActionData = NOWHERE;
	return(AI_ACTION_NONE);
}

INT8 RedAlert_TryToRadioRedAlert(SOLDIERTYPE *pSoldier, RedAlertFlags& flags)
{
	PERFORMANCE_MARKER
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: checking to radio red alert");
	INT16 iChance;

	// if there hasn't been an initial RED ALERT yet in this sector
	if ( !(gTacticalStatus.Team[pSoldier->bTeam].bAwareOfOpposition) || NeedToRadioAboutPanicTrigger() )
		// since I'm at STATUS RED, I obviously know we're being invaded!
		iChance = gbDiff[DIFF_RADIO_RED_ALERT][ SoldierDifficultyLevel( pSoldier ) ];
	else // subsequent radioing (only to update enemy positions, request help)
		// base chance depends on how much new info we have to radio to the others
		iChance = 10 * WhatIKnowThatPublicDont(pSoldier,FALSE);  // use 10 * for RED alert

	// if I actually know something they don't and I ain't swimming (deep water)
	if (iChance && !flags.bInDeepWater)
	{
		// modify base chance according to orders
		switch (pSoldier->aiData.bOrders)
		{
		case STATIONARY:       iChance +=  20;  break;
		case ONGUARD:          iChance +=  15;  break;
		case ONCALL:           iChance +=  10;  break;
		case CLOSEPATROL:                       break;
		case RNDPTPATROL:
		case POINTPATROL:      iChance +=  -5;  break;
		case FARPATROL:        iChance += -10;  break;
		case SEEKENEMY:        iChance += -20;  break;
		case SNIPER:		   iChance += -10;  break; // Sniper contacts should be reported automatically
		}

		// modify base chance according to attitude
		switch (pSoldier->aiData.bAttitude)
		{
		case DEFENSIVE:        iChance +=  20;  break;
		case BRAVESOLO:        iChance += -10;  break;
		case BRAVEAID:                          break;
		case CUNNINGSOLO:      iChance +=  -5;  break;
		case CUNNINGAID:                        break;
		case AGGRESSIVE:       iChance += -20;  break;
		case ATTACKSLAYONLY:	iChance = 0;
		}

		if ( (gTacticalStatus.fPanicFlags & PANIC_TRIGGERS_HERE) && !gTacticalStatus.Team[pSoldier->bTeam].bAwareOfOpposition)
		{
			// ignore morale (which could be really high
		}
		else
		{
			// modify base chance according to morale
			switch (pSoldier->aiData.bAIMorale)
			{
			case MORALE_HOPELESS:  iChance *= 3;    break;
			case MORALE_WORRIED:   iChance *= 2;    break;
			case MORALE_NORMAL:                     break;
			case MORALE_CONFIDENT: iChance /= 2;    break;
			case MORALE_FEARLESS:  iChance /= 3;    break;
			}
		}

#ifdef DEBUGDECISIONS
		AINumMessage("Chance to radio RED alert = ",iChance);
#endif

		if ((INT16) PreRandom(100) < iChance)
		{
#ifdef DEBUGDECISIONS
			AINameMessage(pSoldier,"decides to radio a RED alert!",1000);
#endif

			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: decided to radio red alert");
			return(AI_ACTION_RED_ALERT);
		}
	}
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 RedAlert_TryMainAI(SOLDIERTYPE* pSoldier, RedAlertFlags& flags)
{
	PERFORMANCE_MARKER
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: main red ai");
	////////////////////////////////////////////////////////////////////////////
	// MAIN RED AI: Decide soldier's preference between SEEKING,HELPING & HIDING
	////////////////////////////////////////////////////////////////////////////

	INT8 bSeekPts = 0, bHelpPts = 0, bHidePts = 0, bWatchPts = 0;
	INT8 bHighestWatchLoc;
	UINT8 ubOpponentDir;
	INT16 sClosestOpponent, sClosestFriend;

	// get the location of the closest reachable opponent
	BOOLEAN fClimb;
	INT16 sClosestDisturbance = ClosestReachableDisturbance(pSoldier,flags.ubUnconsciousOK, &fClimb);

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: check to continue flanking");
	// continue flanking
	INT16 tempGridNo;
	if ( sClosestDisturbance == NOWHERE )
		tempGridNo = pSoldier->lastFlankSpot;
	else
		tempGridNo = sClosestDisturbance;

	if ( pSoldier->numFlanks > 0 && pSoldier->numFlanks < MAX_FLANKS_RED  && gAnimControl[ pSoldier->usAnimState ].ubHeight != ANIM_PRONE )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: continue flanking");
		INT16 currDir = GetDirectionFromGridNo ( tempGridNo, pSoldier );
		INT16 origDir = pSoldier->origDir;
		pSoldier->numFlanks += 1;
		if ( pSoldier->flags.lastFlankLeft )
		{
			if ( origDir > currDir )
				origDir -= 8;

			if ( (currDir - origDir) >= 4 )
			{
				pSoldier->numFlanks = MAX_FLANKS_RED;
			}
			else
			{
				pSoldier->aiData.usActionData = FindFlankingSpot (pSoldier, tempGridNo , AI_ACTION_FLANK_LEFT);
				if ( pSoldier->aiData.usActionData != NOWHERE ) //&& (currDir - origDir) < 2 )
					return AI_ACTION_FLANK_LEFT ;
				else
					pSoldier->numFlanks = MAX_FLANKS_RED;
			}
		}
		else
		{
			if ( origDir < currDir )
				origDir += 8;

			if ( (origDir - currDir) >= 4 )
			{
				pSoldier->numFlanks = MAX_FLANKS_RED;
			}
			else
			{
				pSoldier->aiData.usActionData = FindFlankingSpot (pSoldier, tempGridNo , AI_ACTION_FLANK_RIGHT);
				if ( pSoldier->aiData.usActionData != NOWHERE )//&& (origDir - currDir) < 2 )
					return AI_ACTION_FLANK_RIGHT ;
				else
					pSoldier->numFlanks = MAX_FLANKS_RED;
			}
		}
	}
	if ( pSoldier->numFlanks == MAX_FLANKS_RED )
	{
		pSoldier->numFlanks += 1;
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: stop flanking");
		if (PythSpacesAway ( pSoldier->sGridNo, tempGridNo ) > MIN_FLANK_DIST_RED * 2 )
		{
			pSoldier->aiData.usActionData = InternalGoAsFarAsPossibleTowards(pSoldier,tempGridNo,AP_PRONE,AI_ACTION_SEEK_OPPONENT,0);

			if ( LocationToLocationLineOfSightTest( pSoldier->aiData.usActionData, pSoldier->pathing.bLevel, tempGridNo, pSoldier->pathing.bLevel, (UINT8) MaxDistanceVisible(), TRUE ) )
			{
				// reserve APs for a possible crouch plus a shot
				pSoldier->aiData.usActionData = InternalGoAsFarAsPossibleTowards(pSoldier, tempGridNo, (INT8) (MinAPsToAttack( pSoldier, tempGridNo, ADDTURNCOST) + AP_CROUCH), AI_ACTION_SEEK_OPPONENT, FLAG_CAUTIOUS );
				if ( pSoldier->aiData.usActionData != NOWHERE )
				{
					pSoldier->aiData.fAIFlags |= AI_CAUTIOUS;
					pSoldier->aiData.bNextAction = AI_ACTION_END_TURN;
					return(AI_ACTION_SEEK_OPPONENT);
				}
			}
			else
			{
				return(AI_ACTION_SEEK_OPPONENT);
			}
		}
		else
		{
			pSoldier->aiData.usActionData = FindBestNearbyCover(pSoldier);
			return AI_ACTION_TAKE_COVER ;
		}
	}


	// if we can move at least 1 square's worth
	// and have more APs than we want to reserve
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("decideactionred: can we move? = %d, APs = %d",flags.ubCanMove,pSoldier->bActionPoints));

	if (flags.ubCanMove && pSoldier->bActionPoints > MAX_AP_CARRIED)
	{
		if (flags.fCivilian)
		{
			// only interested in hiding out...
			bSeekPts = -99;
			bHelpPts = -99;
			bHidePts =  +1;
		}
		else
		{

			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("decideactionred: checking hide/seek/help/watch points... orders = %d, attitude = %d",pSoldier->aiData.bOrders,pSoldier->aiData.bAttitude));
			// calculate initial points for watch based on highest watch loc

			bWatchPts = GetHighestWatchedLocPoints( pSoldier->ubID );
			if ( bWatchPts <= 0 )
			{
				// no watching
				bWatchPts = -99;
			}

			// modify RED movement tendencies according to morale
			switch (pSoldier->aiData.bAIMorale)
			{
			case MORALE_HOPELESS:  bSeekPts = -99; bHelpPts = -99; bHidePts  = +2; bWatchPts =	-99; break;
			case MORALE_WORRIED:   bSeekPts += -2; bHelpPts +=  0; bHidePts += +2; bWatchPts +=	1; break;
			case MORALE_NORMAL:    bSeekPts +=  0; bHelpPts +=  0; bHidePts +=  0; bWatchPts +=	0; break;
			case MORALE_CONFIDENT: bSeekPts += +1; bHelpPts +=  0; bHidePts += -1; bWatchPts +=	0; break;
			case MORALE_FEARLESS:  bSeekPts += +1; bHelpPts +=  0; bHidePts =  -1; bWatchPts +=  0; break;
			}

			// modify tendencies according to orders
			switch (pSoldier->aiData.bOrders)
			{
			case STATIONARY:   bSeekPts += -1; bHelpPts += -1; bHidePts += +1; bWatchPts += +1; break;
			case ONGUARD:      bSeekPts += -1; bHelpPts +=  0; bHidePts += +1; bWatchPts += +1; break;
			case CLOSEPATROL:  bSeekPts +=  0; bHelpPts +=  0; bHidePts +=  0; bWatchPts +=  0; break;
			case RNDPTPATROL:  bSeekPts +=  0; bHelpPts +=  0; bHidePts +=  0; bWatchPts +=  0; break;
			case POINTPATROL:  bSeekPts +=  0; bHelpPts +=  0; bHidePts +=  0; bWatchPts +=  0; break;
			case FARPATROL:    bSeekPts +=  0; bHelpPts +=  0; bHidePts +=  0; bWatchPts +=  0; break;
			case ONCALL:       bSeekPts +=  0; bHelpPts += +1; bHidePts += -1; bWatchPts +=  0; break;
			case SEEKENEMY:    bSeekPts += +1; bHelpPts +=  0; bHidePts += -1; bWatchPts += -1; break;
			case SNIPER:		bSeekPts += -1; bHelpPts +=  0; bHidePts += +1; bWatchPts += +1; break;
			}

			// modify tendencies according to attitude
			switch (pSoldier->aiData.bAttitude)
			{
			case DEFENSIVE:     bSeekPts += -1; bHelpPts +=  0; bHidePts += +2; bWatchPts += +1; break;
			case BRAVESOLO:     bSeekPts += +1; bHelpPts += -1; bHidePts += -1; bWatchPts += -1; break;
			case BRAVEAID:      bSeekPts += +1; bHelpPts += +1; bHidePts += -1; bWatchPts += -1; break;
			case CUNNINGSOLO:   bSeekPts +=  1; bHelpPts += -1; bHidePts += +1; bWatchPts +=  0; break;
			case CUNNINGAID:    bSeekPts +=  1; bHelpPts += +1; bHidePts += +1; bWatchPts +=  0; break;
			case AGGRESSIVE:    bSeekPts += +1; bHelpPts +=  0; bHidePts += -1; bWatchPts +=  0; break;
			case ATTACKSLAYONLY:bSeekPts += +1; bHelpPts +=  0; bHidePts += -1; bWatchPts +=  0; break;
			}

			//Madd: make militia less likely to go running headlong into trouble
			if ( pSoldier->bTeam == MILITIA_TEAM )
			{
				bSeekPts += -1; bHelpPts +=  0; bHidePts += +1; bWatchPts += +0;
			}
		}

		if (!gfTurnBasedAI)
		{
			// don't search for cover
			bHidePts = -99;
		}

		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("decideactionred: hide = %d, seek = %d, watch = %d, help = %d",bHidePts,bSeekPts,bWatchPts,bHelpPts));
		// while one of the three main RED REACTIONS remains viable
		while ((bSeekPts > -90) || (bHelpPts > -90) || (bHidePts > -90) )
		{
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: checking to seek");
			// if SEEKING is possible and at least as desirable as helping or hiding
			if ( ((bSeekPts > -90) && (bSeekPts >= bHelpPts) && (bSeekPts >= bHidePts) && (bSeekPts >= bWatchPts )) )
			{
#ifdef AI_TIMING_TESTS
				uiStartTime = GetJA2Clock();
#endif

#ifdef AI_TIMING_TESTS
				uiEndTime = GetJA2Clock();
				guiRedSeekTimeTotal += (uiEndTime - uiStartTime);
				guiRedSeekCounter++;
#endif
				// if there is an opponent reachable
				if (sClosestDisturbance != NOWHERE && gAnimControl[ pSoldier->usAnimState ].ubHeight != ANIM_PRONE )
				{
					DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: seek opponent");
					//////////////////////////////////////////////////////////////////////
					// SEEK CLOSEST DISTURBANCE: GO DIRECTLY TOWARDS CLOSEST KNOWN OPPONENT
					//////////////////////////////////////////////////////////////////////

					// try to move towards him
					pSoldier->aiData.usActionData = InternalGoAsFarAsPossibleTowards(pSoldier,sClosestDisturbance,AP_CROUCH,AI_ACTION_SEEK_OPPONENT,0);

					if (pSoldier->aiData.usActionData != NOWHERE)
					{
						// Check for a trap
						if ( !ArmySeesOpponents() )
						{
							if ( GetNearestRottingCorpseAIWarning( pSoldier->aiData.usActionData ) > 0 )
							{
								// abort! abort!
								pSoldier->aiData.usActionData = NOWHERE;
							}
						}
					}

					// if it's possible
					if (pSoldier->aiData.usActionData != NOWHERE)
					{
#ifdef DEBUGDECISIONS
						// do it!
						std::string tempstr = String("%s - SEEKING OPPONENT at grid %d, MOVING to %d",
							pSoldier->name,sClosestDisturbance,pSoldier->aiData.usActionData);
						DebugAI(tempstr);
#endif

						if (fClimb)//&& pSoldier->aiData.usActionData == sClosestDisturbance)
						{
							// need to climb AND have enough APs to get there this turn
							BOOLEAN fUp = TRUE;
							if (pSoldier->pathing.bLevel > 0 )
								fUp = FALSE;

							if (!fUp)
								DebugMsg ( TOPIC_JA2AI , DBG_LEVEL_3 , String("Soldier %d is climbing down",pSoldier->ubID) );

							if ( CanClimbFromHere ( pSoldier, fUp ) )
							{
								if (IsActionAffordable(pSoldier) && pSoldier->bActionPoints >= ( AP_CLIMBROOF + MinAPsToAttack( pSoldier, sClosestDisturbance, ADDTURNCOST)))
								{
									return( AI_ACTION_CLIMB_ROOF );
								}
							}
							else
							{
								pSoldier->aiData.usActionData = FindClosestClimbPoint(pSoldier->sGridNo , sClosestDisturbance , fUp );
								if ( pSoldier->aiData.usActionData != NOWHERE )
								{
									return( AI_ACTION_MOVE_TO_CLIMB  );
								}
							}
						}
						//if ( fClimb && pSoldier->aiData.usActionData == sClosestDisturbance)
						//{
						//	return( AI_ACTION_CLIMB_ROOF );
						//}


						if ( ( pSoldier->aiData.bAttitude == CUNNINGAID || pSoldier->aiData.bAttitude == CUNNINGSOLO  || pSoldier->aiData.bAttitude == BRAVESOLO )  && gAnimControl[ pSoldier->usAnimState ].ubHeight != ANIM_PRONE )
						{
							INT8 action = AI_ACTION_SEEK_OPPONENT;
							INT16 dist = PythSpacesAway ( pSoldier->sGridNo, sClosestDisturbance );
							if ( dist > MIN_FLANK_DIST_RED  && dist < MAX_FLANK_DIST_RED )
							{
								INT16 rdm = Random(6);

								switch (rdm)
								{
								case 1:
								case 2:
								case 3:
									if ( pSoldier->aiData.bLastAction != AI_ACTION_FLANK_LEFT && pSoldier->aiData.bLastAction != AI_ACTION_FLANK_RIGHT )
										action = AI_ACTION_FLANK_LEFT ;
									break;
								default:
									if ( pSoldier->aiData.bLastAction != AI_ACTION_FLANK_LEFT && pSoldier->aiData.bLastAction != AI_ACTION_FLANK_RIGHT )
										action = AI_ACTION_FLANK_RIGHT ;
									break;
								}

								if (action == AI_ACTION_SEEK_OPPONENT) {
									return action;
								}
							}
							else
								return AI_ACTION_SEEK_OPPONENT ;

							pSoldier->aiData.usActionData = FindFlankingSpot (pSoldier, sClosestDisturbance, action );

							if ( pSoldier->aiData.usActionData == NOWHERE || pSoldier->numFlanks >= MAX_FLANKS_RED )
							{
								pSoldier->aiData.usActionData = InternalGoAsFarAsPossibleTowards(pSoldier,sClosestDisturbance,AP_CROUCH, AI_ACTION_SEEK_OPPONENT,0);
								//pSoldier->numFlanks = 0;
								if ( PythSpacesAway( pSoldier->aiData.usActionData, sClosestDisturbance ) < 5 || LocationToLocationLineOfSightTest( pSoldier->aiData.usActionData, pSoldier->pathing.bLevel, sClosestDisturbance, pSoldier->pathing.bLevel, (UINT8) MaxDistanceVisible(), TRUE ) )
								{
									// reserve APs for a possible crouch plus a shot
									pSoldier->aiData.usActionData = InternalGoAsFarAsPossibleTowards(pSoldier, sClosestDisturbance, (INT8) (MinAPsToAttack( pSoldier, sClosestDisturbance, ADDTURNCOST) + AP_CROUCH), AI_ACTION_SEEK_OPPONENT, FLAG_CAUTIOUS );
									if ( pSoldier->aiData.usActionData != NOWHERE )
									{
										pSoldier->aiData.fAIFlags |= AI_CAUTIOUS;
										pSoldier->aiData.bNextAction = AI_ACTION_END_TURN;
										return(AI_ACTION_SEEK_OPPONENT);
									}
								}

								else
								{
									return(AI_ACTION_SEEK_OPPONENT);
								}
							}
							else
							{
								if ( action == AI_ACTION_FLANK_LEFT )
									pSoldier->flags.lastFlankLeft = TRUE;
								else
									pSoldier->flags.lastFlankLeft = FALSE;

								if ( pSoldier->lastFlankSpot != sClosestDisturbance)
									pSoldier->numFlanks=0;


								pSoldier->origDir = GetDirectionFromGridNo ( sClosestDisturbance, pSoldier);
								pSoldier->lastFlankSpot = sClosestDisturbance;
								pSoldier->numFlanks++;
								return(action);
							}
						}
						else
						{
							// let's be a bit cautious about going right up to a location without enough APs to shoot
							if ( PythSpacesAway( pSoldier->aiData.usActionData, sClosestDisturbance ) < 5 || LocationToLocationLineOfSightTest( pSoldier->aiData.usActionData, pSoldier->pathing.bLevel, sClosestDisturbance, pSoldier->pathing.bLevel, (UINT8) MaxDistanceVisible(), TRUE ) )
							{
								// reserve APs for a possible crouch plus a shot
								pSoldier->aiData.usActionData = InternalGoAsFarAsPossibleTowards(pSoldier, sClosestDisturbance, (INT8) (MinAPsToAttack( pSoldier, sClosestDisturbance, ADDTURNCOST) + AP_CROUCH), AI_ACTION_SEEK_OPPONENT, FLAG_CAUTIOUS );
								if ( pSoldier->aiData.usActionData != NOWHERE )
								{
									pSoldier->aiData.fAIFlags |= AI_CAUTIOUS;
									pSoldier->aiData.bNextAction = AI_ACTION_END_TURN;
									return(AI_ACTION_SEEK_OPPONENT);
								}
							}
							else
							{
								return(AI_ACTION_SEEK_OPPONENT);
							}
							break;
						}
					}
				}

				// mark SEEKING as impossible for next time through while loop
#ifdef DEBUGDECISIONS
				AINameMessage(pSoldier,"couldn't SEEK...",1000);
#endif
				bSeekPts = -99;
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: couldn't seek");
			}

			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: checking to watch");
			// if WATCHING is possible and at least as desirable as anything else
			if ((bWatchPts > -90) && (bWatchPts >= bSeekPts) && (bWatchPts >= bHelpPts) && (bWatchPts >= bHidePts ))
			{
				// take a look at our highest watch point... if it's still visible, turn to face it and then wait
				bHighestWatchLoc = GetHighestVisibleWatchedLoc( pSoldier->ubID );
				//sDistVisible =  DistanceVisible( pSoldier, DIRECTION_IRRELEVANT, DIRECTION_IRRELEVANT, gsWatchedLoc[ pSoldier->ubID ][ bHighestWatchLoc ], gbWatchedLocLevel[ pSoldier->ubID ][ bHighestWatchLoc ] );
				if ( bHighestWatchLoc != -1 )
				{
					// see if we need turn to face that location
					ubOpponentDir = atan8( CenterX(pSoldier->sGridNo),CenterY(pSoldier->sGridNo),CenterX( gsWatchedLoc[ pSoldier->ubID ][ bHighestWatchLoc ] ),CenterY( gsWatchedLoc[ pSoldier->ubID ][ bHighestWatchLoc ] ) );

					// if soldier is not already facing in that direction,
					// and the opponent is close enough that he could possibly be seen
					if ( pSoldier->bDirection != ubOpponentDir && pSoldier->InternalSoldierReadyWeapon( ubOpponentDir, gAnimControl[ pSoldier->usAnimState ].ubEndHeight ) )
					{
						// turn
						pSoldier->aiData.usActionData = ubOpponentDir;
						pSoldier->aiData.bNextAction = AI_ACTION_END_TURN;
						return(AI_ACTION_CHANGE_FACING);
					}
					else
					{
						// consider at least crouching
						if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_STAND && pSoldier->InternalSoldierReadyWeapon( ubOpponentDir, ANIM_CROUCH ) )
						{
							pSoldier->aiData.usActionData = ANIM_CROUCH;
							pSoldier->aiData.bNextAction = AI_ACTION_END_TURN;
							return(AI_ACTION_CHANGE_STANCE);
						}
						else if ( gAnimControl[ pSoldier->usAnimState ].ubHeight != ANIM_PRONE )
						{
							// maybe go prone
							if ( PreRandom( 2 ) == 0 && pSoldier->InternalSoldierReadyWeapon( ubOpponentDir, ANIM_PRONE ) )
							{
								pSoldier->aiData.usActionData = ANIM_PRONE;
								pSoldier->aiData.bNextAction = AI_ACTION_END_TURN;
								return( AI_ACTION_CHANGE_STANCE );
							}
							// end turn
							return( AI_ACTION_END_TURN );
						}
					}

				}

				bWatchPts = -99;
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: couldn't watch");

			}


			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: checking to help");
			// if HELPING is possible and at least as desirable as seeking or hiding
			if ((bHelpPts > -90) && (bHelpPts >= bSeekPts) && (bHelpPts >= bHidePts) && (bHelpPts >= bWatchPts ))
			{
#ifdef AI_TIMING_TESTS
				uiStartTime = GetJA2Clock();
#endif
				sClosestFriend = ClosestReachableFriendInTrouble(pSoldier, &fClimb );
#ifdef AI_TIMING_TESTS
				uiEndTime = GetJA2Clock();

				guiRedHelpTimeTotal += (uiEndTime - uiStartTime);
				guiRedHelpCounter++;
#endif

				if (sClosestFriend != NOWHERE)
				{
					//////////////////////////////////////////////////////////////////////
					// GO DIRECTLY TOWARDS CLOSEST FRIEND UNDER FIRE OR WHO LAST RADIOED
					//////////////////////////////////////////////////////////////////////
					pSoldier->aiData.usActionData = InternalGoAsFarAsPossibleTowards(pSoldier,sClosestFriend,AP_CROUCH, AI_ACTION_SEEK_OPPONENT,0);

					if (pSoldier->aiData.usActionData != NOWHERE)
					{
#ifdef DEBUGDECISIONS
						std::string tempstr = String("%s - SEEKING FRIEND at %d, MOVING to %d",
							pSoldier->name,sClosestFriend,pSoldier->aiData.usActionData);
						DebugAI(tempstr);
#endif

						if ( fClimb )//&& pSoldier->aiData.usActionData == sClosestFriend)
						{
							// need to climb AND have enough APs to get there this turn
							BOOLEAN fUp = TRUE;
							if (pSoldier->pathing.bLevel > 0 )
								fUp = FALSE;

							if (!fUp)
								DebugMsg ( TOPIC_JA2AI , DBG_LEVEL_3 , String("Soldier %d is climbing down",pSoldier->ubID) );

							if ( CanClimbFromHere ( pSoldier, fUp ) )
							{
								if (IsActionAffordable(pSoldier) && pSoldier->bActionPoints >= ( AP_CLIMBROOF + MinAPsToAttack( pSoldier, sClosestFriend, ADDTURNCOST)))
								{
									return( AI_ACTION_CLIMB_ROOF );
								}
							}
							else
							{
								pSoldier->aiData.usActionData = FindClosestClimbPoint(pSoldier->sGridNo , sClosestFriend , fUp );
								if ( pSoldier->aiData.usActionData != NOWHERE )
								{
									return( AI_ACTION_MOVE_TO_CLIMB  );
								}
							}
						}
						//if (fClimb && pSoldier->aiData.usActionData == sClosestFriend)
						//{
						// return( AI_ACTION_CLIMB_ROOF );
						//}
						return(AI_ACTION_SEEK_FRIEND);
					}
				}

				// mark SEEKING as impossible for next time through while loop
#ifdef DEBUGDECISIONS
				AINameMessage(pSoldier,"couldn't HELP...",1000);
#endif

				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: couldn't help");
				bHelpPts = -99;
			}


			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: checking to hide");
			// if HIDING is possible and at least as desirable as seeking or helping
			if ((bHidePts > -90) && (bHidePts >= bSeekPts) && (bHidePts >= bHelpPts) && (bHidePts >= bWatchPts ))
			{
				sClosestOpponent = ClosestKnownOpponent( pSoldier, NULL, NULL );
				// if an opponent is known (not necessarily reachable or conscious)
				if (!SkipCoverCheck && sClosestOpponent != NOWHERE )
				{
					//////////////////////////////////////////////////////////////////////
					// TAKE BEST NEARBY COVER FROM ALL KNOWN OPPONENTS
					//////////////////////////////////////////////////////////////////////
#ifdef AI_TIMING_TESTS
					uiStartTime = GetJA2Clock();
#endif

					pSoldier->aiData.usActionData = FindBestNearbyCover(pSoldier);
#ifdef AI_TIMING_TESTS
					uiEndTime = GetJA2Clock();

					guiRedHideTimeTotal += (uiEndTime - uiStartTime);
					guiRedHideCounter++;
#endif

					// let's be a bit cautious about going right up to a location without enough APs to shoot
					if ( pSoldier->aiData.usActionData != NOWHERE )
					{
						sClosestDisturbance = ClosestReachableDisturbance(pSoldier, flags.ubUnconsciousOK, &fClimb);
						if ( sClosestDisturbance != NOWHERE && ( SpacesAway( pSoldier->aiData.usActionData, sClosestDisturbance ) < 5 || SpacesAway( pSoldier->aiData.usActionData, sClosestDisturbance ) + 5 < SpacesAway( pSoldier->sGridNo, sClosestDisturbance ) ) )
						{
							// either moving significantly closer or into very close range
							// ensure will we have enough APs for a possible crouch plus a shot
							if ( InternalGoAsFarAsPossibleTowards( pSoldier, pSoldier->aiData.usActionData, (INT8) (MinAPsToAttack( pSoldier, sClosestOpponent, ADDTURNCOST) + AP_CROUCH), AI_ACTION_TAKE_COVER, 0 ) == pSoldier->aiData.usActionData )
							{
								return(AI_ACTION_TAKE_COVER);
							}
						}
						else
						{
							return(AI_ACTION_TAKE_COVER);
						}
					}

				}

				// mark HIDING as impossible for next time through while loop
#ifdef DEBUGDECISIONS
				AINameMessage(pSoldier,"couldn't HIDE...",1000);
#endif

				bHidePts = -99;
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: couldn't hide");
			}
		}
	}
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: nothing to do!");
	////////////////////////////////////////////////////////////////////////////
	// NOTHING USEFUL POSSIBLE!  IF NPC IS CURRENTLY UNDER FIRE, TRY TO RUN AWAY
	////////////////////////////////////////////////////////////////////////////

	// if we're currently under fire (presumably, attacker is hidden)
	if (pSoldier->aiData.bUnderFire || flags.fCivilian)
	{
		// only try to run if we've actually been hit recently & noticably so
		// otherwise, presumably our current cover is pretty good & sufficient
		if (pSoldier->aiData.bShock > 0 || flags.fCivilian)
		{
			// look for best place to RUN AWAY to (farthest from the closest threat)
			pSoldier->aiData.usActionData = FindSpotMaxDistFromOpponents(pSoldier);

			if (pSoldier->aiData.usActionData != NOWHERE)
			{
#ifdef DEBUGDECISIONS
				std::string tempstr = String("%s RUNNING AWAY to grid %d",pSoldier->name,pSoldier->aiData.usActionData);
				DebugAI(tempstr);
#endif

				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: run away!");
				return(AI_ACTION_RUN_AWAY);
			}
		}


		////////////////////////////////////////////////////////////////////////////
		// UNDER FIRE, DON'T WANNA/CAN'T RUN AWAY, SO CROUCH
		////////////////////////////////////////////////////////////////////////////


		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: crouch or go prone");
		// if not in water and not already crouched
		if (!flags.fCivilian )
		{
			if ( gAnimControl[ pSoldier->usAnimState ].ubHeight == ANIM_STAND && IsValidStance( pSoldier, ANIM_CROUCH ) )
			{
				if (!gfTurnBasedAI || GetAPsToChangeStance( pSoldier, ANIM_CROUCH ) <= pSoldier->bActionPoints)
				{

#ifdef DEBUGDECISIONS
					std::string tempstr = String("%s CROUCHES (STATUS RED)",pSoldier->name);
					DebugAI(tempstr);
#endif

					pSoldier->aiData.usActionData = ANIM_CROUCH;
					return(AI_ACTION_CHANGE_STANCE);
				}
			}
			else if ( gAnimControl[ pSoldier->usAnimState ].ubHeight != ANIM_PRONE )
			{
				// maybe go prone
				if ( PreRandom( 2 ) == 0 && IsValidStance( pSoldier, ANIM_PRONE ) )
				{
					pSoldier->aiData.usActionData = ANIM_PRONE;
					return( AI_ACTION_CHANGE_STANCE );
				}

			}
		}
	}
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 RedAlert_TryToLookAround(SOLDIERTYPE* pSoldier, RedAlertFlags& flags)
{
	PERFORMANCE_MARKER
	UINT8 ubOpponentDir;
	INT16 sDistVisible;
	INT16 iChance;

	// determine the location of the known closest opponent
	// (don't care if he's conscious, don't care if he's reachable at all)
	INT16 sClosestOpponent = ClosestKnownOpponent(pSoldier, NULL, NULL);

	if (sClosestOpponent != NOWHERE)
	{
		// determine direction from this soldier to the closest opponent
		ubOpponentDir = atan8(CenterX(pSoldier->sGridNo),CenterY(pSoldier->sGridNo),CenterX(sClosestOpponent),CenterY(sClosestOpponent));

		// if soldier is not already facing in that direction,
		// and the opponent is close enough that he could possibly be seen
		// note, have to change this to use the level returned from ClosestKnownOpponent
		sDistVisible = DistanceVisible( pSoldier, DIRECTION_IRRELEVANT, DIRECTION_IRRELEVANT, sClosestOpponent, 0, pSoldier );

		if ((pSoldier->bDirection != ubOpponentDir) && (PythSpacesAway(pSoldier->sGridNo,sClosestOpponent) <= sDistVisible))
		{
			// set base chance according to orders
			if ((pSoldier->aiData.bOrders == STATIONARY) || (pSoldier->aiData.bOrders == ONGUARD))
				iChance = 50;
			else           // all other orders
				iChance = 25;

			if (pSoldier->aiData.bAttitude == DEFENSIVE)
				iChance += 25;

			if ( TANK( pSoldier ) )
			{
				iChance += 50;
			}

			if ((INT16)PreRandom(100) < iChance && pSoldier->InternalSoldierReadyWeapon( ubOpponentDir, gAnimControl[ pSoldier->usAnimState ].ubEndHeight ) )
			{
				pSoldier->aiData.usActionData = ubOpponentDir;

#ifdef DEBUGDECISIONS
				std::string tempstr = String("%s - TURNS TOWARDS CLOSEST ENEMY to face direction %d",pSoldier->name,pSoldier->aiData.usActionData);
				DebugAI(tempstr);
#endif
				if ( pSoldier->aiData.bOrders == SNIPER )
				{
					if (!gfTurnBasedAI || GetAPsToReadyWeapon( pSoldier, READY_RIFLE_CROUCH ) <= pSoldier->bActionPoints)
					{
						pSoldier->aiData.bNextAction = AI_ACTION_RAISE_GUN;
					}
				}

				return(AI_ACTION_CHANGE_FACING);
			}
		}
		else if ( pSoldier->bDirection == ubOpponentDir && pSoldier->aiData.bOrders == SNIPER )
		{
			if (!gfTurnBasedAI || GetAPsToReadyWeapon( pSoldier, READY_RIFLE_CROUCH ) <= pSoldier->bActionPoints)
			{
				return AI_ACTION_RAISE_GUN;
			}
		}
	}
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 RedAlert_TryTankAI(SOLDIERTYPE *pSoldier, RedAlertFlags& flags)
{
	PERFORMANCE_MARKER
	// try turning in a random direction as we still can't see anyone.
	if (!gfTurnBasedAI || GetAPsToLook( pSoldier ) <= pSoldier->bActionPoints)
	{
		UINT8 ubOpponentDir;
		INT16 sClosestDisturbance = MostImportantNoiseHeard( pSoldier, NULL, NULL, NULL );
		if ( sClosestDisturbance != NOWHERE )
		{
			ubOpponentDir = atan8( CenterX( pSoldier->sGridNo ), CenterY( pSoldier->sGridNo ), CenterX( sClosestDisturbance ), CenterY( sClosestDisturbance ) );
			if ( pSoldier->bDirection == ubOpponentDir )
			{
				ubOpponentDir = (UINT8) PreRandom( NUM_WORLD_DIRECTIONS );
			}
		}
		else
		{
			ubOpponentDir = (UINT8) PreRandom( NUM_WORLD_DIRECTIONS );
		}

		if ( (pSoldier->bDirection != ubOpponentDir) )
		{
			if ( (pSoldier->bActionPoints == pSoldier->bInitialActionPoints || (INT16)PreRandom(100) < 60) && pSoldier->InternalSoldierReadyWeapon( ubOpponentDir, gAnimControl[ pSoldier->usAnimState ].ubEndHeight ) )
			{
				pSoldier->aiData.usActionData = ubOpponentDir;

#ifdef DEBUGDECISIONS
				std::string tempstr = String("%s - TURNS TOWARDS CLOSEST ENEMY to face direction %d",pSoldier->name,pSoldier->aiData.usActionData);
				DebugAI(tempstr);
#endif

				// limit turning a bit... if the last thing we did was also a turn, add a 60% chance of this being our last turn
				if ( pSoldier->aiData.bLastAction == AI_ACTION_CHANGE_FACING && PreRandom( 100 ) < 60 )
				{
					if ( gfTurnBasedAI )
					{
						pSoldier->aiData.bNextAction = AI_ACTION_END_TURN;
					}
					else
					{
						pSoldier->aiData.bNextAction = AI_ACTION_WAIT;
						pSoldier->aiData.usNextActionData = (UINT16) REALTIME_AI_DELAY;
					}
				}

				return(AI_ACTION_CHANGE_FACING);
			}
		}
	}

	// that's it for tanks
	return( AI_ACTION_NONE );
}

INT8 DecideActionRed(SOLDIERTYPE *pSoldier, UINT8 ubUnconsciousOK)
{
	PERFORMANCE_MARKER
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("DecideActionRed: soldier orders = %d",pSoldier->aiData.bOrders));
	// if we have absolutely no action points, we can't do a thing under RED!
	if (!pSoldier->bActionPoints)
	{
		pSoldier->aiData.usActionData = NOWHERE;
		return(AI_ACTION_NONE);
	}

	RedAlertFlags flags;
	flags.ubUnconsciousOK = ubUnconsciousOK;

	INT8 bActionReturned;
	INT16 sClosestOpponent;
	INT16 sClosestDisturbance;
	UINT8 ubOpponentDir;
#ifdef AI_TIMING_TEST
	UINT32	uiStartTime, uiEndTime;
#endif
#ifdef DEBUGDECISIONS
	std::string tempstr;
#endif
	BOOLEAN fClimb;
	flags.fCivilian = (PTR_CIVILIAN && (pSoldier->ubCivilianGroup == NON_CIV_GROUP ||
		(pSoldier->aiData.bNeutral && gTacticalStatus.fCivGroupHostile[pSoldier->ubCivilianGroup] == CIV_GROUP_NEUTRAL) ||
		(pSoldier->ubBodyType >= FATCIV && pSoldier->ubBodyType <= CRIPPLECIV) ) );


	// can this guy move to any of the neighbouring squares ? (sets TRUE/FALSE)
	flags.ubCanMove = (pSoldier->bActionPoints >= MinPtsToMove(pSoldier));

	// if we're an alerted enemy, and there are panic bombs or a trigger around
	if ( (!PTR_CIVILIAN || pSoldier->ubProfile == WARDEN) && ( ( gTacticalStatus.Team[pSoldier->bTeam].bAwareOfOpposition || (pSoldier->ubID == gTacticalStatus.ubTheChosenOne) || (pSoldier->ubProfile == WARDEN) ) &&
		(gTacticalStatus.fPanicFlags & (PANIC_BOMBS_HERE | PANIC_TRIGGERS_HERE ) ) ) )
	{
		if ( pSoldier->ubProfile == WARDEN && gTacticalStatus.ubTheChosenOne == NOBODY )
		{
			PossiblyMakeThisEnemyChosenOne( pSoldier );
		}

		// do some special panic AI decision making
		bActionReturned = PanicAI(pSoldier,flags.ubCanMove);

		// if we decided on an action while in there, we're done
		if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
			return(bActionReturned);
	}

	if ( pSoldier->ubProfile != NO_PROFILE )
	{
		if ( (pSoldier->ubProfile == QUEEN || pSoldier->ubProfile == JOE) && flags.ubCanMove )
		{
			if ( gWorldSectorX == 3 && gWorldSectorY == MAP_ROW_P && gbWorldSectorZ == 0 && !gfUseAlternateQueenPosition )
			{
				bActionReturned = HeadForTheStairCase( pSoldier );
				if ( bActionReturned != AI_ACTION_NONE )
				{
					return( bActionReturned );
				}
			}
		}
	}


	// determine if we happen to be in water (in which case we're in BIG trouble!)
	flags.bInWater = Water( pSoldier->sGridNo );
	flags.bInDeepWater = Water( pSoldier->sGridNo );

	// check if standing in tear gas without a gas mask on
	flags.bInGas = InGasOrSmoke( pSoldier, pSoldier->sGridNo );

	////////////////////////////////////////////////////////////////////////////
	// WHEN LEFT IN GAS, WEAR GAS MASK IF AVAILABLE AND NOT WORN
	////////////////////////////////////////////////////////////////////////////

	if ( !flags.bInGas && (gWorldSectorX == TIXA_SECTOR_X && gWorldSectorY == TIXA_SECTOR_Y) )
	{
		// only chance if we happen to be caught with our gas mask off
		if ( PreRandom( 10 ) == 0 && WearGasMaskIfAvailable( pSoldier ) )
		{
			// reevaluate
			flags.bInGas = InGasOrSmoke( pSoldier, pSoldier->sGridNo );
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// WHEN IN GAS, GO TO NEAREST REACHABLE SPOT OF UNGASSED LAND
	////////////////////////////////////////////////////////////////////////////

	if (flags.bInGas && flags.ubCanMove)
	{
		pSoldier->aiData.usActionData = FindNearestUngassedLand(pSoldier);

		if (pSoldier->aiData.usActionData != NOWHERE)
		{
#ifdef DEBUGDECISIONS
			tempstr = String("%s - SEEKING NEAREST UNGASSED LAND at grid %d",pSoldier->name,pSoldier->aiData.usActionData);
			DebugAI(tempstr);
#endif

			return(AI_ACTION_LEAVE_WATER_GAS);
		}
	}


	if (flags.fCivilian && !( pSoldier->ubBodyType == COW || pSoldier->ubBodyType == CRIPPLECIV ) )
	{
		bActionReturned = RedAlert_TryToCower(pSoldier, flags);
		if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
			return bActionReturned;
	}

	////////////////////////////////////////////////////////////////////////
	// IF POSSIBLE, FIRE LONG RANGE WEAPONS AT TARGETS REPORTED BY RADIO
	////////////////////////////////////////////////////////////////////////

	// can't do this in realtime, because the player could be shooting a gun or whatever at the same time!
	if (gfTurnBasedAI && !flags.fCivilian && !flags.bInWater && !flags.bInGas && !(pSoldier->flags.uiStatusFlags & SOLDIER_BOXER) && (CanNPCAttack(pSoldier) == TRUE))
	{
		bActionReturned = RedAlert_TryLongRangeWeapons(pSoldier, flags);
		if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
			return bActionReturned;
	}

	/*
	// CALL IN AIR STRIKE & RADIO RED ALERT
	if ( !flags.fCivilian && pSoldier->bTeam != MILITIA_TEAM && gGameOptions.fAirStrikes && airstrikeavailable && (pSoldier->bActionPoints >= AP_RADIO) && !WillAirRaidBeStopped(pSoldier->sSectorX,pSoldier->sSectorY))
	{

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: checking to call in an air strike");

	iChance = Random(50);
	// if I ain't swimming (deep water)
	if ( !flags.bInDeepWater )
	{
	// modify base chance according to orders
	switch (pSoldier->aiData.bOrders)
	{
	case STATIONARY:       iChance +=  20;  break;
	case ONGUARD:          iChance +=  15;  break;
	case ONCALL:           iChance +=  10;  break;
	case CLOSEPATROL:                       break;
	case RNDPTPATROL:
	case POINTPATROL:      iChance +=  -5;  break;
	case FARPATROL:        iChance += -10;  break;
	case SEEKENEMY:        iChance += -20;  break;
	}

	// modify base chance according to attitude
	switch (pSoldier->aiData.bAttitude)
	{
	case DEFENSIVE:        iChance +=  20;  break;
	case BRAVESOLO:        iChance += -10;  break;
	case BRAVEAID:                          break;
	case CUNNINGSOLO:      iChance +=  -5;  break;
	case CUNNINGAID:                        break;
	case AGGRESSIVE:       iChance += -20;  break;
	case ATTACKSLAYONLY:		iChance = 0;
	}

	// modify base chance according to morale
	switch (pSoldier->aiData.bAIMorale)
	{
	case MORALE_HOPELESS:  iChance *= 3;    break;
	case MORALE_WORRIED:   iChance *= 2;    break;
	case MORALE_NORMAL:                     break;
	case MORALE_CONFIDENT: iChance /= 2;    break;
	case MORALE_FEARLESS:  iChance /= 3;    break;
	}

	if ((INT16) Random(100) < iChance)
	{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: decided to call in an air strike!");
	SayQuoteFromAnyBodyInSector( QUOTE_WEARY_SLASH_SUSPUCIOUS );
	EnemyCallInAirStrike ( pSoldier->sSectorX, pSoldier->sSectorY );
	airstrikeavailable = FALSE;

	return(AI_ACTION_RED_ALERT);
	}
	}
	}
	*/

	////////////////////////////////////////////////////////////////////////////
	// WHEN IN THE LIGHT, GET OUT OF THERE!
	////////////////////////////////////////////////////////////////////////////
	if ( flags.ubCanMove && InLightAtNight( pSoldier->sGridNo, pSoldier->pathing.bLevel ) && pSoldier->aiData.bOrders != STATIONARY )
	{
		pSoldier->aiData.usActionData = FindNearbyDarkerSpot( pSoldier );
		if ( pSoldier->aiData.usActionData != NOWHERE )
		{
			// move as if leaving water or gas
			return( AI_ACTION_LEAVE_WATER_GAS );
		}
	}

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: crouch and rest if running out of breath");
	////////////////////////////////////////////////////////////////////////
	// CROUCH & REST IF RUNNING OUT OF BREATH
	////////////////////////////////////////////////////////////////////////

	// if our breath is running a bit low, and we're not in water or under fire
	if ((pSoldier->bBreath < 25) && !flags.bInWater && !pSoldier->aiData.bUnderFire)
	{
		return RedAlert_TryToRest(pSoldier, flags);
	}



	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: calculate morale");
	// calculate our morale
	pSoldier->aiData.bAIMorale = CalcMorale(pSoldier);

	// if a guy is feeling REALLY discouraged, he may continue to run like hell
	if ((pSoldier->aiData.bAIMorale == MORALE_HOPELESS) && flags.ubCanMove)
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: run away");
		////////////////////////////////////////////////////////////////////////
		// RUN AWAY TO SPOT FARTHEST FROM KNOWN THREATS (ONLY IF MORALE HOPELESS)
		////////////////////////////////////////////////////////////////////////

		// look for best place to RUN AWAY to (farthest from the closest threat)
		pSoldier->aiData.usActionData = FindSpotMaxDistFromOpponents(pSoldier);

		if (pSoldier->aiData.usActionData != NOWHERE)
		{
#ifdef DEBUGDECISIONS
			tempstr = String("%s RUNNING AWAY to grid %d",pSoldier->name,pSoldier->aiData.usActionData);
			DebugAI(tempstr);
#endif

			return(AI_ACTION_RUN_AWAY);
		}
	}


	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: radio red alert?");
	////////////////////////////////////////////////////////////////////////////
	// RADIO RED ALERT: determine %chance to call others and report contact
	////////////////////////////////////////////////////////////////////////////

	// if we're a computer merc, and we have the action points remaining to RADIO
	// (we never want NPCs to choose to radio if they would have to wait a turn)
	if ( !flags.fCivilian && (pSoldier->bActionPoints >= AP_RADIO) && (gTacticalStatus.Team[pSoldier->bTeam].bMenInSector > 1) )
	{
		bActionReturned = RedAlert_TryToRadioRedAlert(pSoldier, flags);
		if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
			return bActionReturned;
	}

	if ( !TANK( pSoldier ) )
	{
		bActionReturned = RedAlert_TryMainAI(pSoldier, flags);
		if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
			return bActionReturned;
	}


	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"decideactionred: look around towards opponent");
	////////////////////////////////////////////////////////////////////////////
	// LOOK AROUND TOWARD CLOSEST KNOWN OPPONENT, IF KNOWN
	////////////////////////////////////////////////////////////////////////////

	if (!gfTurnBasedAI || GetAPsToLook( pSoldier ) <= pSoldier->bActionPoints)
	{
		bActionReturned = RedAlert_TryToLookAround(pSoldier, flags);
		if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
			return bActionReturned;
	}

	if ( TANK( pSoldier ) )
	{
		return RedAlert_TryTankAI(pSoldier, flags);
	}

	////////////////////////////////////////////////////////////////////////////
	// LEAVE THE SECTOR
	////////////////////////////////////////////////////////////////////////////

	// NOT IMPLEMENTED


	////////////////////////////////////////////////////////////////////////////
	// PICKUP A NEARBY ITEM THAT'S USEFUL
	////////////////////////////////////////////////////////////////////////////

	if ( flags.ubCanMove && !pSoldier->aiData.bNeutral && (gfTurnBasedAI || pSoldier->bTeam == ENEMY_TEAM ) )
	{

		pSoldier->aiData.bAction = SearchForItems( pSoldier, SEARCH_GENERAL_ITEMS, pSoldier->inv[HANDPOS].usItem );

		if (pSoldier->aiData.bAction != AI_ACTION_NONE)
		{
			return( pSoldier->aiData.bAction );
		}
	}



	////////////////////////////////////////////////////////////////////////////
	// SEEK CLOSEST FRIENDLY MEDIC
	////////////////////////////////////////////////////////////////////////////

	// NOT IMPLEMENTED


	////////////////////////////////////////////////////////////////////////////
	// GIVE FIRST AID TO A NEARBY INJURED/DYING FRIEND
	////////////////////////////////////////////////////////////////////////////
	// - must be BRAVEAID or CUNNINGAID (medic) ?

	// NOT IMPLEMENTED

	/* JULY 29, 1996 - Decided that this was a bad idea, after watching a civilian
	start a random patrol while 2 steps away from a hidden armed opponent...*/

	////////////////////////////////////////////////////////////////////////////
	// SWITCH TO GREEN: soldier does ordinary regular patrol, seeks friends
	////////////////////////////////////////////////////////////////////////////

	// if not in combat or under fire, and we COULD have moved, just chose not to
	if ( (pSoldier->aiData.bAlertStatus != STATUS_BLACK) && !pSoldier->aiData.bUnderFire && flags.ubCanMove && (!gfTurnBasedAI || pSoldier->bActionPoints >= pSoldier->bInitialActionPoints) && ( ClosestReachableDisturbance(pSoldier, TRUE, &fClimb) == NOWHERE) )
	{
		// addition:  if soldier is bleeding then reduce bleeding and do nothing
		if ( pSoldier->bBleeding > MIN_BLEEDING_THRESHOLD )
		{
			// reduce bleeding by 1 point per AP (in RT, APs will get recalculated so it's okay)
			pSoldier->bBleeding = __max( 0, pSoldier->bBleeding - pSoldier->bActionPoints );
			return( AI_ACTION_NONE ); // will end-turn/wait depending on whether we're in TB or realtime
		}
#ifdef RECORDNET
		fprintf(NetDebugFile,"\tDecideActionRed: guynum %d switching to GREEN AI...\n",pSoldier->ubID);
#endif

#ifdef DEBUGDECISIONS
		AINameMessage(pSoldier,"- chose to SKIP all RED actions, BYPASSES to GREEN!",1000);
#endif

		// Skip RED until new situation/next turn, 30% extra chance to do GREEN actions
		pSoldier->aiData.bBypassToGreen = 30;
		return(DecideActionGreen(pSoldier));
	}


	////////////////////////////////////////////////////////////////////////////
	// CROUCH IF NOT CROUCHING ALREADY
	////////////////////////////////////////////////////////////////////////////

	// if not in water and not already crouched, try to crouch down first
	if (!flags.fCivilian && !flags.bInWater && (gAnimControl[ pSoldier->usAnimState ].ubHeight == ANIM_STAND) && IsValidStance( pSoldier, ANIM_CROUCH ) )
	{
		sClosestOpponent = ClosestKnownOpponent(pSoldier, NULL, NULL);

		if ( (sClosestOpponent != NOWHERE && PythSpacesAway( pSoldier->sGridNo, sClosestOpponent ) < (MaxDistanceVisible() * 3) / 2 ) || PreRandom( 4 ) == 0 )
		{
			if (!gfTurnBasedAI || GetAPsToChangeStance( pSoldier, ANIM_CROUCH ) <= pSoldier->bActionPoints)
			{

#ifdef DEBUGDECISIONS
				tempstr = String("%s CROUCHES (STATUS RED)",pSoldier->name );
				DebugAI(tempstr);
#endif

				pSoldier->aiData.usActionData = ANIM_CROUCH;
				return(AI_ACTION_CHANGE_STANCE);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// IF UNDER FIRE, FACE THE MOST IMPORTANT NOISE WE KNOW AND GO PRONE
	////////////////////////////////////////////////////////////////////////////

	if ( !flags.fCivilian && pSoldier->aiData.bUnderFire && pSoldier->bActionPoints >= (pSoldier->bInitialActionPoints - GetAPsToLook( pSoldier ) ) && IsValidStance( pSoldier, ANIM_PRONE ) )
	{
		sClosestDisturbance = MostImportantNoiseHeard( pSoldier, NULL, NULL, NULL );
		if ( sClosestDisturbance != NOWHERE )
		{
			ubOpponentDir = atan8( CenterX( pSoldier->sGridNo ), CenterY( pSoldier->sGridNo ), CenterX( sClosestDisturbance ), CenterY( sClosestDisturbance ) );
			if ( pSoldier->bDirection != ubOpponentDir )
			{
				if ( !gfTurnBasedAI || GetAPsToLook( pSoldier ) <= pSoldier->bActionPoints )
				{
					pSoldier->aiData.usActionData = ubOpponentDir;
					return( AI_ACTION_CHANGE_FACING );
				}
			}
			else if ( (!gfTurnBasedAI || GetAPsToChangeStance( pSoldier, ANIM_PRONE ) <= pSoldier->bActionPoints ) && pSoldier->InternalSoldierReadyWeapon( ubOpponentDir, ANIM_PRONE ) )
			{
				// go prone, end turn
				pSoldier->aiData.bNextAction = AI_ACTION_END_TURN;
				pSoldier->aiData.usActionData = ANIM_PRONE;
				return( AI_ACTION_CHANGE_STANCE );
			}
		}
	}


	////////////////////////////////////////////////////////////////////////////
	// If sniper and nothing else to do then raise gun, and if that doesn't find somebody then goto yellow
	////////////////////////////////////////////////////////////////////////////
	if ( pSoldier->aiData.bOrders == SNIPER )
	{
		if ( pSoldier->sniper == 0 )
		{
			DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DecideActionRed: sniper raising gun..."));
			if (!gfTurnBasedAI || GetAPsToReadyWeapon( pSoldier, READY_RIFLE_CROUCH ) <= pSoldier->bActionPoints)
			{
				pSoldier->sniper = 1;
				return AI_ACTION_RAISE_GUN;
			}
		}
		else
		{
			pSoldier->sniper = 0;
			return(DecideActionYellow(pSoldier));
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// DO NOTHING: Not enough points left to move, so save them for next turn
	////////////////////////////////////////////////////////////////////////////
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DecideActionRed: do nothing at all..."));
#ifdef DEBUGDECISIONS
	AINameMessage(pSoldier,"- DOES NOTHING (RED)",1000);
#endif

	pSoldier->aiData.usActionData = NOWHERE;
	return(AI_ACTION_NONE);
}

struct BlackAlertFlags
{
	UINT8	ubMinAPCost,ubCanMove;
	INT8		bInWater,bInDeepWater,bInGas;
	INT8		bDirection;
	UINT8	ubBestAttackAction;
	INT8		bCanAttack,bActionReturned;
	INT8		bWeaponIn;
	BOOLEAN	fTryPunching;
	BOOLEAN fCivilian;
	UINT8	ubBestStance, ubStanceCost;
	BOOLEAN fChangeStanceFirst; // before firing
	BOOLEAN fClimb, fAllowCoverCheck;
	ATTACKTYPE BestShot,BestThrow,BestStab,BestAttack;
};

INT8 BlackAlert_SetSomeFlags(SOLDIERTYPE* pSoldier, BlackAlertFlags& flags)
{
	PERFORMANCE_MARKER
	if ( pSoldier->flags.uiStatusFlags & SOLDIER_BOXER )
	{
		if ( gTacticalStatus.bBoxingState == PRE_BOXING )
		{
			return( DecideActionBoxerEnteringRing( pSoldier ) );
		}
		else if ( gTacticalStatus.bBoxingState == BOXING )
		{

			flags.bInWater = FALSE;
			flags.bInDeepWater = FALSE;
			flags.bInGas = FALSE;

			// calculate our morale
			pSoldier->aiData.bAIMorale = CalcMorale(pSoldier);
			// and continue on...
		}
		else //????
		{
			return( AI_ACTION_NONE );
		}
	}
	else
	{

		// determine if we happen to be in water (in which case we're in BIG trouble!)
		flags.bInWater = Water( pSoldier->sGridNo );
		flags.bInDeepWater = WaterTooDeepForAttacks( pSoldier->sGridNo );

		// check if standing in tear gas without a gas mask on
		flags.bInGas = InGasOrSmoke( pSoldier, pSoldier->sGridNo );

		// calculate our morale
		pSoldier->aiData.bAIMorale = CalcMorale(pSoldier);

		////////////////////////////////////////////////////////////////////////////
		// WHEN LEFT IN GAS, WEAR GAS MASK IF AVAILABLE AND NOT WORN
		////////////////////////////////////////////////////////////////////////////

		if ( !flags.bInGas && (gWorldSectorX == TIXA_SECTOR_X && gWorldSectorY == TIXA_SECTOR_Y) )
		{
			// only chance if we happen to be caught with our gas mask off
			if ( PreRandom( 10 ) == 0 && WearGasMaskIfAvailable( pSoldier ) )
			{
				flags.bInGas = FALSE;
			}
		}

		////////////////////////////////////////////////////////////////////////////
		// IF GASSED, OR REALLY TIRED (ON THE VERGE OF COLLAPSING), TRY TO RUN AWAY
		////////////////////////////////////////////////////////////////////////////

		// if we're desperately short on breath (it's OK if we're in water, though!)
		if (flags.bInGas || (pSoldier->bBreath < 5))
		{
			// if soldier has enough APs left to move at least 1 square's worth
			if (flags.ubCanMove)
			{
				// look for best place to RUN AWAY to (farthest from the closest threat)
				pSoldier->aiData.usActionData = FindSpotMaxDistFromOpponents(pSoldier);

				if (pSoldier->aiData.usActionData != NOWHERE)
				{
#ifdef DEBUGDECISIONS
					std::string tempstr = String("%s - GASSED or LOW ON BREATH (%d), RUNNING AWAY to grid %d",pSoldier->name,pSoldier->bBreath,pSoldier->aiData.usActionData);
					DebugAI(tempstr);
#endif

					return(AI_ACTION_RUN_AWAY);
				}
			}

			// REALLY tired, can't get away, force soldier's morale to hopeless state
			if ( gGameOptions.ubDifficultyLevel == DIF_LEVEL_INSANE )
			{
				pSoldier->bBreath = pSoldier->bBreathMax;  //Madd: backed into a corner, so go crazy like a wild animal...
				pSoldier->aiData.bAIMorale = MORALE_FEARLESS;
			}
			else
				pSoldier->aiData.bAIMorale = MORALE_HOPELESS;
		}
	}
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 BlackAlert_TryToLeaveGas(SOLDIERTYPE* pSoldier, BlackAlertFlags& flags)
{
	PERFORMANCE_MARKER
	pSoldier->aiData.usActionData = FindNearestUngassedLand(pSoldier);

	if (pSoldier->aiData.usActionData != NOWHERE)
	{
#ifdef DEBUGDECISIONS
		std::string tempstr = String("%s - SEEKING NEAREST UNGASSED LAND at grid %d",pSoldier->name,pSoldier->aiData.usActionData);
		DebugAI(tempstr);
#endif

		return(AI_ACTION_LEAVE_WATER_GAS);
	}

	// couldn't find ANY land within 25 tiles(!), this should never happen...

	// look for best place to RUN AWAY to (farthest from the closest threat)
	pSoldier->aiData.usActionData = FindSpotMaxDistFromOpponents(pSoldier);

	if (pSoldier->aiData.usActionData != NOWHERE)
	{
#ifdef DEBUGDECISIONS
		std::string tempstr = String("%s - NO LAND NEAR, RUNNING AWAY to grid %d",pSoldier->name,pSoldier->aiData.usActionData);
		DebugAI(tempstr);
#endif

		return(AI_ACTION_RUN_AWAY);
	}

	// GIVE UP ON LIFE!  MERCS MUST HAVE JUST CORNERED A HELPLESS ENEMY IN A
	// GAS FILLED ROOM (OR IN WATER MORE THAN 25 TILES FROM NEAREST LAND...)
	if ( flags.bInGas && gGameOptions.ubDifficultyLevel == DIF_LEVEL_INSANE )
	{
		pSoldier->bBreath = pSoldier->bBreathMax;
		pSoldier->aiData.bAIMorale = MORALE_FEARLESS;  // Can't move, can't get away, go nuts instead...
	}
	else
		pSoldier->aiData.bAIMorale = MORALE_HOPELESS;

	return AI_ACTION_NOT_AN_ACTION;
}

INT8 BlackAlert_EvaluateCanAttack(SOLDIERTYPE* pSoldier, BlackAlertFlags& flags)
{
	PERFORMANCE_MARKER
	////////////////////////////////////////////////////////////////////////////
	// SOLDIER CAN ATTACK IF NOT IN WATER/GAS AND NOT DOING SOMETHING TOO FUNKY
	////////////////////////////////////////////////////////////////////////////


	// NPCs in water/tear gas without masks are not permitted to shoot/stab/throw
	if ((pSoldier->bActionPoints < 2) || flags.bInDeepWater || flags.bInGas || pSoldier->aiData.bRTPCombat == RTP_COMBAT_REFRAIN)
	{
		flags.bCanAttack = FALSE;
	}
	else if (pSoldier->flags.uiStatusFlags & SOLDIER_BOXER)
	{
		flags.bCanAttack = TRUE;
		flags.fTryPunching = TRUE;
	}
	else
	{
		do
		{
			flags.bCanAttack = CanNPCAttack(pSoldier);
			if (flags.bCanAttack != TRUE)
			{
				if (flags.fCivilian)
				{
					if ( ( flags.bCanAttack == NOSHOOT_NOWEAPON) && !(pSoldier->flags.uiStatusFlags & SOLDIER_BOXER) && pSoldier->ubBodyType != COW && pSoldier->ubBodyType != CRIPPLECIV )
					{
						// cower in fear!!
						if ( pSoldier->flags.uiStatusFlags & SOLDIER_COWERING )
						{
							if ( pSoldier->aiData.bLastAction == AI_ACTION_COWER )
							{
								// do nothing
								pSoldier->aiData.usActionData = NOWHERE;
								return( AI_ACTION_NONE );
							}
							else
							{
								// set up next action to run away
								pSoldier->aiData.usNextActionData = FindSpotMaxDistFromOpponents( pSoldier );
								if ( pSoldier->aiData.usNextActionData != NOWHERE )
								{
									pSoldier->aiData.bNextAction = AI_ACTION_RUN_AWAY;
									pSoldier->aiData.usActionData = ANIM_STAND;
									return( AI_ACTION_STOP_COWERING );
								}
								else
								{
									return( AI_ACTION_NONE );
								}
							}
						}
						else
						{
							// cower!!!
							pSoldier->aiData.usActionData = ANIM_CROUCH;
							return( AI_ACTION_COWER );
						}
					}
				}
				else if (flags.bCanAttack == NOSHOOT_NOAMMO && flags.ubCanMove && !pSoldier->aiData.bNeutral)
				{
					// try to find more ammo
					pSoldier->aiData.bAction = SearchForItems( pSoldier, SEARCH_AMMO, pSoldier->inv[HANDPOS].usItem );

					if (pSoldier->aiData.bAction == AI_ACTION_NONE)
					{
						// the current weapon appears is useless right now!
						// (since we got a return code of noammo, we know the hand usItem
						// is our gun)
						pSoldier->inv[HANDPOS][0]->data.fFlags |= OBJECT_AI_UNUSABLE;
						// move the gun into another pocket...
						if (!AutoPlaceObject( pSoldier, &(pSoldier->inv[HANDPOS]), FALSE ) )
						{
							// If there's no room in his pockets for the useless gun, just throw it away
							return AI_ACTION_DROP_ITEM;
						}
					}
					else
					{
						return( pSoldier->aiData.bAction );
					}
				}
				else
				{
					flags.bCanAttack = FALSE;
				}
			}
			//WTF???
		//} while( flags.bCanAttack != TRUE && flags.bCanAttack != FALSE );
		} while (false);
	}


#ifdef RETREAT_TESTING
	flags.bCanAttack = FALSE;
#endif

	if (!flags.bCanAttack)
	{
		if (pSoldier->aiData.bAIMorale > MORALE_WORRIED)
		{
			pSoldier->aiData.bAIMorale = MORALE_WORRIED;
		}

		if (!flags.fCivilian)
		{
			// can always attack with HTH as a last resort
			flags.bCanAttack = TRUE;
			flags.fTryPunching = TRUE;
		}
	}

	return AI_ACTION_NOT_AN_ACTION;
}

INT8 BlackAlert_EvaluateFiringAGun(SOLDIERTYPE* pSoldier, BlackAlertFlags& flags)
{
	PERFORMANCE_MARKER
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"FIRE A GUN AT AN OPPONENT");

	INT8 bWeaponIn = FindAIUsableObjClass( pSoldier, IC_GUN );

	if (bWeaponIn != NO_SLOT)
	{
		flags.BestShot.bWeaponIn = bWeaponIn;
		// if it's in another pocket, swap it into his hand temporarily
		AssureItemIsInHandPos(pSoldier, bWeaponIn, TEMPORARILY);

		// now it better be a gun, or the guy can't shoot (but has other attack(s))
		if (Item[pSoldier->inv[HANDPOS].usItem].usItemClass == IC_GUN && pSoldier->inv[HANDPOS][0]->data.gun.bGunStatus >= USABLE)
		{
			// get the minimum cost to attack the same target with this gun
			UINT8 ubMinAPCost = MinAPsToAttack(pSoldier,pSoldier->sLastTarget,ADDTURNCOST);

			// if we have enough action points to shoot with this gun
			if (pSoldier->bActionPoints >= ubMinAPCost)
			{
				// look around for a worthy target (which sets flags.BestShot.ubPossible)
				BOOLEAN shootUnseen = FALSE;
				if (gGameOptions.ubDifficultyLevel > DIF_LEVEL_MEDIUM )
					shootUnseen = TRUE;

				CalcBestShot(pSoldier,&flags.BestShot,shootUnseen);

				if (pSoldier->bTeam == gbPlayerNum && pSoldier->aiData.bRTPCombat == RTP_COMBAT_CONSERVE)
				{
					if (flags.BestShot.ubChanceToReallyHit < 30)
					{
						// skip firing, our chance isn't good enough
						flags.BestShot.ubPossible = FALSE;
					}
				}

				if (flags.BestShot.ubPossible)
				{
					// if the selected opponent is not a threat (unconscious & !serviced)
					// (usually, this means all the guys we see are unconscious, but, on
					//  rare occasions, we may not be able to shoot a healthy guy, too)
					if ((Menptr[flags.BestShot.ubOpponent].stats.bLife < OKLIFE) &&
						!Menptr[flags.BestShot.ubOpponent].bService)
					{
						// if our attitude is NOT aggressive
						if ( pSoldier->aiData.bAttitude != AGGRESSIVE || flags.BestShot.ubChanceToReallyHit < 60 )
						{
							BOOLEAN fClimb;
							// get the location of the closest CONSCIOUS reachable opponent
							INT16 sClosestDisturbance = ClosestReachableDisturbance(pSoldier,FALSE, &fClimb);

							// if we found one
							if (sClosestDisturbance != NOWHERE)
							{
								// don't bother checking GRENADES/KNIVES, he can't have conscious targets
#ifdef RECORDNET
								fprintf(NetDebugFile,"\tDecideActionBlack: all visible opponents unconscious, switching to RED AI...\n");
#endif
								// then make decision as if at alert status RED, but make sure
								// we don't try to SEEK OPPONENT the unconscious guy!
								return(DecideActionRed(pSoldier,FALSE));
							}
							// else kill the guy, he could be the last opponent alive in this sector
						}
						// else aggressive guys will ALWAYS finish off unconscious opponents
					}

					// now we KNOW FOR SURE that we will do something (shoot, at least)
					NPCDoesAct(pSoldier);
					DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"NPC decided to shoot (or something)");

				}
			}

			// if it was in his holster, swap it back into his holster for now
			UndoAssureItemIsInHandPos(pSoldier, bWeaponIn, TEMPORARILY);
		}
	}
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 BlackAlert_EvaluateThrowingSomething(SOLDIERTYPE* pSoldier, BlackAlertFlags& flags)
{
	PERFORMANCE_MARKER
	// this looks for throwables, and sets flags.BestThrow.ubPossible if it can be done
	//if ( !gfHiddenInterrupt )
	// {
	CheckIfTossPossible(pSoldier,&flags.BestThrow);

	if (flags.BestThrow.ubPossible)
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"good throw possible");
		if ( Item[pSoldier->inv[ flags.BestThrow.bWeaponIn ].usItem].mortar )
		{
			UINT8 ubOpponentDir = (UINT8)GetDirectionFromGridNo( flags.BestThrow.sTarget, pSoldier );

			// Get new gridno!
			INT16 sCheckGridNo = NewGridNo( (UINT16)pSoldier->sGridNo, (UINT16)DirectionInc( ubOpponentDir ) );

			if ( !OKFallDirection( pSoldier, sCheckGridNo, pSoldier->pathing.bLevel, ubOpponentDir, pSoldier->usAnimState ) )
			{
				// can't fire!
				flags.BestThrow.ubPossible = FALSE;

				// try behind us, see if there's room to move back
				sCheckGridNo = NewGridNo( (UINT16)pSoldier->sGridNo, (UINT16)DirectionInc( gOppositeDirection[ ubOpponentDir ] ) );
				if ( OKFallDirection( pSoldier, sCheckGridNo, pSoldier->pathing.bLevel, gOppositeDirection[ ubOpponentDir ], pSoldier->usAnimState ) )
				{
					pSoldier->aiData.usActionData = sCheckGridNo;

					return( AI_ACTION_GET_CLOSER );
				}
			}
		}

		if ( flags.BestThrow.ubPossible )
		{
			// now we KNOW FOR SURE that we will do something (throw, at least)
			NPCDoesAct(pSoldier);
		}
	}
	//}
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 BlackAlert_EvaluateStabbing(SOLDIERTYPE* pSoldier, BlackAlertFlags& flags)
{
	PERFORMANCE_MARKER
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"GO STAB AN OPPONENT WITH A KNIFE");
	// if soldier has a knife in his hand
	INT8 bWeaponIn = FindAIUsableObjClass( pSoldier, (IC_BLADE | IC_THROWING_KNIFE) );

	// if the soldier does have a usable knife somewhere
	if (bWeaponIn != NO_SLOT)
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"try to stab");
		flags.BestStab.bWeaponIn = bWeaponIn;

		// get the minimum cost to attack with this knife
		UINT8 ubMinAPCost = MinAPsToAttack(pSoldier,pSoldier->sLastTarget,DONTADDTURNCOST);

		// if we can afford the minimum AP cost to stab with/throw this knife weapon
		if (pSoldier->bActionPoints >= ubMinAPCost)
		{
			// if it's in his holster, swap it into his hand temporarily
			AssureItemIsInHandPos(pSoldier, bWeaponIn, TEMPORARILY);

			// NB throwing knife in hand now
			if ( Item[ pSoldier->inv[HANDPOS].usItem ].usItemClass & IC_THROWING_KNIFE )
			{
				// throwing knife code works like shooting

				// look around for a worthy target (which sets flags.BestStab.ubPossible)
				CalcBestShot(pSoldier,&flags.BestStab,FALSE);

				if (flags.BestStab.ubPossible)
				{
					// if the selected opponent is not a threat (unconscious & !serviced)
					// (usually, this means all the guys we see are unconscious, but, on
					//  rare occasions, we may not be able to shoot a healthy guy, too)
					if ((Menptr[flags.BestStab.ubOpponent].stats.bLife < OKLIFE) &&
						!Menptr[flags.BestStab.ubOpponent].bService)
					{
						// don't throw a knife at him.
						flags.BestStab.ubPossible = FALSE;
					}

					// now we KNOW FOR SURE that we will do something (shoot, at least)
					NPCDoesAct(pSoldier);
					DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"NPC decided to shoot (2)");
				}
			}
			else
			{
				//sprintf((CHAR *)tempstr,"%s - ubMinAPCost = %d",pSoldier->name,ubMinAPCost);
				//PopMessage(tempstr);
				// then look around for a worthy target (which sets flags.BestStab.ubPossible)
				CalcBestStab(pSoldier,&flags.BestStab, TRUE);

				if (flags.BestStab.ubPossible)
				{
					// now we KNOW FOR SURE that we will do something (stab, at least)
					NPCDoesAct(pSoldier);
					DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"NPC decided to stab");
				}
			}

			// if it was in his holster, swap it back into his holster for now
			UndoAssureItemIsInHandPos(pSoldier, bWeaponIn, TEMPORARILY);
		}
	}
	return AI_ACTION_NOT_AN_ACTION;
}

void BlackAlert_DecideBestAction(SOLDIERTYPE* pSoldier, BlackAlertFlags& flags)
{
	PERFORMANCE_MARKER
	flags.ubBestAttackAction = AI_ACTION_NONE;
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"CHOOSE THE BEST TYPE OF ATTACK OUT OF THOSE FOUND TO BE POSSIBLE");
	if (flags.BestShot.ubPossible)
	{
		flags.BestAttack.iAttackValue = flags.BestShot.iAttackValue;
		flags.ubBestAttackAction = AI_ACTION_FIRE_GUN;
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"best action = fire gun");
	}
	else
	{
		flags.BestAttack.iAttackValue = 0;
	}
	if (flags.BestStab.ubPossible && ((flags.BestStab.iAttackValue > flags.BestAttack.iAttackValue) || (flags.ubBestAttackAction == AI_ACTION_NONE)))
	{
		flags.BestAttack.iAttackValue = flags.BestStab.iAttackValue;
		if ( Item[ pSoldier->inv[flags.BestStab.bWeaponIn].usItem ].usItemClass & IC_THROWING_KNIFE )
		{
			flags.ubBestAttackAction = AI_ACTION_THROW_KNIFE;
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"best action = throw knife");
		}
		else
		{
			flags.ubBestAttackAction = AI_ACTION_KNIFE_MOVE;
			DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"best action = move to stab");
		}
	}
	if (flags.BestThrow.ubPossible && ((flags.BestThrow.iAttackValue > flags.BestAttack.iAttackValue) || (flags.ubBestAttackAction == AI_ACTION_NONE)))
	{
		flags.ubBestAttackAction = AI_ACTION_TOSS_PROJECTILE;
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"best action = throw something");
	}

	if ( ( flags.ubBestAttackAction == AI_ACTION_NONE ) && flags.fTryPunching )
	{
		// nothing (else) to attack with so let's try hand-to-hand
		INT8 bWeaponIn = FindObjWithin( pSoldier, NOTHING, HANDPOS, SMALLPOCK8POS );

		if (bWeaponIn != NO_SLOT)
		{
			flags.BestStab.bWeaponIn = bWeaponIn;
			// if it's in his holster, swap it into his hand temporarily
			AssureItemIsInHandPos(pSoldier, bWeaponIn, TEMPORARILY);

			// get the minimum cost to attack by HTH
			UINT8 ubMinAPCost = MinAPsToAttack(pSoldier,pSoldier->sLastTarget,DONTADDTURNCOST);

			// if we can afford the minimum AP cost to use HTH combat
			if (pSoldier->bActionPoints >= ubMinAPCost)
			{
				// then look around for a worthy target (which sets flags.BestStab.ubPossible)
				CalcBestStab(pSoldier,&flags.BestStab, FALSE);

				if (flags.BestStab.ubPossible)
				{
					// now we KNOW FOR SURE that we will do something (stab, at least)
					NPCDoesAct(pSoldier);
					flags.ubBestAttackAction = AI_ACTION_KNIFE_MOVE;
					DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"NPC decided to move to stab");
				}
			}

			// if it was in his holster, swap it back into his holster for now
			UndoAssureItemIsInHandPos(pSoldier, bWeaponIn, TEMPORARILY);
		}
	}



	// copy the information on the best action selected into flags.BestAttack struct
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"copy the information on the best action selected into flags.BestAttack struct");
	switch (flags.ubBestAttackAction)
	{
	case AI_ACTION_FIRE_GUN:
		memcpy(&flags.BestAttack,&flags.BestShot,sizeof(flags.BestAttack));
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"DecideActionBlack: best attack = firing a gun");
		break;

	case AI_ACTION_TOSS_PROJECTILE:
		memcpy(&flags.BestAttack,&flags.BestThrow,sizeof(flags.BestAttack));
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"DecideActionBlack: best attack = tossing a grenade");
		break;

	case AI_ACTION_THROW_KNIFE:
	case AI_ACTION_KNIFE_MOVE:
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"DecideActionBlack: best attack = stab with a knife");
		memcpy(&flags.BestAttack,&flags.BestStab,sizeof(flags.BestAttack));
		break;

	default:
		// set to empty
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"DecideActionBlack: best attack = no good attack");
		memset( &flags.BestAttack, 0, sizeof( flags.BestAttack ) );
		break;
	}
	return;
}

void BlackAlert_DecideOffenseDefense(SOLDIERTYPE* pSoldier, BlackAlertFlags& flags, INT32 iDefense)
{
	PERFORMANCE_MARKER
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"DecideActionBlack: DECIDE BETWEEN ATTACKING AND DEFENDING (TAKING COVER)");
	// gotta compare their merits and select the more desirable option
	INT32 iOffense = flags.BestAttack.ubChanceToReallyHit;

	// based on how we feel about the situation, decide whether to attack first
	switch (pSoldier->aiData.bAIMorale)
	{
	case MORALE_FEARLESS:
		iOffense += iOffense / 2;	// increase 50%
		break;

	case MORALE_CONFIDENT:
		iOffense += iOffense / 4;	// increase 25%
		break;

	case MORALE_NORMAL:
		break;

	case MORALE_WORRIED:
		iDefense += iDefense / 4;	// increase 25%
		break;

	case MORALE_HOPELESS:
		iDefense += iDefense / 2;	// increase 50%
		break;
	}


	// smart guys more likely to try to stay alive, dolts more likely to shoot!
	if (pSoldier->stats.bWisdom >= 50) //Madd: reduced the wisdom required to want to live...
		iDefense += 10;
	else if (pSoldier->stats.bWisdom < 30)
		iDefense -= 10;

	// some orders are more offensive, others more defensive
	if (pSoldier->aiData.bOrders == SEEKENEMY)
		iOffense += 10;
	else if ((pSoldier->aiData.bOrders == STATIONARY) || (pSoldier->aiData.bOrders == ONGUARD) || pSoldier->aiData.bOrders == SNIPER )
		iDefense += 10;

	switch (pSoldier->aiData.bAttitude)
	{
	case DEFENSIVE:		iDefense += 30; break;
	case BRAVESOLO:		iDefense -= 0; break;
	case BRAVEAID:			iDefense -= 0; break;
	case CUNNINGSOLO:	iDefense += 20; break;
	case CUNNINGAID:		iDefense += 20; break;
	case AGGRESSIVE:		iOffense += 10; break;
	case ATTACKSLAYONLY:iOffense += 30; break;
	}

#ifdef DEBUGDECISIONS
	std::string tempstr = String("%s - CHOICE: iOffense = %d, iDefense = %d\n",
		pSoldier->name,iOffense,iDefense);
	DebugAI( tempstr );
#endif

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"DecideActionBlack: if his defensive instincts win out, forget all about the attack");
	// if his defensive instincts win out, forget all about the attack
	if (iDefense > iOffense)
		flags.ubBestAttackAction = AI_ACTION_NONE;
	return;
}

INT8 BlackAlert_TryToFireGun(SOLDIERTYPE* pSoldier, BlackAlertFlags& flags)
{
	PERFORMANCE_MARKER
	INT32 iChance;
	INT8 bDirection;
	UINT8 ubStanceCost;
	UINT8 ubBurstAPs;

	// Do we need to change stance?  NB We'll have to ready our gun again
	if ( !TANK( pSoldier ) && ( pSoldier->bActionPoints >= flags.BestAttack.ubAPCost + AP_CROUCH + MinAPsToAttack( pSoldier, flags.BestAttack.sTarget, ADDTURNCOST, 1 ) ) )
	{
		// since the AI considers shooting chance from standing primarily, if we are not
		// standing we should always consider a stance change
		if ( gAnimControl[ pSoldier->usAnimState ].ubEndHeight != ANIM_STAND )
		{
			iChance = 100;
		}
		else
		{
			iChance = 50;
			switch (pSoldier->aiData.bAttitude)
			{
			case DEFENSIVE:		iChance += 20; break;
			case BRAVESOLO:		iChance -= 10; break;
			case BRAVEAID:			iChance -= 10; break;
			case CUNNINGSOLO:	iChance += 10; break;
			case CUNNINGAID:		iChance += 10; break;
			case AGGRESSIVE:		iChance -= 20; break;
			case ATTACKSLAYONLY: iChance -= 30; break;
			}
		}

		if ( (INT32)PreRandom( 100 ) < iChance || GetRangeInCellCoordsFromGridNoDiff( pSoldier->sGridNo, flags.BestAttack.sTarget ) <= MIN_PRONE_RANGE )
		{

			// first get the direction, as we will need to pass that in to ShootingStanceChange
			bDirection = atan8(CenterX(pSoldier->sGridNo),CenterY(pSoldier->sGridNo),CenterX(flags.BestAttack.sTarget),CenterY(flags.BestAttack.sTarget));

			flags.ubBestStance = ShootingStanceChange( pSoldier, &flags.BestAttack, bDirection );
			if (flags.ubBestStance != 0)
			{
				// change stance first!
				if ( pSoldier->bDirection != bDirection && pSoldier->InternalSoldierReadyWeapon( bDirection, gAnimControl[ pSoldier->usAnimState ].ubEndHeight ) )
				{
					// we're not facing towards him, so turn first!
					pSoldier->aiData.usActionData = bDirection;

#ifdef DEBUGDECISIONS
					std::string tempstr = String("%s - TURNS to face CLOSEST OPPONENT in direction %d",pSoldier->name,pSoldier->aiData.usActionData);
					DebugAI(tempstr);
#endif

					return(AI_ACTION_CHANGE_FACING);
				}

				//						pSoldier->aiData.usActionData = flags.ubBestStance;

				// attack after we change stance
				// we don't just return here because we want to check whether to
				// burst first
				flags.fChangeStanceFirst = TRUE;

				// account for increased AP cost and having to re-ready weapon
				ubStanceCost = (UINT8) GetAPsToChangeStance( pSoldier, flags.ubBestStance );
				flags.BestAttack.ubAPCost = MinAPsToAttack( pSoldier, flags.BestAttack.sTarget, ADDTURNCOST, 1) + ubStanceCost;

				// Clip the aim time if necessary
				if ( flags.BestAttack.ubAPCost + flags.BestAttack.ubAimTime > pSoldier->bActionPoints )
				{
					// AP cost would balance (plus X, minus X) but aim time is reduced
					flags.BestAttack.ubAimTime = pSoldier->bActionPoints - flags.BestAttack.ubAPCost - ubStanceCost;
					if (flags.BestAttack.ubAimTime < 0 )
					{
						// This is actually a logic error situation.  The ChangeStance section is supposed
						// to not make a shot impossible after changing stance.
						flags.BestAttack.ubPossible = 0;
					}
				}
			}
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// IF ENOUGH APs TO BURST, RANDOM CHANCE OF DOING SO
	//////////////////////////////////////////////////////////////////////////

	if (IsGunBurstCapable( pSoldier, flags.BestAttack.bWeaponIn, FALSE ) &&
		!(Menptr[flags.BestShot.ubOpponent].stats.bLife < OKLIFE) && // don't burst at downed targets
		pSoldier->inv[flags.BestAttack.bWeaponIn][0]->data.gun.ubGunShotsLeft > 1 &&
		(pSoldier->bTeam != gbPlayerNum || pSoldier->aiData.bRTPCombat == RTP_COMBAT_AGGRESSIVE) )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"DecideActionBlack: ENOUGH APs TO BURST, RANDOM CHANCE OF DOING SO");

		ubBurstAPs = CalcAPsToBurst( pSoldier->CalcActionPoints( ), &(pSoldier->inv[flags.BestAttack.bWeaponIn]) );

		if (pSoldier->bActionPoints >= flags.BestAttack.ubAPCost + flags.BestAttack.ubAimTime + ubBurstAPs )
		{
			// Base chance of bursting is 25% if best shot was +0 aim, down to 8% at +4
			if ( TANK( pSoldier ) )
			{
				iChance = 100;
			}
			else
			{
				iChance = (25 / max((flags.BestAttack.ubAimTime + 1),1));
				switch (pSoldier->aiData.bAttitude)
				{
				case DEFENSIVE:		iChance += -5; break;
				case BRAVESOLO:		iChance +=  5; break;
				case BRAVEAID:		iChance +=  5; break;
				case CUNNINGSOLO:	iChance +=  0; break;
				case CUNNINGAID:	iChance +=  0; break;
				case AGGRESSIVE:	iChance += 10; break;
				case ATTACKSLAYONLY:iChance += 30; break;
				}

				if ( pSoldier->inv[flags.BestAttack.bWeaponIn][0]->data.gun.ubGunShotsLeft > 50 )
					iChance += 20;

				// increase chance based on proximity and difficulty of enemy
				if ( PythSpacesAway( pSoldier->sGridNo, flags.BestAttack.sTarget ) < 15 )
				{
					DebugMsg(TOPIC_JA2AI,DBG_LEVEL_3,String("DecideActionBlack: check chance to burst"));
					iChance += ( 15 - PythSpacesAway( pSoldier->sGridNo, flags.BestAttack.sTarget ) ) * ( 1 + SoldierDifficultyLevel( pSoldier ) );
					if ( pSoldier->aiData.bAttitude == ATTACKSLAYONLY )
					{
						// increase it more!
						iChance += 5 * ( 15 - PythSpacesAway( pSoldier->sGridNo, flags.BestAttack.sTarget ) );
					}
				}
				else if (PythSpacesAway( pSoldier->sGridNo, flags.BestAttack.sTarget ) < 10 && gGameOptions.ubDifficultyLevel > DIF_LEVEL_EASY )
				{
					iChance += 100;
				}
			}

			if ( (INT32) PreRandom( 100 ) < iChance)
			{
				flags.BestAttack.ubAimTime = BURSTING;
				flags.BestAttack.ubAPCost = flags.BestAttack.ubAPCost + ubBurstAPs;
				// check for spread burst possibilities
				if (pSoldier->aiData.bAttitude != ATTACKSLAYONLY)
				{
					CalcSpreadBurst( pSoldier, flags.BestAttack.sTarget, flags.BestAttack.bTargetLevel );
				}
			}
		}
	}

	if (IsGunAutofireCapable( pSoldier, flags.BestAttack.bWeaponIn ) &&
		!(Menptr[flags.BestShot.ubOpponent].stats.bLife < OKLIFE) && // don't burst at downed targets
		(( pSoldier->inv[flags.BestAttack.bWeaponIn][0]->data.gun.ubGunShotsLeft > 1 &&
		flags.BestAttack.ubAimTime != BURSTING ) || Weapon[pSoldier->inv[flags.BestAttack.bWeaponIn].usItem].NoSemiAuto) )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"DecideActionBlack: ENOUGH APs TO AUTOFIRE, RANDOM CHANCE OF DOING SO");
		pSoldier->bDoAutofire = 0;
		do
		{
			pSoldier->bDoAutofire++;
			ubBurstAPs = CalcAPsToAutofire( pSoldier->CalcActionPoints( ), &(pSoldier->inv[flags.BestAttack.bWeaponIn]), pSoldier->bDoAutofire );
		}
		while(	pSoldier->bActionPoints >= flags.BestAttack.ubAPCost + ubBurstAPs &&
			pSoldier->inv[ pSoldier->ubAttackingHand ][0]->data.gun.ubGunShotsLeft >= pSoldier->bDoAutofire &&
			GetAutoPenalty(&pSoldier->inv[ flags.BestAttack.bWeaponIn ], gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_PRONE)*pSoldier->bDoAutofire <= 80);



		pSoldier->bDoAutofire--;
		if (pSoldier->bDoAutofire > 0)
		{
			ubBurstAPs = CalcAPsToAutofire( pSoldier->CalcActionPoints( ), &(pSoldier->inv[flags.BestAttack.bWeaponIn]), pSoldier->bDoAutofire );

			if (pSoldier->bActionPoints >= flags.BestAttack.ubAPCost + ubBurstAPs )
			{
				// Base chance of bursting is 25% if best shot was +0 aim, down to 8% at +4
				if ( TANK( pSoldier ) )
				{
					iChance = 100;
				}
				else
				{
					iChance = (100 / max((flags.BestAttack.ubAimTime + 1),1));
					switch (pSoldier->aiData.bAttitude)
					{
					case DEFENSIVE:		iChance += -5; break;
					case BRAVESOLO:		iChance +=  5; break;
					case BRAVEAID:		iChance +=  5; break;
					case CUNNINGSOLO:	iChance +=  0; break;
					case CUNNINGAID:	iChance +=  0; break;
					case AGGRESSIVE:	iChance += 10; break;
					case ATTACKSLAYONLY:iChance += 30; break;
					}


					if ( pSoldier->inv[flags.BestAttack.bWeaponIn][0]->data.gun.ubGunShotsLeft > 50 )
						iChance += 30;

					if ( flags.bInGas )
						iChance += 50; //Madd: extra chance of going nuts and autofiring if stuck in gas

					// increase chance based on proximity and difficulty of enemy
					if ( PythSpacesAway( pSoldier->sGridNo, flags.BestAttack.sTarget ) < 15 )
					{
						DebugMsg(TOPIC_JA2AI,DBG_LEVEL_3,String("DecideActionBlack: check chance to autofire"));
						iChance += ( 15 - PythSpacesAway( pSoldier->sGridNo, flags.BestAttack.sTarget ) ) * ( 1 + SoldierDifficultyLevel( pSoldier ) );
						if ( pSoldier->aiData.bAttitude == ATTACKSLAYONLY )
						{
							// increase it more!
							iChance += 5 * ( 15 - PythSpacesAway( pSoldier->sGridNo, flags.BestAttack.sTarget ) );
						}
					}
				}

				if ((INT32) PreRandom( 100 ) < iChance || Weapon[pSoldier->inv[flags.BestAttack.bWeaponIn].usItem].NoSemiAuto)
				{
					flags.BestAttack.ubAimTime = AUTOFIRING + pSoldier->bDoAutofire;
					flags.BestAttack.ubAPCost = flags.BestAttack.ubAPCost + CalcAPsToAutofire( pSoldier->CalcActionPoints( ), &(pSoldier->inv[flags.BestAttack.bWeaponIn]), pSoldier->bDoAutofire );
				}
			}
		}
		else
		{
			pSoldier->aiData.bAimTime	= 0;
			pSoldier->bDoBurst			= 0;
			pSoldier->bDoAutofire		= 0;
			// not enough aps - do somthing else
		}
	}

	//////////////////////////////////////////////////////////////////////////
	// IF NOT CROUCHED & WILL STILL HAVE ENOUGH APs TO DO THIS SAME BEST
	// ATTACK AFTER A STANCE CHANGE, CONSIDER CHANGING STANCE
	//////////////////////////////////////////////////////////////////////////



	if (flags.BestAttack.ubAimTime == BURSTING)
	{
		pSoldier->aiData.bAimTime	= 0;
		pSoldier->bDoBurst			= 1;
		pSoldier->bDoAutofire		= 0;
	}
	else if(flags.BestAttack.ubAimTime >= AUTOFIRING)
	{
		pSoldier->aiData.bAimTime	= 0;
		pSoldier->bDoBurst			= 1;
		pSoldier->bDoAutofire		= flags.BestAttack.ubAimTime-AUTOFIRING;

		flags.BestAttack.ubAimTime = AUTOFIRING;
	}

	/*
	else // defaults already set
	{
	pSoldier->aiData.bAimTime			= flags.BestAttack.ubAimTime;
	pSoldier->bDoBurst			= 0;
	}
	*/

	return AI_ACTION_NOT_AN_ACTION;
}

INT8 BlackAlert_ChangeStance(SOLDIERTYPE* pSoldier, BlackAlertFlags& flags)
{
	PERFORMANCE_MARKER
	if (pSoldier->aiData.usActionData == ANIM_PRONE)
	{
		// we might want to turn before lying down!
		if ( (!gfTurnBasedAI || GetAPsToLook( pSoldier ) <= pSoldier->bActionPoints - GetAPsToChangeStance( pSoldier, (INT8) pSoldier->aiData.usActionData )) &&
			(((pSoldier->aiData.bAIMorale > MORALE_HOPELESS) || flags.ubCanMove) && !AimingGun(pSoldier)) )
		{
			// determine the location of the known closest opponent
			// (don't care if he's conscious, don't care if he's reachable at all)

			INT16 sClosestOpponent = ClosestSeenOpponent(pSoldier, NULL, NULL);
			// if we have a closest seen opponent
			if (sClosestOpponent != NOWHERE)
			{
				INT8 bDirection = atan8(CenterX(pSoldier->sGridNo),CenterY(pSoldier->sGridNo),CenterX(sClosestOpponent),CenterY(sClosestOpponent));

				// if we're not facing towards him
				if (pSoldier->bDirection != bDirection)
				{
					if ( pSoldier->InternalSoldierReadyWeapon( bDirection, (INT8) pSoldier->aiData.usActionData) )
					{
						// change direction, THEN change stance!
						pSoldier->aiData.bNextAction = AI_ACTION_CHANGE_STANCE;
						pSoldier->aiData.usNextActionData = pSoldier->aiData.usActionData;
						pSoldier->aiData.usActionData = bDirection;
#ifdef DEBUGDECISIONS
						std::string tempstr = String("%s - TURNS to face CLOSEST OPPONENT in direction %d",pSoldier->name,pSoldier->aiData.usActionData);
						DebugAI(tempstr);
#endif
						return(AI_ACTION_CHANGE_FACING);
					}
					else if ( (pSoldier->aiData.usActionData == ANIM_PRONE) && (pSoldier->InternalSoldierReadyWeapon( bDirection, ANIM_CROUCH) ) )
					{
						// we shouldn't go prone, since we can't turn to shoot
						pSoldier->aiData.usActionData = ANIM_CROUCH;
						pSoldier->aiData.bNextAction = AI_ACTION_END_TURN;
						return( AI_ACTION_CHANGE_STANCE );
					}
				}
				// else we are facing in the right direction

			}
			// else we don't know any enemies
		}

		// we don't want to turn
	}
	pSoldier->aiData.bNextAction = AI_ACTION_END_TURN;
	return( AI_ACTION_CHANGE_STANCE );
}

INT8 BlackAlert_TryToRadioRedAlert(SOLDIERTYPE* pSoldier, BlackAlertFlags& flags)
{
	PERFORMANCE_MARKER
	INT32 iChance;
	// if there hasn't been an initial RED ALERT yet in this sector
	if ( !(gTacticalStatus.Team[pSoldier->bTeam].bAwareOfOpposition) || NeedToRadioAboutPanicTrigger() )
	{     // since I'm at STATUS RED, I obviously know we're being invaded!
		DebugMsg(TOPIC_JA2AI,DBG_LEVEL_3,String("DecideActionBlack: check chance to radio contact"));
		iChance = gbDiff[DIFF_RADIO_RED_ALERT][ SoldierDifficultyLevel( pSoldier ) ];
	}
	else // subsequent radioing (only to update enemy positions, request help)
		// base chance depends on how much new info we have to radio to the others
		iChance = 10 * WhatIKnowThatPublicDont(pSoldier,FALSE);  // use 10 * for RED alert

	// if I actually know something they don't and I ain't swimming (deep water)
	if (iChance && !flags.bInDeepWater)
	{
		// modify base chance according to orders
		switch (pSoldier->aiData.bOrders)
		{
		case STATIONARY:       iChance +=  20;  break;
		case ONGUARD:          iChance +=  15;  break;
		case ONCALL:           iChance +=  10;  break;
		case CLOSEPATROL:                       break;
		case RNDPTPATROL:
		case POINTPATROL:      iChance +=  -5;  break;
		case FARPATROL:        iChance += -10;  break;
		case SEEKENEMY:        iChance += -20;  break;
		case SNIPER:			  iChance += -10;  break;
		}

		// modify base chance according to attitude
		switch (pSoldier->aiData.bAttitude)
		{
		case DEFENSIVE:        iChance +=  20;  break;
		case BRAVESOLO:        iChance += -10;  break;
		case BRAVEAID:                          break;
		case CUNNINGSOLO:      iChance +=  -5;  break;
		case CUNNINGAID:                        break;
		case AGGRESSIVE:       iChance += -20;  break;
		case ATTACKSLAYONLY:		iChance = 0;
		}

		if (gTacticalStatus.Team[pSoldier->bTeam].bAwareOfOpposition)
		{
			// ignore morale (which could be really high)
		}
		else
		{
			// modify base chance according to morale
			switch (pSoldier->aiData.bAIMorale)
			{
			case MORALE_HOPELESS:  iChance *= 3;    break;
			case MORALE_WORRIED:   iChance *= 2;    break;
			case MORALE_NORMAL:                     break;
			case MORALE_CONFIDENT: iChance /= 2;    break;
			case MORALE_FEARLESS:  iChance /= 3;    break;
			}
		}

		// reduce chance because we're in combat
		iChance /= 2;

#ifdef DEBUGDECISIONS
		AINumMessage("Chance to radio RED alert = ",iChance);
#endif

		if ((INT16) PreRandom(100) < iChance)
		{
#ifdef DEBUGDECISIONS
			AINameMessage(pSoldier,"decides to radio a RED alert!",1000);
#endif

			return(AI_ACTION_RED_ALERT);
		}
	}
	return AI_ACTION_NOT_AN_ACTION;
}

void BlackAlert_ShootGun(SOLDIERTYPE* pSoldier, BlackAlertFlags& flags)
{
	PERFORMANCE_MARKER
	pSoldier->aiData.usActionData = flags.BestAttack.sTarget;
	pSoldier->bTargetLevel = flags.BestAttack.bTargetLevel;

#ifdef DEBUGDECISIONS
	std::string tempstr = String("%d(%s) %s %d(%s) at gridno %d (%d APs aim)\n",
		pSoldier->ubID,pSoldier->name,
		(flags.ubBestAttackAction == AI_ACTION_FIRE_GUN)?"SHOOTS":((flags.ubBestAttackAction == AI_ACTION_TOSS_PROJECTILE)?"TOSSES AT":"STABS"),
		flags.BestAttack.ubOpponent,pSoldier->name ,
		flags.BestAttack.sTarget,flags.BestAttack.ubAimTime );
	DebugAI( tempstr);
#endif


	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, String("DecideActionBlack: Check for GL Bursts, is launcher capable? = %d, rtpcombat? = %d, bestattackaction = %d",IsGunBurstCapable( pSoldier, flags.BestAttack.bWeaponIn, FALSE ),pSoldier->aiData.bRTPCombat,flags.ubBestAttackAction ));
	if (flags.ubBestAttackAction == AI_ACTION_TOSS_PROJECTILE && (Item[pSoldier->inv[flags.BestAttack.bWeaponIn].usItem].usItemClass == IC_LAUNCHER && IsGunBurstCapable( pSoldier, flags.BestAttack.bWeaponIn, FALSE )) &&
		(pSoldier->bTeam != gbPlayerNum || pSoldier->aiData.bRTPCombat == RTP_COMBAT_AGGRESSIVE) )
	{

		DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "DecideActionBlack: Doing burst calc");
		UINT8 ubBurstAPs = CalcAPsToBurst( pSoldier->CalcActionPoints( ), &(pSoldier->inv[flags.BestAttack.bWeaponIn]) );

		if ( (pSoldier->bActionPoints - flags.BestAttack.ubAPCost) >= ubBurstAPs )
		{
			INT32 iChance;
			// Base chance of bursting is 25% if best shot was +0 aim, down to 8% at +4
			if ( TANK( pSoldier ) )
			{
				iChance = 100;
			}
			else
			{
				iChance = 50;
				switch (pSoldier->aiData.bAttitude)
				{
				case DEFENSIVE:		iChance += -5; break;
				case BRAVESOLO:		iChance +=  5; break;
				case BRAVEAID:		iChance +=  5; break;
				case CUNNINGSOLO:	iChance +=  0; break;
				case CUNNINGAID:	iChance +=  0; break;
				case AGGRESSIVE:	iChance += 10; break;
				case ATTACKSLAYONLY:iChance += 30; break;
				}
				// increase chance based on proximity and difficulty of enemy
				DebugMsg(TOPIC_JA2AI,DBG_LEVEL_3,String("DecideActionBlack: check chance to gl burst"));
				iChance += ( 15 - PythSpacesAway( pSoldier->sGridNo, flags.BestAttack.sTarget ) ) * ( 1 + SoldierDifficultyLevel( pSoldier ) );
			}

			if ( (INT32) PreRandom( 100 ) < iChance)
			{
				DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "DecideActionBlack: Doing GL burst");
				flags.BestAttack.ubAimTime = BURSTING;
				flags.BestAttack.ubAPCost = flags.BestAttack.ubAPCost + CalcAPsToBurst( pSoldier->CalcActionPoints( ), &(pSoldier->inv[HANDPOS]) );
				// check for spread burst possibilities
				if (pSoldier->aiData.bAttitude != ATTACKSLAYONLY)
				{
					CalcSpreadBurst( pSoldier, flags.BestAttack.sTarget, flags.BestAttack.bTargetLevel );
				}
				pSoldier->bDoBurst = 1;

			}
		}
	}

	return;
}

INT8 BlackAlert_TryToFaceEnemy(SOLDIERTYPE* pSoldier, BlackAlertFlags& flags)
{
	PERFORMANCE_MARKER
	// hopeless guys shouldn't waste their time this way, UNLESS they CAN move
	// but chose not to to get this far (which probably means they're cornered)
	// ALSO, don't bother turning if we're already aiming a gun
	if ( !gfHiddenInterrupt && ((pSoldier->aiData.bAIMorale > MORALE_HOPELESS) || flags.ubCanMove) && !AimingGun(pSoldier))
	{
		// determine the location of the known closest opponent
		// (don't care if he's conscious, don't care if he's reachable at all)


		INT16 sClosestOpponent = ClosestSeenOpponent(pSoldier, NULL, NULL);
		// if we have a closest reachable opponent
		if (sClosestOpponent != NOWHERE)
		{
			INT8 bDirection = atan8(CenterX(pSoldier->sGridNo),CenterY(pSoldier->sGridNo),CenterX(sClosestOpponent),CenterY(sClosestOpponent));

			// if we're not facing towards him
			if ( pSoldier->bDirection != bDirection && pSoldier->InternalSoldierReadyWeapon( bDirection, gAnimControl[ pSoldier->usAnimState ].ubEndHeight ) )
			{
				pSoldier->aiData.usActionData = bDirection;

#ifdef DEBUGDECISIONS
				std::string tempstr = String("%s - TURNS to face CLOSEST OPPONENT in direction %d",pSoldier->name,pSoldier->aiData.usActionData);
				DebugAI(tempstr);
#endif

				return(AI_ACTION_CHANGE_FACING);
			}
		}
	}
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 BlackAlert_HandleSpecialCases(SOLDIERTYPE* pSoldier, BlackAlertFlags& flags)
{
	PERFORMANCE_MARKER
	INT8 bActionReturned;
	if ( (pSoldier->bTeam == ENEMY_TEAM || pSoldier->ubProfile == WARDEN) && (gTacticalStatus.fPanicFlags & PANIC_TRIGGERS_HERE) && (gTacticalStatus.ubTheChosenOne == NOBODY) )
	{
		INT8 bPanicTrigger;

		bPanicTrigger = ClosestPanicTrigger( pSoldier );
		// if it's an alarm trigger and team is alerted, ignore it
		if ( !(gTacticalStatus.bPanicTriggerIsAlarm[ bPanicTrigger ] && gTacticalStatus.Team[pSoldier->bTeam].bAwareOfOpposition) && PythSpacesAway( pSoldier->sGridNo, gTacticalStatus.sPanicTriggerGridNo[ bPanicTrigger ] ) < 10)
		{
			PossiblyMakeThisEnemyChosenOne( pSoldier );
		}
	}

	// if this soldier is the "Chosen One" (enemies only)
	if (pSoldier->ubID == gTacticalStatus.ubTheChosenOne)
	{
		// do some special panic AI decision making
		bActionReturned = PanicAI(pSoldier,flags.ubCanMove);

		// if we decided on an action while in there, we're done
		if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
			return(bActionReturned);
	}

	if ( pSoldier->ubProfile != NO_PROFILE )
	{
		// if they see enemies, the Queen will keep going to the staircase, but Joe will fight
		if ( (pSoldier->ubProfile == QUEEN) && flags.ubCanMove )
		{
			if ( gWorldSectorX == 3 && gWorldSectorY == MAP_ROW_P && gbWorldSectorZ == 0 && !gfUseAlternateQueenPosition )
			{
				bActionReturned = HeadForTheStairCase( pSoldier );
				if ( bActionReturned != AI_ACTION_NONE )
				{
					return( bActionReturned );
				}
			}
		}
	}
	return AI_ACTION_NOT_AN_ACTION;
}

INT8 DecideActionBlack(SOLDIERTYPE *pSoldier)
{
	PERFORMANCE_MARKER
	DebugMsg(TOPIC_JA2,DBG_LEVEL_3,String("DecideActionBlack: soldier = %d, orders = %d, attitude = %d",pSoldier->ubID,pSoldier->aiData.bOrders,pSoldier->aiData.bAttitude));
	// if we have absolutely no action points, we can't do a thing under BLACK!
	if (!pSoldier->bActionPoints)
	{
		pSoldier->aiData.usActionData = NOWHERE;
		return(AI_ACTION_NONE);
	}

	BlackAlertFlags flags;

	INT32	iCoverPercentBetter = 0, iChance;
	INT16	sClosestOpponent,sBestCover = NOWHERE;
	INT8	bActionReturned;
	flags.fTryPunching = FALSE;
#ifdef DEBUGDECISIONS
	std::string tempstr;
#endif

	flags.fCivilian = (PTR_CIVILIAN && (pSoldier->ubCivilianGroup == NON_CIV_GROUP || pSoldier->aiData.bNeutral || (pSoldier->ubBodyType >= FATCIV && pSoldier->ubBodyType <= CRIPPLECIV) ) );

	flags.fAllowCoverCheck = FALSE;

	// once we hit status black, reset flanking status
	//pSoldier->numFlanks = 0;

	// can this guy move to any of the neighbouring squares ? (sets TRUE/FALSE)
	flags.ubCanMove = (pSoldier->bActionPoints >= MinPtsToMove(pSoldier));



	////////////////////////////////////////////////////////////////////////////
	// HANDLE QUEEN, ALARM TRIGGER
	////////////////////////////////////////////////////////////////////////////
	bActionReturned = BlackAlert_HandleSpecialCases(pSoldier, flags);
	if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
		return(bActionReturned);


	////////////////////////////////////////////////////////////////////////////
	// SET SOME FLAGS, HANDLES A FEW OTHER SPECIAL CASES
	////////////////////////////////////////////////////////////////////////////
	bActionReturned = BlackAlert_SetSomeFlags(pSoldier, flags);
	if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
		return(bActionReturned);



	////////////////////////////////////////////////////////////////////////////
	// STUCK IN WATER OR GAS, NO COVER, GO TO NEAREST SPOT OF UNGASSED LAND
	////////////////////////////////////////////////////////////////////////////

	// if soldier in water/gas has enough APs left to move at least 1 square
	if ( ( flags.bInDeepWater || flags.bInGas ) && flags.ubCanMove)
	{
		bActionReturned = BlackAlert_TryToLeaveGas(pSoldier, flags);
		if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
			return(bActionReturned);
	}

	// offer surrender?
	if ( pSoldier->bTeam == ENEMY_TEAM && pSoldier->bVisible == TRUE && !( gTacticalStatus.fEnemyFlags & ENEMY_OFFERED_SURRENDER ) && pSoldier->stats.bLife >= pSoldier->stats.bLifeMax / 2 )
	{
		if ( gTacticalStatus.Team[ MILITIA_TEAM ].bMenInSector == 0 && NumPCsInSector() < 4 && gTacticalStatus.Team[ ENEMY_TEAM ].bMenInSector >= NumPCsInSector() * 3 )
		{
			//if( GetWorldDay() > STARTDAY_ALLOW_PLAYER_CAPTURE_FOR_RESCUE && !( gStrategicStatus.uiFlags & STRATEGIC_PLAYER_CAPTURED_FOR_RESCUE ) )
			{
				if ( gubQuest[ QUEST_HELD_IN_ALMA ] == QUESTNOTSTARTED || ( gubQuest[ QUEST_HELD_IN_ALMA ] == QUESTDONE && gubQuest[ QUEST_INTERROGATION ] == QUESTNOTSTARTED ) )
				{
					gTacticalStatus.fEnemyFlags |= ENEMY_OFFERED_SURRENDER;
					return( AI_ACTION_OFFER_SURRENDER );
				}
			}
		}
	}


	bActionReturned = BlackAlert_EvaluateCanAttack(pSoldier, flags);
	if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
		return(bActionReturned);


	// if we don't have a gun, look around for a weapon!
	if (flags.ubCanMove && !pSoldier->aiData.bNeutral && FindAIUsableObjClass( pSoldier, IC_GUN ) == ITEM_NOT_FOUND)
	{
		// look around for a gun...
		pSoldier->aiData.bAction = SearchForItems( pSoldier, SEARCH_WEAPONS, pSoldier->inv[HANDPOS].usItem );
		if (pSoldier->aiData.bAction != AI_ACTION_NONE )
		{
			return( pSoldier->aiData.bAction );
		}
	}


	flags.BestShot.ubPossible  = FALSE;	// by default, assume Shooting isn't possible
	flags.BestThrow.ubPossible = FALSE;	// by default, assume Throwing isn't possible
	flags.BestStab.ubPossible  = FALSE;	// by default, assume Stabbing isn't possible

	flags.BestAttack.ubChanceToReallyHit = 0;


	// if we are able attack
	if (flags.bCanAttack)
	{
		pSoldier->bAimShotLocation = AIM_SHOT_RANDOM;

		//////////////////////////////////////////////////////////////////////////
		// FIRE A GUN AT AN OPPONENT
		//////////////////////////////////////////////////////////////////////////
		bActionReturned = BlackAlert_EvaluateFiringAGun(pSoldier, flags);
		if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
			return(bActionReturned);


		//////////////////////////////////////////////////////////////////////////
		// THROW A TOSSABLE ITEM AT OPPONENT(S)
		// 	- HTH: THIS NOW INCLUDES FIRING THE GRENADE LAUNCHAR AND MORTAR!
		//////////////////////////////////////////////////////////////////////////
		bActionReturned = BlackAlert_EvaluateThrowingSomething(pSoldier, flags);
		if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
			return(bActionReturned);


		//////////////////////////////////////////////////////////////////////////
		// GO STAB AN OPPONENT WITH A KNIFE
		//////////////////////////////////////////////////////////////////////////
		// 0verhaul:  And is not a tank!
		if (!TANK( pSoldier)) {
			BlackAlert_EvaluateStabbing(pSoldier, flags);
		}


		//////////////////////////////////////////////////////////////////////////
		// CHOOSE THE BEST TYPE OF ATTACK OUT OF THOSE FOUND TO BE POSSIBLE
		//////////////////////////////////////////////////////////////////////////
		BlackAlert_DecideBestAction(pSoldier, flags);
	}

	// NB a desire of 4 or more is only achievable by brave/aggressive guys with high morale
	if ( pSoldier->bActionPoints == pSoldier->bInitialActionPoints && flags.ubBestAttackAction == AI_ACTION_FIRE_GUN && (pSoldier->aiData.bShock == 0) && (pSoldier->stats.bLife >= pSoldier->stats.bLifeMax / 2) && flags.BestAttack.ubChanceToReallyHit < 30 && ( PythSpacesAway( pSoldier->sGridNo, flags.BestAttack.sTarget ) > Weapon[ pSoldier->inv[ flags.BestAttack.bWeaponIn ].usItem ].usRange / CELL_X_SIZE ) && RangeChangeDesire( pSoldier ) >= 4 )
	{
		// okay, really got to wonder about this... could taking cover be an option?
		if (flags.ubCanMove && pSoldier->aiData.bOrders != STATIONARY && !gfHiddenInterrupt &&
			!(pSoldier->flags.uiStatusFlags & SOLDIER_BOXER) )
		{
			// make militia a bit more cautious
			if ( ( (pSoldier->bTeam == MILITIA_TEAM) && (PreRandom( 20 ) > flags.BestAttack.ubChanceToReallyHit) )
				|| ( (pSoldier->bTeam != MILITIA_TEAM) && (PreRandom( 40 ) > flags.BestAttack.ubChanceToReallyHit) ) )
			{
				//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"AI %d allowing cover check, chance to hit is only %d, at range %d", flags.BestAttack.ubChanceToReallyHit, PythSpacesAway( pSoldier->sGridNo, flags.BestAttack.sTarget ) );
				// maybe taking cover would be better!
				flags.fAllowCoverCheck = TRUE;
				if ( PreRandom( 10 ) > flags.BestAttack.ubChanceToReallyHit )
				{
					DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"DecideActionBlack: can't hit so screw the attack");
					// screw the attack!
					flags.ubBestAttackAction = AI_ACTION_NONE;
				}
			}
		}
	}


	////////////////////////////////////////////////////////////////////////////
	// LOOK FOR SOME KIND OF COVER BETTER THAN WHAT WE HAVE NOW
	////////////////////////////////////////////////////////////////////////////

	// if soldier has enough APs left to move at least 1 square's worth,
	// and either he can't attack any more, or his attack did wound someone
	if ( (flags.ubCanMove && !SkipCoverCheck && !gfHiddenInterrupt &&
		((flags.ubBestAttackAction == AI_ACTION_NONE) || pSoldier->aiData.bLastAttackHit) &&
		(pSoldier->bTeam != gbPlayerNum || pSoldier->aiData.fAIFlags & AI_RTP_OPTION_CAN_SEEK_COVER) &&
		!(pSoldier->flags.uiStatusFlags & SOLDIER_BOXER) )
		|| flags.fAllowCoverCheck )
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"LOOK FOR SOME KIND OF COVER BETTER THAN WHAT WE HAVE NOW");
		sBestCover = FindBestNearbyCover(pSoldier,pSoldier->aiData.bAIMorale,&iCoverPercentBetter);
	}


#ifdef RETREAT_TESTING
	sBestCover = NOWHERE;
#endif

	//////////////////////////////////////////////////////////////////////////
	// IF NECESSARY, DECIDE BETWEEN ATTACKING AND DEFENDING (TAKING COVER)
	//////////////////////////////////////////////////////////////////////////
	// if both are possible
	if ((flags.ubBestAttackAction != AI_ACTION_NONE) && (sBestCover != NOWHERE))
	{
		BlackAlert_DecideOffenseDefense(pSoldier, flags, iCoverPercentBetter);
	}


	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("DecideActionBlack: is attack still desirable?  flags.ubBestAttackAction = %d",flags.ubBestAttackAction));

	// if attack is still desirable (meaning it's also preferred to taking cover)
	if (flags.ubBestAttackAction != AI_ACTION_NONE)
	{
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"DecideActionBlack: attack is still desirable (meaning it's also preferred to taking cover)");
		// if we wanted to be REALLY mean, we could look at chance to hit and decide whether
		// to shoot at the head...

		flags.fChangeStanceFirst = FALSE;

		// default settings
		pSoldier->aiData.bAimTime			= flags.BestAttack.ubAimTime;
		pSoldier->bDoBurst			= 0;

		if (flags.ubBestAttackAction == AI_ACTION_FIRE_GUN)
		{
			bActionReturned = BlackAlert_TryToFireGun(pSoldier, flags);
			if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
				return(bActionReturned);
		}
		else if (flags.ubBestAttackAction == AI_ACTION_THROW_KNIFE)
		{
			if (flags.BestAttack.bWeaponIn != HANDPOS && gAnimControl[ pSoldier->usAnimState ].ubEndHeight == ANIM_STAND )
			{
				// we had better make sure we lower our gun first!

				pSoldier->aiData.bAction = AI_ACTION_LOWER_GUN;
				pSoldier->aiData.usActionData = 0;

				// queue up attack for after we lower weapon if any
				pSoldier->aiData.bNextAction = AI_ACTION_THROW_KNIFE;
				pSoldier->aiData.usNextActionData = flags.BestAttack.sTarget;
				pSoldier->aiData.bNextTargetLevel = flags.BestAttack.bTargetLevel;
			}

		}

		//////////////////////////////////////////////////////////////////////////
		// OTHERWISE, JUST GO AHEAD & ATTACK!
		//////////////////////////////////////////////////////////////////////////
		DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"OTHERWISE, JUST GO AHEAD & ATTACK!");


		// swap weapon to hand if necessary
		AssureItemIsInHandPos(pSoldier, flags.BestAttack.bWeaponIn, FOREVER);

		if (flags.fChangeStanceFirst)
		{ // currently only for guns...
			pSoldier->aiData.bNextAction = AI_ACTION_FIRE_GUN;
			pSoldier->aiData.usNextActionData = flags.BestAttack.sTarget;
			pSoldier->aiData.bNextTargetLevel = flags.BestAttack.bTargetLevel;
			pSoldier->aiData.usActionData = flags.ubBestStance;
			return( AI_ACTION_CHANGE_STANCE );
		}
		else
		{
			BlackAlert_ShootGun(pSoldier, flags);
			return(flags.ubBestAttackAction);
		}
	}

	// end of tank AI
	if ( TANK( pSoldier ) )
	{
		return( AI_ACTION_NONE );
	}

	// try to make boxer close if possible
	if (pSoldier->flags.uiStatusFlags & SOLDIER_BOXER )
	{
		if ( flags.ubCanMove )
		{
			sClosestOpponent = ClosestSeenOpponent(pSoldier, NULL, NULL);
			if ( sClosestOpponent != NOWHERE )
			{
				// temporarily make boxer have orders of CLOSEPATROL rather than STATIONARY
				// And make him patrol the ring, not his usual place
				// so he has a good roaming range
				USHORT tgrd = pSoldier->aiData.usPatrolGrid[0];
				pSoldier->aiData.usPatrolGrid[0] = pSoldier->sGridNo;
				pSoldier->aiData.bOrders = CLOSEPATROL;
				pSoldier->aiData.usActionData = GoAsFarAsPossibleTowards( pSoldier, sClosestOpponent, AI_ACTION_GET_CLOSER );
				pSoldier->aiData.usPatrolGrid[0] = tgrd;
				pSoldier->aiData.bOrders = STATIONARY;
				if ( pSoldier->aiData.usActionData != NOWHERE )
				{
					// truncate path to 1 step
					pSoldier->aiData.usActionData = pSoldier->sGridNo + DirectionInc( pSoldier->pathing.usPathingData[0] );
					pSoldier->pathing.sFinalDestination = pSoldier->aiData.usActionData;
					pSoldier->aiData.bNextAction = AI_ACTION_END_TURN;
					return( AI_ACTION_GET_CLOSER );
				}
			}
		}
		// otherwise do nothing
		return( AI_ACTION_NONE );
	}

	////////////////////////////////////////////////////////////////////////////
	// IF A LOCATION WITH BETTER COVER IS AVAILABLE & REACHABLE, GO FOR IT!
	////////////////////////////////////////////////////////////////////////////

	if (sBestCover != NOWHERE)
	{
#ifdef DEBUGDECISIONS
		std::string tempstr = String("%s - TAKING COVER at gridno %d (%d%% better)\n",
			pSoldier->name,sBestCover,iCoverPercentBetter);
		DebugAI( tempstr ) ;
#endif
		//ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_TESTVERSION, L"AI %d taking cover, morale %d, from %d to %d", pSoldier->ubID, pSoldier->aiData.bAIMorale, pSoldier->sGridNo, sBestCover );
		pSoldier->aiData.usActionData = sBestCover;

		return(AI_ACTION_TAKE_COVER);
	}


	////////////////////////////////////////////////////////////////////////////
	// IF THINGS ARE REALLY HOPELESS, OR UNARMED, TRY TO RUN AWAY
	////////////////////////////////////////////////////////////////////////////

	// if soldier has enough APs left to move at least 1 square's worth
	if ( flags.ubCanMove && (pSoldier->bTeam != gbPlayerNum || pSoldier->aiData.fAIFlags & AI_RTP_OPTION_CAN_RETREAT) )
	{
		if ((pSoldier->aiData.bAIMorale == MORALE_HOPELESS) || !flags.bCanAttack)
		{
			// look for best place to RUN AWAY to (farthest from the closest threat)
			//pSoldier->aiData.usActionData = RunAway( pSoldier );
			pSoldier->aiData.usActionData = FindSpotMaxDistFromOpponents(pSoldier);

			if (pSoldier->aiData.usActionData != NOWHERE)
			{
#ifdef DEBUGDECISIONS
				tempstr = String("%s - RUNNING AWAY to grid %d",pSoldier->name,pSoldier->aiData.usActionData);
				DebugAI(tempstr);
#endif

				return(AI_ACTION_RUN_AWAY);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// IF SPOTTERS HAVE BEEN CALLED FOR, AND WE HAVE SOME NEW SIGHTINGS, RADIO!
	////////////////////////////////////////////////////////////////////////////

	// if we're a computer merc, and we have the action points remaining to RADIO
	// (we never want NPCs to choose to radio if they would have to wait a turn)
	// and we're not swimming in deep water, and somebody has called for spotters
	// and we see the location of at least 2 opponents
	if ((gTacticalStatus.ubSpottersCalledForBy != NOBODY) && (pSoldier->bActionPoints >= AP_RADIO) &&
		(pSoldier->aiData.bOppCnt > 1) && !flags.fCivilian &&
		(gTacticalStatus.Team[pSoldier->bTeam].bMenInSector > 1) && !flags.bInDeepWater)
	{
		// base chance depends on how much new info we have to radio to the others
		iChance = 25 * WhatIKnowThatPublicDont(pSoldier,TRUE);	// just count them

		// if I actually know something they don't
		if (iChance)
		{
#ifdef DEBUGDECISIONS
			AINumMessage("Chance to radio for SPOTTING = ",iChance);
#endif

			if ((INT16)PreRandom(100) < iChance)
			{
#ifdef DEBUGDECISIONS
				AINameMessage(pSoldier,"decides to radio a RED for SPOTTING!",1000);
#endif

				return(AI_ACTION_RED_ALERT);
			}
		}
	}


	////////////////////////////////////////////////////////////////////////////
	// CROUCH IF NOT CROUCHING ALREADY
	////////////////////////////////////////////////////////////////////////////

	// if not in water and not already crouched, try to crouch down first
	if (!gfTurnBasedAI || GetAPsToChangeStance( pSoldier, ANIM_CROUCH ) <= pSoldier->bActionPoints)
	{
		if ( !flags.fCivilian && !gfHiddenInterrupt && IsValidStance( pSoldier, ANIM_CROUCH ) )
		{
			pSoldier->aiData.usActionData = StanceChange( pSoldier, flags.BestAttack.ubAPCost );
			if (pSoldier->aiData.usActionData != 0)
			{
				return BlackAlert_ChangeStance(pSoldier, flags);
			}
		}
	}

	////////////////////////////////////////////////////////////////////////////
	// TURN TO FACE CLOSEST KNOWN OPPONENT (IF NOT FACING THERE ALREADY)
	////////////////////////////////////////////////////////////////////////////

	if (!gfTurnBasedAI || GetAPsToLook( pSoldier ) <= pSoldier->bActionPoints)
	{
		bActionReturned = BlackAlert_TryToFaceEnemy(pSoldier, flags);
		if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
			return(bActionReturned);
	}

	// if a militia has absofreaking nothing else to do, maybe they should radio in a report!

	////////////////////////////////////////////////////////////////////////////
	// RADIO RED ALERT: determine %chance to call others and report contact
	////////////////////////////////////////////////////////////////////////////

	if ( pSoldier->bTeam == MILITIA_TEAM && (pSoldier->bActionPoints >= AP_RADIO) && (gTacticalStatus.Team[pSoldier->bTeam].bMenInSector > 1) )
	{
		bActionReturned = BlackAlert_TryToRadioRedAlert(pSoldier, flags);
		if (bActionReturned != AI_ACTION_NOT_AN_ACTION)
			return(bActionReturned);
	}

	////////////////////////////////////////////////////////////////////////////
	// LEAVE THE SECTOR
	////////////////////////////////////////////////////////////////////////////

	// NOT IMPLEMENTED

	////////////////////////////////////////////////////////////////////////////
	// DO NOTHING: Not enough points left to move, so save them for next turn
	////////////////////////////////////////////////////////////////////////////

#ifdef DEBUGDECISIONS
	AINameMessage(pSoldier,"- DOES NOTHING (BLACK)",1000);
#endif

	// by default, if everything else fails, just stand in place and wait
	pSoldier->aiData.usActionData = NOWHERE;
	return(AI_ACTION_NONE);

}

INT8 DecideAction(SOLDIERTYPE *pSoldier)
{
	PERFORMANCE_MARKER
	INT8 bAction=AI_ACTION_NONE;
#ifdef DEBUGDECISIONS
	std::string tempstr;
#endif

#ifdef AI_TIMING_TESTS
	UINT32		uiStartTime, uiEndTime;
#endif

	if ( pSoldier->bTeam != MILITIA_TEAM )
	{
		if ( !sniperwarning && pSoldier->aiData.bOrders == SNIPER )
		{
			ScreenMsg( FONT_MCOLOR_LTYELLOW, MSG_INTERFACE, L"Watch out for snipers..." );
			sniperwarning = TRUE;
		}

		if (!biggunwarning && FindRocketLauncherOrCannon(pSoldier) != NO_SLOT )
		{
			biggunwarning = TRUE;
			//TODO: don't say this again after reloading a savegame
			SayQuoteFromAnyBodyInSector( QUOTE_WEARY_SLASH_SUSPUCIOUS );
		}
	}
	// turn off cautious flag
	pSoldier->aiData.fAIFlags &= (~AI_CAUTIOUS);
	//reset flank count

	//NumMessage("DecideAction - Guy#",pSoldier->ubID);

	// if the NPC is being "ESCORTED" (seen civilians only for now)
	if (pSoldier->aiData.bUnderEscort)
	{
#ifdef DEBUGDECISIONS
		AIPopMessage("AlertStatus = ESCORT");
#endif

		//bAction = DecideActionEscort(pSoldier);
	}
	else // "normal" NPC AI
	{
		// if status over-ride is set, bypass RED/YELLOW and go directly to GREEN!
		if ((pSoldier->aiData.bBypassToGreen) && (pSoldier->aiData.bAlertStatus < STATUS_BLACK))
		{
			bAction = DecideActionGreen(pSoldier);
			if ( !gfTurnBasedAI )
			{
				// reset bypass now
				pSoldier->aiData.bBypassToGreen = 0;
			}
		}
		else
		{
			switch (pSoldier->aiData.bAlertStatus)
			{
			case STATUS_GREEN:
#ifdef DEBUGDECISIONS
				AIPopMessage("AlertStatus = GREEN");
#endif
#ifdef AI_TIMING_TESTS
				uiStartTime = GetJA2Clock();
#endif
				bAction = DecideActionGreen(pSoldier);
#ifdef AI_TIMING_TESTS
				uiEndTime = GetJA2Clock();
				guiGreenTimeTotal += (uiEndTime - uiStartTime);
				guiGreenCounter++;
#endif
				break;

			case STATUS_YELLOW:
#ifdef DEBUGDECISIONS
				AIPopMessage("AlertStatus = YELLOW");
#endif
#ifdef AI_TIMING_TESTS
				uiStartTime = GetJA2Clock();
#endif
				bAction = DecideActionYellow(pSoldier);
#ifdef AI_TIMING_TESTS
				uiEndTime = GetJA2Clock();
				guiYellowTimeTotal += (uiEndTime - uiStartTime);
				guiYellowCounter++;
#endif
				break;

			case STATUS_RED:
#ifdef DEBUGDECISIONS
				AIPopMessage("AlertStatus = RED");
#endif
#ifdef AI_TIMING_TESTS
				uiStartTime = GetJA2Clock();
#endif
				bAction = DecideActionRed(pSoldier,TRUE);
#ifdef AI_TIMING_TESTS
				uiEndTime = GetJA2Clock();
				guiRedTimeTotal += (uiEndTime - uiStartTime);
				guiRedCounter++;
#endif
				break;

			case STATUS_BLACK:
#ifdef DEBUGDECISIONS
				AIPopMessage("AlertStatus = BLACK");
#endif
#ifdef AI_TIMING_TESTS
				uiStartTime = GetJA2Clock();
#endif
				bAction = DecideActionBlack(pSoldier);
				DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("DecideActionBlack=%d",bAction));
#ifdef AI_TIMING_TESTS
				uiEndTime = GetJA2Clock();
				guiBlackTimeTotal += (uiEndTime - uiStartTime);
				guiBlackCounter++;
#endif
				break;
			}
		}
	}



#ifdef DEBUGDECISIONS
	tempstr = String("DecideAction: selected action %d, actionData %d\n\n",bAction,pSoldier->aiData.usActionData);
	DebugAI( tempstr );
#endif

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"DecideAction done");

	return(bAction);
}

INT8 DecideActionEscort(SOLDIERTYPE *pSoldier)
{
	PERFORMANCE_MARKER
#ifdef DEBUGDECISIONS
	std::string tempstr;
#endif

	// if he has a place to go, and isn't already there... go!
	if (pSoldier->aiData.usActionData != NOWHERE && (pSoldier->sGridNo != pSoldier->aiData.usActionData))
	{
#ifdef DEBUGDECISIONS
		tempstr = String("%s - ESCORTED NPC GOING to grid %d",pSoldier->name,pSoldier->aiData.usActionData);
		DebugAI(tempstr);
#endif

		return(AI_ACTION_ESCORTED_MOVE);
	}
	else
		return(AI_ACTION_NONE);
}


void DecideAlertStatus( SOLDIERTYPE *pSoldier )
{
	PERFORMANCE_MARKER
#ifdef DEBUGDECISIONS
	std::string tempstr;
#endif
	INT8	bOldStatus;
	INT32	iDummy;
	BOOLEAN fClimbDummy,fReachableDummy;

	// THE FOUR (4) POSSIBLE ALERT STATUSES ARE:
	// GREEN - No one seen, no suspicious noise heard, go about regular duties
	// YELLOW - Suspicious noise was heard personally or radioed in by buddy
	// RED - Either saw opponents in person, or definite contact had been radioed
	// BLACK - Currently has one or more opponents in sight

	// save the man's previous status

	if (pSoldier->flags.uiStatusFlags & SOLDIER_MONSTER)
	{
		CreatureDecideAlertStatus( pSoldier );
		return;
	}

	bOldStatus = pSoldier->aiData.bAlertStatus;

	// determine the current alert status for this category of man
	//if (!(pSoldier->flags.uiStatusFlags & SOLDIER_PC))
	{
		if (pSoldier->aiData.bOppCnt > 0)        // opponent(s) in sight
		{
			pSoldier->aiData.bAlertStatus = STATUS_BLACK;
			CheckForChangingOrders( pSoldier );
		}
		else                        // no opponents are in sight
		{
			switch (bOldStatus)
			{
			case STATUS_BLACK:
				// then drop back to RED status
				pSoldier->aiData.bAlertStatus = STATUS_RED;
				break;

			case STATUS_RED:
				// RED can never go back down below RED, only up to BLACK
				break;

			case STATUS_YELLOW:
				// if all enemies have been RED alerted, or we're under fire
				if (!PTR_CIVILIAN && (gTacticalStatus.Team[pSoldier->bTeam].bAwareOfOpposition || pSoldier->aiData.bUnderFire))
				{
					pSoldier->aiData.bAlertStatus = STATUS_RED;
				}
				else
				{
					// if we are NOT aware of any uninvestigated noises right now
					// and we are not currently in the middle of an action
					// (could still be on his way heading to investigate a noise!)
					if ((MostImportantNoiseHeard(pSoldier,&iDummy,&fClimbDummy,&fReachableDummy) == NOWHERE) && !pSoldier->aiData.bActionInProgress)
					{
						// then drop back to GREEN status
						pSoldier->aiData.bAlertStatus = STATUS_GREEN;
						CheckForChangingOrders( pSoldier );
					}
				}
				break;

			case STATUS_GREEN:
				// if all enemies have been RED alerted, or we're under fire
				if (!PTR_CIVILIAN && (gTacticalStatus.Team[pSoldier->bTeam].bAwareOfOpposition || pSoldier->aiData.bUnderFire))
				{
					pSoldier->aiData.bAlertStatus = STATUS_RED;
				}
				else
				{
					// if we ARE aware of any uninvestigated noises right now
					if (MostImportantNoiseHeard(pSoldier,&iDummy,&fClimbDummy,&fReachableDummy) != NOWHERE)
					{
						// then move up to YELLOW status
						pSoldier->aiData.bAlertStatus = STATUS_YELLOW;
					}
				}
				break;
			}
			// otherwise, RED stays RED, YELLOW stays YELLOW, GREEN stays GREEN
		}
	}

#if 0
	else
	{
		if (pSoldier->aiData.bOppCnt > 0)
		{
			pSoldier->aiData.bAlertStatus = STATUS_BLACK; // opponent(s) in sight
		}
		else
		{
			pSoldier->aiData.bAlertStatus = STATUS_RED;         // enemy sector

			/*
			// wow, JA1 stuff...
			// good guys all have a built-in, magic, "enemy detecting radar"...
			if (Status.enemies)
			pSoldier->aiData.bAlertStatus = STATUS_RED;         // enemy sector
			else
			{
			pSoldier->aiData.bAlertStatus = STATUS_GREEN;       // secured sector

			// if he just dropped back from alert status, and it's a GUARD
			if ((oldStatus >= STATUS_RED) && (pSoldier->manCategory == MAN_GUARD))
			{
			if (pSoldier->whereIWas == NOWHERE)       // not assigned to any trees
			// FUTURE ENHANCEMENT: Look for unguarded trees with tappers
			pSoldier->orders = ONCALL;
			else                                 // assigned to trees
			// FUTURE ENHANCEMENT: If his tree is now tapperless, go ONCALL
			pSoldier->orders = CLOSEPATROL;         // go back to his tree area

			// turn off any existing bypass to Green and its "hyper-activity"
			pSoldier->bypassToGreen = FALSE;

			// turn off the "inTheWay" flag, may have been set during TurnBased
			pSoldier->inTheWay = FALSE;

			// make the guard put his gun away if he has it drawn
			HandleNoMoreTarget(pSoldier);
			}
			}
			*/
		}
	}
#endif

	if ( gTacticalStatus.bBoxingState == NOT_BOXING )
	{

		// if the man's alert status has changed in any way
		if (pSoldier->aiData.bAlertStatus != bOldStatus)
		{
			// HERE ARE TRYING TO AVOID NPCs SHUFFLING BACK & FORTH BETWEEN RED & BLACK
			// if either status is < RED (ie. anything but RED->BLACK && BLACK->RED)
			if ((bOldStatus < STATUS_RED) || (pSoldier->aiData.bAlertStatus < STATUS_RED))
			{
				// force a NEW action decision on next pass through HandleManAI()
				SetNewSituation( pSoldier );
			}

			// if this guy JUST discovered that there were opponents here for sure...
			if ((bOldStatus < STATUS_RED) && (pSoldier->aiData.bAlertStatus >= STATUS_RED))
			{
				CheckForChangingOrders(pSoldier);
			}

#ifdef DEBUGDECISIONS
			// don't report status changes for human-controlled mercs
			tempstr = String("%s's Alert Status changed from %d to %d",
				pSoldier->name,bOldStatus,pSoldier->aiData.bAlertStatus);
			DebugAI(tempstr);
#endif

		}
		else   // status didn't change
		{
			// only do this stuff in TB
			// if a guy on status GREEN or YELLOW is running low on breath
			if (((pSoldier->aiData.bAlertStatus == STATUS_GREEN)  && (pSoldier->bBreath < 75)) ||
				((pSoldier->aiData.bAlertStatus == STATUS_YELLOW) && (pSoldier->bBreath < 50)))
			{
				// as long as he's not in water (standing on a bridge is OK)
				if (!pSoldier->MercInWater())
				{
					// force a NEW decision so that he can get some rest
					SetNewSituation( pSoldier );

					// current action will be canceled. if noise is no longer important
					if ((pSoldier->aiData.bAlertStatus == STATUS_YELLOW) &&
						(MostImportantNoiseHeard(pSoldier,&iDummy,&fClimbDummy,&fReachableDummy) == NOWHERE))
					{
						// then drop back to GREEN status
						pSoldier->aiData.bAlertStatus = STATUS_GREEN;
						CheckForChangingOrders( pSoldier );
					}
				}
			}
		}
	}

}
