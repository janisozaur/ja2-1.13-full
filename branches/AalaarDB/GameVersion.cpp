#ifdef PRECOMPILEDHEADERS
	#include "JA2 All.h"
#else
	#include "Types.h"
	#include "GameVersion.h"
#endif
 
   
//
//	Keeps track of the game version
//
 
#ifdef JA2BETAVERSION

//BETA/TEST BUILD VERSION
CHAR16 zVersionLabel[256]		= { L"Map Editor v1.13.857" }; 

#elif defined CRIPPLED_VERSION

//RELEASE BUILD VERSION 
CHAR16 zVersionLabel[256]		= { L"Beta v. 0.98" }; 

#else

	//RELEASE BUILD VERSION  
	CHAR16		zVersionLabel[256]		= { L"Release v1.13.1052" };


#endif
										
CHAR8		czVersionNumber[16]	= { "Build 07.07.15" };		//YY.MM.DD
CHAR16		zTrackingNumber[16]	= { L"Z" };
	

const	UINT32	guiSavedGameVersion = SAVE_GAME_VERSION;
