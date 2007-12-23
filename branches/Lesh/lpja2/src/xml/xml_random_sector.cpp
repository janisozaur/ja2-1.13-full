#include "types.h"
#include "campaign_init.h"
#include "campaign_types.h"
#include "platform.h"
#include "xml.h"
#include "file_man.h"
#include "mem_man.h"
#include "debug_control.h"


typedef enum
{
	ALTSECTORS_ELEMENT_NONE = 0,
	ALTSECTORS_ELEMENT_LIST,
	ALTSECTORS_ELEMENT_ROW,
} ALTSECTORS_PARSE_STAGE;

typedef struct
{
	ALTSECTORS_PARSE_STAGE	curElement;
	CHAR8					szCharData[MAX_CHAR_DATA_LENGTH+1];
	UINT32					uiRowNumber;
	
	UINT32					currentDepth;
	UINT32					maxReadDepth;
} altSectorsParseData;

static void XMLCALL
altSectorsStartElementHandle(void *userData, const char *name, const char **atts)
{
	altSectorsParseData * pData = (altSectorsParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "ALT_SECTORS_LIST") == 0 && pData->curElement == ALTSECTORS_ELEMENT_NONE)
		{
			pData->curElement = ALTSECTORS_ELEMENT_LIST;
			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "ROW") == 0 && pData->curElement == ALTSECTORS_ELEMENT_LIST)
		{
			pData->curElement = ALTSECTORS_ELEMENT_ROW;

			if(strcmp(atts[0], "y") == 0)
			{
				if ( isalpha(atts[1][0]) )
					pData->uiRowNumber = (UINT32)(atts[1][0] - 0x40);
				else
					pData->uiRowNumber = atoi(atts[1]);
			}
			pData->maxReadDepth++; //we are not skipping this element
		}
		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;
}

static void XMLCALL
altSectorsCharacterDataHandle(void *userData, const char *str, int len)
{
	altSectorsParseData * pData = (altSectorsParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth && strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
}


static void XMLCALL
altSectorsEndElementHandle(void *userData, const char *name)
{
	altSectorsParseData * pData = (altSectorsParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "ALT_SECTORS_LIST") == 0 && pData->curElement == ALTSECTORS_ELEMENT_LIST)
		{
			pData->curElement = ALTSECTORS_ELEMENT_NONE;
		}
		else if(strcmp(name, "ROW") == 0 && pData->curElement == ALTSECTORS_ELEMENT_ROW)
		{
			CHAR8 * curBuffer = pData->szCharData + strspn((const char *)pData->szCharData," \t\n\r");
			UINT32 curCellIndex = 0;
			UINT32 curNumber;

			pData->curElement = ALTSECTORS_ELEMENT_LIST;

			while(curBuffer[0] != '\0')
			{
				sscanf( (const char *)curBuffer,"%d",&curNumber);

				RandomSector[curCellIndex+16*(pData->uiRowNumber - 1)] = curNumber;

				curCellIndex++;
				curBuffer += strcspn(curBuffer," \t\n\r\0");
				curBuffer += strspn((const char *)curBuffer," \t\n\r");
			}
		}
		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}


BOOLEAN ReadInAltSectors(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	altSectorsParseData pData;

	// Open xml file
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

	
	XML_SetElementHandler(parser, altSectorsStartElementHandle, altSectorsEndElementHandle);
	XML_SetCharacterDataHandler(parser, altSectorsCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);


    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in AltSectors.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);

	return( TRUE );
}

BOOLEAN WriteInAltSectors(STR fileName)
{
	// Lets output the current Strategic map format using the XML structure I've devised.
	FILE *outfile = fopen(fileName, "wt");

	UINT32 x, y;

	fprintf (outfile, "<ALT_SECTORS_LIST>\n");
	for(y = 1;y <= 16;y++)
	{
		fprintf (outfile, "\t<ROW y=\"%c\">",(y+0x40));
		for(x = 1;x <= 16; x++)
		{
			fprintf (outfile, " %d",RandomSector[ ((y - 1) * 16) + (x - 1) ] );
		}
		fprintf (outfile, " </ROW>\n");
	}
	fprintf (outfile, "</ALT_SECTORS_LIST>\n");

	fclose(outfile);
		
	return (TRUE);
}
