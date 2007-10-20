/*
//These still need to be externalized, 47 defines altogether.
Think we can try FuneralDefines[47], and make all the #define names enum instead.  Then copy in.
#define		FUNERAL_SENTENCE_FONT							FONT12ARIAL
#define		FUNERAL_SENTENCE_COLOR						2//FONT_MCOLOR_WHITE
#define		FUNERAL_SENTENCE_SHADOW_COLOR			FONT_MCOLOR_WHITE//FONT_MCOLOR_DKWHITE

#define		FUNERAL_SMALL_FONT								FONT10ARIAL

#define		FUNERAL_TITLE_FONT								FONT14ARIAL
#define		FUNERAL_TITLE_COLOR								FONT_MCOLOR_WHITE
#define		FUNERAL_TITLE_SHADOW_COLOR				FONT_MCOLOR_DKWHITE

#define		FUNERAL_RIP_SHADOW_COLOR					FONT_MCOLOR_DKWHITE


#define		FUNERAL_MCGILICUTTYS_SIGN_X				LAPTOP_SCREEN_UL_X + 92
#define		FUNERAL_MCGILICUTTYS_SIGN_Y				LAPTOP_SCREEN_WEB_UL_Y + 0

#define		FUNERAL_MORTUARY_SIGN_X						LAPTOP_SCREEN_UL_X + 58
#define		FUNERAL_MORTUARY_SIGN_Y						LAPTOP_SCREEN_WEB_UL_Y + 43

#define		FUNERAL_LEFT_COLUMN_X							LAPTOP_SCREEN_UL_X + 0
#define		FUNERAL_LEFT_COLUMN_Y							LAPTOP_SCREEN_WEB_UL_Y + 43

#define		FUNERAL_RIGHT_COLUMN_X						LAPTOP_SCREEN_UL_X + 442
#define		FUNERAL_RIGHT_COLUMN_Y						LAPTOP_SCREEN_WEB_UL_Y + 43

#define		FUNERAL_LINK_1_X									LAPTOP_SCREEN_UL_X + 37
#define		FUNERAL_LINK_1_Y									LAPTOP_SCREEN_WEB_UL_Y + 329
#define		FUNERAL_LINK_1_WIDTH							85
#define		FUNERAL_LINK_1_HEIGHT							60

#define		FUNERAL_LINK_OFFSET_X							85
#define		FUNERAL_NUMBER_OF_LINKS						5

#define		FUNERAL_LINK_TEXT_OFFSET_X				4
#define		FUNERAL_LINK_TEXT_OFFSET_Y				17
#define		FUNERAL_LINK_TEXT_WIDTH						76


#define		FUNERAL_MARBLE_WIDTH							125
#define		FUNERAL_MARBLE_HEIGHT							100

#define		FUNERAL_SENTENCE_WIDTH						380

#define		FUNERAL_SENTENCE_1_X							LAPTOP_SCREEN_UL_X + 60
#define		FUNERAL_SENTENCE_1_Y							LAPTOP_SCREEN_WEB_UL_Y + 164

#define		FUNERAL_SENTENCE_2_X							FUNERAL_SENTENCE_1_X
#define		FUNERAL_SENTENCE_2_Y							LAPTOP_SCREEN_WEB_UL_Y + 198

#define		FUNERAL_SENTENCE_3_X							FUNERAL_SENTENCE_1_X
#define		FUNERAL_SENTENCE_3_Y							LAPTOP_SCREEN_WEB_UL_Y + 227

#define		FUNERAL_SENTENCE_4_X							FUNERAL_SENTENCE_1_X
#define		FUNERAL_SENTENCE_4_Y							LAPTOP_SCREEN_WEB_UL_Y + 261

#define		FUNERAL_SENTENCE_5_X							FUNERAL_SENTENCE_1_X
#define		FUNERAL_SENTENCE_5_Y							LAPTOP_SCREEN_WEB_UL_Y + 303


#define		FUNERAL_CLOSED_RIP_SIGN_X					LAPTOP_SCREEN_UL_X + 72
#define		FUNERAL_CLOSED_RIP_SIGN_Y					LAPTOP_SCREEN_WEB_UL_Y + 151
#define		FUNERAL_CLOSED_WIDTH							364
#define		FUNERAL_CLOSED_HEIGHT							204

#define		FUNERAL_RIP_SENTENCE_WIDTH				260

#define		FUNERAL_RIP_SENTENCE_1_X					FUNERAL_CLOSED_RIP_SIGN_X + 55
#define		FUNERAL_RIP_SENTENCE_1_Y					FUNERAL_CLOSED_RIP_SIGN_Y + 98

#define		FUNERAL_RIP_SENTENCE_2_X					FUNERAL_RIP_SENTENCE_1_X
#define		FUNERAL_RIP_SENTENCE_2_Y					FUNERAL_CLOSED_RIP_SIGN_Y + 162
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
typedef FuneralParseData;

static void XMLCALL 
FuneralStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	FuneralParseData * pData = (FuneralParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "FuneralData") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strstr(name, "FuneralText") != NULL && pData->curElement == ELEMENT_LIST)
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
FuneralCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	FuneralParseData * pData = (FuneralParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && 
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	  ){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
FuneralEndElementHandle(void *userData, const XML_Char *name)
{	
	char temp;
	FuneralParseData * pData = (FuneralParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "FuneralData") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "FuneralText") == 0)
		{
			pData->curElement = ELEMENT_LIST;
		}
		else if(strcmp(name, "TEXT") == 0)
		{
			//YES, I realize this is not done that well, but give me a break I'm not that great of a coder.
			//Maybe later we can have additional text in the Funeral area, but until then at least it can be changed.
			//Gotthard, 10/18/07
			pData->curElement = ELEMENT;
			if(pData->curIndex < pData->maxArraySize)
			{
				for(int i=0;i<min((int)strlen(pData->szCharData),MAX_CHAR_DATA_LENGTH);i++)
				{
					temp = pData->szCharData[i];
					sFuneralString[pData->curIndex][i] = temp;
				}
			}
		}
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}
BOOLEAN ReadInFuneralText(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	FuneralParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading Funeral.xml" );

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

	
	XML_SetElementHandler(parser, FuneralStartElementHandle, FuneralEndElementHandle);
	XML_SetCharacterDataHandler(parser, FuneralCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	pData.maxArraySize = MAXITEMS; 
	pData.curIndex = -1;
	
	XML_SetUserData(parser, &pData);


    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Funeral.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);

	return( TRUE );
}