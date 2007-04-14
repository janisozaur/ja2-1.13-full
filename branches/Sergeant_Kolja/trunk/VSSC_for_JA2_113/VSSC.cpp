/* _______________________________________________________
 * VSSC.cpp : a very Simple Sylog Client
 *
 * Copyright :
 * (c) 2006, SergeantKolja(at)yahoo.de (aka Modem-Man)
 * all rights reserved
 * 
 * don't forget to include license.txt in every 
 * distribution of this files. 
 * Even, don't forget to read it. ;-)
 * _______________________________________________________
 */

#include "stdafx.h" /* we expect to have all global & common project settings in this file */
#include "vssc.h"

#include <windows.h>
#include <winsock2.h>
#include <stdlib.h>
#include <tchar.h>
#include <assert.h>
#include <time.h>
#include <limits.h>

#include <sys/types.h> /* need for stat */
#include <sys/stat.h>  /* need for stat */

#include <shlobj.h> /* for home directories under Win32 */
#ifndef CSIDL_COMMON_APPDATA
#define CSIDL_COMMON_APPDATA (0x0023)
#endif

char const * const VsscVersion = "0.1.750.build";

/*! \file VSSC.cpp
 *  \brief VSSC basic functions interface
 *
 *  Details following below:
 *
 * \author Sergeant Kolja (Initial Author)<br>
 *
 * \todo here follows a list what I think what has to be done in this file:
 *   - LogLevel from Registry
 *
 */




#ifdef __cplusplus
  extern "C" {
#endif


/*========================================================*/
/*== internal Helper structures                         ==*/
/*========================================================*/

/*! \cond NEVER_DOX */
#ifndef FIELD_OFFSET
/* calc offset in bytes from start of struct up to the given su-element */
#define FIELD_OFFSET(type, field)    ((LONG)&(((type *)0)->field))
#endif

#define MAX_CONFIG_STRINGLEN 128  /* no argument can exceed this lenght. otherwise we mus use dynamic CfgString */
#define MAKE_CFGLEN( a ) do{ a.LastPos = DIM( a.Text )-1; }while(0)
#define MAKE_CFGSTR( grp, nam, typ, cr, dflt )    \
  {                                               \
   _T(#grp), _T(#nam), et_##typ,                  \
   FIELD_OFFSET( CONFIG_BLOCK, grp.nam.Text ),    \
   MAX_CONFIG_STRINGLEN, (cr), (LONG)_T(dflt)     \
  }

#define MAKE_CFGINT( grp, nam, typ, cr, dflt ) \
  {                                            \
   _T(#grp), _T(#nam), et_##typ,               \
   FIELD_OFFSET( CONFIG_BLOCK, grp.nam ),      \
   sizeof(typ), (cr),    (LONG)(dflt)          \
  }

/* no other types are currently allowed. 
 * This can be extended, but there is no real use for more types
 * Be aware: 'et_' is a suffix. the prefix MUST be a predefined type
 */
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

#define INI_LOCATION_EXE        0 /* same path as the Application    */
#define INI_LOCATION_USRAPPDATA 1 /* Users home directory            */
#define INI_LOCATION_ALLUSRAPPD 2 /* Common All Users Home directory */
#define INI_LOCATIONS           (1+INI_LOCATION_ALLUSRAPPD)


#define _SLOG_CREATE_NOT          0
#define _SLOG_CREATE_INIEXE     0x1
#define _SLOG_CREATE_INIHOME    0x2
#define _SLOG_CREATE_INIALLHOME 0x4
#define _SLOG_CREATE_REGHKCU    0x8
#define _SLOG_CREATE_BOTH  ( _SLOG_CREATE_INIEXE | _SLOG_CREATE_REGHKCU )
#define _SLOG_CREATE_REGHKLM   0x10 /* usually not used */


/* a structure which holds auto-reading config values */
typedef struct configdata_
  {
  TCHAR const * pGroupName;
  TCHAR const * pKeyName;
  enum configdata_types ValType;
  LONG Offset;
  LONG Size;
  USHORT WriteIfNotExist;
  LONG DefaultVal;
  } TS_configdata, *PTS_configdata;

/* a string which holds a static known number of elements */
typedef struct
  {
  size_t LastPos;
  TCHAR Text[MAX_CONFIG_STRINGLEN+1]; /* don't bother me with termchar! */
  }  P32STRING, *PP32STRING;

/*! \endcond */

/*========================================================*/
/*== internal Handle structure                          ==*/
/*========================================================*/

/*! \cond NEVER_DOX */
typedef struct vssc_data_
  {
  int            SecCookie1;
  /* ------[forbidden to set]---- */
  struct
    {
    WSADATA        wsaData;
    SOCKET         SndSocket;
    SOCKADDR_IN    SndAddr;
    SOCKADDR_IN    OwnAddr;
    HANDLE         hMutex;
    unsigned short MsgCount;
    DWORD          UserPID;
    TCHAR *        pMyCmdLine;
    TCHAR          Ini_File[INI_LOCATIONS][MAX_PATH];
    } internal;
  /* --------------------------- */
  struct
    {
    TCHAR          tcsProfileName[MAX_PATH];
    } helper;
  /* ------[direct setables]---- */
  struct
    {
    P32STRING      AppName;
    ULONG          ZoneMask;
    USHORT         ControlFlags;
    USHORT         ReEntranceBlock;
    } global;
  struct
    {
    USHORT         Use;
    USHORT         Syslog_Facility;
    USHORT         Syslog_MaxLevel;
    P32STRING      SendTo_Addr;
    USHORT         SendTo_Port;
    USHORT         From_Port;
    } udp;
  struct
    {
    USHORT         Use;
    } file;
  /* --------------------------- */
  int            SecCookie2;
  } CONFIG_BLOCK, TS_vssc_data, *PTS_vssc_data;
/*! \endcond */



/*! \cond NEVER_DOX */
static TS_configdata AutoConfig[] = 
  {
  /*====================================================================================*/
  /* due to some restrictions, this Value MUST be the first we set!                     */
   MAKE_CFGSTR( global, AppName        , P32STRING, _SLOG_CREATE_NOT, _T("0815.exe")     )
  /*====================================================================================*/
  ,MAKE_CFGINT( global, ZoneMask       , ULONG    , _SLOG_CREATE_REGHKCU, 7              )
  ,MAKE_CFGINT( global, ControlFlags   , USHORT   , _SLOG_CREATE_REGHKCU, 0              )
  ,MAKE_CFGINT( global, ReEntranceBlock, USHORT   , _SLOG_CREATE_REGHKCU, 0              )
  /*====================================================================================*/
  ,MAKE_CFGINT( udp   , Use            , USHORT   , _SLOG_CREATE_BOTH   , 0              )
  ,MAKE_CFGINT( udp   , Syslog_Facility, USHORT   , _SLOG_CREATE_REGHKCU, 0              )
  ,MAKE_CFGINT( udp   , Syslog_MaxLevel, USHORT   , _SLOG_CREATE_INIEXE , 7              )
  ,MAKE_CFGSTR( udp   , SendTo_Addr    , P32STRING, _SLOG_CREATE_INIEXE , _T("localhost"))
  ,MAKE_CFGINT( udp   , SendTo_Port    , USHORT   , _SLOG_CREATE_INIEXE , 514            )
  ,MAKE_CFGINT( udp   , From_Port      , USHORT   , _SLOG_CREATE_NOT    , 0              )
  /*====================================================================================*/
  ,MAKE_CFGINT( file  , Use            , USHORT   , _SLOG_CREATE_REGHKCU, 0              )
  /*====================================================================================*/
  };
/*! \endcond */


/*! \cond NEVER_DOX */
typedef struct Settings_
  {
  char const * pKey;
  short AccessToken;
  short StringType;
  } TS_Settings, *PTS_Settings;
/*! \endcond */


/*! \cond NEVER_DOX */
#define CTRL_GENERAL_VERSION   0
#define CTRL_GENERAL_APPNAME   1
#define CTRL_GENERAL_ZONEMASK  2
#define CTRL_SYSLOG_FACILITY   3  
#define CTRL_SYSLOG_MAXLEVEL   4
#define CTRL_SYSLOG_HOST       5
#define CTRL_SYSLOG_PORT       6
#define CTRL_SYSLOG_OUTPORT    7    
#define CTRL_SYSLOG_OUTBIND    8
/*! \endcond */




/*! \cond NEVER_DOX */
static
TS_Settings ControlHash[] = 
  {
     { "Version"        , CTRL_GENERAL_VERSION , 1 }
    ,{ "AppName"        , CTRL_GENERAL_APPNAME , 1 } 
    ,{ "ZoneMask"       , CTRL_GENERAL_ZONEMASK, 0 } 
    ,{ "SyslogHost"     , CTRL_SYSLOG_HOST     , 1 } 
    ,{ "SyslogPort"     , CTRL_SYSLOG_PORT     , 0 } 
    ,{ "SyslogBindPort" , CTRL_SYSLOG_OUTPORT  , 0 }  
    ,{ "SyslogBindAddr" , CTRL_SYSLOG_OUTBIND  , 1 } 
    ,{ "SyslogFacility" , CTRL_SYSLOG_FACILITY , 0 }   
    ,{ "SyslogLevel"    , CTRL_SYSLOG_MAXLEVEL , 0 } 
    ,{ NULL,              -1                   , 0 } 
  };
/*! \endcond */



/*! \cond NEVER_DOX */
#if !defined( LOG_EMERG )
#  define LOG_EMERG    0
#  define LOG_ALERT    1
#  define LOG_CRIT     2
#  define LOG_ERR      3
#  define LOG_WARNING  4
#  define LOG_NOTICE   5
#  define LOG_INFO     6
#  define LOG_DEBUG    7
#endif
/*! \endcond */


/*========================================================*/
/*== internal helper functions                          ==*/
/*========================================================*/
/*! \cond NEVER_DOX */
void TrimmLine( char * pLine );
BOOL IsComment( char * pLine );
BOOL Separate( char * pLine, char ** ppRightpart );
static BOOL _internal_GetCmdLine( PTS_vssc_data pHnd );
static BOOL _internal_GetIniFileName( PTS_vssc_data pHnd, int Index );
static BOOL _internal_GetValue( TCHAR const * const pKeyValuePair,
                                TCHAR const * const pVar_Name,
                                TCHAR const * const pVarToken,
                                TCHAR const * const pValTokenIn,
                                TCHAR const * const pValTokenOut,
                                TCHAR *pValBuffer,
                                size_t MaxBuf );
static BOOL _internal_SetValue( DWORD *pdwValue, 
                                TCHAR *ptcValue, 
                                size_t MaxValue, 
                                TCHAR const * const pSourceBuffer );

/*! \endcond */



/*! \cond NEVER_DOX */
__inline
void OnDebug_ReportT( TCHAR const * const pMessage )
{{
  #if defined( DEBUG ) || defined( _DEBUG )
  OutputDebugString( pMessage );
  #endif
  return;
}}
/*! \endcond */





/*! \cond NEVER_DOX */
static 
int _internal_GetHash( char const * const pControl )
{{
  int i=0;
  while( ControlHash[i].pKey )
    {
    if( 0==stricmp( pControl, ControlHash[i].pKey ) )
      return ControlHash[i].AccessToken;
    i++;
    }
  return -1;
}}
/*! \endcond */





/*! \cond NEVER_DOX */
static 
int _internal_GetListOfCommands( char * pListBuff, size_t MaxBuff )
{{
  int i=0;
  size_t len, Remainder = MaxBuff -1;

  if( !pListBuff || Remainder<1 )
    return 0;

  *pListBuff=0;
  
  while( ControlHash[i].pKey )
    {
    len = strlen( ControlHash[i].pKey );
    if( Remainder >= len + 11 )
      {
      strcat( pListBuff, ControlHash[i].pKey );
      if( ControlHash[i].StringType==1 )
        strcat( pListBuff, " (string)\r\n" );
      else
        strcat( pListBuff, " (number)\r\n" );

      Remainder -= (len+11);
      }
    else
      {
      *pListBuff=0;
      return 0;
      }
    i++;
    }

  return (int) strlen(pListBuff);
}}
/*! \endcond */







/*! \cond NEVER_DOX */
static
char *_internal_IpFromName( PTS_vssc_data pHnd, const char * HostName, char * IPAddr, int MaxIPlen )
{{
  unsigned long addr;
  struct hostent *HostEntry_s;
  struct in_addr in;

  /* some security here... */
  if( !HostName || !*HostName || !IPAddr || MaxIPlen < 16 )
    {
    return "";
    }

  *IPAddr='\0'; /* default: unbekannter Name*/

  /* do not resolve VALID IP ADRESS */
  addr = inet_addr( HostName );
  if(addr != INADDR_NONE)
    {
    strncpy( IPAddr, HostName, MaxIPlen );
    *(IPAddr+MaxIPlen-1)='\0';
    return IPAddr;
    }

  /* seems to be really a string/name */
  HostEntry_s = gethostbyname( HostName );
  if( HostEntry_s == NULL)
    {
    return "";
    }

  /* take 1st interface only */
  memcpy (&in.s_addr, HostEntry_s->h_addr_list [0], sizeof (in.s_addr));
  strncpy( IPAddr, inet_ntoa(in), MaxIPlen );

  *(IPAddr+MaxIPlen-1)='\0';
  return IPAddr;
}}
/*! \endcond */




/*! \cond NEVER_DOX */
/* takes strings like "127.0.0.1:514" or "192.168.0.222" and sets it into the internal 
 * SendTo_XXX Buffers. if so, return is TRUE, else FALSE.
 * But it does not set SndAddr.
 */
static
BOOL _internal_VsscRefreshAddr( PTS_vssc_data pHnd, char const * const pDestination )
{{
  char *p;
  char IPAddr[16];
  char DestCopy[MAX_PATH];

  if( !pDestination || !*pDestination )
    return FALSE;

  strncpy( DestCopy, pDestination, DIM(DestCopy) );
  DestCopy[DIM(DestCopy)-1]=0;
  if( NULL != (p=strrchr(DestCopy, ':')) )
    {
    *p=0;
    p++;
    pHnd->udp.SendTo_Port = atoi( p );
    }

  if( 0 == pHnd->udp.SendTo_Port )
    {
    pHnd->udp.SendTo_Port = 514;
    }

  p = _internal_IpFromName( pHnd, DestCopy, IPAddr, DIM(IPAddr) );
  strncpy( pHnd->udp.SendTo_Addr.Text , (p && *p) ? p : DestCopy, pHnd->udp.SendTo_Addr.LastPos );
  pHnd->udp.SendTo_Addr.Text[ pHnd->udp.SendTo_Addr.LastPos ]=0;

  return TRUE;
}};
/*! \endcond */





/*! \cond NEVER_DOX */
/* takes Facility and/or Level and stores it into handle struct.
 * if -1 is given, the parameter is not used. if so, return is FALSE, else TRUE.
 */
static
BOOL _internal_VsscRefreshLevel( PTS_vssc_data pHnd, short Facility, short Level )
{{
  BOOL bRet = FALSE;

  if(Facility>=0)
    {
    pHnd->udp.Syslog_Facility = (unsigned) Facility;
    bRet = TRUE;
    }
  if(Level>=0)
    {
    /* pHnd->udp.Syslog_MaxLevel = 0 : show nothing
     *               1 : show emergency (LOG_EMERG, 0)
     *               2 : show alert  (LOG_ALERT, 1)
     * so we have a Offset of +1 between both Levels
     */
    pHnd->udp.Syslog_MaxLevel = (unsigned) Level-1;
    bRet = TRUE;
    }

  return bRet;
}};
/*! \endcond */







/*! \cond NEVER_DOX */
/* takes ZoneMask and stores it into handle struct.
 */
static
BOOL _internal_VsscRefreshZone( PTS_vssc_data pHnd, unsigned ZoneMask )
{{
  pHnd->global.ZoneMask = ZoneMask;
  return TRUE;
}};
/*! \endcond */






/*! \cond NEVER_DOX */
/* converts SLOG_xxx global logtype into RFC-SYSLOG compliant level (severity) */
static
short _internal_Vssc2syslogLevel( short VsscLevel )
{{
  switch( VsscLevel )
    {
    case SLOG_SILENT   : return LOG_DEBUG; /* this will not be reached, but for shure... */
    case SLOG_EMERG    : return LOG_EMERG;
    case SLOG_ALERT    : return LOG_ALERT;
    case SLOG_CRIT     : return LOG_CRIT;
    case SLOG_ERR      : return LOG_ERR;
    case SLOG_WARNING  : return LOG_WARNING;
    case SLOG_NOTICE   : return LOG_NOTICE;
    case SLOG_INFO     : return LOG_INFO;
    case SLOG_DEBUG    : 
    case SLOG_DEBUG2   : 
    case SLOG_DEBUGMAX : 
    default            : return LOG_DEBUG;
    }
}};
/*! \endcond */




/*! \cond NEVER_DOX */
/* returns okay if the time stamp is filled or - if the ts not used - if it is properly set empty */
static
BOOL _internal_GetTimeStamp( PTS_vssc_data pHnd, char * pTimeStamp, size_t MaxTimeStamp )
{{
  time_t now;
  struct tm * time_s;

  if( !pTimeStamp || MaxTimeStamp<20 )
    return FALSE;

  *pTimeStamp='\0';
  if(pHnd->global.ControlFlags & LOGFLGS_RFC3164_TIMESTAMP)
    {
    int iRet;
    now   = time(NULL);
    time_s = localtime(&now);
    iRet = strftime( pTimeStamp, MaxTimeStamp, "%b %d %H:%M:%S ", time_s);
    if(0==iRet)
      {
      *pTimeStamp=0;
      return FALSE;
      }
    if( pTimeStamp[4]=='0' ) /* RFC3164 requirement: single digit Day with one more space */
        pTimeStamp[4] =' ';
    }
  return TRUE;
}};
/*! \endcond */





/*! \cond NEVER_DOX */
/* returns okay if the IP/Hostname is filled or - if the ip/hn not used - if it is properly set empty */
static
BOOL _internal_GetIPorName( PTS_vssc_data pHnd, char * pSourceIP, size_t MaxSourceIP )
{{
  if( !pSourceIP || MaxSourceIP<20 )
    return FALSE;

  *pSourceIP='\0';
  if(pHnd->global.ControlFlags & LOGFLGS_RFC3164_SOURCEIP )
    {
    int iRet;
    size_t len;
    iRet = gethostname( pSourceIP, MaxSourceIP );
    if( 0!=iRet )
      {
      *pSourceIP='\0';
      return FALSE;
      }
    len = strlen(pSourceIP);
    if( len < MaxSourceIP-1 )
      {
      strcat( pSourceIP, " " );
      }
    else /*cut to long name */
      {
      pSourceIP[MaxSourceIP-2]=' ';
      pSourceIP[MaxSourceIP-1]='\0';
      }
    }
  return TRUE;
}};
/*! \endcond */



/*! \cond NEVER_DOX */
/* returns okay if the PID is filled or - if the PID not used - if it is properly set empty */
static
BOOL _internal_GetModulePID( PTS_vssc_data pHnd, char *pPID, size_t MaxPID )
{{
  if( !pPID || MaxPID<10 )
    return FALSE;

  *pPID='\0';
  if(pHnd->global.ControlFlags & LOGFLGS_PROCESS_ID)
    {
    _snprintf( pPID, MaxPID, "%08x ", pHnd->internal.UserPID );
    }
  return TRUE;
}};
/*! \endcond */



/*! \cond NEVER_DOX */
/* returns okay if the Sourcename is filled or - if the sn not used - if it is properly set empty */
static
BOOL _internal_GetModuleSource( PTS_vssc_data pHnd, char *pModuleName, size_t MaxModuleName, char const * const pModule )
{{
  if( !pModuleName || MaxModuleName<1 )
    return FALSE;

  *pModuleName='\0';
  if(pHnd->global.ControlFlags & LOGFLGS_MODULE_SOURCE)
    {
    size_t Leftpart, Rightpart, MaxSpace, Remainder;

    if(pHnd->global.ControlFlags & LOGFLGS_LONGTAG)
      {
      Leftpart=31; 
      Rightpart=32; 
      }
    else /* RFC does not allow TAG longer 32 char in whole */
      {
      Leftpart=15;
      Rightpart=16; 
      }
    if( MaxModuleName < (Leftpart+1+Rightpart+1) )
      return FALSE;

    MaxSpace = MIN( MaxModuleName, (Leftpart+1+Rightpart+1) ) - 1;
    Remainder = MaxSpace;

    strncpy( pModuleName, pHnd->global.AppName.Text, Remainder );
    pModuleName[ Remainder-1 ]='\0'; /*cut one more character, to get space for ".\0" */
    Remainder = MaxSpace - strlen(pModuleName);

    strcat( pModuleName, "." );
    Remainder = MaxSpace - strlen(pModuleName);

    strncat( pModuleName, pModule, Remainder );
    pModuleName[ Remainder-1 ]='\0';/*cut one more character, to get space for " \0" */
    strcat( pModuleName, " " );
    }
  return TRUE;
}};
/*! \endcond */




/*! \cond NEVER_DOX */
/* returns okay if the Counter is filled or - if the counter not used - if it is properly set empty */
static
BOOL _internal_GetCounter( PTS_vssc_data pHnd, char *pCounter64, size_t MaxCounter64 )
{{
  if( !pCounter64 || MaxCounter64<6 )
    return FALSE;

  *pCounter64='\0';
  if(pHnd->global.ControlFlags & LOGFLGS_COUNTER64)
    {
    _snprintf( pCounter64, MaxCounter64, "{%02u} ", pHnd->internal.MsgCount & 0x3F );
    }
  return TRUE;
}};
/*! \endcond */





static BOOL _internal_GetCmdLine( PTS_vssc_data pHnd )
{{
  TCHAR *pCmdLine, *pSeekPos;
  size_t len;

  if( pHnd->internal.pMyCmdLine )
    return (*pHnd->internal.pMyCmdLine) ? TRUE : FALSE; /* is already filled or empty */
  
  pCmdLine = GetCommandLine();
  len = _tcslen( pCmdLine ) + 1;
  pHnd->internal.pMyCmdLine = (TCHAR*) malloc( len );
  _tcscpy( pHnd->internal.pMyCmdLine, pCmdLine );

  _tcsupr( pHnd->internal.pMyCmdLine );
  pSeekPos = _tcsstr( pHnd->internal.pMyCmdLine, _T("-SLOG ") );
  if( !pSeekPos )
       pSeekPos = _tcsstr( pHnd->internal.pMyCmdLine, _T("/SLOG ") );

  if( !pSeekPos ) /* we didn't found somewhat after /SLOG or SLOG */
    {
    *(pHnd->internal.pMyCmdLine)=0;
    return FALSE;
    }

  /* we found somewhat after /SLOG or SLOG */
  /*copy again, this time without changing upper/lower*/
  _tcscpy( pHnd->internal.pMyCmdLine, pSeekPos );
  /* Array now contains only everything following the SLOG specifier */
  
  return (*pHnd->internal.pMyCmdLine) ? TRUE : FALSE; /* is already filled or empty */
}}

static BOOL _internal_FileExist( TCHAR const * const pFileName )
{{
  struct _stat status;
  int result;

  result = _tstat( pFileName, &status );
  if( (0==result) && (status.st_mode & _S_IFREG) && (status.st_mode & _S_IREAD) )
    return TRUE;
  else
    return FALSE;
}}


static BOOL _internal_GetIniFileName( PTS_vssc_data pHnd, int Index )
{{
  size_t RemainLen;
  int    i;
  TCHAR *pPatch;
  TCHAR  Name[MAX_PATH] = {0};
  
  for( i=0 ;i < INI_LOCATIONS; i++ )
    {
    if( ! *pHnd->internal.Ini_File[i] )
      {
      switch(i)
        {
        case INI_LOCATION_EXE: /* same path as the Application */
          {
          GetModuleFileName( NULL, Name, DIM( Name )-1 );
          pPatch = _tcsrchr( Name, _T('.') );
          if(pPatch) 
            *pPatch=0; /* cut away .EXE suffix */
          RemainLen = DIM( pHnd->internal.Ini_File[ i ] )-1;
          _tcsncpy( pHnd->internal.Ini_File[ i ], pPatch, RemainLen );
          pHnd->internal.Ini_File[ i ][ RemainLen ]=0;
          RemainLen -= _tcslen( pHnd->internal.Ini_File[ i ] );
          _tcsncat( pHnd->internal.Ini_File[ i ], _T(".ini"), RemainLen ); 
          pHnd->internal.Ini_File[ i ][ RemainLen ]=0;
          };break;

        case INI_LOCATION_USRAPPDATA: /* Users home directory */
        case INI_LOCATION_ALLUSRAPPD: /* Common All Users Home directory */
          {
          int Folder;
          HRESULT res;
          if( INI_LOCATION_USRAPPDATA==i )
            Folder = CSIDL_APPDATA;
          else
            Folder = CSIDL_COMMON_APPDATA;

          res = SHGetSpecialFolderPath( NULL, Name, Folder, FALSE );
          if( NOERROR!=res ) break;

          RemainLen = DIM( pHnd->internal.Ini_File[ i ] )-1;
          _tcsncpy( pHnd->internal.Ini_File[ i ], Name, RemainLen );
          pHnd->internal.Ini_File[ i ][ RemainLen ]=0;
          RemainLen -= _tcslen( pHnd->internal.Ini_File[ i ] );

          _tcsncat( pHnd->internal.Ini_File[ i ], _T("\\"), RemainLen ); 
          pHnd->internal.Ini_File[ i ][ RemainLen ]=0;
          RemainLen -= _tcslen( pHnd->internal.Ini_File[ i ] );

          _tcsncat( pHnd->internal.Ini_File[ i ], pHnd->global.AppName.Text, RemainLen ); 
          pHnd->internal.Ini_File[ i ][ RemainLen ]=0;
          RemainLen -= _tcslen( pHnd->internal.Ini_File[ i ] );

          _tcsncat( pHnd->internal.Ini_File[ i ], _T(".ini"), RemainLen ); 
          pHnd->internal.Ini_File[ i ][ RemainLen ]=0;
          };break;
        }
      }
    }

  if( _internal_FileExist( pHnd->internal.Ini_File[ Index ] ) )
    return TRUE;
  else
    return FALSE;
}};



static BOOL _internal_GetValue( TCHAR const * const pKeyValuePair,
                                TCHAR const * const pVar_Name,
                                TCHAR const * const pVarToken,
                                TCHAR const * const pValTokenIn,
                                TCHAR const * const pValTokenOut,
                                TCHAR *pValBuffer,
                                size_t MaxBuf )
{{
  BOOL bFound=FALSE;
  size_t len;
  TCHAR *pKeyPos, *pValPos, *pValEnd;
  TCHAR *pParseBuffer;
  TCHAR DefaultYES[]="1";

  pParseBuffer = (TCHAR*) malloc( (_tcslen(pKeyValuePair) * sizeof(TCHAR)) +1 );
  if(!pParseBuffer)
    return FALSE;

  _tcscpy( pParseBuffer, pKeyValuePair );
  /* pParseBuffer now contains everything following the /SLOG: specifier */

  len = _tcslen( pVar_Name );
  
  #pragma message("special handling for empty or NULL pVarToken required for Environment")
  /* seek for first sequence of 'VarDelimiter, Var_Name, ValDelimiter, Value' */
  pValPos = pKeyPos = _tcstok( pParseBuffer, pVarToken ); 
  while( !bFound && pKeyPos )
    {
    /*did we found 'VarDelimiter, Var_Name' ? */
    if( 0==_tcsnicmp( pKeyPos, pVar_Name, len ) )
      {
      pValPos = _tcspbrk( pKeyPos, pValTokenIn );
      /*YES, But did we really found 'VarDelimiter, Var_Name, ValDelimiter' or only a related Var_Name_longer ? */
      if( pValPos == (pKeyPos+len) )
        {
        pValPos++;
        bFound=TRUE; /*we found EXACTLY our Var_Name*/
        }
      else if( pVarToken && *pVarToken && 
               ((pKeyPos+len)==_tcspbrk( pKeyPos, pValTokenOut ) || 
                (pKeyPos+len)==_tcspbrk( pKeyPos, pVarToken    )
               )
             )
        {
        /* "special handling for empty Values (only switch, means bool YES) required. only with NULL!=pVarToken" */
        pValPos=DefaultYES;
        bFound=TRUE; /*we found EXACTLY our Var_Name but with no Argument. BOOL switch*/
        }
      }
    else
      {
      pKeyPos = _tcstok( pParseBuffer, NULL ); 
      }
    }
    
  if( (pValPos<=pKeyPos) && (pValPos!=DefaultYES) ) /*sanity check*/
    bFound=FALSE;
  
  if( bFound )
    {
    TCHAR Hyphen=0;
    /* we found our switch! */
    while( *pValPos && (*pValPos==_T(' ') || *pValPos==_T('\t')) )
       pValPos++; /* skip optional whitespaces */

    if( *pValPos=='"' || *pValPos=='\'' || *pValPos=='´' || *pValPos=='`' )
      {
      Hyphen=*pValPos;
      pValPos++;
      }

    pValEnd = pValPos;
    
    /* hyphenated or quoted string? */
    while( Hyphen && *pValEnd && ((*pValEnd!=Hyphen) || (*(pValEnd-1)=='\\')) )
      {
      pValEnd++;
      }

    if( Hyphen )
      {    
      *pValEnd=0;  /* cut string at last hyphen */
      }
    else
      {
      pValEnd = _tcspbrk( pValEnd, pValTokenOut );
      if(pValEnd) 
        *pValEnd=0; /* cut string at first Out-Token */
      }
    _tcsncpy( pValBuffer, pValPos, MaxBuf );
    pValBuffer[ MaxBuf-1 ]=0;

    /*if only a fraction of Value was copied, we report having nothing:*/
    if(_tcslen(pValBuffer) != _tcslen(pValPos) )
      bFound=FALSE;
    }

  free( pParseBuffer );
  return bFound;
}}






static BOOL _internal_GetIniValString( TCHAR const * const pIniFile,
                                       TCHAR const * const pGroupName,  TCHAR const * const pKeyName,
                                       TCHAR *pValBuffer,               size_t MaxBuf, 
                                       TCHAR const * const pDefaultVal, const USHORT WriteIfNotExist )
{{
  DWORD dwRet;
  size_t len;
  //TCHAR DefBuffer[MAX_PATH];
  TCHAR ValBuffer[MAX_PATH];
  BOOL  bFound=FALSE;
  TCHAR DummyString[]="0xCaFeAfFe"; /* cafe+affe reads in German as Coffe and Ape ;-) */

  /* to see, if we have the Value inside the INI, we must use a trick:
   * First, we try to read it, but give a total crazy default-value.
   * if we get this value returned, we really don't knwo if it is 
   * crazy or not (can be coincidental the real value).
   * So in this case, we call it again, but this time with the real default
   * value. If we again get the Crazy Value, the value isn't that crazy.
   */
  len = DIM(ValBuffer) - 1;
  if( 0==_tcscmp( ValBuffer, pDefaultVal ) )
    {
    /* make shure they are not identical!*/
    DummyString[1]++;
    }

  dwRet = GetPrivateProfileString( pGroupName, pKeyName, DummyString, ValBuffer, len, pIniFile );
  if( dwRet <= len )
    {
    /* we found it in the INI-File. Or not. In this case, we need to read again, but with different defaultval */
    if( 0==_tcscmp( ValBuffer, DummyString ) )
      {
      dwRet = GetPrivateProfileString( pGroupName, pKeyName, pDefaultVal, ValBuffer, len, pIniFile );
      if( dwRet <= len )
        {
        /* at least NOW we have read a value */
        bFound = TRUE;
        }
      }
    else
      {
      bFound = TRUE;
      }
    } /*1st read okay */

  if( bFound ) /* only use the value, if it is real */
    {
    _tcsncpy( pValBuffer, ValBuffer, MaxBuf );
    pValBuffer[ MaxBuf-1 ]=0;
    }
  else /* otherwise use the DefaultVal */
    {
    _tcsncpy( pValBuffer, pDefaultVal, MaxBuf );
    pValBuffer[ MaxBuf-1 ]=0;
    }

  if( !bFound && (WriteIfNotExist & _SLOG_CREATE_INI) )
    {
    WritePrivateProfileString( pGroupName, pKeyName, pDefaultVal, pIniFile );
    WritePrivateProfileString( NULL, NULL, NULL, pIniFile ); /* flush the INI cache */
    }
  
  return bFound;
}}







static BOOL  _internal_GetIniValDword( TCHAR const * const pIniFile,
                                       TCHAR const * const pGroupName,  TCHAR const * const pKeyName,
                                       DWORD *pdwValue,                 const DWORD DefaultVal,
                                       const USHORT WriteIfNotExist )
{{
  size_t len;
  TCHAR DefBuffer[MAX_PATH];
  TCHAR ValBuffer[MAX_PATH];
  BOOL bFound=FALSE;

  len = DIM(DefBuffer) - 1;

  /* some luxurity follows: 
   *    negative values between -1 and -1024 are used as negative decimal string 
   *    positive values between  0 and +1024 are used as positive decimal string
   *    all others are used as hexadecimal strings
   */
  if( ((long)DefaultVal <= -1L) && ((long)DefaultVal <= -1024L) )
    _sntprintf( DefBuffer, len, _T("%li"), (long) DefaultVal ); 
  else if( (DefaultVal >= 0) && (DefaultVal <= 1024) )
    _sntprintf( DefBuffer, len, _T("%lu"), DefaultVal ); 
  else
    _sntprintf( DefBuffer, len, _T("0x%08x"), DefaultVal ); 

  len = DIM(ValBuffer) - 1;
  bFound = _internal_GetIniValString( pIniFile, pGroupName, pKeyName, ValBuffer, len, DefBuffer, WriteIfNotExist );
  /* we use the string function because of more powerful number format handling */

  if( bFound )
    {
    #pragma message ("use better xtol()")
    *pdwValue = (DWORD) _ttol( ValBuffer );
    }

  if( !bFound && (WriteIfNotExist & _SLOG_CREATE_INI) )
    {
    WritePrivateProfileString( pGroupName, pKeyName, DefBuffer, pIniFile );
    WritePrivateProfileString( NULL, NULL, NULL, pIniFile ); /* flush the INI cache */
    }
  
  return bFound;
}}








static BOOL _internal_SetValue( DWORD *pdwValue, TCHAR *ptcValue, size_t MaxValue, TCHAR const * const pSourceBuffer )
{{
  BOOL bRet=FALSE;

  if(ptcValue && MaxValue>1)
    {
    /* we have the string now !, if STRING foo, we can '_tcsncpy(target,..,..)' " */
    _tcsncpy( ptcValue, pSourceBuffer, MaxValue );
    ptcValue[ MaxValue-1 ]=0;
    bRet = TRUE;
    }

  if(pdwValue) /*if pointer to DWORD, we want to get the DWORD val */
    {
    #pragma message ("use better xtol()")
    *pdwValue = (DWORD) _ttol( pSourceBuffer );
    bRet = TRUE;
    }
  return bRet;
}}





#pragma message("WriteIfNotExist shall specify if writing to INI, HKCU, HKLM (define some Bitmasks!)" )




/*! \cond NEVER_DOX */
/* read 32 bit numerical params from Cmd-Line (all after '-SLOG' ), Environment, Private Profile .INI, Registry */
int _internal_ReadConfig(  PTS_vssc_data pHnd, 
                           TCHAR const * const pGroupName, 
                           TCHAR const * const pKeyName, 
                           const short WriteIfNotExist,
                           TCHAR const * const ptcDefault, 
                           TCHAR* ptcValue, 
                           const size_t MaxValue, 
                           const DWORD DefaultVal, 
                           DWORD * pdwValue )
{{
  DWORD dwRet = FALSE;
  BOOL bRet = FALSE, bFound = FALSE;
  size_t RemainLen;
  //UINT  Ini_value;
  int   i;
  TCHAR Var_Name[MAX_PATH];
  TCHAR ValBuffer[MAX_PATH];
  //TCHAR *pSeekPos;

  if( !pdwValue || !pGroupName || !pKeyName )
    return FALSE;

  do{ /* do-once */
    /* combine Groupname and Varname to one word, for Environment and for CmdLine. They both dont support GroupNames */
    RemainLen = DIM( Var_Name ) -1;
    _tcsncpy( Var_Name, pGroupName, RemainLen ); Var_Name[ RemainLen ] =0; RemainLen -= _tcslen(Var_Name);
    _tcsncat( Var_Name, _T("_")   , RemainLen ); Var_Name[ RemainLen ] =0; RemainLen -= _tcslen(Var_Name);
    _tcsncat( Var_Name, pKeyName  , RemainLen ); Var_Name[ RemainLen ] =0; 

    /*=========================================================================*
     *           1st START LOOKING for Cmd Line Param                          *
     *=========================================================================*/
    if( _internal_GetCmdLine( pHnd ) &&
        _internal_GetValue( pHnd->internal.pMyCmdLine, Var_Name, "-/", ":= \t", " \t-/", ValBuffer, DIM(ValBuffer) ) )
      {
      bRet = _internal_SetValue( pdwValue, ptcValue, MaxValue, ValBuffer );
      if(bRet) 
        break; /* to the end of do-once, but only if we got some value */
      } /*end-if-got a nice formed value from CmdLine */

    /*=========================================================================*
     *           2nd now LOOKING for Environment                               *
     *=========================================================================*/
    dwRet = GetEnvironmentVariable( Var_Name, ValBuffer, DIM(ValBuffer)-1 );
    if( dwRet>0 && dwRet<DIM(ValBuffer) )
      {
      /* we found it in the Environment*/
      bRet = _internal_SetValue( pdwValue, ptcValue, MaxValue, ValBuffer );
      if(bRet) 
        break; /* to the end of do-once */
      }

    /*=========================================================================*
     *           3rd: if noting in Environment,                                *
     *           next look inside <Appname>.ini, for Values                    *
     *=========================================================================*/
    for( i=0 ; i<INI_LOCATIONS ; i++ )
      {
      if( _internal_GetIniFileName( pHnd, i ) )
        {
        /* we shall read a string ? */
        if( ptcValue && _internal_GetIniValString( pHnd->internal.Ini_File[i], 
                                                   pGroupName, pKeyName, 
                                                   ptcValue,   MaxValue, 
                                                   ptcDefault, WriteIfNotExist ) )
          {
          bRet = TRUE; /* we found it in the INI File */
          }
        /* we shall read a DWORD */
        if( pdwValue && _internal_GetIniValDword( pHnd->internal.Ini_File[i], 
                                                  pGroupName, pKeyName, 
                                                  pdwValue,   DefaultVal,
                                                  WriteIfNotExist ) )
          {
          bRet = TRUE; /* we found it in the INI File */
          }
        } /* endif this INI FILE FOUND */
      }/* end for all INI locations */
    if( bFound ) /* do not search further */
      break; /* to the end of do-once */

    /*=========================================================================*
     *           NOW START LOOKING INSIDE REGISTRY                             *
     *=========================================================================*/

    /* startLocalBlock( REG ) */
      {
      HKEY   hKey;
      DWORD  dwDisp;
      DWORD  dwType;
      DWORD  ValLen;
      DWORD  Value;
      TCHAR  GroupKey[ MAX_PATH ];
      size_t nLastChar;

      nLastChar = DIM( GroupKey ) - 1;
      _tcsncpy( GroupKey, pHnd->helper.tcsProfileName, nLastChar );
      GroupKey[ nLastChar ] = 0;
      _tcsncat( GroupKey, _T("\\"), nLastChar- _tcslen( pHnd->helper.tcsProfileName ) );
      GroupKey[ nLastChar ] = 0;
      _tcsncat( GroupKey, pGroupName, nLastChar- _tcslen( pHnd->helper.tcsProfileName ) );
      GroupKey[ nLastChar ] = 0;

      /*-----------------------------------------------------------------*
       *          STEP 1: try to read from Registry-HKCU                 *
       *          if okay, we did it well                                *
       *          if failed, we assume first access, so we try to copy   *
       *          it from HKLM (if access is granted)                    *
       *-----------------------------------------------------------------*/
      dwRet = RegCreateKeyEx( HKEY_CURRENT_USER, 
                            GroupKey, 
                            0 /*Reserved; must be zero*/, 
                            NULL /*object type, ignored on read or if already exist*/, 
                            REG_OPTION_NON_VOLATILE, 
                            KEY_READ /*desired Access*/, 
                            NULL /*lpSec*/, 
                            &hKey /*receives the handle we will need next*/, 
                            &dwDisp );
      if( (dwRet == ERROR_SUCCESS) && (dwDisp == REG_OPENED_EXISTING_KEY) )
        {
        dwType = REG_DWORD;
        ValLen = sizeof( DWORD );
        dwRet  = RegQueryValueEx( hKey, pKeyName, NULL /*Reserved; must be NULL.*/, &dwType, (BYTE*) pdwValue, &ValLen );
        RegCloseKey( hKey );
        if( dwRet == ERROR_SUCCESS )
          { 
          bRet = TRUE;
          break; /* to the end of do-once */
          }
        }
  
      /*-----------------------------------------------------------------*
       *          STEP 2: try to read from Registry-HKLM                 *
       *          if okay, we copy the valuse to HKCU + we have done     *
       *          if failed, we only lack of rights. so we take the      *
       *          default, try to copy it to HKCU and finished           *
       *-----------------------------------------------------------------*/
      dwRet = RegCreateKeyEx( HKEY_LOCAL_MACHINE, 
                            GroupKey, 
                            0 /*Reserved; must be zero*/, 
                            NULL /*object type, ignored on read or if already exist*/, 
                            REG_OPTION_NON_VOLATILE, 
                            KEY_READ /*desired Access*/, 
                            NULL /*lpSec*/, 
                            &hKey /*receives the handle we will need next*/, 
                            &dwDisp );
      if( dwRet == ERROR_SUCCESS )
        {
        dwType = REG_DWORD;
        ValLen = sizeof( DWORD );
        dwRet  = RegQueryValueEx( hKey, pKeyName, NULL /*Reserved; must be NULL.*/, &dwType, (BYTE*) pdwValue, &ValLen );
        RegCloseKey( hKey );
        if( dwRet != ERROR_SUCCESS )
          {
          Value = DefaultVal; 
          }
        *pdwValue = Value;

        /* create the copy in HKCU for the next time we access it */
        if( WriteIfNotExist & _SLOG_CREATE_HKCU )
          {
          dwRet = RegCreateKeyEx( HKEY_CURRENT_USER, 
                            GroupKey, 
                            0 /*Reserved; must be zero*/, 
                            NULL /*object type, ignored on read or if already exist*/, 
                            REG_OPTION_NON_VOLATILE, 
                            KEY_WRITE /*desired Access*/, 
                            NULL /*lpSec*/, 
                            &hKey /*receives the handle we will need next*/, 
                            &dwDisp );
          if( dwRet == ERROR_SUCCESS )
            {
            dwType = REG_DWORD;
            ValLen = sizeof( DWORD );
            dwRet  = RegSetValueEx( hKey, pKeyName, NULL /*Reserved; must be NULL.*/, dwType, (BYTE*) &Value, ValLen );
            RegCloseKey( hKey );
            }
          }
        }
      else /* not able to open HKLM: */
        {
        if( WriteIfNotExist & _SLOG_CREATE_HKLM )
          {
          dwRet = RegCreateKeyEx( HKEY_LOCAL_MACHINE, 
                            GroupKey, 
                            0 /*Reserved; must be zero*/, 
                            NULL /*object type, ignored on read or if already exist*/, 
                            REG_OPTION_NON_VOLATILE, 
                            KEY_WRITE /*desired Access*/, 
                            NULL /*lpSec*/, 
                            &hKey /*receives the handle we will need next*/, 
                            &dwDisp );
          if( dwRet == ERROR_SUCCESS )
            {
            dwType = REG_DWORD;
            ValLen = sizeof( DWORD );
            dwRet  = RegSetValueEx( hKey, pKeyName, NULL /*Reserved; must be NULL.*/, dwType, (BYTE*) &DefaultVal, ValLen );
            RegCloseKey( hKey );
            }
        /* also create a copy in HKCU for the next time we access it */
        if( WriteIfNotExist & _SLOG_CREATE_HKCU )
          {
          dwRet = RegCreateKeyEx( HKEY_CURRENT_USER, 
                            GroupKey, 
                            0 /*Reserved; must be zero*/, 
                            NULL /*object type, ignored on read or if already exist*/, 
                            REG_OPTION_NON_VOLATILE, 
                            KEY_WRITE /*desired Access*/, 
                            NULL /*lpSec*/, 
                            &hKey /*receives the handle we will need next*/, 
                            &dwDisp );
          if( dwRet == ERROR_SUCCESS )
            {
            dwType = REG_DWORD;
            ValLen = sizeof( DWORD );
            dwRet  = RegSetValueEx( hKey, pKeyName, NULL /*Reserved; must be NULL.*/, dwType, (BYTE*) &Value, ValLen );
            RegCloseKey( hKey );
            }
          }
        } /* opend HKLM or not */
      } /* startLocalBlock( REG ) */

    /* finally, if nothing worked til here, we use the default. */
    *pdwValue = DefaultVal;
    }
  }while(0); /* do-once */

  return TRUE;
}};
/*! \endcond */



/*! \cond NEVER_DOX */
/* read TCHAR params from Registry, Private Profile, Environment, Cmd-Line (all after '-SLOG' )*/
int _internal_ReadConfig_TCHAR(  PTS_vssc_data pHnd, 
                                 TCHAR const * const pGroupName, 
                                 TCHAR const * const pKeyName, 
                                 const short WriteIfNotExist,
                                 TCHAR const * const ptcDefault, 
                                 TCHAR* ptcValue, 
                                 const size_t MaxValue )
{{
  BOOL bIsSpecialName;

  if( !ptcValue || MaxValue<2 || !pGroupName || !pKeyName )
    return FALSE;

  bIsSpecialName = ( (0==_tcsicmp( pKeyName, _T("AppName"))) && (0==_tcsicmp( pGroupName, _T("global"))) );
  /*=========================================================================*
   *           1st START LOOKING for Environment                             *
   *=========================================================================*/
  //#error "not done yet"
  if( bIsSpecialName )
    {
    ; /* dont return! even if we got the name, we have to make the tcsProfileName from it */
    }

  /*=========================================================================*
   *           if noting in Environment, next look inside                    *
   *           <Appname>.ini, for Values                                     *
   *=========================================================================*/
  //#error "not done yet"
  if( bIsSpecialName )
    {
    ; /* dont return! even if we got the name, we have to make the tcsProfileName from it */
    }

  /*=========================================================================*
   *           NOW START LOOKING INSIDE REGISTRY                             *
   *=========================================================================*/
  if( bIsSpecialName )
    {
    /* if we come to here, we still don't know our name. 
     * If so, we take it from the default Value.
     * If this is also empty, we use GetModuleFileName()
     */
    size_t nLastChar;
    
    if( ptcDefault )
      {
      _tcsncpy( pHnd->global.AppName.Text, ptcDefault, pHnd->global.AppName.LastPos );
      pHnd->global.AppName.Text[ pHnd->global.AppName.LastPos ]=0;
      }
    else
      {
      TCHAR Name[MAX_PATH] = {0};
      TCHAR *pPatch;
      GetModuleFileName( NULL, Name, DIM( Name )-1 );
      pPatch = _tcsrchr( Name, _T('.') );
      if(pPatch) *pPatch=0; /* cut away .EXE suffix */
      pPatch = _tcsrchr( Name, _T('\\') );
      if(!pPatch) pPatch=Name;

      _tcsncpy( pHnd->global.AppName.Text, pPatch, pHnd->global.AppName.LastPos );
      pHnd->global.AppName.Text[ pHnd->global.AppName.LastPos ]=0;
      }

    nLastChar = DIM( pHnd->helper.tcsProfileName ) - 1;
    _tcsncpy( pHnd->helper.tcsProfileName, _T("Software\\sLoggy\\"), nLastChar );
    pHnd->helper.tcsProfileName[ nLastChar ] = 0;
    _tcsncat( pHnd->helper.tcsProfileName, pHnd->global.AppName.Text, nLastChar- _tcslen( pHnd->helper.tcsProfileName ) );
    pHnd->helper.tcsProfileName[ nLastChar ] = 0;
    return TRUE;
    }


  /* startLocalBlock( REG ) */
    {
    OnDebug_ReportT( _T("real Reading of Cfg String Values not yet finished\r\n") );
    }


  /* finally, if nothing worked til here, we use the default. */
  if( !ptcDefault )
    _tcsncpy( ptcValue, _T(""), MaxValue );
  else
    _tcsncpy( ptcValue, ptcDefault, MaxValue );
  ptcValue[ MaxValue-1 ] = 0;
  return TRUE;
}};
/*! \endcond */




/*========================================================*/
/*== PUBLIC functions                                   ==*/
/*========================================================*/

/*! \addtogroup basic_functions Functions (basic)
 *@{
 */

/*! \brief simple initialisation
 *
 * This is the most simple init function. No params required.
 *
 * sample call:
 *  \code
    char const * const mod = "MyModule";
    int hLog;

    hLog = VSSC_open();
    if( hLog )
      {
      VSSC_Log( hLog, SLOG_DEBUG, mod, "Message of typ: 'SILENT' %u", SLOG_EMERG  );
      VSSC_close( hLog );
      }
 *  \endcode
 *
 * \note
 * this function is a variant. there is another one which can be parameterized.
 */
int VSSC_open( void )
{{
  return VSSC_open2( NULL, -1, -1, NULL, TRUE ); /*! \retval same as VSSC_open2() */
}}




/*! \brief complex initialisation
 *
 * This is the more powerful init function. Params are usually required, but can be 0 or Zero
 *
 * sample call:
 *  \code
    char const * const mod = "MyModule";
    int hLog;

    hLog = VSSC_open2( "localhost:514", LOG_LOCAL2, LOG_DEBUG, "TestProg" );
    if( hLog )
      {
      VSSC_Log( hLog, SLOG_DEBUG, mod, "Message of typ: 'SILENT' %u", SLOG_EMERG  );
      VSSC_close( hLog );
      }
 *  \endcode
 *
 * \note
 * this function is a variant. there is another one which can be parameterized.
 */
int VSSC_open2( char const * pDestination, /*!< [in] IP and Port for Syslogd. may be NULL or "" for don't care */
                short Facility,            /*!< [in] Facility (program type) as declarewd in RFC. May be -1 for don't care */
                short Level,               /*!< [in] Level. 0=Silent, 7=verbose. May be -1 for don't care */
                char const * pAppName,     /*!< [in] Show all logs as comming from.... may be NULL or "" for don't care */
                short bAutoConfig          /*!< [in] >0 means this function automatically tries to read all Setup parameters itself. No further VSSC_SetStr() or VSSC_SetInt() is required */
              )
{{
  PTS_vssc_data pHnd;
  int iRet;
  int Blocking = 1;

  pHnd = (PTS_vssc_data) malloc( sizeof(TS_vssc_data) );
  if( !pHnd )
    {
    OutputDebugString( _T("Simple Syslog Client: outofmem\r\n") );
    return 0; /*! \retval 0 not enough memory. open failed */
    }
  memset( pHnd, 0x00, sizeof( TS_vssc_data ) );
  pHnd->SecCookie1 = pHnd->SecCookie2 = 0xC001B001;
  pHnd->internal.MsgCount = 0;
  pHnd->internal.UserPID = GetCurrentProcessId();

  /* init all P32STRING Members or they never will be filled! */
  MAKE_CFGLEN( pHnd->global.AppName );
  MAKE_CFGLEN( pHnd->udp.SendTo_Addr );

  pHnd->internal.hMutex = CreateMutex( NULL, FALSE, NULL );
  if( !pHnd->internal.hMutex ) /* Check for error. */
    {
    OutputDebugString( _T("Simple Syslog Client: MUTEX Problem\r\n") );
    free( pHnd );
    return 0; /*! \retval 0 cannot create a needed kernel object. open failed */
    }

  /*
  if( ERROR_ALREADY_EXISTS == GetLastError() )
    {
#   ifdef DEBUG
    OutputDebugString( _T("I am not the first user of this Mutex, but this is Okay") );
#   endif
    }
  */

  /* read the full Setup ? */
  if( bAutoConfig )
    {
    iRet = VSSC_AutoCfg( (int) pHnd, bAutoConfig );
    }
  else
    {
    /* \todo:  later move this code into VSSC_AutoCfg() and AutoCfg[] Array. */
    if( !pDestination || pDestination ) /* if no destination was given, default 'syslog @ this pc' */
      { pDestination = "localhost:514"; }
    _internal_VsscRefreshAddr( pHnd, pDestination );
    _internal_VsscRefreshLevel(pHnd, (short)((Facility>=0) ? Facility : LOG_LOCAL7), 
                                     (short)((Level   >=0) ? Level    : LOG_INFO ));
    _internal_VsscRefreshZone( pHnd, LOGZONE_EVERYTHING );

    *pHnd->global.AppName.Text=0;
    if( !pAppName || !*pAppName ) /* if no Name was given, take current exe name */
      {
      char acBuffer[ MAX_PATH ];
      char *p;
      GetModuleFileName( NULL, acBuffer, DIM(acBuffer) -1 );
      p = strrchr(acBuffer, '.');     /* trow ".exe" part  */
      if(p) *p=0;
      p = strrchr(acBuffer, '\\');    /* throw path prefix */
      if(p)
        {
        *p++;
        strncpy( pHnd->global.AppName.Text, p, pHnd->global.AppName.LastPos ); /**/
        pHnd->global.AppName.Text[ pHnd->global.AppName.LastPos ]=0;
        }
      }
    else
      {
      strncpy( pHnd->global.AppName.Text, pAppName, pHnd->global.AppName.LastPos );
      pHnd->global.AppName.Text[ pHnd->global.AppName.LastPos ]=0;
      }

#   ifdef _DEBUG
    pHnd->global.ControlFlags = LOGFLGS_RFC3164_TIMESTAMP | LOGFLGS_MODULE_SOURCE | LOGFLGS_RFC3164_SOURCEIP | LOGFLGS_COUNTER64 | LOGFLGS_PROCESS_ID | LOGFLGS_LONGTAG;
#   else
    pHnd->global.ControlFlags = LOGFLGS_RFC3164_TIMESTAMP | LOGFLGS_MODULE_SOURCE | LOGFLGS_COUNTER64;
#   endif
    }

  if( pHnd->udp.Use )
    {
    iRet = WSAStartup( MAKEWORD(2,2), &pHnd->internal.wsaData );
    pHnd->internal.SndSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
    if( pHnd->internal.SndSocket == INVALID_SOCKET)
      {
      OutputDebugString( _T("Simple Syslog Client: socket() FAILED\r\n") );
      free( pHnd );
      return 0; /*! \retval 0 cannot create a IP socket. open failed */
      }

    memset( &pHnd->internal.SndAddr, 0x00, sizeof( struct sockaddr_in ));
    pHnd->internal.SndAddr.sin_family = AF_INET;
    pHnd->internal.SndAddr.sin_port = htons( pHnd->udp.SendTo_Port );
    pHnd->internal.SndAddr.sin_addr.s_addr = inet_addr( pHnd->udp.SendTo_Addr.Text );

    pHnd->udp.From_Port=0;
    memset( &pHnd->internal.OwnAddr, 0x00, sizeof( struct sockaddr_in ));
    pHnd->internal.OwnAddr.sin_family = AF_INET;
    pHnd->internal.OwnAddr.sin_port = htons( pHnd->udp.From_Port );
    pHnd->internal.OwnAddr.sin_addr.s_addr = INADDR_ANY;

    iRet = setsockopt( pHnd->internal.SndSocket, SOL_SOCKET, SO_REUSEADDR, (const char*) &Blocking, sizeof(Blocking) );
    } /*if( pHnd->udp.Use )*/

# if 0
  /* old: usually, there is no need for binding UDP sockets, 
   *      but later we will send simply with send() instead of sendto(), 
   *      so this makes things more ease.
   * new: we now send using sendto(), so we can change the log target on the fly
   */
  iRet = bind( pHnd->internal.SndSocket, (struct sockaddr *)&pHnd->internal.OwnAddr, sizeof(pHnd->internal.OwnAddr) );
  if( iRet == SOCKET_ERROR )
    {
    OutputDebugString( _T("Simple Syslog Client: bind() FAILED\r\n") );
    free( pHnd );
    return 0; /*! \retval 0 cannot bind the outgoing udp port. open failed */
    }
# endif

  /*local_block()*/
    {
    char OutTxt[512];
    _snprintf( OutTxt, DIM(OutTxt), "Syslog for %s:%u open and ready. App='%s', Fac=%u, Lev=%u\r\n",
              pHnd->udp.SendTo_Addr.Text, 
              pHnd->udp.SendTo_Port,
              pHnd->global.AppName.Text, 
              pHnd->udp.Syslog_Facility, 
              pHnd->udp.Syslog_MaxLevel );
    OutputDebugString( OutTxt );
    }
  return (int) pHnd;  /*! \retval pHnd A value of nonzero is a valid handle for subsequent VSSC_Log() calls. */
}}




/*! \brief closing the logging
 *
 * This function closes the Logging system and releases all the ressources (memory, mutex, sockets, etc.)
 *
 * sample call:
 *  \code
    char const * const mod = "MyModule";
    int hLog;

    hLog = VSSC_open2( "localhost:514", LOG_LOCAL2, LOG_DEBUG, "TestProg" );
    if( hLog )
      {
      VSSC_Log( hLog, SLOG_DEBUG, mod, "Message of typ: 'SILENT' %u", SLOG_EMERG  );
      VSSC_close( hLog );
      }
 *  \endcode
 *
 */
int VSSC_close( int Handle /*!< [in] Handle a valid Handle from succesful VSSC_openX() call */)
{{
  PTS_vssc_data pHnd = (PTS_vssc_data) Handle;
  if( !Handle || (pHnd->SecCookie1 != 0xC001B001) || (pHnd->SecCookie2 != 0xC001B001) )
    {
    OutputDebugString( _T("Simple Syslog Handle corrupt\r\n") );
    return (int) FALSE;  /*! \retval FALSE if the Handle is not valid. */
    }

  /*local_block()*/
    {
    char OutTxt[512];
    _snprintf( OutTxt, DIM(OutTxt), "Syslog for %s:%u closed. App='%s'\r\n",
              pHnd->udp.SendTo_Addr.Text, 
              pHnd->udp.SendTo_Port,
              pHnd->global.AppName.Text );
    OutputDebugString( OutTxt );
    }

  closesocket( pHnd->internal.SndSocket );
  WSACleanup();
  while( ReleaseMutex( pHnd->internal.hMutex ) ){;/*do-a-nothing*/}
  CloseHandle( pHnd->internal.hMutex );

  free( pHnd->internal.pMyCmdLine );
  free( pHnd );
  return (int) TRUE;  /*! \retval TRUE if everything was closed successful. */
}}




/*! \brief WRITING A LOG LINE
 *
 * This function is the heart of the Logging System
 *
 * sample call:
 *  \code
    char const * const mod = "MyModule";
    int hLog;

    hLog = VSSC_open2( "localhost:514", LOG_LOCAL2, LOG_DEBUG, "TestProg" );
    if( hLog )
      {
      VSSC_Log( hLog, SLOG_DEBUG, mod, "Message of typ: 'SILENT' %u", SLOG_EMERG  );
      VSSC_close( hLog );
      }
 *  \endcode
 *
 */
int VSSC_Log( int Handle,                /*!< [in] Handle a valid Handle from succesful VSSC_openX() call */
               unsigned Level,            /*!< [in] Level under which the message shall be handled */
               char const * const Module, /*!< [in] module-name, a short string of, f.i. the file where it is located */
               char const * const fmt,    /*!< [in] THE message itself. can be a printf() compatible string expression */
               ...                        /*!< [in] ... variable list: if fmt contains printf expressions, here have more parameters to follow */
             )
{{
  va_list arg_list;                 /* variable args for printf-mask */
  static int ReentranceBlocker = 0; /* prevent deadlocks when logging inside log system */
  PTS_vssc_data pHnd = (PTS_vssc_data) Handle;
  size_t const logtext_size = 1024; /* maximum size of a syslog-usermessage */
  char logtext[logtext_size+1];
  size_t const logmsg_size = 1024; /* maximum size of a syslog-datagramm */
  char logmsg[logmsg_size+1];
  DWORD dwWaitResult; 
  int iRet;
  unsigned BitMask;


  if( !pHnd || !Module || !fmt )
    {
    OnDebug_ReportT( _T("Err: Null Arg\r\n") );
    return -1; /*! \retval -1 : Error in Parameter, not allowed NULL param was given */
    }
  
  if( (pHnd->SecCookie1 != 0xC001B001) || (pHnd->SecCookie2 != 0xC001B001) )
    {
    OnDebug_ReportT( _T("Simple Syslog Handle corrupt\r\n") );
    return -2; /*! \retval -2 : Invalid or corrupt Handle. This Error can not be caught outside of GPF in every case. */
    }

  BitMask = (Level & LOGZONE_MASK);
  if( (BitMask && !(pHnd->global.ZoneMask & BitMask)) || (BitMask==0 && pHnd->global.ZoneMask!=LOGZONE_EVERYTHING) )
    {
    OnDebug_ReportT( _T("suppressed because of Bitmask\r\n") );
    return 0; /*! \retval 0 : no need to show the message because of Zonemask. */
    }

  /* pHnd->udp.Syslog_MaxLevel = 0 : show nothing
   *               1 : show emergency (LOG_EMERG, 0)
   *               2 : show alert  (LOG_ALERT, 1)
   * so we have a Offset of +1 between both Levels
   */
  Level &= 0x0F; /* Real Level is only lower 4 bit */
  if( Level==0 )
    {
    OnDebug_ReportT( _T("Suppressed because own Level 0\r\n") );
    return 0; /*! \retval 0 : no need to show the message because of too low own Level. */
    }

  Level = _internal_Vssc2syslogLevel( Level );
  if( pHnd->udp.Syslog_MaxLevel==-1L || pHnd->udp.Syslog_MaxLevel < Level )
    {
    OnDebug_ReportT( _T("Suppressed because low Level\r\n") );
    return 0; /*! \retval 0 : no need to show the message because of Lower global Level. */
    }

  /* request Mutex from other Thread or give up if it last longer than, say, 1/10 s
   * If this trashes to many logs, set a higher value or connect a pipe to the final reader
   */
  dwWaitResult = WaitForSingleObject( pHnd->internal.hMutex, 100L);
  /* sorry, lasts too long. The other Thread won the race */
  /* or mutex killed inbetween? Shit ... at least do not log anything */
  if( (dwWaitResult == WAIT_TIMEOUT) || (dwWaitResult == WAIT_ABANDONED) )
    {
    OnDebug_ReportT( _T("Suppressed because reentrace protection blocked the call\r\n") );
    return -3; /*! \retval -3 : cannot log, because reentrace protection blocked the call. */
    }

  /* should never be seen...  */
  if( (dwWaitResult == WAIT_FAILED) || (dwWaitResult != WAIT_OBJECT_0) )
    {
    OnDebug_ReportT( _T("Suppressed because reentrace protection blocked failed to lock the MTX\r\n") );
    assert(0);
    return -4; /*! \retval -4 : cannot log, because reentrace protection blocker faild to lock mutex. */
    }

  /* if a recursing call comes here again, 
   * I don't like to see him. Really. Because it would get INF
   * \todo: am I to histeric? mutex should have blocked it? lets find out later...
   */
  if( ReentranceBlocker==0 )
    {
    char TimeStamp[20] = "";
    char SourceIp[MAX_PATH] = "";
    char PID[16] = "";
    char ModuleName[32+1+32+1] = "";
    char Counter64[16] = "";
    unsigned short FacLev;
    
    ReentranceBlocker++;

    /* Build optionally Parts of the HEADER. */
    /* RFC requires TIMESTAMP plus HOSTNAME (or IP) */
    _internal_GetTimeStamp( pHnd, TimeStamp, DIM(TimeStamp) );
    _internal_GetIPorName( pHnd, SourceIp, DIM(SourceIp) );

    /* Build optionally Parts of the MSG. */
    /* RFC supposing TAG in form of a programm or process name */
    _internal_GetModulePID( pHnd, PID, DIM(PID) );
    _internal_GetModuleSource( pHnd, ModuleName, DIM(ModuleName), Module );
    _internal_GetCounter( pHnd, Counter64, DIM(Counter64) );

    /* now build user provided printf-string to be the RFC 'CONTENT' */
    va_start( arg_list,fmt );
    _vsnprintf( logtext, logtext_size, fmt, arg_list );
    va_end( arg_list );

    /* glue them all together */
    pHnd->internal.MsgCount++;
    FacLev = (pHnd->udp.Syslog_Facility & 0x03f8) | (Level & 0x07);
    if( pHnd->internal.SndSocket > 0 )
      {
      /* we will start with the PRI part (incl. <> brackets) 
       * then folowed by HEADER parts. Our HEADER can be configured to be invalid (shortened)
       * then followed by MSG parts (TAG+CONTENT). Our MSG can be configured to have extralong
       * TAGs. But CONTENT is in any case 'logtext'
       */
      _snprintf( logmsg, logmsg_size, "<%u>%s%s%s%s%s%s\r\n", 
                                      FacLev, 
                                      (pHnd->global.ControlFlags & LOGFLGS_RFC3164_TIMESTAMP)?TimeStamp :"", 
                                      (pHnd->global.ControlFlags & LOGFLGS_RFC3164_SOURCEIP )?SourceIp  :"", 
                                      (pHnd->global.ControlFlags & LOGFLGS_PROCESS_ID       )?PID       :"",
                                      (pHnd->global.ControlFlags & LOGFLGS_MODULE_SOURCE    )?ModuleName:"",
                                      (pHnd->global.ControlFlags & LOGFLGS_COUNTER64        )?Counter64 :"",
                                      logtext );
      /* send it to syslog socket */
      iRet = sendto( pHnd->internal.SndSocket, logmsg, strlen(logmsg), 0, (struct sockaddr*)&pHnd->internal.SndAddr, sizeof(pHnd->internal.SndAddr) );
      if(iRet<1)
        {
        DWORD dwErr = GetLastError();
        OnDebug_ReportT( _T("not sent, sendto() failed\r\n") );
        iRet = - (int) dwErr; /*! \retval -n : remove the '-' and lookup in winerror.h why sendto() failed */
        }
      }
    _snprintf( logmsg, logmsg_size, "%08u: <%u> %s\r\n", GetTickCount(), FacLev, logtext );
    OutputDebugString( logmsg );
    ReentranceBlocker--;
    }
  else
    {
    OnDebug_ReportT( _T("Suppressed because reentrace protection blocker != 0\r\n") );
    }

  ReleaseMutex( pHnd->internal.hMutex );
  return iRet; /*! \retval 0..1024 : count of characters successfully given to UDP Socket Stack */
}}



/*! @} */












/*! \addtogroup control_functions Functions (control behavior)
 *@{
 */

/*! \brief Auto-reading all Parameters at once
 *
 */
int VSSC_AutoCfg( int Handle,         /*!< [in] Handle a valid Handle from succesful VSSC_openX() call */
                  short bAutoConfig   /*!< [in] >0 means this function automatically tries to read all Setup parameters itself. No further VSSC_SetStr() or VSSC_SetInt() is required */
                )
{{
  PTS_vssc_data pHnd = (PTS_vssc_data) Handle;
  int i, iRet=0;
  
  if( !Handle )
    {
    OutputDebugString( _T("Parameter Error\r\n") );
    return (int) FALSE;  /*! \retval FALSE if one Parameter is invalid. */
    }

  if( (pHnd->SecCookie1 != 0xC001B001) || (pHnd->SecCookie2 != 0xC001B001) )
    {
    OutputDebugString( _T("Simple Syslog Handle corrupt\r\n") );
    return (int) FALSE;  /*! \retval FALSE if the Handle is not valid. */
    }

  for( i=0; i<DIM(AutoConfig); i++ )
    {
    switch( AutoConfig[i].ValType )
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
        DWORD   dwValue;
        ULONG  *pStart_UL;
        LONG   *pStart_SL;
        BYTE   *pStart_UC;
        CHAR   *pStart_SC;
        USHORT *pStart_US;
        SHORT  *pStart_SS;
        /* ... and more numerical types can be inserted here ... */

        iRet = VSSC_Log( Handle, SLOG_INFO, pHnd->global.AppName.Text, 
                             "now reading Type %u of Group='%s', Key='%s'. Dflt/u=%lu, Dflt/s=%ld, writeNew=%u. %u bytes can be written at offs %u", 
                             AutoConfig[i].ValType,
                             AutoConfig[i].pGroupName,
                             AutoConfig[i].pKeyName,
                             (ULONG) AutoConfig[i].DefaultVal,
                             (LONG ) AutoConfig[i].DefaultVal,
                             AutoConfig[i].WriteIfNotExist,
                             AutoConfig[i].Size,
                             AutoConfig[i].Offset );
        
        iRet = _internal_ReadConfig( pHnd, AutoConfig[i].pGroupName,
                                           AutoConfig[i].pKeyName,
                                           AutoConfig[i].WriteIfNotExist,
                                           NULL/*string default*/,
                                           NULL/*string buffer*/, 0/*string maxlen*/, 
                                           AutoConfig[i].DefaultVal,
                                           &dwValue );
        if( !iRet )
          dwValue = (DWORD) AutoConfig[i].DefaultVal;

        pHelp  = (char*) pHnd;
        pHelp += AutoConfig[i].Offset;
        pStart_UL = (ULONG *) pHelp;
        pStart_SL = (LONG  *) pHelp;
        pStart_UC = (BYTE  *) pHelp;
        pStart_SC = (CHAR  *) pHelp;
        pStart_US = (USHORT*) pHelp;
        pStart_SS = (SHORT *) pHelp;
        /* ... and more numerical types can be inserted here ... */

        switch( AutoConfig[i].ValType )
          {
          case et_BYTE  : *pStart_UC = (BYTE  ) dwValue; break;
          case et_CHAR  : *pStart_SC = (CHAR  ) dwValue; break;
          case et_USHORT: *pStart_US = (USHORT) dwValue; break;
          case et_SHORT : *pStart_SS = (SHORT ) dwValue; break;
          case et_ULONG : *pStart_UL = (ULONG ) dwValue; break;
          case et_LONG  : *pStart_SL = (LONG  ) dwValue; break;
          /* ... and, finally more numerical types must be inserted here. */
          }
        };break;

      case et_P32STRING:
        {
        size_t maxlen, Size;
        int Size_Offset, delta;
        char *pStart, *pEnd;

        /* calcultae MaxSize of destination field.
         * we do not know, which destination field, but we know the Type (so we can get delta between Text and Len)
         * and we know base addr of text - so we can hook into Len field 
         */
        delta = FIELD_OFFSET( P32STRING, Text ) - FIELD_OFFSET( P32STRING, LastPos );
        Size_Offset = (int)((char*) pHnd);
        Size_Offset+= (int)( AutoConfig[i].Offset - delta );
        Size = (size_t) *( (ULONG*) Size_Offset );

        maxlen = MIN( (size_t) AutoConfig[i].Size, Size );
        iRet = VSSC_Log( Handle, SLOG_INFO, pHnd->global.AppName.Text, 
                             "now reading Type %u of Group='%s', Key='%s'. Dflt=%s, writeNew=%u. %u bytes can be written at offs %u", 
                             AutoConfig[i].ValType,
                             AutoConfig[i].pGroupName,
                             AutoConfig[i].pKeyName,
                             (TCHAR*) AutoConfig[i].DefaultVal,
                             AutoConfig[i].WriteIfNotExist,
                             AutoConfig[i].Size,
                             AutoConfig[i].Offset );

        pStart = (char*) pHnd;
        pStart+= AutoConfig[i].Offset;

        iRet = _internal_ReadConfig( pHnd, AutoConfig[i].pGroupName,
                                           AutoConfig[i].pKeyName,
                                           AutoConfig[i].WriteIfNotExist,
                                           (TCHAR*) AutoConfig[i].DefaultVal,
                                           (TCHAR*) pStart, 
                                           maxlen+1 /* +1 because maxlen is without \0 and our foo is with \0 */, 
                                           0 /*dword default*/,
                                           NULL/*dword pointer*/ );
        if( !iRet )
          {
          pEnd = pStart + (sizeof(TCHAR)*(maxlen));
          _tcsncpy( (TCHAR*) pStart, "", maxlen );
          *((TCHAR*)(pEnd)) = 0;
          }

        };break;
      } /*sw-end*/
    } /*for-end*/ 

               /*! \retval 0: parameter not set. */
  return iRet; /*! \retval not 0: everything went okay. */
}}

/*! \brief Setting a Parameter (string ASCIIZ)
 *
 * This function is used to control (set) different settings.
 * The value who is controlled is given as a string. 
 * It <i>can</i> be used for controlling integer values.
 * Integer values can be prefixed with '0x' or '$' or '&H' for interpreting the given string as an hex value.
 * Integer values can be prefixed with '0d' for interpreting the given string as an decimal value.
 * Integer values can be prefixed with '0b' for interpreting the following up to 32 characters as an binary value.
 *
 * sample call:
 *  \code
    char const * const mod = "MyModule";
    int hLog;

    hLog = VSSC_open2( "localhost:514", LOG_LOCAL2, LOG_DEBUG, "TestProg" );
    if( hLog )
      {
      VSSC_SetStr( hLog, "KeyName1", "Value1" );
      VSSC_SetInt( hLog, "A_Number", 21 );
      VSSC_close( hLog );
      }
 *  \endcode
 *
 */
int VSSC_SetStr( int Handle,              /*!< [in] Handle a valid Handle from succesful VSSC_openX() call */
                  char const * const Key, /*!< [in] Name of the Key which has to be modified. A short string. f.i. <b>"LogHost"</b> */
                  char const * const Val  /*!< [in] THE value which has to be changed. f.i. <b>"192.168.0.254"</b> */
                )
{{
  PTS_vssc_data pHnd = (PTS_vssc_data) Handle;
  int Index, iRet=0;
  
  if( !Handle || !Key || !*Key || !Val )
    {
    OutputDebugString( _T("Parameter Error\r\n") );
    return (int) FALSE;  /*! \retval FALSE if one Parameter is invalid. */
    }

  if( (pHnd->SecCookie1 != 0xC001B001) || (pHnd->SecCookie2 != 0xC001B001) )
    {
    OutputDebugString( _T("Simple Syslog Handle corrupt\r\n") );
    return (int) FALSE;  /*! \retval FALSE if the Handle is not valid. */
    }

  Index = _internal_GetHash( Key );
  switch( Index )
    {
    case CTRL_GENERAL_APPNAME:
      if( strlen(Val) <= pHnd->global.AppName.LastPos )
        {
        strncpy( pHnd->global.AppName.Text, Val, pHnd->global.AppName.LastPos );
        pHnd->global.AppName.Text[ pHnd->global.AppName.LastPos ] = 0;
        iRet = 1;
        }
      break;

    case CTRL_SYSLOG_HOST:
      if( strlen(Val) > 0 )
        {
        iRet = (int) _internal_VsscRefreshAddr( pHnd, Val );
        }
      break;

    default:
      {
      signed long Number=0;
      if(strlen(Val)>2)
        {
        if( (Val[0]=='0') && ('X'==toupper(Val[1])) && (1==sscanf( Val, "%x", &Number )) )
          {/*do nothing, we now have Number*/;
          }
        else if( (('$'==Val[0])  || ('&'==Val[0])) && (1==sscanf( Val+1, "%x", &Number )) )
          {/*do nothing, we now have Number*/;
          }
        else if( ('&'==Val[0]) && ('H'==toupper(Val[1])) && (1==sscanf( Val+2, "%x", &Number )) )
          {/*do nothing, we now have Number*/;
          }
        else if( (Val[0]=='0') && ('D'==toupper(Val[1])) && (1==sscanf( Val+2, "%i", &Number )) )
          {/*do nothing, we now have Number*/;
          }
        else if( (Val[0]=='0') && ('B'==toupper(Val[1])) )
          {
          char Bits[32+1];
          int i, len;
          unsigned long DWord=0, Weight=1;

          strncpy( Bits, Val+2, DIM(Bits) );
          Bits[32]=0;
          /*now right align it*/
          for( len=0 ; (Bits[len]!=0) && ((Bits[len]=='0') || (Bits[len]=='1')); len++ ){;} /*only Count valid chars */
          for( i=0 ; (i<len); i++ )
            {
            Bits[31-i] = Bits[ len-1-i ];
            }
          Bits[32]=0;
          for( ; (i<32); i++ )
            {
            Bits[31-i] = '0';
            }

          len = strlen(Bits);
          for( i=0 ; i<len ; i++,Weight<<=1 )
            {
            DWord |= (Weight * (Bits[31-i]-'0'));
            }
          Number = (signed long) DWord;
          }
        else if( isdigit( Val[0] ) )
          {
          Number = atol( Val );
          }
        else
          {
          return 0; /*! \retval 0: invalid Data Format (integers may be strings with prefixed 0x or 0b) */
          }
        }
      else
        {
        Number = atol( Val );
        }
      return VSSC_SetInt( Handle, Key, Number, LOGFLGS_DEFAULT ); /*! \retval 0: this Parameter is currently not implemented or is not of String Type */
      }
    }
               /*! \retval 0: parameter not set. */
  return iRet; /*! \retval not 0: everything went okay. */
}}

/*! \brief Setting a Parameter (signed long int)
 *
 * This function is used to control (set) different settings.
 * The value who is controlled is given as asigned long int. 
 * For controlling string values see VSSC_SetStr().
 *
 * sample call:
 *  \code
    char const * const mod = "MyModule";
    int hLog;

    hLog = VSSC_open2( "localhost:514", LOG_LOCAL2, LOG_DEBUG, "TestProg" );
    if( hLog )
      {
      VSSC_SetStr( hLog, "KeyName1", "Value1" );
      VSSC_SetInt( hLog, "A_Number", 21 );
      VSSC_close( hLog );
      }
 *  \endcode
 *
 */
int VSSC_SetInt( int Handle,             /*!< [in] Handle a valid Handle from succesful VSSC_openX() call */
                 char const * const Key, /*!< [in] Name of the Key which has to be modified. A short string. f.i. <b>"LogLevel"</b> */
                 const long lVal,        /*!< [in] THE value which has to be changed. f.i. <b>7</b> */
                 const unsigned short Option /*!< [in] optional (if not 0) Flag. f.i. <b>0</b> */
                )
{{
  PTS_vssc_data pHnd = (PTS_vssc_data) Handle;
  int Index, iRet = 0;
  unsigned long ulVal;

  if( !Handle || !Key || !*Key )
    {
    OutputDebugString( _T("Parameter Error\r\n") );
    return (int) FALSE;  /*! \retval FALSE if one Parameter is invalid. */
    }

  if( (pHnd->SecCookie1 != 0xC001B001) || (pHnd->SecCookie2 != 0xC001B001) )
    {
    OutputDebugString( _T("Simple Syslog Handle corrupt\r\n") );
    return (int) FALSE;  /*! \retval FALSE if the Handle is not valid. */
    }

  ulVal = (long) lVal;
  Index = _internal_GetHash( Key );
  switch( Index )
    {
    case CTRL_GENERAL_ZONEMASK:
      {
      ulVal &= LOGZONE_MASK; /* throw away bits that are reserved for other use (level) */
      switch(Option)
        {
        case LOGFLGS_SET_BITS   : { pHnd->global.ZoneMask |=  ulVal; iRet=1; };break;
        case LOGFLGS_CLR_BITS   : { pHnd->global.ZoneMask &= ~ulVal; iRet=1; };break;
        case LOGFLGS_TOGGLE_BITS: { pHnd->global.ZoneMask ^=  ulVal; iRet=1; };break;
        case LOGFLGS_SET_VALUE  : /* explicit fall trough */ 
        default                 : { pHnd->global.ZoneMask  =  ulVal; iRet=1; };break;
        }
      }; break;

    case CTRL_SYSLOG_PORT:
      if( lVal <= USHRT_MAX )
        {
        switch(Option)
            {
            case LOGFLGS_SET_VALUE : { pHnd->udp.SendTo_Port = (unsigned short) lVal; iRet=1; };break;
            default                : { iRet = FALSE; };break; /*! \retval FALSE if the Option is not valid for the given Key. */
            }
        }
      break;

    case CTRL_SYSLOG_FACILITY  :
      switch(Option)
          {
          case LOGFLGS_SET_VALUE : { iRet = _internal_VsscRefreshLevel( pHnd, (short) lVal, -1 ); };break;
          default                : { iRet = FALSE; };break; /*! \retval FALSE if the Option is not valid for the given Key. */
          }
      break;

    case CTRL_SYSLOG_MAXLEVEL  :
      switch(Option)
          {
          case LOGFLGS_SET_VALUE : { iRet = _internal_VsscRefreshLevel( pHnd, -1 , (short) lVal); };break;
          default                : { iRet = FALSE; };break; /*! \retval FALSE if the Option is not valid for the given Key. */
          }
      break;

    default:
       return 0; /*! \retval 0 : this Parameter is currently not implemented or is not of Integer Type */
    }
               /*! \retval 0: parameter not set. */
  return iRet; /*! \retval not 0: everything went okay. */
}}




/*! \brief Reading a Parameter (string ASCIIZ)
 *
 * This function is used to read out preset or previously written settings.
 * The value who is read is given as a ASCIZ string into a supplied buffer. 
 * For reading integer values see VSSC_GetInt().
 *
 * sample call:
 *  \code
    char const * const mod = "MyModule";
    int hLog;

    hLog = VSSC_open2( "localhost:514", LOG_LOCAL2, LOG_DEBUG, "TestProg" );
    if( hLog )
      {
      char Buffer[80] = "";
      long Level;

      if( VSSC_GetStr( hLog, "LogHost", Buffer, DIM(Buffer) ) )
        {
        if( VSSC_GetInt( hLog, "LogLevel", &Level ) )
          {
          printf( "Logging with Level=%ul to Host='%s'\r\n", Buffer, Level );
          }
        }

      VSSC_close( hLog );
      }
 *  \endcode
 *
 */
int VSSC_GetStr( int Handle,              /*!< [in] Handle a valid Handle from succesful VSSC_openX() call */
                  char const * const Key, /*!< [in] Name of the Key which has to be modified. A short string. f.i. <b>"LogLevel"</b> */
                  char * pValBuf,         /*!< [out] a pointer to a buffer which has sufficient space for the Value which was requested to be read */
                  size_t MaxBuf           /*!< [in] Maximum Space in pValBuf, counted in characters, including the 0x00 terminator */
                )
{{
  PTS_vssc_data pHnd = (PTS_vssc_data) Handle;
  int Index, iRet=0;
  
  if( !Handle || !Key || !*Key || !pValBuf || MaxBuf<1 )
    {
    OutputDebugString( _T("Parameter Error\r\n") );
    return (int) FALSE;  /*! \retval FALSE if one Parameter is invalid. */
    }
  MaxBuf--; /* regard the terminating zero */
  *pValBuf='\0';

  if( (pHnd->SecCookie1 != 0xC001B001) || (pHnd->SecCookie2 != 0xC001B001) )
    {
    OutputDebugString( _T("Simple Syslog Handle corrupt\r\n") );
    return (int) FALSE;  /*! \retval FALSE if the Handle is not valid. */
    }

  Index = _internal_GetHash( Key );
  switch( Index )
    {
    case CTRL_GENERAL_VERSION:
      if( MaxBuf >= strlen( VsscVersion ) )
        {
        strcpy( pValBuf, VsscVersion );
        iRet=1;
        }
      break;

    case CTRL_GENERAL_APPNAME:
      if( MaxBuf >= strlen( pHnd->global.AppName.Text ) )
        {
        strcpy( pValBuf, pHnd->global.AppName.Text );
        iRet=1;
        }
      break;

    case CTRL_SYSLOG_HOST:
      if( MaxBuf >= strlen( pHnd->udp.SendTo_Addr.Text ) )
        {
        strcpy( pValBuf, pHnd->udp.SendTo_Addr.Text );
        iRet=1;
        }
      break;

    case CTRL_SYSLOG_OUTBIND:
      if( MaxBuf >= 16 )
        {
        sprintf( pValBuf, "%u.%u.%u.%u:%u", 
                 pHnd->internal.OwnAddr.sin_addr.S_un.S_un_b.s_b1,
                 pHnd->internal.OwnAddr.sin_addr.S_un.S_un_b.s_b2,
                 pHnd->internal.OwnAddr.sin_addr.S_un.S_un_b.s_b3,
                 pHnd->internal.OwnAddr.sin_addr.S_un.S_un_b.s_b4,
                 pHnd->internal.OwnAddr.sin_port );
        iRet=1;
        }
      break;

    default:
       return 0; /*! \retval 0: this Parameter is currently not implemented or is not of String Type */
    }
               /*! \retval 0: parameter not set. */
  return iRet; /*! \retval not 0: everything went okay. */
}}



/*! \brief Reading a Parameter (signed long int)
 *
 * This function is used to read out preset or previously written settings.
 * The value who is controlled is given as a signed long int into a buffer of the same type.
 * For reading string values see VSSC_GetStr().
 *
 * sample call:
 *  \code
    char const * const mod = "MyModule";
    int hLog;

    hLog = VSSC_open2( "localhost:514", LOG_LOCAL2, LOG_DEBUG, "TestProg" );
    if( hLog )
      {
      char Buffer[80] = "";
      long Level;

      if( VSSC_GetStr( hLog, "LogHost", Buffer, DIM(Buffer) ) )
        {
        if( VSSC_GetInt( hLog, "LogLevel", &Level ) )
          {
          printf( "Logging with Level=%ul to Host='%s'\r\n", Buffer, Level );
          }
        }

      VSSC_close( hLog );
      }
 *  \endcode
 *
 */
int VSSC_GetInt( int Handle,              /*!< [in] Handle a valid Handle from succesful VSSC_openX() call */ 
                  char const * const Key, /*!< [in] Name of the Key which has to be modified. A short string. f.i. <b>"LogLevel"</b> */
                  long *plVal             /*!< [out] A Pointer to a signed long int Variable which gets the Value which was requested to be read */
                )
{{
  PTS_vssc_data pHnd = (PTS_vssc_data) Handle;
  int Index, iRet=0;
  long Val;
  
  if( !Handle || !Key || !*Key || !plVal )
    {
    OutputDebugString( _T("Parameter Error\r\n") );
    return (int) FALSE;  /*! \retval FALSE if one Parameter is invalid. */
    }

  if( (pHnd->SecCookie1 != 0xC001B001) || (pHnd->SecCookie2 != 0xC001B001) )
    {
    OutputDebugString( _T("Simple Syslog Handle corrupt\r\n") );
    return (int) FALSE;  /*! \retval FALSE if the Handle is not valid. */
    }
  *plVal=-1L;

  Index = _internal_GetHash( Key );
  switch( Index )
    {
    case CTRL_GENERAL_VERSION:
      Val = atol( VsscVersion );
      break;

    case CTRL_GENERAL_ZONEMASK:
      Val = (long) pHnd->global.ZoneMask;
      break;

    case CTRL_SYSLOG_PORT:
      Val = (long) pHnd->udp.SendTo_Port;
      break;

    case CTRL_SYSLOG_OUTPORT:
      Val = (long) pHnd->internal.OwnAddr.sin_port;
      break;

    case CTRL_SYSLOG_FACILITY:
      Val = (long) pHnd->udp.Syslog_Facility;
      break;

    case CTRL_SYSLOG_MAXLEVEL:
      Val = (long) pHnd->udp.Syslog_MaxLevel;
      break;

    default:
       return 0; /*! \retval 0 : this Parameter is currently not implemented or is not of Integer Type */
    }

  *plVal = Val;
  return 1; /*! \retval not 0: everything went okay. */
}}


/*! \brief Setting a couple of Parameters at once
 *
 * This function is used to control (set) different settings.
 * The value who is controlled is given as a string. For controlling
 * integer values see VSSC_SetInt().
 * For param 2, both common types of a 'list' are allowed: 
 * MultiLine-with-CRLF and C-Stringlists. 
 * - MultiLine-with-CRLF is a simple char-array, each line separated by CR/LF 
 *   and finally the whole chain terminated by a single '\\0'.
 *   It could be, f.i. a complete group of an INI file. 
 * - C-Stringlist is also a char-array but the chain of strings is separated by a single '\0' from the next
 *   and finally terminated the by a double terminator ('\\0\\0').
 *   It could be, f.i. a REG_MULTI_SZ block from the windows registry (but in ANSI instead of UNICODE format).
 *
 * \note The functiontries to autodetect this types. If it can find a CR or LF, it assumes to have
 *       a Multiline and thus, it does'nt need the double terminator. But if the param 2 doesn't
 *       contain whether the CR/LF nor the '\\0\\0', <b>it will crash with a read exception!</b>
 *       So, if there is a chance to have only one line in the buffer, it is recommended to terminate it
 *       with '\\0\\0'. Or, better: use the double terminator in <b>any</b> case.
 *
 * sample call:
 *  \code
    char const * const mod = "MyModule";
    int hLog;

    hLog = VSSC_open2( "localhost:514", LOG_LOCAL2, LOG_DEBUG, "TestProg" );
    if( hLog )
      {
      VSSC_SetAll( hLog, "Key1=1\r\nKey2=Value2\r\nKey3=Value3\r\nKey4=\r\n" );
      VSSC_close( hLog );
      }
 *  \endcode
 *
 */
int VSSC_SetAll( int Handle,             /*!< [in] Handle a valid Handle from succesful VSSC_openX() call */
                 char const * const Keys /*!< [in] CRLF separated list of usually more than one 'Key'='Value' pairs.*/
                )
{{
  PTS_vssc_data pHnd = (PTS_vssc_data) Handle;
  size_t len, all_len, longest_line;
  char *pLine=NULL, *pCR, *pLF, *pKey, *pValue, *ptr;
  char const *pcStr, *pLeft;
  BOOL StringlistType;
  int iRet, iRetOkay=0, iRetBad=0;

  if( !Handle || !Keys || !*Keys )
    {
    OutputDebugString( _T("Parameter Error\r\n") );
    return 0;  /*! \retval 0 if one Parameter is invalid. */
    }

  if( (pHnd->SecCookie1 != 0xC001B001) || (pHnd->SecCookie2 != 0xC001B001) )
    {
    OutputDebugString( _T("Simple Syslog Handle corrupt\r\n") );
    return 0;  /*! \retval 0 if the Handle is not valid. */
    }

  /* let's guess what type we have. May be we can integrate this later in the cutter code */
  longest_line=0;
  if( NULL!=(pcStr = strchr( Keys, '\r' )) || NULL!=(pcStr = strchr( Keys, '\r' )) )
    {
    /* we have CRLF type */
    StringlistType=FALSE;
    all_len = strlen( Keys ) + 2; /* +2 for the terminators*/
    /*now get the longest line*/
    pLeft = Keys;
    do{
      /*CR, LF, CRLF will be handled properly. We get the rightmost linefeed char.*/
      pCR = strchr( pLeft, '\r' );
      pLF = strchr( pLeft, '\n' );
      /*-------------------------------------------------*
       * case 1) \r\n
       * case 2) \n\r
       * case 3) \r and \n, but \n not adjectant to \r
       * case 4) \n and \r, but \r not adjectant to \n
       * case 5) \r
       * case 6) \n
       * case 7) none of them
       *-------------------------------------------------*/
      if( pCR && pLF && (pCR+1==pLF) )
        {pcStr=pLF;}
      else if( pCR && pLF && (pLF+1==pCR) )
        {pcStr=pCR;}
      else if( pCR && pLF && (pCR+1<pLF) )
        {pcStr=pCR;}
      else if( pCR && pLF && (pLF+1<pCR) )
        {pcStr=pLF;}
      else if( pCR && pLF==NULL )
        {pcStr=pCR;}
      else if( pLF && pCR==NULL )
        {pcStr=pLF;}
      else
        {pcStr=pLeft+strlen(pLeft);}
      /*----*/
      len = pcStr - pLeft + 1 + 1; /* +1 to be correct, +1 for termchar */
      if( longest_line < len )
          longest_line = len;
      pLeft = pcStr + 1;
      } while( *pLeft );
    }
  else
    {
    /* we have Stringlist type */
    StringlistType=TRUE;
    all_len = 0;
    pcStr = Keys;
    do{
      len = strlen( pcStr ) + 1;
      if( len>longest_line) 
        longest_line=len;
      pcStr = pcStr + len; /* move pcStr to the one char AFTER line terminator */
      all_len += len; /* respect the characters plus their terminator */
      } while( *pcStr ); /* if this char AFTER is not the second terminator, continue */
    all_len++; /* regard the second terminator */
    }

  /* now the cutter code */
  pLine = (char*) malloc(longest_line+2);
  if( !Handle || !Keys || !*Keys )
    {
    OutputDebugString( _T("OutOfMem\r\n") );
    return 0;  /*! \retval 0 if out of MEM. */
    }

  pLeft = Keys;
  do{
    strncpy( pLine, pLeft, longest_line );
    *(pLine+longest_line+0)='\0';
    *(pLine+longest_line+1)='\0';

    if(!StringlistType)
      {
      /*CR, LF, CRLF will be handled properly. We get the rightmost linefeed char.*/
      pCR = strchr( pLine, '\r' );
      pLF = strchr( pLine, '\n' );
      /*-------------------------------------------------*
       * case 1) \r\n
       * case 2) \n\r
       * case 3) \r and \n, but \n not adjectant to \r
       * case 4) \n and \r, but \r not adjectant to \n
       * case 5) \r
       * case 6) \n
       * case 7) none of them
       *-------------------------------------------------*/
      if( pCR && pLF && (pCR+1==pLF) )
        {ptr=pLF;}
      else if( pCR && pLF && (pLF+1==pCR) )
        {ptr=pCR;}
      else if( pCR && pLF && (pCR+1<pLF) )
        {ptr=pCR;}
      else if( pCR && pLF && (pLF+1<pCR) )
        {ptr=pLF;}
      else if( pCR && pLF==NULL )
        {ptr=pCR;}
      else if( pLF && pCR==NULL )
        {ptr=pLF;}
      else
        {ptr=NULL;}
      /*----*/
      if(ptr)
        {
        *ptr='\0'; /*cut the rightmost linefeed, set terminator instead. */
        pLeft += (ptr - pLine) + 1; /* one char behind terminator 0 */
        }
      else
        {
        pLeft = NULL; /*Last Line, no CR/LF*/
        }
      }
    else
      {
      pLeft += strlen(pLine) + 1; /* one char behind terminator 0 */
      }

    TrimmLine( pLine );
    if( !IsComment(pLine) && Separate( pLine, &pValue ) )
      {
      pKey=pLine;
      TrimmLine( pKey );
      TrimmLine( pValue );

      iRet = VSSC_SetStr( Handle, pKey, pValue );
      if(iRet==0) 
        iRetBad--;
      else
        iRetOkay++;
      }
    } while( pLeft && *pLeft );

  free( pLine );

  if(iRetBad==0) 
    return iRetOkay; /*! \retval n>0: all n Params properly set. */
  else
    return iRetBad; /*! \retval n<0: n of the given Params couldn't set. */
}}







/*! \brief List all valid Parameter Key Names
 *
 * This function is used to give a hint which Strings are Valis for 'Key' Param of the functions 
 * - VSSC_SetStr
 * - VSSC_SetInt
 * - VSSC_GetStr
 * - VSSC_GetInt
 *
 * The list is given in a human readable but easily parseable format.
 *
 * sample call:
 *  \code
    char const * const mod = "MyModule";
    char Buffer[512];

    if( VSSC_GetValidKeyNames( Buffer, DIM(Buffer) ) )
      {
      printf( "The following key names are valid:\r\n%s\r\n", Buffer );
      }
 *  \endcode
 *
 */
int VSSC_GetValidKeyNames( char * Buffer,    /*!< [out] a Buffer for aprx. > 128 characters. After succesfully called, this Buffer contains an ASCIIZ string with a multilined List of all Keys */
                           size_t MaxBuffer  /*!< [in] Maximum Space in Buffer, counted in characters, including the 0x00 terminator */
                         )
{{
  if( !Buffer || MaxBuffer<128 )
    {
    OutputDebugString( _T("Parameter Error\r\n") );
    return 0;  /*! \retval 0 : Buffer too small */
    }

  return _internal_GetListOfCommands( Buffer, MaxBuffer );
}}

/*! @} */



/*! \cond NEVER_DOX */
void TrimmLine( char * pLine )
{{
  size_t len;
  char *ptr;

  if(!pLine)
    return;
  
  len = strlen(pLine);
  ptr = pLine+len-1; /*last char*/
  len++; /*with termchar now*/
  while(*ptr<=' ')
    {
    *ptr-- = 0;
    len--;
    }
  ptr = pLine;
  while(*ptr<=' ')
    {
    ptr++;
    len--;
    }
  memmove( pLine, ptr, len );
  return;
}};

BOOL IsComment( char * pLine )
{{
  if(!pLine)
    return TRUE;

  if( (!*pLine) || (*pLine==';') || (*pLine=='#') || ((*pLine=='/') && (*(pLine+1)=='/')) )
    return TRUE;

  return FALSE;
}};


BOOL Separate( char * pLine, char ** ppRightpart )
{{
  char *ptr;

  if( !pLine || !ppRightpart )
    return FALSE;

  ptr=strchr( pLine, '=' );
  if(ptr)
    {
    *ptr=0;
    *ppRightpart=ptr+1;
    return TRUE;
    }

  *ppRightpart="";
  return FALSE;
}};
/*! \endcond */



#ifdef __cplusplus
  }
#endif

  
/*<EOF>*/
