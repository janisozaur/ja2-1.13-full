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

/*! file VSSC.h
 *  \brief prototypes and macros for the Very Simple Syslog Client
 *
 * This file exports the low layer interface for the Very Simple Syslog Client (VSSC)
 * and some predifined constats as well as some useful macros.
 * Project special Macros like SDKs RETAILMSG etc. will appaer in an extra file
 */


#include "stdafx.h" /* we assume to have all global & common project settings in this file */




#if !defined(DIM)
#  define DIM(x)        (sizeof(x)/sizeof(x[0]))
#endif

#if !defined(MIN)
# define MIN(a,b)       ((a) < (b) ? (a) : (b))
# define MAX(a,b)       ((a) > (b) ? (a) : (b))
#endif


/*---- Facilities ---------*/
/* Simply taken from BSD syslog. The values looks a litle complicated,
 * but You don't have to understand them. I left them original to prevent 
 * problems under MinGW or CygWin. 
 * Facilities are a sort of 'grouping' Applications. Under normal circumstances
 * You'll use one of the LOG_LOCALx entries.
 */
#if !defined( LOG_KERN )
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
#endif

/*---- Severyties ---------*/
/* today, we do NOT use the BSD LOG-Levels, because they use 0 as an active
 * value, while we use 0 as 'dont log' and furthermore they stop at LOG_DEBUG
 * while we prefer to have more than one Debug-Level.
 *
 * this will be no problem for campatibility, because we use the original
 * syslog values later (internal), just AFTER our own filtering.
 * f.i.: if You 
 */
#  define  SLOG_SILENT    0 /*!< SLOG_SILENT : if global set, nothing will be logged. If given as parameter, the Msg will NEVER appear */
#  define  SLOG_EMERG     1 /*!< SLOG_EMERG : same as BSDs 'LOG_EMERG'. Use only on very problematic errors, i.e. before BSOD ;-) */
#  define  SLOG_ALERT     2 /*!< SLOG_ALERT : same as BSDs 'LOG_ALERT'. All, a User or Admin must see, because more Problems following */
#  define  SLOG_CRIT      3 /*!< SLOG_CRIT : same as BSDs 'LOG_CRIT'. Critical Situations which will bring the Applic to not work or to work only limited */
#  define  SLOG_ERR       4 /*!< SLOG_ERR : same as BSDs 'LOG_ERR'. Every unexpected Error, like locked files, out of mem, etc. */
#  define  SLOG_WARNING   5 /*!< SLOG_WARNING : same as BSDs 'LOG_WARNING'. Use it for situations which could lead to problems */
#  define  SLOG_NOTICE    6 /*!< SLOG_NOTICE : same as BSDs 'LOG_NOTICE'. Use freely */
#  define  SLOG_INFO      7 /*!< SLOG_INFO : same as BSDs 'LOG_INFO'. More Informations, but not as important as Notices */
#  define  SLOG_DEBUG     8 /*!< SLOG_DEBUG : same as BSDs 'LOG_DEBUG'. Most times used for Messages needed while debugging */
#  define  SLOG_DEBUG2    9 /*!< SLOG_DEBUG2 : mapped to BSDs 'LOG_DEBUG'. Debug info that is only important in some cases */
#  define  SLOG_DEBUGMAX 10 /*!< SLOG_DEBUGMAX : also mapped to BSDs 'LOG_DEBUG'. Debug info that is only important in some very rare cases */
#  define  SLOG_ERROR   SLOG_ERR /* only a double. prevent me for misstyping <vbg> */
#  define  SLOG_WARN    SLOG_WARNING /* only a double. prevent me for misstyping <vbg> again */




#  define LOGZONE_EVERYTHING (0xFFFFFF) /*!< LOGZONE_EVERYTHING : if global set, every module will be logged. If given as parameter, the Msg will EVER appear */
#  define LOGZONE_STARTUP    (0x000001) /*!< LOGZONE_STARTUP : use this in, f.i. Init_xxx() functions */
#  define LOGZONE_CLEANUP    (0x800000) /*!< LOGZONE_CLEANUP : use this in, f.i. Exit_xxx() functions, define more if needed */


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
int  VSSC_open( void );
int  VSSC_open2( char const * pDestination, short Facility, short Level, char const * pAppName ); /* pointers can be NULL, shorts can be -1 for default */
int  VSSC_close( int Handle );
void VSSC_Log(   int Handle, unsigned Level, char const * const Module, char const * const fmt, ... );
/*========================================================*/
/*==                                                    ==*/
/*========================================================*/



#ifdef __cplusplus
  }
#endif



#endif /* _VSSC_H_ */