#ifdef PRECOMPILEDHEADERS
	#include "strategic_all.h"
#else
	#include "types.h"
	#include "campaign_init.h"
	#include "random.h"
	#include "campaign_types.h"
	#include "queen_command.h"
	#include "overhead.h"
	#include "strategic_movement.h"
	#include "strategic_status.h"
	#include "game_settings.h"
	#include "creature_spreading.h"
	#include "strategic_ai.h"
	#include "debug.h"
	#include "tactical_save.h"
	#include "platform.h"
	#include "debug_control.h"
	#include "platform.h"
#endif

extern BOOLEAN InitStrategicMovementCosts();
void InitKnowFacilitiesFlags( );

UNDERGROUND_SECTORINFO* gpUndergroundSectorInfoTail = NULL;

// Lesh: this array controls randomization of sectors
// Note that some sectors in game already using alternative map
// It is Skyrider quest: B15, E14, D12, C16
//       weapon caches: E11, H5, H10, J12, M9
//       Madlab quest: H7, H16, I11, E4
// Do not enable randomization of this sectors until you are know what you're doing
BOOLEAN RandomSector[256] = 
{
	//		1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16
	/* A */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* B */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* C */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* D */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* E */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* F */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* G */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* H */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* I */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* J */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* K */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* L */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* M */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* N */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* O */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
	/* P */	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0
};

UNDERGROUND_SECTORINFO* NewUndergroundNode( UINT8 ubSectorX, UINT8 ubSectorY, UINT8 ubSectorZ )
{
	UNDERGROUND_SECTORINFO *curr;
	curr = (UNDERGROUND_SECTORINFO*)MemAlloc( sizeof( UNDERGROUND_SECTORINFO ) );	
	AssertMsg( curr, "Failed to create an underground sector info node." );
	memset( curr, 0, sizeof( UNDERGROUND_SECTORINFO ) );

	curr->ubSectorX = ubSectorX;
	curr->ubSectorY = ubSectorY;
	curr->ubSectorZ = ubSectorZ;

	if( gpUndergroundSectorInfoTail )
	{
		gpUndergroundSectorInfoTail->next = curr;
		gpUndergroundSectorInfoTail = gpUndergroundSectorInfoTail->next;
	}
	else
	{
		gpUndergroundSectorInfoHead = curr;
		gpUndergroundSectorInfoTail = gpUndergroundSectorInfoHead;
	}
	return curr;
}

// setup which know facilities are in which cities
void InitKnowFacilitiesFlags( )
{
	SECTORINFO *pSector;

	// Cambria hospital
	pSector = &SectorInfo[SEC_G8];
	pSector->uiFacilitiesFlags |= SFCF_HOSPITAL;
	pSector = &SectorInfo[SEC_F8];
	pSector->uiFacilitiesFlags |= SFCF_HOSPITAL;
	pSector = &SectorInfo[SEC_G9];
	pSector->uiFacilitiesFlags |= SFCF_HOSPITAL;
	pSector = &SectorInfo[SEC_F9];
	pSector->uiFacilitiesFlags |= SFCF_HOSPITAL;

	// Drassen airport
	pSector = &SectorInfo[SEC_B13];
	pSector->uiFacilitiesFlags |= SFCF_AIRPORT;
	pSector = &SectorInfo[SEC_C13];
	pSector->uiFacilitiesFlags |= SFCF_AIRPORT;
	pSector = &SectorInfo[SEC_D13];
	pSector->uiFacilitiesFlags |= SFCF_AIRPORT;

	// Meduna airport & military complex
	pSector = &SectorInfo[SEC_N3];
	pSector->uiFacilitiesFlags |= SFCF_AIRPORT;
	pSector = &SectorInfo[SEC_N4];
	pSector->uiFacilitiesFlags |= SFCF_AIRPORT;
	pSector = &SectorInfo[SEC_N5];
	pSector->uiFacilitiesFlags |= SFCF_AIRPORT;
	pSector = &SectorInfo[SEC_O3];
	pSector->uiFacilitiesFlags |= SFCF_AIRPORT;
	pSector = &SectorInfo[SEC_O4];
	pSector->uiFacilitiesFlags |= SFCF_AIRPORT;

	return;
}


void InitMiningLocations()
{
	SECTORINFO *pSector;
	//Set up mining sites
	
	pSector = &SectorInfo[SEC_D4];
	pSector->uiFlags |= SF_MINING_SITE;
//	pSector->ubIncomeValue = 33;		

	pSector = &SectorInfo[SEC_D13];
	pSector->uiFlags |= SF_MINING_SITE;
//	pSector->ubIncomeValue = 41;
	
	pSector = &SectorInfo[SEC_B2];
	pSector->uiFlags |= SF_MINING_SITE;
//	pSector->ubIncomeValue = 20;
	
	pSector = &SectorInfo[SEC_H8];
	pSector->uiFlags |= SF_MINING_SITE;
//	pSector->ubIncomeValue = 64;
	
	pSector = &SectorInfo[SEC_I14];
	pSector->uiFlags |= SF_MINING_SITE;
//	pSector->ubIncomeValue = 80;
	
	//Grumm
	pSector = &SectorInfo[SEC_H3];
	pSector->uiFlags |= SF_MINING_SITE;
//	pSector->ubIncomeValue = 100;
}

//Mobile groups are handled separately from sectors, because they are on the move.  
void GeneratePatrolGroups()
{
	GROUP *pGroup;
	UINT8 ubNumTroops;
	ubNumTroops = (UINT8)(3 + gGameOptions.ubDifficultyLevel + Random( 3 ));
	pGroup = CreateNewEnemyGroupDepartingFromSector( SEC_C7, 0, ubNumTroops, 0 );
	pGroup->ubTransportationMask = CAR;
	AddWaypointToPGroup( pGroup, 8, 3 ); //C8
	AddWaypointToPGroup( pGroup, 7, 3 ); //C7

	ubNumTroops = (UINT8)(3 + gGameOptions.ubDifficultyLevel + Random( 3 ));
	pGroup = CreateNewEnemyGroupDepartingFromSector( SEC_D9, 0, ubNumTroops, 0 );
	AddWaypointToPGroup( pGroup, 9, 5 ); //E9
	AddWaypointToPGroup( pGroup, 9, 4 ); //D9
	pGroup->ubTransportationMask = TRUCK;

	ubNumTroops = (UINT8)(3 + gGameOptions.ubDifficultyLevel + Random( 3 ));
	pGroup = CreateNewEnemyGroupDepartingFromSector( SEC_B9, 0, ubNumTroops, 0 );
	AddWaypointToPGroup( pGroup, 12, 2 ); //B12
	AddWaypointToPGroup( pGroup, 9, 2 ); //B9
	pGroup->ubTransportationMask = FOOT;

	ubNumTroops = (UINT8)(3 + gGameOptions.ubDifficultyLevel + Random( 3 ));
	pGroup = CreateNewEnemyGroupDepartingFromSector( SEC_A14, 0, ubNumTroops, 0 );
	pGroup->ubMoveType = ENDTOEND_FORWARDS;
	AddWaypointToPGroup( pGroup, 13, 1 ); //A13
	AddWaypointToPGroup( pGroup, 15, 1 ); //A15
	AddWaypointToPGroup( pGroup, 15, 5 ); //E15
	AddWaypointToPGroup( pGroup, 13, 5 ); //E13
	AddWaypointToPGroup( pGroup, 12, 5 ); //E12
	AddWaypointToPGroup( pGroup, 12, 3 ); //C12
	pGroup->ubTransportationMask = TRACKED;

	ubNumTroops = (UINT8)(5 + gGameOptions.ubDifficultyLevel * 2 + Random( 4 ));
	pGroup = CreateNewEnemyGroupDepartingFromSector( SEC_N6, 0, ubNumTroops, 0 );
	AddWaypointToPGroup( pGroup, 9, 14 ); //N9
	AddWaypointToPGroup( pGroup, 6, 14 ); //N6
	pGroup->ubTransportationMask = CAR;

	ubNumTroops = (UINT8)(5 + gGameOptions.ubDifficultyLevel * 2 + Random( 4 ));
	pGroup = CreateNewEnemyGroupDepartingFromSector( SEC_N10, 0, ubNumTroops, 0 );
	AddWaypointToPGroup( pGroup, 10, 11 ); //K10
	AddWaypointToPGroup( pGroup, 10, 14 ); //N10
	pGroup->ubTransportationMask = CAR;
}

void TrashUndergroundSectorInfo()
{
	UNDERGROUND_SECTORINFO *curr;
	while( gpUndergroundSectorInfoHead )
	{
		curr = gpUndergroundSectorInfoHead;
		gpUndergroundSectorInfoHead = gpUndergroundSectorInfoHead->next;
		MemFree( curr );
	}
	gpUndergroundSectorInfoHead = NULL;
	gpUndergroundSectorInfoTail = NULL;
}

//Defines the sectors that can be occupied by enemies, creatures, etc.  It also
//contains the network of cave connections critical for strategic creature spreading, as we can't
//know how the levels connect without loading the maps.  This is completely hardcoded, and any 
//changes to the maps, require changes accordingly.
void BuildUndergroundSectorInfoList()
{
	UNDERGROUND_SECTORINFO *curr;
	SECTORINFO			   *pSector = NULL;

	TrashUndergroundSectorInfo();

	//********************
	//* BASEMENT LEVEL 1 *
	//********************

	//Miguel's basement.  Nothing here.
	curr = NewUndergroundNode( 10, 1, 1 );
	
	//Chitzena mine.  Nothing here.
	curr = NewUndergroundNode( 2, 2, 1 );

	//San mona mine.  Nothing here.
	curr = NewUndergroundNode( 4, 4, 1 );
	curr = NewUndergroundNode( 5, 4, 1 );

	//J9
	curr = NewUndergroundNode( 9, 10, 1 );
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			curr->ubNumTroops = 8;
			break;
		case DIF_LEVEL_MEDIUM:
			curr->ubNumTroops = 11;
			break;
		case DIF_LEVEL_HARD:
			curr->ubNumTroops = 15;
			break;
		case DIF_LEVEL_INSANE:
			curr->ubNumTroops = 20;
			break;
	}
	//J9 feeding zone
	curr = NewUndergroundNode( 9, 10, 2 );
	curr->ubNumCreatures = (UINT8)(2 + gGameOptions.ubDifficultyLevel*2 + Random( 2 ));

	//K4
	curr = NewUndergroundNode( 4, 11, 1 );
	curr->ubNumTroops = (UINT8)(6 + gGameOptions.ubDifficultyLevel*2 + Random( 3 ));
	curr->ubNumElites = (UINT8)(4 + gGameOptions.ubDifficultyLevel + Random( 2 ));
	
	//O3
	curr = NewUndergroundNode( 3, 15, 1 );
	curr->ubNumTroops = (UINT8)(6 + gGameOptions.ubDifficultyLevel*2 + Random( 3 ));
	curr->ubNumElites = (UINT8)(4 + gGameOptions.ubDifficultyLevel + Random( 2 ));
	curr->ubAdjacentSectors |= SOUTH_ADJACENT_SECTOR;
	
	//P3
	curr = NewUndergroundNode( 3, 16, 1 );
	switch( gGameOptions.ubDifficultyLevel )
	{
		case DIF_LEVEL_EASY:
			curr->ubNumElites = (UINT8)(8 + Random( 3 ));
			break;
		case DIF_LEVEL_MEDIUM:
			curr->ubNumElites = (UINT8)(10 + Random( 5 ));
			break;
		case DIF_LEVEL_HARD:
			curr->ubNumElites = (UINT8)(14 + Random( 6 ));
			break;
		case DIF_LEVEL_INSANE:
			curr->ubNumElites = 20;
			break;
	}
	curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR;

	//Do all of the mandatory underground mine sectors
	
	//Drassen's mine
	//D13_B1
	curr = NewUndergroundNode( 13, 4, 1 );
	curr->ubAdjacentSectors |= SOUTH_ADJACENT_SECTOR;
	//E13_B1
	curr = NewUndergroundNode( 13, 5, 1 );
	curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR;
	//E13_B2
	curr = NewUndergroundNode( 13, 5, 2 );
	curr->ubAdjacentSectors |= SOUTH_ADJACENT_SECTOR;
	//F13_B2
	curr = NewUndergroundNode( 13, 6, 2 );
	curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR | SOUTH_ADJACENT_SECTOR;
	//G13_B2
	curr = NewUndergroundNode( 13, 7, 2 );
	curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR;
	//G13_B3
	curr = NewUndergroundNode( 13, 7, 3 );
	curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR;
	//F13_B3
	curr = NewUndergroundNode( 13, 6, 3 );
	curr->ubAdjacentSectors |= SOUTH_ADJACENT_SECTOR;

	//Cambria's mine
	//H8_B1
	curr = NewUndergroundNode( 8, 8, 1 );
	curr->ubAdjacentSectors |= EAST_ADJACENT_SECTOR;
	//H9_B1
	curr = NewUndergroundNode( 9, 8, 1 );
	curr->ubAdjacentSectors |= WEST_ADJACENT_SECTOR;
	//H9_B2
	curr = NewUndergroundNode( 9, 8, 2 );
	curr->ubAdjacentSectors |= WEST_ADJACENT_SECTOR;
	//H8_B2
	curr = NewUndergroundNode( 8, 8, 2 );
	curr->ubAdjacentSectors |= EAST_ADJACENT_SECTOR;
	//H8_B3
	curr = NewUndergroundNode( 8, 8, 3 );
	curr->ubAdjacentSectors |= SOUTH_ADJACENT_SECTOR;
	//I8_B3
	curr = NewUndergroundNode( 8, 9, 3 );
	curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR | SOUTH_ADJACENT_SECTOR;
	//J8_B3
	curr = NewUndergroundNode( 8, 10, 3 );
	curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR;

	//Alma's mine
	//I14_B1
	curr = NewUndergroundNode( 14, 9, 1 );
	curr->ubAdjacentSectors |= SOUTH_ADJACENT_SECTOR;
	//J14_B1
	curr = NewUndergroundNode( 14, 10, 1 );
	curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR;
	//J14_B2
	curr = NewUndergroundNode( 14, 10, 2 );
	curr->ubAdjacentSectors |= WEST_ADJACENT_SECTOR;
	//J13_B2
	curr = NewUndergroundNode( 13, 10, 2 );
	curr->ubAdjacentSectors |= EAST_ADJACENT_SECTOR;
	//J13_B3
	curr = NewUndergroundNode( 13, 10, 3 );
	curr->ubAdjacentSectors |= SOUTH_ADJACENT_SECTOR;
	//K13_B3
	curr = NewUndergroundNode( 13, 11, 3 );
	curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR;

	//Grumm's mine
	//H3_B1
	curr = NewUndergroundNode( 3, 8, 1 );
	curr->ubAdjacentSectors |= SOUTH_ADJACENT_SECTOR;
	//I3_B1
	curr = NewUndergroundNode( 3, 9, 1 );
	curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR;
	//I3_B2
	curr = NewUndergroundNode( 3, 9, 2 );
	curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR;
	//H3_B2
	curr = NewUndergroundNode( 3, 8, 2 );
	curr->ubAdjacentSectors |= SOUTH_ADJACENT_SECTOR | EAST_ADJACENT_SECTOR;
	//H4_B2
	curr = NewUndergroundNode( 4, 8, 2 );
	curr->ubAdjacentSectors |= WEST_ADJACENT_SECTOR;
	curr->uiFlags |= SF_PENDING_ALTERNATE_MAP;
	//H4_B3
	curr = NewUndergroundNode( 4, 8, 3 );
	curr->ubAdjacentSectors |= NORTH_ADJACENT_SECTOR;
	//G4_B3
	curr = NewUndergroundNode( 4, 7, 3 );
	curr->ubAdjacentSectors |= SOUTH_ADJACENT_SECTOR;
}

// Lesh: this function creates randomized world
//       every sector can be randomized between common and alternative, chances 50/50
//       randomization of individual sectors can be switched off via array RandomSector[]
void InitWorld()
{
	INT16	sSectorCounter;

	for (sSectorCounter = 0; sSectorCounter < 256; sSectorCounter++)
	{
		if ( RandomSector[ sSectorCounter ] )
		{
			if ( Random(100) >= 50 )
				SectorInfo[ sSectorCounter ].uiFlags |= SF_USE_ALTERNATE_MAP;
		}
	}
}

//This is the function that is called only once, when the player begins a new game.  This will calculate
//starting numbers of the queen's army in various parts of the map, which will vary from campaign to campaign.
//This is also highly effected by the game's difficulty setting.
void InitNewCampaign()
{
	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitNewCampaign");
	//First clear all the sector information of all enemy existance.  Conveniently, the
	//ubGroupType is also cleared, which is perceived to be an empty group.
	memset( &SectorInfo, 0, sizeof( SECTORINFO ) * 256 );
	InitStrategicMovementCosts();
	RemoveAllGroups();

    InitWorld();	// Lesh: generate different world each time using alternative maps
	InitMiningLocations();
	InitKnowFacilitiesFlags( );

	BuildUndergroundSectorInfoList();
	
	// allow overhead view of omerta A9 on game onset
	SetSectorFlag( 9, 1, 0, SF_ALREADY_VISITED );

	//Generates the initial forces in a new campaign.  The idea is to randomize numbers and sectors
	//so that no two games are the same.
	InitStrategicAI();

	InitStrategicStatus();

	DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"InitNewCampaign done");
}
