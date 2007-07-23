










#if 0


#define VEHICLE




INT32 FindBestPath(SOLDIERTYPE *s , INT16 sDestination, INT8 ubLevel, INT16 usMovementMode, INT8 bCopy, UINT8 fFlags )
{
	//__try
	//{
	INT32 iDestination = sDestination, iOrigination;
	INT32 iCnt=-1, iStructIndex;
	INT32 iLoopStart = 0, iLoopEnd = 0;
	INT8	bLoopState = LOOPING_CLOCKWISE;
	//BOOLEAN fLoopForwards = FALSE;
	BOOLEAN	fCheckedBehind = FALSE;
	UINT8	ubMerc;
	INT32 iDestX,iDestY, iLocX, iLocY, dx, dy;
	INT32	newLoc,curLoc;
	//INT32 curY;
	INT32 curCost,newTotCost,nextCost;
	INT16	sCurPathNdx;
	INT32 prevCost;
	INT32 iWaterToWater;
	UINT8 ubCurAPCost,ubAPCost;
	UINT8 ubNewAPCost=0;
	#ifdef VEHICLE
		//BOOLEAN fTurnSlow = FALSE;
		//BOOLEAN fReverse = FALSE; // stuff for vehicles turning
		BOOLEAN fMultiTile, fVehicle;
		//INT32 iLastDir, iPrevToLastDir;
		//INT8 bVehicleCheckDir;
		//UINT16 adjLoc;
		STRUCTURE_FILE_REF * pStructureFileRef=NULL;
		UINT16							 usAnimSurface;
		//INT32 iCnt2, iCnt3;
	#endif

	INT32			iLastDir = 0;

	path_t *	pNewPtr;
	path_t *	pCurrPtr;

	path_t *	pUpdate[ABSMAX_SKIPLIST_LEVEL];
	path_t *	pCurr;
	path_t *	pNext;
	path_t *	pDel;
	UINT32		uiCost;
	INT32			iCurrLevel, iLoop;

	BOOLEAN		fHiddenStructVisible;				// Used for hidden struct visiblity
	UINT16		usOKToAddStructID=0;

	BOOLEAN		fCopyReachable;
	BOOLEAN		fCopyPathCosts;
	BOOLEAN		fVisitSpotsOnlyOnce;
	INT32			iOriginationX, iOriginationY, iX, iY;

	BOOLEAN		fTurnBased;
	BOOLEAN		fPathingForPlayer;
	INT32			iDoorGridNo=-1;
	BOOLEAN		fDoorIsObstacleIfClosed= 0; // if false, door is obstacle if it is open
	DOOR_STATUS *	pDoorStatus;
	DOOR *				pDoor;
	STRUCTURE *		pDoorStructure;
	BOOLEAN		fDoorIsOpen = FALSE;
	BOOLEAN		fNonFenceJumper;
	BOOLEAN		fNonSwimmer;
	BOOLEAN		fPathAroundPeople;
	BOOLEAN		fConsiderPersonAtDestAsObstacle;
	BOOLEAN		fGoingThroughDoor = FALSE; // for one tile
	BOOLEAN		fContinuousTurnNeeded;
	BOOLEAN		fCloseGoodEnough;
  UINT16    usMovementModeToUseForAPs;
	INT16			sClosePathLimit;

#ifdef PATHAI_SKIPLIST_DEBUG
	CHAR8				zTempString[1000], zTS[50];
#endif

#ifdef PATHAI_VISIBLE_DEBUG
	UINT16		usCounter = 0;
#endif

	fVehicle = FALSE;
	iOriginationX = iOriginationY = 0;
	iOrigination = (INT32) s->sGridNo;

	if (iOrigination < 0 || iOrigination > WORLD_MAX)
	{
		#ifdef JA2BETAVERSION
			ScreenMsg( FONT_MCOLOR_RED, MSG_TESTVERSION, L"ERROR!  Trying to calculate path from off-world gridno %d to %d", iOrigination, sDestination );
		#endif
		return( 0 );
	}
	else if (!GridNoOnVisibleWorldTile( (INT16) iOrigination ) )
	{
		#ifdef JA2BETAVERSION
			ScreenMsg( FONT_MCOLOR_RED, MSG_TESTVERSION, L"ERROR!  Trying to calculate path from non-visible gridno %d to %d", iOrigination, sDestination );
		#endif
		return( 0 );
	}
	else if (s->pathing.bLevel != ubLevel)
	{
		// pathing to a different level... bzzzt!
		return( 0 );
	}

	if ( gubGlobalPathFlags )
	{
		fFlags |= gubGlobalPathFlags;	
	}

	fTurnBased = ( (gTacticalStatus.uiFlags & TURNBASED) && (gTacticalStatus.uiFlags & INCOMBAT) );

	fPathingForPlayer = ( (s->bTeam == gbPlayerNum) && (!gTacticalStatus.fAutoBandageMode) && !(s->flags.uiStatusFlags & SOLDIER_PCUNDERAICONTROL) );
	fNonFenceJumper = !( IS_MERC_BODY_TYPE( s ) );
	fNonSwimmer = !( IS_MERC_BODY_TYPE( s ) );
	if ( fNonSwimmer )
	{
		if ( Water( sDestination ) )
		{
			return( 0 );
		}
	}
	fPathAroundPeople = ( (fFlags & PATH_THROUGH_PEOPLE) == 0 );
	fCloseGoodEnough = ( (fFlags & PATH_CLOSE_GOOD_ENOUGH) != 0);
	if ( fCloseGoodEnough )
	{
		sClosePathLimit = __min( PythSpacesAway( (INT16)s->sGridNo, sDestination ) - 1,  PATH_CLOSE_RADIUS );
		if ( sClosePathLimit <= 0 )
		{
			return( 0 );
		}
	}

	fConsiderPersonAtDestAsObstacle = (BOOLEAN)( fPathingForPlayer && fPathAroundPeople && !(fFlags & PATH_IGNORE_PERSON_AT_DEST) );

	if (bCopy >= COPYREACHABLE)
	{
		fCopyReachable = TRUE;
		fCopyPathCosts = (bCopy == COPYREACHABLE_AND_APS);
		fVisitSpotsOnlyOnce = (bCopy == COPYREACHABLE);
		// make sure we aren't trying to copy path costs for an area greater than the AI array...
		if (fCopyPathCosts && gubNPCDistLimit > AI_PATHCOST_RADIUS)
		{
			// oy!!!! dis no supposed to happen!
			gubNPCDistLimit = AI_PATHCOST_RADIUS;
		}
	}
	else
	{
		fCopyReachable = FALSE;
		fCopyPathCosts = FALSE;
		fVisitSpotsOnlyOnce = FALSE;
	}

	gubNPCPathCount++;

	if (gubGlobalPathCount == 255)
	{
		// reset arrays!
		memset( trailCostUsed, 0, MAPLENGTH );
		gubGlobalPathCount = 1;
	}
	else
	{
		gubGlobalPathCount++;
	}

	// only allow nowhere destination if distance limit set
	if (sDestination == NOWHERE)
	{
		/*
		if (gubNPCDistLimit == 0)
		{
			return( FALSE );
		}	
		*/	 
	}
	else
	{
		// the very first thing to do is make sure the destination tile is reachable
		if (!NewOKDestination( s, sDestination, fConsiderPersonAtDestAsObstacle, ubLevel ))
		{
			gubNPCAPBudget = 0;
			gubNPCDistLimit = 0;
			return( FALSE );
		}

		if (sDestination == s->sGridNo)
		{
			return( FALSE );
		}
	}

	if (gubNPCAPBudget)
	{
		ubAPCost = MinAPsToStartMovement( s, usMovementMode );
		if (ubAPCost > gubNPCAPBudget)
		{
			gubNPCAPBudget = 0;
			gubNPCDistLimit = 0;
			return( 0 );
		}
		else
		{
			gubNPCAPBudget -= ubAPCost;
		}
	}
	
#ifdef COUNT_PATHS
	guiTotalPathChecks++;
#endif

#ifdef VEHICLE	

	fMultiTile = ((s->flags.uiStatusFlags & SOLDIER_MULTITILE) != 0);
	if (fMultiTile)
	{
		// Get animation surface...
		// Chris_C... change this to use parameter.....
		usAnimSurface = DetermineSoldierAnimationSurface( s, usMovementMode );
		// Get structure ref...
		pStructureFileRef = GetAnimationStructureRef( s->ubID, usAnimSurface, usMovementMode );
		
		if ( pStructureFileRef )
		{
			fVehicle = ( (s->flags.uiStatusFlags & SOLDIER_VEHICLE) != 0 );

			fContinuousTurnNeeded = ( ( s->flags.uiStatusFlags & (SOLDIER_MONSTER | SOLDIER_ANIMAL | SOLDIER_VEHICLE) ) != 0 );

			/*
			if (fVehicle && s->bReverse)
			{
				fReverse = TRUE;
			}
			*/
			/*
			if (fVehicle || s->ubBodyType == COW || s->ubBodyType == BLOODCAT) // or a vehicle
			{
				fTurnSlow = TRUE;
			}
			*/
			if ( gfEstimatePath )
			{
				usOKToAddStructID = IGNORE_PEOPLE_STRUCTURE_ID;
			}
			else if ( s->pLevelNode != NULL && s->pLevelNode->pStructureData != NULL )
			{
				usOKToAddStructID = s->pLevelNode->pStructureData->usStructureID;
			}
			else
			{
				usOKToAddStructID = INVALID_STRUCTURE_ID;
			}

		}
		else
		{
			// turn off multitile pathing
			fMultiTile = FALSE;
			fContinuousTurnNeeded = FALSE;
		}

	}
	else
	{
		fContinuousTurnNeeded = FALSE;
	}

#endif

	if (!fContinuousTurnNeeded)
	{
		iLoopStart = 0;
		iLoopEnd = 0;
		bLoopState = LOOPING_CLOCKWISE;
	}

	ubCurAPCost = 0;
	queRequests = 2;

	//initialize the path data structures
	memset( pathQ, 0, iMaxPathQ * sizeof( path_t ) );
	memset( trailTree, 0, iMaxTrailTree * sizeof( trail_t ) );

#if defined( PATHAI_VISIBLE_DEBUG )
	if (gfDisplayCoverValues && gfDrawPathPoints)
	{
		memset( gsCoverValue, 0x7F, sizeof( INT16 ) * WORLD_MAX );
	}
#endif

	bSkipListLevel = 1;
	iSkipListSize = 0;
	iClosedListSize = 0;

	trailTreeNdx=0;

	//set up common info
	if (fCopyPathCosts)
	{
		iOriginationY = (iOrigination / MAPWIDTH);
		iOriginationX = (iOrigination % MAPWIDTH);
	}
	
	iDestY = (iDestination / MAPWIDTH);
	iDestX = (iDestination % MAPWIDTH);
	
	// if origin and dest is water, then user wants to stay in water!
	// so, check and set waterToWater flag accordingly
	if (iDestination == NOWHERE)
	{
		iWaterToWater = 0;
	}
	else
	{
		if (ISWATER(gubWorldMovementCosts[iOrigination][0][ubLevel]) && ISWATER(gubWorldMovementCosts[iDestination][0][ubLevel]))
				iWaterToWater = 1;
		else
				iWaterToWater = 0;
	}

	//setup Q and first path record

	SETLOC( *pQueueHead, iOrigination );
	pQueueHead->usCostSoFar	= MAXCOST;
	pQueueHead->bLevel			= iMaxSkipListLevel - 1;

	pClosedHead->pNext[0] = pClosedHead;
	pClosedHead->pNext[1] = pClosedHead;

	//setup first path record
	iLocY = iOrigination / MAPWIDTH;
	iLocX = iOrigination % MAPWIDTH;

	SETLOC( pathQ[1], iOrigination );
	pathQ[1].sPathNdx						= 0;
	pathQ[1].usCostSoFar				= 0;
	if ( fCopyReachable )
	{
		pathQ[1].usCostToGo					= 100;
	}
	else
	{
		pathQ[1].usCostToGo					= REMAININGCOST( &(pathQ[1]) );
	}
	pathQ[1].usTotalCost				= pathQ[1].usCostSoFar + pathQ[1].usCostToGo;
	pathQ[1].ubLegDistance			= LEGDISTANCE( iLocX, iLocY, iDestX, iDestY );
	pathQ[1].bLevel							= 1;
	pQueueHead->pNext[0] = &( pathQ[1] );
	iSkipListSize++;

	trailTreeNdx						=	0;
	trailCost[iOrigination]	=	0;
	pCurrPtr											= pQueueHead->pNext[0];
	pCurrPtr->sPathNdx						= trailTreeNdx;
	trailTreeNdx++;


	do
	{
		//remove the first and best path so far from the que
		pCurrPtr				= pQueueHead->pNext[0];
		curLoc		= pCurrPtr->iLocation;
		curCost		= pCurrPtr->usCostSoFar;
		sCurPathNdx = pCurrPtr->sPathNdx;

		// remember the cost used to get here...
		prevCost = gubWorldMovementCosts[ trailTree[ sCurPathNdx ].sGridNo ][ trailTree[ sCurPathNdx ].stepDir ][ ubLevel ]; 

#if defined( PATHAI_VISIBLE_DEBUG )
		if (gfDisplayCoverValues && gfDrawPathPoints)
		{
			if (gsCoverValue[ curLoc ] > 0)
			{
				gsCoverValue[ curLoc ] *= -1;
			}
		}
#endif

#ifdef VEHICLE
		/*
		if (fTurnSlow)
		{
			if (pCurrPtr->pNext[0] == 0)
			{
				if (fReverse)
				{
					iLastDir = gOppositeDirection[s->bDirection];
				}
				else
				{
					iLastDir = s->bDirection;
				}
				// start prev-to-last dir at same as current (could cause a problem)
				iPrevToLastDir = iLastDir;
			}
			else 
			{
				iPrevToLastDir = trailTree[trailTree[pCurrPtr->sPathNdx].nextLink].dirDelta;
				iLastDir = trailTree[pCurrPtr->sPathNdx].dirDelta;
			}

		}
		*/
#endif

		if (gubNPCAPBudget)
		{
			ubCurAPCost = pCurrPtr->ubTotalAPCost;
		}
		if (fCopyReachable && prevCost != TRAVELCOST_FENCE)
		{
			gpWorldLevelData[curLoc].uiFlags |= MAPELEMENT_REACHABLE;
			if (gubBuildingInfoToSet > 0)
			{
				gubBuildingInfo[ curLoc ] = gubBuildingInfoToSet;
			}
		}

		DELQUENODE( pCurrPtr );
				
		if ( trailCostUsed[curLoc] == gubGlobalPathCount && trailCost[curLoc] < curCost)
			goto NEXTDIR;

		//DebugMsg( TOPIC_JA2, DBG_LEVEL_3, String( "PATHAI %d", curLoc ) );

		if (fContinuousTurnNeeded)
		{
			if (trailTreeNdx < 2)
			{
				iLastDir = s->bDirection;
			}
			else if ( trailTree[pCurrPtr->sPathNdx].fFlags & STEP_BACKWARDS )
			{
				iLastDir = gOppositeDirection[ trailTree[pCurrPtr->sPathNdx].stepDir ];
			}
			else
			{
				iLastDir = trailTree[pCurrPtr->sPathNdx].stepDir;
			}
			iLoopStart = iLastDir;
			iLoopEnd = iLastDir;
			bLoopState = LOOPING_CLOCKWISE;
			fCheckedBehind = FALSE;
		}

		//contemplate a new path in each direction
		//for ( iCnt = iLoopStart; iCnt != iLoopEnd; iCnt = (iCnt + iLoopIncrement) % MAXDIR )
		for ( iCnt = iLoopStart; ; )
		{

#ifdef VEHICLE
			/*
			if (fTurnSlow)
			{
				if (iLastDir == iPrevToLastDir)
				{
					if ( iCnt != iLastDir && iCnt != gOneCDirection[ iLastDir ] && iCnt != gOneCCDirection[ iLastDir ])
					{
						goto NEXTDIR;
					}
				}
				else
				{
					if ( iCnt != iLastDir )
					{
						goto NEXTDIR;
					}
				}
			}
			*/

			if ( bLoopState == LOOPING_REVERSE )
			{
				iStructIndex = gOppositeDirection[ gOneCDirection[ iCnt ] ];
			}
			else
			{
				iStructIndex = gOneCDirection[ iCnt ];
			}

			if (fMultiTile)
			{
				if ( fContinuousTurnNeeded )				
				{
					if ( iCnt != iLastDir )
					{
						if ( !OkayToAddStructureToWorld( (INT16) curLoc, ubLevel, &(pStructureFileRef->pDBStructureRef[ iStructIndex ]), usOKToAddStructID ) )
						{
							// we have to abort this loop and possibly reset the loop conditions to
							// search in the other direction (if we haven't already done the other dir)
							if (bLoopState == LOOPING_CLOCKWISE)
							{
								iLoopStart = iLastDir;
								iLoopEnd = iCnt;
								bLoopState = LOOPING_COUNTERCLOCKWISE; // backwards
								// when we go to the bottom of the loop, iLoopIncrement will be added to iCnt
								// which is good since it avoids duplication of effort
								iCnt = iLoopStart;
								goto NEXTDIR;
							}
							else if ( bLoopState == LOOPING_COUNTERCLOCKWISE && !fCheckedBehind )
							{
								// check rear dir
								bLoopState = LOOPING_REVERSE;

								// NB we're stuck with adding 1 to the loop counter down below so configure to accomodate...
								//iLoopStart = (iLastDir + (MAXDIR / 2) - 1) % MAXDIR;
								iLoopStart = gOppositeDirection[ gOneCCDirection[ iLastDir ] ];
								iLoopEnd = (iLoopStart + 2) % MAXDIR;
								iCnt = iLoopStart;
								fCheckedBehind = TRUE;
								goto NEXTDIR;
							}
							else
							{
								// done
								goto ENDOFLOOP;
							}

						}
					}
				}
				else if ( pStructureFileRef )
				{
					// check to make sure it's okay for us to turn to the new direction in our current tile 
					if (!OkayToAddStructureToWorld( (INT16) curLoc, ubLevel, &(pStructureFileRef->pDBStructureRef[ iStructIndex ]), usOKToAddStructID ) )
					{
						goto NEXTDIR;
					}
				}
			}

#endif

			newLoc = curLoc + dirDelta[iCnt];


			if ( fVisitSpotsOnlyOnce && trailCostUsed[newLoc] == gubGlobalPathCount )
			{
				// on a "reachable" test, never revisit locations!
				goto NEXTDIR;
			}

			
			//if (gpWorldLevelData[newLoc].sHeight != ubLevel)
			//ATE: Movement onto cliffs? Check vs the soldier's gridno height
			// CJC: PREVIOUS LOCATION's height
			if ( gpWorldLevelData[ newLoc ].sHeight != gpWorldLevelData[ curLoc ].sHeight )
			{
				goto NEXTDIR;
			}

			if (gubNPCDistLimit)
			{
				if ( gfNPCCircularDistLimit )
				{
					if (PythSpacesAway( (INT16) iOrigination, (INT16) newLoc) > gubNPCDistLimit)
					{
						goto NEXTDIR;
					}
				}
				else
				{
					if (SpacesAway( (INT16) iOrigination, (INT16) newLoc) > gubNPCDistLimit)
					{
						goto NEXTDIR;
					}				
				}
			}

			// AI check for mines
			if ( gpWorldLevelData[newLoc].uiFlags & MAPELEMENT_ENEMY_MINE_PRESENT && s->bSide != 0)
			{
				goto NEXTDIR;
			}

			// WANNE: Know mines (for enemy or player) do not explode - BEGIN
			if ( gpWorldLevelData[newLoc].uiFlags & (MAPELEMENT_ENEMY_MINE_PRESENT | MAPELEMENT_PLAYER_MINE_PRESENT) )
			{
				if (s->bSide == 0)
				{
					// For our team, skip a location with a known mines unless it is the end of our
					// path; for others on our side, skip such locations completely;
					if (s->bTeam != gbPlayerNum || newLoc != iDestination)
					{
						if (gpWorldLevelData[newLoc].uiFlags & MAPELEMENT_PLAYER_MINE_PRESENT)
						{
							goto NEXTDIR;
						}
					}
				}
				else
				{
					// For the enemy, always skip known mines
					if (gpWorldLevelData[newLoc].uiFlags & MAPELEMENT_ENEMY_MINE_PRESENT)
					{
						goto NEXTDIR;
					}
				}
			}
			// WANNE: - END

			//how much is admission to the next tile
			if ( gfPathAroundObstacles )
			{
				nextCost = gubWorldMovementCosts[ newLoc ][ iCnt ][ ubLevel ];

				//ATE:	Check for differences from reality 
				// Is next cost an obstcale
				if ( nextCost == TRAVELCOST_HIDDENOBSTACLE )
				{
					if ( fPathingForPlayer )
					{
						// Is this obstcale a hidden tile that has not been revealed yet?
						if( DoesGridnoContainHiddenStruct( (UINT16)newLoc, &fHiddenStructVisible ) )
						{
							// Are we not visible, if so use terrain costs!
							if ( !fHiddenStructVisible )
							{
								// Set cost of terrain!
								nextCost = gTileTypeMovementCost[ gpWorldLevelData[ newLoc ].ubTerrainID ];
							}
						}
					}
				}
				else if ( nextCost == TRAVELCOST_NOT_STANDING )
				{
					// for path plotting purposes, use the terrain value
					nextCost = gTileTypeMovementCost[ gpWorldLevelData[ newLoc ].ubTerrainID ];
				}
				else if ( nextCost == TRAVELCOST_EXITGRID )
				{
					if (gfPlotPathToExitGrid)
					{
						// replace with terrain cost so that we can plot path, otherwise is obstacle
						nextCost = gTileTypeMovementCost[ gpWorldLevelData[ newLoc ].ubTerrainID ];
					}
				}
				else if ( nextCost == TRAVELCOST_FENCE && fNonFenceJumper )
				{
					goto NEXTDIR;
				}
				else if ( IS_TRAVELCOST_DOOR( nextCost ) )
				{

					// don't let anyone path diagonally through doors!
					if (iCnt & 1)
					{
						goto NEXTDIR;
					}

					switch( nextCost )
					{
						case TRAVELCOST_DOOR_CLOSED_HERE:
							fDoorIsObstacleIfClosed = TRUE;
							iDoorGridNo = newLoc;
							break;
						case TRAVELCOST_DOOR_CLOSED_N:
							fDoorIsObstacleIfClosed = TRUE;
							iDoorGridNo = newLoc + dirDelta[ NORTH ];
							break;
						case TRAVELCOST_DOOR_CLOSED_W:
							fDoorIsObstacleIfClosed = TRUE;
							iDoorGridNo = newLoc + dirDelta[ WEST ];
							break;
						case TRAVELCOST_DOOR_OPEN_HERE:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc;
							break;
						case TRAVELCOST_DOOR_OPEN_N:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ NORTH ];
							break;
						case TRAVELCOST_DOOR_OPEN_NE:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ NORTHEAST ];
							break;
						case TRAVELCOST_DOOR_OPEN_E:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ EAST ];
							break;
						case TRAVELCOST_DOOR_OPEN_SE:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ SOUTHEAST ];
							break;
						case TRAVELCOST_DOOR_OPEN_S:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ SOUTH ];
							break;
						case TRAVELCOST_DOOR_OPEN_SW:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ SOUTHWEST ];
							break;
						case TRAVELCOST_DOOR_OPEN_W:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ WEST ];
							break;
						case TRAVELCOST_DOOR_OPEN_NW:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ NORTHWEST ];
							break;
						case TRAVELCOST_DOOR_OPEN_N_N:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ NORTH ] + dirDelta[ NORTH ];
							break;
						case TRAVELCOST_DOOR_OPEN_NW_N:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ NORTHWEST ] + dirDelta[ NORTH ];
							break;
						case TRAVELCOST_DOOR_OPEN_NE_N:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ NORTHEAST ] + dirDelta[ NORTH ];
							break;
						case TRAVELCOST_DOOR_OPEN_W_W:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ WEST ] + dirDelta[ WEST ];
							break;
						case TRAVELCOST_DOOR_OPEN_SW_W:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ SOUTHWEST ] + dirDelta[ WEST ];
							break;
						case TRAVELCOST_DOOR_OPEN_NW_W:
							fDoorIsObstacleIfClosed = FALSE;
							iDoorGridNo = newLoc + dirDelta[ NORTHWEST ] + dirDelta[ WEST ];
							break;
						default:
							break;
					}

					if ( fPathingForPlayer && gpWorldLevelData[ iDoorGridNo ].ubExtFlags[0] & MAPELEMENT_EXT_DOOR_STATUS_PRESENT )
					{
						// check door status
						pDoorStatus = GetDoorStatus( (INT16) iDoorGridNo );
						if (pDoorStatus)
						{
							fDoorIsOpen = (pDoorStatus->ubFlags & DOOR_PERCEIVED_OPEN) != 0;
						}
						else
						{
							// door destroyed?
							nextCost = gTileTypeMovementCost[ gpWorldLevelData[ newLoc ].ubTerrainID ];
						}
					}
					else
					{
						// check door structure
						pDoorStructure = FindStructure( (INT16) iDoorGridNo, STRUCTURE_ANYDOOR );
						if (pDoorStructure)
						{
							fDoorIsOpen = (pDoorStructure->fFlags & STRUCTURE_OPEN) != 0;
						}
						else
						{
							// door destroyed?
							nextCost = gTileTypeMovementCost[ gpWorldLevelData[ newLoc ].ubTerrainID ];
						}
					}

					// now determine movement cost... if it hasn't been changed already
					if ( IS_TRAVELCOST_DOOR( nextCost ) )
					{

						if (fDoorIsOpen)
						{
							if (fDoorIsObstacleIfClosed)
							{
								nextCost = gTileTypeMovementCost[ gpWorldLevelData[ newLoc ].ubTerrainID ];
							}
							else
							{
								nextCost = TRAVELCOST_OBSTACLE;
							}
						}
						else
						{
							if (fDoorIsObstacleIfClosed)
							{				
								// door is closed and this should be an obstacle, EXCEPT if we are calculating
								// a path for an enemy or NPC with keys
								if ( fPathingForPlayer || ( s && (s->flags.uiStatusFlags & SOLDIER_MONSTER || s->flags.uiStatusFlags & SOLDIER_ANIMAL) ) )
								{
									nextCost = TRAVELCOST_OBSTACLE;
								}
								else
								{
									// have to check if door is locked and NPC does not have keys!
									pDoor = FindDoorInfoAtGridNo( iDoorGridNo );
									if (pDoor)
									{
										if (!pDoor->fLocked || s->pathing.bHasKeys)
										{
											// add to AP cost
											if (gubNPCAPBudget)
											{
												fGoingThroughDoor = TRUE;
											}
											nextCost = gTileTypeMovementCost[ gpWorldLevelData[ newLoc ].ubTerrainID ];
										}
										else
										{
											nextCost = TRAVELCOST_OBSTACLE;
										}
									}
									else
									{
										nextCost = gTileTypeMovementCost[ gpWorldLevelData[ newLoc ].ubTerrainID ];
									}
								}
							}
							else
							{
								nextCost = gTileTypeMovementCost[ gpWorldLevelData[ newLoc ].ubTerrainID ];
							}
						}
					}
				}
				else if ( (nextCost == TRAVELCOST_SHORE || nextCost == TRAVELCOST_KNEEDEEP || nextCost == TRAVELCOST_DEEPWATER ) && fNonSwimmer )
				{
					// creatures and animals can't go in water
					nextCost = TRAVELCOST_OBSTACLE;
				}
			

				// Apr. '96 - moved up be ahead of AP_Budget stuff
				if ( ( nextCost >= NOPASS) ) // || ( nextCost == TRAVELCOST_DOOR ) )
						goto NEXTDIR;

			}
			else
			{
				nextCost = TRAVELCOST_FLAT;	
			}

			if ( newLoc > GRIDSIZE )
			{
				// WHAT THE??? hack.
				goto NEXTDIR;
			}
			
			// if contemplated tile is NOT final dest and someone there, disqualify route
			// when doing a reachable test, ignore all locations with people in them
			if (fPathAroundPeople && ( (newLoc != iDestination) || fCopyReachable) )
			{
				 // ATE: ONLY cancel if they are moving.....
				ubMerc = WhoIsThere2( (UINT16) newLoc, s->pathing.bLevel);

				if ( ubMerc < NOBODY && ubMerc != s->ubID )
				{	
					// Check for movement....
					//if ( fTurnBased || ( (Menptr[ ubMerc ].sFinalDestination == Menptr[ ubMerc ].sGridNo) || (Menptr[ ubMerc ].fDelayedMovement) ) )
					//{
						goto NEXTDIR;	
					//}
				//	else
					//{
					//	nextCost += 50;
					//}
				}
			}

#ifdef VEHICLE
			if (fMultiTile)
			{
				// vehicle test for obstacles: prevent movement to next tile if
				// a tile covered by the vehicle in that position & direction
				// has an obstacle in it

				// because of the order in which animations are stored (dir 7 first,
				// then 0 1 2 3 4 5 6), we must subtract 1 from the direction
				// ATE: Send in our existing structure ID so it's ignored!

				if (!OkayToAddStructureToWorld( (INT16) newLoc, ubLevel, &(pStructureFileRef->pDBStructureRef[ iStructIndex ]), usOKToAddStructID ) )
				{
					goto NEXTDIR;
				}

				/*
				// vehicles aren't moving any more.... 
				if (fVehicle)
				{
					// transmogrify pathing costs for vehicles!
					switch(nextCost)
					{
						case TRAVELCOST_THICK		:	nextCost = TRAVELCOST_GRASS;
																			break;
						case TRAVELCOST_SHORE		: 
						case TRAVELCOST_KNEEDEEP:	   
						case TRAVELCOST_DEEPWATER:
//						case TRAVELCOST_VEINEND	:
//						case TRAVELCOST_VEINMID	:
						//case TRAVELCOST_DOOR		:
						case TRAVELCOST_FENCE		:	nextCost = TRAVELCOST_OBSTACLE;
																			break;

						default									:	break;
					}
				}
				*/
			}
#endif

			// NEW Apr 21 by Ian: abort if cost exceeds budget
			if (gubNPCAPBudget)
			 {
				switch(nextCost)
				{
					case TRAVELCOST_NONE		: ubAPCost = 0;
																		break;

					case TRAVELCOST_DIRTROAD:
					case TRAVELCOST_FLAT		: ubAPCost = AP_MOVEMENT_FLAT; 
																		break;
					//case TRAVELCOST_BUMPY	:		
					case TRAVELCOST_GRASS		: ubAPCost = AP_MOVEMENT_GRASS;
																		break;
					case TRAVELCOST_THICK		:	ubAPCost = AP_MOVEMENT_BUSH;
																		break;
					case TRAVELCOST_DEBRIS	: ubAPCost = AP_MOVEMENT_RUBBLE;
																		break;
					case TRAVELCOST_SHORE		: ubAPCost = AP_MOVEMENT_SHORE; // wading shallow water
																		break;
					case TRAVELCOST_KNEEDEEP:	ubAPCost = AP_MOVEMENT_LAKE; // wading waist/chest deep - very slow
																		break;

					case TRAVELCOST_DEEPWATER:ubAPCost = AP_MOVEMENT_OCEAN; // can swim, so it's faster than wading
																		break;
					//case TRAVELCOST_VEINEND	:
					//case TRAVELCOST_VEINMID	: ubAPCost = AP_MOVEMENT_FLAT;
					//													break;

					//case TRAVELCOST_DOOR		:	ubAPCost = AP_MOVEMENT_FLAT;
					//													break;

					case TRAVELCOST_FENCE		: ubAPCost = AP_JUMPFENCE;
			
					
																		break;					

					case TRAVELCOST_OBSTACLE: 
					default									:	goto NEXTDIR;	// Cost too much to be considered!
																		break;
				}

			  
			  // don't make the mistake of adding directly to
			  // ubCurAPCost, that must be preserved for remaining dirs!
				if (iCnt & 1)
				{
					//ubAPCost++;
					//ubAPCost = gubDiagCost[ubAPCost];
					ubAPCost = (ubAPCost * 14) / 10;
				}
				
				usMovementModeToUseForAPs = usMovementMode;

				// ATE: if water, force to be walking always!
				if ( nextCost == TRAVELCOST_SHORE || nextCost == TRAVELCOST_KNEEDEEP || nextCost == TRAVELCOST_DEEPWATER )
				{
				  usMovementModeToUseForAPs = WALKING;
				}

				// adjust AP cost for movement mode
				switch( usMovementModeToUseForAPs )
				{
					case RUNNING:	
					case ADULTMONSTER_WALKING:	
						// save on casting
						ubAPCost = ubAPCost * 10 / ( (UINT8) (RUNDIVISOR * 10));
						//ubAPCost = (INT16)(DOUBLE)( (sTileCost / RUNDIVISOR) );	break;
						break;
					case WALKING:
					case ROBOT_WALK:
						ubAPCost = (ubAPCost + WALKCOST);
						break;
					case SWATTING:
						ubAPCost = (ubAPCost + SWATCOST);
						break;
					case CRAWLING:
						ubAPCost = (ubAPCost + CRAWLCOST);
						break;
				}

				if (nextCost == TRAVELCOST_FENCE)
				{
					switch( usMovementModeToUseForAPs )
					{
						case RUNNING:	
						case WALKING :
							// Here pessimistically assume the path will continue after hopping the fence
							ubAPCost += AP_CROUCH;
							break;

						case SWATTING:	

							// Add cost to stand once there BEFORE jumping....
							ubAPCost += AP_CROUCH;
							break;

						case CRAWLING:	

							// Can't do it here.....
							goto NEXTDIR;
					}
				}
				else if (nextCost == TRAVELCOST_NOT_STANDING)
				{
					switch(usMovementModeToUseForAPs)
					{
						case RUNNING:	
						case WALKING :
							// charge crouch APs for ducking head!
							ubAPCost += AP_CROUCH;
							break;

						default:				
							break;
					}
				}
				else if (fGoingThroughDoor)
				{
					ubAPCost += AP_OPEN_DOOR;
					fGoingThroughDoor = FALSE;
				}


			  ubNewAPCost = ubCurAPCost + ubAPCost;


			  if (ubNewAPCost > gubNPCAPBudget)
			    goto NEXTDIR;

			 }//end if gubNPCAPBudget
			

			if ( fCloseGoodEnough )
			{
				if ( PythSpacesAway( (INT16)newLoc, sDestination ) <= sClosePathLimit )
				{
					// stop the path here!
					iDestination = newLoc;
					sDestination = (INT16) newLoc;
					fCloseGoodEnough = FALSE;
				}
			}
			//make the destination look very attractive
			if (newLoc == iDestination)
					nextCost = 0;
			else
				 //if (_KeyDown(CTRL_DOWN) && nextCost < TRAVELCOST_VEINEND)
				 if (gfPlotDirectPath && nextCost < NOPASS)
						nextCost = TRAVELCOST_FLAT;
				  


			//if (ISVEIN(prevCost))
			//		prevCost=VEINCOST;


			// make water cost attractive for water to water paths
			if (iWaterToWater)
			{
				if (ISWATER(prevCost))
					prevCost = EASYWATERCOST;
				if (ISWATER(nextCost))
					nextCost = EASYWATERCOST;
			}

// NOTE: on September 24, 1997, Chris went back to a diagonal bias system
			if (iCnt & 1)
			{
				// moving on a diagonal
				//nextCost = gubDiagCost[nextCost];
				nextCost = nextCost * 14 / 10;
				//nextCost++;
			}

			if ( bLoopState == LOOPING_REVERSE)
			{
				// penalize moving backwards to encourage turning sooner
				nextCost += 50;
			}

			newTotCost = curCost + nextCost;


			// have we found a path to the current location that
			// costs less than the best so far to the same location?
			if (trailCostUsed[newLoc] != gubGlobalPathCount || newTotCost < trailCost[newLoc])
			{
				//NEWQUENODE;				
				{
					if (queRequests<QPOOLNDX)
					{
						pNewPtr = pathQ + (queRequests);
						queRequests++;
						memset( pNewPtr->pNext, 0, sizeof( path_t *) * ABSMAX_SKIPLIST_LEVEL );
						pNewPtr->bLevel = RandomSkipListLevel();
					}
					else if (iClosedListSize > 0)
					{
						pNewPtr = pClosedHead->pNext[0];
						pClosedHead->pNext[0] = pNewPtr->pNext[0];
						iClosedListSize--;
						queRequests++;
						memset( pNewPtr->pNext, 0, sizeof( path_t *) * ABSMAX_SKIPLIST_LEVEL );
						pNewPtr->bLevel = RandomSkipListLevel();
					}
					else
					{
						pNewPtr = NULL;
					}
				}

				if (pNewPtr == NULL)
				{
					#ifdef COUNT_PATHS
						guiFailedPathChecks++;
					#endif
					gubNPCAPBudget = 0;
					gubNPCDistLimit = 0;
					return(0);
				}

				//make new path to current location
				trailTree[trailTreeNdx].nextLink	= sCurPathNdx;
				trailTree[trailTreeNdx].stepDir	= (INT8) iCnt;
				if ( bLoopState == LOOPING_REVERSE )
				{
					trailTree[trailTreeNdx].fFlags = STEP_BACKWARDS;
				}
				else
				{
					trailTree[trailTreeNdx].fFlags = 0;
				}
				trailTree[trailTreeNdx].sGridNo		= (INT16) newLoc;
				pNewPtr->sPathNdx						= trailTreeNdx;
				trailTreeNdx++;

				if (trailTreeNdx >= iMaxTrailTree)
				{
					#ifdef COUNT_PATHS
						guiFailedPathChecks++;
					#endif
					gubNPCAPBudget = 0;
					gubNPCDistLimit = 0;
					return(0);
				}

				iLocY = newLoc / MAPWIDTH;
				iLocX = newLoc % MAPWIDTH;
				SETLOC( *pNewPtr, newLoc );
				pNewPtr->usCostSoFar		= (UINT16) newTotCost;
				pNewPtr->usCostToGo			= (UINT16) REMAININGCOST(pNewPtr);
				if ( fCopyReachable )
				{
					pNewPtr->usCostToGo			= 100;
				}
				else
				{
					pNewPtr->usCostToGo			= (UINT16) REMAININGCOST(pNewPtr);
				}

				pNewPtr->usTotalCost		= newTotCost + pNewPtr->usCostToGo;
				pNewPtr->ubLegDistance	= LEGDISTANCE( iLocX, iLocY, iDestX, iDestY );
				
				if (gubNPCAPBudget)
				{
				  //save the AP cost so far along this path
				  pNewPtr->ubTotalAPCost = ubNewAPCost;
					// update the AP costs in the AI array of path costs if necessary...
					if (fCopyPathCosts)
					{
						iX = AI_PATHCOST_RADIUS + iLocX - iOriginationX;
						iY = AI_PATHCOST_RADIUS + iLocY - iOriginationY;
						gubAIPathCosts[iX][iY] = ubNewAPCost;
					}
				}

				//update the trail map to reflect the newer shorter path
				trailCost[newLoc] = (UINT16) newTotCost;
				trailCostUsed[newLoc] = gubGlobalPathCount;
			
			}

NEXTDIR:
			if (bLoopState == LOOPING_CLOCKWISE) // backwards
			{
				iCnt = gOneCCDirection[ iCnt ];
			}
			else
			{
				iCnt = gOneCDirection[ iCnt ];
			}
			if ( iCnt == iLoopEnd )
			{
ENDOFLOOP:
				break;
			}
			else if ( fContinuousTurnNeeded && iCnt == gOppositeDirection[ iLoopStart ] )
			{
				fCheckedBehind = TRUE;
			}
		}
	}
	while (pathQNotEmpty && pathNotYetFound);


	#if defined( PATHAI_VISIBLE_DEBUG )
		if (gfDisplayCoverValues && gfDrawPathPoints)
		{
			SetRenderFlags( RENDER_FLAG_FULL );
			if ( guiCurrentScreen == GAME_SCREEN )
			{
				RenderWorld();
				RenderCoverDebug( );
				InvalidateScreen( );
				EndFrameBufferRender();
				RefreshScreen( NULL );
			}
		}
	#endif

	
	// work finished. Did we find a path?
	if (pathQNotEmpty && pathFound)
	{
		INT16 z,_z,_nextLink; //,tempgrid;

		_z=0;
		z = (INT16) pQueueHead->pNext[0]->sPathNdx;
		
		while (z)
		{
			_nextLink = trailTree[z].nextLink;
			trailTree[z].nextLink = _z;
			_z = z;
			z = _nextLink;
		}

		// if this function was called because a solider is about to embark on an actual route
		// (as opposed to "test" path finding (used by cursor, etc), then grab all pertinent
		// data and copy into soldier's database
		if (bCopy == COPYROUTE)
		{
		  z=_z;
		  
			for (iCnt=0; z && (iCnt < MAX_PATH_LIST_SIZE); iCnt++)
		  {
			  s->pathing.usPathingData[iCnt] = (INT16) trailTree[z].stepDir;
			
			  z = trailTree[z].nextLink;
		  }
		  
			s->pathing.usPathIndex = 0;
		  s->pathing.usPathDataSize  = (UINT16) iCnt;

		}
		else if (bCopy == NO_COPYROUTE)
		{

		  z=_z;

			for (iCnt=0; z != 0; iCnt++)
		  {
			  guiPathingData[ iCnt ] = trailTree[z].stepDir;
			
			  z = trailTree[z].nextLink;
		  }
		  
		  giPathDataSize = (UINT16) iCnt;

		}

		#if defined( PATHAI_VISIBLE_DEBUG )
			if (gfDisplayCoverValues && gfDrawPathPoints)
			{
				SetRenderFlags( RENDER_FLAG_FULL );
				RenderWorld();
				RenderCoverDebug( );
				InvalidateScreen( );
				EndFrameBufferRender();
				RefreshScreen( NULL );
			}
		#endif

		
		// return path length : serves as a "successful" flag and a path length counter
		#ifdef COUNT_PATHS
			guiSuccessfulPathChecks++;
		#endif
		gubNPCAPBudget = 0;
		gubNPCDistLimit = 0;

		//TEMP:  This is returning zero when I am generating edgepoints, so I am force returning 1 until
		//       this is fixed?
		if( gfGeneratingMapEdgepoints )
		{
			return TRUE;
		}


		return(iCnt);
	}

	#ifdef COUNT_PATHS
		if (fCopyReachable)
		{
			// actually this is a success
			guiSuccessfulPathChecks++;
		}
		else
		{
			guiUnsuccessfulPathChecks++;
		}
	#endif

	// failed miserably, report...
	gubNPCAPBudget = 0;
	gubNPCDistLimit = 0;
	return(0);
	//}
	//__except(filter(GetExceptionCode(), GetExceptionInformation()))
	//{
	//	return (0);
	//}

}
#endif