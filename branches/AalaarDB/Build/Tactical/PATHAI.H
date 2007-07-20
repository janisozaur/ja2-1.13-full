/*
	Filename        :       pathai.h
	Author          :       Ray E. Bornert II
	Date            :       1992-MAR-15

	Copyright (C) 1993 HixoxiH Software
*/

#ifndef _PATHAI_H
#define _PATHAI_H
#include "isometric utils.h"
#include "BinaryHeap.hpp"

#include <vector>

enum eAStar
{
	AStar_Init,
	AStar_Open,
	AStar_Closed
};

class GridNode
{
public:
	GridNode(UINT8 x, UINT8 y) {GridNode::x = x; GridNode::y = y;};
	GridNode operator + (const GridNode& point) const {return GridNode(this->x + point.x, this->y + point.y);};
	bool	isInWorld() {return (x < WORLD_COLS && y < WORLD_ROWS);};
	UINT8 x;
	UINT8 y;
};

struct AStar_Data
{
	//no H
	int			cost;//G
	int			f;//F
	int			APCost;//the APs spent to get here
	int			extraGCoverCost;//an extra cost that makes stopping in midpath at a node with little cover worse
	GridNode	parent;
	eAStar		status;
};

typedef HEAP<int, GridNode>		AStarHeap;

class AStarPathfinder
{
public:
	AStarPathfinder();
	int				GetPath	(INT16 const StartNode,
							INT16 const DestNode,
							bool const waterToWater);

private:
	std::vector<GridNode>		ClosedList;
	CBinaryHeap<int, GridNode>	OpenHeap;

	SOLDIERTYPE*	pSoldier;
	int		maxAPCost;//the merc only has so many points to move with
	bool	onRooftop;//ubLevel
	bool	fNonFenceJumper;
	bool	fNonSwimmer;
	bool	fPathingForPlayer;
	bool	fPathAroundObstacles;
	bool	fPlotPathToExitGrid;
	bool	fGoingThroughDoor;
	bool	usOKToAddStructID;
	bool	bLoopState;

	//vehicle defined in cpp
	//#ifdef VEHICLE
		//BOOLEAN fTurnSlow = FALSE;
		//BOOLEAN fReverse = FALSE; // stuff for vehicles turning
		BOOLEAN fMultiTile, fVehicle;
		//INT32 iLastDir, iPrevToLastDir;
		//INT8 bVehicleCheckDir;
		//UINT16 adjLoc;
		STRUCTURE_FILE_REF * pStructureFileRef;
		UINT16							 usAnimSurface;
		//INT32 iCnt2, iCnt3;
	//#endif

	// member variables to prevent passing them around
	GridNode	StartNode;
	GridNode	DestNode;
	GridNode	NextStepNode;
	GridNode	CurrentNode;
	INT16	CurrentNodeIndex;
	INT16	NextNodeIndex;

	AStar_Data	AStarData[WORLD_COLS][WORLD_ROWS];





	AStarHeap		AStar	(GridNode const start,
							GridNode const dest);

	void	ExecuteAStarLogic();

	void	ResetAStarList	();

	int		CalcG			(int const direction);
	int		CalcAP			();
	int		CalcH			();
	int		CalcGCover		(const GridNode ThisNode);

	eAStar	GetAStarStatus	(const GridNode node) const {return AStarData[node.x][node.y].status;};
	GridNode	GetAStarParent	(const GridNode node) const {return AStarData[node.x][node.y].parent;};
	int		GetAStarG		(const GridNode node) const {return AStarData[node.x][node.y].cost;};
	int		GetExtraGCover	(const GridNode node) const {return AStarData[node.x][node.y].extraGCoverCost;};
	int		GetAStarF		(const GridNode node) const {return AStarData[node.x][node.y].f;};
	int		GetActionPoints	(const GridNode node) const {return AStarData[node.x][node.y].APCost;};

	void	SetAStarStatus	(const GridNode node, const eAStar status) {AStarData[node.x][node.y].status = status;};
	void	SetAStarParent	(const GridNode node, GridNode const parent) {AStarData[node.x][node.y].parent = parent;};
	void	SetAStarG		(const GridNode node, const int cost) {AStarData[node.x][node.y].cost = cost;};
	void	SetExtraGCover	(const GridNode node, const int extraGCoverCost) {AStarData[node.x][node.y].extraGCoverCost = extraGCoverCost;};
	void	SetAStarF		(const GridNode node, const int f) {AStarData[node.x][node.y].f = f;};
	void	SetActionPoints	(const GridNode node, const int APCost) {AStarData[node.x][node.y].APCost = APCost;};

	INT16	PythSpacesAway	(GridNode const node1,
							GridNode const node2);
	INT16	SpacesAway		(GridNode const node1,
							GridNode const node2);
	INT16	GridNodeToInt	(GridNode const node) {return (node.x + node.y * MAPWIDTH);};
	GridNode	IntToGridNode	(INT16 const loc) {return (GridNode(loc % MAPWIDTH, loc / MAPWIDTH));};
	bool	IsDiagonal		(GridNode const node1,
							GridNode const node2) {return (abs(node1.x - node2.x) && abs(node1.y - node2.y));};

	void	InitVehicle		();
	bool	VehicleObstacleCheck(int const direction);
};


BOOLEAN InitPathAI( void );
void ShutDownPathAI( void );
INT16 PlotPath( SOLDIERTYPE *pSold, INT16 sDestGridno, INT8 bCopyRoute, INT8 bPlot, INT8 bStayOn, UINT16 usMovementMode, INT8 bStealth, INT8 bReverse , INT16 sAPBudget);
INT16 UIPlotPath( SOLDIERTYPE *pSold, INT16 sDestGridno, INT8 bCopyRoute, INT8 bPlot, INT8 bStayOn, UINT16 usMovementMode, INT8 bStealth, INT8 bReverse , INT16 sAPBudget);
INT16 EstimatePlotPath( SOLDIERTYPE *pSold, INT16 sDestGridno, INT8 bCopyRoute, INT8 bPlot, INT8 bStayOn, UINT16 usMovementMode, INT8 bStealth, INT8 bReverse , INT16 sAPBudget);

void ErasePath(char bEraseOldOne);
INT32 FindBestPath(SOLDIERTYPE *s , INT16 sDestination, INT8 ubLevel, INT16 usMovementMode, INT8 bCopy, UINT8 fFlags );
void GlobalReachableTest( INT16 sStartGridNo );
void GlobalItemsReachableTest( INT16 sStartGridNo1, INT16 sStartGridNo2 );
void RoofReachableTest( INT16 sStartGridNo, UINT8 ubBuildingID );
void LocalReachableTest( INT16 sStartGridNo, INT8 bRadius );

UINT8 DoorTravelCost( SOLDIERTYPE * pSoldier, INT32 iGridNo, UINT8 ubMovementCost, BOOLEAN fReturnPerceivedValue, INT32 * piDoorGridNo );
UINT8 InternalDoorTravelCost( SOLDIERTYPE * pSoldier, INT32 iGridNo, UINT8 ubMovementCost, BOOLEAN fReturnPerceivedValue, INT32 * piDoorGridNo, BOOLEAN fReturnDoorCost );

INT16 RecalculatePathCost( SOLDIERTYPE *pSoldier, UINT16 usMovementMode );

// Lesh: setup max length of path
#define MAX_PATH_DATA_LENGTH 512

// Exporting these global variables
extern UINT32 guiPathingData[MAX_PATH_DATA_LENGTH];
extern UINT8 gubNPCAPBudget;
extern UINT16 gusNPCMovementMode;
extern UINT8 gubNPCDistLimit;
extern UINT8 gubNPCPathCount;
extern BOOLEAN gfPlotPathToExitGrid;
extern BOOLEAN gfNPCCircularDistLimit;
extern BOOLEAN gfEstimatePath;
extern BOOLEAN	gfPathAroundObstacles;
extern UINT8 gubGlobalPathFlags;

// Ian's terrain values for travelling speed/pathing purposes
// Fixed by CJC March 4, 1998.  Please do not change these unless familiar
// with how this will affect the path code!

#define TRAVELCOST_NONE						0
#define TRAVELCOST_FLAT						10
#define TRAVELCOST_BUMPY					12
#define TRAVELCOST_GRASS					12
#define TRAVELCOST_THICK					16
#define TRAVELCOST_DEBRIS					20
#define TRAVELCOST_SHORE					30
#define TRAVELCOST_KNEEDEEP				36
#define TRAVELCOST_DEEPWATER			50
#define TRAVELCOST_FENCE					40

// these values are used to indicate "this is an obstacle
// if there is a door (perceived) open/closed in this tile
#define TRAVELCOST_DOOR_CLOSED_HERE	220
#define TRAVELCOST_DOOR_CLOSED_N		221
#define TRAVELCOST_DOOR_CLOSED_W		222
#define TRAVELCOST_DOOR_OPEN_HERE		223
#define TRAVELCOST_DOOR_OPEN_N			224
#define TRAVELCOST_DOOR_OPEN_NE			225
#define TRAVELCOST_DOOR_OPEN_E			226
#define TRAVELCOST_DOOR_OPEN_SE			227
#define TRAVELCOST_DOOR_OPEN_S			228
#define TRAVELCOST_DOOR_OPEN_SW			229
#define TRAVELCOST_DOOR_OPEN_W			230
#define TRAVELCOST_DOOR_OPEN_NW			231
#define TRAVELCOST_DOOR_OPEN_N_N		232
#define TRAVELCOST_DOOR_OPEN_NW_N		233
#define TRAVELCOST_DOOR_OPEN_NE_N		234
#define TRAVELCOST_DOOR_OPEN_W_W		235
#define TRAVELCOST_DOOR_OPEN_SW_W		236
#define TRAVELCOST_DOOR_OPEN_NW_W		237
#define TRAVELCOST_NOT_STANDING		248
#define TRAVELCOST_OFF_MAP				249
#define TRAVELCOST_CAVEWALL				250
#define TRAVELCOST_HIDDENOBSTACLE 251
#define TRAVELCOST_DOOR						252
#define TRAVELCOST_OBSTACLE				253
#define TRAVELCOST_WALL						254
#define TRAVELCOST_EXITGRID				255

#define TRAVELCOST_TRAINTRACKS		30
#define TRAVELCOST_DIRTROAD				9
#define TRAVELCOST_PAVEDROAD			9
#define TRAVELCOST_FLATFLOOR			10

#define TRAVELCOST_BLOCKED (TRAVELCOST_OFF_MAP)
#define IS_TRAVELCOST_DOOR( x ) (x >= TRAVELCOST_DOOR_CLOSED_HERE && x <= TRAVELCOST_DOOR_OPEN_NW_W)
#define IS_TRAVELCOST_CLOSED_DOOR( x ) (x >= TRAVELCOST_DOOR_CLOSED_HERE && x << TRAVELCOST_DOOR_CLOSED_W)

// ------------------------------------------
// PLOT PATH defines
#define NOT_STEALTH							0
#define	STEALTH									1

#define NO_PLOT									0
#define PLOT										1

#define TEMPORARY								0
#define PERMANENT								1

#define FORWARD									0
#define REVERSE									1

#define NO_COPYROUTE						0
#define COPYROUTE								1
#define COPYREACHABLE           2
#define COPYREACHABLE_AND_APS		3

#define PATH_THROUGH_PEOPLE					0x01
#define PATH_IGNORE_PERSON_AT_DEST	0x02
#define PATH_CLOSE_GOOD_ENOUGH			0x04

#define PATH_CLOSE_RADIUS 5

// ------------------------------------------

#endif
