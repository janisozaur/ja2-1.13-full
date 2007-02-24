// PseudoMIPS.cpp : Definiert den Einsprungpunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "cpuclock.h"


int _tmain( int argc, TCHAR* targv[] )
{{
  unsigned long Clock;

  Clock = GetCpuClock();
  _tprintf( _T("CPU Clock may be %lu MHz\r\n"), Clock );
  

  Clock = GetCpuMOPS();
  _tprintf( _T("CPU ADD+CALL-Speed may be %lu TOPs\r\n"), Clock );

  
  Clock = GetQuantiSpeed();
  _tprintf( _T("CPU QuantiSpeed is ~ %lu MOPs\r\n"), Clock );

  
  Clock = GetCoreclockSpeed();
  _tprintf( _T("CPU ClockSpeed is ~ %lu MOPs\r\n"), Clock );
  
  _gettchar();
  return 0;
}}
