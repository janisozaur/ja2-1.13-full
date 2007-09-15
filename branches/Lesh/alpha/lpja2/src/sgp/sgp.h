#ifndef __SGP_
#define __SGP_

#include "local.h"
#include "types.h"
#include "debug.h"
#include "video.h"
#include "input.h"

#ifdef __cplusplus
extern "C" {
#endif

extern BOOLEAN		gfApplicationActive;	// FALSE = iconified window
extern BOOLEAN		gfSGPInputReceived;
extern BOOLEAN		gfProgramIsRunning;		// Turn this to FALSE to exit program
extern UINT32		giStartMem;
extern UINT8		gbPixelDepth;			// GLOBAL RUN-TIME SETTINGS
extern BOOLEAN		gfDontUseDDBlits;		// GLOBAL FOR USE OF DD BLITTING

#if !defined(JA2) && !defined(UTILS)
extern BOOLEAN		gfLoadAtStartup;
extern CHAR8		*gzStringDataOverride;
extern BOOLEAN		gfUsingBoundsChecker;
extern BOOLEAN		gfCapturingVideo;

#endif

// function prototypes
BOOLEAN InitializeStandardGamingPlatform(void);
void ShutdownStandardGamingPlatform(void);
void SGPExit(void);
void ShutdownWithErrorBox(CHAR8 *pcMessage);

#ifdef __cplusplus
}
#endif

#endif
