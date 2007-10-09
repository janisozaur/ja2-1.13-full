#ifndef __SOLDER_PROFILE_H
#define __SOLDER_PROFILE_H

#include "Soldier Control.h"
#include "Soldier Profile Type.h"
#include "Merc Hiring.h"

extern INT8 gbSkillTraitBonus[NUM_SKILLTRAITS];
extern UINT8 gubTerrorists[];
extern INT16 gsTerroristSector[][5][2];
extern BOOLEAN	gfPotentialTeamChangeDuringDeath;

extern MERCPROFILESTRUCT gMercProfiles[ NUM_PROFILES ];

#define AIM_AND_MERC_MERCS		51		// A.I.M. is 0-39, M.E.R.C.s are 40-50

//use this to sort out the above define
BOOLEAN IsProfileIdAnAimOrMERCMerc( INT16 ubProfileID );
//enums for the mercs 
enum NPCIDs
{
	BIFF = 40,
	HAYWIRE,
	GASKET,
	RAZOR,
	FLO,
	GUMPY,
	LARRY_NORMAL,
	LARRY_DRUNK,
	COUGAR,
	NUMB,
	BUBBA,

	MIGUEL = 57,
	CARLOS,
	IRA,
	DIMITRI,

	DEVIN,	// 61
	ROBOT,
	HAMOUS,
	SLAY,
	RPC65,
	DYNAMO,
	SHANK,
	IGGY,
	VINCE,
	CONRAD,

	RPC71,	// 71
	MADDOG,
	DARREL,
	PERKO,
	QUEEN,
	AUNTIE,
	ENRICO, //GABBY, //ENRICO,
	CARMEN,
	JOE,
	STEVE,

	RAT,		// 81
	ANNIE,
	CHRIS,
	BOB,
	BRENDA,
	KINGPIN,
	DARREN,
	MARIA,
	ANGEL,
	JOEY,

	TONY,		// 91
	FRANK,
	SPIKE,
	DAMON,
	KYLE,
	MICKY,
	SKYRIDER,
	PABLO,
	SAL,
	FATHER,

	FATIMA,	// 101
	WARDEN,
	GORDON,
	GABBY, //ENRICO, //GABBY,
	ERNEST,
	FRED,
	MADAME,
	YANNI,
	MARTHA,
	TIFFANY,

	T_REX,	// 111
	DRUGGIST,	// ELGIN
	JAKE,
	PACOS,
	GERARD,
	SKIPPER,
	HANS,
	JOHN,
	MARY,
	GENERAL,

	SERGEANT,	// 121
	ARMAND,
	LORA,
	FRANZ,
	HOWARD,
	SAM,
	ELDIN,
	ARNIE,
	TINA,
	FREDO,

	WALTER,	// 131
	JENNY,
	BILLY,
	BREWSTER,
	ELLIOT,
	DEREK,
	OLIVER,
	WALDO,
	DOREEN,
	JIM,

	JACK,		// 141
	OLAF,
	RAY,
	OLGA,
	TYRONE,
	MADLAB,
	KEITH,
	MATT,
	MIKE,
	DARYL,

	HERVE,	// 151
	PETER,
	ALBERTO,
	CARLO,
	MANNY,
	OSWALD,
	CALVIN,
	CARL,
	SPECK,
	
	PROF_HUMMER,

	PROF_ELDERODO,	// 161
	PROF_ICECREAM,
	PROF_HELICOPTER,
	NPC164,
	GASTON,
	STOGIE,
	TEX,
	BIGGENS,
	NPC169,
} ;

BOOLEAN LoadMercProfiles(void);

// This function will overload the default for the face index ( which is the ubCharNum )
void SetProfileFaceData( INT16 ubCharNum, UINT8 ubFaceIndex, UINT16 usEyesX, UINT16 usEyesY, UINT16 usMouthX, UINT16 usMouthY );
SOLDIERTYPE * FindSoldierByProfileID( INT16 ubProfileID, BOOLEAN fPlayerMercsOnly );

BOOLEAN RecruitRPC( INT16 ubCharNum );

BOOLEAN RecruitEPC( INT16 ubCharNum );
BOOLEAN UnRecruitEPC( INT16 ubCharNum );

INT16 WhichBuddy( INT16 ubCharNum, INT16 ubBuddy );
INT16 WhichHated( INT16 ubCharNum, INT16 ubHated );

SOLDIERTYPE *ChangeSoldierTeam( SOLDIERTYPE *pSoldier, UINT8 ubTeam );

BOOLEAN IsProfileATerrorist( INT16 ubProfile );
BOOLEAN IsProfileAHeadMiner( INT16 ubProfile );

void UpdateSoldierPointerDataIntoProfile( BOOLEAN fPlayerMercs );


// Returns true if a buddy of the merc is on team
BOOLEAN DoesMercHaveABuddyOnTheTeam( INT16 ubMercID );

void StartSomeMercsOnAssignment( void );

void DecideOnAssassin( void );

extern BOOLEAN gubNumTerrorists;

void MakeRemainingTerroristsTougher( void );
void MakeRemainingAssassinsTougher( void );
SOLDIERTYPE * SwapLarrysProfiles( SOLDIERTYPE * pSoldier );

BOOLEAN DoesNPCOwnBuilding( SOLDIERTYPE *pSoldier, INT32 sGridNo );

#endif
