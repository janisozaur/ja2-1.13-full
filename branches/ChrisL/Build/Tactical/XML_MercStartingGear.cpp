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

	CHAR8				szCharData[MAX_CHAR_DATA_LENGTH+1];
	MERCPROFILEGEAR		curMercStartingGear;
	MERCPROFILEGEAR *	curArray;
	UINT32				maxArraySize;
	
	UINT32				currentDepth;
	UINT32				maxReadDepth;
}
typedef MercStartingGearParseData;

static void XMLCALL 
MercStartingGearStartElementHandle(void *userData, const XML_Char *name, const XML_Char **atts)
{
	MercStartingGearParseData * pData = (MercStartingGearParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "MERCGEARLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			//pData->curArray = &(MERCPROFILEGEAR());
			//memset(pData->curArray,0,sizeof(MERCPROFILEGEAR)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "MERCGEAR") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			pData->curMercStartingGear = MERCPROFILEGEAR();
			//memset(&pData->curMercStartingGear,0,sizeof(MERCPROFILEGEAR));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "mIndex") == 0 ||
				strcmp(name, "mName") == 0 ||
				strcmp(name, "mHelmet") == 0 ||
				strcmp(name, "mHelmetStatus") == 0 ||
				strcmp(name, "mHelmetDrop") == 0 ||
				strcmp(name, "mVest") == 0 ||
				strcmp(name, "mVestStatus") == 0 ||
				strcmp(name, "mVestDrop") == 0 ||
				strcmp(name, "mLeg") == 0 ||
				strcmp(name, "mLegStatus") == 0 ||
				strcmp(name, "mLegDrop") == 0 ||
				strcmp(name, "mWeapon") == 0 ||
				strcmp(name, "mWeaponStatus") == 0 ||
				strcmp(name, "mWeaponDrop") == 0 ||
				strcmp(name, "mBig0") == 0 ||
				strcmp(name, "mBig0Quantity") == 0 ||
				strcmp(name, "mBig0Status") == 0 ||
				strcmp(name, "mBig0Drop") == 0 ||
				strcmp(name, "mBig1") == 0 ||
				strcmp(name, "mBig1Quantity") == 0 ||
				strcmp(name, "mBig1Status") == 0 ||
				strcmp(name, "mBig1Drop") == 0 ||
				strcmp(name, "mBig2") == 0 ||
				strcmp(name, "mBig2Quantity") == 0 ||
				strcmp(name, "mBig2Status") == 0 ||
				strcmp(name, "mBig2Drop") == 0 ||
				strcmp(name, "mBig3") == 0 ||
				strcmp(name, "mBig3Quantity") == 0 ||
				strcmp(name, "mBig3Status") == 0 ||
				strcmp(name, "mBig3Drop") == 0 ||
				strcmp(name, "mSmall0") == 0 ||
				strcmp(name, "mSmall0Quantity") == 0 ||
				strcmp(name, "mSmall0Status") == 0 ||
				strcmp(name, "mSmall1") == 0 ||
				strcmp(name, "mSmall1Quantity") == 0 ||
				strcmp(name, "mSmall1Status") == 0 ||
				strcmp(name, "mSmall2") == 0 ||
				strcmp(name, "mSmall2Quantity") == 0 ||
				strcmp(name, "mSmall2Status") == 0 ||
				strcmp(name, "mSmall3") == 0 ||
				strcmp(name, "mSmall3Quantity") == 0 ||
				strcmp(name, "mSmall3Status") == 0 ||
				strcmp(name, "mSmall4") == 0 ||
				strcmp(name, "mSmall4Quantity") == 0 ||
				strcmp(name, "mSmall4Status") == 0 ||
				strcmp(name, "mSmall5") == 0 ||
				strcmp(name, "mSmall5Quantity") == 0 ||
				strcmp(name, "mSmall5Status") == 0 ||
				strcmp(name, "mSmall6") == 0 ||
				strcmp(name, "mSmall6Quantity") == 0 ||
				strcmp(name, "mSmall6Status") == 0 ||
				strcmp(name, "mSmall7") == 0 ||
				strcmp(name, "mSmall7Quantity") == 0 ||
				strcmp(name, "mSmall7Status") == 0 ||
				strcmp(name, "lVest") == 0 ||
				strcmp(name, "lVestStatus") == 0 ||
				strcmp(name, "lLeftThigh") == 0 ||
				strcmp(name, "lLeftThighStatus") == 0 ||
				strcmp(name, "lRightThigh") == 0 ||
				strcmp(name, "lRightThighStatus") == 0 ||
				strcmp(name, "lCPack") == 0 ||
				strcmp(name, "lCPackStatus") == 0 ||
				strcmp(name, "lBPack") == 0 ||
				strcmp(name, "lBPackStatus") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
MercStartingGearCharacterDataHandle(void *userData, const XML_Char *str, int len)
{
	MercStartingGearParseData * pData = (MercStartingGearParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && 
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	  ){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	  }
}


static void XMLCALL
MercStartingGearEndElementHandle(void *userData, const XML_Char *name)
{
	MercStartingGearParseData * pData = (MercStartingGearParseData *)userData;
	char temp;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "MERCGEARLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "MERCGEAR") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curMercStartingGear.mIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curMercStartingGear.mIndex] = pData->curMercStartingGear; //write the armour into the table
			}
		}
		else if(strcmp(name, "mIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.mIndex   = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "mName") == 0)
		{
			pData->curElement = ELEMENT;
			if(MAX_CHAR_DATA_LENGTH >= strlen(pData->szCharData))
				strcpy(pData->curMercStartingGear.mName,pData->szCharData);
			else
			{
				strncpy(pData->curMercStartingGear.mName,pData->szCharData,MAX_CHAR_DATA_LENGTH);
				pData->curMercStartingGear.mName[MAX_CHAR_DATA_LENGTH] = '\0';
			}
			for(int i=0;i<min((int)strlen(pData->szCharData),MAX_CHAR_DATA_LENGTH);i++)
			{
				temp = pData->szCharData[i];
				pData->curMercStartingGear.mName[i] = temp;
				//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,String("itemEndElementHandle: itemname[%d] = %s, temp = %s",i,&pData->curItem.szItemName[i],&temp));
			}
		}
		else if(strcmp(name, "mHelmet") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.inv[0]  = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mVest") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.inv[1] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mLeg") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.inv[2] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mWeapon") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.inv[5] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig0") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.inv[7] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig1") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.inv[8] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig2") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.inv[9] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig3") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.inv[10] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall0") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.inv[11] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall1") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.inv[12] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall2") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.inv[13] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall3") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.inv[14] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall4") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.inv[15] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall5") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.inv[16] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall6") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.inv[17] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall7") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.inv[18] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mHelmetStatus") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iStatus[0] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mVestStatus") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iStatus[1] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mLegStatus") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iStatus[2] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mWeaponStatus") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iStatus[5] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig0Status") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iStatus[7] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig1Status") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iStatus[8] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig2Status") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iStatus[9] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig3Status") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iStatus[10] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall0Status") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iStatus[11] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall1Status") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iStatus[12] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall2Status") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iStatus[13] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall3Status") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iStatus[14] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall4Status") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iStatus[15] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall5Status") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iStatus[16] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall6Status") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iStatus[17] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall7Status") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iStatus[18] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mHelmetDrop") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iDrop[0] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mVestDrop") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iDrop[1] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mLegDrop") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iDrop[2] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mWeaponDrop") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iDrop[5] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig0Drop") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iDrop[7] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig1Drop") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iDrop[8] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig2Drop") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iDrop[9] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig3Drop") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iDrop[10] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig0Quantity") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iNumber[7] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig1Quantity") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iNumber[8] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig2Quantity") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iNumber[9] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mBig3Quantity") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iNumber[10] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall0Quantity") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iNumber[11] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall1Quantity") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iNumber[12] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall2Quantity") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iNumber[13] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall3Quantity") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iNumber[14] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall4Quantity") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iNumber[15] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall5Quantity") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iNumber[16] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall6Quantity") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iNumber[17] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "mSmall7Quantity") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.iNumber[18] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "lVest") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.lbe[0] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "lLeftThigh") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.lbe[1] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "lRightThigh") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.lbe[2] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "lCPack") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.lbe[3] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "lBPack") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.lbe[4] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "lVestStatus") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.lStatus[0] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "lLeftThighStatus") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.lStatus[1] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "lRightThighStatus") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.lStatus[2] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "lCPackStatus") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.lStatus[3] = (int) atol(pData->szCharData);
		}
		else if(strcmp(name, "lBPackStatus") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curMercStartingGear.lStatus[4] = (int) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInMercStartingGearStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	MercStartingGearParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading MercStartingGear.xml" );

	// Open gMercProfileGear file
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

	
	XML_SetElementHandler(parser, MercStartingGearStartElementHandle, MercStartingGearEndElementHandle);
	XML_SetCharacterDataHandler(parser, MercStartingGearCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	pData.curArray = gMercProfileGear;
	pData.maxArraySize = NUM_PROFILES; 
	
	XML_SetUserData(parser, &pData);


    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in MercStartingGear.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}
BOOLEAN WriteMercStartingGearStats()
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writemercstartinggearstats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\MercStartingGear out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );
	
	{
		UINT32 cnt;
		CHAR16 str[100];

		FilePrintf(hFile,"<MERCGEARLIST>\r\n");
		for(cnt = 0;cnt < NUM_PROFILES;cnt++)
		{

			FilePrintf(hFile,"\t<MERCGEAR>\r\n");

			FilePrintf(hFile,"\t\t<mIndex>%d</mIndex>\r\n",								cnt );
//			FilePrintf(hFile,"\t\t<mName>%d</mName>\r\n",								gMercProfileGear[cnt].mName  );

			
			int j = -1;
			for (int i=0;i<80;i++)
			{
				j++;
				if ( i<(int)strlen(gMercProfileGear[cnt].mName))
				{
					str[i] = gMercProfileGear[cnt].mName [j];
				}
				else
				{
					str[i] ='\0';
				}
			}
			STR16 szRemainder = str;
			FilePrintf(hFile,"\t\t<mName>");
			while(szRemainder[0] != '\0')
			{
				UINT32 uiCharLoc = wcscspn(szRemainder, L"&<>\'\"\0");
				CHAR16 invChar = szRemainder[uiCharLoc];
				
				if(uiCharLoc)
				{
					szRemainder[uiCharLoc] = '\0';
					FilePrintf(hFile,"%S",szRemainder);
					szRemainder[uiCharLoc] = invChar;
				}

				szRemainder += uiCharLoc;

				switch(invChar)
				{
					case '&':
						FilePrintf(hFile,"&amp;");
						szRemainder++;
					break;

					case '<':
						FilePrintf(hFile,"&lt;");
						szRemainder++;
					break;

					case '>':
						FilePrintf(hFile,"&gt;");
						szRemainder++;
					break;

					case '\'':
						FilePrintf(hFile,"&apos;");
						szRemainder++;
					break;

					case '\"':
						FilePrintf(hFile,"&quot;");
						szRemainder++;
					break;
				}
			}
			FilePrintf(hFile,"</mName>\r\n");
			FilePrintf(hFile,"\t\t<mHelmet>%d</mHelmet>\r\n",						gMercProfileGear[cnt].inv[0]  );
			FilePrintf(hFile,"\t\t<mVest>%d</mVest>\r\n",							gMercProfileGear[cnt].inv[1]   );
			FilePrintf(hFile,"\t\t<mLeg>%d</mLeg>\r\n",								gMercProfileGear[cnt].inv[2]   );
			FilePrintf(hFile,"\t\t<mWeapon>%d</mWeapon>\r\n",						gMercProfileGear[cnt].inv[5]   );
			FilePrintf(hFile,"\t\t<mHelmetStatus>%d</mHelmetStatus>\r\n",			gMercProfileGear[cnt].iStatus[0]   );
			FilePrintf(hFile,"\t\t<mVestStatus>%d</mVestStatus>\r\n",				gMercProfileGear[cnt].iStatus[1]   );
			FilePrintf(hFile,"\t\t<mLegStatus>%d</mLegStatus>\r\n",					gMercProfileGear[cnt].iStatus[2]   );
			FilePrintf(hFile,"\t\t<mWeaponStatus>%d</mWeaponStatus>\r\n",			gMercProfileGear[cnt].iStatus[5]   );
			FilePrintf(hFile,"\t\t<mHelmetDrop>%d</mHelmetDrop>\r\n",				gMercProfileGear[cnt].iDrop[0]   );
			FilePrintf(hFile,"\t\t<mVestDrop>%d</mVestDrop>\r\n",					gMercProfileGear[cnt].iDrop[1]   );
			FilePrintf(hFile,"\t\t<mLegDrop>%d</mLegDrop>\r\n",						gMercProfileGear[cnt].iDrop[2]   );
			FilePrintf(hFile,"\t\t<mWeaponDrop>%d</mWeaponDrop>\r\n",				gMercProfileGear[cnt].iDrop[5]   );
			FilePrintf(hFile,"\t\t<mBig0>%d</mBig0>\r\n",							gMercProfileGear[cnt].inv[7]   );
			FilePrintf(hFile,"\t\t<mBig1>%d</mBig1>\r\n",							gMercProfileGear[cnt].inv[8]   );
			FilePrintf(hFile,"\t\t<mBig2>%d</mBig2>\r\n",							gMercProfileGear[cnt].inv[9]   );
			FilePrintf(hFile,"\t\t<mBig3>%d</mBig3>\r\n",							gMercProfileGear[cnt].inv[10]   );
			FilePrintf(hFile,"\t\t<mBig0Quantity>%d</mBig0Quantity>\r\n",			gMercProfileGear[cnt].iNumber[7]   );
			FilePrintf(hFile,"\t\t<mBig1Quantity>%d</mBig1Quantity>\r\n",			gMercProfileGear[cnt].iNumber[8]   );
			FilePrintf(hFile,"\t\t<mBig2Quantity>%d</mBig2Quantity>\r\n",			gMercProfileGear[cnt].iNumber[9]   );
			FilePrintf(hFile,"\t\t<mBig3Quantity>%d</mBig3Quantity>\r\n",			gMercProfileGear[cnt].iNumber[10]   );
			FilePrintf(hFile,"\t\t<mBig0Status>%d</mBig0Status>\r\n",				gMercProfileGear[cnt].iStatus[7]   );
			FilePrintf(hFile,"\t\t<mBig1Status>%d</mBig1Status>\r\n",				gMercProfileGear[cnt].iStatus[8]   );
			FilePrintf(hFile,"\t\t<mBig2Status>%d</mBig2Status>\r\n",				gMercProfileGear[cnt].iStatus[9]   );
			FilePrintf(hFile,"\t\t<mBig3Status>%d</mBig3Status>\r\n",				gMercProfileGear[cnt].iStatus[10]   );
			FilePrintf(hFile,"\t\t<mBig0Drop>%d</mBig0Drop>\r\n",					gMercProfileGear[cnt].iDrop[7]   );
			FilePrintf(hFile,"\t\t<mBig1Drop>%d</mBig1Drop>\r\n",					gMercProfileGear[cnt].iDrop[8]   );
			FilePrintf(hFile,"\t\t<mBig2Drop>%d</mBig2Drop>\r\n",					gMercProfileGear[cnt].iDrop[9]   );
			FilePrintf(hFile,"\t\t<mBig3Drop>%d</mBig3Drop>\r\n",					gMercProfileGear[cnt].iDrop[10]   );
			FilePrintf(hFile,"\t\t<mSmall0>%d</mSmall0>\r\n",						gMercProfileGear[cnt].inv[11]   );
			FilePrintf(hFile,"\t\t<mSmall1>%d</mSmall1>\r\n",						gMercProfileGear[cnt].inv[12]   );
			FilePrintf(hFile,"\t\t<mSmall2>%d</mSmall2>\r\n",						gMercProfileGear[cnt].inv[13]   );
			FilePrintf(hFile,"\t\t<mSmall3>%d</mSmall3>\r\n",						gMercProfileGear[cnt].inv[14]   );
			FilePrintf(hFile,"\t\t<mSmall4>%d</mSmall4>\r\n",						gMercProfileGear[cnt].inv[15]   );
			FilePrintf(hFile,"\t\t<mSmall5>%d</mSmall5>\r\n",						gMercProfileGear[cnt].inv[16]   );
			FilePrintf(hFile,"\t\t<mSmall6>%d</mSmall6>\r\n",						gMercProfileGear[cnt].inv[17]   );
			FilePrintf(hFile,"\t\t<mSmall7>%d</mSmall7>\r\n",						gMercProfileGear[cnt].inv[18]   );
			FilePrintf(hFile,"\t\t<mSmall0Quantity>%d</mSmall0Quantity>\r\n",		gMercProfileGear[cnt].iNumber[11]   );
			FilePrintf(hFile,"\t\t<mSmall1Quantity>%d</mSmall1Quantity>\r\n",		gMercProfileGear[cnt].iNumber[12]   );
			FilePrintf(hFile,"\t\t<mSmall2Quantity>%d</mSmall2Quantity>\r\n",		gMercProfileGear[cnt].iNumber[13]   );
			FilePrintf(hFile,"\t\t<mSmall3Quantity>%d</mSmall3Quantity>\r\n",		gMercProfileGear[cnt].iNumber[14]   );
			FilePrintf(hFile,"\t\t<mSmall4Quantity>%d</mSmall4Quantity>\r\n",		gMercProfileGear[cnt].iNumber[15]   );
			FilePrintf(hFile,"\t\t<mSmall5Quantity>%d</mSmall5Quantity>\r\n",		gMercProfileGear[cnt].iNumber[16]   );
			FilePrintf(hFile,"\t\t<mSmall6Quantity>%d</mSmall6Quantity>\r\n",		gMercProfileGear[cnt].iNumber[17]   );
			FilePrintf(hFile,"\t\t<mSmall7Quantity>%d</mSmall7Quantity>\r\n",		gMercProfileGear[cnt].iNumber[18]   );
			FilePrintf(hFile,"\t\t<mSmall0Status>%d</mSmall0Status>\r\n",			gMercProfileGear[cnt].iStatus[11]   );
			FilePrintf(hFile,"\t\t<mSmall1Status>%d</mSmall1Status>\r\n",			gMercProfileGear[cnt].iStatus[12]   );
			FilePrintf(hFile,"\t\t<mSmall2Status>%d</mSmall2Status>\r\n",			gMercProfileGear[cnt].iStatus[13]   );
			FilePrintf(hFile,"\t\t<mSmall3Status>%d</mSmall3Status>\r\n",			gMercProfileGear[cnt].iStatus[14]   );
			FilePrintf(hFile,"\t\t<mSmall4Status>%d</mSmall4Status>\r\n",			gMercProfileGear[cnt].iStatus[15]   );
			FilePrintf(hFile,"\t\t<mSmall5Status>%d</mSmall5Status>\r\n",			gMercProfileGear[cnt].iStatus[16]   );
			FilePrintf(hFile,"\t\t<mSmall6Status>%d</mSmall6Status>\r\n",			gMercProfileGear[cnt].iStatus[17]   );
			FilePrintf(hFile,"\t\t<mSmall7Status>%d</mSmall7Status>\r\n",			gMercProfileGear[cnt].iStatus[18]   );
			FilePrintf(hFile,"\t\t<lVest>%d</lVest>\r\n",							gMercProfileGear[cnt].lbe[0]   );
			FilePrintf(hFile,"\t\t<lLeftThigh>%d</lLeftThigh>\r\n",					gMercProfileGear[cnt].lbe[1]   );
			FilePrintf(hFile,"\t\t<lRightThigh>%d</lRightThigh>\r\n",				gMercProfileGear[cnt].lbe[2]   );
			FilePrintf(hFile,"\t\t<lCPack>%d</lCPack>\r\n",							gMercProfileGear[cnt].lbe[3]   );
			FilePrintf(hFile,"\t\t<lBPack>%d</lBPack>\r\n",							gMercProfileGear[cnt].lbe[4]   );
			FilePrintf(hFile,"\t\t<lVestStatus>%d</lVestStatus>\r\n",				gMercProfileGear[cnt].lStatus[0]   );
			FilePrintf(hFile,"\t\t<lLeftThighStatus>%d</lLeftThighStatus>\r\n",		gMercProfileGear[cnt].lStatus[1]   );
			FilePrintf(hFile,"\t\t<lRightThighStatus>%d</lRightThighStatus>\r\n",	gMercProfileGear[cnt].lStatus[2]   );
			FilePrintf(hFile,"\t\t<lCPackStatus>%d</lCPackStatus>\r\n",				gMercProfileGear[cnt].lStatus[3]   );
			FilePrintf(hFile,"\t\t<lBPackStatus>%d</lBPackStatus>\r\n",				gMercProfileGear[cnt].lStatus[4]   );

			FilePrintf(hFile,"\t</MERCGEAR>\r\n");
		}
		FilePrintf(hFile,"</MERCGEARLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
