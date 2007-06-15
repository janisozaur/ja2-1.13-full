#include "ini_reader.h"
#include "platform.h"
#include "SDL_config_lib.h"
#include "file_man.h"
#include "vfs.hpp"


// Kaiden: INI reading function definitions:


CIniReader::CIniReader()
{
	fIsOpen = false;
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
	// Lesh: If absolute (direct) path is given, use it. Otherwise...
	// Snap: Look for the INI file in the custom Data directory.
	// If not there, leave at default location.
	if ( bAbsolutePath )
	{
		strcpy(m_szFileName, szFileName);
	}
//	else if ( gCustomDataCat.FindFile(szFileName) )
//	{
//		sprintf(m_szFileName, "%s\\%s", gCustomDataCat.GetRootDir().c_str(), szFileName);
//	}
//	else
//	{
//		sprintf(m_szFileName, "%s\\%s", gDefaultDataCat.GetRootDir().c_str(), szFileName);
//	}
	else if ( !VFS.GetResourceFilename( szFileName, m_szFileName, MAX_PATH ) )
		return FALSE;

	printf("CIniReader::Open(): make proper opening\n");

	if ( CFG_OpenFile( m_szFileName, &cfg ) != CFG_OK )
	{
		fprintf( stderr, "INI: can't open file %s\n", m_szFileName );
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
		fprintf( stderr, "INI: can't close file %s\n", m_szFileName );
	}
}
