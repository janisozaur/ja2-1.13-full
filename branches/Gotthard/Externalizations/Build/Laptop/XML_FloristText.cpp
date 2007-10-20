/*

//Gotthard 10/16/07
//Things I still would like to externalize... Don't know how to do defines yet.

//From Florist Order Form.cpp

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

//From Florist Gallery.cpp

#define	FLOR_GALLERY_TITLE_FONT								FONT10ARIAL
#define	FLOR_GALLERY_TITLE_COLOR							FONT_MCOLOR_WHITE

#define	FLOR_GALLERY_FLOWER_TITLE_FONT				FONT14ARIAL
#define	FLOR_GALLERY_FLOWER_TITLE_COLOR				FONT_MCOLOR_WHITE

#define	FLOR_GALLERY_FLOWER_PRICE_FONT				FONT12ARIAL
#define	FLOR_GALLERY_FLOWER_PRICE_COLOR				FONT_MCOLOR_WHITE

#define	FLOR_GALLERY_FLOWER_DESC_FONT					FONT12ARIAL
#define	FLOR_GALLERY_FLOWER_DESC_COLOR				FONT_MCOLOR_WHITE

#define	FLOR_GALLERY_NUMBER_FLORAL_BUTTONS		3
#define	FLOR_GALLERY_NUMBER_FLORAL_IMAGES			10

#define	FLOR_GALLERY_FLOWER_DESC_TEXT_FONT		FONT12ARIAL
#define	FLOR_GALLERY_FLOWER_DESC_TEXT_COLOR		FONT_MCOLOR_WHITE

#define FLOR_GALLERY_BACK_BUTTON_X						(LAPTOP_SCREEN_UL_X + 8)
#define FLOR_GALLERY_BACK_BUTTON_Y						LAPTOP_SCREEN_WEB_UL_Y + 12

#define FLOR_GALLERY_NEXT_BUTTON_X						LAPTOP_SCREEN_UL_X + 420
#define FLOR_GALLERY_NEXT_BUTTON_Y						FLOR_GALLERY_BACK_BUTTON_Y
	
#define FLOR_GALLERY_FLOWER_BUTTON_X					(LAPTOP_SCREEN_UL_X + 7)
#define FLOR_GALLERY_FLOWER_BUTTON_Y					LAPTOP_SCREEN_WEB_UL_Y + 74

#define FLOR_GALLERY_FLOWER_BUTTON_OFFSET_Y		112

#define FLOR_GALLERY_TITLE_TEXT_X							(LAPTOP_SCREEN_UL_X + 0)
#define FLOR_GALLERY_TITLE_TEXT_Y							LAPTOP_SCREEN_WEB_UL_Y + 48
#define FLOR_GALLERY_TITLE_TEXT_WIDTH						(613 - 111)	

#define FLOR_GALLERY_FLOWER_TITLE_X						FLOR_GALLERY_FLOWER_BUTTON_X + 88

#define FLOR_GALLERY_DESC_WIDTH								390
	
#define FLOR_GALLERY_FLOWER_TITLE_OFFSET_Y		9
#define FLOR_GALLERY_FLOWER_PRICE_OFFSET_Y		FLOR_GALLERY_FLOWER_TITLE_OFFSET_Y + 17
#define FLOR_GALLERY_FLOWER_DESC_OFFSET_Y			FLOR_GALLERY_FLOWER_PRICE_OFFSET_Y + 15

//From Florist.cpp

#define		FLORIST_SENTENCE_FONT							FONT12ARIAL
#define		FLORIST_SENTENCE_COLOR						FONT_MCOLOR_WHITE

#define		FLORIST_TITLE_FONT								FONT14ARIAL
#define		FLORIST_TITLE_COLOR								FONT_MCOLOR_WHITE

#define		FLORIST_BACKGROUND_WIDTH					125
#define		FLORIST_BACKGROUND_HEIGHT					100

#define		FLORIST_BIG_TITLE_X								LAPTOP_SCREEN_UL_X + 113
#define		FLORIST_BIG_TITLE_Y								LAPTOP_SCREEN_WEB_UL_Y + 0

#define		FLORIST_SMALL_TITLE_X							LAPTOP_SCREEN_UL_X + 195
#define		FLORIST_SMALL_TITLE_Y							LAPTOP_SCREEN_WEB_UL_Y + 0
#define		FLORIST_SMALL_TITLE_WIDTH					100
#define		FLORIST_SMALL_TITLE_HEIGHT					49


#define		FLORIST_FIRST_BULLET_X						LAPTOP_SCREEN_UL_X + 5
#define		FLORIST_FIRST_BULLET_Y						LAPTOP_SCREEN_WEB_UL_Y + 135

#define		FLORIST_BULLET_OFFSET_Y						54

#define		FLORIST_NUMBER_OF_BULLETS					4

#define		FLORIST_GALLERY_BUTTON_X					LAPTOP_SCREEN_UL_X + 210
#define		FLORIST_GALLERY_BUTTON_Y					LAPTOP_SCREEN_WEB_UL_Y + 360

#define		FLORIST_FIRST_SENTENCE_COLUMN_TEXT_X	LAPTOP_SCREEN_UL_X + 53
#define		FLORIST_FIRST_SENTENCE_COLUMN_TEXT_WIDTH	136

#define		FLORIST_SECOND_SENTENCE_COLUMN_TEXT_X			LAPTOP_SCREEN_UL_X + 200
#define		FLORIST_SECOND_SENTENCE_COLUMN_TEXT_WIDTH	300

#define		FLORIST_COMPANY_INFO_TEXT_X				LAPTOP_SCREEN_UL_X + 117
#define		FLORIST_COMPANY_INFO_TEXT_WIDTH		290

#define		FLORIST_COMPANY_INFO_LINE_1_Y			LAPTOP_SCREEN_WEB_UL_Y + 79
#define		FLORIST_COMPANY_INFO_LINE_2_Y			LAPTOP_SCREEN_WEB_UL_Y + 94
#define		FLORIST_COMPANY_INFO_LINE_3_Y			LAPTOP_SCREEN_WEB_UL_Y + 107
#define		FLORIST_COMPANY_INFO_LINE_4_Y			LAPTOP_SCREEN_WEB_UL_Y + 119
*/


#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead types.h"
	#include "Sound Control.h"
	#include "Soldier Control.h"
	#include "overhead.h"
	#include "Event Pump.h"
	#include "weapons.h"
	#include "Animation Control.h"
	#include "sys globals.h"
	#include "Handle UI.h"
	#include "Isometric Utils.h"
	#include "worldman.h"
	#include "math.h"
	#include "points.h"
	#include "ai.h"
	#include "los.h"
	#include "renderworld.h"
	#include "opplist.h"
	#include "interface.h"
	#include "message.h"
	#include "campaign.h"
	#include "items.h"
	#include "text.h"
	#include "Soldier Profile.h"
	#include "tile animation.h"
	#include "Dialogue Control.h"
	#include "SkillCheck.h"
	#include "explosion control.h"
	#include "Quests.h"
	#include "Physics.h"
	#include "Random.h"
	#include "Vehicles.h"
	#include "bullets.h"
	#include "morale.h"
	#include "meanwhile.h"
	#include "SkillCheck.h"
	#include "gamesettings.h"
	#include "SaveLoadMap.h"
	#include "Debug Control.h"
	#include "expat.h"
	#include "XML.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];

	UINT32			maxArraySize;
	UINT32			curIndex;	
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef FloristParseData;

static void XMLCALL 
FloristStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	FloristParseData * pData = (FloristParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "FloristData") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strstr(name, "FloristText") != NULL && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
			pData->curIndex++;
		}
		else if(pData->curElement == ELEMENT && strcmp(name, "TEXT") == 0 )
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
FloristCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	FloristParseData * pData = (FloristParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && 
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	  ){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
FloristEndElementHandle(void *userData, const XML_Char *name)
{	
	char temp;
	FloristParseData * pData = (FloristParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "FloristData") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "FloristText") == 0)
		{
			pData->curElement = ELEMENT_LIST;
		}
		else if(strcmp(name, "TEXT") == 0)
		{
			//YES, I realize this is not done that well, but give me a break I'm not that great of a coder.
			//Maybe later we can have additional text in the florist area, but until then at least it can be changed.
			//Gotthard, 10/16/07
			pData->curElement = ELEMENT;
			if(pData->curIndex < pData->maxArraySize)
			{
				for(int i=0;i<min((int)strlen(pData->szCharData),MAX_CHAR_DATA_LENGTH);i++)
				{
					temp = pData->szCharData[i];
					if(pData->curIndex < 14)
					{
						sFloristText[pData->curIndex][i] = temp;
					}
					else if(pData->curIndex < 36)
					{
						sOrderFormText[pData->curIndex - 14][i] = temp;
					}
					else if(pData->curIndex < 41)
					{
						sFloristGalleryText[pData->curIndex - 36][i] = temp;
					}
					else if(pData->curIndex < 43)
					{
						sFloristCards[pData->curIndex - 41][i] = temp;
					}
				}
			}
		}
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}
BOOLEAN ReadInFloristText(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	FloristParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading Florist.xml" );

	// Open ammos file
	hFile = FileOpen( fileName, FILE_ACCESS_READ, FALSE );
	if ( !hFile )
		return( FALSE );
	
	uiFSize = FileGetSize(hFile);
	lpcBuffer = (CHAR8 *) MemAlloc(uiFSize+1);

	//Read in block
	if ( !FileRead( hFile, lpcBuffer, uiFSize, &uiBytesRead ) )
	{
		MemFree(lpcBuffer);
		return( FALSE );
	}

	lpcBuffer[uiFSize] = 0; //add a null terminator

	FileClose( hFile );

	
	XML_SetElementHandler(parser, FloristStartElementHandle, FloristEndElementHandle);
	XML_SetCharacterDataHandler(parser, FloristCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	pData.maxArraySize = MAXITEMS; 
	pData.curIndex = -1;
	
	XML_SetUserData(parser, &pData);


    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Florist.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);

	return( TRUE );
}