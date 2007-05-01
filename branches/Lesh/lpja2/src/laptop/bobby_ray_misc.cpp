#ifdef PRECOMPILEDHEADERS
	#include "laptop_all.h"
#else
	#include "laptop.h"
	#include "bobby_ray_misc.h"
	#include "bobby_ray.h"
	#include "bobby_ray_guns.h"
	#include "utilities.h"
	#include "wcheck.h"
	#include "word_wrap.h"
	#include "cursors.h"
	#include "text.h"
#endif


UINT32		guiMiscBackground;
UINT32		guiMiscGrid;



void GameInitBobbyRMisc()
{

}

BOOLEAN EnterBobbyRMisc()
{
  VOBJECT_DESC    VObjectDesc;

	// load the background graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\miscbackground.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiMiscBackground));

	// load the gunsgrid graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\miscgrid.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiMiscGrid));

	InitBobbyBrTitle();

	guiPrevMiscFilterMode = -1;
	guiCurrentMiscFilterMode = -1;

	SetFirstLastPagesForNew( IC_BOBBY_MISC, guiCurrentMiscFilterMode );

	//Draw menu bar
	InitBobbyMenuBar( );
	// WANNE
	InitBobbyRMiscFilterBar();


	
//	CalculateFirstAndLastIndexs();

	RenderBobbyRMisc( );

	return(TRUE);
}

void ExitBobbyRMisc()
{
	DeleteVideoObjectFromIndex(guiMiscBackground);
	DeleteVideoObjectFromIndex(guiMiscGrid);
	DeleteBobbyBrTitle();

	DeleteBobbyMenuBar();

	// WANNE
	DeleteBobbyRMiscFilter();

	DeleteMouseRegionForBigImage();

	guiLastBobbyRayPage = LAPTOP_MODE_BOBBY_R_MISC;
}

void HandleBobbyRMisc()
{
}

void RenderBobbyRMisc()
{
  HVOBJECT hPixHandle;

	WebPageTileBackground(BOBBYR_NUM_HORIZONTAL_TILES, BOBBYR_NUM_VERTICAL_TILES, BOBBYR_BACKGROUND_WIDTH, BOBBYR_BACKGROUND_HEIGHT, guiMiscBackground);

	//Display title at top of page
	//DisplayBobbyRBrTitle();

	// GunForm
	GetVideoObject(&hPixHandle, guiMiscGrid);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0, BOBBYR_GRIDLOC_X, BOBBYR_GRIDLOC_Y, VO_BLT_SRCTRANSPARENCY,NULL);

	// WANNE
	DisplayItemInfo(IC_BOBBY_MISC, guiCurrentMiscFilterMode);
	UpdateButtonText(guiCurrentLaptopMode);
	UpdateMiscFilterButtons();

	MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
	InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
  	fReDrawScreenFlag = TRUE;
	fPausedReDrawScreenFlag = TRUE;	
}







 


