#ifndef EVENT_PROCESSOR_H
#define EVENT_PROCESSOR_H

#include "Event Manager.h"


#define NETWORK_PATH_DATA_SIZE	6
// Enumerate all events for JA2
enum eJA2Events
{

	E_PLAYSOUND,
	S_CHANGEDEST,
//	S_GETNEWPATH,
	S_BEGINTURN,
	S_CHANGESTANCE,
	S_SETDESIREDDIRECTION,
	S_BEGINFIREWEAPON,
	S_FIREWEAPON,
	S_WEAPONHIT,
	S_STRUCTUREHIT,
	S_WINDOWHIT,
	S_MISS,
	S_NOISE,	
	S_STOP_MERC,

	EVENTS_LOCAL_AND_NETWORK,				// Events above here are sent locally and over network
	
	S_GETNEWPATH,
	S_SETPOSITION,
	S_CHANGESTATE,
	S_SETDIRECTION,
	EVENTS_ONLY_USED_LOCALLY,			// Events above are only used locally 

	S_SENDPATHTONETWORK,
	S_UPDATENETWORKSOLDIER,
  EVENTS_ONLY_SENT_OVER_NETWORK,	// Events above are only sent to the network

	NUM_EVENTS

} ;

// This definition is used to denote events with a special delay value;
// it indicates that these events will not be processed until specifically
// called for in a special loop.
#define DEMAND_EVENT_DELAY 0xFFFF

// Enumerate all structures for events
typedef struct
{
	UINT16		usIndex;
	UINT16		usRate;
	UINT8			ubVolume;
	UINT8			ubLoops;
	UINT32		uiPan;

} EV_E_PLAYSOUND;


typedef struct
{
	INT16  usSoldierID;
	UINT32  uiUniqueId;
	UINT16	usNewState;
	INT16		sXPos;
	INT16		sYPos;
	UINT16  usStartingAniCode;
	BOOLEAN	fForce;

} EV_S_CHANGESTATE;


typedef struct
{
	INT16  usSoldierID;
	UINT32  uiUniqueId;
	//INT32	usNewDestination; Changed to accomodate Destination changing to INT16.
	//Might need to change back if pathing errors occur. Gotthard 10/8/07
	INT16 usNewDestination;

} EV_S_CHANGEDEST;

typedef struct
{
	INT16 usSoldierID;
	UINT32  uiUniqueId;
	FLOAT	 dNewXPos;
	FLOAT	 dNewYPos;

} EV_S_SETPOSITION;

typedef struct
{
	INT16	 usSoldierID;
	UINT32	 uiUniqueId;
	INT32 sDestGridNo;
	UINT16		usMovementAnim;		 

} EV_S_GETNEWPATH;

typedef struct
{
	INT16  usSoldierID;
	UINT32  uiUniqueId;
} EV_S_BEGINTURN;

typedef struct
{
	INT16  usSoldierID;
	UINT32  uiUniqueId;
	UINT8		ubNewStance;
	INT16		sXPos;
	INT16		sYPos;

} EV_S_CHANGESTANCE;

typedef struct
{
	INT16  usSoldierID;
	UINT32  uiUniqueId;
	INT8 usNewDirection;

} EV_S_SETDIRECTION;

typedef struct
{
	INT16  usSoldierID;
	UINT32  uiUniqueId;
	INT8	usDesiredDirection;

} EV_S_SETDESIREDDIRECTION;


typedef struct
{
	INT16  usSoldierID;
	UINT32  uiUniqueId;
	INT32 sTargetGridNo;
	INT8		bTargetLevel;
	INT8		bTargetCubeLevel;
} EV_S_BEGINFIREWEAPON;


typedef struct
{
	INT16  usSoldierID;
	UINT32  uiUniqueId;
	INT32 sTargetGridNo;
	INT8		bTargetLevel;
	INT8		bTargetCubeLevel;
} EV_S_FIREWEAPON;

typedef struct
{
	INT16  usSoldierID;
	UINT32  uiUniqueId;
	UINT16	usWeaponIndex;
	INT16		sDamage;
	INT16		sBreathLoss;
	INT8	usDirection;
	INT16		sXPos;
	INT16		sYPos;
	INT16		sZPos;
	INT16		sRange;
	INT16		ubAttackerID;
	BOOLEAN fHit;
	UINT8		ubSpecial;
	UINT8		ubLocation;

} EV_S_WEAPONHIT;

typedef struct
{
	INT16		sXPos;
	INT16		sYPos;
	INT16		sZPos;
	UINT16	usWeaponIndex;
	INT8		bWeaponStatus;
	INT16 ubAttackerID;
	INT16 usStructureID;
	INT32		iImpact;
	INT32		iBullet;

} EV_S_STRUCTUREHIT;

typedef struct
{
	INT32 sGridNo;
	INT16	usStructureID;
	BOOLEAN	fBlowWindowSouth;
	BOOLEAN	fLargeForce;
} EV_S_WINDOWHIT;

typedef struct
{
	INT16		ubAttackerID;
} EV_S_MISS;

typedef struct
{
	INT16 ubNoiseMaker;
	INT32 sGridNo;
	UINT8 bLevel;
	UINT8 ubTerrType;
	UINT8 ubVolume;
	UINT8 ubNoiseType;
} EV_S_NOISE;


typedef struct
{
	INT16  usSoldierID;
	UINT32  uiUniqueId;
	INT8		bDirection;
	INT32 sGridNo;
	INT16		sXPos;
	INT16		sYPos;

} EV_S_STOP_MERC;



typedef struct
{
	INT16 usSoldierID;
	UINT32  uiUniqueId;
	UINT8		usPathDataSize;			// Size of Path
	INT32 sAtGridNo;					// Owner merc is at this tile when sending packet
	UINT8		usCurrentPathIndex;	// Index the owner of the merc is at when sending packet
	UINT8		usPathData[ NETWORK_PATH_DATA_SIZE ];		// make define  // Next X tile to go to
	UINT8		ubNewState;			// new movment Anim
//	INT8		bActionPoints;
//	INT8		bBreath;			// current breath value
//	INT8		bDesiredDirection;

	// maybe send current action & breath points
} EV_S_SENDPATHTONETWORK;

typedef struct
{
	INT16		usSoldierID;	
	UINT32  uiUniqueId;
	INT32 sAtGridNo;					// Owner merc is at this tile when sending packet
	INT8		bActionPoints;			// current A.P. value
	INT8		bBreath;						// current breath value
} EV_S_UPDATENETWORKSOLDIER;


// FUNCTIONS
BOOLEAN AddGameEvent( UINT32 uiEvent, UINT16 usDelay, PTR pEventData );
BOOLEAN AddGameEventFromNetwork( UINT32 uiEvent, UINT16 usDelay, PTR pEventData );
BOOLEAN DequeAllGameEvents( BOOLEAN fExecute );
BOOLEAN DequeueAllDemandGameEvents( BOOLEAN fExecute );

// clean out the evetn queue
BOOLEAN ClearEventQueue( void );


#endif