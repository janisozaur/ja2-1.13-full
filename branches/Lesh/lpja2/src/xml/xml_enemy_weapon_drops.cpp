#ifdef PRECOMPILEDHEADERS
	#include "tactical_all.h"
#else
	#include "types.h"
	#include "debug.h"
	#include "mem_man.h"
	#include "text.h"
	#include "weapons.h"
	#include "sgp_str.h"
	#include "file_man.h"
	#include "debug_control.h"
	#include "expat.h"
	#include "xml.h"
	#include "game_settings.h"
	#include "platform.h"
	#include "enemy_item_drops.h"
#endif

struct
{
	PARSE_STAGE	curElement;

	CHAR8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	WEAPON_DROPS	curWeaponDrop;
	WEAPON_DROPS *	curArray;

	UINT32			maxArraySize;
	UINT32			curIndex;	
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef weaponDropParseData;

static void XMLCALL 
weaponDropStartElementHandle(void *userData, const char *name, const char **atts)
{
	weaponDropParseData * pData = (weaponDropParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "WEAPONDROPLIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(WEAPON_DROPS)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "DROPITEM") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curWeaponDrop,0,sizeof(WEAPON_DROPS));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "uiIndex") == 0 || 
				strcmp(name, "ubWeaponType") == 0 ||
				strcmp(name, "ubEnemyDropRate") == 0 || 
				strcmp(name, "ubMilitiaDropRate") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
weaponDropCharacterDataHandle(void *userData, const char *str, int len)
{
	weaponDropParseData * pData = (weaponDropParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && 
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	  ){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
weaponDropEndElementHandle(void *userData, const char *name)
{	
	weaponDropParseData * pData = (weaponDropParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "WEAPONDROPLIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "DROPITEM") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curWeaponDrop.uiIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curWeaponDrop.uiIndex] = pData->curWeaponDrop; //write the inventory into the table
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeaponDrop.uiIndex   = (UINT32) atoi(pData->szCharData);
		}
		else if(strcmp(name, "ubWeaponType") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeaponDrop.ubWeaponType   = (UINT8) atoi(pData->szCharData);
		}
		else if(strcmp(name, "ubEnemyDropRate") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeaponDrop.ubEnemyDropRate  = (UINT8) atoi(pData->szCharData);
		}
		else if(strcmp(name, "ubMilitiaDropRate") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curWeaponDrop.ubMilitiaDropRate  = (UINT8) atoi(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInEnemyWeaponDropsStats(WEAPON_DROPS *pEnemyWeaponDrops, STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	weaponDropParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading EnemyWeaponDrops.xml" );

	// Open inventory file
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

	
	XML_SetElementHandler(parser, weaponDropStartElementHandle, weaponDropEndElementHandle);
	XML_SetCharacterDataHandler(parser, weaponDropCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	pData.curArray = pEnemyWeaponDrops;
	pData.maxArraySize = MAX_DROP_ITEMS; 
	
	XML_SetUserData(parser, &pData);


    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in EnemyWeaponDrops.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}

BOOLEAN WriteEnemyWeaponDropsStats(WEAPON_DROPS *pEnemyWeaponDrops, STR fileName)
{
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( fileName, FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );
	
	{
		UINT32 cnt;

		FilePrintf(hFile,"<WEAPONDROPLIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{

			FilePrintf(hFile,"\t<DROPITEM>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n", cnt);
			FilePrintf(hFile,"\t\t<ubWeaponType>%d</ubWeaponType>\r\n", pEnemyWeaponDrops[cnt].ubWeaponType);
			FilePrintf(hFile,"\t\t<ubEnemyDropRate>%d</ubEnemyDropRate>\r\n", pEnemyWeaponDrops[cnt].ubEnemyDropRate);
			FilePrintf(hFile,"\t\t<ubMilitiaDropRate>%d</ubMilitiaDropRate>\r\n", pEnemyWeaponDrops[cnt].ubMilitiaDropRate);

			FilePrintf(hFile,"\t</DROPITEM>\r\n");
		}
		FilePrintf(hFile,"</WEAPONDROPLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}