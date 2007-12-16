// WANNE: EDITOR: done
#ifdef PRECOMPILEDHEADERS
	#include "editor_all.h"
#else
	#include "build_defines.h"
#endif

#ifdef JA2EDITOR

#ifndef PRECOMPILEDHEADERS
	#include "tile_def.h"
	#include "edit_sys.h"
	#include "vsurface.h"
	#include "vobject.h"
	#include "mouse_system.h"
	#include "button_system.h"
	#include "wcheck.h"
	#include "vsurface.h"
	#include "line.h"
	#include "input.h"
	#include "sysutil.h"
	#include "font.h"
	#include "font_control.h"
	#include "edit_screen.h"
	#include "select_win.h"
	#include "video.h"
	#include "vobject_blitters.h"
	#include "interface_panels.h"
	#include "interface_items.h"
	#include "utilities.h"
	#include "world_items.h"
	#include "world_man.h"
	#include "overhead.h"	//GetSoldier
	#include "render_world.h"
	#include "animation_data.h"
	#include "animation_control.h"
	#include "lighting.h"
	#include "editor_defines.h"
	#include "editor_mercs.h"
	#include "editor_terrain.h" //for access to TerrainTileDrawMode
	#include "soldier_create.h" //The stuff that connects the editor generated information
	#include "soldier_init_list.h"
	#include "strategic_map.h"
	#include "soldier_add.h"
	#include "soldier_control.h"
	#include "soldier_profile_type.h"
	#include "soldier_profile.h"
	#include "text_input.h"
	#include "random.h"
	#include "word_wrap.h"
	#include "editor_items.h"
	#include "editor_taskbar_utils.h"
	#include "exit_grids.h"
	#include "editor_undo.h"
	#include "item_statistics.h"
	#include "map_information.h"
	#include "editor_map_info.h"
	#include "environment.h"
	#include "simple_render_utils.h"
	#include "platform.h"
	#include "sgp_str.h"
	
#endif

INT8 gbDefaultLightType = PRIMETIME_LIGHT;

SGPPaletteEntry	gEditorLightColor;

BOOLEAN gfEditorForceShadeTableRebuild = FALSE;

void SetupTextInputForMapInfo()
{
	CHAR16 str[10];
	
	InitTextInputModeWithScheme( DEFAULT_SCHEME );

	AddUserInputField( NULL );  //just so we can use short cut keys while not typing.

	//light rgb fields
	WSTR_SPrintf( str, WSTRLEN(str), L"%d", gEditorLightColor.peRed );
	AddTextInputField( iScreenWidthOffset + 10, 2 * iScreenHeightOffset + 394, 25, 18, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	WSTR_SPrintf( str, WSTRLEN(str), L"%d", gEditorLightColor.peGreen );
	AddTextInputField( iScreenWidthOffset + 10, 2 * iScreenHeightOffset + 414, 25, 18, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	WSTR_SPrintf( str, WSTRLEN(str), L"%d", gEditorLightColor.peBlue );
	AddTextInputField( iScreenWidthOffset + 10, 2 * iScreenHeightOffset + 434, 25, 18, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );

	WSTR_SPrintf( str, WSTRLEN(str), L"%d", gsLightRadius );
	AddTextInputField( iScreenWidthOffset + 120, 2 * iScreenHeightOffset + 394, 25, 18, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_NUMERICSTRICT );
	WSTR_SPrintf( str, WSTRLEN(str), L"%d", gusLightLevel );
	AddTextInputField( iScreenWidthOffset + 120, 2 * iScreenHeightOffset + 414, 25, 18, MSYS_PRIORITY_NORMAL, str, 2, INPUTTYPE_NUMERICSTRICT );

	//Scroll restriction ID
	if( !gMapInformation.ubRestrictedScrollID )
		WSTR_SPrintf( str, WSTRLEN(str), L"" );
	else
		WSTR_SPrintf( str, WSTRLEN(str), L"%d", gMapInformation.ubRestrictedScrollID );
	AddTextInputField( iScreenWidthOffset + 210, 2 * iScreenHeightOffset + 420, 30, 20, MSYS_PRIORITY_NORMAL, str, 2, INPUTTYPE_NUMERICSTRICT );

	//exit grid input fields
	WSTR_SPrintf( str, WSTRLEN(str), L"%c%d", gExitGrid.ubGotoSectorY + 'A' - 1, gExitGrid.ubGotoSectorX );
	AddTextInputField( iScreenWidthOffset + 338, 2 * iScreenHeightOffset + 363, 30, 18, MSYS_PRIORITY_NORMAL, str, 3, INPUTTYPE_EXCLUSIVE_COORDINATE );
	WSTR_SPrintf( str, WSTRLEN(str), L"%d", gExitGrid.ubGotoSectorZ );
	AddTextInputField( iScreenWidthOffset + 338, 2 * iScreenHeightOffset + 383, 30, 18, MSYS_PRIORITY_NORMAL, str, 1, INPUTTYPE_NUMERICSTRICT );
	WSTR_SPrintf( str, WSTRLEN(str), L"%d", gExitGrid.usGridNo );
	AddTextInputField( iScreenWidthOffset + 338, 2 * iScreenHeightOffset + 403, 40, 18, MSYS_PRIORITY_NORMAL, str, 5, INPUTTYPE_NUMERICSTRICT );
}

void UpdateMapInfo()
{
	SetFont( FONT10ARIAL );
	SetFontShadow( FONT_NEARBLACK );

	SetFontForeground( FONT_RED );
	mprintf( iScreenWidthOffset + 38, 2 * iScreenHeightOffset + 399, L"R");
	SetFontForeground( FONT_GREEN );
	mprintf( iScreenWidthOffset + 38, 2 * iScreenHeightOffset + 419, L"G");
	SetFontForeground( FONT_DKBLUE );
	mprintf( iScreenWidthOffset + 38, 2 * iScreenHeightOffset + 439, L"B");

	SetFontForeground( FONT_YELLOW );
	mprintf( iScreenWidthOffset + 65, 2 * iScreenHeightOffset + 369, L"Prime" );
	mprintf( iScreenWidthOffset + 65, 2 * iScreenHeightOffset + 382, L"Night" );
	mprintf( iScreenWidthOffset + 65, 2 * iScreenHeightOffset + 397, L"24Hrs" );

	SetFontForeground( FONT_YELLOW );
	mprintf( iScreenWidthOffset + 148, 2 * iScreenHeightOffset + 399, L"Radius");

	if( !gfBasement && !gfCaves )
		SetFontForeground( FONT_DKYELLOW );
	mprintf( iScreenWidthOffset + 148, 2 * iScreenHeightOffset + 414, L"Underground" );
	mprintf( iScreenWidthOffset + 148, 2 * iScreenHeightOffset + 423, L"Light Level" );

	SetFontForeground( FONT_YELLOW );
	mprintf( iScreenWidthOffset + 230, 2 * iScreenHeightOffset + 369, L"Outdoors" );
	mprintf( iScreenWidthOffset + 230, 2 * iScreenHeightOffset + 384, L"Basement" );
	mprintf( iScreenWidthOffset + 230, 2 * iScreenHeightOffset + 399, L"Caves" );

	SetFontForeground( FONT_ORANGE );
	mprintf( iScreenWidthOffset + 250, 2 * iScreenHeightOffset + 420, L"Restricted" );
	mprintf( iScreenWidthOffset + 250, 2 * iScreenHeightOffset + 430, L"Scroll ID" );

	SetFontForeground( FONT_YELLOW );
	mprintf( iScreenWidthOffset + 368, 2 * iScreenHeightOffset + 363, L"Destination");
	mprintf( iScreenWidthOffset + 368, 2 * iScreenHeightOffset + 372, L"Sector" );
	mprintf( iScreenWidthOffset + 368, 2 * iScreenHeightOffset + 383, L"Destination");
	mprintf( iScreenWidthOffset + 368, 2 * iScreenHeightOffset + 392, L"Bsmt. Level" );
	mprintf( iScreenWidthOffset + 378, 2 * iScreenHeightOffset + 403, L"Dest." );
	mprintf( iScreenWidthOffset + 378, 2 * iScreenHeightOffset + 412, L"GridNo" );
	SetFontForeground( FONT_RED );
}

void UpdateMapInfoFields()
{
	CHAR16 str[10];
	//Update the text fields to reflect the validated values.
	//light rgb fields
	WSTR_SPrintf( str, WSTRLEN(str), L"%d", gEditorLightColor.peRed );
	SetInputFieldStringWith16BitString( 1, str );
	WSTR_SPrintf( str, WSTRLEN(str), L"%d", gEditorLightColor.peGreen );
	SetInputFieldStringWith16BitString( 2, str );
	WSTR_SPrintf( str, WSTRLEN(str), L"%d", gEditorLightColor.peBlue );
	SetInputFieldStringWith16BitString( 3, str );

	WSTR_SPrintf( str, WSTRLEN(str), L"%d", gsLightRadius );
	SetInputFieldStringWith16BitString( 4, str );
	WSTR_SPrintf( str, WSTRLEN(str), L"%d", gusLightLevel );
	SetInputFieldStringWith16BitString( 5, str );

	if( !gMapInformation.ubRestrictedScrollID )
		WSTR_SPrintf( str, WSTRLEN(str), L"" );
	else
		WSTR_SPrintf( str, WSTRLEN(str), L"%d", gMapInformation.ubRestrictedScrollID );
	SetInputFieldStringWith16BitString( 6, str );

	ApplyNewExitGridValuesToTextFields();
}

void ExtractAndUpdateMapInfo()
{
	CHAR16 str[10];
	INT32 temp;
	BOOLEAN fUpdateLight1 = FALSE;
	//extract light1 colors
	temp = SGP_min( GetNumericStrictValueFromField( 1 ), 255 );
	if( temp != -1 && temp != gEditorLightColor.peRed )
	{
		fUpdateLight1 = TRUE;
		gEditorLightColor.peRed = (UINT8)temp;
	}
	temp = SGP_min( GetNumericStrictValueFromField( 2 ), 255 );
	if( temp != -1 && temp != gEditorLightColor.peGreen )
	{
		fUpdateLight1 = TRUE;
		gEditorLightColor.peGreen = (UINT8)temp;
	}
	temp = SGP_min( GetNumericStrictValueFromField( 3 ), 255 );
	if( temp != -1 && temp != gEditorLightColor.peBlue )
	{
		fUpdateLight1 = TRUE;
		gEditorLightColor.peBlue = (UINT8)temp;
	}
	if( fUpdateLight1 )
	{
		gfEditorForceShadeTableRebuild = TRUE;
		LightSetColors(	&gEditorLightColor, 1 );
		gfEditorForceShadeTableRebuild = FALSE;
	}

	//extract radius
	temp = SGP_max( SGP_min( GetNumericStrictValueFromField( 4 ), 8 ), 1 );
	if( temp != -1 )
		gsLightRadius = (INT16)temp;
	temp = SGP_max( SGP_min( GetNumericStrictValueFromField( 5 ), 15 ), 1 );
	if( temp != -1 && temp != gusLightLevel )
	{
		gusLightLevel = (UINT16)temp;
		gfRenderWorld = TRUE;
		ubAmbientLightLevel = (UINT8)(EDITOR_LIGHT_MAX - gusLightLevel);
		LightSetBaseLevel( ubAmbientLightLevel );
		LightSpriteRenderAll();
	}

	temp = (INT8)GetNumericStrictValueFromField( 6 );
	if( temp == -1 )
		gMapInformation.ubRestrictedScrollID = 0;
	else
		gMapInformation.ubRestrictedScrollID = (UINT8)temp;
	
	//set up fields for exitgrid information 
	Get16BitStringFromField( 7, str );
	if( str[0] >= 'a' && str[0] <= 'z' )
		str[0] -= 32; //uppercase it!
	if( str[0] >= 'A' && str[0] <= 'Z' && 
		  str[1] >= '0' && str[1] <= '9' )
	{ //only update, if coordinate is valid.
		gExitGrid.ubGotoSectorY = (UINT8)(str[0] - 'A' + 1);
		gExitGrid.ubGotoSectorX = (UINT8)(str[1] - '0');
		if( str[2] >= '0' && str[2] <= '9' )
			gExitGrid.ubGotoSectorX = (UINT8)(gExitGrid.ubGotoSectorX * 10 + str[2] - '0' );
		gExitGrid.ubGotoSectorX = (UINT8)SGP_max( SGP_min( gExitGrid.ubGotoSectorX, 16 ), 1 );
		gExitGrid.ubGotoSectorY = (UINT8)SGP_max( SGP_min( gExitGrid.ubGotoSectorY, 16 ), 1 );
	}
	gExitGrid.ubGotoSectorZ    = (UINT8)SGP_max( SGP_min( GetNumericStrictValueFromField( 8 ), 3 ), 0 );
	gExitGrid.usGridNo					 = (UINT16)SGP_max( SGP_min( GetNumericStrictValueFromField( 9 ), 25600 ), 0 );

	UpdateMapInfoFields();
}

BOOLEAN ApplyNewExitGridValuesToTextFields()
{
	CHAR16 str[10];
	//exit grid input fields
	if( iCurrentTaskbar != TASK_MAPINFO )
		return FALSE;
	WSTR_SPrintf( str, WSTRLEN(str), L"%c%d", gExitGrid.ubGotoSectorY + 'A' - 1, gExitGrid.ubGotoSectorX );
	SetInputFieldStringWith16BitString( 7, str );
	WSTR_SPrintf( str, WSTRLEN(str), L"%d", gExitGrid.ubGotoSectorZ );
	SetInputFieldStringWith16BitString( 8, str );
	WSTR_SPrintf( str, WSTRLEN(str), L"%d", gExitGrid.usGridNo );
	SetInputFieldStringWith16BitString( 9, str );
	SetActiveField( 0 );
	return TRUE;
}

UINT16 usCurrentExitGridNo = 0;
void LocateNextExitGrid()
{
	EXITGRID ExitGrid;
	UINT16 i;
	for( i = usCurrentExitGridNo + 1; i < WORLD_MAX; i++ )
	{
		if( GetExitGrid( i, &ExitGrid ) )
		{
			usCurrentExitGridNo = i;
			CenterScreenAtMapIndex( i );
			return;
		}
	}
	for( i = 0; i < usCurrentExitGridNo; i++ )
	{
		if( GetExitGrid( i, &ExitGrid ) )
		{
			usCurrentExitGridNo = i;
			CenterScreenAtMapIndex( i );
			return;
		}
	}
}

void ChangeLightDefault( INT8 bLightType )
{
	UnclickEditorButton( MAPINFO_PRIMETIME_LIGHT + gbDefaultLightType );
	gbDefaultLightType = bLightType;
	ClickEditorButton( MAPINFO_PRIMETIME_LIGHT + gbDefaultLightType );
}

#endif



 


