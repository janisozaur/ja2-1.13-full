#include "ini_reader.h"
#include "platform.h"
#include "SDL_config_lib.h"
#include "file_man.h"
#include "mem_man.h"


// Kaiden: INI reading function definitions:


CIniReader::CIniReader()
{
	fIsOpen   = false;
	iniBuffer = NULL;
	stream    = NULL;
}


int CIniReader::ReadInteger(const char* szSection, const char* szKey, int iDefaultValue)
{
	if ( !fIsOpen )
	{
		fprintf( stderr, "INI: trying to read int from unopened file\n" );
		return iDefaultValue;
	}
	else
	{
		if ( CFG_SelectGroup( szSection, FALSE ) != CFG_OK )
			return iDefaultValue;
		else
			return CFG_ReadInt( szKey, iDefaultValue );
	}
}


int CIniReader::ReadInteger(const char* szSection, const char* szKey, int iDefaultValue, int iMinValue, int iMaxValue)
{
	int i = ReadInteger(szSection,  szKey, iDefaultValue); 
	if (i < iMinValue)
		return iMinValue;
	else if (i > iMaxValue)
		return iMaxValue;
	return i;
}


float CIniReader::ReadFloat(const char* szSection, const char* szKey, float fltDefaultValue)
{
	if ( !fIsOpen )
	{
		fprintf( stderr, "INI: trying to read float from unopened file\n" );
		return fltDefaultValue;
	}
	else
	{
		if ( CFG_SelectGroup( szSection, FALSE ) != CFG_OK )
			return fltDefaultValue;
		else
			return CFG_ReadFloat( szKey, fltDefaultValue );
	}
}


bool CIniReader::ReadBoolean(const char* szSection, const char* szKey, bool bolDefaultValue)
{
	if ( !fIsOpen )
	{
		fprintf( stderr, "INI: trying to read boolean from unopened file\n" );
		return bolDefaultValue;
	}
	else
	{
		if ( CFG_SelectGroup( szSection, FALSE ) != CFG_OK )
			return bolDefaultValue;
		else
			return CFG_ReadBool( szKey, bolDefaultValue );
	}
}


const char* CIniReader::ReadString(const char* szSection, const char* szKey, const char* szDefaultValue)
{
	if ( !fIsOpen )
	{
		fprintf( stderr, "INI: trying to read from unopened file\n" );
		return szDefaultValue;
	}
	else
	{
		if ( CFG_SelectGroup( szSection, FALSE ) != CFG_OK )
			return szDefaultValue;
		else
			return CFG_ReadText( szKey, szDefaultValue );
	}
}

bool CIniReader::Open(const char* szFileName, bool bAbsolutePath)
{
	INT32	iniSize;
	HWFILE	ini;
	
	// check the file existance	
	if ( !FileExistsNoDB((CHAR8*)szFileName) )
	{
		fprintf( stderr, "INI: file doesn't exist %s\n", szFileName );
		return false;
	}
	
	// read it into memory
	iniSize   = FileSize((CHAR8*)szFileName);
	iniBuffer = MemAlloc(iniSize);
	if ( !iniBuffer )
	{
		fprintf( stderr, "INI: can't allocate memory for ini file\n" );
		return false;
	}
	
	ini = FileOpen( (CHAR8*)szFileName, FILE_ACCESS_READ, FALSE);
	if ( !FileIsValidHandle(ini) )
	{
		fprintf( stderr, "INI: can't open ini file\n" );
		return false;
	}
	if ( !FileRead(ini, iniBuffer, iniSize, NULL) )
	{
		fprintf( stderr, "INI: failed to read ini file\n" );
		return false;
	}
	FileClose(ini);
		
	// now open config file
	stream = SDL_RWFromConstMem(iniBuffer, iniSize);
	if ( CFG_OpenFile_RW( stream, &cfg ) != CFG_OK )
	{
		fprintf( stderr, "INI: can't open file %s\n", szFileName );
		fIsOpen = FALSE;
		return FALSE;
	}
	fIsOpen = TRUE;
	return TRUE;
}

void CIniReader::Close()
{
	if ( fIsOpen && CFG_CloseFile( &cfg ) != CFG_OK )
	{
		fprintf( stderr, "INI: can't close file\n" );
	}
	if ( stream )
	{
		SDL_RWclose(stream);
		stream = NULL;
	}
	if ( iniBuffer )
	{
		MemFree(iniBuffer);
		iniBuffer = NULL;
	}
}
