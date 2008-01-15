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
	#include "florist.h"
#endif

extern INT16 iFloristConstants[];
extern INT16 iFloristOrderFormConstants[];
extern INT16 iFloristCardsConstants[];
extern INT16 iFloristGalleryConstants[];


struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];

	UINT32			maxArraySize;
	UINT32			curIndex;	
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef FloristLocationsParseData;

static void XMLCALL 
FloristLocationsStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	FloristLocationsParseData * pData = (FloristLocationsParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "FloristLocations") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strstr(name, "LOCATION") != NULL && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->maxReadDepth++; //we are not skipping this element
			pData->curIndex++;
		}
		else if(pData->curElement == ELEMENT && strcmp(name, "VALUE") == 0 )
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
FloristLocationsCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	FloristLocationsParseData * pData = (FloristLocationsParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && 
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	  ){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
FloristLocationsEndElementHandle(void *userData, const XML_Char *name)
{	
	//char temp;

	FloristLocationsParseData * pData = (FloristLocationsParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "FloristLocations") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "LOCATION") == 0)
		{
			pData->curElement = ELEMENT_LIST;
		}
		else if(strcmp(name, "VALUE") == 0)
		{
			//Hopefully should read in an integer and replace the one found in the Florist.cpp file.
			pData->curElement = ELEMENT;
			if(pData->curIndex < pData->maxArraySize)//Yeah probably not needed, but I have no idea what I am doing.
			{
				if(pData->curIndex < 32)
				{
					iFloristConstants[pData->curIndex] += (INT16) atol(pData->szCharData);
				}
				if(pData->curIndex < 59)
				{
					iFloristGalleryConstants[pData->curIndex - 32] += (INT16) atol(pData->szCharData);
				}
				if(pData->curIndex < 141)
				{
					iFloristOrderFormConstants[pData->curIndex - 59] += (INT16) atol(pData->szCharData);
				}
				if(pData->curIndex > 141)
				{
					iFloristCardsConstants[pData->curIndex - 141] += (INT16) atol(pData->szCharData);
				}
			}
		}
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}
BOOLEAN ReadInFloristLocations(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	FloristLocationsParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading FloristLocations.xml" );

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

	
	XML_SetElementHandler(parser, FloristLocationsStartElementHandle, FloristLocationsEndElementHandle);
	XML_SetCharacterDataHandler(parser, FloristLocationsCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	pData.maxArraySize = MAXITEMS; 
	pData.curIndex = -1;
	
	XML_SetUserData(parser, &pData);


    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in FloristLocations.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);

	return( TRUE );
}