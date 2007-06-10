#ifdef PRECOMPILEDHEADERS
	#include "utils_all.h"
#else
	#include "encrypted_file.h"
	#include "file_man.h"
	#include "debug.h"
	#include "sgp_str.h"
	#include "mem_man.h"
	
#endif

#include "language_defines.h"

//=====================================================================
//
//	LoadEncryptedDataFromFile16 - load data from file with simple
//	encryption on 16-bit elements.
//
//=====================================================================
BOOLEAN LoadEncryptedDataFromFile16(STR pFileName, void *pDestString, UINT32 uiSeekFrom, UINT32 uiSeekAmount)
{
	HWFILE		hFile;
	UINT16		i;
	UINT32		uiBytesRead, uiSymbolsCount;
	UINT16		*pBuffer = (UINT16*) pDestString;

	uiSymbolsCount = uiSeekAmount / sizeof(UINT16);
	hFile = FileOpen(pFileName, FILE_ACCESS_READ, FALSE);
	if ( !hFile )
	{
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "LoadEncryptedDataFromFile: Failed to FileOpen");
		return( FALSE );
	}

	if ( FileSeek( hFile, uiSeekFrom, FILE_SEEK_FROM_START ) == FALSE )
	{
		FileClose(hFile);
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "LoadEncryptedDataFromFile: Failed FileSeek");
		return( FALSE );
	}

	if( !FileRead( hFile, pDestString, uiSeekAmount, &uiBytesRead) )
	{
		FileClose(hFile);
		DebugMsg( TOPIC_JA2, DBG_LEVEL_3, "LoadEncryptedDataFromFile: Failed FileRead");
		return( FALSE );
	}

	// Decrement, by 1, any value > 32
	for(i=0; (i<uiSymbolsCount) && (pBuffer[i] != 0); i++ )
	{
		if( pBuffer[i] > 33 )
			pBuffer[i] -= 1;
		#ifdef POLISH
			switch( pBuffer[ i ] )
			{
				case 260:		pBuffer[ i ] = 165;		break;
				case 262:		pBuffer[ i ] = 198;		break;
				case 280:		pBuffer[ i ] = 202;		break;
				case 321:		pBuffer[ i ] = 163;		break;
				case 323:		pBuffer[ i ] = 209;		break;
				case 211:		pBuffer[ i ] = 211;		break;

				case 346:		pBuffer[ i ] = 338;		break;
				case 379:		pBuffer[ i ] = 175;		break;
				case 377:		pBuffer[ i ] = 143;		break;
				case 261:		pBuffer[ i ] = 185;		break;
				case 263:		pBuffer[ i ] = 230;		break;
				case 281:		pBuffer[ i ] = 234;		break;

				case 322:		pBuffer[ i ] = 179;		break;
				case 324:		pBuffer[ i ] = 241;		break;
				case 243:		pBuffer[ i ] = 243;		break;
				case 347:		pBuffer[ i ] = 339;		break;
				case 380:		pBuffer[ i ] = 191;		break;
				case 378:		pBuffer[ i ] = 376;		break;
			}
		#endif
	}

	FileClose(hFile);
	return(TRUE);
}

//=====================================================================
//
//	LoadEncryptedDataFromFile - load UNICODE string from encrypted file
//	This is wrapper function. It is needed, because game resources
//	contains strings in UTF-16, while linux uses UTF-32.
//
//=====================================================================
BOOLEAN LoadEncryptedDataFromFile(STR pFileName, STR16 pDestString, UINT32 uiSeekFrom, UINT32 uiSeekAmount)
{
	UINT16	*pUTF16String;
	UINT16	usMaxSymbolCount = uiSeekAmount / sizeof(UINT16);

//	printf("encLoading %s\n", pFileName);
//	printf("pos=%d, size=%d\n", uiSeekFrom, uiSeekAmount);

	// allocate and prepare memory
	pUTF16String = (UINT16*) MemAlloc( uiSeekAmount );
	if ( !pUTF16String )
		return FALSE;
	memset( pUTF16String, 0, uiSeekAmount );

	// read data and convert
	if ( !LoadEncryptedDataFromFile16(pFileName, pUTF16String, uiSeekFrom, uiSeekAmount) )
		return FALSE;
	ConvertUTF16to32( pUTF16String, pDestString, usMaxSymbolCount );

	// free used memory and finish
	MemFree( pUTF16String );
	return TRUE;
}
