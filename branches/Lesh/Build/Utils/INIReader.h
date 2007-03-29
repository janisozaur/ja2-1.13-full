#ifndef INIREADER_H
#define INIREADER_H

#include "Platform.h"
#include "SDL_config_lib.h"


// Kaiden: This will read any value out of 
// an INI file as long as the correct type is specified.
// Methods should be fairly self explainatory:
class CIniReader
{
public:
	CIniReader();

	bool Open(const char* szFileName);
	void Close();

	int ReadInteger(const char* szSection, const char* szKey, int iDefaultValue);
	int ReadInteger(const char* szSection, const char* szKey, int iDefaultValue, int iMinValue, int iMaxValue);
	float ReadFloat(const char* szSection, const char* szKey, float fltDefaultValue);
	bool ReadBoolean(const char* szSection, const char* szKey, bool bolDefaultValue);
	const char* ReadString(const char* szSection, const char* szKey, const char* szDefaultValue);
private:
	char m_szFileName[MAX_PATH];
	CFG_File	*cfg;
};

#endif//INIREADER_H
