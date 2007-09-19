#include "build_defines.h"

#ifdef JA2EDITOR
#ifndef __CURSOR_MODES_H
#define __CURSOR_MODES_H

enum 
{
	SMALLSELECTION,		// 1x1
	MEDIUMSELECTION,	// 3x3
	LARGESELECTION,		// 5x5
	XLARGESELECTION,	// 7x7
	LINESELECTION,		// v or h line with a specified width
	AREASELECTION,		// user controlled area
	NUMSELECTIONTYPES
};

extern UINT16 gusSelectionType;
extern UINT16 gusSelectionWidth;
extern UINT16 gusPreserveSelectionWidth;
extern UINT16 gusSelectionDensity;
extern UINT16 gusSavedSelectionType;
extern UINT16 gusSavedBuildingSelectionType;

BOOLEAN PerformDensityTest();
void SetDrawingMode( INT32 iMode );
void UpdateCursorAreas();
void IncreaseSelectionDensity();
void DecreaseSelectionDensity();
void RemoveCursors();

extern CHAR16 *wszSelType[6];

extern BOOLEAN gfCurrentSelectionWithRightButton;

#endif

#endif
