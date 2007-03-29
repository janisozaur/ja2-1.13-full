#ifdef PRECOMPILEDHEADERS
	#include "Tactical All.h"
#else
	#include "sgp.h"
	#include "overhead types.h"
	#include "Sound Control.h"
	#include "Soldier Control.h"
	#include "Overhead.h"
	#include "Event Pump.h"
	#include "Weapons.h"
	#include "Animation Control.h"
	#include "Sys Globals.h"
	#include "Handle UI.h"
	#include "Isometric Utils.h"
	#include "worldman.h"
	#include "math.h"
	#include "points.h"
	#include "ai.h"
	#include "los.h"
	#include "renderworld.h"
	#include "opplist.h"
	#include "Interface.h"
	#include "message.h"
	#include "campaign.h"
	#include "Items.h"
	#include "Weapons.h"
	#include "Text.h"
	#include "Soldier Profile.h"
	#include "tile animation.h"
	#include "Dialogue Control.h"
	#include "SkillCheck.h"
	#include "explosion control.h"
	#include "Quests.h"
	#include "Physics.h"
	#include "random.h"
	#include "Vehicles.h"
	#include "bullets.h"
	#include "Morale.h"
	#include "Meanwhile.h"
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

	INT8		szCharData[MAX_CHAR_DATA_LENGTH+1];
	ComboMergeInfoStruct		curAttachmentComboMerge;
	ComboMergeInfoStruct *	curArray;
	UINT32			maxArraySize;
	
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef attachmentcombomergeParseData;

static void XMLCALL 
attachmentcombomergeStartElementHandle(void *userData, const char *name, const char **atts)
{
	attachmentcombomergeParseData * pData = (attachmentcombomergeParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "ATTACHMENTCOMBOMERGELIST") == 0 && pData->curElement == ELEMENT_NONE)
		{
			pData->curElement = ELEMENT_LIST;

			memset(pData->curArray,0,sizeof(ComboMergeInfoStruct)*pData->maxArraySize);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "ATTACHMENTCOMBOMERGE") == 0 && pData->curElement == ELEMENT_LIST)
		{
			pData->curElement = ELEMENT;

			memset(&pData->curAttachmentComboMerge,0,sizeof(ComboMergeInfoStruct));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == ELEMENT &&
				(strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "usItem") == 0 ||
				strcmp(name, "usAttachment1") == 0 ||
				strcmp(name, "usAttachment2") == 0 ||
				strcmp(name, "usResult") == 0 ))
		{
			pData->curElement = ELEMENT_PROPERTY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
attachmentcombomergeCharacterDataHandle(void *userData, const char *str, int len)
{
	attachmentcombomergeParseData * pData = (attachmentcombomergeParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && 
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	  ){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	  }
}


static void XMLCALL
attachmentcombomergeEndElementHandle(void *userData, const char *name)
{
	attachmentcombomergeParseData * pData = (attachmentcombomergeParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "ATTACHMENTCOMBOMERGELIST") == 0)
		{
			pData->curElement = ELEMENT_NONE;
		}
		else if(strcmp(name, "ATTACHMENTCOMBOMERGE") == 0)
		{
			pData->curElement = ELEMENT_LIST;

			if(pData->curAttachmentComboMerge.uiIndex < pData->maxArraySize)
			{
				pData->curArray[pData->curAttachmentComboMerge.uiIndex] = pData->curAttachmentComboMerge; //write the attachmentcombomerge into the table
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachmentComboMerge.uiIndex   = (UINT32) atol(pData->szCharData);
		}
		else if(strcmp(name, "usItem") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachmentComboMerge.usItem  = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "usAttachment1") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachmentComboMerge.usAttachment[0] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "usAttachment2") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachmentComboMerge.usAttachment[1] = (UINT16) atol(pData->szCharData);
		}
		else if(strcmp(name, "usResult") == 0)
		{
			pData->curElement = ELEMENT;
			pData->curAttachmentComboMerge.usResult   = (UINT16) atol(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInAttachmentComboMergeStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	attachmentcombomergeParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading AttachmentComboMerges.xml" );

	// Open attachmentcombomerge file
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

	
	XML_SetElementHandler(parser, attachmentcombomergeStartElementHandle, attachmentcombomergeEndElementHandle);
	XML_SetCharacterDataHandler(parser, attachmentcombomergeCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	pData.curArray = AttachmentComboMerge;
	pData.maxArraySize = MAXITEMS; 
	
	XML_SetUserData(parser, &pData);


    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in AttachmentComboMerges.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);


	XML_ParserFree(parser);


	return( TRUE );
}
BOOLEAN WriteAttachmentComboMergeStats()
{
	//DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"writeattachmentcombomergestats");
	HWFILE		hFile;

	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\AttachmentComboMerge out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );
	
	{
		UINT32 cnt;

		FilePrintf(hFile,"<ATTACHMENTCOMBOMERGELIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{

			FilePrintf(hFile,"\t<ATTACHMENTCOMBOMERGE>\r\n");

			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",								cnt );
			FilePrintf(hFile,"\t\t<usItem>%d</usItem>\r\n",								AttachmentComboMerge[cnt].usItem   );
			FilePrintf(hFile,"\t\t<usAttachment1>%d</usAttachment1>\r\n",								AttachmentComboMerge[cnt].usAttachment[0]    );
			FilePrintf(hFile,"\t\t<usAttachment2>%d</usAttachment2>\r\n",								AttachmentComboMerge[cnt].usAttachment[1]   );
			FilePrintf(hFile,"\t\t<usResult>%d</usResult>\r\n",								AttachmentComboMerge[cnt].usResult    );

			FilePrintf(hFile,"\t</ATTACHMENTCOMBOMERGE>\r\n");
		}
		FilePrintf(hFile,"</ATTACHMENTCOMBOMERGELIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}	