#include <windows.h>
#include <tchar.h>
#include <math.h>

/* ----------------------------------------------------------
 * GetCpuOPS_Primes()
 *
 * Get CPU Thousand Operations per Second
 *
 * Counts something like prime numbers to _estimate_ the CPU 
 * speed.
 * This depends mostly on CPU Level1 Cache and implementation
 * of GetTickCount for this CPU. As well as of the CPU type.
 * The time when each x86 core needs the same number of clocks 
 * for a given ASM code, are far away since NEC V20 clone ;-)
 * ----------------------------------------------------------
 * returns: Loops. Somewhat Compareable with doubled MHz
 * ----------------------------------------------------------
 * If anybody has a better, C-only implementation or Source
 * of MIPS, BogoMIPS, Whetstone, Drystone please mail to
 * Sergeant_Kolja (at) yahoo de
 * ----------------------------------------------------------
 */
unsigned long GetCpuOPS_Primes( void )
{{
  /* count some prime Numbers */
  DWORD MilliSec_Start, MilliSec_Ende;
  unsigned long i = 0L, j = 0L, Zahl = 0L;

  MilliSec_Start = MilliSec_Ende = GetTickCount();
  MilliSec_Start += 100; /* fresh 100ms Slice */
  MilliSec_Ende  = MilliSec_Start + 1000;

  /* start at a fresh 100ms Slice to eliminate jittering */
  while( GetTickCount() < MilliSec_Start )
    {
    /*noop*/
    };

  /* now run a complete 1000ms Slice */
  while( GetTickCount() < MilliSec_Ende )
    {
		i++;		
		for (j = 2; j < i/2; j++)
      {
			if (i % j == 0)	
        break;
      }
    /* if it is not a multiple, increase */
		if (i % j != 0) Zahl++;
    };

  return Zahl;
}};




unsigned long GetCpuOPS_Int( void )
{{
  const unsigned long N =  10000LU;

  DWORD MilliSec_Start, MilliSec_Ende;
  unsigned long Zahl = 0L;

  register long int i;
  register long int j;

  /* start at a fresh ms Slice to eliminate jittering */
  MilliSec_Start = GetTickCount()+10;
  while( GetTickCount() < MilliSec_Start )
    {
    /*noop*/
    };

  MilliSec_Start = GetTickCount();
  for(i = 0; i < N; i++)
    {
    for(j = 0; j < N; j++)
      {
      if( (i*j) % 17 == 0 ) j++;
      }
    }
  MilliSec_Ende = GetTickCount();
  Zahl = (MilliSec_Ende - MilliSec_Start);
  if (Zahl==0) Zahl=1;
  Zahl = 6553500LU / Zahl;

  return Zahl;
}};




unsigned long GetCpuOPS_Float( void )
{{
  const unsigned long M = 5000000LU;

  DWORD MilliSec_Start, MilliSec_Ende;
  unsigned long Zahl = 0L;

  register long int i;
  register long int k = 1;
  double res = 0;

  /* start at a fresh ms Slice to eliminate jittering */
  MilliSec_Start = GetTickCount()+10;
  while( GetTickCount() < MilliSec_Start )
    {
    /*noop*/
    };

  for(i = 1; i < M; i++)
    {
    k++;
    res = res + pow(-1, k) * pow(i, -1);
    i++;
    }
  MilliSec_Ende = GetTickCount();
  Zahl = (MilliSec_Ende - MilliSec_Start);
  if (Zahl==0) Zahl=1;
  Zahl = 1638400LU / Zahl;

  return Zahl;
}};






/* ----------------------------------------------------------
 * GetQuantiSpeed()
 *
 * Get CPU AMD QuantiSpeed estimation.
 * 
 * This Foo is also very unaccarate. I simply measures the 
 * Prime Numbers OPS above, the scaling it to fit _my_ 
 * Athlons / Celerons.
 *
 * If anybody has a better, C-only implementation or Source
 * of MIPS, BogoMIPS, Whetstone, Drystone please mail to
 * Sergeant_Kolja (at) yahoo de
 * ----------------------------------------------------------
 * returns: Somewhat compareable with AMD MHz stamp.
 * ----------------------------------------------------------
 */
unsigned long GetQuantiSpeed( void )
{{
  unsigned long TOPS, QS;
  TOPS = GetCpuOPS_Primes();
  QS = (TOPS * 685) / 1000;  /* calibrated on different Athlons 1800...3000*/

  return QS;
}};


/* ----------------------------------------------------------
 * GetCoreclockSpeed()
 *
 * Get CPU Intel Single Core Clock estimation
 * 
 * This Foo is also very unaccarate. I simply measures the 
 * Prime Numbers OPS above, the scaling it to fit _my_ 
 * Athlons / Celerons.
 *
 * If anybody has a better, C-only implementation or Source
 * of MIPS, BogoMIPS, Whetstone, Drystone please mail to
 * Sergeant_Kolja (at) yahoo de
 *
 * ----------------------------------------------------------
 * returns: Somewhat compareable with Intel BIOS clock setting.
 * ----------------------------------------------------------
 */
unsigned long GetCoreclockSpeed( void )
{{
  unsigned long TOPS, CS;
  TOPS = GetCpuOPS_Primes();
  CS = (TOPS * 480) / 1000;  /* calibrated on different Athlons 1800...3000*/

  return CS;
}};






/* ----------------------------------------------------------
 * GetCpuClock()
 *
 * simply reads the CPU clock frequency of CORE 0 from the 
 * Windows NT Registry.
 *
 * I dunno if this also works on NT4 and Win9X. Please report
 * to Sergeant_Kolja (at) yahoo de if You know more...
 *
 * ----------------------------------------------------------
 * returns: on XP nearly exact the CPU Core Clock.
 * ----------------------------------------------------------
 */
unsigned long GetCpuClock( void )
{{
  HKEY hKey;
  long lRet;
  DWORD   dwMHz;
  DWORD dwBufSize = sizeof(dwMHz);
  DWORD dwValType = REG_DWORD;

  /* open the key whith CPU clock value(s): 
   * in fact, there can be more than 1 cores/cpus,
   * but since JA2 only runs in one core, we only
   * read the speed of the first one.
   * ---------------------------------------------
   * not very exact at al, but it is only used
   * for ESTIMATION of the speed...
   */
  lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                       _T("HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0"),
                       0,
                       KEY_READ,
                       &hKey);
    
  if( ERROR_SUCCESS == lRet )
    {
    /* now query the Value of MHz */
    lRet = RegQueryValueEx( hKey, _T("~MHz"), NULL, &dwValType, (LPBYTE) &dwMHz, &dwBufSize);
    RegCloseKey( hKey );

    if( (ERROR_SUCCESS == lRet) && (REG_DWORD==dwValType) )
      {
      return (unsigned long) dwMHz;
      }
    }

   /* if not able to read the speed, we guess/estimate it */
  return GetCoreclockSpeed();
}};




