#ifndef __SOUNDMAN_
#define __SOUNDMAN_

#include "types.h"
#include "fmod.h"

/*
#ifdef __cplusplus
extern "C" {
#endif
*/

// Sample status flags
#define		SAMPLE_ALLOCATED		0x00000001
#define		SAMPLE_LOCKED			0x00000002
#define		SAMPLE_RANDOM			0x00000004
#define		SAMPLE_RANDOM_MANUAL	0x00000008

// Sound error values (they're all the same)
#define		NO_SAMPLE				0xffffffff
#define		SOUND_ERROR				0xffffffff

// Maximum allowable priority value
#define		PRIORITY_MAX			0xfffffffe
#define		PRIORITY_RANDOM			PRIORITY_MAX-1


// Structure definition for sound parameters being passed down to the sample playing function
typedef struct {
//				INT32			iSpeed;
//				UINT32			uiPitchBend;					// Random pitch bend range +/-
				INT16			sVolume;
				INT16			sPan;
				INT32			iLoop;
				INT32			iPriority;
				void			(*EOSCallback)(void *);
				void			*pCallbackData;
				} SOUNDPARMS;


const INT32		SOUND_PARAMS_DEFAULT_SPEED		= -1;
const INT16		SOUND_PARAMS_DEFAULT_VOLUME		= -1;
const INT16		SOUND_PARAMS_DEFAULT_PAN		= -1;
const INT32		SOUND_PARAMS_DEFAULT_LOOP		= -1;
const INT32		SOUND_PARAMS_DEFAULT_PRIORITY	= -1;


// Structure definition for parameters to the random sample playing function
typedef struct {
				INT32			iTimeMin, iTimeMax;
//				UINT32			uiSpeedMin, uiSpeedMax;
				INT16			sVolMin, sVolMax;
				INT16			sPanMin, sPanMax;
				INT32			iPriority;
				INT32			iMaxInstances;
				} RANDOMPARMS;

const INT32		SOUND_PARAMS_DEFAULT_TIME		= -1;
const INT32		SOUND_PARAMS_DEFAULT_INSTANCES	= -1;

// Global startup/shutdown functions
extern BOOLEAN	InitializeSoundManager(void);
extern void		ShutdownSoundManager(void);

// Configuration functions
extern BOOLEAN	SoundSetMemoryLimit(UINT32 uiLimit);
extern BOOLEAN	SoundSetCacheThreshhold(UINT32 uiThreshold);
extern void *   SoundGetDriverHandle( void );

// Master volume control functions
extern void		SoundSetDefaultVolume(UINT32 uiVolume);
extern UINT32	SoundGetDefaultVolume(void);

// Cache control functions
//extern UINT32	SoundLoadSample(STR pFilename);
UINT32	SoundLoadSample(STR pFilename);

extern UINT32	SoundFreeSample(STR pFilename);
extern UINT32	SoundLockSample(STR pFilename);
extern UINT32	SoundUnlockSample(STR pFilename);
extern BOOLEAN	SoundEmptyCache(void);

// Play/service sample functions
extern UINT32	SoundPlay(STR pFilename, SOUNDPARMS *pParms);
extern UINT32	SoundPlayStreamedFile( STR pFilename, SOUNDPARMS *pParms );
extern UINT32	SoundPlayRandom(STR pFilename, RANDOMPARMS *pParms);
extern BOOLEAN	SoundServiceStreams(void);
extern BOOLEAN	SoundServiceRandom(void);

// Sound instance manipulation functions
extern BOOLEAN	SoundStopAll(void);
extern BOOLEAN	SoundStopAllRandom(void);
extern BOOLEAN	SoundStop(UINT32 uiSoundID);
extern BOOLEAN	SoundIsPlaying(UINT32 uiSoundID);
extern BOOLEAN	SoundSetVolume(UINT32 uiSoundID, UINT32 uiVolume);
extern BOOLEAN	SoundSetPan(UINT32 uiSoundID, UINT32 uiPan);
extern UINT32	SoundGetVolume(UINT32 uiSoundID);
extern UINT32	SoundGetPosition(UINT32 uiSoundID);

extern void SoundSetSampleFlags( UINT32 uiSample, UINT32 uiFlags );
extern void SoundRemoveSampleFlags( UINT32 uiSample, UINT32 uiFlags );

extern void SoundEnableSound(BOOLEAN fEnable);

extern void SoundLog(CHAR8 *strMessage);

/*
#ifdef __cplusplus
}
#endif
*/

#endif
