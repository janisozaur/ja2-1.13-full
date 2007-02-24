/* _______________________________________________________
 * VSSC.cpp : a very Simple Sylog Client
 *
 * Copyright :
 * (c) 2006, SergeantKolja(at)yahoo.de (aka Modem-Man)
 * all rights reserved
 * 
 * don't forget to include license.txt in every 
 * distribution of this files. 
 * Even, don't forget to read them.
 * _______________________________________________________
 */

#include "stdafx.h" /* we assume to have all global & common project settings in this file */
#include "vssc.h"

#include <windows.h>
#include <winsock2.h>
#include <stdlib.h>
#include <tchar.h>
#include <assert.h>
#include <time.h>
#include <limits.h>

char const * const VsscVersion = "0.1.737.build";

/*! \file VSSC.cpp
 *  \brief VSSC basic functions interface
 *
 *  Details following below:
 *
 * \author Sergeant Kolja (Initial Author)<br>
 *
 * \todo here follows a list what I think what has to be done in this file:
 *   - Reading Registry  HKLM/Software/vssc/appname
 *   - writing + reading HKCU ...
 *   - LogLevel from Registry
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




#ifdef __cplusplus
  extern "C" {
#endif



/*========================================================*/
/*== internal Handle structure                          ==*/
/*========================================================*/

/*! \cond NEVER_DOX */
typedef struct vssc_data_
  {
  int            SecCookie1;
  /* --------------------------- */
  WSADATA        wsaData;
  SOCKET         SndSocket;
  SOCKADDR_IN    SndAddr;
  SOCKADDR_IN    OwnAddr;
  HANDLE         hMutex;
  unsigned short MsgCount;
  DWORD          UserPID;
  /* ------[direct setables]---- */
  char           AppName[32+1];
  unsigned       ZoneMask;
  char           SendTo_Addr[MAX_PATH];
  unsigned short SendTo_Port;
  unsigned short From_Port;
  unsigned       Syslog_Facility;
  unsigned       Syslog_MaxLevel;
  /* ------[indirect setables]-- */
  unsigned       ControlFlags;
  /* --------------------------- */
  int            SecCookie2;
  } TS_vssc_data, *PTS_vssc_data;
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
__inline
void OnDebug_ReportT( TCHAR const * const pMessage )
{{
  #if defined( DEBUG ) || defined( _DEBUG )
  OutputDebugString( pMessage );
  #endif
  return;
}}
/*! \endcond */





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
    pHnd->SendTo_Port = atoi( p );
    }

  if( 0 == pHnd->SendTo_Port )
    {
    pHnd->SendTo_Port = 514;
    }

  p = _internal_IpFromName( pHnd, DestCopy, IPAddr, DIM(IPAddr) );
  strncpy( pHnd->SendTo_Addr, (p && *p) ? p : DestCopy, DIM(pHnd->SendTo_Addr) );
  pHnd->SendTo_Addr[ DIM(pHnd->SendTo_Addr)-1 ]=0;

  return TRUE;
}};





/* takes Facility and/or Level and stores it into handle struct.
 * if -1 is given, the parameter is not used. if so, return is FALSE, else TRUE.
 */
static
BOOL _internal_VsscRefreshLevel( PTS_vssc_data pHnd, short Facility, short Level )
{{
  BOOL bRet = FALSE;

  if(Facility>=0)
    {
    pHnd->Syslog_Facility = (unsigned) Facility;
    bRet = TRUE;
    }
  if(Level>=0)
    {
    /* pHnd->Syslog_MaxLevel = 0 : show nothing
     *               1 : show emergency (LOG_EMERG, 0)
     *               2 : show alert  (LOG_ALERT, 1)
     * so we have a Offset of +1 between both Levels
     */
    pHnd->Syslog_MaxLevel = (unsigned) Level-1;
    bRet = TRUE;
    }

  return bRet;
}};







/* takes ZoneMask and stores it into handle struct.
 */
static
BOOL _internal_VsscRefreshZone( PTS_vssc_data pHnd, unsigned ZoneMask )
{{
  pHnd->ZoneMask = ZoneMask;
  return TRUE;
}};



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




/* returns okay if the time stamp is filled or - if the ts not used - if it is properly set empty */
static
BOOL _internal_GetTimeStamp( PTS_vssc_data pHnd, char * pTimeStamp, size_t MaxTimeStamp )
{{
  time_t now;
  struct tm * time_s;

  if( !pTimeStamp || MaxTimeStamp<20 )
    return FALSE;

  *pTimeStamp='\0';
  if(pHnd->ControlFlags & LOGFLGS_RFC3164_TIMESTAMP)
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


/* returns okay if the IP/Hostname is filled or - if the ip/hn not used - if it is properly set empty */
static
BOOL _internal_GetIPorName( PTS_vssc_data pHnd, char * pSourceIP, size_t MaxSourceIP )
{{
  if( !pSourceIP || MaxSourceIP<20 )
    return FALSE;

  *pSourceIP='\0';
  if(pHnd->ControlFlags & LOGFLGS_RFC3164_SOURCEIP )
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



/* returns okay if the PID is filled or - if the PID not used - if it is properly set empty */
static
BOOL _internal_GetModulePID( PTS_vssc_data pHnd, char *pPID, size_t MaxPID )
{{
  if( !pPID || MaxPID<10 )
    return FALSE;

  *pPID='\0';
  if(pHnd->ControlFlags & LOGFLGS_PROCESS_ID)
    {
    _snprintf( pPID, MaxPID, "%08x ", pHnd->UserPID );
    }
  return TRUE;
}};



/* returns okay if the Sourcename is filled or - if the sn not used - if it is properly set empty */
static
BOOL _internal_GetModuleSource( PTS_vssc_data pHnd, char *pModuleName, size_t MaxModuleName, char const * const pModule )
{{
  if( !pModuleName || MaxModuleName<1 )
    return FALSE;

  *pModuleName='\0';
  if(pHnd->ControlFlags & LOGFLGS_MODULE_SOURCE)
    {
    size_t Leftpart, Rightpart, MaxSpace, Remainder;

    if(pHnd->ControlFlags & LOGFLGS_LONGTAG)
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

    strncpy( pModuleName, pHnd->AppName, Remainder );
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


/* returns okay if the Counter is filled or - if the counter not used - if it is properly set empty */
static
BOOL _internal_GetCounter( PTS_vssc_data pHnd, char *pCounter64, size_t MaxCounter64 )
{{
  if( !pCounter64 || MaxCounter64<6 )
    return FALSE;

  *pCounter64='\0';
  if(pHnd->ControlFlags & LOGFLGS_COUNTER64)
    {
    _snprintf( pCounter64, MaxCounter64, "{%02u} ", pHnd->MsgCount & 0x3F );
    }
  return TRUE;
}};




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
  return VSSC_open2( NULL, -1, -1, NULL ); /*! \retval same as VSSC_open2() */
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
                char const * pAppName      /*!< [in] Show all logs as comming from.... may be NULL or "" for don't care */
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

  pHnd->hMutex = CreateMutex( NULL, FALSE, NULL );
  if( !pHnd->hMutex ) /* Check for error. */
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

  iRet = WSAStartup( MAKEWORD(2,2), &pHnd->wsaData );
  pHnd->SndSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
  if( pHnd->SndSocket == INVALID_SOCKET)
    {
    OutputDebugString( _T("Simple Syslog Client: socket() FAILED\r\n") );
    free( pHnd );
    return 0; /*! \retval 0 cannot create a IP socket. open failed */
    }

  if( !pDestination || pDestination ) /* if no destination was given, default 'syslog @ this pc' */
    {
    pDestination = "localhost:514";
    }
  _internal_VsscRefreshAddr( pHnd, pDestination );
  _internal_VsscRefreshLevel(pHnd, (short)((Facility>=0) ? Facility : LOG_LOCAL7), 
                                   (short)((Level   >=0) ? Level    : LOG_INFO ));
  _internal_VsscRefreshZone( pHnd, LOGZONE_EVERYTHING );

  *pHnd->AppName=0;
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
      strncpy( pHnd->AppName, p, DIM(pHnd->AppName) ); /**/
      pHnd->AppName[DIM(pHnd->AppName)-1]=0;
      }
    }
  else
    {
    strncpy( pHnd->AppName, pAppName, DIM(pHnd->AppName) );
    pHnd->AppName[DIM(pHnd->AppName)-1]=0;
    }
  

  pHnd->MsgCount = 0;

# ifdef _DEBUG
  pHnd->ControlFlags = LOGFLGS_RFC3164_TIMESTAMP | LOGFLGS_MODULE_SOURCE | LOGFLGS_RFC3164_SOURCEIP | LOGFLGS_COUNTER64 | LOGFLGS_PROCESS_ID | LOGFLGS_LONGTAG;
# else
  pHnd->ControlFlags = LOGFLGS_RFC3164_TIMESTAMP | LOGFLGS_MODULE_SOURCE | LOGFLGS_COUNTER64;
# endif

  
  memset( &pHnd->SndAddr, 0x00, sizeof( struct sockaddr_in ));
  pHnd->SndAddr.sin_family = AF_INET;
  pHnd->SndAddr.sin_port = htons( pHnd->SendTo_Port );
  pHnd->SndAddr.sin_addr.s_addr = inet_addr( pHnd->SendTo_Addr );

  pHnd->From_Port=0;
  memset( &pHnd->OwnAddr, 0x00, sizeof( struct sockaddr_in ));
  pHnd->OwnAddr.sin_family = AF_INET;
  pHnd->OwnAddr.sin_port = htons( pHnd->From_Port );
  pHnd->OwnAddr.sin_addr.s_addr = INADDR_ANY;

  iRet = setsockopt( pHnd->SndSocket, SOL_SOCKET, SO_REUSEADDR, (const char*) &Blocking, sizeof(Blocking) );

# if 0
  /* old: usually, there is no need for binding UDP sockets, 
   *      but later we will send simply with send() instead of sendto(), 
   *      so this makes things more ease.
   * new: we now send using sendto(), so we can change the log target on the fly
   */
  iRet = bind( pHnd->SndSocket, (struct sockaddr *)&pHnd->OwnAddr, sizeof(pHnd->OwnAddr) );
  if( iRet == SOCKET_ERROR )
    {
    OutputDebugString( _T("Simple Syslog Client: bind() FAILED\r\n") );
    free( pHnd );
    return 0; /*! \retval 0 cannot bind the outgoing udp port. open failed */
    }
# endif
  pHnd->UserPID = GetCurrentProcessId();

  /*local_block()*/
    {
    char OutTxt[512];
    _snprintf( OutTxt, DIM(OutTxt), "Syslog for %s:%u open and ready. App='%s', Fac=%u, Lev=%u\r\n",
              pHnd->SendTo_Addr, 
              pHnd->SendTo_Port,
              pHnd->AppName, 
              pHnd->Syslog_Facility, 
              pHnd->Syslog_MaxLevel );
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
              pHnd->SendTo_Addr, 
              pHnd->SendTo_Port,
              pHnd->AppName );
    OutputDebugString( OutTxt );
    }

  closesocket( pHnd->SndSocket );
  WSACleanup();
  while( ReleaseMutex( pHnd->hMutex ) ){;/*do-a-nothing*/}
  CloseHandle( pHnd->hMutex );

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
  if( (BitMask && !(pHnd->ZoneMask & BitMask)) || (BitMask==0 && pHnd->ZoneMask!=LOGZONE_EVERYTHING) )
    {
    OnDebug_ReportT( _T("suppressed because of Bitmask\r\n") );
    return 0; /*! \retval 0 : no need to show the message because of Zonemask. */
    }

  /* pHnd->Syslog_MaxLevel = 0 : show nothing
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
  if( pHnd->Syslog_MaxLevel==-1L || pHnd->Syslog_MaxLevel < Level )
    {
    OnDebug_ReportT( _T("Suppressed because low Level\r\n") );
    return 0; /*! \retval 0 : no need to show the message because of Lower global Level. */
    }

  /* request Mutex from other Thread or give up if it last longer than, say, 1/10 s
   * If this trashes to many logs, set a higher value or connect a pipe to the final reader
   */
  dwWaitResult = WaitForSingleObject( pHnd->hMutex, 100L);
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
    pHnd->MsgCount++;
    FacLev = (pHnd->Syslog_Facility & 0x03f8) | (Level & 0x07);
    if( pHnd->SndSocket > 0 )
      {
      /* we will start with the PRI part (incl. <> brackets) 
       * then folowed by HEADER parts. Our HEADER can be configured to be invalid (shortened)
       * then followed by MSG parts (TAG+CONTENT). Our MSG can be configured to have extralong
       * TAGs. But CONTENT is in any case 'logtext'
       */
      _snprintf( logmsg, logmsg_size, "<%u>%s%s%s%s%s%s\r\n", 
                                      FacLev, 
                                      (pHnd->ControlFlags & LOGFLGS_RFC3164_TIMESTAMP)?TimeStamp :"", 
                                      (pHnd->ControlFlags & LOGFLGS_RFC3164_SOURCEIP )?SourceIp  :"", 
                                      (pHnd->ControlFlags & LOGFLGS_PROCESS_ID       )?PID       :"",
                                      (pHnd->ControlFlags & LOGFLGS_MODULE_SOURCE    )?ModuleName:"",
                                      (pHnd->ControlFlags & LOGFLGS_COUNTER64        )?Counter64 :"",
                                      logtext );
      /* send it to syslog socket */
      iRet = sendto( pHnd->SndSocket, logmsg, strlen(logmsg), 0, (struct sockaddr*)&pHnd->SndAddr, sizeof(pHnd->SndAddr) );
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

  ReleaseMutex( pHnd->hMutex );
  return iRet; /*! \retval 0..1024 : count of characters successfully given to UDP Socket Stack */
}}



/*! @} */












/*! \addtogroup control_functions Functions (control behavior)
 *@{
 */


/*! \brief Setting a Parameter (string ASCIIZ)
 *
 * This function is used to control (set) different settings.
 * The value who is controlled is given as a string. For controlling
 * integer values see VSSC_SetInt().
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
      if( strlen(Val)<DIM(pHnd->AppName) )
        {
        strcpy( pHnd->AppName, Val );
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
       return 0; /*! \retval 0: this Parameter is currently not implemented or is not of String Type */
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
int VSSC_SetInt( int Handle,              /*!< [in] Handle a valid Handle from succesful VSSC_openX() call */
                  char const * const Key, /*!< [in] Name of the Key which has to be modified. A short string. f.i. <b>"LogLevel"</b> */
                  const long lVal         /*!< [in] THE value which has to be changed. f.i. <b>7</b> */
                )
{{
  PTS_vssc_data pHnd = (PTS_vssc_data) Handle;
  int Index, iRet = 0;
  
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

  Index = _internal_GetHash( Key );
  switch( Index )
    {
    case CTRL_GENERAL_ZONEMASK:
      if( lVal>0xF || lVal==0 )
        {
        pHnd->ZoneMask = (unsigned long int) lVal;
        iRet=1;
        }
      break;

    case CTRL_SYSLOG_PORT:
      if( lVal <= USHRT_MAX )
        {
        pHnd->SendTo_Port = (unsigned short int) lVal;
        iRet=1;
        }
      break;

    case CTRL_SYSLOG_FACILITY  :
      iRet = _internal_VsscRefreshLevel( pHnd, (short) lVal, -1 );
      break;

    case CTRL_SYSLOG_MAXLEVEL  :
      iRet = _internal_VsscRefreshLevel( pHnd, -1 , (short) lVal);
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
      if( MaxBuf >= strlen( pHnd->AppName ) )
        {
        strcpy( pValBuf, pHnd->AppName );
        iRet=1;
        }
      break;

    case CTRL_SYSLOG_HOST:
      if( MaxBuf >= strlen( pHnd->SendTo_Addr ) )
        {
        strcpy( pValBuf, pHnd->SendTo_Addr );
        iRet=1;
        }
      break;

    case CTRL_SYSLOG_OUTBIND:
      if( MaxBuf >= 16 )
        {
        sprintf( pValBuf, "%u.%u.%u.%u:%u", 
                 pHnd->OwnAddr.sin_addr.S_un.S_un_b.s_b1,
                 pHnd->OwnAddr.sin_addr.S_un.S_un_b.s_b2,
                 pHnd->OwnAddr.sin_addr.S_un.S_un_b.s_b3,
                 pHnd->OwnAddr.sin_addr.S_un.S_un_b.s_b4,
                 pHnd->OwnAddr.sin_port );
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
      Val = (long) pHnd->ZoneMask;
      break;

    case CTRL_SYSLOG_PORT:
      Val = (long) pHnd->SendTo_Port;
      break;

    case CTRL_SYSLOG_OUTPORT:
      Val = (long) pHnd->OwnAddr.sin_port;
      break;

    case CTRL_SYSLOG_FACILITY:
      Val = (long) pHnd->Syslog_Facility;
      break;

    case CTRL_SYSLOG_MAXLEVEL:
      Val = (long) pHnd->Syslog_MaxLevel;
      break;

    default:
       return 0; /*! \retval 0 : this Parameter is currently not implemented or is not of Integer Type */
    }

  *plVal = Val;
  return 1; /*! \retval not 0: everything went okay. */
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



#ifdef __cplusplus
  }
#endif

  
/*<EOF>*/
