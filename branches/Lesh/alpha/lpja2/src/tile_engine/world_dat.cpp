#ifdef PRECOMPILEDHEADERS
	#include "tile_engine_all.h"
#else
	#include "platform.h"
	#include "world_dat.h"
	#include "world_def.h"
	#include "tile_def.h"
	#include "sys_globals.h"
	#include "tile_surface.h"
	#include "file_man.h"
	#include "sgp_str.h"
#endif

// THIS FILE CONTAINS DEFINITIONS FOR TILESET FILES

void SetTilesetThreeTerrainValues();
void SetTilesetFourTerrainValues();

// Snap: This global gives the number of tilesets in JA2SET.DAT
// It is properly initialized in InitEngineTilesets,
// where it is read from JA2SET.DAT header.
UINT8 gubNumSets = MAX_TILESETS;

TILESET	 gTilesets[ MAX_TILESETS ];


void InitEngineTilesets( )
{
	UINT32		cnt, cnt2, uiNumFiles;
//	FILE					*hfile;
	HWFILE			hfile;
	CHAR8			zName[32];
	UINT32		uiNumBytesRead;

	//OPEN FILE
//	hfile = fopen( "BINARYDATA\\JA2SET.DAT", "rb" );
	hfile = FileOpen( "BINARYDATA\\JA2SET.DAT", FILE_ACCESS_READ, FALSE );
	if ( !hfile  )
	{
		SET_ERROR( "Cannot open tileset data file" );
		return;
	}

		
	// READ # TILESETS and compare
//	fread( &gubNumSets, sizeof( gubNumSets ), 1, hfile );
	FileRead( hfile, &gubNumSets, sizeof( gubNumSets ), &uiNumBytesRead );
	// CHECK
	if ( gubNumSets > MAX_TILESETS )
	{
		// Report error
		SET_ERROR( "Too many tilesets in the data file" );
		return;
	}

	// READ #files
//	fread( &uiNumFiles, sizeof( uiNumFiles ), 1, hfile );
	FileRead( hfile, &uiNumFiles, sizeof( uiNumFiles ), &uiNumBytesRead );

	// COMPARE
	if ( uiNumFiles != NUMBEROFTILETYPES )
	{
		// Report error
		SET_ERROR( "Number of tilesets slots in code does not match data file" );
		return;
	}


	// Loop through each tileset, load name then files
	for ( cnt = 0; cnt < gubNumSets; cnt++ )
	{
		//Read name
//		fread( &zName, sizeof( zName ), 1, hfile );
		FileRead( hfile, &zName, sizeof( zName ), &uiNumBytesRead );

		// Read ambience value
//		fread( &(gTilesets[ cnt ].ubAmbientID), sizeof( UINT8), 1, hfile );
		FileRead( hfile, &(gTilesets[ cnt ].ubAmbientID), sizeof( UINT8 ), &uiNumBytesRead );

		// Set into tileset 
		WSTR_SPrintf( (wchar_t *)gTilesets[ cnt ].zName, 32, L"%S", zName );

		// Loop for files
		for ( cnt2 = 0; cnt2 < uiNumFiles; cnt2++ )
		{
			// Read file name
//			fread( &zName, sizeof( zName ), 1, hfile );
			FileRead( hfile, &zName, sizeof( zName ), &uiNumBytesRead );

			// Set into database
			strcpy( gTilesets[ cnt ].TileSurfaceFilenames[ cnt2 ], zName );

		}

	}

//	fclose( hfile );
	FileClose( hfile );


	// SET CALLBACK FUNTIONS!!!!!!!!!!!!!
	gTilesets[ TLS_CAVES_1 ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetTwoTerrainValues;
	gTilesets[ TLS_AIRSTRIP ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_DEAD_AIRSTRIP ].MovementCostFnc	= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_PALACE ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_BALIME ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_BALIME_MUSEUM ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_QUEENS_TROPICAL ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_MEDUNA_INNER_TOWN ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_QUEENS_SAM ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;
	gTilesets[ TLS_QUEENS_AIRPORT ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetThreeTerrainValues;

	gTilesets[ TLS_TROPICAL_1 ].MovementCostFnc			= (TILESET_CALLBACK)SetTilesetFourTerrainValues;
	gTilesets[ TLS_DESERT_SAM ].MovementCostFnc				= (TILESET_CALLBACK)SetTilesetFourTerrainValues;
}




void SetTilesetOneTerrainValues( )
{
		// FIRST TEXUTRES
		gTileSurfaceArray[ FIRSTTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ SECONDTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ THIRDTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ FOURTHTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ FIFTHTEXTURE ]->ubTerrainID = LOW_GRASS;
		gTileSurfaceArray[ SIXTHTEXTURE ]->ubTerrainID = LOW_GRASS;
		gTileSurfaceArray[ SEVENTHTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ REGWATERTEXTURE ]->ubTerrainID = LOW_WATER;
		gTileSurfaceArray[ DEEPWATERTEXTURE ]->ubTerrainID = DEEP_WATER;

		// NOW ROADS
		gTileSurfaceArray[ FIRSTROAD ]->ubTerrainID = DIRT_ROAD;
		gTileSurfaceArray[ ROADPIECES ]->ubTerrainID = DIRT_ROAD;

		// NOW FLOORS
		gTileSurfaceArray[ FIRSTFLOOR ]->ubTerrainID = FLAT_FLOOR;
		gTileSurfaceArray[ SECONDFLOOR ]->ubTerrainID = FLAT_FLOOR;
		gTileSurfaceArray[ THIRDFLOOR ]->ubTerrainID = FLAT_FLOOR;
		gTileSurfaceArray[ FOURTHFLOOR ]->ubTerrainID = FLAT_FLOOR;

		// NOW ANY TERRAIN MODIFYING DEBRIS

}


void SetTilesetTwoTerrainValues( )
{
		// FIRST TEXUTRES
		gTileSurfaceArray[ FIRSTTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ SECONDTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ THIRDTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ FOURTHTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ FIFTHTEXTURE ]->ubTerrainID = LOW_GRASS;
		gTileSurfaceArray[ SIXTHTEXTURE ]->ubTerrainID = LOW_GRASS;
		gTileSurfaceArray[ SEVENTHTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ REGWATERTEXTURE ]->ubTerrainID = LOW_WATER;
		gTileSurfaceArray[ DEEPWATERTEXTURE ]->ubTerrainID = DEEP_WATER;

		// NOW ROADS
		gTileSurfaceArray[ FIRSTROAD ]->ubTerrainID = DIRT_ROAD;
		gTileSurfaceArray[ ROADPIECES ]->ubTerrainID = DIRT_ROAD;


		// NOW FLOORS
		gTileSurfaceArray[ FIRSTFLOOR ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ SECONDFLOOR ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ THIRDFLOOR ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ FOURTHFLOOR ]->ubTerrainID = FLAT_GROUND;

}


void SetTilesetThreeTerrainValues( )
{
		// DIFFERENCE FROM #1 IS THAT ROADS ARE PAVED

		// FIRST TEXUTRES
		gTileSurfaceArray[ FIRSTTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ SECONDTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ THIRDTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ FOURTHTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ FIFTHTEXTURE ]->ubTerrainID = LOW_GRASS;
		gTileSurfaceArray[ SIXTHTEXTURE ]->ubTerrainID = LOW_GRASS;
		gTileSurfaceArray[ SEVENTHTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ REGWATERTEXTURE ]->ubTerrainID = LOW_WATER;
		gTileSurfaceArray[ DEEPWATERTEXTURE ]->ubTerrainID = DEEP_WATER;

		// NOW ROADS
		gTileSurfaceArray[ FIRSTROAD ]->ubTerrainID = PAVED_ROAD;
		gTileSurfaceArray[ ROADPIECES ]->ubTerrainID = PAVED_ROAD;


		// NOW FLOORS
		gTileSurfaceArray[ FIRSTFLOOR ]->ubTerrainID = FLAT_FLOOR;
		gTileSurfaceArray[ SECONDFLOOR ]->ubTerrainID = FLAT_FLOOR;
		gTileSurfaceArray[ THIRDFLOOR ]->ubTerrainID = FLAT_FLOOR;
		gTileSurfaceArray[ FOURTHFLOOR ]->ubTerrainID = FLAT_FLOOR;

		// NOW ANY TERRAIN MODIFYING DEBRIS

}

void SetTilesetFourTerrainValues( )
{
		// DIFFERENCE FROM #1 IS THAT FLOOR2 IS NOT FLAT_FLOOR BUT FLAT_GROUND

		// FIRST TEXUTRES
		gTileSurfaceArray[ FIRSTTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ SECONDTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ THIRDTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ FOURTHTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ FIFTHTEXTURE ]->ubTerrainID = LOW_GRASS;
		gTileSurfaceArray[ SIXTHTEXTURE ]->ubTerrainID = LOW_GRASS;
		gTileSurfaceArray[ SEVENTHTEXTURE ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ REGWATERTEXTURE ]->ubTerrainID = LOW_WATER;
		gTileSurfaceArray[ DEEPWATERTEXTURE ]->ubTerrainID = DEEP_WATER;

		// NOW ROADS
		gTileSurfaceArray[ FIRSTROAD ]->ubTerrainID = DIRT_ROAD;
		gTileSurfaceArray[ ROADPIECES ]->ubTerrainID = DIRT_ROAD;


		// NOW FLOORS
		gTileSurfaceArray[ FIRSTFLOOR ]->ubTerrainID = FLAT_FLOOR;
		gTileSurfaceArray[ SECONDFLOOR ]->ubTerrainID = FLAT_GROUND;
		gTileSurfaceArray[ THIRDFLOOR ]->ubTerrainID = FLAT_FLOOR;
		gTileSurfaceArray[ FOURTHFLOOR ]->ubTerrainID = FLAT_FLOOR;

		// NOW ANY TERRAIN MODIFYING DEBRIS

}
