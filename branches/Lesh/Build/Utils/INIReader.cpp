#include "INIReader.h"
#include "Platform.h"
#include "SDL_config_lib.h"
#include "FileMan.h"

// Kaiden: INI reading function definitions:


CIniReader::CIniReader()
{
	cfg = NULL;
}


int CIniReader::ReadInteger(const char* szSection, const char* szKey, int iDefaultValue)
{
	if ( !cfg )
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
//	return GetPrivateProfileInt(szSection,  szKey, iDefaultValue, m_szFileName); 
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
	if ( !cfg )
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
// char szResult[255];
// char szDefault[255];
// float fltResult;
// sprintf(szDefault, "%f",fltDefaultValue);
// GetPrivateProfileString(szSection,  szKey, szDefault, szResult, 255, m_szFileName); 
// fltResult = (float) atof(szResult);
// return fltResult;
}


bool CIniReader::ReadBoolean(const char* szSection, const char* szKey, bool bolDefaultValue)
{
	if ( !cfg )
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
// char szResult[255];
// char szDefault[255];
// bool bolResult;
// sprintf(szDefault, "%s", bolDefaultValue? "TRUE" : "FALSE");
// GetPrivateProfileString(szSection, szKey, szDefault, szResult, 255, m_szFileName); 
// bolResult =  (strcmp(szResult, "TRUE") == 0 || strcmp(szResult, "TRUE") == 0) ? true : false;
// return bolResult;
}


const char* CIniReader::ReadString(const char* szSection, const char* szKey, const char* szDefaultValue)
{
	if ( !cfg )
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
// char* szResult = new char[255];
// memset(szResult, 0x00, 255);
// GetPrivateProfileString(szSection,  szKey, szDefaultValue, szResult, 255, m_szFileName); 
// return szResult;
}

bool CIniReader::Open(const char* szFileName)
{
	// Snap: Look for the INI file in the custom Data directory.
	// If not there, leave at default location.
	if ( gCustomDataCat.FindFile(szFileName) )
	{
		sprintf(m_szFileName, "%s\\%s", gCustomDataCat.GetRootDir().c_str(), szFileName);
	}
	else
	{
		sprintf(m_szFileName, "%s\\%s", gDefaultDataCat.GetRootDir().c_str(), szFileName);
	}

	BACKSLASH( m_szFileName );

	if ( CFG_OpenFile( m_szFileName, cfg ) != CFG_OK )
	{
		fprintf( stderr, "INI: can't open file %s\n", m_szFileName );
		return FALSE;
	}
	return TRUE;
}

void CIniReader::Close()
{
	if ( cfg && CFG_CloseFile( cfg ) != CFG_OK )
	{
		fprintf( stderr, "INI: can't close file\n" );
	}
}





