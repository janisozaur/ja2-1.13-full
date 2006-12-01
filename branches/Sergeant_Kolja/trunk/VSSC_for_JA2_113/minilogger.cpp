/* _______________________________________________________
 * minilogger.cpp : currently only a starting platform
 *                  for a (later to be sorted) logger
 *
 * Copyright :
 * (c) 2006, Sergeant_Kolja@yahoo.de (aka Modem-Man)
 * all rights reserved
 * 
 * don't forget to include license.txt in every 
 * distribution of this files. Event, don'T forget to read 
 * them.
 * _______________________________________________________
 */

#include "stdafx.h"

#include <windows.h>
#include <winsock2.h>
#include <stdlib.h>
#include <tchar.h>
#include <assert.h>
#include <time.h>


#if !defined(DIM)
#  define DIM(x)        (sizeof(x)/sizeof(x[0]))
#endif

#if !defined(MIN)
# define MIN(a,b)       ((a) < (b) ? (a) : (b))
# define MAX(a,b)       ((a) > (b) ? (a) : (b))
#endif


/*---- Severyties ---------*/
#  define  LOG_EMERG   0  
#  define  LOG_ALERT   1  
#  define  LOG_CRIT    2  
#  define  LOG_ERR     3  
#  define  LOG_WARNING 4  
#  define  LOG_NOTICE  5  
#  define  LOG_INFO    6  
#  define  LOG_DEBUG   7

#  define  LOG_KERN      (0<<3) /*!< Facility : Kernel */
#  define  LOG_USER      (1<<3) /*!< Facility : User Process*/
#  define  LOG_MAIL      (2<<3) /*!< Facility : internal Mail & eMail */
#  define  LOG_DAEMON    (3<<3) /*!< Facility : Drivers and Deamons */
#  define  LOG_AUTH      (4<<3) /*!< Facility : Authentication */
#  define  LOG_SYSLOG    (5<<3) /*!< Facility : Syslog itself */
#  define  LOG_LPR       (6<<3) /*!< Facility : Printer system */
#  define  LOG_NEWS      (7<<3) /*!< Facility : Usenet or RSS */
#  define  LOG_UUCP      (8<<3) /*!< Facility : UUCP */
#  define  LOG_CRON      (9<<3) /*!< Facility : Timer / Cron / Zeitplandienst */
#  define  LOG_AUTHPRIV  (10<<3) /*!< Facility : Authentication */
#  define  LOG_FTP       (11<<3) /*!< Facility : FTP & co */
/* Codes through 15 are reserved for system use */
#  define LOG_LOCAL0     (16<<3) /*!< Facility : some Application #0 */
#  define LOG_LOCAL1     (17<<3) /*!< Facility : some Application #1*/
#  define LOG_LOCAL2     (18<<3) /*!< Facility : some Application #2*/
#  define LOG_LOCAL3     (19<<3) /*!< Facility : some Application #3*/
#  define LOG_LOCAL4     (20<<3) /*!< Facility : some Application #4*/
#  define LOG_LOCAL5     (21<<3) /*!< Facility : some Application #5*/
#  define LOG_LOCAL6     (22<<3) /*!< Facility : some Application #6*/
#  define LOG_LOCAL7     (23<<3) /*!< Facility : some Application #7*/



#  define LOGZONE_EVERYTHING (0xFFFFFF)
#  define LOGZONE_STARTUP    (0x000001)
#  define LOGZONE_CLEANUP    (0x800000)


#  define LOGFLGS_RFC3164_TIMESTAMP 0x0001 /* if 0, no Timestamp is sent */
#  define LOGFLGS_RFC3164_SOURCEIP  0x0002 /* if 0, no IP is sent */
#  define LOGFLGS_MODULE_SOURCE     0x0004 /* if 1, module name is sent (instead of Source IP) */
#  define LOGFLGS_COUNTER64         0x0008 /* if 1, right before user Message part a {nn} counter is set in */


/*========================================================*/
/*==                                                    ==*/
/*========================================================*/
#ifdef __cplusplus
  extern "C" {
#endif

int  VSSC_open( char const * const pDestination, short Facility, short Level, char const * const AppName );
void VSSC_Log(   int Handle, unsigned Level, char const * const Module, char const * const fmt, ... );
int  VSSC_close( int Handle );

#ifdef __cplusplus
  }
#endif



/*========================================================*/
/*==                                                    ==*/
/*========================================================*/
#ifdef __cplusplus
  extern "C" {
#endif



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
static BOOL _internal_VsscRefreshAddr( PTS_vssc_data pHnd, char const * const pDestination )
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





/* takes Facility and or Level and stores it into handle struct.
 * if -1 is given, the parameter is not used. if so, return is FALSE, else TRUE.
 */
static BOOL _internal_VsscRefreshLevel( PTS_vssc_data pHnd, short Facility, short Level )
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
static BOOL _internal_VsscRefreshZone( PTS_vssc_data pHnd, unsigned ZoneMask )
{{
  pHnd->ZoneMask = ZoneMask;
  return TRUE;
}};






/* arg is optional */
int VSSC_open( char const * const pDestination, short Facility, short Level, char const * const AppName )
{{
  PTS_vssc_data pHnd;
  int iRet;
  int Blocking = 1;

  pHnd = (PTS_vssc_data) malloc( sizeof(TS_vssc_data) );
  if( !pHnd )
    {
    OutputDebugString( _T("Simple Syslog Client: outofmem\r\n") );
    return 0;
    }
  memset( pHnd, 0x00, sizeof( TS_vssc_data ) );
  pHnd->SecCookie1 = pHnd->SecCookie2 = 0xC001B001;

  pHnd->hMutex = CreateMutex( NULL, FALSE, NULL );
  if( !pHnd->hMutex ) /* Check for error. */
    {
    OutputDebugString( _T("Simple Syslog Client: MUTEX Problem\r\n") );
    free( pHnd );
    return 0;
    }

  /*
  if( ERROR_ALREADY_EXISTS == GetLastError() )
    { I am not the first user of this Mutex, but this is Okay }
  */

  iRet = WSAStartup( MAKEWORD(2,2), &pHnd->wsaData );
  pHnd->SndSocket = socket( AF_INET, SOCK_DGRAM, IPPROTO_UDP );
  if( pHnd->SndSocket == INVALID_SOCKET)
    {
    OutputDebugString( _T("Simple Syslog Client: socket() FAILED\r\n") );
    free( pHnd );
    return 0;
    }

  _internal_VsscRefreshAddr( pHnd, (pDestination) ? pDestination : "127.0.0.1:514" );
  _internal_VsscRefreshLevel(pHnd, (short)((Facility>=0) ? Facility : LOG_LOCAL7), 
                                   (short)((Level   >=0) ? Level    : LOG_INFO ));
  _internal_VsscRefreshZone( pHnd, LOGZONE_EVERYTHING );
  pHnd->MsgCount = 0;
  strncpy( pHnd->AppName, AppName?AppName:"", 32 );
  pHnd->AppName[32]=0;

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
  
  iRet = bind( pHnd->SndSocket, (struct sockaddr *)&pHnd->OwnAddr, sizeof(pHnd->OwnAddr) );
  if( iRet == SOCKET_ERROR )
    {
    OutputDebugString( _T("Simple Syslog Client: bind() FAILED\r\n") );
    free( pHnd );
    return 0;
    }

  return (int) pHnd;
}}




int VSSC_close( int Handle )
{{
  PTS_vssc_data pHnd = (PTS_vssc_data) Handle;
  if( (pHnd->SecCookie1 != 0xC001B001) || (pHnd->SecCookie2 != 0xC001B001) )
    {
    OutputDebugString( _T("Simple Syslog Handle corrupt\r\n") );
    return (int) FALSE;
    }

  closesocket( pHnd->SndSocket );
  WSACleanup();
  while( ReleaseMutex( pHnd->hMutex ) ){;/*do-a-nothing*/}
  CloseHandle( pHnd->hMutex );

  free( pHnd );
  return (int) TRUE;
}}




void VSSC_Log( int Handle, unsigned Level, char const * const Module, char const * const fmt, ... )
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
  if( pHnd->Level < Level+1 )
    {
    return;
    }

  if( (Level & 0xFF) > LOG_DEBUG ) /* SYSLOG RFC3164 Level is only lower 3 bit */
    Level = LOG_DEBUG;

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


/***************************************************************************/


char const * const mod = "minilogger";





int main(int argc, char* argv[])
{{
  int Handle;

  printf("-----------------------------\r\n");
  //Handle = VSSC_open( NULL, -1, -1, NULL );
  Handle = VSSC_open( "localhost:514", LOG_LOCAL2, LOG_DEBUG, "TestProg" );
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
  
  VSSC_close( Handle );
  printf("-----------------------------\r\n");

  
  return 0;
}}

