#ifndef WORLD_DAT_H
#define WORLD_DAT_H

#include "types.h"
#include "tile_dat.h"
#include "world_tileset_enums.h"

typedef void (*TILESET_CALLBACK)( void );	


typedef struct
{
	INT16								zName[ 32 ];
	CHAR8								TileSurfaceFilenames[ NUMBEROFTILETYPES ][32];
	UINT8								ubAmbientID;
	TILESET_CALLBACK		MovementCostFnc;

} TILESET;

// Snap: The maximum number of tilesets allowed is max of UINT8
#define MAX_TILESETS 255

extern UINT8 gubNumSets;  // from WorldDat.cpp

extern TILESET	 gTilesets[ MAX_TILESETS ];


void InitEngineTilesets( );


// THESE FUNCTIONS WILL SET TERRAIN VALUES - CALL ONE FOR EACH TILESET
void SetTilesetOneTerrainValues( );
void SetTilesetTwoTerrainValues( );



#endif
