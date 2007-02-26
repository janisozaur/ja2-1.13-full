// PseudoMIPS.cpp : Definiert den Einsprungpunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "cpuclock.h"


void CorrectionFactor( unsigned long Clock, unsigned long OPS );




int _tmain( int argc, TCHAR* targv[] )
{{
  unsigned long Clock, OPS;

  Clock = GetCpuClock();
  _tprintf( _T("%lu MHz CPU Clock\r\n"), Clock );
  
  OPS = GetQuantiSpeed();
  _tprintf( _T("%lu MHz QuantiSpeed (estimated)\r\n"), OPS );

  OPS = GetCoreclockSpeed();
  _tprintf( _T("%lu MHz CPU CoreSpeed (estimated)\r\n"), OPS );
  
  OPS = GetCpuOPS_Int();
  _tprintf( _T("%lu OPs Int Numbers crunched\r\n"), OPS );

  OPS = GetCpuOPS_Float();
  _tprintf( _T("%lu OPs Float Numbers crunched\r\n"), OPS );

  OPS = GetCpuOPS_Primes();
  _tprintf( _T("%lu OPs Prime Numbers crunched\r\n"), OPS );
  
  _tprintf( _T("\r\n---------------\r\n") );
  CorrectionFactor( Clock, OPS );
  CorrectionFactor( 3000,  OPS );
  CorrectionFactor( 1800,  OPS );
  CorrectionFactor( 1500,  OPS );
  _tprintf( _T("\r\n---------------\r\n") );
  
  _tprintf( _T("\r\n press Enter") );
  _gettchar();
  _tprintf( _T("\r\n\r\n") );
  return 0;
}}



void CorrectionFactor( unsigned long Clock, unsigned long OPS )
{{

  unsigned long FactorA = (Clock * 1000) / (OPS);
  unsigned long FactorB = (OPS * 1000) / (Clock);

  if( FactorA >0 )
    _tprintf( _T("%lu * %ld / 1000 would be a good estimation for %lu\r\n"), OPS, FactorA, Clock );
  else if( FactorB >0 )
    _tprintf( _T("%lu * 1000 / %ld would be a good estimation for %lu\r\n"), OPS, FactorB, Clock );
  else
    _tprintf( _T("Cannot calc Difference to Registry stored MHz\r\n") );

  return;
}}
