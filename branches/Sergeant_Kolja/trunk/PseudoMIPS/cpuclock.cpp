#include <windows.h>
#include <tchar.h>

/* counts the amount of ADD64 plus a call to GetTickCount() 
 * to estimate CPU speed.
 * than normalize to get aprx same speed as CPU clock
 */
unsigned long GetCpuMOPS( void )
{{
  unsigned __int64 Zahl=0, Result64;
  DWORD StartSeconds, EndSeconds;
  unsigned long Result;

  StartSeconds = EndSeconds = GetTickCount();
  StartSeconds += 100;
  EndSeconds   += 1100;

  /* start at e fresh second to eliminate jittering */
  while( GetTickCount() < StartSeconds ) {/*noop*/};
  do{
    Zahl++;
    } while( GetTickCount() < EndSeconds );

  Result64 = Zahl / (__int64)1000;
  Result = (unsigned long) Result64;

  return Result;
}};





unsigned long GetQuantiSpeed( void )
{{
  unsigned long MOPS;
  const unsigned long QuantiSpeed = 15L; /* estimated on Athlon 3000+ */

  MOPS = GetCpuMOPS();

  return (MOPS / QuantiSpeed);
}};



unsigned long GetCoreclockSpeed( void )
{{
  unsigned long MOPS;
  const unsigned long Normalize = 21L; /* estimated on Athlon 3000+ */

  MOPS = GetCpuMOPS();

  return (MOPS / Normalize);
}};






/* simply reads the CPU clock Frequency of CORE 0 from the 
 * Registry
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




