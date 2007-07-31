#ifdef PRECOMPILEDHEADERS
	#include "ja2_all.h"
#else
	#include "types.h"
	#include "game_version.h"
#endif
 
   
//
//	Keeps track of the game version
//
 
#ifdef JA2BETAVERSION

//BETA/TEST BUILD VERSION
CHAR16		zVersionLabel[256]		= { L"Map Editor v1.13.687" }; 

#elif defined CRIPPLED_VERSION

//RELEASE BUILD VERSION 
CHAR16		zVersionLabel[256]		= { L"Beta v. 0.98" }; 

#else

	//RELEASE BUILD VERSION  
	CHAR16		zVersionLabel[256]		= L"Release v1.13.844";


#endif
										
CHAR8		czVersionNumber[16]	= { "Build 07.04.19" };		//YY.MM.DD
CHAR16		zTrackingNumber[16]	= { L"Z" };
	

//
//		Keeps track of the saved game version.  Increment the saved game version whenever 
//	you will invalidate the saved game file

#define			SAVE_GAME_VERSION					101	//100 // 99

const	UINT32	guiSavedGameVersion = SAVE_GAME_VERSION;
