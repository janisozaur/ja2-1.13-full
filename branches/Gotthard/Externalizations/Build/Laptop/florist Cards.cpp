#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "laptop.h"
	#include "florist.h"
	#include "florist Cards.h"
	#include "WCheck.h"
	#include "Utilities.h"
	#include "WordWrap.h"
	#include "Cursors.h"
	#include "Encrypted File.h"
	#include "Florist Gallery.h"
	#include "Text.h"
#endif
//Just a note as to what some of the constants will be eventually defined as... Gotthard 11/4/07
//iScreenWidthOffset = ((SCREEN_WIDTH - 640) / 2) + 111 + ;
//iScreenHeightOffset = ((SCREEN_HEIGHT - 480) / 2) + 19;
//Commented out to help externalization to XML files.  Gotthard 10/31/07
/*
#define		FLORIST_CARDS_SENTENCE_FONT			FONT12ARIAL
#define		FLORIST_CARDS_SENTENCE_COLOR		FONT_MCOLOR_WHITE

#define		FLORIST_CARD_FIRST_POS_X				LAPTOP_SCREEN_UL_X + 7	
#define		FLORIST_CARD_FIRST_POS_Y				LAPTOP_SCREEN_WEB_UL_Y + 72	
#define		FLORIST_CARD_FIRST_OFFSET_X			174	
#define		FLORIST_CARD_FIRST_OFFSET_Y			109	

#define		FLORIST_CARD_CARD_WIDTH					135
#define		FLORIST_CARD_CARD_HEIGHT				100

#define		FLORIST_CARD_TEXT_WIDTH					121
#define		FLORIST_CARD_TEXT_HEIGHT				90

#define		FLORIST_CARD_TITLE_SENTENCE_X		LAPTOP_SCREEN_UL_X
#define		FLORIST_CARD_TITLE_SENTENCE_Y		LAPTOP_SCREEN_WEB_UL_Y + 53
#define		FLORIST_CARD_TITLE_SENTENCE_WIDTH			613 - 111

#define		FLORIST_CARD_BACK_BUTTON_X			LAPTOP_SCREEN_UL_X + 8
#define		FLORIST_CARD_BACK_BUTTON_Y			LAPTOP_SCREEN_WEB_UL_Y + 12

//#define		FLORIST_CARD_
//#define		FLORIST_CARD_
//#define		FLORIST_CARD_
*/
//New enum to reference the constants we just commented out above.  Gotthard 10/31/07
enum {
FLORIST_CARDS_SENTENCE_FONT,
FLORIST_CARDS_SENTENCE_COLOR,

FLORIST_CARD_FIRST_POS_X,
FLORIST_CARD_FIRST_POS_Y,
FLORIST_CARD_FIRST_OFFSET_X,
FLORIST_CARD_FIRST_OFFSET_Y,

FLORIST_CARD_CARD_WIDTH,
FLORIST_CARD_CARD_HEIGHT,

FLORIST_CARD_TEXT_WIDTH,
FLORIST_CARD_TEXT_HEIGHT,

FLORIST_CARD_TITLE_SENTENCE_X,
FLORIST_CARD_TITLE_SENTENCE_Y,
FLORIST_CARD_TITLE_SENTENCE_WIDTH,

FLORIST_CARD_BACK_BUTTON_X,
FLORIST_CARD_BACK_BUTTON_Y,
};


//This array holds all the original constants, and they are referenced using the enumeration defined above.  Gotthard 10/31/07
extern INT16 iFloristCardsConstants[15] = {0};/*
{
FONT12ARIAL,
FONT_MCOLOR_WHITE,

((SCREEN_WIDTH - 640) / 2) + 111 + 7,	
((SCREEN_HEIGHT - 480) / 2) + 27 + 19 + 72,
174,
109,

135,
100,

121,
90,

((SCREEN_WIDTH - 640) / 2) + 111,
((SCREEN_HEIGHT - 480) / 2) + 27 + 19 + 53,
613 - 111,

((SCREEN_WIDTH - 640) / 2) + 111 + 8,
((SCREEN_HEIGHT - 480) / 2) + 27 + 19 + 12,
};*/

UINT32		guiCardBackground;

INT8			gbCurrentlySelectedCard;

//link to the card gallery
MOUSE_REGION    gSelectedFloristCardsRegion[9];
void SelectFloristCardsRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );


INT32		guiFlowerCardsButtonImage;
void		BtnFlowerCardsBackButtonCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiFlowerCardsBackButton;



void GameInitFloristCards()
{

}

BOOLEAN EnterFloristCards()
{
	UINT16 i, j, usPosX, usPosY;
  VOBJECT_DESC    VObjectDesc;
	UINT8						ubCount;

	InitFloristDefaults();

	// load the Flower Account Box graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\CardBlank.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiCardBackground));

	ubCount = 0;
	usPosY = iFloristCardsConstants[FLORIST_CARD_FIRST_POS_Y];
	for(j=0; j<3; j++)
	{
		usPosX = iFloristCardsConstants[FLORIST_CARD_FIRST_POS_X];
		for(i=0; i<3; i++)
		{
			MSYS_DefineRegion( &gSelectedFloristCardsRegion[ubCount], usPosX, usPosY, (UINT16)(usPosX + iFloristCardsConstants[FLORIST_CARD_CARD_WIDTH]), (UINT16)(usPosY + iFloristCardsConstants[FLORIST_CARD_CARD_HEIGHT]), MSYS_PRIORITY_HIGH,
							 CURSOR_WWW, MSYS_NO_CALLBACK, SelectFloristCardsRegionCallBack ); 
			MSYS_AddRegion(&gSelectedFloristCardsRegion[ubCount]); 
			MSYS_SetRegionUserData( &gSelectedFloristCardsRegion[ubCount], 0, ubCount );	
			ubCount++;
			usPosX += iFloristCardsConstants[FLORIST_CARD_FIRST_OFFSET_X];
		}
		usPosY += iFloristCardsConstants[FLORIST_CARD_FIRST_OFFSET_Y];
	}


	guiFlowerCardsButtonImage	= LoadButtonImage("LAPTOP\\FloristButtons.sti", -1,0,-1,1,-1 );

	guiFlowerCardsBackButton = CreateIconAndTextButton( guiFlowerCardsButtonImage, sFloristCards[FLORIST_CARDS_BACK], iFloristConstants[FLORIST_BUTTON_TEXT_FONT], 
													 iFloristConstants[FLORIST_BUTTON_TEXT_UP_COLOR], iFloristConstants[FLORIST_BUTTON_TEXT_SHADOW_COLOR], 
													 iFloristConstants[FLORIST_BUTTON_TEXT_DOWN_COLOR], iFloristConstants[FLORIST_BUTTON_TEXT_SHADOW_COLOR], 
													 TEXT_CJUSTIFIED, 
													 iFloristCardsConstants[FLORIST_CARD_BACK_BUTTON_X], iFloristCardsConstants[FLORIST_CARD_BACK_BUTTON_Y], BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													 DEFAULT_MOVE_CALLBACK, BtnFlowerCardsBackButtonCallback);
	SetButtonCursor(guiFlowerCardsBackButton, CURSOR_WWW );


	//passing the currently selected card to -1, so it is not used
	gbCurrentlySelectedCard = -1;

	RenderFloristCards();
	return(TRUE);
}

void ExitFloristCards()
{
	UINT8	i;

	RemoveFloristDefaults();
	DeleteVideoObjectFromIndex( guiCardBackground );

	//card gallery 
	for(i=0; i<9; i++)
		MSYS_RemoveRegion( &gSelectedFloristCardsRegion[i]);

	UnloadButtonImage( guiFlowerCardsButtonImage );
	RemoveButton( guiFlowerCardsBackButton );
}

void HandleFloristCards()
{

}

void RenderFloristCards()
{
	UINT8	i,j, ubCount;
	UINT16	usPosX, usPosY;
	CHAR16		sTemp[ 640 ];
	UINT32	uiStartLoc=0;
  HVOBJECT hPixHandle;
	UINT16		usHeightOffset;

	DisplayFloristDefaults();

	DrawTextToScreen( sFloristCards[FLORIST_CARDS_CLICK_SELECTION], iFloristCardsConstants[FLORIST_CARD_TITLE_SENTENCE_X], iFloristCardsConstants[FLORIST_CARD_TITLE_SENTENCE_Y], iFloristCardsConstants[FLORIST_CARD_TITLE_SENTENCE_WIDTH], FONT10ARIAL, (UINT8)iFloristCardsConstants[FLORIST_CARDS_SENTENCE_COLOR], FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED );

	GetVideoObject(&hPixHandle, guiCardBackground);
	usPosY = iFloristCardsConstants[FLORIST_CARD_FIRST_POS_Y];
	ubCount = 0;
	for(j=0; j<3; j++)
	{
		usPosX = iFloristCardsConstants[FLORIST_CARD_FIRST_POS_X];
		for(i=0; i<3; i++)
		{
			//The flowe account box
			BltVideoObject(FRAME_BUFFER, hPixHandle, 0, usPosX, usPosY, VO_BLT_SRCTRANSPARENCY,NULL);

			//Get and display the card saying
			uiStartLoc = FLOR_CARD_TEXT_TITLE_SIZE * ubCount;
			LoadEncryptedDataFromFile(FLOR_CARD_TEXT_FILE, sTemp, uiStartLoc, FLOR_CARD_TEXT_TITLE_SIZE);

//			DisplayWrappedString((UINT16)(usPosX+7), (UINT16)(usPosY+15), FLORIST_CARD_TEXT_WIDTH, 2, FLORIST_CARDS_SENTENCE_FONT, FLORIST_CARDS_SENTENCE_COLOR,  sTemp, FONT_MCOLOR_BLACK, FALSE, CENTER_JUSTIFIED);
				usHeightOffset = IanWrappedStringHeight( (UINT16)(usPosX+7), (UINT16)(usPosY), iFloristCardsConstants[FLORIST_CARD_TEXT_WIDTH], 2, 
															 iFloristCardsConstants[FLORIST_CARDS_SENTENCE_FONT], (UINT8)iFloristCardsConstants[FLORIST_CARDS_SENTENCE_COLOR], sTemp, 
															 0, FALSE, 0);

				usHeightOffset = ( iFloristCardsConstants[FLORIST_CARD_TEXT_HEIGHT] - usHeightOffset ) / 2;


				IanDisplayWrappedString( (UINT16)(usPosX+7), (UINT16)(usPosY+10+usHeightOffset), iFloristCardsConstants[FLORIST_CARD_TEXT_WIDTH], 2, 
															 iFloristCardsConstants[FLORIST_CARDS_SENTENCE_FONT], (UINT8)iFloristCardsConstants[FLORIST_CARDS_SENTENCE_COLOR], sTemp, 
															 0, FALSE, 0);

			ubCount++;
			usPosX += iFloristCardsConstants[FLORIST_CARD_FIRST_OFFSET_X];
		}
		usPosY += iFloristCardsConstants[FLORIST_CARD_FIRST_OFFSET_Y];
	}

  MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
  InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}



void SelectFloristCardsRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{ 
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		gbCurrentlySelectedCard = (UINT8) MSYS_GetRegionUserData( pRegion, 0 );

		guiCurrentLaptopMode = LAPTOP_MODE_FLORIST_ORDERFORM;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	} 
}



void BtnFlowerCardsBackButtonCallback(GUI_BUTTON *btn,INT32 reason)
{
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_DWN )
	{
		btn->uiFlags |= BUTTON_CLICKED_ON;
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
	if(reason & MSYS_CALLBACK_REASON_LBUTTON_UP )
	{
		if (btn->uiFlags & BUTTON_CLICKED_ON)
		{
			btn->uiFlags &= (~BUTTON_CLICKED_ON );

			guiCurrentLaptopMode = LAPTOP_MODE_FLORIST_ORDERFORM;

			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
} 
