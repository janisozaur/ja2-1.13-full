#ifdef PRECOMPILEDHEADERS
	#include "tile_engine_all.h"
#else
	#include "types.h"
	#include "fog_of_war.h"
	#include "isometric_utils.h"
	#include "world_man.h"
	#include "simple_render_utils.h"
	#include "render_world.h"
	#include "lighting.h"
#endif

//When line of sight reaches a gridno, and there is a light there, it turns it on.
//This is only done in the cave levels.
void RemoveFogFromGridNo( UINT32 uiGridNo )
{
	INT32 i;
	INT32 x, y;
	UINT32 uiAdjacentGridNo = 0;
	x = uiGridNo % WORLD_COLS;
	y = uiGridNo / WORLD_COLS;
	for( i = 0; i < MAX_LIGHT_SPRITES; i++ )
	{
		if( LightSprites[ i ].iX == x && LightSprites[ i ].iY == y )
		{
			if( !(LightSprites[ i ].uiFlags & LIGHT_SPR_ON) )
			{
				LightSpritePower( i, TRUE );
				LightDraw(	LightSprites[i].uiLightType, LightSprites[i].iTemplate, LightSprites[i].iX, LightSprites[i].iY, i );
				MarkWorldDirty();
				return;
			}
		}
	}
}
