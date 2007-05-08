/* xtended atoi , supports dezimal, hex, bin, oct 
 * (c) 2005 Modem Man ()modem-man (at) gmx.net
 *
 * This File is licensed under LGPL
 */

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <windows.h>


#if !defined( DIM )
  #define DIM(x)  (sizeof(x)/sizeof(x[0])) /*!< Macro for calculating the Dimension (max nb. of elements) of an array */
#endif


#ifdef __cplusplus
extern "C" {
#endif

 
__int64 xtoll( const char *s )
{{
  __int64 ll_x=0;
  int MaxValIdx, Shift, idx;
  char Hex[]="0123456789ABCDEF";
  char Buff[80];
  char *hVal;

  strncpy( Buff, s, DIM(Buff) );
  Buff[DIM(Buff)-1] = 0;

  strupr( Buff );
  hVal = Buff;

  if( *hVal == '$' ) /* written like: $FFF (ancient Style) */
    {
    hVal++;
    MaxValIdx=15;
    Shift=4;
    }
  else if( (*hVal=='0') && ( *(hVal+1)=='X') ) /* written like: 0xFFF (c Style) */
    {
    hVal+=2;
    MaxValIdx=15;
    Shift=4;
    }
  else if( (*hVal=='&') && ( *(hVal+1)=='H') ) /* written like: &HFFF (pascal Style) */
    {
    hVal+=2;
    MaxValIdx=15;
    Shift=4;
    }
  else if( (*hVal=='0') && ( *(hVal+1)=='O') ) /* written like: 0o010 octal */
    {
    hVal+=2;
    MaxValIdx=7;
    Shift=3;
    }
  else if( (*hVal=='0') && ( *(hVal+1)=='B') ) /* written like: 0b010 binary */
    {
    hVal+=2;
    MaxValIdx=1;
    Shift=1;
    }
  else if( (*hVal=='0') && ( *(hVal+1)=='D') ) /* written like:  0d123 decimal */
    {
    return _atoi64( hVal+2 );
    }
  else
    {
    return _atoi64( hVal ); /* written like: normal ascii decimal */
    }

  /* while not end-of-string && valid char */
  while( *hVal && ((isdigit(*hVal)) || ((*hVal>='A') && (*hVal<='F'))) )
    {
	/* search the actual char inside the Hexadecimal-Array. 
	 * From last valid char to zero.
	 */
    for( idx=MaxValIdx ; idx>=0 ; idx-- )
      {
      if( *hVal == Hex[idx] ) 
	       break; /* found valid one */
      }
    if(idx<0) 
	     return ll_x; /* invalid char found. Skip remainder fro m here, last result is final result! */

    ll_x <<= Shift;
    ll_x += idx;
    hVal++;
    };

  return ll_x;
}};




long xtol( const char *s )
{{
  return (long) xtoll( s );
}};

int xtoi( const char *s )
{{
  return (int) xtoll( s );
}};




/*---- WIDE-CHAR functions following ----*/
__int64 _wxtoll( const wchar_t *s )
{{
  char Ascii[80];
  wcstombs( Ascii, s, DIM(Ascii) );
  return xtoll( Ascii );
}};

long _wxtol( const wchar_t *s )
{{
  char Ascii[80];
  wcstombs( Ascii, s, DIM(Ascii) );
  return (long) xtoll( Ascii );
}};

int _wxtoi( const wchar_t *s )
{{
  char Ascii[80];
  wcstombs( Ascii, s, DIM(Ascii) );
  return (int) xtoll( Ascii );
}};



#ifdef __cplusplus
}
#endif
