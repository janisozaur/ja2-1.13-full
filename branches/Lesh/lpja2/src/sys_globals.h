#ifndef __SYS_GLOBALS_H
#define __SYS_GLOBALS_H

#define			SHOW_MIN_FPS			0
#define			SHOW_FULL_FPS			1

extern UINT8	gubErrorText[ 200 ];
extern BOOLEAN	gfAniEditMode;
extern BOOLEAN	gfEditMode;
extern BOOLEAN	fFirstTimeInGameScreen;
extern BOOLEAN	fDirtyRectangleMode;
extern INT8		gDebugStr[128];
extern INT8		gSystemDebugStr[128];

extern BOOLEAN	gfMode;
extern INT16	gsCurrentActionPoints;
extern INT8		gbFPSDisplay;
extern BOOLEAN	gfResetInputCheck;
extern BOOLEAN	gfGlobalError;

extern UINT32	guiGameCycleCounter;


// VIDEO OVERLAYS 
extern INT32	giFPSOverlay;
extern INT32	giCounterPeriodOverlay;

extern	BOOLEAN  SET_ERROR( const char *String, ...);

#endif
