#ifndef _GAME_VERSION_H_
#define _GAME_VERSION_H_


#ifdef __cplusplus
extern "C" {
#endif


//
//	Keeps track of the game version
//

extern	CHAR16		zVersionLabel[256];
extern	CHAR8		czVersionNumber[16];
extern	CHAR16		zTrackingNumber[16];


//ADB:  I needed these here so I moved them, and why put them in *.cpp anyways?
//
//		Keeps track of the saved game version.  Increment the saved game version whenever 
//	you will invalidate the saved game file

#define			SAVE_GAME_VERSION					102	//101	//100 // 99
#define			CURRENT_SOLDIERTYPE_VERSION			102
#define			FIRST_SOLDIERTYPE_CHANGE			102
//#define			SECOND_SOLDIERTYPE_CHANGE			999, #define	CURRENT_SOLDIERTYPE_VERSION	999




//#define RUSSIANGOLD
const extern	UINT32	guiSavedGameVersion;

#ifdef __cplusplus
}
#endif


#endif
