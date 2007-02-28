// 30frames.cpp : Definiert den Einsprungpunkt für die Konsolenanwendung.
//
#include "stdafx.h"
#include <math.h>


float ran1(long *idum);
float gasdev(long *idum);
long RandomLinear(long min, long max, long *idum );
long RandomGauss( long min, long max, long *idum, long slew );


#define TRACE_DETAILS 0
#define TRACE_FRAMERATE 1
#define LOOP_GRANULITY 10 /* 10 Seconds per Measuremet Run */


typedef struct dynSleeper_
  {
  int NextIdx;
  DWORD Ticks[32];
  } TS_dynSleeper, *PTS_dynSleeper;




bool do_SleepGentle( PTS_dynSleeper pDynS, int MinFPS );
bool do_SomethingRandom( void );


TS_dynSleeper dynS = {0};



int main(int argc, char* argv[])
{{
  time_t t;
  DWORD Tick_Start, Tick_Stop;
  ULONG Frames, FPS;

  t = time(NULL);
  srand( (unsigned) t );

  bool bRunning = true;
  int Loops = 0;

  /* --------------------------------------------- *
   *  MAIN LOOP Simulation
   * --------------------------------------------- */
  printf( "\r\nStart Counting %us Slices\r\n", LOOP_GRANULITY );
  do{
    Tick_Start = GetTickCount();
    Tick_Stop = Tick_Start + (LOOP_GRANULITY * 1000);

    Frames = 0L;
    /* run the inner loop 10s, then give Control to outer Loop */
    while( GetTickCount() < Tick_Stop )
      {
      bRunning = do_SomethingRandom();
      Frames++;
      do_SleepGentle( &dynS, 32 );
      }

    /* after runed inner loop 10s, report Amount of Frames */
    FPS = (Frames * 1000) /(Tick_Stop-Tick_Start);

#   if defined (TRACE_FRAMERATE) && (TRACE_FRAMERATE)
    printf( "%4lu fps counted.\r\n", FPS );
#   endif

    }while( bRunning && ++Loops < 60 );
  
  return 0;
}}




/* burns down time between ~0 and 50 ms */
bool do_SomethingRandom( void )
{{
  DWORD Tick_Stop;
  int x = 1;
  int rnd = 0;
  static long RndSeed=0;
  
  rnd = RandomGauss(  1, 30, &RndSeed, 5 );
  //rnd = RandomLinear( 1, 100, &RndSeed );

# if defined (TRACE_DETAILS) && (TRACE_DETAILS)
  printf( "burn %4lu ms, ", rnd );
# endif

  Tick_Stop = GetTickCount() + rnd;
  while( GetTickCount() < Tick_Stop )
    {
    x = rand() % (10+x); /* this rand is only for CPU saturation */
    };
  
  return true;
}}



bool do_SleepGentle( PTS_dynSleeper pDynS, int MinFPS )
{{
  int prevIdx;
  DWORD Time_Now, Time_Oldest;
  long TpF_Done, TpF_Required; /* Time per Frame (milliseconds per frame) */
  long SloopyTime;

  /* first get the oldest tick */
  prevIdx = pDynS->NextIdx+1;
  if( prevIdx>=DIM(pDynS->Ticks) )
    prevIdx = 0;
  Time_Oldest = pDynS->Ticks[ prevIdx ];

  /* save the curent tick */
  Time_Now = GetTickCount();
  pDynS->Ticks[ pDynS->NextIdx ] = Time_Now;

# if defined (TRACE_DETAILS) && (TRACE_DETAILS)
  printf(" (%3lu) ", Time_Now - Time_Oldest );
# endif
  
  pDynS->NextIdx++;

  /*need to wrap? */
  if( (pDynS->NextIdx % DIM(pDynS->Ticks))==0 )
    pDynS->NextIdx=0;

  /* first 32 runs after Start the Array is not filled! 
   * in this case, asume it is now-10s to act as we have 3.2 fps
   */
  if( Time_Oldest==0L )
    Time_Oldest = Time_Now-10000;

  /* To enshure 32 fps, we calculate what time the last 32 frames last
   * and Sleep the remainig Time to 1 second.
   * F.i. if 32 frames last 800ms, we can Sleep 200ms, b'cause the
   * 32 frames are already drawn.
   * ----------------------------------------------------------------
   * This sounds a little bit brute if the machine runs 32 frames in, 
   * say, 360ms: we would wait then 640ms and get a stuttering graphic?
   * No way! we do a slicing by 32, so we wait 20ms, then Paint for 
   * ~10ms, then again wait 20ms and so on...
   * ----------------------------------------------------------------
   * a FPS of 32 is a very simple example, because our Array is also 
   * dimensioned to 32. Beware of this, it will also run with 20fps or 
   * 80fps (I hope)
   */
  TpF_Required = 1000 / MinFPS; /* we have xxx ms per frame. If we need more, we drop the FPS rate */
  TpF_Done = (Time_Now - Time_Oldest) / DIM(pDynS->Ticks); /* we used yyy ms per frame. Is this below TpF_Required, we have time left! */

  //FPS_Done = (DIM(pDynS->Ticks)*1000) / (Time_Now - Time_Oldest);
# if defined (TRACE_DETAILS) && (TRACE_DETAILS)
  printf(" --TpF=%3lu-- ", TpF_Done );
# endif
  
  SloopyTime = TpF_Required - TpF_Done;

  if( SloopyTime<0 ) 
    SloopyTime=0;

# if defined (TRACE_DETAILS) && (TRACE_DETAILS)
  printf("Sloopy(%3lu)\r\n", SloopyTime );
# endif
  
  Sleep( SloopyTime );
  return true;
}}








/* gaussian normal distributed random numbers 
 * the numbers are in Range of -very_large ... + very_large
 */
float gasdev(long *idum)
  {
  static int iset=0;
  static double gset;
  double fac,rsq,v1,v2;

  if( 0==iset )
    {
    do{
      v1 = 2.0 * ran1(idum)-1.0;
      v2 = 2.0 * ran1(idum)-1.0;
      rsq= v1*v1 + v2*v2;
      } while (rsq >= 1.0 || rsq == 0.0);

    fac=sqrt(-2.0*log(rsq)/rsq);
    gset=v1*fac;
    iset=1;
    return (float)(v2*fac);
    } 
  else
    {
    iset=0;
    return (float)gset;
    }
  }

/* Ranged Gauss distributed Random Numbers */
long RandomGauss( long min, long max, long *idum, long slew )
{{
  float fGauss;
  fGauss = gasdev( idum ) * slew;
  fGauss += min + (min+max)/2;
  if(fGauss < (float)min)
     fGauss = (float)min;
  if(fGauss > (float)max)
     fGauss = (float)max;
  return (long) fGauss;
}}

long RandomLinear(long min, long max, long *idum )
{{
  return (long) (ran1(idum) * ((max)-(min)) + min);
}};


#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

float ran1(long *idum)
 {
 int j;
 long k;
 static long iy=0;
 static long iv[NTAB];
 float temp;

 if( *idum <= 0 || !iy )
   {
   if( -(*idum) < 1)
     *idum=1;
   else
     *idum = -(*idum);

   for (j=NTAB+7;j>=0;j--)
     {
     k=(*idum)/IQ;
     *idum=IA*(*idum-k*IQ)-IR*k;
     if( *idum < 0 )
       *idum += IM;
     if(j < NTAB )
       iv[j] = *idum;
     }
   iy=iv[0];
   }
 k=(*idum)/IQ;
 *idum=IA*(*idum-k*IQ)-IR*k;
 if( *idum < 0 ) 
   *idum += IM;
 j=iy/NDIV;
 iy=iv[j];
 iv[j] = *idum;
 temp=(float)AM*iy;
 if (temp > RNMX) 
   return (float)RNMX;
 else 
   return temp;
 }
#undef IA
#undef IM
#undef AM
#undef IQ
#undef IR
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX
