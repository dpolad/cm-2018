#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "rangen_double.h"


void randominit()
{
  int i;
  unsigned seedfather;

  srand((unsigned)time(NULL));

  seed = (unsigned int) rand();
  if (seed % 2 == 0) seed += 1; /* seed and mod are relative prime */
  for (i=1; i<=97; i++)
  {
      seed = a*seed;                 /* mod 2**32  */
      ir[i]= seed;                   /* initialize the shuffle table    */
  }
  iy=1;
}


double uniformrandom()
{
  #define a 1664525lu
  #define mod 4294967296.0                /* is 2**32 */

  int j;

  j=1 + 97.0*iy/mod;
  iy=ir[j];
  seed = a*seed;                          /* mod 2**32 */
  ir[j] = seed;
  return( (double) iy/mod );
}


double gaussdouble(double mean, double variance)
{
  static int iset=0;
  static double gset;
  double fac,r,v1,v2;

  if (iset == 0){
     do{
           v1 = 2.0*uniformrandom()-1.0;
           v2 = 2.0*uniformrandom()-1.0;
           r = v1*v1+v2*v2;
        }  while (r >= 1.0);
     fac = sqrt(-2.0*log(r)/r);
     gset= v1*fac;
     iset=1;
     return(sqrt(variance)*v2*fac + mean);
  } else {
     iset=0;
     return(sqrt(variance)*gset + mean);
  }
}
