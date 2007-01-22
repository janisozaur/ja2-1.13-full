#ifndef _VSSC_H_
#define _VSSC_H_
/* _______________________________________________________
 * VSSC.H: Interface for a very Simple Sylog Client
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



/*========================================================*/
/*==                                                    ==*/
/*========================================================*/
int  VSSC_open( char const * pDestination, short Facility, short Level, char const * pAppName );
int  VSSC_close( int Handle );
void VSSC_Log(   int Handle, unsigned Level, char const * const Module, char const * const fmt, ... );
/*========================================================*/
/*==                                                    ==*/
/*========================================================*/



#ifdef __cplusplus
  }
#endif



#endif /* _VSSC_H_ */