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
	#include "weapons.h"
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

	CHAR8			szCharData[MAX_CHAR_DATA_LENGTH+1];
	POCKETTYPE		curLBEPocket;
//	POCKETTYPE *	curArray;
//	UINT32			maxArraySize;
	
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef lbepocketParseData;

static void XMLCALL 
lbepocketStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	lbepocketParseData * pData = (lbepocketParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "POCKETLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			//memset(pData->curArray,0,sizeof(POCKETTYPE)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "POCKET") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			//memset(&pData->curLBEPocket,0,sizeof(POCKETTYPE));
			pData->curLBEPocket = POCKETTYPE();

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "pIndex") == 0 ||
				strcmp(name, "pName") == 0 ||
				strcmp(name, "pSilhouette") == 0 ||
				strcmp(name, "pType") == 0 ||
				strcmp(name, "pRestriction") == 0 ||
				strcmp(name, "ItemCapacityPerSize.0") == 0 ||
				strcmp(name, "ItemCapacityPerSize.1") == 0 ||
				strcmp(name, "ItemCapacityPerSize.2") == 0 ||
				strcmp(name, "ItemCapacityPerSize.3") == 0 ||
				strcmp(name, "ItemCapacityPerSize.4") == 0 ||
				strcmp(name, "ItemCapacityPerSize.5") == 0 ||
				strcmp(name, "ItemCapacityPerSize.6") == 0 ||
				strcmp(name, "ItemCapacityPerSize.7") == 0 ||
				strcmp(name, "ItemCapacityPerSize.8") == 0 ||
				strcmp(name, "ItemCapacityPerSize.9") == 0 ||
				strcmp(name, "ItemCapacityPerSize.10") == 0 ||
				strcmp(name, "ItemCapacityPerSize.11") == 0 ||
				strcmp(name, "ItemCapacityPerSize.12") == 0 ||
				strcmp(name, "ItemCapacityPerSize.13") == 0 ||
				strcmp(name, "ItemCapacityPerSize.14") == 0 ||
				strcmp(name, "ItemCapacityPerSize.15") == 0 ||
				strcmp(name, "ItemCapacityPerSize.16") == 0 ||
				strcmp(name, "ItemCapacityPerSize.17") == 0 ||
				strcmp(name, "ItemCapacityPerSize.18") == 0 ||
				strcmp(name, "ItemCapacityPerSize.19") == 0 ||
				strcmp(name, "ItemCapacityPerSize.20") == 0 ||
				strcmp(name, "ItemCapacityPerSize.21") == 0 ||
				strcmp(name, "ItemCapacityPerSize.22") == 0 ||
				strcmp(name, "ItemCapacityPerSize.23") == 0 ||
				strcmp(name, "ItemCapacityPerSize.24") == 0 ||
				strcmp(name, "ItemCapacityPerSize.25") == 0 ||
				strcmp(name, "ItemCapacityPerSize.26") == 0 ||
				strcmp(name, "ItemCapacityPerSize.27") == 0 ||
				strcmp(name, "ItemCapacityPerSize.28") == 0 ||
				strcmp(name, "ItemCapacityPerSize.29") == 0 ||
				strcmp(name, "ItemCapacityPerSize.30") == 0 ||
				strcmp(name, "ItemCapacityPerSize.31") == 0 ||
				strcmp(name, "ItemCapacityPerSize.32") == 0 ||
				strcmp(name, "ItemCapacityPerSize.33") == 0 ||
				strcmp(name, "ItemCapacityPerSize.34") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
lbepocketCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	lbepocketParseData * pData = (lbepocketParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && 
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	  ){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	  }
}


static void XMLCALL
lbepocketEndElementHandle(void *userData, const XML_Char *name)
{
	lbepocketParseData * pData = (lbepocketParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "POCKETLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "POCKET") == 0)
		{
			pData->curElement = ELEMENT_LIST;
			LBEPocketType.push_back(pData->curLBEPocket);
		}
		else if(strcmp(name, "pIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.pIndex   = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "pName") == 0)
		{
			pData->curElement = ELEMENT;
			if(MAX_CHAR_DATA_LENGTH >= strlen(pData->szCharData))
				strcpy(pData->curLBEPocket.pName,pData->szCharData);
			else
			{
				strncpy(pData->curLBEPocket.pName,pData->szCharData,MAX_CHAR_DATA_LENGTH);
				pData->curLBEPocket.pName[MAX_CHAR_DATA_LENGTH] = '\0';
			}
		}
		else if(strcmp(name, "pSilhouette") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.pSilhouette  = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "pType") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.pType = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "pRestriction") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.pRestriction = (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.0") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[0] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.1") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[1] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.2") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[2] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.3") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[3] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.4") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[4] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.5") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[5] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.6") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[6] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.7") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[7] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.8") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[8] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.9") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[9] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.10") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[10] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.11") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[11] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.12") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[12] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.13") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[13] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.14") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[14] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.15") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[15] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.16") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[16] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.17") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[17] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.18") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[18] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.19") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[19] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.20") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[20] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.21") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[21] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.22") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[22] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.23") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[23] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.24") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[24] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.25") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[25] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.26") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[26] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.27") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[27] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.28") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[28] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.29") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[29] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.30") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[30] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.31") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[31] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.32") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[32] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.33") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[33] = (UINT8) atol(pData->szCharData);
		}
		else if(strcmp(name, "ItemCapacityPerSize.34") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curLBEPocket.ItemCapacityPerSize[34] = (UINT8) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInLBEPocketStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	lbepocketParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading LBEPocket.xml" );

	// Open loadbearingequipment file
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

	
	XML_SetElementHandler(parser, lbepocketStartElementHandle, lbepocketEndElementHandle);
	XML_SetCharacterDataHandler(parser, lbepocketCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	//pData.curArray = LBEPocketType;
	//pData.maxArraySize = MAXITEMS; 
	
	XML_SetUserData(parser, &pData);


    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Pocket.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}
BOOLEAN WriteLBEPocketEquipmentStats()
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writelbepocketsstats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\Pocket out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );
	
	{
		UINT32 cnt;

		FilePrintf(hFile,"<POCKETLIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{

			FilePrintf(hFile,"\t<POCKET>\r\n");

			FilePrintf(hFile,"\t\t<pIndex>%d</pIndex>\r\n",								cnt );
			FilePrintf(hFile,"\t\t<pSilhouette>%d</pSilhouette>\r\n",								LBEPocketType[cnt].pSilhouette  );
			FilePrintf(hFile,"\t\t<pType>%d</pType>\r\n",								LBEPocketType[cnt].pType   );
			FilePrintf(hFile,"\t\t<pRestriction>%d</pRestriction>\r\n",						LBEPocketType[cnt].pRestriction	);
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.0>%d</ItemCapacityPerSize.0>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[0]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.1>%d</ItemCapacityPerSize.1>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[1]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.2>%d</ItemCapacityPerSize.2>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[2]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.3>%d</ItemCapacityPerSize.3>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[3]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.4>%d</ItemCapacityPerSize.4>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[4]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.5>%d</ItemCapacityPerSize.5>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[5]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.6>%d</ItemCapacityPerSize.6>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[6]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.7>%d</ItemCapacityPerSize.7>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[7]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.8>%d</ItemCapacityPerSize.8>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[8]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.9>%d</ItemCapacityPerSize.9>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[9]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.10>%d</ItemCapacityPerSize.10>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[10]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.11>%d</ItemCapacityPerSize.11>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[11]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.12>%d</ItemCapacityPerSize.12>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[12]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.13>%d</ItemCapacityPerSize.13>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[13]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.14>%d</ItemCapacityPerSize.14>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[14]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.15>%d</ItemCapacityPerSize.15>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[15]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.16>%d</ItemCapacityPerSize.16>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[16]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.17>%d</ItemCapacityPerSize.17>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[17]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.18>%d</ItemCapacityPerSize.18>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[18]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.19>%d</ItemCapacityPerSize.19>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[19]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.20>%d</ItemCapacityPerSize.20>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[20]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.21>%d</ItemCapacityPerSize.21>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[21]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.22>%d</ItemCapacityPerSize.22>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[22]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.23>%d</ItemCapacityPerSize.23>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[23]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.24>%d</ItemCapacityPerSize.24>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[24]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.25>%d</ItemCapacityPerSize.25>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[25]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.26>%d</ItemCapacityPerSize.26>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[26]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.27>%d</ItemCapacityPerSize.27>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[27]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.28>%d</ItemCapacityPerSize.28>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[28]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.29>%d</ItemCapacityPerSize.29>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[29]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.30>%d</ItemCapacityPerSize.30>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[30]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.31>%d</ItemCapacityPerSize.31>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[31]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.32>%d</ItemCapacityPerSize.32>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[32]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.33>%d</ItemCapacityPerSize.33>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[33]   );
			FilePrintf(hFile,"\t\t<ItemCapacityPerSize.34>%d</ItemCapacityPerSize.34>\r\n",								LBEPocketType[cnt].ItemCapacityPerSize[34]   );

			FilePrintf(hFile,"\t</POCKET>\r\n");
		}
		FilePrintf(hFile,"</POCKETLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
