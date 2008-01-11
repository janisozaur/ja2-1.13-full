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
typedef SectorNamesParseData;

static void XMLCALL 
SectorNamesStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	SectorNamesParseData * pData = (SectorNamesParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "SectorNames") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strstr(name, "SectorName") != NULL && pData->curElement == ELEMENT_LIST)
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
SectorNamesCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	SectorNamesParseData * pData = (SectorNamesParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && 
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	  ){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
SectorNamesEndElementHandle(void *userData, const XML_Char *name)
{	
	SectorNamesParseData * pData = (SectorNamesParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "SectorNames") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "SectorName") == 0)
		{
			pData->curElement = ELEMENT_LIST;
		}
		else if(strcmp(name, "TEXT") == 0)
		{

			pData->curElement = ELEMENT;
			if(pData->curIndex < pData->maxArraySize)
			{
				
				size_t origsize = strlen(pData->szCharData) + 1;
				const size_t newsize = MAX_CHAR_DATA_LENGTH;
				size_t convertedChars = 0;
				wchar_t wcstring[newsize];
				//mbstowcs_s(&convertedChars, wcstring, origsize, pData->szCharData, _TRUNCATE);
				//mbstowcs(wcstring, pData->szCharData, origsize);
				mbstowcs(pLandTypeStrings[pData->curIndex],pData->szCharData, origsize);
				//pLandTypeStrings[pData->curIndex] = wcstring;
				//wcsncpy(pLandTypeStrings[pData->curIndex], wcstring,origsize);
				/*
				for(int i=0;i<min((int)strlen(pData->szCharData),MAX_CHAR_DATA_LENGTH);i++)
				{
					temp = pData->szCharData[i];
					pLandTypeStrings[pData->curIndex][i] = temp;
				}
				*/
				//temp2[strlen(pData->szCharData)] = '\0';
				//STR16 buffer = pLandTypeStrings[pData->curIndex];
				//MultiByteToWideChar(1200,0, temp2, -1,buffer, sizeof(buffer)/sizeof(buffer[0]));
				//pLandTypeStrings[pData->curIndex] = buffer;
				//memcpy(pLandTypeStrings[pData->curIndex], &temp2, strlen(pData->szCharData));
			}
		}
		pData->maxReadDepth--;
	}
	pData->currentDepth--;
}
BOOLEAN ReadInSectorNamesText(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	SectorNamesParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading SectorNames.xml" );

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

	
	XML_SetElementHandler(parser, SectorNamesStartElementHandle, SectorNamesEndElementHandle);
	XML_SetCharacterDataHandler(parser, SectorNamesCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	pData.maxArraySize = MAXITEMS; 
	pData.curIndex = -1;
	
	XML_SetUserData(parser, &pData);


    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in SectorNames.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);

	return( TRUE );
}