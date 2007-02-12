// resume: file is almost useless and pending of removal
//         same about timer.h
//-------------------------------------------------------
#ifdef JA2_PRECOMPILED_HEADERS
	#include "JA2 SGP ALL.H"
#elif defined( WIZ8_PRECOMPILED_HEADERS )
	#include "WIZ8 SGP ALL.H"
#else
	#include "types.h"
	#include <windows.h>
	#if defined( JA2 ) || defined( UTIL )
		#include "video.h"
	#else
		#include "video2.h"
	#endif
	#include "timer.h"
#endif

#ifndef WIN32_LEAN_AND_MEAN
	#define WIN32_LEAN_AND_MEAN
#endif

UINT32 guiStartupTime;	// local use
UINT32 guiCurrentTime;	// local use

// used locally
void CALLBACK Clock( HWND hWindow, UINT uMessage, UINT idEvent, DWORD dwTime )
{
  guiCurrentTime = GetTickCount();
  if (guiCurrentTime < guiStartupTime)
  { // Adjust guiCurrentTime because of loopback on the timer value
    guiCurrentTime = guiCurrentTime + (0xffffffff - guiStartupTime);
  }
  else
  { // Adjust guiCurrentTime because of loopback on the timer value
    guiCurrentTime = guiCurrentTime - guiStartupTime;
  }
}

// called from sgp.cpp
BOOLEAN InitializeClockManager(void)
{

  // Register the start time (use WIN95 API call)
  guiCurrentTime = guiStartupTime = GetTickCount();
  SetTimer(ghWindow, MAIN_TIMER_ID, 10, (TIMERPROC)Clock);


  return TRUE;
}

// called from sgp.cpp
void    ShutdownClockManager(void)
{

  // Make sure we kill the timer
  KillTimer(ghWindow, MAIN_TIMER_ID);

}

// called in mousesystem.cpp ONLY
TIMER   GetClock(void)
{
  return guiCurrentTime;
}

// not used at all
TIMER   SetCountdownClock(UINT32 uiTimeToElapse)
{
  return (guiCurrentTime + uiTimeToElapse);
}

// not used at all
UINT32 ClockIsTicking(TIMER uiTimer)
{
  if (uiTimer > guiCurrentTime)
  { // Well timer still hasn't elapsed
    return (uiTimer - guiCurrentTime);
  }
  // Time's up
  return 0;
}
