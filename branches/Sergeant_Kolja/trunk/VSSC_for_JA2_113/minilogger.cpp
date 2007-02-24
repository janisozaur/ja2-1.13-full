/* _______________________________________________________
 * minilogger.cpp : currently only a starting platform
 *                  for a (later to be sorted) logger
 *
 * Copyright :
 * (c) 2006, SergeantKolja(at)yahoo.de (aka Modem-Man)
 * all rights reserved
 * 
 * don't forget to include license.txt in every 
 * distribution of this files. Event, don't forget to read 
 * them.
 * _______________________________________________________
 */

#include "stdafx.h"
#include "vssc.h"

#include <windows.h>
#include <stdlib.h>
#include <tchar.h>
#include <assert.h>

/*! \file minilogger.cpp
 *
 *  \brief Sample Code for the Very Simple Syslog Client.
 *  This file shows the typical usage and may act as a test workbench.
 *
 *  \author Sergeant Kolja (Initial Author)<br>
 *
 *  \bug Bugs I found or got reported:
 *  - currently none
 *
 *  \todo here follows a list what I think what has to be done in the whole project:
 *  - Interface for VSSC_LogSysError() (log a printf text plus full text of GetLastError() result, if not 0)
 *  - Interface for JA2 Debug Macros
 *  - Interface for SDK-Macros like RETAILMSG()
 *  - Interface for WCHAR / TCHAR
 *  - convert OEM to Ascii, Convert national char to UTF/Mime
 *  - Doxygen Documentation for remaining PUBLIC Symbols
 *  - Doxygen Footer with JA2-Logo ;-) and Link to JA2.113
 *  - optional log to UART
 *  - optional log to File
 *  - optional log to SysEventLog
 *  - LogLevel from Registry
 *  - manage different LogLevels for Syslog, Uart, DebugString (Reg-Tree for Each one)
 *  - Reading Registry  HKCU/Software/vssc/%appname% if not found, copy from ...
 *  - Reading Registry  HKLM/Software/vssc/%appname% if not found fill with defaults, if Rights are appropriate
 *  - Interface for On_ConfigChangedEvent()
 *  - Internal On_ConfigChangedPolling() if Polling Time Delta is given (only for System without RegEvent)
 *  - Registry Changed Event
 *  - Implement Second Part of RFC 3164: sending XML-like Datagrams over TCP
 *  - Porting to WinCE3..6, (it is mostly Training and for possibly re-use this module at my employer)
 *  - Porting to ***nix, altough it is not required there, its a good training
 *  - Registry Tree for Setup, like:
 *  \code
    Reg\Syslog\Url=UDP://localhost:514
            \Level=5
             \Mask=FFFFFF
    Reg\Uart\Url=COM1://19200,8n1
          \Level=3
           \Mask=A00C100
    Reg\File\Url=FILE://C:\Temp\Log.txt
          \Level=1
           \Mask=000002
 *  \endcode
 *
 */




/***************************************************************************/
/*! \cond NEVER_DOX */
char const * const mod = "minilogger";
/*! \endcond */



/***************************************************************************/


/*! \cond NEVER_DOX */
int main(int argc, char* argv[])
{{
  int Handle, iRet;
  char Buffer[1024];
  long lVal;
  char *p;

  printf("-----------------------------\r\n");
  Handle = VSSC_open();
  //Handle = VSSC_open2( "localhost:514", LOG_LOCAL2, LOG_DEBUG, "TestProg" );
  if(!Handle)
    return -1;

  iRet = VSSC_Log( Handle, SLOG_SILENT , mod, "Nachricht Typ: 'SILENT' %u", SLOG_EMERG  );
  iRet = VSSC_Log( Handle, SLOG_EMERG  , mod, "Nachricht Typ: 'EMERG'  %u", SLOG_EMERG  );
  iRet = VSSC_Log( Handle, SLOG_ALERT  , mod, "Nachricht Typ: 'ALERT'  %u", SLOG_ALERT  );
  iRet = VSSC_Log( Handle, SLOG_CRIT   , mod, "Nachricht Typ: 'CRIT'   %u", SLOG_CRIT   );
  iRet = VSSC_Log( Handle, SLOG_ERR    , mod, "Nachricht Typ: 'ERR'    %u", SLOG_ERR    );
  iRet = VSSC_Log( Handle, SLOG_WARNING, mod, "Nachricht Typ: 'WARNING'%u", SLOG_WARNING);
  iRet = VSSC_Log( Handle, SLOG_NOTICE , mod, "Nachricht Typ: 'NOTICE' %u", SLOG_NOTICE );
  iRet = VSSC_Log( Handle, SLOG_INFO   , mod, "Nachricht Typ: 'INFO'   %u", SLOG_INFO   );
  iRet = VSSC_Log( Handle, SLOG_DEBUG  , mod, "Nachricht Typ: 'DEBUG'  %u", SLOG_DEBUG  );
  iRet = VSSC_Log( Handle, SLOG_DEBUG  , mod, "Nachricht Typ: 'DEBUG'  %u, %020s, %03u, 0x%08x", SLOG_DEBUG, 
                                      "Test-Ausgabe eines Textes...", 77, 88 );
  
  printf("-----------------------------\r\n");
  iRet = VSSC_GetValidKeyNames( Buffer, DIM(Buffer) );
  printf("Valid Keys are:\r\n%s\r\n", Buffer);
  iRet = VSSC_Log( Handle, SLOG_INFO, mod, "Valid Keys are: %s", Buffer ); 

  p = "Key";
  iRet = VSSC_GetStr( Handle, p, Buffer, DIM(Buffer) );
  iRet = VSSC_Log( Handle, SLOG_INFO, mod, "'%s' is '%s'", p, Buffer ); 
  p = "Version";
  iRet = VSSC_GetStr( Handle, p, Buffer, DIM(Buffer) );
  iRet = VSSC_Log( Handle, SLOG_INFO, mod, "'%s' is '%s'", p, Buffer ); 
  p = "AppName";
  iRet = VSSC_GetStr( Handle, p, Buffer, DIM(Buffer) );
  iRet = VSSC_Log( Handle, SLOG_INFO, mod, "'%s' is '%s'", p, Buffer ); 
  p = "SyslogHost";
  iRet = VSSC_GetStr( Handle, p, Buffer, DIM(Buffer) );
  iRet = VSSC_Log( Handle, SLOG_INFO, mod, "'%s' is '%s'", p, Buffer ); 
  p = "SyslogBindAddr";
  iRet = VSSC_GetStr( Handle, p, Buffer, DIM(Buffer) );
  iRet = VSSC_Log( Handle, SLOG_INFO, mod, "'%s' is '%s'", p, Buffer ); 

  p = "Key";
  iRet = VSSC_GetInt( Handle, p, &lVal );
  iRet = VSSC_Log( Handle, SLOG_INFO, mod, "'%s'=%ld (0x%08x)", p, lVal, lVal ); 
  p = "Version";
  iRet = VSSC_GetInt( Handle, p, &lVal );
  iRet = VSSC_Log( Handle, SLOG_INFO, mod, "'%s'=%ld (0x%08x)", p, lVal, lVal ); 
  p = "ZoneMask";
  iRet = VSSC_GetInt( Handle, p, &lVal );
  iRet = VSSC_Log( Handle, SLOG_INFO, mod, "'%s'=%ld (0x%08x)", p, lVal, lVal ); 
  p = "SyslogPort";
  iRet = VSSC_GetInt( Handle, p, &lVal );
  iRet = VSSC_Log( Handle, SLOG_INFO, mod, "'%s'=%ld (0x%08x)", p, lVal, lVal ); 
  p = "SyslogBindPort";
  iRet = VSSC_GetInt( Handle, p, &lVal );
  iRet = VSSC_Log( Handle, SLOG_INFO, mod, "'%s'=%ld (0x%08x)", p, lVal, lVal ); 
  p = "SyslogFacility";
  iRet = VSSC_GetInt( Handle, p, &lVal );
  iRet = VSSC_Log( Handle, SLOG_INFO, mod, "'%s'=%ld (0x%08x)", p, lVal, lVal ); 
  p = "SyslogLevel";
  iRet = VSSC_GetInt( Handle, p, &lVal );
  iRet = VSSC_Log( Handle, SLOG_INFO, mod, "'%s'=%ld (0x%08x)", p, lVal, lVal ); 


  iRet = VSSC_SetStr( Handle, "AppName", "Demo-0815" );

  
  iRet = VSSC_SetInt( Handle, "SyslogLevel", SLOG_INFO );
  iRet = VSSC_Log( Handle, SLOG_NOTICE , mod, "'NOTICE' MUSS" );
  iRet = VSSC_Log( Handle, SLOG_INFO   , mod, "'INFO'   MUSS" );
  iRet = VSSC_Log( Handle, SLOG_DEBUG  , mod, "'DEBUG'  darf NICHT" );

  iRet = VSSC_SetInt( Handle, "SyslogLevel", SLOG_EMERG );
  iRet = VSSC_Log( Handle, SLOG_SILENT , mod, "'SILENT' soll nicht " );
  iRet = VSSC_Log( Handle, SLOG_EMERG  , mod, "'EMERG'  MUSS" );
  iRet = VSSC_Log( Handle, SLOG_ALERT  , mod, "'ALERT'  darf nicht" );
  iRet = VSSC_Log( Handle, SLOG_CRIT   , mod, "'CRIT'   darf nicht" );
  iRet = VSSC_Log( Handle, SLOG_ERR    , mod, "'ERR'    darf nicht" );
  iRet = VSSC_Log( Handle, SLOG_WARNING, mod, "'WARNING'darf nicht" );
  iRet = VSSC_Log( Handle, SLOG_NOTICE , mod, "'NOTICE' darf nicht" );
  iRet = VSSC_Log( Handle, SLOG_INFO   , mod, "'INFO'   darf nicht" );
  iRet = VSSC_Log( Handle, SLOG_DEBUG  , mod, "'DEBUG'  darf nicht" );
  iRet = VSSC_Log( Handle, SLOG_DEBUG2 , mod, "'DEBUG2' darf nicht" );
  
  iRet = VSSC_SetInt( Handle, "SyslogLevel", SLOG_ALERT );
  iRet = VSSC_Log( Handle, SLOG_SILENT , mod, "'SILENT' soll nicht " );
  iRet = VSSC_Log( Handle, SLOG_EMERG  , mod, "'EMERG'  MUSS" );
  iRet = VSSC_Log( Handle, SLOG_ALERT  , mod, "'ALERT'  MUSS" );
  iRet = VSSC_Log( Handle, SLOG_CRIT   , mod, "'CRIT'   darf nicht" );
  iRet = VSSC_Log( Handle, SLOG_ERR    , mod, "'ERR'    darf nicht" );
  iRet = VSSC_Log( Handle, SLOG_WARNING, mod, "'WARNING'darf nicht" );
  iRet = VSSC_Log( Handle, SLOG_NOTICE , mod, "'NOTICE' darf nicht" );
  iRet = VSSC_Log( Handle, SLOG_INFO   , mod, "'INFO'   darf nicht" );
  iRet = VSSC_Log( Handle, SLOG_DEBUG  , mod, "'DEBUG'  darf nicht" );
  iRet = VSSC_Log( Handle, SLOG_DEBUG2 , mod, "'DEBUG2' darf nicht" );
  
  iRet = VSSC_SetInt( Handle, "SyslogLevel", SLOG_SILENT );
  iRet = VSSC_Log( Handle, SLOG_SILENT , mod, "'SILENT' soll nicht" );
  iRet = VSSC_Log( Handle, SLOG_EMERG  , mod, "'EMERG'  darf nicht" );
  iRet = VSSC_Log( Handle, SLOG_ALERT  , mod, "'ALERT'  darf nicht" );
  iRet = VSSC_Log( Handle, SLOG_CRIT   , mod, "'CRIT'   darf nicht" );
  iRet = VSSC_Log( Handle, SLOG_ERR    , mod, "'ERR'    darf nicht" );
  iRet = VSSC_Log( Handle, SLOG_WARNING, mod, "'WARNING'darf nicht" );
  iRet = VSSC_Log( Handle, SLOG_NOTICE , mod, "'NOTICE' darf nicht" );
  iRet = VSSC_Log( Handle, SLOG_INFO   , mod, "'INFO'   darf nicht" );
  iRet = VSSC_Log( Handle, SLOG_DEBUG  , mod, "'DEBUG'  darf nicht" );
  iRet = VSSC_Log( Handle, SLOG_DEBUG2 , mod, "'DEBUG2' darf nicht" );
  
  iRet = VSSC_SetInt( Handle, "SyslogLevel", SLOG_DEBUGMAX );
  iRet = VSSC_Log( Handle, SLOG_NOTICE , mod, "'NOTICE' muss" );
  iRet = VSSC_Log( Handle, SLOG_INFO   , mod, "'INFO' muss"   );
  iRet = VSSC_Log( Handle, SLOG_DEBUG  , mod, "'DEBUG' muss" );


  iRet = VSSC_SetStr( Handle, "AppName", "__XX__" );
  iRet = VSSC_SetInt( Handle, "ZoneMask", LOGZONE_MAINLOOP | LOGZONE_ENTRY | LOGZONE_EXIT | LOGZONE_DATA );

  iRet = VSSC_Log( Handle, SLOG_EMERG | LOGZONE_EVERYTHING, mod, "** LOGZONES** 'EVERYTHING EMERG'  %u", SLOG_EMERG  );
  iRet = VSSC_Log( Handle, SLOG_EMERG | LOGZONE_NOTHING   , mod, "** LOGZONES** 'NOTHING EMERG'  %u", SLOG_EMERG  );
  iRet = VSSC_Log( Handle, SLOG_EMERG | LOGZONE_MAINLOOP  , mod, "** LOGZONES** 'MAINLOOP EMERG'  %u", SLOG_EMERG  );
  
  iRet = VSSC_close( Handle );
  printf("-----------------------------\r\n");

  
  return 0;
}}
/*! \endcond */



