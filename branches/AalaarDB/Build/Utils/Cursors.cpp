#ifdef PRECOMPILEDHEADERS
	#include "Utils All.h"
	#include "interface items.h"
#else
	#include "builddefines.h"
	#include <wchar.h>
	#include "sgp.h"
	#include "cursors.h"
	#include "Timer Control.h"
	#include "jascreens.h"
	#include "font.h"
	#include "Font Control.h"
	#include "Sys Globals.h"
	#include "Handle UI.h"
	#include "interface.h"
	#include "overhead.h"
	#include "Cursor Control.h"
	#include "Sound Control.h"
#endif

//aim
extern UINT8	gubShowActionPointsInRed;
UINT8 gpShadesFromWhiteToRed[] = {FONT_MCOLOR_WHITE, FONT_MCOLOR_DKWHITE, FONT_MCOLOR_DKGRAY, FONT_MCOLOR_DKRED, FONT_MCOLOR_RED };
//aim

#define NUM_MOUSE_LEVELS		2

INT16 gsMouseGlobalYOffsets[ NUM_MOUSE_LEVELS ] =
{
	0, 50
};


void UpdateFlashingCursorFrames( UINT32 uiCursorIndex );


extern MOUSE_REGION	gDisableRegion;
extern MOUSE_REGION gUserTurnRegion;

void BltJA2CursorData( );

void DrawMouseText( );

CursorFileData CursorFileDatabase[] =
{
	{ "CURSORS\\cursor.sti"										, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\cur_targ.sti"									, FALSE, 0, ANIMATED_CURSOR,	7, NULL },
	{ "CURSORS\\cur_tagr.sti"									, FALSE, 0, ANIMATED_CURSOR,	7, NULL },
	{ "CURSORS\\targblak.sti"									, FALSE, 0, ANIMATED_CURSOR,	7, NULL },
	{ "CURSORS\\cur_bst.sti"										, FALSE, 0, ANIMATED_CURSOR,	7, NULL },
	{ "CURSORS\\cur_rbst.sti"									, FALSE, 0, ANIMATED_CURSOR,	7, NULL },
	{ "CURSORS\\burstblk.sti"									, FALSE, 0, ANIMATED_CURSOR,	7, NULL },
	{ "CURSORS\\cur_tr.sti"										, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\cur_trw.sti"										, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\cur_tb.sti"										, FALSE, 0, ANIMATED_CURSOR,	7, NULL },

	{ "CURSORS\\punch.sti"										, FALSE, 0, ANIMATED_CURSOR,	6, NULL },
	{ "CURSORS\\punchr.sti"										, FALSE, 0, ANIMATED_CURSOR,	6, NULL },
	{ "CURSORS\\cur_run.sti"										, FALSE, 0, ANIMATED_CURSOR,	10, NULL },
	{ "CURSORS\\cur_walk.sti"									, FALSE, 0, ANIMATED_CURSOR,	10, NULL },
	{ "CURSORS\\cur_swat.sti"									, FALSE, 0, ANIMATED_CURSOR,	10, NULL },
	{ "CURSORS\\cur_pron.sti"									, FALSE, 0, ANIMATED_CURSOR,	10, NULL },
	{ "CURSORS\\grabsr.sti"										, FALSE, 0, 0,	0, NULL },
	{ "CURSORS\\grabs.sti"										, FALSE, 0, 0,	0, NULL },
	{ "CURSORS\\stab.sti"											, FALSE, 0, ANIMATED_CURSOR,	6, NULL },
	{ "CURSORS\\stabr.sti"										, FALSE, 0, ANIMATED_CURSOR,	6, NULL },

	{ "CURSORS\\cross1.sti"										, FALSE, 0, ANIMATED_CURSOR,	6, NULL },
	{ "CURSORS\\cross2.sti"									, FALSE, 0, ANIMATED_CURSOR,	6, NULL },
	{ "LAPTOP\\FingerCursor.sti"										, FALSE, 0, 0, 0, NULL },
	{ "LAPTOP\\LapTopCursor.sti"										, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\ibeam.sti"										, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\cur_look.sti"									, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\cur_talk.sti"									, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\cur_talkb.sti"								, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\cur_talkr.sti"								, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\cur_exit.sti"									, FALSE, 0, 0, 0, NULL },

	{ "CURSORS\\VehicleCursor.sti"									, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\WalkingCursor.sti"									, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\que.sti"											, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\chopper.sti"											, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\check.sti"												, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\cur_try.sti"											, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\wirecut.sti"											, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\wirecutr.sti"											, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\bullet_g.sti"											, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\bullet_d.sti"											, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\ibeamWhite.sti"										, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\throwg.sti"												, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\throwb.sti"												, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\throwr.sti"												, FALSE, 0, 0, 0, NULL },
	{ ""																					, FALSE, 0, USE_EXTERN_VO_CURSOR | USE_OUTLINE_BLITTER, 0, NULL },
	{ "CURSORS\\bombg.sti"												, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\bombr.sti"												, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\remoteg.sti"											, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\remoter.sti"											, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\steering.sti"											, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\cur_car.sti"											, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\cur_wait.sti"											, FALSE, 0, 0, 0, NULL },
	
	//Tactical GUI cursors
	{ "CURSORS\\singlecursor.sti"									, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\groupcursor.sti"									, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\singledcursor.sti"								, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\groupdcursor.sti"									, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\repair.sti"												, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\repairr.sti"											, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\jar_cur.sti"											, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\jar_cur_red.sti"									, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\cur_x.sti"												, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\can_01.sti"												, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\can_02.sti"												, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\cur_swit.sti"											, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\bullseye.sti"											, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\deadleap.sti"											, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\can_01.sti"												, FALSE, 0, 0, 0, NULL },
	{ "CURSORS\\can_02.sti"												, FALSE, 0, 0, 0, NULL },

};

void RaiseMouseToLevel( INT8 bLevel )
{
	gsGlobalCursorYOffset = gsMouseGlobalYOffsets[ bLevel ];
}

CursorData CursorDatabase[] =
{
	{ C_MISC,					0, 0, 0, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1,	0,	0, 0, 0									, 0, 0 },
		
	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
	//{ C_TRINGS,				6, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_ACTIONMODE,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

		// TARGET ( NORMAL W/ RINGS )
	{ 
		C_ACTIONMODERED,			0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_ACTIONMODEBLACK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,		
		C_TRINGS,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_ACTIONMODERED,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_ACTIONMODEBLACK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,		
		C_TRINGS,				5, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_ACTIONMODERED,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_ACTIONMODEBLACK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,		
		C_TRINGS,				1, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },


	{ 
		C_ACTIONMODERED,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_ACTIONMODEBLACK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,		
		C_TRINGS,				6, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		3,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_ACTIONMODERED,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_ACTIONMODEBLACK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,		
		C_TRINGS,				2, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_ACTIONMODERED,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_ACTIONMODEBLACK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				3, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_ACTIONMODERED,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_ACTIONMODEBLACK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				7, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_ACTIONMODERED,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_ACTIONMODEBLACK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				8, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_ACTIONMODERED,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_ACTIONMODEBLACK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				4, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

		// TARGET WITH WHITE RINGS
	{ 
		C_ACTIONMODERED,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_ACTIONMODEBLACK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TWRINGS,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },


	{ 
		C_ACTIONMODERED,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_ACTIONMODEBLACK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TWRINGS,				1, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_ACTIONMODERED,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_ACTIONMODEBLACK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TWRINGS,				2, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_ACTIONMODERED,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_ACTIONMODEBLACK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_TWRINGS,				3, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_ACTIONMODERED,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_ACTIONMODEBLACK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_TWRINGS,				4, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

		// TARGET RED CURSOR
	{ C_ACTIONMODERED,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },


		// TARGET BLACK CURSOR
	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_BLACKTARGET,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },


		// TARGET DARK BLACK CURSOR
	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_ACTIONMODEBLACK,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

		// TARGET RED CURSOR

	{ 
		C_TARGMODEBURSTRED,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TARGMODEBURSTBLACK,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				1, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				2, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				3, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		5,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ C_TARGMODEBURST,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ C_TARGMODEBURSTRED,	0, 0, 0, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },


	{ C_TARGMODEBURSTBLACK,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_PUNCHGRAY,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR	, 0, 0									, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_PUNCHRED,			0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR	, 0, 0									, 0, 0 },

	{ C_PUNCHRED,			0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				1, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR	, 0, 0									, 0, 0 },

	{	C_PUNCHRED,			0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				2, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR	, 0, 0									, 0, 0 },

	{ C_PUNCHRED,			0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_YELLOWRINGS,	1, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR	, 0, 0									, 0, 0 },

	{	C_PUNCHRED,			0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_YELLOWRINGS,	2, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR	, 0, 0									, 0, 0 },


	{
		C_PUNCHRED,			0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TWRINGS,			1, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR	, 0, 0									, 0, 0 },

	{	
		C_PUNCHRED,			0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TWRINGS,			2, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR	, 0, 0									, 0, 0 },


	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR,		HIDE_SUBCURSOR, 
		C_RUN1,					0, 0, CENTER_SUBCURSOR, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, 20	, 0, 0									, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR,		HIDE_SUBCURSOR, 
		C_WALK1,				0, 0, CENTER_SUBCURSOR, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, 20 , 0, 0					, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR,		HIDE_SUBCURSOR, 
		C_SWAT1,				0, 0, CENTER_SUBCURSOR, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, 10	, 0, 0									, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR,		HIDE_SUBCURSOR, 
		C_PRONE1,				0, 0, CENTER_SUBCURSOR, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, 10 , 0, 0									, 0, 0 },

	{ C_TRINGS,				0, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
	C_GRAB1,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR	, 0, 0									, 0, 0 },

	{ C_TRINGS,				0, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_GRAB2,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR	, 0, 0									, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_KNIFE1,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR	, 0, 0							, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_KNIFE2,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR	, 0, 0									, 0, 0 },

	{	C_KNIFE2,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				1, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR , 0, 0									, 0, 0 },

	{	C_KNIFE2,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				2, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR , 0, 0									, 0, 0 },

	{	C_KNIFE2,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_YELLOWRINGS,	1, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR , 0, 0									, 0, 0 },

	{	C_KNIFE2,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_YELLOWRINGS,	2, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR , 0, 0									, 0, 0 },

	{	C_KNIFE2,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TWRINGS,			1, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR , 0, 0									, 0, 0 },

	{	C_KNIFE2,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TWRINGS,			2, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR , 0, 0									, 0, 0 },

	{ C_CROSS1,					0, 0, 0, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1, CENTER_CURSOR, CENTER_CURSOR	, 0, 0									, 0, 0 },

	{ C_CROSS2,					0, 0, 0, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1, CENTER_CURSOR, CENTER_CURSOR	, 0, 0									, 0, 0 },
	{ C_WWW,					0, 0, 0, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1,	0,	0, 0, 0									, 0, 0 },
	{ C_LAPTOPSCREEN,					0, 0, 0, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1,	0,	0, 0, 0									, 0, 0 },
	{ C_IBEAM,				0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1, CENTER_CURSOR, CENTER_CURSOR, 0, 0 , 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
	C_LOOK,					0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR, 0, 0 , 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_TALK,					0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR, 0, 0 , 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_BLACKTALK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR, 0, 0 , 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_REDTALK,			0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR, 0, 0 , 0, 0 },

		// Exit arrows
	{ C_EXITARROWS	,	0, 0, 0, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1,	CENTER_CURSOR, TOP_CURSOR, 0, 0	, 0, 0 },

	{ C_EXITARROWS	,	1, 0, 0, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1,	CENTER_CURSOR, BOTTOM_CURSOR, 0, 0	, 0, 0 },

	{ C_EXITARROWS	,	2, 0, 0, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1,	RIGHT_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ C_EXITARROWS	,	3, 0, 0, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1,	LEFT_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ C_EXITARROWS	,	4, 0, 0, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1,	CENTER_CURSOR, TOP_CURSOR, 0, 0	, 0, 0 },

	{ C_EXITARROWS	,	5, 0, 0, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1,	CENTER_CURSOR, BOTTOM_CURSOR, 0, 0	, 0, 0 },

	{ C_EXITARROWS	,	6, 0, 0, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1,	RIGHT_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ C_EXITARROWS	,	7, 0, 0, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1,	LEFT_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ C_EXITARROWS	,	8, 0, 0, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1,	CENTER_CURSOR, TOP_CURSOR, 0, 0	, 0, 0 },

	{ C_EXITARROWS	,	9, 0, 0, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1,	CENTER_CURSOR, BOTTOM_CURSOR, 0, 0	, 0, 0 },

	{ C_EXITARROWS	,	10, 0, 0, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1,	RIGHT_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ C_EXITARROWS	,	11, 0, 0, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1,	LEFT_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_STRATVEH	,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
	C_STRATFOOT	,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ C_TRINGS,					6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
	C_INVALIDACTION	,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,								0, 0, 0, 0,
		0,								0, 0, 0, 0,
		0,								0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },
	{ C_TRINGS,					6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
	C_CHOPPER	,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,								0, 0, 0, 0,
		0,								0, 0, 0, 0,
		0,								0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ 
		C_ACTIONMODE,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_ACTIONMODEBLACK,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		3,	CENTER_CURSOR, CENTER_CURSOR, 0, 0,	CURSOR_TO_FLASH, 0 },

	{ C_TARGMODEBURST,									0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TARGMODEBURSTBLACK,							0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		0,																0, 0, 0, 0,
		0,																0, 0, 0, 0,
		0,																0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0,	CURSOR_TO_FLASH, 0 },

	{ 
		C_TALK,					0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_BLACKTALK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		3, CENTER_CURSOR, CENTER_CURSOR, 0, 0, CURSOR_TO_FLASH, 0 },

	{ 
		C_REDTALK,					0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_BLACKTALK,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,									0, 0, 0, 0,
		0,									0, 0, 0, 0,
		3, CENTER_CURSOR, CENTER_CURSOR, 0, 0, CURSOR_TO_FLASH, 0 },

	{ 
		C_CHECKMARK,					0, 0, 0, 0,
		0,				0, 0, 0, 0,
		0,				0, 0, 0, 0, 
		0,									0, 0, 0, 0,
		0,									0, 0, 0, 0,
		1, CENTER_CURSOR, CENTER_CURSOR, 0, 0, 0, 0 },

	{ 
		C_ACTIONMODERED,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_ACTIONMODEBLACK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_YELLOWRINGS,				5, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_ACTIONMODERED,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_ACTIONMODEBLACK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_YELLOWRINGS,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_ACTIONMODERED,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_ACTIONMODEBLACK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_YELLOWRINGS,				1, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_ACTIONMODERED,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_ACTIONMODEBLACK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_YELLOWRINGS,				2, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_ACTIONMODERED,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_ACTIONMODEBLACK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_YELLOWRINGS,				3, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ C_EXITARROWS	,	12, 0, 0, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ C_EXITARROWS	,	13, 0, 0, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ C_EXITARROWS	,	14, 0, 0, 0, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_WIRECUTR	,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_WIRECUT	,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ 
		C_ACTIONMODEBLACK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_RELOAD						,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,							6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		3,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_FLASH, 0 },

	{ 
		C_ACTIONMODEBLACK,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_RELOADR	,			0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		3,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_FLASH, 0 },

	{ C_IBEAM_WHITE,	0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1, CENTER_CURSOR, CENTER_CURSOR, 0, 0 , 0, 0 },

	{ 
		C_THROWG,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ 
		C_THROWB,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ 
		C_THROWR,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ 
		C_THROWG,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWB,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		3,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_FLASH, 0 },


	// THROW CURSORS W/ RINGS
	{ 
		C_THROWR,			0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWB,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,		
		C_TRINGS,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_THROWR,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWB,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,		
		C_TRINGS,				5, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_THROWR,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWB,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,		
		C_TRINGS,				1, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },


	{ 
		C_THROWR,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWB,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,		
		C_TRINGS,				6, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		3,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_THROWR,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWB,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,		
		C_TRINGS,				2, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_THROWR,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWB,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				3, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_THROWR,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWB,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				7, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_THROWR,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWB,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				8, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_THROWR,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWB,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				4, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

		// TARGET WITH WHITE RINGS
	{ 
		C_THROWR,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWB,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TWRINGS,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },


	{ 
		C_THROWR,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWB,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TWRINGS,				1, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_THROWR,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWB,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TWRINGS,				2, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_THROWR,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWB,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_TWRINGS,				3, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_THROWR,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWB,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_TWRINGS,				4, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

		// YELLOW RINGS
	{ 
		C_THROWR,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWB,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_YELLOWRINGS,				5, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_THROWR,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWB,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_YELLOWRINGS,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_THROWR,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWB,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_YELLOWRINGS,				1, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_THROWR,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWB,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_YELLOWRINGS,				2, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	{ 
		C_THROWR,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWB,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_YELLOWRINGS,				3, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_SUB_CONDITIONALLY, 0 },

	// ITEM THROW ONES...
	{ 
		C_ITEMTHROW,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWG,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				0, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		3,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ 
		C_THROWB,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ 
		C_ITEMTHROW,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWR,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		3,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ 
		C_ITEMTHROW,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWG,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_THROWB,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		4,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_FLASH2, 0 },

	{ 
		C_ITEMTHROW,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, CURSOR_TO_FLASH2, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_BOMB_GREY,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR	, 0, 0									, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_BOMB_RED	,		0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR	, 0, 0									, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_REMOTE_GREY,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR	, 0, 0									, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_REMOTE_RED	,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR	, 0, 0									, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_ENTERV,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR,		HIDE_SUBCURSOR, 
		C_MOVEV,				0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR , 0, 0					, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR,		HIDE_SUBCURSOR, 
		C_WAIT,					0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR , 0, 0					, DELAY_START_CURSOR, 0 },

	//Tactical Placement GUI cursors
	{ C_PLACEMERC,		0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1, CENTER_CURSOR, BOTTOM_CURSOR, 0, 0, 0, 0 },
	{ C_PLACEGROUP,		0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1, CENTER_CURSOR, BOTTOM_CURSOR, 0, 0, 0, 0 },
	{ C_DPLACEMERC,		0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1, CENTER_CURSOR, BOTTOM_CURSOR, 0, 0, 0, 0 },

	{ C_DPLACEGROUP,		0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1, CENTER_CURSOR, BOTTOM_CURSOR, 0, 0, 0, 0 },
		
	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_REPAIR			,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_REPAIRR			,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_JAR					,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_JARRED			,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_CAN					,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_CANRED			,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{	
		C_X,						0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1, CENTER_CURSOR, CENTER_CURSOR	, 0, 0									, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR,		HIDE_SUBCURSOR, 
		C_WAIT,					0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR , 0, 0					, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR,		HIDE_SUBCURSOR, 
		C_EXCHANGE,			0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR , 0, 0					, 0, 0 },

	{ C_BULLSEYE,			0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		1,	CENTER_CURSOR, CENTER_CURSOR, 0, 0,									0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR,		HIDE_SUBCURSOR, 
		C_JUMPOVER,			0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2, CENTER_CURSOR, CENTER_CURSOR , 0, 0					, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_FUEL				,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },

	{ C_TRINGS,				6, 0, HIDE_SUBCURSOR, HIDE_SUBCURSOR, 
		C_FUEL_RED		,	0, 0, CENTER_SUBCURSOR, CENTER_SUBCURSOR, 
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		0,							0, 0, 0, 0,
		2,	CENTER_CURSOR, CENTER_CURSOR, 0, 0	, 0, 0 },


};


void InitCursors( )
{
		InitCursorDatabase( CursorFileDatabase, CursorDatabase, NUM_CURSOR_FILES );

		SetMouseBltHook( (MOUSEBLT_HOOK)BltJA2CursorData );
}


void HandleAnimatedCursors( )
{

	if ( COUNTERDONE( CURSORCOUNTER ) )
	{
		RESETCOUNTER( CURSORCOUNTER );

		if ( gViewportRegion.uiFlags & MSYS_MOUSE_IN_AREA )
		{
			UpdateAnimatedCursorFrames( gViewportRegion.Cursor );
			SetCurrentCursorFromDatabase(	gViewportRegion.Cursor	);
		}


		if ( gDisableRegion.uiFlags & MSYS_MOUSE_IN_AREA )
		{
			UpdateAnimatedCursorFrames( gDisableRegion.Cursor );
			SetCurrentCursorFromDatabase(	gDisableRegion.Cursor	);
		}


		if ( gUserTurnRegion.uiFlags & MSYS_MOUSE_IN_AREA )
		{
			UpdateAnimatedCursorFrames( gUserTurnRegion.Cursor );
			SetCurrentCursorFromDatabase(	gUserTurnRegion.Cursor	);
		}

	}

	if ( COUNTERDONE( CURSORFLASHUPDATE ) )
	{
		RESETCOUNTER( CURSORFLASHUPDATE );

		if ( gViewportRegion.uiFlags & MSYS_MOUSE_IN_AREA )
		{
			UpdateFlashingCursorFrames( gViewportRegion.Cursor );
			SetCurrentCursorFromDatabase(	gViewportRegion.Cursor	);
		}

	}


}

/*extern INT16					gsCurMouseOffsetX;
extern INT16 				gsCurMouseOffsetY;
extern UINT16				gsCurMouseHeight;
extern UINT16				gsCurMouseWidth;*/

void DrawMouseGraphics( )
{
	UINT16 * ptrBuf;
	UINT32 uiPitch;
	UINT32 cnt;
	UINT32 actualPct		= __min(gbCtH,99);
	UINT16 usCBorderTop		= Get16BPPColor( FROMRGB( 155, 155, 155 ) );
	UINT16 usCBorderBottom	= Get16BPPColor( FROMRGB( 120, 120, 120 ) );
	UINT16 usCBar			= Get16BPPColor( FROMRGB( 255, 255-255*actualPct/99, 0 ) );
	UINT16 usCBack			= Get16BPPColor( FROMRGB( 155, 155-155*actualPct/99, 0 ) );
	UINT16 usCBar2			= Get16BPPColor( FROMRGB( 180, 140-140*actualPct/99, 0 ) );
	UINT16 usCBack2			= Get16BPPColor( FROMRGB( 110, 100-100*actualPct/99, 0 ) );
	UINT32 barLength		= __min(35,gsCurMouseWidth);
	UINT32 barY				= gsCurMouseOffsetY-__min(35,gsCurMouseHeight)/2;

	if(gfUICtHBar)
	{
		ptrBuf = (UINT16 *) LockMouseBuffer( &uiPitch );
		uiPitch >>= 1;

		for(cnt = gsCurMouseOffsetX+barLength/2;cnt > gsCurMouseOffsetX-barLength/2+1;cnt--)
		{
			ptrBuf[cnt-1 + uiPitch*(3+barY)] = usCBorderBottom;
			ptrBuf[cnt-1 + uiPitch*barY] = usCBorderTop;	
		}

		ptrBuf[gsCurMouseOffsetX+barLength/2 + uiPitch*(1+barY)] = usCBorderBottom;
		ptrBuf[gsCurMouseOffsetX-barLength/2 + uiPitch*(1+barY)] = usCBorderTop;

		ptrBuf[gsCurMouseOffsetX+barLength/2 + uiPitch*(2+barY)] = usCBorderBottom;
		ptrBuf[gsCurMouseOffsetX-barLength/2 + uiPitch*(2+barY)] = usCBorderTop;


		for(cnt = 0;cnt < (barLength-2)*actualPct/99;cnt++)
		{
			ptrBuf[cnt + gsCurMouseOffsetX-barLength/2+1 + uiPitch*(barY+2)] = usCBar2;
			ptrBuf[cnt + gsCurMouseOffsetX-barLength/2+1 + uiPitch*(barY+1)] = usCBar;
		}

		for(cnt = (barLength-2)*actualPct/99;cnt < (barLength-2);cnt++)
		{
			ptrBuf[cnt + gsCurMouseOffsetX-barLength/2+1 + uiPitch*(barY+2)] = usCBack2;
			ptrBuf[cnt + gsCurMouseOffsetX-barLength/2+1 + uiPitch*(barY+1)] = usCBack;
		}


		UnlockMouseBuffer();
	}

}

void BltJA2CursorData( )
{
	if ( ( gViewportRegion.uiFlags & MSYS_MOUSE_IN_AREA	) )
	{
		DrawMouseText();
		DrawMouseGraphics();
	}
}


void DrawMouseText( )
{
	CHAR16 pStr[ 512 ];
	INT16 sX, sY;
	static BOOLEAN fShow = FALSE;
	static BOOLEAN fHoldInvalid = TRUE;

	//EnterMutex(MOUSE_BUFFER_MUTEX, __LINE__, __FILE__);

	if ( gfUIBodyHitLocation )
	{
		// Set dest for gprintf to be different
		SetFontDestBuffer( MOUSE_BUFFER , 0, 0, 64, 64, FALSE );

		FindFontCenterCoordinates( 0, 0, gsCurMouseWidth, gsCurMouseHeight, gzLocation, TINYFONT1, &sX, &sY );
		SetFont( TINYFONT1 );

		SetFontBackground( FONT_MCOLOR_BLACK );
		SetFontForeground( FONT_MCOLOR_WHITE );

		mprintf( sX, sY + 12, gzLocation );
		// reset
		SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	}

	if ( gfUIAutofireBulletCount )
	{
		// Set dest for gprintf to be different
		SetFontDestBuffer( MOUSE_BUFFER , 0, 0, 64, 64, FALSE );


		SetFont( TINYFONT1 );

		SetFontBackground( FONT_MCOLOR_BLACK );

		if(gTintBulletCounts)
			SetFontForeground( FONT_ORANGE );		
		else
			SetFontForeground( FONT_MCOLOR_LTYELLOW );

		swprintf( pStr, L"%d", gsBulletCount );
		FindFontCenterCoordinates( 0, 0, gsCurMouseWidth, gsCurMouseHeight, pStr, TINYFONT1, &sX, &sY );
		mprintf( sX, sY - 10 - GetFontHeight(TINYFONT1), pStr );

		swprintf( pStr, L"%d", gsTotalBulletCount );
		FindFontCenterCoordinates( 0, 0, gsCurMouseWidth, gsCurMouseHeight, pStr, TINYFONT1, &sX, &sY );
		mprintf( sX, sY + 7 + GetFontHeight(TINYFONT1), pStr );

		// reset
		SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	}

	if ( gfUIIntTileLocation )
	{
		// Set dest for gprintf to be different
		SetFontDestBuffer( MOUSE_BUFFER , 0, 0, 64, 64, FALSE );

		FindFontCenterCoordinates( 0, 0, gsCurMouseWidth, gsCurMouseHeight, gzIntTileLocation, TINYFONT1, &sX, &sY );
		SetFont( TINYFONT1 );

		SetFontBackground( FONT_MCOLOR_BLACK );
		SetFontForeground( FONT_MCOLOR_WHITE );

		mprintf( sX, sY + 6, gzIntTileLocation );
		// reset
		SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );

	}

	if ( gfUIIntTileLocation2 )
	{
		// Set dest for gprintf to be different
		SetFontDestBuffer( MOUSE_BUFFER , 0, 0, 64, 64, FALSE );

		FindFontCenterCoordinates( 0, 0, gsCurMouseWidth, gsCurMouseHeight, gzIntTileLocation2, TINYFONT1, &sX, &sY );
		SetFont( TINYFONT1 );

		SetFontBackground( FONT_MCOLOR_BLACK );
		SetFontForeground( FONT_MCOLOR_WHITE );

		mprintf( sX, sY - 2, gzIntTileLocation2 );
		// reset
		SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
	}


	//if ( ( ( gTacticalStatus.uiFlags & TURNBASED ) && ( gTacticalStatus.uiFlags & INCOMBAT ) ) )
	{
		if ( gfUIDisplayActionPoints )
		{
			if ( gfUIDisplayActionPointsInvalid )
			{
				if ( !fHoldInvalid )
				{
					if ( COUNTERDONE( INVALID_AP_HOLD ) )
					{
						RESETCOUNTER( INVALID_AP_HOLD );
						RESETCOUNTER( CURSORFLASH );
						
						fShow = !fShow;
						fHoldInvalid = !fHoldInvalid;
					}

				}
				else
				{
					if ( COUNTERDONE( CURSORFLASH ) )
					{
						RESETCOUNTER( CURSORFLASH );
						RESETCOUNTER( INVALID_AP_HOLD );
						
						fShow = !fShow;
						fHoldInvalid = !fHoldInvalid;
					}
				}
			}
			else
			{
				fShow = TRUE;
				fHoldInvalid = FALSE;
			}

				// Set dest for gprintf to be different
			SetFontDestBuffer( MOUSE_BUFFER , 0, 0, 64, 64, FALSE );

			swprintf( pStr, L"%d", gsCurrentActionPoints );

			if ( gfUIDisplayActionPointsCenter )
			{
				FindFontCenterCoordinates( 0, 0, gsCurMouseWidth, gsCurMouseHeight, pStr, TINYFONT1, &sX, &sY );
			}
			else
			{
				FindFontCenterCoordinates( gUIDisplayActionPointsOffX, gUIDisplayActionPointsOffY, 1, 1, pStr, TINYFONT1, &sX, &sY );
			}

			SetFont( TINYFONT1 );

			if ( fShow )
			{
				if ( gfUIDisplayActionPointsInvalid )
				{
					SetFontBackground( FONT_MCOLOR_BLACK );
					SetFontForeground( 141 );
					//SetFontShadow( NO_SHADOW );				
					SetFontShadow( DEFAULT_SHADOW );				
				}
				else
				{
					SetFontBackground( FONT_MCOLOR_BLACK );
					SetFontForeground( FONT_MCOLOR_WHITE );
					SetFontShadow( DEFAULT_SHADOW );				

					//aim
					if ( gubShowActionPointsInRed )
					{
						SetFontForeground( gpShadesFromWhiteToRed[ gubShowActionPointsInRed - 1 ] );
						gubShowActionPointsInRed = 0;
					}
					else
						SetFontForeground( FONT_MCOLOR_WHITE );
					//aim
				}
			}
			else
			{
				gfUIDisplayActionPointsBlack = TRUE;
			}

			if ( gfUIDisplayActionPointsBlack )
			{
				//aim
				if ( gubShowActionPointsInRed )
				{
					SetFontForeground( gpShadesFromWhiteToRed[ gubShowActionPointsInRed - 1 ] );
					gubShowActionPointsInRed = 0;
				}
				else
				//aim
				SetFontForeground( FONT_MCOLOR_WHITE );
				SetFontShadow( DEFAULT_SHADOW );				
			}

			mprintf( sX, sY, L"%d", gsCurrentActionPoints );
			//mprintf( sX, sY, L"%d %d", sX, sY );

			//LeaveMutex(MOUSE_BUFFER_MUTEX, __LINE__, __FILE__);

			SetFontShadow( DEFAULT_SHADOW );

			// reset
			SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
		}
	}

	//if ( gpItemPointer != NULL )
#if 0
	{
		if ( gpItemPointer->ubNumberOfObjects > 1 )
		{
			SetFontDestBuffer( MOUSE_BUFFER , 0, 0, 64, 64, FALSE );

			swprintf( pStr, L"x%d", gpItemPointer->ubNumberOfObjects );

			FindFontCenterCoordinates( 0, 0, gsCurMouseWidth, gsCurMouseHeight, pStr, TINYFONT1, &sX, &sY );

			SetFont( TINYFONT1 );

			SetFontBackground( FONT_MCOLOR_BLACK );
			SetFontForeground( FONT_MCOLOR_WHITE );
			SetFontShadow( DEFAULT_SHADOW );				

			if ( !( gViewportRegion.uiFlags & MSYS_MOUSE_IN_AREA	) )
			{
				mprintf( sX + 10, sY - 10, L"x%d", gpItemPointer->ubNumberOfObjects );
			}

			// reset
			SetFontDestBuffer( FRAME_BUFFER, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, FALSE );
		}
	}
#endif
}

void UpdateAnimatedCursorFrames( UINT32 uiCursorIndex )
{
	CursorData		*pCurData;
	CursorImage		*pCurImage;
	UINT32				cnt;

	if ( uiCursorIndex != VIDEO_NO_CURSOR )
	{
		pCurData = &( CursorDatabase[ uiCursorIndex ] );

		for ( cnt = 0; cnt < pCurData->usNumComposites; cnt++ )
		{
			
			pCurImage = &( pCurData->Composites[ cnt ] );

			if ( CursorFileDatabase[ pCurImage->uiFileIndex ].ubFlags & ANIMATED_CURSOR )
			{
				pCurImage->uiCurrentFrame++;

				if ( pCurImage->uiCurrentFrame == CursorFileDatabase[ pCurImage->uiFileIndex ].ubNumberOfFrames )
				{
					pCurImage->uiCurrentFrame = 0;
				}
			
			}
		}
	}
}


void UpdateFlashingCursorFrames( UINT32 uiCursorIndex )
{
	CursorData		*pCurData;

	if ( uiCursorIndex != VIDEO_NO_CURSOR )
	{
		pCurData = &( CursorDatabase[ uiCursorIndex ] );

		if ( ( pCurData->bFlags & ( CURSOR_TO_FLASH | CURSOR_TO_FLASH2 ) ) )
		{
			pCurData->bFlashIndex = !pCurData->bFlashIndex;

		// Should we play a sound?
		if ( pCurData->bFlags & ( CURSOR_TO_PLAY_SOUND ) )
		{
		 if ( pCurData->bFlashIndex )
		 {
			PlayJA2Sample( TARGET_OUT_OF_RANGE, RATE_11025, MIDVOLUME, 1, MIDDLEPAN );						
		 }
		}
		}
	}

}


void SetCursorSpecialFrame( UINT32 uiCursor, UINT8 ubFrame )
{
	CursorDatabase[ uiCursor ].bFlashIndex = ubFrame;
}

void SetCursorFlags( UINT32 uiCursor, UINT8 ubFlags )
{
	CursorDatabase[ uiCursor ].bFlags |= ubFlags;
}


void RemoveCursorFlags( UINT32 uiCursor, UINT8 ubFlags )
{
	CursorDatabase[ uiCursor ].bFlags &= ( ~ubFlags );
}


HVOBJECT GetCursorFileVideoObject( UINT32 uiCursorFile )
{
	return( CursorFileDatabase[ uiCursorFile ].hVObject );
}


void SyncPairedCursorFrames( UINT32 uiSrcIndex, UINT32 uiDestIndex )
{
#if 0
	CursorData		*pSrcCurData, *pDestCurData;
	CursorImage		*pSrcCurImage, *pDestCurImage;
	UINT32				cnt;
	INT32				iCurFrame = -1;

	if ( uiSrcIndex == VIDEO_NO_CURSOR || uiDestIndex == VIDEO_NO_CURSOR )
	{
		return;
	}

	pSrcCurData = &( CursorDatabase[ uiSrcIndex ] );
	pDestCurData = &( CursorDatabase[ uiDestIndex ] );

	// Get Current frame from src
	for ( cnt = 0; cnt < pSrcCurData->usNumComposites; cnt++ )
	{
		pSrcCurImage = &( pSrcCurData->Composites[ cnt ] );

		if ( CursorFileDatabase[ pSrcCurImage->uiFileIndex ][0]->data.fFlags & ANIMATED_CURSOR )
		{
			iCurFrame = pSrcCurImage->uiCurrentFrame;
			break;
		}
	}

	// If we are not an animated cursor, return now
	if ( iCurFrame == -1 )
	{
		return;
	}

	// Update dest
	for ( cnt = 0; cnt < pDestCurData->usNumComposites; cnt++ )
	{
		pDestCurImage = &( pDestCurData->Composites[ cnt ] );

		if ( CursorFileDatabase[ pDestCurImage->uiFileIndex ][0]->data.fFlags & ANIMATED_CURSOR )
		{
			pDestCurImage->uiCurrentFrame = iCurFrame;
		}
	}
#endif
}