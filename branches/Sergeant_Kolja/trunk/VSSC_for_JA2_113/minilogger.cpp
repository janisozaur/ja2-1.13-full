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
 *  \brief Sample Code for the Very Simple Syslog Client
 *  This file shows the typical usage and may act as test workbench.
 *
 *  \author Sergeant Kolja (Initial Author)<br>
 *
 *  \bug Bugs I found or got reported:
 *  - switch from bind()/sendto() to sendto(), so the Host can be changed on-the-fly
 *    bind() is superflous, it is a remainder of bind()/send() which I changed in the past
 *    to sendto() but forgot the removal of bind().
 *
 *  \todo here follows a list what I think what has to be done in the whole project:
 *  - Doxygen Documentation for all PUBLIC Symbols
 *  - Doxygen Documentation for some PRIVATE things to increase understanding (?)
 *  - Doxygen Footer with JA2-Logo ;-) and Link to JA2.113
 *  - RFC 3164 header implementation check: is pid & host mandantory or required? correction if req'd
 *  - Masking: From that 28 bits, set  some of the MSB 
 *    with some meaningful predefines, while keeping LSB free for cutomizing
 *    f.i. SLOG_ENTRY, SLOG_EXIT, SLOG_DATA, SLOG_SOCK, SLOG_COMM, SLOG_TACT, SLOG_STRAT, 
 *    SLOG_GDI, SLOG_UI, SLOG_RIGHTS, SLOG_FILEIO, SLOG_INVOKE
 *  - Interface for Set_LogLevel, Mask, Host. Proposal: one Setter method with string switch
 *  - Interface for JA2 Debug Macros
 *  - Interface for SDK-Macros like RETAILMSG()
 *  - optional log to UART
 *  - optional log to File
 *  - optional log to SysEventLog
 *  - LogLevel from Registry
 *  - manage different LogLevels for Syslog, Uart, DebugString (Reg-Tree for Each one)
 *  - Interface for WCHAR / TCHAR
 *  - Interface for VSSC_LogSysError() (log a printf text plus full text of GetLastError() result, if not 0)
 *  - convert OEM to Ascii, Convert national char to UTF/Mime
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
  int Handle;

  printf("-----------------------------\r\n");
  Handle = VSSC_open();
  //Handle = VSSC_open2( "localhost:514", LOG_LOCAL2, LOG_DEBUG, "TestProg" );
  if(!Handle)
    return -1;

  VSSC_Log( Handle, SLOG_SILENT , mod, "Nachricht Typ: 'SILENT' %u", SLOG_EMERG  );
  VSSC_Log( Handle, SLOG_EMERG  , mod, "Nachricht Typ: 'EMERG'  %u", SLOG_EMERG  );
  VSSC_Log( Handle, SLOG_ALERT  , mod, "Nachricht Typ: 'ALERT'  %u", SLOG_ALERT  );
  VSSC_Log( Handle, SLOG_CRIT   , mod, "Nachricht Typ: 'CRIT'   %u", SLOG_CRIT   );
  VSSC_Log( Handle, SLOG_ERR    , mod, "Nachricht Typ: 'ERR'    %u", SLOG_ERR    );
  VSSC_Log( Handle, SLOG_WARNING, mod, "Nachricht Typ: 'WARNING'%u", SLOG_WARNING);
  VSSC_Log( Handle, SLOG_NOTICE , mod, "Nachricht Typ: 'NOTICE' %u", SLOG_NOTICE );
  VSSC_Log( Handle, SLOG_INFO   , mod, "Nachricht Typ: 'INFO'   %u", SLOG_INFO   );
  VSSC_Log( Handle, SLOG_DEBUG  , mod, "Nachricht Typ: 'DEBUG'  %u", SLOG_DEBUG  );
  VSSC_Log( Handle, SLOG_DEBUG  , mod, "Nachricht Typ: 'DEBUG'  %u, %020s, %03u, 0x%08x", SLOG_DEBUG, 
                                      "Test-Ausgabe eines Textes...", 77, 88 );
  
  VSSC_close( Handle );
  printf("-----------------------------\r\n");

  
  return 0;
}}
/*! \endcond */



