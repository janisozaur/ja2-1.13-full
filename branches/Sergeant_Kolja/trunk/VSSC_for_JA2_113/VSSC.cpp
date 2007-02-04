/* _______________________________________________________
 * VSSC.cpp : a very Simple Sylog Client
 *
 * Copyright :
 * (c) 2006, Sergeant_Kolja@yahoo.de (aka Modem-Man)
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

/*! file VSSC.cpp
 *  \brief VSSC basic interface
 *
 *  Details following below:
 */



/* _______________________________________________________
 *
 * ToDo:
 *   exported foo for setting Level, Fac, Mask
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




#ifdef __cplusplus
  extern "C" {
#endif



/*========================================================*/
/*== internal Handle structure                          ==*/
/*========================================================*/
typedef struct vssc_data_
  {
  int            SecCookie1;
  /* -------------------- */
  WSADATA        wsaData;
  SOCKET         SndSocket;
  SOCKADDR_IN    SndAddr;
  SOCKADDR_IN    OwnAddr;
  HANDLE         hMutex;
  char           SendTo_Addr[MAX_PATH];
  unsigned short SendTo_Port;
  unsigned short From_Port;
  unsigned       Facility;
  unsigned       Level;
  unsigned       ZoneMask;
  unsigned       ControlFlags;
  char           AppName[32+1];
  unsigned short MsgCount;
  /* -------------------- */
  int            SecCookie2;
  } TS_vssc_data, *PTS_vssc_data;




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

/*========================================================*/
/*== internal helper functions                          ==*/
/*========================================================*/
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
    pHnd->Facility = (unsigned) Facility;
    bRet = TRUE;
    }
  if(Level>=0)
    {
    /* pHnd->Level = 0 : show nothing
     *               1 : show emergency (LOG_EMERG, 0)
     *               2 : show alert  (LOG_ALERT, 1)
     * so we have a Offset of +1 between both Levels
     */
    pHnd->Level = (unsigned) Level+1;
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






/*========================================================*/
/*== PUBLIC functions                                   ==*/
/*========================================================*/

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
  pHnd->ControlFlags = LOGFLGS_RFC3164_TIMESTAMP | LOGFLGS_MODULE_SOURCE | LOGFLGS_RFC3164_SOURCEIP | LOGFLGS_COUNTER64;
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
  /* usually, there is no need for binding UDP sockets, but later we will send simply with send() instead of sendto(), so this makes things more ease */
  iRet = bind( pHnd->SndSocket, (struct sockaddr *)&pHnd->OwnAddr, sizeof(pHnd->OwnAddr) );
  if( iRet == SOCKET_ERROR )
    {
    OutputDebugString( _T("Simple Syslog Client: bind() FAILED\r\n") );
    free( pHnd );
    return 0; /*! \retval 0 cannot bind the outgoing udp port. open failed */
    }

  /*local_block()*/
    {
    char OutTxt[512];
    _snprintf( OutTxt, DIM(OutTxt), "Syslog for %s:%u open and ready. App='%s', Fac=%u, Lev=%u\r\n",
              pHnd->SendTo_Addr, 
              pHnd->SendTo_Port,
              pHnd->AppName, 
              pHnd->Facility, 
              pHnd->Level );
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
  if( (pHnd->SecCookie1 != 0xC001B001) || (pHnd->SecCookie2 != 0xC001B001) )
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




void VSSC_Log( int Handle,                /*!< [in] Handle a valid Handle from succesful VSSC_openX() call */
               unsigned Level,            /*!< [in] Level under which the message shall be handled */
               char const * const Module, /*!< [in] module-name, a short string of, f.i. the file where it is located */
               char const * const fmt,    /*!< [in] THE message itself. can be a printf() compatible string expression */
               ...                        /*!< [in] if fmt contains printf expressions, here have more parameters to follow */
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
    return;
    }
  
  if( (pHnd->SecCookie1 != 0xC001B001) || (pHnd->SecCookie2 != 0xC001B001) )
    {
    OutputDebugString( _T("Simple Syslog Handle corrupt\r\n") );
    return;
    }

  BitMask = (Level & 0xFFFFFF00) >> 8;
  if( BitMask && !(pHnd->ZoneMask & BitMask) )
    {
    return;
    }

  /* pHnd->Level = 0 : show nothing
   *               1 : show emergency (LOG_EMERG, 0)
   *               2 : show alert  (LOG_ALERT, 1)
   * so we have a Offset of +1 between both Levels
   */
  Level &= 0xFF; /* Real Level is only lower 8 bit */
  if( Level==0 )
    {
    return;
    }

  Level = _internal_Vssc2syslogLevel( Level );
  if( pHnd->Level < Level )
    {
    return;
    }

  /* request Mutex from other Thread or give up if it last longer than, say, 1/10 s
   * If this trashes to many logs, set a higher value or connect a pipe to the final reader
   */
  dwWaitResult = WaitForSingleObject( pHnd->hMutex, 100L);
  /* sorry, lasts too long. The other Thread won the race */
  /* or mutex killed inbetween? Shit ... at least do not log anything */
  if( (dwWaitResult == WAIT_TIMEOUT) || (dwWaitResult == WAIT_ABANDONED) )
    {return;}

  /* should never be seen...  */
  if( (dwWaitResult == WAIT_FAILED) || (dwWaitResult != WAIT_OBJECT_0) )
    {
    assert(0);
    return;
    }

  /* if a recursing call comes here again, 
   * I don't like to see him. Really. Because it would get INF
   * \todo: am I to histeric? mutex should have blocked it? lets find out later...
   */
  if( ReentranceBlocker==0 )
    {
    time_t now;
    struct tm * time_s;
    char TimeStamp[20];
    char SourceIp[MAX_PATH];
    char Counter64[16];
    char ModuleName[32+1+32+1];
    unsigned short FacLev;
    
    ReentranceBlocker++;

    *TimeStamp=0;
    if(pHnd->ControlFlags & LOGFLGS_RFC3164_TIMESTAMP)
      {
      now   = time(NULL);
      time_s = localtime(&now);
      strftime( TimeStamp, DIM(TimeStamp), "%b %d %H:%M:%S ", time_s);
      if( TimeStamp[4]=='0' ) /* RFC3164 requirement: single digit Day with one more space */
          TimeStamp[4] =' ';
      }

    *SourceIp=0;
    if(pHnd->ControlFlags & LOGFLGS_RFC3164_SOURCEIP )
      {
      int len;
      gethostname( SourceIp, DIM(SourceIp) );
      len = strlen(SourceIp);
      if( len<DIM(SourceIp)-1 )
        {
        strcat( SourceIp, " " );
        }
      }
  
    *ModuleName=0;
    if(pHnd->ControlFlags & LOGFLGS_MODULE_SOURCE)
      {
      strncpy( ModuleName, pHnd->AppName, 32 );
      ModuleName[32]=0;
      strcat( ModuleName, "." );
      strncat( ModuleName, Module, 32 );
      ModuleName[DIM(ModuleName)-2]=0;
      strcat( ModuleName, " " );
      }
    
    *Counter64=0;
    if(pHnd->ControlFlags & LOGFLGS_COUNTER64)
      {
      _snprintf( Counter64, DIM(Counter64), "{%02u} ", pHnd->MsgCount & 0x3F );
      }


    va_start( arg_list,fmt );
    _vsnprintf( logtext, logtext_size, fmt, arg_list );
    va_end( arg_list );

    pHnd->MsgCount++;
    FacLev = (pHnd->Facility & 0x03f8) | (Level & 0x07);
    if( pHnd->SndSocket > 0 )
      {
      _snprintf( logmsg, logmsg_size, "<%u>%s%s%s%s%s\r\n", 
                                      FacLev, 
                                      (pHnd->ControlFlags & LOGFLGS_RFC3164_TIMESTAMP)?TimeStamp :"", 
                                      (pHnd->ControlFlags & LOGFLGS_RFC3164_SOURCEIP )?SourceIp  :"", 
                                      (pHnd->ControlFlags & LOGFLGS_MODULE_SOURCE    )?ModuleName:"",
                                      (pHnd->ControlFlags & LOGFLGS_COUNTER64        )?Counter64 :"",
                                      logtext );
      iRet = sendto( pHnd->SndSocket, logmsg, strlen(logmsg), 0, (struct sockaddr*)&pHnd->SndAddr, sizeof(pHnd->SndAddr) );
      if(iRet<1)
        {
        DWORD dwErr = GetLastError();
        }
      }
    _snprintf( logmsg, logmsg_size, "%08u: <%u> %s\r\n", GetTickCount(), FacLev, logtext );
    OutputDebugString( logmsg );
    ReentranceBlocker--;
    }

  ReleaseMutex( pHnd->hMutex );
  return;
}}


#ifdef __cplusplus
  }
#endif

  
/*<EOF>*/
