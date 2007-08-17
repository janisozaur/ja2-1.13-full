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
CHAR16 zVersionLabel[256]		= { L"Map Editor v1.13.1202" }; 

#elif defined CRIPPLED_VERSION

//RELEASE BUILD VERSION 
CHAR16 zVersionLabel[256]		= { L"Beta v. 0.98" }; 

#else

	//RELEASE BUILD VERSION  
	CHAR16		zVersionLabel[256]		= { L"Release v1.13.1202" };


#endif
										
CHAR8		czVersionNumber[16]	= { "Build 07.08.14" };		//YY.MM.DD
CHAR16		zTrackingNumber[16]	= { L"Z" };
	

//
//		Keeps track of the saved game version.  Increment the saved game version whenever 
//	you will invalidate the saved game file

#define			SAVE_GAME_VERSION					102 //101	//100 // 99

const	UINT32	guiSavedGameVersion = SAVE_GAME_VERSION;
const UINT32 SAVE_GAMES_HAVE_NEW_INVENTORY_VECTORS = 102;
