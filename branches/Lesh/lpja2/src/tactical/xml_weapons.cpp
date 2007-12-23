#include "weapons.h"
#include "xml.h"
#include "file_man.h"
#include "debug_control.h"
#include "debug.h"


enum
{
	WEAPON_ELEMENT_NONE = 0,
	WEAPON_ELEMENT_WEAPONLIST,
	WEAPON_ELEMENT_WEAPON,
	WEAPON_ELEMENT_WEAPON_PROPERY,
}
typedef WEP_PARSE_STAGE;

struct
{
	WEP_PARSE_STAGE	curElement;

	CHAR8			szCharData[MAX_CHAR_DATA_LENGTH+1];
	WEAPONTYPE		curWeapon;
	WEAPONTYPE *	curWeaponList;
	UINT32			maxWeapons;
	
	UINT32			currentDepth;
	UINT32			maxReadDepth;
}
typedef weaponParseData;


static void XMLCALL
weaponStartElementHandle(void *userData, const char *name, const char **atts)
{
	weaponParseData * pData = (weaponParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //are we reading this element?
	{
		if(strcmp(name, "WEAPONLIST") == 0 && pData->curElement == WEAPON_ELEMENT_NONE)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPONLIST;

			memset(pData->curWeaponList,0,sizeof(WEAPONTYPE)*pData->maxWeapons);

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(strcmp(name, "WEAPON") == 0 && pData->curElement == WEAPON_ELEMENT_WEAPONLIST)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;

			memset(&pData->curWeapon,0,sizeof(WEAPONTYPE));

			pData->maxReadDepth++; //we are not skipping this element
		}
		else if(pData->curElement == WEAPON_ELEMENT_WEAPON &&
				(strcmp(name, "uiIndex") == 0 ||
				strcmp(name, "szWeaponName") == 0 ||
				strcmp(name, "ubWeaponClass") == 0 ||
				strcmp(name, "ubWeaponType") == 0 ||
				strcmp(name, "ubCalibre") == 0 ||
				strcmp(name, "ubReadyTime") == 0 ||
				strcmp(name, "ubShotsPer4Turns") == 0 ||
				strcmp(name, "ubShotsPerBurst") == 0 ||
				strcmp(name, "ubBurstPenalty") == 0 ||
				strcmp(name, "ubBulletSpeed") == 0 ||
				strcmp(name, "ubImpact") == 0 ||
				strcmp(name, "ubDeadliness") == 0 ||
				strcmp(name, "bAccuracy") == 0 ||
				strcmp(name, "ubMagSize") == 0 ||
				strcmp(name, "usRange")	 == 0 ||
				strcmp(name, "usReloadDelay") == 0 ||
				strcmp(name, "ubAttackVolume") == 0 ||
				strcmp(name, "ubHitVolume") == 0 ||
				strcmp(name, "sSound") == 0 ||
				strcmp(name, "sBurstSound") == 0 ||
				strcmp(name, "sSilencedBurstSound") == 0 ||
				strcmp(name, "sReloadSound") == 0 ||
				strcmp(name, "sLocknLoadSound") == 0 ||
				strcmp(name, "bBurstAP") == 0 ||
				strcmp(name, "bAutofireShotsPerFiveAP") == 0 ||
				strcmp(name, "APsToReload") == 0 ||
				strcmp(name, "SwapClips") == 0 ||
				strcmp(name, "AutoPenalty") == 0 ||
				strcmp(name, "NoSemiAuto") == 0 ||
				strcmp(name, "MaxDistForMessyDeath") == 0 ||
				strcmp(name, "SilencedSound") == 0 || // Lesh: add new field (OR operand)
                strcmp(name, "BurstAniDelay") == 0 || // Lesh: add new field (field itself)
				strcmp(name, "APsToReloadManually") == 0 ||
				strcmp(name, "ManualReloadSound") == 0)
				)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON_PROPERY;

			pData->maxReadDepth++; //we are not skipping this element
		}

		pData->szCharData[0] = '\0';
	}

	pData->currentDepth++;

}

static void XMLCALL
weaponCharacterDataHandle(void *userData, const char *str, int len)
{
	weaponParseData * pData = (weaponParseData *)userData;

	if( (pData->currentDepth <= pData->maxReadDepth) && 
		(strlen(pData->szCharData) < MAX_CHAR_DATA_LENGTH)
	  ){
		strncat(pData->szCharData,str,__min((unsigned int)len,MAX_CHAR_DATA_LENGTH-strlen(pData->szCharData)));
	}
}


static void XMLCALL
weaponEndElementHandle(void *userData, const char *name)
{
	weaponParseData * pData = (weaponParseData *)userData;

	if(pData->currentDepth <= pData->maxReadDepth) //we're at the end of an element that we've been reading
	{
		if(strcmp(name, "WEAPONLIST") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_NONE;
		}
		else if(strcmp(name, "WEAPON") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPONLIST;

			if(pData->curWeapon.uiIndex < pData->maxWeapons)
			{
				pData->curWeaponList[pData->curWeapon.uiIndex] = pData->curWeapon; //write the weapon into the table
			}
		}
		else if(strcmp(name, "uiIndex") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.uiIndex = atoi(pData->szCharData);
		}
		else if(strcmp(name, "szWeaponName") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;

			if(MAX_WEAPON_NAME_LENGTH >= strlen(pData->szCharData))
				strcpy(pData->curWeapon.szWeaponName,pData->szCharData);
			else
			{
				strncpy(pData->curWeapon.szWeaponName,pData->szCharData,MAX_WEAPON_NAME_LENGTH);
				pData->curWeapon.szWeaponName[MAX_WEAPON_NAME_LENGTH] = '\0';
			}
		}
		else if(strcmp(name, "ubWeaponClass") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubWeaponClass = (UINT8) atoi(pData->szCharData);
		}
		else if(strcmp(name, "ubWeaponType") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubWeaponType = (UINT8) atoi(pData->szCharData);
		}
		else if(strcmp(name, "ubCalibre") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubCalibre = (UINT8) atoi(pData->szCharData);
		}
		else if(strcmp(name, "ubReadyTime") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubReadyTime = (UINT8) atoi(pData->szCharData);
		}
		else if(strcmp(name, "ubShotsPer4Turns") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubShotsPer4Turns = (UINT8) atoi(pData->szCharData);
		}
		else if(strcmp(name, "ubShotsPerBurst") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubShotsPerBurst = (UINT8) atoi(pData->szCharData);
		}
		else if(strcmp(name, "ubBurstPenalty") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubBurstPenalty = (UINT8) atoi(pData->szCharData);
		}
		else if(strcmp(name, "ubBulletSpeed") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubBulletSpeed = (UINT8) atoi(pData->szCharData);
		}
		else if(strcmp(name, "ubImpact") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubImpact = (UINT8) atoi(pData->szCharData);
		}
		else if(strcmp(name, "ubDeadliness") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubDeadliness = (UINT8) atoi(pData->szCharData);
		}
		else if(strcmp(name, "bAccuracy") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.bAccuracy = (INT8) atoi(pData->szCharData);
		}
		else if(strcmp(name, "ubMagSize") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubMagSize = (UINT8) atoi(pData->szCharData);
		}
		else if(strcmp(name, "usRange")	 == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.usRange = (UINT16) atoi(pData->szCharData);
		}
		else if(strcmp(name, "usReloadDelay") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.usReloadDelay = (UINT16) atoi(pData->szCharData);
		}
		else if(strcmp(name, "ubAttackVolume") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubAttackVolume = (UINT8) atoi(pData->szCharData);
		}
		else if(strcmp(name, "ubHitVolume") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.ubHitVolume = (UINT8) atoi(pData->szCharData);
		}
		else if(strcmp(name, "sSound") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.sSound = (UINT16) atoi(pData->szCharData);
		}
		else if(strcmp(name, "sBurstSound") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.sBurstSound = (UINT16) atoi(pData->szCharData);
		}
		else if(strcmp(name, "sSilencedBurstSound") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.sSilencedBurstSound = (UINT16) atoi(pData->szCharData);
		}
		else if(strcmp(name, "sReloadSound") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.sReloadSound = (UINT16) atoi(pData->szCharData);
		}
		else if(strcmp(name, "sLocknLoadSound") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.sLocknLoadSound = (UINT16) atoi(pData->szCharData);
		}
		else if(strcmp(name, "bBurstAP") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.bBurstAP = (UINT8) atoi(pData->szCharData);
		}
		else if(strcmp(name, "bAutofireShotsPerFiveAP") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.bAutofireShotsPerFiveAP = (UINT8) atoi(pData->szCharData);
		}
		else if(strcmp(name, "APsToReload") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.APsToReload = (UINT8) atoi(pData->szCharData);
		}
		else if(strcmp(name, "SilencedSound") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.silencedSound = (UINT16) atoi(pData->szCharData);
		}
		else if(strcmp(name, "SwapClips") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.swapClips = (BOOLEAN) atoi(pData->szCharData);
		}
		else if(strcmp(name, "MaxDistForMessyDeath") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.maxdistformessydeath = (UINT8) atoi(pData->szCharData);
		}
		else if(strcmp(name, "AutoPenalty") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.AutoPenalty = (UINT8) atoi(pData->szCharData);
		}
		else if(strcmp(name, "NoSemiAuto") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
			pData->curWeapon.NoSemiAuto = (BOOLEAN) atoi(pData->szCharData);
		}
        // Lesh: xmlreader must handle new field
   		else if(strcmp(name, "BurstAniDelay") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
            pData->curWeapon.sAniDelay = (INT16) atoi(pData->szCharData);
		}
        // Lesh: end
   		else if(strcmp(name, "APsToReloadManually") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
            pData->curWeapon.APsToReloadManually = (UINT8) atoi(pData->szCharData);
		}
   		else if(strcmp(name, "ManualReloadSound") == 0)
		{
			pData->curElement = WEAPON_ELEMENT_WEAPON;
            pData->curWeapon.ManualReloadSound = (UINT16) atoi(pData->szCharData);
		}

		pData->maxReadDepth--;
	}

	pData->currentDepth--;
}




BOOLEAN ReadInWeaponStats(STR fileName)
{
	HWFILE		hFile;
	UINT32		uiBytesRead;
	UINT32		uiFSize;
	CHAR8 *		lpcBuffer;
	XML_Parser	parser = XML_ParserCreate(NULL);
	
	weaponParseData pData;

	DebugMsg(TOPIC_JA2, DBG_LEVEL_3, "Loading weapons.xml" );

	// Open weapons file
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

	
	XML_SetElementHandler(parser, weaponStartElementHandle, weaponEndElementHandle);
	XML_SetCharacterDataHandler(parser, weaponCharacterDataHandle);

	
	memset(&pData,0,sizeof(pData));
	pData.curWeaponList = Weapon;
	pData.maxWeapons = MAXITEMS; 
	
	XML_SetUserData(parser, &pData);


    if(!XML_Parse(parser, lpcBuffer, uiFSize, TRUE))
	{
		CHAR8 errorBuf[511];

		sprintf(errorBuf, "XML Parser Error in Weapons.xml: %s at line %d", XML_ErrorString(XML_GetErrorCode(parser)), XML_GetCurrentLineNumber(parser));
		LiveMessage(errorBuf);

		MemFree(lpcBuffer);
		return FALSE;
	}

	MemFree(lpcBuffer);

#ifdef JA2TESTVERSION
	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\~Weapons out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );
	
	{
		UINT32 cnt;

		FilePrintf(hFile,"<WEAPONLIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{
			INT8 * szRemainder = Weapon[cnt].szWeaponName; //the remaining string to be output (for making valid XML)

			FilePrintf(hFile,"\t<WEAPON>\r\n");
			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",									Weapon[cnt].uiIndex);

			FilePrintf(hFile,"\t\t<szWeaponName>");
			while(szRemainder[0] != '\0')
			{
				UINT32 uiCharLoc = strcspn(szRemainder,"&<>\'\"\0");
				INT8 invChar = szRemainder[uiCharLoc];
				
				if(uiCharLoc)
				{
					szRemainder[uiCharLoc] = '\0';
					FilePrintf(hFile,"%s",szRemainder);
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
			FilePrintf(hFile,"</szWeaponName>\r\n");

			FilePrintf(hFile,"\t\t<ubWeaponClass>%d</ubWeaponClass>\r\n",						Weapon[cnt].ubWeaponClass);
			FilePrintf(hFile,"\t\t<ubWeaponType>%d</ubWeaponType>\r\n",							Weapon[cnt].ubWeaponType);
			FilePrintf(hFile,"\t\t<ubCalibre>%d</ubCalibre>\r\n",								Weapon[cnt].ubCalibre);
			FilePrintf(hFile,"\t\t<ubReadyTime>%d</ubReadyTime>\r\n",							Weapon[cnt].ubReadyTime);
			FilePrintf(hFile,"\t\t<ubShotsPer4Turns>%d</ubShotsPer4Turns>\r\n",					Weapon[cnt].ubShotsPer4Turns);
			FilePrintf(hFile,"\t\t<ubShotsPerBurst>%d</ubShotsPerBurst>\r\n",					Weapon[cnt].ubShotsPerBurst);
			FilePrintf(hFile,"\t\t<ubBurstPenalty>%d</ubBurstPenalty>\r\n",						Weapon[cnt].ubBurstPenalty);
			FilePrintf(hFile,"\t\t<ubBulletSpeed>%d</ubBulletSpeed>\r\n",						Weapon[cnt].ubBulletSpeed);
			FilePrintf(hFile,"\t\t<ubImpact>%d</ubImpact>\r\n",									Weapon[cnt].ubImpact);
			FilePrintf(hFile,"\t\t<ubDeadliness>%d</ubDeadliness>\r\n",							Weapon[cnt].ubDeadliness);
			FilePrintf(hFile,"\t\t<bAccuracy>%d</bAccuracy>\r\n",								Weapon[cnt].bAccuracy);
			FilePrintf(hFile,"\t\t<ubMagSize>%d</ubMagSize>\r\n",								Weapon[cnt].ubMagSize);
			FilePrintf(hFile,"\t\t<usRange>%d</usRange>\r\n",									Weapon[cnt].usRange);
			FilePrintf(hFile,"\t\t<usReloadDelay>%d</usReloadDelay>\r\n",						Weapon[cnt].usReloadDelay);
			FilePrintf(hFile,"\t\t<ubAttackVolume>%d</ubAttackVolume>\r\n",						Weapon[cnt].ubAttackVolume);
			FilePrintf(hFile,"\t\t<ubHitVolume>%d</ubHitVolume>\r\n",							Weapon[cnt].ubHitVolume);
			FilePrintf(hFile,"\t\t<sSound>%d</sSound>\r\n",										Weapon[cnt].sSound);
			FilePrintf(hFile,"\t\t<sBurstSound>%d</sBurstSound>\r\n",							Weapon[cnt].sBurstSound);
			FilePrintf(hFile,"\t\t<sSilencedBurstSound>%d</sSilencedBurstSound>\r\n",			Weapon[cnt].sSilencedBurstSound);
			FilePrintf(hFile,"\t\t<sReloadSound>%d</sReloadSound>\r\n",							Weapon[cnt].sReloadSound);
			FilePrintf(hFile,"\t\t<sLocknLoadSound>%d</sLocknLoadSound>\r\n",					Weapon[cnt].sLocknLoadSound);
			FilePrintf(hFile,"\t\t<bBurstAP>%d</bBurstAP>\r\n",									Weapon[cnt].bBurstAP);
			FilePrintf(hFile,"\t\t<bAutofireShotsPerFiveAP>%d</bAutofireShotsPerFiveAP>\r\n",	Weapon[cnt].bAutofireShotsPerFiveAP);
			FilePrintf(hFile,"\t\t<APsToReloadManually>%d</APsToReloadManually>\r\n",			Weapon[cnt].APsToReloadManually);
			FilePrintf(hFile,"\t\t<ManualReloadSound>%d</ManualReloadSound>\r\n",				Weapon[cnt].ManualReloadSound);
			FilePrintf(hFile,"\t</WEAPON>\r\n");
		}
		FilePrintf(hFile,"</WEAPONLIST>\r\n");
	}
	FileClose( hFile );
#endif

	XML_ParserFree(parser);

	return( TRUE );
}

BOOLEAN WriteWeaponStats()
{
	HWFILE		hFile;
	
	//Debug code; make sure that what we got from the file is the same as what's there
	// Open a new file
	hFile = FileOpen( "TABLEDATA\\Weapons out.xml", FILE_ACCESS_WRITE | FILE_CREATE_ALWAYS, FALSE );
	if ( !hFile )
		return( FALSE );
	
	{
		UINT32 cnt;

		FilePrintf(hFile,"<WEAPONLIST>\r\n");
		for(cnt = 0;cnt < MAXITEMS;cnt++)
		{
			CHAR8 * szRemainder = Weapon[cnt].szWeaponName; //the remaining string to be output (for making valid XML)

			FilePrintf(hFile,"\t<WEAPON>\r\n");
			FilePrintf(hFile,"\t\t<uiIndex>%d</uiIndex>\r\n",									cnt);//Weapon[cnt].uiIndex);

			FilePrintf(hFile,"\t\t<szWeaponName>");
			while(szRemainder[0] != '\0')
			{
				UINT32 uiCharLoc = strcspn(szRemainder,"&<>\'\"\0");
				CHAR8 invChar = szRemainder[uiCharLoc];
				
				if(uiCharLoc)
				{
					szRemainder[uiCharLoc] = '\0';
					FilePrintf(hFile,"%s",szRemainder);
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
			FilePrintf(hFile,"</szWeaponName>\r\n");

			FilePrintf(hFile,"\t\t<ubWeaponClass>%d</ubWeaponClass>\r\n",						Weapon[cnt].ubWeaponClass);
			FilePrintf(hFile,"\t\t<ubWeaponType>%d</ubWeaponType>\r\n",							Weapon[cnt].ubWeaponType);
			FilePrintf(hFile,"\t\t<ubCalibre>%d</ubCalibre>\r\n",								Weapon[cnt].ubCalibre);
			FilePrintf(hFile,"\t\t<ubReadyTime>%d</ubReadyTime>\r\n",							Weapon[cnt].ubReadyTime);
			FilePrintf(hFile,"\t\t<ubShotsPer4Turns>%d</ubShotsPer4Turns>\r\n",					Weapon[cnt].ubShotsPer4Turns);
			FilePrintf(hFile,"\t\t<ubShotsPerBurst>%d</ubShotsPerBurst>\r\n",					Weapon[cnt].ubShotsPerBurst);
			FilePrintf(hFile,"\t\t<ubBurstPenalty>%d</ubBurstPenalty>\r\n",						Weapon[cnt].ubBurstPenalty);
			FilePrintf(hFile,"\t\t<ubBulletSpeed>%d</ubBulletSpeed>\r\n",						Weapon[cnt].ubBulletSpeed);
			FilePrintf(hFile,"\t\t<ubImpact>%d</ubImpact>\r\n",									Weapon[cnt].ubImpact);
			FilePrintf(hFile,"\t\t<ubDeadliness>%d</ubDeadliness>\r\n",							Weapon[cnt].ubDeadliness);
			FilePrintf(hFile,"\t\t<bAccuracy>%d</bAccuracy>\r\n",								Weapon[cnt].bAccuracy);
			FilePrintf(hFile,"\t\t<ubMagSize>%d</ubMagSize>\r\n",								Weapon[cnt].ubMagSize);
			FilePrintf(hFile,"\t\t<usRange>%d</usRange>\r\n",									Weapon[cnt].usRange);
			FilePrintf(hFile,"\t\t<usReloadDelay>%d</usReloadDelay>\r\n",						Weapon[cnt].usReloadDelay);
			FilePrintf(hFile,"\t\t<BurstAniDelay>%d</BurstAniDelay>\r\n",						Weapon[cnt].sAniDelay);
			FilePrintf(hFile,"\t\t<ubAttackVolume>%d</ubAttackVolume>\r\n",						Weapon[cnt].ubAttackVolume);
			FilePrintf(hFile,"\t\t<ubHitVolume>%d</ubHitVolume>\r\n",							Weapon[cnt].ubHitVolume);
			FilePrintf(hFile,"\t\t<sSound>%d</sSound>\r\n",										Weapon[cnt].sSound);
			FilePrintf(hFile,"\t\t<sBurstSound>%d</sBurstSound>\r\n",							Weapon[cnt].sBurstSound);
			FilePrintf(hFile,"\t\t<sSilencedBurstSound>%d</sSilencedBurstSound>\r\n",							Weapon[cnt].sSilencedBurstSound);
			FilePrintf(hFile,"\t\t<sReloadSound>%d</sReloadSound>\r\n",							Weapon[cnt].sReloadSound);
			FilePrintf(hFile,"\t\t<sLocknLoadSound>%d</sLocknLoadSound>\r\n",					Weapon[cnt].sLocknLoadSound);
			FilePrintf(hFile,"\t\t<SilencedSound>%d</SilencedSound>\r\n",						Weapon[cnt].silencedSound  );
			FilePrintf(hFile,"\t\t<bBurstAP>%d</bBurstAP>\r\n",				Weapon[cnt].bBurstAP);
			FilePrintf(hFile,"\t\t<bAutofireShotsPerFiveAP>%d</bAutofireShotsPerFiveAP>\r\n",	Weapon[cnt].bAutofireShotsPerFiveAP);
			FilePrintf(hFile,"\t\t<APsToReload>%d</APsToReload>\r\n",							Weapon[cnt].APsToReload);
			FilePrintf(hFile,"\t\t<SwapClips>%d</SwapClips>\r\n",								Weapon[cnt].swapClips );
			FilePrintf(hFile,"\t\t<MaxDistForMessyDeath>%d</MaxDistForMessyDeath>\r\n",			Weapon[cnt].maxdistformessydeath);
			FilePrintf(hFile,"\t\t<AutoPenalty>%d</AutoPenalty>\r\n",			Weapon[cnt].AutoPenalty);
			FilePrintf(hFile,"\t\t<NoSemiAuto>%d</NoSemiAuto>\r\n",			Weapon[cnt].NoSemiAuto);



			FilePrintf(hFile,"\t</WEAPON>\r\n");
		}
		FilePrintf(hFile,"</WEAPONLIST>\r\n");
	}
	FileClose( hFile );

	return( TRUE );
}
