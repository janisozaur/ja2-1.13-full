#ifndef STRUCURE_WRAP_H
#define STRUCURE_WRAP_H


BOOLEAN	IsTreePresentAtGridno( INT32 sGridNo );
BOOLEAN	IsFencePresentAtGridno( INT32 sGridNo );
BOOLEAN	IsJumpableFencePresentAtGridno( INT32 sGridNo );
BOOLEAN	IsDoorPresentAtGridno( INT32 sGridNo );

// OK, THIS FUNCTION RETURNS A LEVELNODE POINTER TO A WALL OF THE SAME ORIENTATION
// AS WAS GIVEN. RETURNS NULL IF NONE FOUND.
LEVELNODE	*GetWallLevelNodeOfSameOrientationAtGridno( INT32 sGridNo, INT8 ubOrientation );

LEVELNODE *IsWallPresentAtGridno( INT32 sGridNo );

BOOLEAN IsDoorVisibleAtGridNo( INT32 sGridNo );

BOOLEAN IsHiddenStructureVisible( INT32 sGridNo, UINT16 usIndex );

BOOLEAN DoesGridnoContainHiddenStruct( INT32 sGridNo, BOOLEAN *pfVisible );

BOOLEAN	WallExistsOfTopLeftOrientation( INT32 sGridNo );

BOOLEAN	WallExistsOfTopRightOrientation( INT32 sGridNo );

BOOLEAN	WallOrClosedDoorExistsOfTopLeftOrientation( INT32 sGridNo );

BOOLEAN	WallOrClosedDoorExistsOfTopRightOrientation( INT32 sGridNo );

BOOLEAN OpenRightOrientedDoorWithDoorOnRightOfEdgeExists( INT32 sGridNo );
BOOLEAN OpenLeftOrientedDoorWithDoorOnLeftOfEdgeExists( INT32 sGridNo );

LEVELNODE	*GetWallLevelNodeAndStructOfSameOrientationAtGridno( INT32 sGridNo, INT8 ubOrientation, STRUCTURE **ppStructure );

BOOLEAN CutWireFence( INT32 sGridNo );
BOOLEAN IsCuttableWireFenceAtGridNo( INT32 sGridNo );
BOOLEAN IsCutWireFenceAtGridNo( INT32 sGridNo );

BOOLEAN IsRepairableStructAtGridNo( INT32 sGridNo, UINT8 *pubID );
BOOLEAN IsRefuelableStructAtGridNo( INT32 sGridNo, UINT8 *pubID );


BOOLEAN	IsRoofPresentAtGridno( INT32 sGridNo );

INT32 FindDoorAtGridNoOrAdjacent( INT32 sGridNo );

BOOLEAN IsCorpseAtGridNo( INT32 sGridNo, UINT8 ubLevel );

BOOLEAN SetOpenableStructureToClosed( INT32 sGridNo, UINT8 ubLevel );

#endif
