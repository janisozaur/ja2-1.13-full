/* xtended atoi , supports dezimal, hex, bin, oct 
 * (c) 2005 Modem Man ( modem-man (at) gmx.net )
 *
 * This File is licensed under LGPL
 */



#ifdef __cplusplus
extern "C" {
#endif


/*---- ASCII-CHAR functions following ----*/
__int64 xtoll( const char *s );
long    xtol(  const char *s );
int     xtoi(  const char *s );

/*---- WIDE-CHAR functions following ----*/
__int64 _wxtoll( const wchar_t *s );
long    _wxtol(  const wchar_t *s );
int     _wxtoi(  const wchar_t *s );


/*---- TCHAR maxros following ----*/
#if defined( _UNICODE )
#  define _txtoll(str) _wxtoll((str)) 
#  define _txtol(str)  _wxtol((str)) 
#  define _txtoi(str)  _wxtoi((str)) 
#else
#  define _txtoll(str) xtoll((str)) 
#  define _txtol(str)  xtol((str)) 
#  define _txtoi(str)  xtoi((str)) 
#endif


#ifdef __cplusplus
}
#endif
