#include "xml.h"
#include "debug_control.h"
#include "debug.h"
#include "file_man.h"
#include "campaign_types.h"
#include "mem_man.h"
#include "strategic_movement.h"

typedef enum
{
	SMCTABLE_ELEMENT_NONE = 0,
	SMCTABLE_ELEMENT_STRATEGICMOVEMENTCOSTS,
	SMCTABLE_ELEMENT_SECTORTRAVERSAL,
	SMCTABLE_ELEMENT_SECTOR,
	SMCTABLE_ELEMENT_TRAVELRATING,
	SMCTABLE_ELEMENT_TRAVELTYPE,
	SMCTABLE_ELEMENT_SHORTNAME,
} SMCTABLE_PARSE_STAGE;

typedef struct
{
	SMCTABLE_PARSE_STAGE	curElement;
	CHAR8					szCharData[MAX_CHAR_DATA_LENGTH+1];
	UINT32					currentDepth;
	UINT32					maxReadDepth;

	//INT8					szCustomShortname[MAX_SECTORNAME_LENGTH+1];
	UINT32					uiRowNumber;
	UINT32                  uiColNumber;
	UINT32					travRating;
	UINT32					travNorth;
	UINT32					travEast;
	UINT32					travSouth;
	UINT32					travWest;
	UINT32					travHere;
	
} smctableParseData;

/** Process the opening tag in this expat callback.
 */
static void XMLCALL
smctableStartElementHandle(void *userData, const char *name, const char **atts)
{
	smctableParseData * pData = (smctableParseData *) userData;
	//FILE *outfile = fopen("smc.log", "at");

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "StrategicMovementCosts") == 0 && pData->curElement == SMCTABLE_ELEMENT_NONE)
		{
			// Enter 
			pData->curElement = SMCTABLE_ELEMENT_STRATEGICMOVEMENTCOSTS;
			pData->maxReadDepth++; //we are not skipping this element
			//fprintf (outfile, "Entering Strategic Movement Costs\n");
		}
		else if(strcmp(name, "SectorTraversal") == 0 && pData->curElement == SMCTABLE_ELEMENT_STRATEGICMOVEMENTCOSTS)
		{
			pData->curElement = SMCTABLE_ELEMENT_SECTORTRAVERSAL;
			pData->maxReadDepth++; //we are not skipping this element
			//fprintf (outfile, "Exiting Sector Traversal\n");
		}
		else if(strcmp(name, "Sector") == 0 && pData->curElement == SMCTABLE_ELEMENT_SECTORTRAVERSAL)
		{
			UINT32 uiAttrIndex;
			pData->curElement = SMCTABLE_ELEMENT_SECTOR;
			// Extract the x and y attributes to use as the sector co-ordinate
			for(uiAttrIndex = 0;atts[uiAttrIndex] != NULL;uiAttrIndex += 2)
			{
				if(strcmp(atts[uiAttrIndex], "y") == 0)
				{
					if ( isalpha(atts[uiAttrIndex+1][0]) )
					{
						pData->uiRowNumber = (UINT32)(atts[uiAttrIndex+1][0] & 0x1F);
					}
					else
					{
						pData->uiRowNumber = atol(atts[uiAttrIndex+1]);
					}
				}
				else if(strcmp(atts[uiAttrIndex], "x") == 0)
				{
					pData->uiColNumber = atol(atts[uiAttrIndex+1]);
				}
			}
			//fprintf(outfile, "Extracted sector co-ordinates: [x=%d, y=%d]\n", pData->uiColNumber, pData->uiRowNumber);
			// A simple test to try and stop bogus entries
			if(pData->uiRowNumber <= 16 && pData->uiColNumber <= 16)
			{
				pData->maxReadDepth++; //we are not skipping this element
				//fprintf (outfile, "Entering Sector\n");
			}
		}
		else if(strcmp(name, "TravelRating") == 0 && pData->curElement == SMCTABLE_ELEMENT_SECTOR)
		{
			pData->curElement = SMCTABLE_ELEMENT_TRAVELRATING;
			pData->maxReadDepth++; //we are not skipping this element
			//fprintf (outfile, "Entering Traversal Rating\n");
		}
		else if((strcmp(name, "North") == 0 || strcmp(name, "East") == 0 || strcmp(name, "South") == 0 || strcmp(name, "West") == 0 || strcmp(name, "Here") == 0)
				&& pData->curElement == SMCTABLE_ELEMENT_SECTOR)
		{
			pData->curElement = SMCTABLE_ELEMENT_TRAVELTYPE;
			pData->maxReadDepth++; //we are not skipping this element
			//fprintf (outfile, "Entering Traversal Type\n");
		}
		else if(strcmp(name, "Shortname") == 0 && pData->curElement == SMCTABLE_ELEMENT_SECTOR)
		{
			pData->curElement = SMCTABLE_ELEMENT_SHORTNAME;
			pData->maxReadDepth++; //we are not skipping this element
			//fprintf (outfile, "Entering Traversal Rating\n");
		}
		pData->szCharData[0] = '\0';
	}
	pData->currentDepth++;
	//fclose(outfile);
}

/** Process any text content in this callback.
 */
static void XMLCALL
smctableCharacterDataHandle(void *userData, const char *str, int len)
{
	smctableParseData * pData = (smctableParseData *) userData;

	if(pData->currentDepth <= pData->maxReadDepth && strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
}

/** Process the closing tag in this expat callback.
 */
static void XMLCALL
smctableEndElementHandle(void *userData, const char *name)
{
	smctableParseData * pData = (smctableParseData *) userData;

	//FILE *outfile = fopen("C:/JA2/debug.log", "a");
	//fprintf (outfile, "TOWN INDEX: %d\n", TOWN);
	//fprintf (outfile, "Encountered Close Tag: %s\n", name);

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "StrategicMovementCosts") == 0 && pData->curElement == SMCTABLE_ELEMENT_STRATEGICMOVEMENTCOSTS)
		{
			//fprintf (outfile, "Exiting Strategic Movement Costs\n");
			pData->curElement = SMCTABLE_ELEMENT_NONE;
		}
		else if(strcmp(name, "SectorTraversal") == 0 && pData->curElement == SMCTABLE_ELEMENT_SECTORTRAVERSAL)
		{
			//fprintf (outfile, "Exiting Sector Traversal\n");
			pData->curElement = SMCTABLE_ELEMENT_STRATEGICMOVEMENTCOSTS;
		}
		else if(strcmp(name, "Sector") == 0 && pData->curElement == SMCTABLE_ELEMENT_SECTOR)
		{
			// We've read in a complete section. Process the contents of pData into the
			// SectionInfo array
			MOVEMENT_INFO *pSector;
			//UINT32 sec_index = SECTOR(pData->uiColNumber, pData->uiRowNumber);
			pSector = &MovementInfo[  SECTOR(pData->uiColNumber, pData->uiRowNumber)  ];
			pSector->travRating = pData->travRating;
			pSector->travNorth  = pData->travNorth;
			pSector->travEast   = pData->travEast;
			pSector->travSouth  = pData->travSouth;
			pSector->travWest   = pData->travWest;
			pSector->travHere   = pData->travHere;
			//strncpy(pSector->szCustomShortname, pData->szCustomShortname, MAX_SECTORNAME_LENGTH);
			//fprintf (outfile, "Added SectorInfo #%d: y %d, x %d, name: %s, tr %d, n %d, e %d, s %d, w %d, h %d\n", 
			//	sec_index, pData->uiRowNumber, pData->uiColNumber, pData->szCustomShortname,
			//	pSector->ubTravelRating, pSector->ubTraversability[ NORTH_STRATEGIC_MOVE ], 
			//	pSector->ubTraversability[ EAST_STRATEGIC_MOVE ], 
			//	pSector->ubTraversability[ SOUTH_STRATEGIC_MOVE ], 
			//	pSector->ubTraversability[ WEST_STRATEGIC_MOVE ], 
			//	pSector->ubTraversability[ THROUGH_STRATEGIC_MOVE ]);

			// And finish the sector.
			//fprintf (outfile, "Exiting Sector\n");
			//pData->szCustomShortname[0] = '\0';
			pData->curElement = SMCTABLE_ELEMENT_SECTORTRAVERSAL;
		}
		else if(strcmp(name, "TravelRating") == 0 && pData->curElement == SMCTABLE_ELEMENT_TRAVELRATING)
		{
			// Convert the travel rating to an integer
			pData->travRating = atol(pData->szCharData);
			// Done, done, and I'm on to the next one.
			//fprintf (outfile, "Extracted traversal rating: %d\n", pData->travRating);
			//fprintf (outfile, "Exiting Traversal Rating\n");
			pData->curElement = SMCTABLE_ELEMENT_SECTOR;
		}
		else if(pData->curElement == SMCTABLE_ELEMENT_TRAVELTYPE)
		{
			// Determine the traversal type from the string in pData
			UINT32 trav_type;
			//fprintf (outfile, "Extracted terrain type: %s\n", pData->szCharData);
			if(strcmp(pData->szCharData, "TOWN") == 0)
			{
				trav_type = TOWN;
			}
			else if(strcmp(pData->szCharData, "ROAD") == 0)
			{
				trav_type = ROAD;
			}
			else if(strcmp(pData->szCharData, "PLAINS") == 0)
			{
				trav_type = PLAINS;
			}
			else if(strcmp(pData->szCharData, "SAND") == 0)
			{
				trav_type = SAND;
			}
			else if(strcmp(pData->szCharData, "SPARSE") == 0)
			{
				trav_type = SPARSE;
			}
			else if(strcmp(pData->szCharData, "DENSE") == 0)
			{
				trav_type = DENSE;
			}
			else if(strcmp(pData->szCharData, "SWAMP") == 0)
			{
				trav_type = SWAMP;
			}
			else if(strcmp(pData->szCharData, "WATER") == 0)
			{
				trav_type = WATER;
			}
			else if(strcmp(pData->szCharData, "HILLS") == 0)
			{
				trav_type = HILLS;
			}
			else if(strcmp(pData->szCharData, "NS_RIVER") == 0)
			{
				trav_type = NS_RIVER;
			}
			else if(strcmp(pData->szCharData, "EW_RIVER") == 0)
			{
				trav_type = EW_RIVER;
			}
			else if(strcmp(pData->szCharData, "EDGEOFWORLD") == 0)
			{
				trav_type = EDGEOFWORLD;
			}
			else if(strcmp(pData->szCharData, "TROPICS") == 0)
			{
				trav_type = TROPICS;
			}
			else if(strcmp(pData->szCharData, "FARMLAND") == 0)
			{
				trav_type = FARMLAND;
			}
			else if(strcmp(pData->szCharData, "PLAINS_ROAD") == 0)
			{
				trav_type = PLAINS_ROAD;
			}
			else if(strcmp(pData->szCharData, "SPARSE_ROAD") == 0)
			{
				trav_type = SPARSE;
			}
			else if(strcmp(pData->szCharData, "FARMLAND_ROAD") == 0)
			{
				trav_type = FARMLAND_ROAD;
			}
			else if(strcmp(pData->szCharData, "TROPICS_ROAD") == 0)
			{
				trav_type = TROPICS_ROAD;
			}
			else if(strcmp(pData->szCharData, "DENSE_ROAD") == 0)
			{
				trav_type = DENSE_ROAD;
			}
			else if(strcmp(pData->szCharData, "COASTAL") == 0)
			{
				trav_type = COASTAL;
			}
			else if(strcmp(pData->szCharData, "HILLS_ROAD") == 0)
			{
				trav_type = HILLS_ROAD;
			}
			else if(strcmp(pData->szCharData, "COASTAL_ROAD") == 0)
			{
				trav_type = COASTAL_ROAD;
			}
			else if(strcmp(pData->szCharData, "SAND_ROAD") == 0)
			{
				trav_type = SAND_ROAD;
			}
			else if(strcmp(pData->szCharData, "SWAMP_ROAD") == 0)
			{
				trav_type = SWAMP_ROAD;
			}
			else 
			{
				trav_type = GROUNDBARRIER;
			}
			// Now assign it to the correct directory using the close tag as a guide
			if(strcmp(name, "North") == 0)
			{
				pData->travNorth = trav_type;
			}
			else if(strcmp(name, "East") == 0)
			{
				pData->travEast = trav_type;
			}
			else if(strcmp(name, "South") == 0)
			{
				pData->travSouth = trav_type;
			}
			else if(strcmp(name, "West") == 0)
			{
				pData->travWest = trav_type;
			}
			else if(strcmp(name, "Here") == 0)
			{
				pData->travHere = trav_type;
			}
			else
			{
				// Bogus Traversal Reference
				Assert(FALSE);
			}
			//fprintf (outfile, "Extracted %s traversal: %d\n", name, trav_type);
			//fprintf (outfile, "Exiting Traversal Type\n");
			pData->curElement = SMCTABLE_ELEMENT_SECTOR;
		}
		//else if(strcmp(name, "Shortname") == 0 && pData->curElement == SMCTABLE_ELEMENT_SHORTNAME)
		//{
		//	// Copy this string into the pTownNames array
		//	//fprintf(outfile, "Element data: %s\n", pData->szCharData);
		//	strncpy(pData->szCustomShortname, pData->szCharData, MAX_SECTORNAME_LENGTH);
		//	//fprintf(outfile, "Shortname extracted: %s\n", pData->szCustomShortname);
		//	pData->curElement = SMCTABLE_ELEMENT_SECTOR;
		//	//fprintf(outfile, "Exiting Shortname\n");
		//}

		pData->maxReadDepth--;
	}
	pData->currentDepth--;

    //fclose (outfile);
}


BOOLEAN ReadInStrategicMovementCosts(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	smctableParseData pData;

	// Open weapons file
	hFile = FileOpen( fileName, FILE_ACCESS_READ, FALSE );
	if ( !hFile )
	{
		return( FALSE );
	}

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

	// Setup expat callbacks
	XML_SetElementHandler(parser, smctableStartElementHandle, smctableEndElementHandle);
	XML_SetCharacterDataHandler(parser, smctableCharacterDataHandle);

	memset(&pData,0,sizeof(pData));
	XML_SetUserData(parser, &pData);

    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in StrategicMovementCosts.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

	XML_ParserFree(parser);
	return TRUE;
}


BOOLEAN WriteInStrategicMovementCosts(STR fileName)
{
	// Lets output the current Strategic map format using the XML structure I've devised.
	FILE *outfile = fopen(fileName, "wt");

	UINT32 x, y, dir_type, trav_type;
	SECTORINFO *pSector;

	fprintf (outfile, "<StrategicMovementCosts>\n");
	fprintf (outfile, "\t<SectorTraversal>\n");
	for(y = 1;y <= 16;y++)
	{
		for(x = 1;x <= 16; x++)
		{
			pSector = &SectorInfo[ ((y - 1) * 16) + (x - 1) ];
			fprintf (outfile, "\t\t<Sector y=\"%c\" x=\"%d\">\n",(y+0x40),x);
			//if(pSector->szCustomShortname != NULL)
			//{
			//	fprintf (outfile, "\t\t\t<Shortname>%s</Shortname>\n",pSector->szCustomShortname);
			//}
			fprintf (outfile, "\t\t\t<TravelRating>%d</TravelRating>\n",pSector->ubTravelRating);
			// Direction Types:
			// 0. NORTH
			// 1. EAST
			// 2. SOUTH
			// 3. WEST
			// 4. THROUGH
			for(dir_type = 0; dir_type < 5; dir_type++)
			{
				trav_type = pSector->ubTraversability[ dir_type ];

				// Output the traverse direction as the XML tag
				switch(dir_type)
				{
				case NORTH_STRATEGIC_MOVE:
					fprintf (outfile, "\t\t\t<North>");
					break;
				case EAST_STRATEGIC_MOVE:
					fprintf (outfile, "\t\t\t<East>");
					break;
				case SOUTH_STRATEGIC_MOVE:
					fprintf (outfile, "\t\t\t<South>");
					break;
				case WEST_STRATEGIC_MOVE:
					fprintf (outfile, "\t\t\t<West>");
					break;
				default:
					fprintf (outfile, "\t\t\t<Here>");
				}
				// Output the traversal type
				switch(trav_type)
				{
				case TOWN:
					fprintf (outfile, "TOWN");
					break;
				case ROAD:
					fprintf (outfile, "ROAD");
					break;
				case PLAINS:
					fprintf (outfile, "PLAINS");
					break;
				case SAND:
					fprintf (outfile, "SAND");
					break;
				case SPARSE:
					fprintf (outfile, "SPARSE");
					break;
				case DENSE: 
					fprintf (outfile, "DENSE");
					break;
				case SWAMP:
					fprintf (outfile, "SWAMP");
					break;
				case WATER:
					fprintf (outfile, "WATER");
					break;
				case HILLS:
					fprintf (outfile, "HILLS");
					break;
				case GROUNDBARRIER:
					fprintf (outfile, "GROUNDBARRIER");
					break;
				case NS_RIVER:
					fprintf (outfile, "NS_RIVER");
					break;
				case EW_RIVER:
					fprintf (outfile, "EW_RIVER");
					break;
				case EDGEOFWORLD:
					fprintf (outfile, "EDGEOFWORLD");
					break;
				case TROPICS:
					fprintf (outfile, "TROPICS");
					break;
				case FARMLAND:
					fprintf (outfile, "FARMLAND");
					break;
				case PLAINS_ROAD:
					fprintf (outfile, "PLAINS_ROAD");
					break;
				case SPARSE_ROAD:
					fprintf (outfile, "SPARSE_ROAD");
					break;
				case FARMLAND_ROAD:
					fprintf (outfile, "FARMLAND_ROAD");
					break;
				case TROPICS_ROAD:
					fprintf (outfile, "TROPICS_ROAD");
					break;
				case DENSE_ROAD:
					fprintf (outfile, "DENSE_ROAD");
					break;
				case COASTAL:
					fprintf (outfile, "COASTAL");
					break;
				case HILLS_ROAD:
					fprintf (outfile, "HILLS_ROAD");
					break;
				case COASTAL_ROAD:
					fprintf (outfile, "COASTAL_ROAD");
					break;
				case SAND_ROAD:
					fprintf (outfile, "SAND_ROAD");
					break;
				case SWAMP_ROAD:
					fprintf (outfile, "SWAMP_ROAD");
					break;
				default:
					fprintf (outfile, "%d", trav_type);
				}
				// Output the traverse direction as the closing tag
				switch(dir_type)
				{
				case NORTH_STRATEGIC_MOVE:
					fprintf (outfile, "</North>\n");
					break;
				case EAST_STRATEGIC_MOVE:
					fprintf (outfile, "</East>\n");
					break;
				case SOUTH_STRATEGIC_MOVE:
					fprintf (outfile, "</South>\n");
					break;
				case WEST_STRATEGIC_MOVE:
					fprintf (outfile, "</West>\n");
					break;
				default:
					fprintf (outfile, "</Here>\n");
				}
			}
			fprintf (outfile, "\t\t</Sector>\n");
			}
	}
	fprintf (outfile, "\t</SectorTraversal>\n");
	fprintf (outfile, "</StrategicMovementCosts>\n");

	fclose(outfile);
		
	return (TRUE);
}
