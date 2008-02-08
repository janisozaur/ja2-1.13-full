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
        PARSE_STAGE        curElement;

        CHAR8                szCharData[MAX_CHAR_DATA_LENGTH+1];

        UINT32                        maxArraySize;
        UINT32                        curIndex;        
        UINT32                        currentDepth;
        UINT32                        maxReadDepth;
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
                else if(pData->curElement == ELEMENT &&
#ifdef ENGLISH                
                        (strcmp(name, "ENGLISH_TEXT") == 0 ))
#elif RUSSIAN
                        (strcmp(name, "GERMAN_TEXT") == 0 ))
#elif DUTCH
                        (strcmp(name, "RUSSIAN_TEXT") == 0 ))
#elif ITALIAN
                        (strcmp(name, "POLISH_TEXT") == 0 ))
#elif POLISH
                        (strcmp(name, "DUTCH_TEXT") == 0 ))
#elif FRENCH
                        (strcmp(name, "FRENCH_TEXT") == 0 ))
#elif GERMAN
                        (strcmp(name, "ITALIAN_TEXT") == 0 ))
#elif TAIWANESE
                        (strcmp(name, "TAIWANESE_TEXT") == 0 ))
#endif
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
#ifdef ENGLISH                
                else if(strcmp(name, "ENGLISH_TEXT") == 0)
#elif RUSSIAN
                else if(strcmp(name, "RUSSIAN_TEXT") == 0)
#elif DUTCH
                else if(strcmp(name, "DUTCH_TEXT") == 0)
#elif ITALIAN
                else if(strcmp(name, "ITALIAN_TEXT") == 0)
#elif POLISH
                else if(strcmp(name, "POLISH_TEXT") == 0)
#elif FRENCH
                else if(strcmp(name, "FRENCH_TEXT") == 0)
#elif GERMAN
                else if(strcmp(name, "GERMAN_TEXT") == 0)
#elif TAIWANESE
                else if(strcmp(name, "TAIWANESE_TEXT") == 0)
#endif
                {
                        pData->curElement = ELEMENT;
						DebugMsg (TOPIC_JA2,DBG_LEVEL_3,"In Florist");
                        if(pData->curIndex < pData->maxArraySize)
                        {
                                size_t origsize = strlen(pData->szCharData) + 5;
                                if(pData->curIndex < 14)
                                {
                                        mbstowcs(sFloristText[pData->curIndex],pData->szCharData, origsize);
                                }
                                else if(pData->curIndex < 36)
                                {
                                        mbstowcs(sOrderFormText[pData->curIndex - 14] ,pData->szCharData, origsize);
                                }
                                else if(pData->curIndex < 41)
                                {
                                        mbstowcs(sFloristGalleryText[pData->curIndex - 36] ,pData->szCharData, origsize);
                                }
                                else if(pData->curIndex < 43)
                                {
                                        mbstowcs(sFloristCards[pData->curIndex - 41] ,pData->szCharData, origsize);
                                }
                        }
                }
                pData->maxReadDepth--;
        }
        pData->currentDepth--;
}
BOOLEAN ReadInFloristText(STR fileName)
{
        HWFILE                hFile;
        UINT32                uiBytesRead;
        UINT32                uiFSize;
        CHAR8 *                lpcBuffer;
        XML_Parser        parser = XML_ParserCreate(NULL);
        
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