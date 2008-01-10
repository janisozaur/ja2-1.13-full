#ifdef PRECOMPILEDHEADERS
	#include "Laptop All.h"
#else
	#include "laptop.h"
	#include "florist.h"
	#include "florist Order Form.h"
	#include "WCheck.h"
	#include "Utilities.h"
	#include "WordWrap.h"
	#include "Cursors.h"
	#include "florist Gallery.h"
	#include "stdio.h"
	#include "Encrypted File.h"
	#include "florist Cards.h"
	#include "Text Input.h"
	#include "Finances.h"
	#include "Game clock.h"
	#include "english.h"
	#include "Text.h"
	#include "LaptopSave.h"
	#include "Random.h"
#endif

#include "meanwhile.h"
//Commented out as they have been moved into the array iFloristOrderFormConstants[].  
//Defines names are used to reference the arrays, and are declared in an enum below.
//This was to facilitate an eventual migration over into XML files.  Gotthard 11/3/07
/*
#define		FLOWER_ORDEER_TINY_FONT					FONT10ARIAL
#define		FLOWER_ORDEER_SMALL_FONT				FONT12ARIAL
#define		FLOWER_ORDEER_BIG_FONT					FONT12ARIAL
#define		FLOWER_ORDEER_SMALL_COLOR				FONT_MCOLOR_WHITE
#define		FLOWER_ORDEER_LINK_COLOR				FONT_MCOLOR_LTYELLOW
#define		FLOWER_ORDEER_DROP_DOWN_FONT		FONT12ARIAL
#define		FLOWER_ORDEER_DROP_DOWN_COLOR		FONT_MCOLOR_WHITE

#define		FLOWER_ORDER_STATIC_TEXT_COLOR	76

#define		FLOWER_ORDER_FLOWER_BOX_X					LAPTOP_SCREEN_UL_X + 7
#define		FLOWER_ORDER_FLOWER_BOX_Y					LAPTOP_SCREEN_WEB_UL_Y + 63
#define		FLOWER_ORDER_FLOWER_BOX_WIDTH			75
#define		FLOWER_ORDER_FLOWER_BOX_HEIGHT		100


#define		FLOWER_ORDER_SENTIMENT_BOX_X			LAPTOP_SCREEN_UL_X + 14
#define		FLOWER_ORDER_SENTIMENT_BOX_Y			LAPTOP_SCREEN_WEB_UL_Y + 226
#define		FLOWER_ORDER_SENTIMENT_BOX_WIDTH	468			
#define		FLOWER_ORDER_SENTIMENT_BOX_HEIGHT	27	

#define		FLOWER_ORDER_NAME_BOX_X						LAPTOP_SCREEN_UL_X + 60
#define		FLOWER_ORDER_NAME_BOX_Y						LAPTOP_SCREEN_WEB_UL_Y + 314 - FLOWER_ORDER_SMALLER_PS_OFFSET_Y

#define		FLOWER_ORDER_SMALLER_PS_OFFSET_Y		27


#define		FLOWER_ORDER_DELIVERY_LOCATION_X	LAPTOP_SCREEN_UL_X + 205	
#define		FLOWER_ORDER_DELIVERY_LOCATION_Y	LAPTOP_SCREEN_WEB_UL_Y + 143	
#define		FLOWER_ORDER_DELIVERY_LOCATION_WIDTH	252
#define		FLOWER_ORDER_DELIVERY_LOCATION_HEIGHT	20

#define		FLOWER_ORDER_BACK_BUTTON_X				LAPTOP_SCREEN_UL_X + 8
#define		FLOWER_ORDER_BACK_BUTTON_Y				LAPTOP_SCREEN_WEB_UL_Y + 12

#define		FLOWER_ORDER_SEND_BUTTON_X				LAPTOP_SCREEN_UL_X + 124
#define		FLOWER_ORDER_SEND_BUTTON_Y				LAPTOP_SCREEN_WEB_UL_Y + 364

#define		FLOWER_ORDER_CLEAR_BUTTON_X				LAPTOP_SCREEN_UL_X + 215
#define		FLOWER_ORDER_CLEAR_BUTTON_Y				FLOWER_ORDER_SEND_BUTTON_Y

#define		FLOWER_ORDER_GALLERY_BUTTON_X			LAPTOP_SCREEN_UL_X + 305
#define		FLOWER_ORDER_GALLERY_BUTTON_Y			FLOWER_ORDER_SEND_BUTTON_Y

#define		FLOWER_ORDER_FLOWER_NAME_X				LAPTOP_SCREEN_UL_X + 94
#define		FLOWER_ORDER_FLOWER_NAME_Y				LAPTOP_SCREEN_WEB_UL_Y + 68

#define		FLOWER_ORDER_BOUQUET_NAME_X				FLOWER_ORDER_FLOWER_NAME_X
#define		FLOWER_ORDER_BOUQUET_NAME_Y				FLOWER_ORDER_ORDER_NUM_NAME_Y + 15//FLOWER_ORDER_FLOWER_NAME_Y + 15

#define		FLOWER_ORDER_ORDER_NUM_NAME_X			FLOWER_ORDER_BOUQUET_NAME_X
#define		FLOWER_ORDER_ORDER_NUM_NAME_Y			FLOWER_ORDER_FLOWER_NAME_Y + 15//FLOWER_ORDER_BOUQUET_NAME_Y + 15

#define		FLOWER_ORDER_DATE_X								FLOWER_ORDER_FLOWER_NAME_X
#define		FLOWER_ORDER_DATE_Y								LAPTOP_SCREEN_WEB_UL_Y + 126

#define		FLOWER_ORDER_LOCATION_X						FLOWER_ORDER_FLOWER_NAME_X
#define		FLOWER_ORDER_LOCATION_Y						FLOWER_ORDER_DELIVERY_LOCATION_Y + 4

#define		FLOWER_ORDER_ADDITIONAL_SERVICES_X	FLOWER_ORDER_FLOWER_BOX_X
#define		FLOWER_ORDER_ADDITIONAL_SERVICES_Y	LAPTOP_SCREEN_WEB_UL_Y + 167

#define		FLOWER_ORDER_PERSONAL_SENT_TEXT_X		FLOWER_ORDER_ADDITIONAL_SERVICES_X
#define		FLOWER_ORDER_PERSONAL_SENT_TEXT_Y		LAPTOP_SCREEN_WEB_UL_Y + 212

#define		FLOWER_ORDER_PERSONAL_SENT_BOX_X		FLOWER_ORDER_SENTIMENT_BOX_X + 5
#define		FLOWER_ORDER_PERSONAL_SENT_BOX_Y		FLOWER_ORDER_SENTIMENT_BOX_Y + 5
#define		FLOWER_ORDER_PERSONAL_SENT_TEXT_WIDTH		457
#define		FLOWER_ORDER_PERSONAL_SENT_TEXT_HEIGHT	17//44

#define		FLOWER_ORDER_BILLING_INFO_X					FLOWER_ORDER_ADDITIONAL_SERVICES_X
#define		FLOWER_ORDER_BILLING_INFO_Y					LAPTOP_SCREEN_WEB_UL_Y + 296 - FLOWER_ORDER_SMALLER_PS_OFFSET_Y

#define		FLOWER_ORDER_NAME_TEXT_X						FLOWER_ORDER_ADDITIONAL_SERVICES_X
#define		FLOWER_ORDER_NAME_TEXT_Y						FLOWER_ORDER_NAME_BOX_Y+4
#define		FLOWER_ORDER_NAME_TEXT_WIDTH				50

#define		FLOWER_ORDER_NAME_TEXT_BOX_X							FLOWER_ORDER_NAME_BOX_X + 3
#define		FLOWER_ORDER_NAME_TEXT_BOX_Y							FLOWER_ORDER_NAME_BOX_Y + 3
#define		FLOWER_ORDER_NAME_TEXT_BOX_WIDTH					257
#define		FLOWER_ORDER_NAME_TEXT_BOX_HEIGHT				15

#define		FLOWER_ORDER_CHECK_WIDTH						20
#define		FLOWER_ORDER_CHECK_HEIGHT						17

#define		FLOWER_ORDER_CHECK_BOX_0_X					LAPTOP_SCREEN_UL_X + 186
#define		FLOWER_ORDER_CHECK_BOX_0_Y					FLOWER_ORDER_DATE_Y	- 3

#define		FLOWER_ORDER_CHECK_BOX_1_X					LAPTOP_SCREEN_UL_X + 270
#define		FLOWER_ORDER_CHECK_BOX_1_Y					FLOWER_ORDER_CHECK_BOX_0_Y	

#define		FLOWER_ORDER_CHECK_BOX_2_X					LAPTOP_SCREEN_UL_X + 123
#define		FLOWER_ORDER_CHECK_BOX_2_Y					FLOWER_ORDER_ADDITIONAL_SERVICES_Y

#define		FLOWER_ORDER_CHECK_BOX_3_X					LAPTOP_SCREEN_UL_X + 269
#define		FLOWER_ORDER_CHECK_BOX_3_Y					FLOWER_ORDER_CHECK_BOX_2_Y

#define		FLOWER_ORDER_CHECK_BOX_4_X					FLOWER_ORDER_CHECK_BOX_2_X
#define		FLOWER_ORDER_CHECK_BOX_4_Y					FLOWER_ORDER_CHECK_BOX_2_Y + 25

#define		FLOWER_ORDER_CHECK_BOX_5_X					FLOWER_ORDER_CHECK_BOX_3_X
#define		FLOWER_ORDER_CHECK_BOX_5_Y					FLOWER_ORDER_CHECK_BOX_4_Y

#define		FLOWER_ORDER_LINK_TO_CARD_GALLERY_X	LAPTOP_SCREEN_UL_X + 190
#define		FLOWER_ORDER_LINK_TO_CARD_GALLERY_Y	LAPTOP_SCREEN_WEB_UL_Y + 284 - FLOWER_ORDER_SMALLER_PS_OFFSET_Y

#define		FLOWER_ORDER_DROP_DOWN_LOCATION_X							FLOWER_ORDER_DELIVERY_LOCATION_X
#define		FLOWER_ORDER_DROP_DOWN_LOCATION_Y							FLOWER_ORDER_DELIVERY_LOCATION_Y + 19
#define		FLOWER_ORDER_DROP_DOWN_LOCATION_WIDTH					230

#define		FLOWER_ORDER_DROP_DOWN_CITY_START_X						FLOWER_ORDER_DROP_DOWN_LOCATION_X + 5
#define		FLOWER_ORDER_DROP_DOWN_CITY_START_Y						FLOWER_ORDER_DROP_DOWN_LOCATION_Y + 3

#define		FLOWER_ORDER_PERSONEL_SENTIMENT_NUM_CHARS			75
#define		FLOWER_ORDER_NAME_FIELD_NUM_CHARS							35

#define		FLOWER_ORDER_NUMBER_OF_DROP_DOWN_LOCATIONS		17
*/
//This is an enumeration to reference the array of all the constants in FloristOrderForm.  Gotthard 11/03/07
enum
{
FLOWER_ORDEER_TINY_FONT,
FLOWER_ORDEER_SMALL_FONT,
FLOWER_ORDEER_BIG_FONT,
FLOWER_ORDEER_SMALL_COLOR,
FLOWER_ORDEER_LINK_COLOR,
FLOWER_ORDEER_DROP_DOWN_FONT,
FLOWER_ORDEER_DROP_DOWN_COLOR,

FLOWER_ORDER_STATIC_TEXT_COLOR,

FLOWER_ORDER_FLOWER_BOX_X,
FLOWER_ORDER_FLOWER_BOX_Y,
FLOWER_ORDER_FLOWER_BOX_WIDTH,
FLOWER_ORDER_FLOWER_BOX_HEIGHT,

FLOWER_ORDER_SENTIMENT_BOX_X,
FLOWER_ORDER_SENTIMENT_BOX_Y,
FLOWER_ORDER_SENTIMENT_BOX_WIDTH,
FLOWER_ORDER_SENTIMENT_BOX_HEIGHT,

FLOWER_ORDER_NAME_BOX_X,
FLOWER_ORDER_NAME_BOX_Y,

FLOWER_ORDER_SMALLER_PS_OFFSET_Y,

FLOWER_ORDER_DELIVERY_LOCATION_X,
FLOWER_ORDER_DELIVERY_LOCATION_Y,
FLOWER_ORDER_DELIVERY_LOCATION_WIDTH,
FLOWER_ORDER_DELIVERY_LOCATION_HEIGHT,

FLOWER_ORDER_BACK_BUTTON_X,
FLOWER_ORDER_BACK_BUTTON_Y,

FLOWER_ORDER_SEND_BUTTON_X,
FLOWER_ORDER_SEND_BUTTON_Y,

FLOWER_ORDER_CLEAR_BUTTON_X,
FLOWER_ORDER_CLEAR_BUTTON_Y,

FLOWER_ORDER_GALLERY_BUTTON_X,
FLOWER_ORDER_GALLERY_BUTTON_Y,

FLOWER_ORDER_FLOWER_NAME_X,
FLOWER_ORDER_FLOWER_NAME_Y,

FLOWER_ORDER_BOUQUET_NAME_X,
FLOWER_ORDER_BOUQUET_NAME_Y,

FLOWER_ORDER_ORDER_NUM_NAME_X,
FLOWER_ORDER_ORDER_NUM_NAME_Y,

FLOWER_ORDER_DATE_X,
FLOWER_ORDER_DATE_Y,

FLOWER_ORDER_LOCATION_X,
FLOWER_ORDER_LOCATION_Y,

FLOWER_ORDER_ADDITIONAL_SERVICES_X,
FLOWER_ORDER_ADDITIONAL_SERVICES_Y,

FLOWER_ORDER_PERSONAL_SENT_TEXT_X,
FLOWER_ORDER_PERSONAL_SENT_TEXT_Y,

FLOWER_ORDER_PERSONAL_SENT_BOX_X,
FLOWER_ORDER_PERSONAL_SENT_BOX_Y,
FLOWER_ORDER_PERSONAL_SENT_TEXT_WIDTH,
FLOWER_ORDER_PERSONAL_SENT_TEXT_HEIGHT,

FLOWER_ORDER_BILLING_INFO_X,
FLOWER_ORDER_BILLING_INFO_Y,

FLOWER_ORDER_NAME_TEXT_X,
FLOWER_ORDER_NAME_TEXT_Y,
FLOWER_ORDER_NAME_TEXT_WIDTH,

FLOWER_ORDER_NAME_TEXT_BOX_X,
FLOWER_ORDER_NAME_TEXT_BOX_Y,
FLOWER_ORDER_NAME_TEXT_BOX_WIDTH,
FLOWER_ORDER_NAME_TEXT_BOX_HEIGHT,

FLOWER_ORDER_CHECK_WIDTH,
FLOWER_ORDER_CHECK_HEIGHT,

FLOWER_ORDER_CHECK_BOX_0_X,
FLOWER_ORDER_CHECK_BOX_0_Y,

FLOWER_ORDER_CHECK_BOX_1_X,
FLOWER_ORDER_CHECK_BOX_1_Y,

FLOWER_ORDER_CHECK_BOX_2_X,
FLOWER_ORDER_CHECK_BOX_2_Y,

FLOWER_ORDER_CHECK_BOX_3_X,
FLOWER_ORDER_CHECK_BOX_3_Y,

FLOWER_ORDER_CHECK_BOX_4_X,
FLOWER_ORDER_CHECK_BOX_4_Y,

FLOWER_ORDER_CHECK_BOX_5_X,
FLOWER_ORDER_CHECK_BOX_5_Y,

FLOWER_ORDER_LINK_TO_CARD_GALLERY_X,
FLOWER_ORDER_LINK_TO_CARD_GALLERY_Y,

FLOWER_ORDER_DROP_DOWN_LOCATION_X,
FLOWER_ORDER_DROP_DOWN_LOCATION_Y,
FLOWER_ORDER_DROP_DOWN_LOCATION_WIDTH,

FLOWER_ORDER_DROP_DOWN_CITY_START_X,
FLOWER_ORDER_DROP_DOWN_CITY_START_Y,

FLOWER_ORDER_PERSONEL_SENTIMENT_NUM_CHARS,
FLOWER_ORDER_NAME_FIELD_NUM_CHARS,

FLOWER_ORDER_NUMBER_OF_DROP_DOWN_LOCATIONS,
};


//This is an array to hold the original #defines now commented out above.  It is referenced using the new enumeration with the same comstant names.  Gotthard 11/3/07

UINT16 iFloristOrderFormConstants[] =
{
FONT10ARIAL,
FONT12ARIAL,
FONT12ARIAL,
FONT_MCOLOR_WHITE,
FONT_MCOLOR_LTYELLOW,
FONT12ARIAL,
FONT_MCOLOR_WHITE,

76,

LAPTOP_SCREEN_UL_X + 7,
LAPTOP_SCREEN_WEB_UL_Y + 63,
75,
100,


LAPTOP_SCREEN_UL_X + 14,
LAPTOP_SCREEN_WEB_UL_Y + 226,
468,		
27,

LAPTOP_SCREEN_UL_X + 60,
LAPTOP_SCREEN_WEB_UL_Y + 314 - (27),//Previously FLOWER_ORDER_SMALLER_PS_OFFSET_Y

27,

LAPTOP_SCREEN_UL_X + 205,
LAPTOP_SCREEN_WEB_UL_Y + 143,
252,
20,

LAPTOP_SCREEN_UL_X + 8,
LAPTOP_SCREEN_WEB_UL_Y + 12,

LAPTOP_SCREEN_UL_X + 124,
LAPTOP_SCREEN_WEB_UL_Y + 364,

LAPTOP_SCREEN_UL_X + 215,
(LAPTOP_SCREEN_WEB_UL_Y + 364),//Previously FLOWER_ORDER_SEND_BUTTON_Y

LAPTOP_SCREEN_UL_X + 305,
(LAPTOP_SCREEN_WEB_UL_Y + 364),//Previously FLOWER_ORDER_SEND_BUTTON_Y

LAPTOP_SCREEN_UL_X + 94,
LAPTOP_SCREEN_WEB_UL_Y + 68,

(LAPTOP_SCREEN_UL_X + 94),//Previously FLOWER_ORDER_FLOWER_NAME_X
(LAPTOP_SCREEN_WEB_UL_Y + 68) + 15,//Previously FLOWER_ORDER_FLOWER_NAME_Y

(LAPTOP_SCREEN_UL_X + 94),//Previously FLOWER_ORDER_BOUQUET_NAME_X
(LAPTOP_SCREEN_WEB_UL_Y + 68) + 15,//Previously FLOWER_ORDER_FLOWER_NAME_Y

(LAPTOP_SCREEN_UL_X + 94),//Previously FLOWER_ORDER_FLOWER_NAME_X
LAPTOP_SCREEN_WEB_UL_Y + 126,

(LAPTOP_SCREEN_UL_X + 94),//Previously FLOWER_ORDER_FLOWER_NAME_X
(LAPTOP_SCREEN_WEB_UL_Y + 143) + 4,//Previously FLOWER_ORDER_DELIVERY_LOcATION_Y

(LAPTOP_SCREEN_UL_X + 7),//Previously FLOWER_ORDER_FLOWER_BOX_X
LAPTOP_SCREEN_WEB_UL_Y + 167,

(LAPTOP_SCREEN_UL_X + 7),//Previously FLOWER_ORDER_ADDITIONAL_SERVICES_X
LAPTOP_SCREEN_WEB_UL_Y + 212,

(LAPTOP_SCREEN_UL_X + 14) + 5,//Previously FLOWER_ORDER_SENTIMENT_BOX_X
(LAPTOP_SCREEN_WEB_UL_Y + 226) + 5,//Previously FLOWER_ORDER_SENTIMENT_BOX_Y
457,
17,//44

(LAPTOP_SCREEN_UL_X + 7),//Previously FLOWER_ORDER_ADDITIONAL_SERVICES_X
LAPTOP_SCREEN_WEB_UL_Y + 296 - (27),//Previously FLOWER_ORDER_SMALLER_PS_OFFSET_Y

(LAPTOP_SCREEN_UL_X + 7),//Previously FLOWER_ORDER_ADDITIONAL_SERVICES_X
(LAPTOP_SCREEN_WEB_UL_Y + 314 - (27))+4,//Previously FLOWER_ORDER_NAME_BOX_Y
50,

(LAPTOP_SCREEN_UL_X + 60) + 3,//Previously FLOWER_ORDER_NAME_BOX_X
(LAPTOP_SCREEN_WEB_UL_Y + 314 - (27)) + 3,//Previously FLOWER_ORDER_NAME_BOX_Y
257,
15,

20,
17,

LAPTOP_SCREEN_UL_X + 186,
(LAPTOP_SCREEN_WEB_UL_Y + 126)	- 3,//Previously FLOWER_ORDER_DATE_Y

LAPTOP_SCREEN_UL_X + 270,
((LAPTOP_SCREEN_WEB_UL_Y + 126)	- 3),//Previously FLOWER_ORDER_CHECK_BOX_0_Y

LAPTOP_SCREEN_UL_X + 123,
(LAPTOP_SCREEN_WEB_UL_Y + 167),//Previously FLOWER_ORDER_ADDITIONAL_SERVICES_Y

LAPTOP_SCREEN_UL_X + 269,
((LAPTOP_SCREEN_WEB_UL_Y + 167)),//Previously FLOWER_ORDER_CHECK_BOX_2_Y

(LAPTOP_SCREEN_UL_X + 123),//Previously FLOWER_ORDER_CHECK_BOX_2_X
((LAPTOP_SCREEN_WEB_UL_Y + 167)) + 25,//Previously FLOWER_ORDER_CHECK_BOX_2_Y

((LAPTOP_SCREEN_UL_X + 123)),//Previously FLOWER_ORDER_CHECK_BOX_3_X
(((LAPTOP_SCREEN_WEB_UL_Y + 167)) + 25),//Previously FLOWER_ORDER_CHECK_BOX_4_Y

LAPTOP_SCREEN_UL_X + 190,
LAPTOP_SCREEN_WEB_UL_Y + 284 - (27),//Previously FLOWER_ORDER_SMALLER_PS_OFFSET_Y

(LAPTOP_SCREEN_UL_X + 205),//Previously FLOWER_ORDER_DELIVERY_LOCATION_X
(LAPTOP_SCREEN_WEB_UL_Y + 143) + 19,//Previously FLOWER_ORDER_DELIVERY_LOCATION_Y
230,

(LAPTOP_SCREEN_UL_X + 205) + 5,//Previously FLOWER_ORDER_DROP_DOWN_LOCATION_X
(LAPTOP_SCREEN_WEB_UL_Y + 143) + 3,//Previously FLOWER_ORDER_DROP_DOWN_LOCATION_Y

75,
35,

17,
};

typedef struct
{
	STR16 *psCityLoc;
	UINT8	ubNextDayDeliveryCost;
	UINT8	ubWhenItGetsThereCost;
} FlowerOrderLocationStruct;



FlowerOrderLocationStruct FlowerOrderLocations[FLOWER_ORDER_NUMBER_OF_DROP_DOWN_LOCATIONS]={
	{	&pDeliveryLocationStrings[0],	20, 15},
	{	&pDeliveryLocationStrings[1],	95, 70},
	{	&pDeliveryLocationStrings[2],	100, 75},
	{	&pDeliveryLocationStrings[3],	50, 35},
	{	&pDeliveryLocationStrings[4],	70, 50},
	{	&pDeliveryLocationStrings[5],	45, 35},
	{	&pDeliveryLocationStrings[6],	30, 25},
	{	&pDeliveryLocationStrings[7],	100, 75},
	{	&pDeliveryLocationStrings[8],	100, 75},
	{	&pDeliveryLocationStrings[9],	30, 25},
	{	&pDeliveryLocationStrings[10],	95, 70},
	{	&pDeliveryLocationStrings[11],	30, 25},
	{	&pDeliveryLocationStrings[12],	40, 30},
	{	&pDeliveryLocationStrings[13],	45, 35},
	{	&pDeliveryLocationStrings[14],	95, 70},
	{	&pDeliveryLocationStrings[15],	50, 40},
	{	&pDeliveryLocationStrings[16],	40, 30}
};

UINT32		guiDeliveryLocation;
UINT32		guiFlowerFrame;
UINT32		guiCurrentlySelectedFlowerImage;
UINT32		guiNameBox;
UINT32		guiPersonalSentiments;
UINT32		guiFlowerOrderCheckBoxButtonImage;
extern UINT32		guiDropDownBorder; // symbol already defined in BobbyRMailOrder.cpp (jonathanl)


BOOLEAN		gfFLoristCheckBox0Down = FALSE;			// next day delviery
BOOLEAN		gfFLoristCheckBox1Down = TRUE;			// when it gets there delivery
BOOLEAN		gfFLoristCheckBox2Down = FALSE;
BOOLEAN		gfFLoristCheckBox3Down = FALSE;
BOOLEAN		gfFLoristCheckBox4Down = FALSE;
BOOLEAN		gfFLoristCheckBox5Down = FALSE;

UINT32		guiFlowerPrice;

//drop down menu
enum
{
	FLOWER_ORDER_DROP_DOWN_NO_ACTION,
	FLOWER_ORDER_DROP_DOWN_CREATE,
	FLOWER_ORDER_DROP_DOWN_DESTROY,
	FLOWER_ORDER_DROP_DOWN_DISPLAY,
};
//the current mode of the drop down display
UINT8	gubFlowerDestDropDownMode;
UINT8	gubCurrentlySelectedFlowerLocation;

/*
CHAR16		gsSentimentTextField[ iFloristOrderFormConstants[FLOWER_ORDER_PERSONEL_SENTIMENT_NUM_CHARS] ] = {0} ; 
CHAR16		gsNameTextField[ iFloristOrderFormConstants[FLOWER_ORDER_NAME_FIELD_NUM_CHARS] ] = {0};
*///Commented out until I can figure out how to get a non-const variable to declare the array size.  Gotthard 11/3/07
//These will hopefully be replaced with the lines above when I work everything out.  Gotthard 11/3/07
CHAR16		gsSentimentTextField[ 79 ] = {0} ; 
CHAR16		gsNameTextField[ 80 ] = {0};

//buttons
INT32		guiFlowerOrderButtonImage;

UINT8		gubFlowerOrder_AdditioanalServicePrices[] = {10, 20, 10, 10};

void		BtnFlowerOrderBackButtonCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiFlowerOrderBackButton;

void		BtnFlowerOrderSendButtonCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiFlowerOrderSendButton;

void		BtnFlowerOrderClearButtonCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiFlowerOrderClearButton;

void		BtnFlowerOrderGalleryButtonCallback(GUI_BUTTON *btn,INT32 reason);
UINT32	guiFlowerOrderGalleryButton;


//Clicking on OrderCheckBox
MOUSE_REGION    gSelectedFloristCheckBoxRegion[ 6 ];
void SelectFlorsitCheckBoxRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );

//link to the card gallery
MOUSE_REGION    gSelectedFloristCardGalleryLinkRegion;
void SelectFloristCardGalleryLinkRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );

//link to the flower gallery by clicking on the flower
MOUSE_REGION    gSelectedFloristGalleryLinkRegion;
void SelectFloristGalleryLinkRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );


//the drop down for the city
MOUSE_REGION    gSelectedFloristDropDownRegion;
void SelectFloristDropDownRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );

//to disable the drop down window
MOUSE_REGION    gSelectedFloristDisableDropDownRegion;
void SelectFloristDisableDropDownRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );


//mouse region for the drop down city location area
MOUSE_REGION    gSelectedFlowerDropDownRegion[ FLOWER_ORDER_NUMBER_OF_DROP_DOWN_LOCATIONS ];
void SelectFlowerDropDownRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );
void SelectFlowerDropDownMovementCallBack(MOUSE_REGION * pRegion, INT32 iReason );


//to select typing in the personal sentiment box
//MOUSE_REGION    gSelectedFloristPersonalSentimentBoxRegion;
//void SelectFloristPersonalSentimentBoxRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason );


void DisplayFloristCheckBox();
void DisplayFlowerDynamicItems();
BOOLEAN CreateDestroyFlowerOrderDestDropDown( UINT8 ubDropDownMode );
void FlowerOrderDrawSelectedCity( UINT8 ubNumber );
void FlowerOrderDisplayShippingLocationCity();
void InitFlowerOrderTextInputBoxes();
void DestroyFlowerOrderTextInputBoxes();
void HandleFloristOrderKeyBoardInput();
void FlowerOrderUserTextFieldCallBack( UINT8 ubID, BOOLEAN fEntering);




void GameInitFloristOrderForm()
{

}

BOOLEAN EnterFloristOrderForm()
{
  VOBJECT_DESC    VObjectDesc;
	UINT8	i;
	char sTemp[40];
	UINT16 usPosX, usWidth, usHeight;

	InitFloristDefaults();


	// load the DeliveryLocation graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\DeliveryLocation.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiDeliveryLocation));

	// load the Flower frame graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\FlowerFrame.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiFlowerFrame));

	// load the Personel sentiments graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\PersonalSentiments.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiPersonalSentiments));

	// load the Name Box graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\NameBox.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiNameBox));

	// load the Check Box graphic and add it
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP("LAPTOP\\OrderCheckBox.sti", VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiFlowerOrderCheckBoxButtonImage));

	// load the currently selected flower bouquet
	sprintf( sTemp, "LAPTOP\\Flower_%d.sti", guiCurrentlySelectedFlower);
	VObjectDesc.fCreateFlags=VOBJECT_CREATE_FROMFILE;
	FilenameForBPP( sTemp, VObjectDesc.ImageFile);
	CHECKF(AddVideoObject(&VObjectDesc, &guiCurrentlySelectedFlowerImage));


  // border
  VObjectDesc.fCreateFlags = VOBJECT_CREATE_FROMFILE;
	FilenameForBPP( "INTERFACE\\TactPopUp.sti", VObjectDesc.ImageFile );
	CHECKF( AddVideoObject( &VObjectDesc, &guiDropDownBorder ) );


	guiFlowerOrderButtonImage	= LoadButtonImage("LAPTOP\\FloristButtons.sti", -1,0,-1,1,-1 );

	guiFlowerOrderBackButton = CreateIconAndTextButton( guiFlowerOrderButtonImage, sOrderFormText[FLORIST_ORDER_BACK], iFloristConstants[FLORIST_BUTTON_TEXT_FONT], 
													 iFloristConstants[FLORIST_BUTTON_TEXT_UP_COLOR], iFloristConstants[FLORIST_BUTTON_TEXT_SHADOW_COLOR], 
													 iFloristConstants[FLORIST_BUTTON_TEXT_DOWN_COLOR], iFloristConstants[FLORIST_BUTTON_TEXT_SHADOW_COLOR], 
													 TEXT_CJUSTIFIED, 
													 iFloristOrderFormConstants[FLOWER_ORDER_BACK_BUTTON_X], iFloristOrderFormConstants[FLOWER_ORDER_BACK_BUTTON_Y], BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													 DEFAULT_MOVE_CALLBACK, BtnFlowerOrderBackButtonCallback);
	SetButtonCursor(guiFlowerOrderBackButton, CURSOR_WWW );

	guiFlowerOrderSendButton = CreateIconAndTextButton( guiFlowerOrderButtonImage, sOrderFormText[FLORIST_ORDER_SEND], iFloristConstants[FLORIST_BUTTON_TEXT_FONT], 
													 iFloristConstants[FLORIST_BUTTON_TEXT_UP_COLOR], iFloristConstants[FLORIST_BUTTON_TEXT_SHADOW_COLOR], 
													 iFloristConstants[FLORIST_BUTTON_TEXT_DOWN_COLOR], iFloristConstants[FLORIST_BUTTON_TEXT_SHADOW_COLOR], 
													 TEXT_CJUSTIFIED, 
													 iFloristOrderFormConstants[FLOWER_ORDER_SEND_BUTTON_X], iFloristOrderFormConstants[FLOWER_ORDER_SEND_BUTTON_Y], BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													 DEFAULT_MOVE_CALLBACK, BtnFlowerOrderSendButtonCallback);
	SetButtonCursor(guiFlowerOrderSendButton, CURSOR_WWW );

	guiFlowerOrderClearButton = CreateIconAndTextButton( guiFlowerOrderButtonImage, sOrderFormText[FLORIST_ORDER_CLEAR], iFloristConstants[FLORIST_BUTTON_TEXT_FONT], 
													 iFloristConstants[FLORIST_BUTTON_TEXT_UP_COLOR], iFloristConstants[FLORIST_BUTTON_TEXT_SHADOW_COLOR], 
													 iFloristConstants[FLORIST_BUTTON_TEXT_DOWN_COLOR], iFloristConstants[FLORIST_BUTTON_TEXT_SHADOW_COLOR], 
													 TEXT_CJUSTIFIED, 
													 iFloristOrderFormConstants[FLOWER_ORDER_CLEAR_BUTTON_X], iFloristOrderFormConstants[FLOWER_ORDER_CLEAR_BUTTON_Y], BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													 DEFAULT_MOVE_CALLBACK, BtnFlowerOrderClearButtonCallback);
	SetButtonCursor(guiFlowerOrderClearButton, CURSOR_WWW );

	guiFlowerOrderGalleryButton = CreateIconAndTextButton( guiFlowerOrderButtonImage, sOrderFormText[FLORIST_ORDER_GALLERY], iFloristConstants[FLORIST_BUTTON_TEXT_FONT], 
													 iFloristConstants[FLORIST_BUTTON_TEXT_UP_COLOR], iFloristConstants[FLORIST_BUTTON_TEXT_SHADOW_COLOR], 
													 iFloristConstants[FLORIST_BUTTON_TEXT_DOWN_COLOR], iFloristConstants[FLORIST_BUTTON_TEXT_SHADOW_COLOR], 
													 TEXT_CJUSTIFIED, 
													 iFloristOrderFormConstants[FLOWER_ORDER_GALLERY_BUTTON_X], iFloristOrderFormConstants[FLOWER_ORDER_GALLERY_BUTTON_Y], BUTTON_TOGGLE, MSYS_PRIORITY_HIGH,
													 DEFAULT_MOVE_CALLBACK, BtnFlowerOrderGalleryButtonCallback);
	SetButtonCursor(guiFlowerOrderGalleryButton, CURSOR_WWW );


	//
	//	The check box mouse regions
	//
	i=0;
	MSYS_DefineRegion( &gSelectedFloristCheckBoxRegion[i], iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_0_X], iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_0_Y], (UINT16)(iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_0_X] + iFloristOrderFormConstants[FLOWER_ORDER_CHECK_WIDTH]), (UINT16)(iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_0_Y] + iFloristOrderFormConstants[FLOWER_ORDER_CHECK_HEIGHT]), MSYS_PRIORITY_HIGH,
						 CURSOR_WWW, MSYS_NO_CALLBACK, SelectFlorsitCheckBoxRegionCallBack ); 
	MSYS_AddRegion(&gSelectedFloristCheckBoxRegion[i]); 
	MSYS_SetRegionUserData( &gSelectedFloristCheckBoxRegion[i], 0, i );	
	i++;

	MSYS_DefineRegion( &gSelectedFloristCheckBoxRegion[i], iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_1_X], iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_1_Y], (UINT16)(iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_1_X] + iFloristOrderFormConstants[FLOWER_ORDER_CHECK_WIDTH]), (UINT16)(iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_1_Y] + iFloristOrderFormConstants[FLOWER_ORDER_CHECK_HEIGHT]), MSYS_PRIORITY_HIGH,
						 CURSOR_WWW, MSYS_NO_CALLBACK, SelectFlorsitCheckBoxRegionCallBack ); 
	MSYS_AddRegion(&gSelectedFloristCheckBoxRegion[i]); 
	MSYS_SetRegionUserData( &gSelectedFloristCheckBoxRegion[i], 0, i );	
	i++;

	MSYS_DefineRegion( &gSelectedFloristCheckBoxRegion[i], iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_2_X], iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_2_Y], (UINT16)(iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_2_X] + iFloristOrderFormConstants[FLOWER_ORDER_CHECK_WIDTH]), (UINT16)(iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_2_Y] + iFloristOrderFormConstants[FLOWER_ORDER_CHECK_HEIGHT]), MSYS_PRIORITY_HIGH,
						 CURSOR_WWW, MSYS_NO_CALLBACK, SelectFlorsitCheckBoxRegionCallBack ); 
	MSYS_AddRegion(&gSelectedFloristCheckBoxRegion[i]); 
	MSYS_SetRegionUserData( &gSelectedFloristCheckBoxRegion[i], 0, i );	
	i++;

	MSYS_DefineRegion( &gSelectedFloristCheckBoxRegion[i], iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_3_X], iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_3_Y], (UINT16)(iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_3_X] + iFloristOrderFormConstants[FLOWER_ORDER_CHECK_WIDTH]), (UINT16)(iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_3_Y] + iFloristOrderFormConstants[FLOWER_ORDER_CHECK_HEIGHT]), MSYS_PRIORITY_HIGH,
						 CURSOR_WWW, MSYS_NO_CALLBACK, SelectFlorsitCheckBoxRegionCallBack ); 
	MSYS_AddRegion(&gSelectedFloristCheckBoxRegion[i]); 
	MSYS_SetRegionUserData( &gSelectedFloristCheckBoxRegion[i], 0, i );	
	i++;

	MSYS_DefineRegion( &gSelectedFloristCheckBoxRegion[i], iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_4_X], iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_4_Y], (UINT16)(iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_4_X] + iFloristOrderFormConstants[FLOWER_ORDER_CHECK_WIDTH]), (UINT16)(iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_4_Y] + iFloristOrderFormConstants[FLOWER_ORDER_CHECK_HEIGHT]), MSYS_PRIORITY_HIGH,
						 CURSOR_WWW, MSYS_NO_CALLBACK, SelectFlorsitCheckBoxRegionCallBack ); 
	MSYS_AddRegion(&gSelectedFloristCheckBoxRegion[i]); 
	MSYS_SetRegionUserData( &gSelectedFloristCheckBoxRegion[i], 0, i );	
	i++;

	MSYS_DefineRegion( &gSelectedFloristCheckBoxRegion[i], iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_5_X], iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_5_Y], (UINT16)(iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_5_X] + iFloristOrderFormConstants[FLOWER_ORDER_CHECK_WIDTH]), (UINT16)(iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_5_Y] + iFloristOrderFormConstants[FLOWER_ORDER_CHECK_HEIGHT]), MSYS_PRIORITY_HIGH,
						 CURSOR_WWW, MSYS_NO_CALLBACK, SelectFlorsitCheckBoxRegionCallBack ); 
	MSYS_AddRegion(&gSelectedFloristCheckBoxRegion[i]); 
	MSYS_SetRegionUserData( &gSelectedFloristCheckBoxRegion[i], 0, i );	
	i++;

	usPosX = StringPixLength( sOrderFormText[FLORIST_ORDER_SELECT_FROM_OURS], iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_FONT]) + 2 + iFloristOrderFormConstants[FLOWER_ORDER_LINK_TO_CARD_GALLERY_X];
	usWidth = StringPixLength( sOrderFormText[FLORIST_ORDER_STANDARDIZED_CARDS], iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_FONT]);
	usHeight = GetFontHeight(iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_FONT]);
	MSYS_DefineRegion( &gSelectedFloristCardGalleryLinkRegion, usPosX, iFloristOrderFormConstants[FLOWER_ORDER_LINK_TO_CARD_GALLERY_Y], (UINT16)(usPosX + usWidth), (UINT16)(iFloristOrderFormConstants[FLOWER_ORDER_LINK_TO_CARD_GALLERY_Y] + usHeight), MSYS_PRIORITY_HIGH,
						 CURSOR_WWW, MSYS_NO_CALLBACK, SelectFloristCardGalleryLinkRegionCallBack ); 
	MSYS_AddRegion(&gSelectedFloristCardGalleryLinkRegion); 


	//flower link
	MSYS_DefineRegion( &gSelectedFloristGalleryLinkRegion, iFloristOrderFormConstants[FLOWER_ORDER_FLOWER_BOX_X], iFloristOrderFormConstants[FLOWER_ORDER_FLOWER_BOX_Y], (UINT16)(iFloristOrderFormConstants[FLOWER_ORDER_FLOWER_BOX_X] + iFloristOrderFormConstants[FLOWER_ORDER_FLOWER_BOX_WIDTH]), (UINT16)(iFloristOrderFormConstants[FLOWER_ORDER_FLOWER_BOX_Y] + iFloristOrderFormConstants[FLOWER_ORDER_FLOWER_BOX_HEIGHT]), MSYS_PRIORITY_HIGH,
					 CURSOR_WWW, MSYS_NO_CALLBACK, SelectFloristGalleryLinkRegionCallBack ); 
	MSYS_AddRegion(&gSelectedFloristGalleryLinkRegion); 

	//drop down city location
	MSYS_DefineRegion( &gSelectedFloristDropDownRegion, iFloristOrderFormConstants[FLOWER_ORDER_DELIVERY_LOCATION_X], iFloristOrderFormConstants[FLOWER_ORDER_DELIVERY_LOCATION_Y], (UINT16)(iFloristOrderFormConstants[FLOWER_ORDER_DELIVERY_LOCATION_X] + iFloristOrderFormConstants[FLOWER_ORDER_DELIVERY_LOCATION_WIDTH]), (UINT16)(iFloristOrderFormConstants[FLOWER_ORDER_DELIVERY_LOCATION_Y] + iFloristOrderFormConstants[FLOWER_ORDER_DELIVERY_LOCATION_HEIGHT]), MSYS_PRIORITY_HIGH,
					 CURSOR_WWW, MSYS_NO_CALLBACK, SelectFloristDropDownRegionCallBack); 
	MSYS_AddRegion(&gSelectedFloristDropDownRegion); 

		//to disable the drop down city location
	MSYS_DefineRegion( &gSelectedFloristDisableDropDownRegion, LAPTOP_SCREEN_UL_X, LAPTOP_SCREEN_WEB_UL_Y, LAPTOP_SCREEN_LR_X, LAPTOP_SCREEN_WEB_LR_Y, MSYS_PRIORITY_HIGH+2,
					 CURSOR_LAPTOP_SCREEN, MSYS_NO_CALLBACK, SelectFloristDisableDropDownRegionCallBack); 
	MSYS_AddRegion(&gSelectedFloristDisableDropDownRegion); 
  MSYS_DisableRegion(&gSelectedFloristDisableDropDownRegion); 


	//to select typing in the personal sentiment box
//	MSYS_DefineRegion( &gSelectedFloristPersonalSentimentBoxRegion, FLOWER_ORDER_SENTIMENT_BOX_X, FLOWER_ORDER_SENTIMENT_BOX_Y, (UINT16)(FLOWER_ORDER_SENTIMENT_BOX_X + FLOWER_ORDER_SENTIMENT_BOX_WIDTH), (UINT16)(FLOWER_ORDER_SENTIMENT_BOX_Y + FLOWER_ORDER_SENTIMENT_BOX_HEIGHT), MSYS_PRIORITY_HIGH,
//					 CURSOR_WWW, MSYS_NO_CALLBACK, SelectFloristPersonalSentimentBoxRegionCallBack); 
//	MSYS_AddRegion( &gSelectedFloristPersonalSentimentBoxRegion ); 

	InitFlowerOrderTextInputBoxes();

	LaptopSaveInfo.uiFlowerOrderNumber += Random( 5 ) + 1;
	
	RenderFloristOrderForm();

//	guiFlowerPrice = 0;
//	gubFlowerDestDropDownMode = FLOWER_ORDER_DROP_DOWN_NO_ACTION;
//	gubCurrentlySelectedFlowerLocation = 0;

	return(TRUE);
}

void InitFloristOrderFormVariables()
{
	guiFlowerPrice = 0;
	gubFlowerDestDropDownMode = (UINT8)iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_NO_ACTION];
	gubCurrentlySelectedFlowerLocation = 0;
}


void ExitFloristOrderForm()
{
	UINT8	i;
	RemoveFloristDefaults();

	DeleteVideoObjectFromIndex( guiDeliveryLocation );
	DeleteVideoObjectFromIndex( guiFlowerFrame );
	DeleteVideoObjectFromIndex( guiNameBox );
	DeleteVideoObjectFromIndex( guiPersonalSentiments );
	DeleteVideoObjectFromIndex( guiFlowerOrderCheckBoxButtonImage );
	DeleteVideoObjectFromIndex( guiCurrentlySelectedFlowerImage );
	DeleteVideoObjectFromIndex(guiDropDownBorder);

	for(i=0; i<6; i++)
		MSYS_RemoveRegion( &gSelectedFloristCheckBoxRegion[i]);

	//card gallery link
	MSYS_RemoveRegion( &gSelectedFloristCardGalleryLinkRegion);

	//flower link
	MSYS_RemoveRegion( &gSelectedFloristGalleryLinkRegion);

	//flower link
	MSYS_RemoveRegion( &gSelectedFloristDropDownRegion);

	//to select typing in the personal sentiment box
//	MSYS_RemoveRegion( &gSelectedFloristPersonalSentimentBoxRegion);

	//disable the drop down window
	MSYS_RemoveRegion( &gSelectedFloristDisableDropDownRegion);

	UnloadButtonImage( guiFlowerOrderButtonImage );

	RemoveButton( guiFlowerOrderBackButton );
	RemoveButton( guiFlowerOrderSendButton );
	RemoveButton( guiFlowerOrderClearButton );
	RemoveButton( guiFlowerOrderGalleryButton );

	//Store the text fields
	Get16BitStringFromField( 1, gsSentimentTextField );
	Get16BitStringFromField( 2, gsNameTextField );
	gbCurrentlySelectedCard = -1;


	DestroyFlowerOrderTextInputBoxes();
}

void HandleFloristOrderForm()
{
	if( gubFlowerDestDropDownMode != iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_NO_ACTION] )
	{
		CreateDestroyFlowerOrderDestDropDown( gubFlowerDestDropDownMode );
	}
	HandleFloristOrderKeyBoardInput();

	RenderAllTextFields();
}

void RenderFloristOrderForm()
{
  HVOBJECT hPixHandle;
	UINT16 usPosX;
	CHAR16		sTemp[ 640 ];
	UINT32	uiStartLoc=0;

	DisplayFloristDefaults();

	//The flowe Delivery location
	GetVideoObject(&hPixHandle, guiDeliveryLocation);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0,iFloristOrderFormConstants[FLOWER_ORDER_DELIVERY_LOCATION_X], iFloristOrderFormConstants[FLOWER_ORDER_DELIVERY_LOCATION_Y], VO_BLT_SRCTRANSPARENCY,NULL);

	//The flowe Flower Frame
	GetVideoObject(&hPixHandle, guiFlowerFrame);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0,iFloristOrderFormConstants[FLOWER_ORDER_FLOWER_BOX_X], iFloristOrderFormConstants[FLOWER_ORDER_FLOWER_BOX_Y], VO_BLT_SRCTRANSPARENCY,NULL);

	//The currenltly selected flwoer
	GetVideoObject(&hPixHandle, guiCurrentlySelectedFlowerImage);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0,iFloristOrderFormConstants[FLOWER_ORDER_FLOWER_BOX_X]+5, iFloristOrderFormConstants[FLOWER_ORDER_FLOWER_BOX_Y]+5, VO_BLT_SRCTRANSPARENCY,NULL);

	//The flowe Name Box
	GetVideoObject(&hPixHandle, guiNameBox);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0,iFloristOrderFormConstants[FLOWER_ORDER_NAME_BOX_X], iFloristOrderFormConstants[FLOWER_ORDER_NAME_BOX_Y], VO_BLT_SRCTRANSPARENCY,NULL);


	//The flower Personel sentiments
	GetVideoObject(&hPixHandle, guiPersonalSentiments);
	BltVideoObject(FRAME_BUFFER, hPixHandle, 0,iFloristOrderFormConstants[FLOWER_ORDER_SENTIMENT_BOX_X], iFloristOrderFormConstants[FLOWER_ORDER_SENTIMENT_BOX_Y], VO_BLT_SRCTRANSPARENCY,NULL);

	//Bouquet name, price and order number,text
	DrawTextToScreen(sOrderFormText[FLORIST_ORDER_NAME_BOUQUET], iFloristOrderFormConstants[FLOWER_ORDER_FLOWER_NAME_X], iFloristOrderFormConstants[FLOWER_ORDER_FLOWER_NAME_Y], 0, iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDER_STATIC_TEXT_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen(sOrderFormText[FLORIST_ORDER_PRICE], iFloristOrderFormConstants[FLOWER_ORDER_BOUQUET_NAME_X], iFloristOrderFormConstants[FLOWER_ORDER_BOUQUET_NAME_Y], 0, iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDER_STATIC_TEXT_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen(sOrderFormText[FLORIST_ORDER_ORDER_NUMBER], iFloristOrderFormConstants[FLOWER_ORDER_ORDER_NUM_NAME_X], iFloristOrderFormConstants[FLOWER_ORDER_ORDER_NUM_NAME_Y], 0, iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDER_STATIC_TEXT_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	//The flower name
	usPosX = StringPixLength( sOrderFormText[FLORIST_ORDER_NAME_BOUQUET], iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_FONT]) + 5 + iFloristOrderFormConstants[FLOWER_ORDER_FLOWER_NAME_X];
	uiStartLoc = FLOR_GALLERY_TEXT_TOTAL_SIZE * guiCurrentlySelectedFlower;
	LoadEncryptedDataFromFile(FLOR_GALLERY_TEXT_FILE, sTemp, uiStartLoc, FLOR_GALLERY_TEXT_TITLE_SIZE);
	DrawTextToScreen( sTemp, usPosX, iFloristOrderFormConstants[FLOWER_ORDER_FLOWER_NAME_Y], 0, iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );



	
	//Deliverry Date
	DrawTextToScreen(sOrderFormText[FLORIST_ORDER_DELIVERY_DATE], iFloristOrderFormConstants[FLOWER_ORDER_ORDER_NUM_NAME_X], iFloristOrderFormConstants[FLOWER_ORDER_DATE_Y], 0, iFloristOrderFormConstants[FLOWER_ORDEER_BIG_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDER_STATIC_TEXT_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	//Next day
	DrawTextToScreen(sOrderFormText[FLORIST_ORDER_NEXT_DAY], iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_0_X]+iFloristOrderFormConstants[FLOWER_ORDER_CHECK_WIDTH]+3, iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_0_Y]+2, 0, iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	//When it get there
	DrawTextToScreen(sOrderFormText[FLORIST_ORDER_GETS_THERE], iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_1_X]+iFloristOrderFormConstants[FLOWER_ORDER_CHECK_WIDTH]+3, iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_1_Y]+2, 0, iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	//Deliverry locatiuon
	DrawTextToScreen(sOrderFormText[FLORIST_ORDER_DELIVERY_LOCATION], iFloristOrderFormConstants[FLOWER_ORDER_ORDER_NUM_NAME_X], iFloristOrderFormConstants[FLOWER_ORDER_LOCATION_Y], 0, iFloristOrderFormConstants[FLOWER_ORDEER_BIG_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDER_STATIC_TEXT_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	//Additional services
	DrawTextToScreen(sOrderFormText[FLORIST_ORDER_ADDITIONAL_CHARGES], iFloristOrderFormConstants[FLOWER_ORDER_ADDITIONAL_SERVICES_X], iFloristOrderFormConstants[FLOWER_ORDER_ADDITIONAL_SERVICES_Y], 0, iFloristOrderFormConstants[FLOWER_ORDEER_BIG_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDER_STATIC_TEXT_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	//crushed bouquet
	DrawTextToScreen(sOrderFormText[FLORIST_ORDER_CRUSHED], iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_2_X]+iFloristOrderFormConstants[FLOWER_ORDER_CHECK_WIDTH]+3, iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_2_Y]+2, 0, iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	//black roses
	DrawTextToScreen(sOrderFormText[FLORIST_ORDER_BLACK_ROSES], iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_3_X]+iFloristOrderFormConstants[FLOWER_ORDER_CHECK_WIDTH]+3, iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_3_Y]+2, 0, iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	//wilted bouquet
	DrawTextToScreen(sOrderFormText[FLORIST_ORDER_WILTED], iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_4_X]+iFloristOrderFormConstants[FLOWER_ORDER_CHECK_WIDTH]+3, iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_4_Y]+2, 0, iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	//fruit cake
	DrawTextToScreen(sOrderFormText[FLORIST_ORDER_FRUIT_CAKE], iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_5_X]+iFloristOrderFormConstants[FLOWER_ORDER_CHECK_WIDTH]+3, iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_5_Y]+2, 0, iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );


	//Personal sentiment
	usPosX = iFloristOrderFormConstants[FLOWER_ORDER_PERSONAL_SENT_TEXT_X] + StringPixLength( sOrderFormText[FLORIST_ORDER_PERSONAL_SENTIMENTS], iFloristOrderFormConstants[FLOWER_ORDEER_BIG_FONT]) + 5;
	DrawTextToScreen(sOrderFormText[FLORIST_ORDER_PERSONAL_SENTIMENTS], iFloristOrderFormConstants[FLOWER_ORDER_PERSONAL_SENT_TEXT_X], iFloristOrderFormConstants[FLOWER_ORDER_PERSONAL_SENT_TEXT_Y], 0, iFloristOrderFormConstants[FLOWER_ORDEER_BIG_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDER_STATIC_TEXT_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	DrawTextToScreen(sOrderFormText[FLORIST_ORDER_CARD_LENGTH], usPosX, iFloristOrderFormConstants[FLOWER_ORDER_PERSONAL_SENT_TEXT_Y]+2, 0, iFloristOrderFormConstants[FLOWER_ORDEER_TINY_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	//Billing information
	DrawTextToScreen(sOrderFormText[FLORIST_ORDER_BILLING_INFO], iFloristOrderFormConstants[FLOWER_ORDER_BILLING_INFO_X], iFloristOrderFormConstants[FLOWER_ORDER_BILLING_INFO_Y], 0, iFloristOrderFormConstants[FLOWER_ORDEER_BIG_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDER_STATIC_TEXT_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	//Billing Name
	DrawTextToScreen(sOrderFormText[FLORIST_ORDER_NAME], iFloristOrderFormConstants[FLOWER_ORDER_NAME_TEXT_X], iFloristOrderFormConstants[FLOWER_ORDER_NAME_TEXT_Y], iFloristOrderFormConstants[FLOWER_ORDER_NAME_TEXT_WIDTH], iFloristOrderFormConstants[FLOWER_ORDEER_BIG_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDER_STATIC_TEXT_COLOR], FONT_MCOLOR_BLACK, FALSE, RIGHT_JUSTIFIED );

	//the text to link to the card gallery
	DrawTextToScreen(sOrderFormText[FLORIST_ORDER_SELECT_FROM_OURS], iFloristOrderFormConstants[FLOWER_ORDER_LINK_TO_CARD_GALLERY_X], iFloristOrderFormConstants[FLOWER_ORDER_LINK_TO_CARD_GALLERY_Y], 0, iFloristOrderFormConstants[FLOWER_ORDEER_BIG_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDER_STATIC_TEXT_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
	usPosX = StringPixLength( sOrderFormText[FLORIST_ORDER_SELECT_FROM_OURS], iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_FONT]) + 5 + iFloristOrderFormConstants[FLOWER_ORDER_LINK_TO_CARD_GALLERY_X];
	DrawTextToScreen(sOrderFormText[FLORIST_ORDER_STANDARDIZED_CARDS], usPosX, iFloristOrderFormConstants[FLOWER_ORDER_LINK_TO_CARD_GALLERY_Y], 0, iFloristOrderFormConstants[FLOWER_ORDEER_BIG_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDEER_LINK_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	DisplayFloristCheckBox();

	//display all the things that change for the different bouquet collections
	DisplayFlowerDynamicItems();

	//Display the currently selected city
	FlowerOrderDisplayShippingLocationCity();

  MarkButtonsDirty( );
	RenderWWWProgramTitleBar( );
  InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}



void BtnFlowerOrderBackButtonCallback(GUI_BUTTON *btn,INT32 reason)
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

			guiCurrentLaptopMode = LAPTOP_MODE_FLORIST_FLOWER_GALLERY;

			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
} 

void BtnFlowerOrderSendButtonCallback(GUI_BUTTON *btn,INT32 reason)
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

			//add an entry in the finacial page for the medical deposit
			AddTransactionToPlayersBook(	PURCHASED_FLOWERS, 0, GetWorldTotalMin(), -(INT32)(guiFlowerPrice) );

			if ( gubCurrentlySelectedFlowerLocation == 7 )
			{
				// sent to meduna!
				if ( gfFLoristCheckBox0Down )
				{
					HandleFlowersMeanwhileScene( 0 );
				}
				else
				{
					HandleFlowersMeanwhileScene( 1 );
				}
			}

			//increment the order number
			LaptopSaveInfo.uiFlowerOrderNumber += ( 1 + Random( 2 ) );

			guiCurrentLaptopMode = LAPTOP_MODE_FLORIST;
			InitFloristOrderForm();

			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
} 

void BtnFlowerOrderClearButtonCallback(GUI_BUTTON *btn,INT32 reason)
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

			guiCurrentLaptopMode = LAPTOP_MODE_FLORIST;
			InitFloristOrderForm();

			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
} 

void BtnFlowerOrderGalleryButtonCallback(GUI_BUTTON *btn,INT32 reason)
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

			guiCurrentLaptopMode = LAPTOP_MODE_FLORIST_FLOWER_GALLERY;

			//reset the gallery back to page 0
			gubCurFlowerIndex = 0;

			InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
		}
	}
	if(reason & MSYS_CALLBACK_REASON_LOST_MOUSE)
	{
		btn->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(btn->Area.RegionTopLeftX, btn->Area.RegionTopLeftY, btn->Area.RegionBottomRightX, btn->Area.RegionBottomRightY);
	}
} 


void SelectFlorsitCheckBoxRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{ 
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		UINT32	uiUserData;

		uiUserData = MSYS_GetRegionUserData( pRegion, 0 );

		switch( uiUserData )
		{
			case 0:
				if( gfFLoristCheckBox0Down )
				{
					gfFLoristCheckBox0Down = FALSE;
					gfFLoristCheckBox1Down = TRUE;
				}
				else
				{
					gfFLoristCheckBox0Down = TRUE;
					gfFLoristCheckBox1Down = FALSE;
				}
				break;
			case 1:
				if( gfFLoristCheckBox1Down )
				{
					gfFLoristCheckBox1Down = FALSE;
					gfFLoristCheckBox0Down = TRUE;
				}
				else
				{
					gfFLoristCheckBox1Down = TRUE;
					gfFLoristCheckBox0Down = FALSE;
				}
				break;
			case 2:
				if( gfFLoristCheckBox2Down )
					gfFLoristCheckBox2Down = FALSE;
				else
					gfFLoristCheckBox2Down = TRUE;
				break;
			case 3:
				if( gfFLoristCheckBox3Down )
					gfFLoristCheckBox3Down = FALSE;
				else
					gfFLoristCheckBox3Down = TRUE;
				break;
			case 4:
				if( gfFLoristCheckBox4Down )
					gfFLoristCheckBox4Down = FALSE;
				else
					gfFLoristCheckBox4Down = TRUE;
				break;
			case 5:
				if( gfFLoristCheckBox5Down )
					gfFLoristCheckBox5Down = FALSE;
				else
					gfFLoristCheckBox5Down = TRUE;
				break;
		}
		DisplayFloristCheckBox();
		fPausedReDrawScreenFlag = TRUE;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	} 
}

void DisplayFloristCheckBox()
{
  HVOBJECT hPixHandle;

	//check box
	GetVideoObject(&hPixHandle, guiFlowerOrderCheckBoxButtonImage);
	if( gfFLoristCheckBox0Down )
		BltVideoObject(FRAME_BUFFER, hPixHandle, 1,  iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_0_X], iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_0_Y], VO_BLT_SRCTRANSPARENCY,NULL);
	else
		BltVideoObject(FRAME_BUFFER, hPixHandle, 0,  iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_0_X], iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_0_Y], VO_BLT_SRCTRANSPARENCY,NULL);

	//first check box
	GetVideoObject(&hPixHandle, guiFlowerOrderCheckBoxButtonImage);
	if( gfFLoristCheckBox1Down )
		BltVideoObject(FRAME_BUFFER, hPixHandle, 1,  iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_1_X], iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_1_Y], VO_BLT_SRCTRANSPARENCY,NULL);
	else
		BltVideoObject(FRAME_BUFFER, hPixHandle, 0,  iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_1_X],  iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_1_Y], VO_BLT_SRCTRANSPARENCY,NULL);

	//second check box
	GetVideoObject(&hPixHandle, guiFlowerOrderCheckBoxButtonImage);
	if( gfFLoristCheckBox2Down )
		BltVideoObject(FRAME_BUFFER, hPixHandle, 1,  iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_2_X],  iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_2_Y], VO_BLT_SRCTRANSPARENCY,NULL);
	else
		BltVideoObject(FRAME_BUFFER, hPixHandle, 0,  iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_2_X],  iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_2_Y], VO_BLT_SRCTRANSPARENCY,NULL);

	//third check box
	GetVideoObject(&hPixHandle, guiFlowerOrderCheckBoxButtonImage);
	if( gfFLoristCheckBox3Down )
		BltVideoObject(FRAME_BUFFER, hPixHandle, 1,  iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_3_X],  iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_3_Y], VO_BLT_SRCTRANSPARENCY,NULL);
	else
		BltVideoObject(FRAME_BUFFER, hPixHandle, 0,  iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_3_X],  iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_3_Y], VO_BLT_SRCTRANSPARENCY,NULL);

	//Foiurth check box
	GetVideoObject(&hPixHandle, guiFlowerOrderCheckBoxButtonImage);
	if( gfFLoristCheckBox4Down )
		BltVideoObject(FRAME_BUFFER, hPixHandle, 1,  iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_4_X],  iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_4_Y], VO_BLT_SRCTRANSPARENCY,NULL);
	else
		BltVideoObject(FRAME_BUFFER, hPixHandle, 0,  iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_4_X],  iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_4_Y], VO_BLT_SRCTRANSPARENCY,NULL);

	//fifth check box
	GetVideoObject(&hPixHandle, guiFlowerOrderCheckBoxButtonImage);
	if( gfFLoristCheckBox5Down )
		BltVideoObject(FRAME_BUFFER, hPixHandle, 1,  iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_5_X],  iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_5_Y], VO_BLT_SRCTRANSPARENCY,NULL);
	else
		BltVideoObject(FRAME_BUFFER, hPixHandle, 0,  iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_5_X],  iFloristOrderFormConstants[FLOWER_ORDER_CHECK_BOX_5_Y], VO_BLT_SRCTRANSPARENCY,NULL);

  InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
}




void SelectFloristCardGalleryLinkRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{ 
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		guiCurrentLaptopMode = LAPTOP_MODE_FLORIST_CARD_GALLERY;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	} 
}


//display the things that change on the screen
void DisplayFlowerDynamicItems()
{
	UINT32	uiStartLoc=0;
	UINT16	usPosX;
	CHAR16	sTemp[ 640 ];
//	CHAR16	sText[ 640 ];
	UINT16	usPrice;
/*
	//display the card saying
	if( gbCurrentlySelectedCard != -1 )
	{
		//Get and display the card saying
		//Display Flower Desc

		uiStartLoc = FLOR_CARD_TEXT_TITLE_SIZE * + gbCurrentlySelectedCard;
		LoadEncryptedDataFromFile( FLOR_CARD_TEXT_FILE, sTemp, uiStartLoc, FLOR_CARD_TEXT_TITLE_SIZE);

		CleanOutControlCodesFromString(sTemp, sText);

		DisplayWrappedString( (UINT16)(FLOWER_ORDER_SENTIMENT_BOX_X+10), (UINT16)(FLOWER_ORDER_SENTIMENT_BOX_Y+7), FLOWER_ORDER_PERSONAL_SENT_TEXT_WIDTH, 2, FLOWER_ORDEER_SMALL_FONT, FLOWER_ORDEER_SMALL_COLOR,  sText, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED);
	}
*/
	//order number
	usPosX = StringPixLength( sOrderFormText[FLORIST_ORDER_ORDER_NUMBER], iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_FONT]) + 5 + iFloristOrderFormConstants[FLOWER_ORDER_ORDER_NUM_NAME_X];
	swprintf(sTemp, L"%d", LaptopSaveInfo.uiFlowerOrderNumber );
	DrawTextToScreen( sTemp, usPosX, iFloristOrderFormConstants[FLOWER_ORDER_ORDER_NUM_NAME_Y], 0, iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );

	guiFlowerPrice = 0;
	// the user selected crushed bouquet
	if( gfFLoristCheckBox2Down )
		guiFlowerPrice += gubFlowerOrder_AdditioanalServicePrices[0];

	// the user selected blak roses
	if( gfFLoristCheckBox3Down )
		guiFlowerPrice += gubFlowerOrder_AdditioanalServicePrices[1];

	// the user selected wilted bouquet
	if( gfFLoristCheckBox4Down )
		guiFlowerPrice += gubFlowerOrder_AdditioanalServicePrices[2];

	// the user selected fruit cake
	if( gfFLoristCheckBox5Down )
		guiFlowerPrice += gubFlowerOrder_AdditioanalServicePrices[3];

	//price
	usPosX = StringPixLength( sOrderFormText[FLORIST_ORDER_PRICE], iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_FONT]) + 5 + iFloristOrderFormConstants[FLOWER_ORDER_BOUQUET_NAME_X];
	uiStartLoc = FLOR_GALLERY_TEXT_TOTAL_SIZE * guiCurrentlySelectedFlower + FLOR_GALLERY_TEXT_TITLE_SIZE;
	LoadEncryptedDataFromFile(FLOR_GALLERY_TEXT_FILE, sTemp, uiStartLoc, FLOR_GALLERY_TEXT_PRICE_SIZE);
	swscanf( sTemp, L"%hu", &usPrice);

	//if its the next day delivery
	if( gfFLoristCheckBox0Down )
		guiFlowerPrice += usPrice + FlowerOrderLocations[ gubCurrentlySelectedFlowerLocation ].ubNextDayDeliveryCost;
	//else its the 'when it gets there' delivery
	else
		guiFlowerPrice += usPrice + FlowerOrderLocations[ gubCurrentlySelectedFlowerLocation ].ubWhenItGetsThereCost;

	swprintf( sTemp, L"$%d.00 %s", guiFlowerPrice, pMessageStrings[ MSG_USDOLLAR_ABBREVIATION ] );
	DrawTextToScreen( sTemp, usPosX, iFloristOrderFormConstants[FLOWER_ORDER_BOUQUET_NAME_Y], 0, iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED );
}


void SelectFloristGalleryLinkRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{ 
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if(iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		guiCurrentLaptopMode = LAPTOP_MODE_FLORIST_FLOWER_GALLERY;
	}
	else if (iReason & MSYS_CALLBACK_REASON_RBUTTON_UP)
	{
	} 
}


void SelectFloristDropDownRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{ 
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		gubFlowerDestDropDownMode = FLOWER_ORDER_DROP_DOWN_CREATE;
	} 
}

void SelectFloristDisableDropDownRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{ 
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		gubFlowerDestDropDownMode = FLOWER_ORDER_DROP_DOWN_DESTROY;
	} 
}


void SelectFlowerDropDownRegionCallBack(MOUSE_REGION * pRegion, INT32 iReason )
{ 
	if (iReason & MSYS_CALLBACK_REASON_INIT)
	{
	}
	else if (iReason & MSYS_CALLBACK_REASON_LBUTTON_UP)
	{
		gubCurrentlySelectedFlowerLocation = (UINT8)MSYS_GetRegionUserData( pRegion, 0 );
		FlowerOrderDrawSelectedCity( gubCurrentlySelectedFlowerLocation );
		gubFlowerDestDropDownMode = FLOWER_ORDER_DROP_DOWN_DESTROY;
	} 
}

void SelectFlowerDropDownMovementCallBack(MOUSE_REGION * pRegion, INT32 reason )
{
	if( reason & MSYS_CALLBACK_REASON_LOST_MOUSE )
	{
		pRegion->uiFlags &= (~BUTTON_CLICKED_ON );
		InvalidateRegion(pRegion->RegionTopLeftX, pRegion->RegionTopLeftY, pRegion->RegionBottomRightX, pRegion->RegionBottomRightY);
	}
	else if( reason & MSYS_CALLBACK_REASON_GAIN_MOUSE )
	{
		pRegion->uiFlags |= BUTTON_CLICKED_ON ;

		gubCurrentlySelectedFlowerLocation = (UINT8)MSYS_GetRegionUserData( pRegion, 0 );
		FlowerOrderDrawSelectedCity( gubCurrentlySelectedFlowerLocation );

		InvalidateRegion(pRegion->RegionTopLeftX, pRegion->RegionTopLeftY, pRegion->RegionBottomRightX, pRegion->RegionBottomRightY);
	}
} 




BOOLEAN CreateDestroyFlowerOrderDestDropDown( UINT8 ubDropDownMode )
{
	static UINT16 usHeight;
	static BOOLEAN fMouseRegionsCreated=FALSE;

	switch( ubDropDownMode )
	{
		case 	FLOWER_ORDER_DROP_DOWN_NO_ACTION:
		{

		}
		break;

		case 	FLOWER_ORDER_DROP_DOWN_CREATE:
		{
			UINT8 i;
			UINT16 usPosX, usPosY;
			UINT16 usTemp;
			UINT16 usFontHeight = GetFontHeight( iFloristOrderFormConstants[FLOWER_ORDEER_DROP_DOWN_FONT] );
			UINT8 ubTextFieldID;

			if( fMouseRegionsCreated )
			{
				return(FALSE);
				break;
			}

//Get the current text from the text box
			ubTextFieldID = (UINT8) GetActiveFieldID();

			//if its the personel sentiment field
			if( ubTextFieldID == 1 )
			{
				Get16BitStringFromField( ubTextFieldID, gsSentimentTextField );
			}
			else if( ubTextFieldID == 2 )
			{
				//else its the name field
				Get16BitStringFromField( ubTextFieldID, gsNameTextField );
			}

			SetActiveField(0);



			fMouseRegionsCreated = TRUE;

			usPosX = iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_CITY_START_X];
			usPosY = iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_CITY_START_Y];
			for( i=0; i< iFloristOrderFormConstants[FLOWER_ORDER_NUMBER_OF_DROP_DOWN_LOCATIONS]; i++)
			{
				MSYS_DefineRegion( &gSelectedFlowerDropDownRegion[i], usPosX, (UINT16)(usPosY+4), (UINT16)(usPosX+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_WIDTH]), (UINT16)(usPosY+usFontHeight), MSYS_PRIORITY_HIGH+3,
										 CURSOR_WWW, SelectFlowerDropDownMovementCallBack, SelectFlowerDropDownRegionCallBack); 
				MSYS_AddRegion(&gSelectedFlowerDropDownRegion[i]); 
				MSYS_SetRegionUserData( &gSelectedFlowerDropDownRegion[ i ], 0, i);

				usPosY += usFontHeight + 2;
			}
			usTemp = iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_CITY_START_Y];
			usHeight = usPosY - usTemp + 10;


			gubFlowerDestDropDownMode = FLOWER_ORDER_DROP_DOWN_DISPLAY;
		  MSYS_EnableRegion(&gSelectedFloristDisableDropDownRegion); 


			//disable the text entry fields
//			DisableAllTextFields();
				Get16BitStringFromField( 1, gsSentimentTextField );
				KillTextInputMode();

			//disable the clear order and accept order buttons, (their rendering interferes with the drop down graphics)
	}
		break;
		
		case 	FLOWER_ORDER_DROP_DOWN_DESTROY:
		{
			UINT8 i;

			if( !fMouseRegionsCreated )
				break;

			for( i=0; i< iFloristOrderFormConstants[FLOWER_ORDER_NUMBER_OF_DROP_DOWN_LOCATIONS]; i++)
			  MSYS_RemoveRegion( &gSelectedFlowerDropDownRegion[i]);

			//display the name on the title bar
			ColorFillVideoSurfaceArea( FRAME_BUFFER, iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_X]+3, iFloristOrderFormConstants[FLOWER_ORDER_DELIVERY_LOCATION_Y]+3, iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_X]+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_WIDTH],	iFloristOrderFormConstants[FLOWER_ORDER_DELIVERY_LOCATION_Y]+iFloristOrderFormConstants[FLOWER_ORDER_DELIVERY_LOCATION_HEIGHT]-2, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
			DrawTextToScreen( *(FlowerOrderLocations[ gubCurrentlySelectedFlowerLocation ].psCityLoc), iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_CITY_START_X]+6, iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_CITY_START_Y]+3, 0, iFloristOrderFormConstants[FLOWER_ORDEER_DROP_DOWN_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDEER_DROP_DOWN_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED	);

			//enable the drop down region
		  MSYS_DisableRegion(&gSelectedFloristDisableDropDownRegion); 

			fPausedReDrawScreenFlag = TRUE;
			fMouseRegionsCreated  = FALSE;
			gubFlowerDestDropDownMode = FLOWER_ORDER_DROP_DOWN_NO_ACTION;

			//enable the text entry fields
			InitFlowerOrderTextInputBoxes();
	}
		break;

		case 	FLOWER_ORDER_DROP_DOWN_DISPLAY:
		{
			UINT8 i;
			UINT16 usPosY, usPosX;
			UINT16 usFontHeight = GetFontHeight( iFloristOrderFormConstants[FLOWER_ORDEER_DROP_DOWN_FONT] );
		  HVOBJECT	hImageHandle;

			//Display the background for the drop down window
			ColorFillVideoSurfaceArea( FRAME_BUFFER, iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_X], iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_Y], iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_X]+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_WIDTH],	iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_Y]+usHeight, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );

			//
			// Place the border around the background
			//

			GetVideoObject(&hImageHandle, guiDropDownBorder);

			usPosX = usPosY = 0;
			//blit top row of images
			for(i=10; i< iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_WIDTH]-10; i+=10)
			{
				//TOP ROW
				BltVideoObject(FRAME_BUFFER, hImageHandle, 1,i+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_X], usPosY+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_Y], VO_BLT_SRCTRANSPARENCY,NULL);
				
				//BOTTOM ROW
				BltVideoObject(FRAME_BUFFER, hImageHandle, 6,i+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_X], usHeight-10+6+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_Y], VO_BLT_SRCTRANSPARENCY,NULL);
			}

			//blit the left and right row of images
			usPosX = 0;
			for(i=10; i< usHeight-10; i+=10)
			{
				BltVideoObject(FRAME_BUFFER, hImageHandle, 3,usPosX+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_X], i+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_Y], VO_BLT_SRCTRANSPARENCY,NULL);
				BltVideoObject(FRAME_BUFFER, hImageHandle, 4,usPosX+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_WIDTH]-4+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_X], i+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_Y], VO_BLT_SRCTRANSPARENCY,NULL);
			}

			//blt the corner images for the row
			//top left
			BltVideoObject(FRAME_BUFFER, hImageHandle, 0, 0+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_X], usPosY+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_Y], VO_BLT_SRCTRANSPARENCY,NULL);
			//top right
			BltVideoObject(FRAME_BUFFER, hImageHandle, 2, iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_WIDTH]-10+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_X], usPosY+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_Y], VO_BLT_SRCTRANSPARENCY,NULL);
			//bottom left
			BltVideoObject(FRAME_BUFFER, hImageHandle, 5, 0+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_X], usHeight-10+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_Y], VO_BLT_SRCTRANSPARENCY,NULL);
			//bottom right
			BltVideoObject(FRAME_BUFFER, hImageHandle, 7, iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_WIDTH]-10+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_X], usHeight-10+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_Y], VO_BLT_SRCTRANSPARENCY,NULL);


			//Display the list of cities
			usPosY = iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_CITY_START_Y] + 3;
			for( i=0; i< iFloristOrderFormConstants[FLOWER_ORDER_NUMBER_OF_DROP_DOWN_LOCATIONS]; i++)
			{
				DrawTextToScreen( *(FlowerOrderLocations[i].psCityLoc), iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_CITY_START_X]+6, usPosY, 0, iFloristOrderFormConstants[FLOWER_ORDEER_DROP_DOWN_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDEER_DROP_DOWN_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED	);
				usPosY += usFontHeight + 2;
			}

			FlowerOrderDrawSelectedCity( gubCurrentlySelectedFlowerLocation );

		  InvalidateRegion(LAPTOP_SCREEN_UL_X,LAPTOP_SCREEN_WEB_UL_Y,LAPTOP_SCREEN_LR_X,LAPTOP_SCREEN_WEB_LR_Y);
		}
		break;
	}
	return(TRUE);
}


void FlowerOrderDrawSelectedCity( UINT8 ubNumber )
{
	UINT16 usPosY;
	UINT16 usFontHeight = GetFontHeight( iFloristOrderFormConstants[FLOWER_ORDEER_DROP_DOWN_FONT] );

	usPosY = (usFontHeight + 2) * ubNumber + iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_CITY_START_Y];

	//display the name in the list
	ColorFillVideoSurfaceArea( FRAME_BUFFER, iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_CITY_START_X], usPosY+2, iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_CITY_START_X]+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_WIDTH]-9,	usPosY+usFontHeight+4, Get16BPPColor( FROMRGB( 255, 255, 255 ) ) );

	SetFontShadow(NO_SHADOW);	
	DrawTextToScreen( *(FlowerOrderLocations[ubNumber].psCityLoc), iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_CITY_START_X]+6, (UINT16)(usPosY+3), 0, iFloristOrderFormConstants[FLOWER_ORDEER_DROP_DOWN_FONT], 2, FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED	);
	SetFontShadow(DEFAULT_SHADOW);

	FlowerOrderDisplayShippingLocationCity();
	SetFontShadow(DEFAULT_SHADOW);	

}


void FlowerOrderDisplayShippingLocationCity()
{
	//display the name on the title bar
	ColorFillVideoSurfaceArea( FRAME_BUFFER, iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_X]+3, iFloristOrderFormConstants[FLOWER_ORDER_DELIVERY_LOCATION_Y]+3, iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_X]+iFloristOrderFormConstants[FLOWER_ORDER_DROP_DOWN_LOCATION_WIDTH],	iFloristOrderFormConstants[FLOWER_ORDER_DELIVERY_LOCATION_Y]+iFloristOrderFormConstants[FLOWER_ORDER_DELIVERY_LOCATION_HEIGHT]-2, Get16BPPColor( FROMRGB( 0, 0, 0 ) ) );
	DrawTextToScreen( *(FlowerOrderLocations[ gubCurrentlySelectedFlowerLocation ].psCityLoc), iFloristOrderFormConstants[FLOWER_ORDER_DELIVERY_LOCATION_X]+5, iFloristOrderFormConstants[FLOWER_ORDER_DELIVERY_LOCATION_Y]+5, 0, iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_FONT], (UINT8)iFloristOrderFormConstants[FLOWER_ORDEER_SMALL_COLOR], FONT_MCOLOR_BLACK, FALSE, LEFT_JUSTIFIED	);
}


void InitFlowerOrderTextInputBoxes()
{
	UINT32	uiStartLoc=0;
	CHAR16	sTemp[ 640 ];
	CHAR16	sText[ 640 ];

	
	InitTextInputMode();
	SetTextInputFont( (UINT16)FONT12ARIAL );
	Set16BPPTextFieldColor( Get16BPPColor(FROMRGB( 255, 255, 255) ) );
	SetBevelColors( Get16BPPColor(FROMRGB(136, 138, 135)), Get16BPPColor(FROMRGB(24, 61, 81)) );
	SetTextInputRegularColors( 2, FONT_WHITE ); 
	SetTextInputHilitedColors( FONT_WHITE, 2, 141  );
	SetCursorColor( Get16BPPColor(FROMRGB(0, 0, 0) ) );

	AddUserInputField( FlowerOrderUserTextFieldCallBack );

	if( gbCurrentlySelectedCard != -1 )
	{
		//Get and display the card saying
		//Display Flower Desc

		uiStartLoc = FLOR_CARD_TEXT_TITLE_SIZE * + gbCurrentlySelectedCard;
		LoadEncryptedDataFromFile( FLOR_CARD_TEXT_FILE, sTemp, uiStartLoc, FLOR_CARD_TEXT_TITLE_SIZE);
		CleanOutControlCodesFromString(sTemp, sText);

		wcsncpy( gsSentimentTextField, sText, iFloristOrderFormConstants[FLOWER_ORDER_PERSONEL_SENTIMENT_NUM_CHARS] - 1 );

		gbCurrentlySelectedCard = -1;

	}

	if( wcslen( gsSentimentTextField ) >= iFloristOrderFormConstants[FLOWER_ORDER_PERSONEL_SENTIMENT_NUM_CHARS] - 2 )
	{
		gsSentimentTextField[ iFloristOrderFormConstants[FLOWER_ORDER_PERSONEL_SENTIMENT_NUM_CHARS] - 1] = L'\0';
	}

	//personal sentiment box
	AddTextInputField( iFloristOrderFormConstants[FLOWER_ORDER_PERSONAL_SENT_BOX_X], iFloristOrderFormConstants[FLOWER_ORDER_PERSONAL_SENT_BOX_Y], iFloristOrderFormConstants[FLOWER_ORDER_PERSONAL_SENT_TEXT_WIDTH], iFloristOrderFormConstants[FLOWER_ORDER_PERSONAL_SENT_TEXT_HEIGHT], MSYS_PRIORITY_HIGH+2, gsSentimentTextField, (UINT8)iFloristOrderFormConstants[FLOWER_ORDER_PERSONEL_SENTIMENT_NUM_CHARS], INPUTTYPE_ASCII );

	//Name box
	AddTextInputField( iFloristOrderFormConstants[FLOWER_ORDER_NAME_TEXT_BOX_X], iFloristOrderFormConstants[FLOWER_ORDER_NAME_TEXT_BOX_Y], iFloristOrderFormConstants[FLOWER_ORDER_NAME_TEXT_BOX_WIDTH], iFloristOrderFormConstants[FLOWER_ORDER_NAME_TEXT_BOX_HEIGHT], MSYS_PRIORITY_HIGH+2, gsNameTextField, (UINT8)iFloristOrderFormConstants[FLOWER_ORDER_NAME_FIELD_NUM_CHARS], INPUTTYPE_ASCII );
}

void DestroyFlowerOrderTextInputBoxes()
{
	KillTextInputMode();
}


void HandleFloristOrderKeyBoardInput()
{
	InputAtom					InputEvent;

	while (DequeueEvent(&InputEvent) == TRUE)
	{
		if( !HandleTextInput( &InputEvent ) && InputEvent.usEvent == KEY_DOWN )
		{
			UINT8 ubTextFieldID;
			switch (InputEvent.usParam)
			{
				case ENTER:

					ubTextFieldID = (UINT8) GetActiveFieldID();

					//if its the personel sentiment field
					if( ubTextFieldID == 1 )
					{
						Get16BitStringFromField( ubTextFieldID, gsSentimentTextField );
					}
					else if( ubTextFieldID == 2 )
					{
						//else its the name field
						Get16BitStringFromField( ubTextFieldID, gsNameTextField );
					}

					SetActiveField(0);
					break;

				case ESC:
					SetActiveField(0);
					break;

				default:
					HandleKeyBoardShortCutsForLapTop( InputEvent.usEvent, InputEvent.usParam, InputEvent.usKeyState );
					break;
			}
		}
	}
}



void FlowerOrderUserTextFieldCallBack( UINT8 ubID, BOOLEAN fEntering )
{
	if( fEntering )
	{
//		SetActiveField(1);
 	}
}


//Initialize the Florsit Order Page (reset some variables)
void InitFloristOrderForm()
{
	gsSentimentTextField[0] = 0;

	gfFLoristCheckBox0Down = FALSE;			// next day delviery
	gfFLoristCheckBox1Down = TRUE;			// when it gets there delivery
	gfFLoristCheckBox2Down = FALSE;
	gfFLoristCheckBox3Down = FALSE;
	gfFLoristCheckBox4Down = FALSE;
	gfFLoristCheckBox5Down = FALSE;

	guiFlowerPrice = 0;

	gubCurrentlySelectedFlowerLocation = 0;
	gbCurrentlySelectedCard = -1;

	gsSentimentTextField[0] = 0;
	gsNameTextField[0] = 0;

}
