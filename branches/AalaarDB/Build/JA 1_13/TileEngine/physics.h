#ifndef __PHYSICS_H
#define	__PHYSICS_H

#include "phys math.h"
#include "sgp.h"
#include "worlddef.h"
#include "items.h"
#include "Soldier Control.h"


extern UINT32 guiNumObjectSlots;

class OLD_REAL_OBJECT_101
{
public:
	BOOLEAN			fAllocated;
	BOOLEAN			fAlive;
	BOOLEAN			fApplyFriction;
	BOOLEAN			fColliding;
	BOOLEAN			fZOnRest;
	BOOLEAN			fVisible;
	BOOLEAN			fInWater;
	BOOLEAN			fTestObject;
	BOOLEAN			fTestEndedWithCollision;
	BOOLEAN			fTestPositionNotSet;
	
	real				TestZTarget;
	real				OneOverMass;
	real				AppliedMu;

	vector_3		Position;
	vector_3		TestTargetPosition;
	vector_3		OldPosition;
	vector_3		Velocity;
	vector_3		OldVelocity;
	vector_3		InitialForce;
	vector_3		Force;
	vector_3		CollisionNormal;
	vector_3		CollisionVelocity;
	real				CollisionElasticity;

	INT16				sGridNo;
	INT32				iID;
	LEVELNODE		*pNode;
	LEVELNODE		*pShadow;

	INT16				sConsecutiveCollisions;
	INT16				sConsecutiveZeroVelocityCollisions;
	INT32				iOldCollisionCode;

	FLOAT				dLifeLength;
	FLOAT				dLifeSpan;
	OLD_OBJECTTYPE_101		oldObj;
	BOOLEAN			fFirstTimeMoved;
	INT16				sFirstGridNo;
	UINT8				ubOwner;
	UINT8				ubActionCode;
	UINT32			uiActionData;
	BOOLEAN			fDropItem;
	UINT32			uiNumTilesMoved;
	BOOLEAN			fCatchGood;
	BOOLEAN			fAttemptedCatch;
	BOOLEAN			fCatchAnimOn;
	BOOLEAN			fCatchCheckDone;
	BOOLEAN			fEndedWithCollisionPositionSet;
	vector_3		EndedWithCollisionPosition;
	BOOLEAN			fHaveHitGround;
  BOOLEAN     fPotentialForDebug;
  INT16       sLevelNodeGridNo;
  INT32       iSoundID;
  UINT8       ubLastTargetTakenDamage;
	UINT8				ubPadding[1];

};

class REAL_OBJECT
{
public:
	REAL_OBJECT		() {initialize();};
	REAL_OBJECT&	operator=(OLD_REAL_OBJECT_101& src);
	BOOLEAN			Load(HWFILE hFile);
	BOOLEAN			Save(HWFILE hFile);
	void			initialize();

	BOOLEAN			fAllocated;
	BOOLEAN			fAlive;
	BOOLEAN			fApplyFriction;
	BOOLEAN			fColliding;
	BOOLEAN			fZOnRest;
	BOOLEAN			fVisible;
	BOOLEAN			fInWater;
	BOOLEAN			fTestObject;
	BOOLEAN			fTestEndedWithCollision;
	BOOLEAN			fTestPositionNotSet;
	
	real				TestZTarget;
	real				OneOverMass;
	real				AppliedMu;

	vector_3		Position;
	vector_3		TestTargetPosition;
	vector_3		OldPosition;
	vector_3		Velocity;
	vector_3		OldVelocity;
	vector_3		InitialForce;
	vector_3		Force;
	vector_3		CollisionNormal;
	vector_3		CollisionVelocity;
	real				CollisionElasticity;

	INT16				sGridNo;
	INT32				iID;
	LEVELNODE		*pNode;
	LEVELNODE		*pShadow;

	INT16				sConsecutiveCollisions;
	INT16				sConsecutiveZeroVelocityCollisions;
	INT32				iOldCollisionCode;

	FLOAT				dLifeLength;
	FLOAT				dLifeSpan;
	BOOLEAN			fFirstTimeMoved;
	INT16				sFirstGridNo;
	UINT8				ubOwner;
	UINT8				ubActionCode;
	UINT32			uiActionData;
	BOOLEAN			fDropItem;
	UINT32			uiNumTilesMoved;
	BOOLEAN			fCatchGood;
	BOOLEAN			fAttemptedCatch;
	BOOLEAN			fCatchAnimOn;
	BOOLEAN			fCatchCheckDone;
	BOOLEAN			fEndedWithCollisionPositionSet;
	vector_3		EndedWithCollisionPosition;
	BOOLEAN			fHaveHitGround;
  BOOLEAN     fPotentialForDebug;
  INT16       sLevelNodeGridNo;
  INT32       iSoundID;
  UINT8       ubLastTargetTakenDamage;

  char				endOfPod;
	OBJECTTYPE		Obj;
};
#define SIZEOF_REAL_OBJECT_POD offsetof(REAL_OBJECT, endOfPod)


#define					NUM_OBJECT_SLOTS	50

extern  REAL_OBJECT			ObjectSlots[ NUM_OBJECT_SLOTS ];


// OBJECT LIST STUFF
INT32	CreatePhysicalObject( OBJECTTYPE *pGameObj, real dLifeLength, real xPos, real yPos, real zPos, real xForce, real yForce, real zForce, UINT8 ubOwner, UINT8 ubActionCode, UINT32 uiActionData, BOOLEAN fTestObject );
BOOLEAN		RemoveObjectSlot( INT32 iObject );
void RemoveAllPhysicsObjects( );


FLOAT CalculateLaunchItemAngle( SOLDIERTYPE *pSoldier, INT16 sGridNo, UINT8 ubHeight, real dForce, OBJECTTYPE *pItem, INT16 *psGridNo );

BOOLEAN CalculateLaunchItemChanceToGetThrough( SOLDIERTYPE *pSoldier, OBJECTTYPE *pItem, INT16 sGridNo, UINT8 ubLevel, INT16 sEndZ, INT16 *psFinalGridNo, BOOLEAN fArmed, INT8 *pbLevel, BOOLEAN fFromUI );

void CalculateLaunchItemParamsForThrow( SOLDIERTYPE *pSoldier, INT16 sGridNo, UINT8 ubLevel, INT16 sZPos, OBJECTTYPE *pItem, INT8 bMissBy, UINT8 ubActionCode, UINT32 uiActionData );





// SIMULATE WORLD
void SimulateWorld(  );


BOOLEAN	SavePhysicsTableToSaveGameFile( HWFILE hFile );

BOOLEAN	LoadPhysicsTableFromSavedGameFile( HWFILE hFile );


#endif