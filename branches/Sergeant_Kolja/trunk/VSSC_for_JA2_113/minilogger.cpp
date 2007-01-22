/* _______________________________________________________
 * minilogger.cpp : currently only a starting platform
 *                  for a (later to be sorted) logger
 *
 * Copyright :
 * (c) 2006, Sergeant_Kolja@yahoo.de (aka Modem-Man)
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

/* _______________________________________________________
 *
 * ToDo:
 *   Doxygen calling conventions
 *   Reading Registry  HKLM/Software/vssc/appname
 *   writing + reading HKCU ...
 *   optional send to UART
 *   LogLevel from Registry
 *   LogLevel for Syslog, Uart, DebugString different
 *   Interface for JA2 Debug Macros
 *
 * _______________________________________________________
 */




/***************************************************************************/
char const * const mod = "minilogger";



/***************************************************************************/


int main(int argc, char* argv[])
{{
  int Handle;

  printf("-----------------------------\r\n");
  Handle = VSSC_open( NULL, -1, -1, NULL );
  //Handle = VSSC_open( "localhost:514", LOG_LOCAL2, LOG_DEBUG, "TestProg" );
  if(!Handle)
    return -1;

  VSSC_Log( Handle, LOG_EMERG  , mod, "Nachricht Typ: 'EMERG'  %u", LOG_EMERG  );
  VSSC_Log( Handle, LOG_ALERT  , mod, "Nachricht Typ: 'ALERT'  %u", LOG_ALERT  );
  VSSC_Log( Handle, LOG_CRIT   , mod, "Nachricht Typ: 'CRIT'   %u", LOG_CRIT   );
  VSSC_Log( Handle, LOG_ERR    , mod, "Nachricht Typ: 'ERR'    %u", LOG_ERR    );
  VSSC_Log( Handle, LOG_WARNING, mod, "Nachricht Typ: 'WARNING'%u", LOG_WARNING);
  VSSC_Log( Handle, LOG_NOTICE , mod, "Nachricht Typ: 'NOTICE' %u", LOG_NOTICE );
  VSSC_Log( Handle, LOG_INFO   , mod, "Nachricht Typ: 'INFO'   %u", LOG_INFO   );
  VSSC_Log( Handle, LOG_DEBUG  , mod, "Nachricht Typ: 'DEBUG'  %u", LOG_DEBUG  );
  VSSC_Log( Handle, LOG_DEBUG  , mod, "Nachricht Typ: 'DEBUG'  %u, %020s, %03u, 0x%08x", LOG_DEBUG, 
                                      "Test-Ausgabe eines Textes...", 77, 88 );
  
  VSSC_close( Handle );
  printf("-----------------------------\r\n");

  
  return 0;
}}

