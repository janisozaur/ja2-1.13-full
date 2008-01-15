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
	#include "funeral.h"
#endif

extern INT16 iFuneralConstants[];

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];

	UINT32			maxArraySize;
	UINT32			curIndex;	
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef FuneralLocationsParseData;

static void XMLCALL 
FuneralLocationsStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	FuneralLocationsParseData * pData = (FuneralLocationsParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "FuneralLocations") == 0 && pData->curElement == ELEMENT_NONE)
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
FuneralLocationsCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	FuneralLocationsParseData * pData = (FuneralLocationsParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && 
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	  ){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
FuneralLocationsEndElementHandle(void *userData, const XML_Char *name)
{	
	//char temp;

	FuneralLocationsParseData * pData = (FuneralLocationsParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "FuneralLocations") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "LOCATION") == 0)
		{
			pData->curElement = ELEMENT_LIST;
		}
		else if(strcmp(name, "VALUE") == 0)
		{
			//Hopefully should read in an integer and replace the one found in the Funeral.cpp file.
			pData->curElement = ELEMENT;
			if(pData->curIndex < pData->maxArraySize)//Yeah probably not needed, but I have no idea what I am doing.
			{
				iFuneralConstants[pData->curIndex] += (INT16) atol(pData->szCharData);//Theoretically should copy a UINT into the extern array, but we'll see...
			}
		}
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}
BOOLEAN ReadInFuneralLocations(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	FuneralLocationsParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading FuneralLocations.xml" );

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

	
	XML_SetElementHandler(parser, FuneralLocationsStartElementHandle, FuneralLocationsEndElementHandle);
	XML_SetCharacterDataHandler(parser, FuneralLocationsCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	pData.maxArraySize = MAXITEMS; 
	pData.curIndex = -1;
	
	XML_SetUserData(parser, &pData);


    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in FuneralLocations.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);

	return( TRUE );
}