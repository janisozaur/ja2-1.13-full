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
 *  - sub-struct for syslog outstream and preparation of sub-struct for file-outstream
 *  - prepare to have multiple log output streams
 *  - prepare to have multiple log levels & filters
 *  - outstream-flagfields: if used/registered, the line is only used on the given outstream(s) instead of beeing ORed like the other flagbits
 *  - more power for Controlflags (can on the fly decide if {%02u} counter is used etc.)
 *  - Interface for Config from CmdLine/PrivateProfile/Reg
 *  - LogLevel from Registry
 *  - Reading Registry  HKCU/Software/vssc/%appname% if not found, copy from ...
 *  - Reading Registry  HKLM/Software/vssc/%appname% if not found fill with defaults, if Rights are appropriate
 *  - Interface for On_ConfigChangedEvent()
 *  - Internal On_ConfigChangedPolling() if Polling Time Delta is given (only for System without RegEvent)
 *
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
 *  - manage different LogLevels for Syslog, Uart, DebugString (Reg-Tree for Each one)
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
 *   - Reading Registry  HKLM/Software/vssc/appname
 *   - writing + reading HKCU ...
 *   - Interface for SDK Debug Macros like "IFDBG(svslog_DebugOut(LOG_ERRORS, L"ERROR: buffer length.\n"));"
 *     IFDBG is usually
 *     \code
       #if defined(DEBUG)
       #define IFDBG(x)    x
       #define IFNDBG(x)
       #else
       #define IFDBG(x)
       #define IFNDBG(x)   x
       #endif
       \endcode
 *
 */




/***************************************************************************/
/*! \cond NEVER_DOX */
char const * const mod = "minilogger";
/*! \endcond */



/***************************************************************************/
#ifndef FIELD_OFFSET
/* calc offset in bytes from start of struct up to the given su-element */
#define FIELD_OFFSET(type, field)    ((LONG)&(((type *)0)->field))
#endif

#define MAX_CONFIG_STRINGLEN 80
enum configdata_types 
  { 
   et_BYTE 
  ,et_CHAR 
  ,et_USHORT 
  ,et_SHORT 
  ,et_ULONG 
  ,et_LONG 
  ,et_P32STRING
  };

typedef struct configdata_
  {
  TCHAR const * pGroupName;
  TCHAR const * pKeyName;
  enum configdata_types ValType;
  LONG Offset;
  LONG Size;
  BOOL WriteIfNotExist;
  LONG DefaultVal;
  } TS_configdata, *PTS_configdata;

typedef struct
  {
  size_t LastPos;
  TCHAR Text[MAX_CONFIG_STRINGLEN+1];
  } P32STRING, *PP32STRING;

typedef struct
  {
  /*-------------------------------*/
  struct
    {
    P32STRING gstring1;
    ULONG     Level;
    LONG      NegativeValue;
    } global;
  /*-------------------------------*/
  struct
    {
    P32STRING Portname;
    ULONG     Portnummer;
    } udp;
  /*-------------------------------*/
  struct
    {
    P32STRING Filename;
    ULONG     FileSize;
    } file;
  /*-------------------------------*/
  } CONFIG_BLOCK;


struct HandleS
  {
  int some;
  CONFIG_BLOCK Setup;
  int other;
  };


#define MAKE_CFGSTR( grp, nam, typ, cr, dflt )    \
  {                                               \
   _T(#grp), _T(#nam), et_##typ,                  \
   FIELD_OFFSET( CONFIG_BLOCK, grp.nam.Text ),    \
   MAX_CONFIG_STRINGLEN, (cr!=0), (LONG)_T(dflt)  \
  }

#define MAKE_CFGINT( grp, nam, typ, cr, dflt ) \
  {                                            \
   _T(#grp), _T(#nam), et_##typ,               \
   FIELD_OFFSET( CONFIG_BLOCK, grp.nam ),      \
   sizeof(typ), (cr!=0), (LONG)(dflt)          \
  }

#define MAKE_CFGLEN( a ) do{ a.LastPos = DIM( a.Text )-1; }while(0)

/***************************************************************************/

/*! \cond NEVER_DOX */
int main(int argc, char* argv[])
{{
  const int test_number = 0x10;

  int Handle, iRet;
  char Buffer[1024];
  long lVal;
  char *p;

  /*===========================================================================*
   *
   *         Test #1: open + log with different Levels + close
   *
   *===========================================================================*/
  printf("-----------------------------\r\n");
  Handle = VSSC_open();
  //Handle = VSSC_open2( "localhost:514", LOG_LOCAL2, LOG_DEBUG, "TestProg" );
  if(!Handle)
    return -1;

  if( test_number & 0x1 )
    {
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
    iRet = VSSC_close( Handle );
    printf("-----------------------------\r\n");
    }
  
  /*===========================================================================*
   *
   *         Test #2: open + single get methods
   *
   *===========================================================================*/
  if( test_number & 0x2 )
    {
    Handle = VSSC_open2( "localhost:514", LOG_LOCAL2, SLOG_DEBUG, "TestProg", FALSE );
    if(!Handle)
      return -1;

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
    printf("-----------------------------\r\n");
    }


  /*===========================================================================*
   *
   *         Test #3: single set methods with checking their effect to loglevel
   *
   *===========================================================================*/
  if( test_number & 0x4 )
    {
    iRet = VSSC_SetStr( Handle, "AppName", "Demo-0815" );
    iRet = VSSC_SetInt( Handle, "SyslogLevel", SLOG_INFO, LOGFLGS_SET_VALUE );
    iRet = VSSC_Log( Handle, SLOG_NOTICE , mod, "'NOTICE' MUSS" );
    iRet = VSSC_Log( Handle, SLOG_INFO   , mod, "'INFO'   MUSS" );
    iRet = VSSC_Log( Handle, SLOG_DEBUG  , mod, "'DEBUG'  darf NICHT" );

    iRet = VSSC_SetInt( Handle, "SyslogLevel", SLOG_EMERG, LOGFLGS_SET_VALUE );
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
  
    iRet = VSSC_SetInt( Handle, "SyslogLevel", SLOG_ALERT, LOGFLGS_SET_VALUE );
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
  
    iRet = VSSC_SetInt( Handle, "SyslogLevel", SLOG_SILENT, LOGFLGS_SET_VALUE );
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
  
    iRet = VSSC_SetInt( Handle, "SyslogLevel", SLOG_DEBUGMAX, LOGFLGS_SET_VALUE );
    iRet = VSSC_Log( Handle, SLOG_NOTICE , mod, "'NOTICE' muss" );
    iRet = VSSC_Log( Handle, SLOG_INFO   , mod, "'INFO' muss"   );
    iRet = VSSC_Log( Handle, SLOG_DEBUG  , mod, "'DEBUG' muss" );

    iRet = VSSC_SetStr( Handle, "AppName", "__XX__" );
    iRet = VSSC_SetInt( Handle, "ZoneMask", LOGZONE_MAINLOOP | LOGZONE_ENTRY | LOGZONE_EXIT | LOGZONE_DATA, LOGFLGS_SET_VALUE );
    iRet = VSSC_Log( Handle, SLOG_EMERG | LOGZONE_EVERYTHING, mod, "** LOGZONES** 'EVERYTHING EMERG'  %u", SLOG_EMERG  );
    iRet = VSSC_Log( Handle, SLOG_EMERG | LOGZONE_NOTHING   , mod, "** LOGZONES** 'NOTHING EMERG'  %u", SLOG_EMERG  );
    iRet = VSSC_Log( Handle, SLOG_EMERG | LOGZONE_MAINLOOP  , mod, "** LOGZONES** 'MAINLOOP EMERG'  %u", SLOG_EMERG  );

    iRet = VSSC_SetInt( Handle, "ZoneMask", LOGZONE_DEFAULT, LOGFLGS_DEFAULT );
    iRet = VSSC_Log( Handle, SLOG_EMERG | LOGZONE_MAINLOOP, mod, "** show **" );
    iRet = VSSC_SetInt( Handle, "ZoneMask", LOGZONE_MAINLOOP, LOGFLGS_CLR_BITS );
    iRet = VSSC_Log( Handle, SLOG_EMERG | LOGZONE_MAINLOOP, mod, "** dont show **" );
    iRet = VSSC_SetInt( Handle, "ZoneMask", LOGZONE_MAINLOOP, LOGFLGS_TOGGLE_BITS );
    iRet = VSSC_Log( Handle, SLOG_EMERG | LOGZONE_MAINLOOP, mod, "** show **" );
    iRet = VSSC_SetInt( Handle, "ZoneMask", LOGZONE_MAINLOOP, LOGFLGS_CLR_BITS );
    iRet = VSSC_SetInt( Handle, "ZoneMask", LOGZONE_MAINLOOP, LOGFLGS_SET_BITS );
    iRet = VSSC_Log( Handle, SLOG_EMERG | LOGZONE_MAINLOOP, mod, "** show **" );

    printf("-----------------------------\r\n");
    }

  /*===========================================================================*
   *
   *         Test #4: single set methods with flexible numerical values
   *                  bunch-at-once set methods
   *
   *===========================================================================*/
  if( test_number & 0x8 )
    {
    iRet = VSSC_SetStr( Handle, "ZoneMask", "0xAFFE" );
    iRet = VSSC_SetStr( Handle, "ZoneMask", "$CAFFE" );
    iRet = VSSC_SetStr( Handle, "ZoneMask", "&HBEAF21" );
    iRet = VSSC_SetStr( Handle, "ZoneMask", "-1" );
    iRet = VSSC_SetStr( Handle, "ZoneMask", "0b10000000100000001111111101010101" );

    iRet = VSSC_SetAll( Handle, "" );
    iRet = VSSC_SetAll( Handle, "SyslogLevel=5\0\0" );

    char MultiLine[]="ZoneMask=0x0AFFE100\r\n"
                     "# a comment \n"
                     " SyslogHost = 127.0.0.1  \t  \n\r"
                     "\t  SyslogPort = 514 \r"
                     "  ; another comment \n"
                     "Unknown Switch=unknown Value\r\n"
                     "SyslogBindPort=  0d0\r\n"
                     "SyslogBindAddr=0.0.0.0\r\n"
                     "SyslogFacility=174\r"
                     "\n"
                     "SyslogLevel=5";
    char Stringlist[]="ZoneMask=0x0AFFE100\0"
                      "# a comment \0"
                      " SyslogHost = 127.0.0.1  \t  \n\0"
                      "\t  SyslogPort = 514 \0"
                      "  ; another comment \0"
                      "Unknown Switch=unknown Value\0"
                      "SyslogBindPort=  0d0\r\0"
                     "SyslogBindAddr=0.0.0.0\0"
                     "SyslogFacility=174\0"
                     "SyslogLevel=5\0\0";

    iRet = VSSC_SetAll( Handle, MultiLine );
    iRet = VSSC_SetAll( Handle, Stringlist );
    printf("-----------------------------\r\n");
    }

  /*===========================================================================*
   *
   *         Test #5: Reading Setup at once
   *
   *===========================================================================*/
  if( test_number & 0x10 )
    {
    int i;
    struct HandleS OurDemoHandle={0};

    MAKE_CFGLEN( OurDemoHandle.Setup.global.gstring1 );
    MAKE_CFGLEN( OurDemoHandle.Setup.udp.Portname );
    MAKE_CFGLEN( OurDemoHandle.Setup.file.Filename );

    /*localBlock(), needed for ANSI-C-Compliance */
      {
      TS_configdata cfg[] = 
        {
        /*==============================================================*/
         MAKE_CFGSTR( global, gstring1     , P32STRING, 1, "default string" )
        ,MAKE_CFGINT( global, Level        , ULONG    , 1, 889900L          )
        ,MAKE_CFGINT( global, NegativeValue, LONG     , 0, -123L            )
        /*==============================================================*/
        ,MAKE_CFGSTR( udp, Portname        , P32STRING, TRUE, "localhost"   )
        ,MAKE_CFGINT( udp, Portnummer      , ULONG    , TRUE, 514           )
        /*==============================================================*/
        ,MAKE_CFGSTR( file, Filename       , P32STRING, TRUE, "logfile.txt" )
        ,MAKE_CFGINT( file, FileSize       , ULONG    , TRUE, 120*1024      )
        /*==============================================================*/
        };
      iRet = VSSC_SetInt( Handle, "SyslogLevel", SLOG_DEBUGMAX, LOGFLGS_SET_VALUE );

      iRet = VSSC_Log( Handle, SLOG_INFO, mod, "--------------{BEFORE}-------------------" );
      iRet = VSSC_Log( Handle, SLOG_INFO, mod, "global.gstring1.Text='%s'", OurDemoHandle.Setup.global.gstring1.Text );
      iRet = VSSC_Log( Handle, SLOG_INFO, mod, "global.Level        =%lu" , OurDemoHandle.Setup.global.Level );
      iRet = VSSC_Log( Handle, SLOG_INFO, mod, "global.NegativeValue=%ld" , OurDemoHandle.Setup.global.NegativeValue );
      iRet = VSSC_Log( Handle, SLOG_INFO, mod, "udp.Portname.Text   ='%s'", OurDemoHandle.Setup.udp.Portname.Text );
      iRet = VSSC_Log( Handle, SLOG_INFO, mod, "udp.Portnummer      =%lu" , OurDemoHandle.Setup.udp.Portnummer );
      iRet = VSSC_Log( Handle, SLOG_INFO, mod, "file.Filename.Text  ='%s'", OurDemoHandle.Setup.file.Filename.Text );
      iRet = VSSC_Log( Handle, SLOG_INFO, mod, "file.FileSize       =%lu" , OurDemoHandle.Setup.file.FileSize );

      for( i=0; i<DIM(cfg); i++ )
        {
        /* HandleS.Setup */
        switch( cfg[i].ValType )
          {
          case et_BYTE  :
          case et_CHAR  :
          case et_USHORT: 
          case et_SHORT :
          case et_ULONG :
          case et_LONG  :
          /* more numerical types can be inserted here ... */
            {
            char *pHelp;
            ULONG  *pStart_UL;
            LONG   *pStart_SL;
            BYTE   *pStart_UC;
            CHAR   *pStart_SC;
            USHORT *pStart_US;
            SHORT  *pStart_SS;
            /* ... and more numerical types can be inserted here ... */

            iRet = VSSC_Log( Handle, SLOG_INFO, mod, 
                             "now reading Type %u of Group='%s', Key='%s'. Dflt/u=%lu, Dflt/s=%ld, writeNew=%u. %u bytes can be written at offs %u", 
                             cfg[i].ValType,
                             cfg[i].pGroupName,
                             cfg[i].pKeyName,
                             (ULONG) cfg[i].DefaultVal,
                             (LONG ) cfg[i].DefaultVal,
                             cfg[i].WriteIfNotExist,
                             cfg[i].Size,
                             cfg[i].Offset );
            /*real reading / writing not implemented yet */

            pHelp  = (char*) &OurDemoHandle.Setup;
            pHelp += cfg[i].Offset;
            pStart_UL = (ULONG *) pHelp;
            pStart_SL = (LONG  *) pHelp;
            pStart_UC = (BYTE  *) pHelp;
            pStart_SC = (CHAR  *) pHelp;
            pStart_US = (USHORT*) pHelp;
            pStart_SS = (SHORT *) pHelp;
            /* ... and more numerical types can be inserted here ... */

            switch( cfg[i].ValType )
              {
              case et_BYTE  : *pStart_UC = (BYTE  ) cfg[i].DefaultVal; break;
              case et_CHAR  : *pStart_SC = (CHAR  ) cfg[i].DefaultVal; break;
              case et_USHORT: *pStart_US = (USHORT) cfg[i].DefaultVal; break;
              case et_SHORT : *pStart_SS = (SHORT ) cfg[i].DefaultVal; break;
              case et_ULONG : *pStart_UL = (ULONG ) cfg[i].DefaultVal; break;
              case et_LONG  : *pStart_SL = (LONG  ) cfg[i].DefaultVal; break;
              /* ... and, finally more numerical types must be inserted here. */
              }
            };break;

          case et_P32STRING:
            {
            size_t maxlen;
            char *pStart, *pEnd;

            maxlen = MIN( (size_t) cfg[i].Size, OurDemoHandle.Setup.global.gstring1.LastPos );
            iRet = VSSC_Log( Handle, SLOG_INFO, mod, 
                             "now reading Type %u of Group='%s', Key='%s'. Dflt=%s, writeNew=%u. %u bytes can be written at offs %u", 
                             cfg[i].ValType,
                             cfg[i].pGroupName,
                             cfg[i].pKeyName,
                             (TCHAR*) cfg[i].DefaultVal,
                             cfg[i].WriteIfNotExist,
                             cfg[i].Size,
                             cfg[i].Offset );
            /*real reading / writing not implemented yet */
            pStart = (char*) &OurDemoHandle.Setup;
            pStart+= cfg[i].Offset;
            pEnd   = pStart + (sizeof(TCHAR)*(maxlen));
            _tcsncpy( (TCHAR*) pStart, (TCHAR*) (cfg[i].DefaultVal), maxlen );
            *((TCHAR*)(pEnd)) = 0;
            };break;
          } /*sw-end*/
        } /*for-end*/ 
      iRet = VSSC_Log( Handle, SLOG_INFO, mod, "--------------{AFTER}-------------------" );
      iRet = VSSC_Log( Handle, SLOG_INFO, mod, "global.gstring1.Text='%s'", OurDemoHandle.Setup.global.gstring1.Text );
      iRet = VSSC_Log( Handle, SLOG_INFO, mod, "global.Level        =%lu" , OurDemoHandle.Setup.global.Level );
      iRet = VSSC_Log( Handle, SLOG_INFO, mod, "global.NegativeValue=%ld" , OurDemoHandle.Setup.global.NegativeValue );
      iRet = VSSC_Log( Handle, SLOG_INFO, mod, "udp.Portname.Text   ='%s'", OurDemoHandle.Setup.udp.Portname.Text );
      iRet = VSSC_Log( Handle, SLOG_INFO, mod, "udp.Portnummer      =%lu" , OurDemoHandle.Setup.udp.Portnummer );
      iRet = VSSC_Log( Handle, SLOG_INFO, mod, "file.Filename.Text  ='%s'", OurDemoHandle.Setup.file.Filename.Text );
      iRet = VSSC_Log( Handle, SLOG_INFO, mod, "file.FileSize       =%lu" , OurDemoHandle.Setup.file.FileSize );
      } /*localBlock-end*/
    printf("-----------------------------\r\n");
    }


  
  iRet = VSSC_close( Handle );
  printf("-----------------------------\r\n");
  return 0;
}}
/*! \endcond */



