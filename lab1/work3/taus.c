#include <time.h>
#include "taus.h"

unsigned int taus()
{  
  b = (((s0 << 13) ^ s0) >> 19);
  s0 = (((s0 & 0xFFFFFFFE) << 12)^  b);
  b = (((s1 << 2) ^ s1) >> 25);
  s1 = (((s1 & 0xFFFFFFF8) << 4)^  b);
  b = (((s2 << 3) ^ s2) >> 11);
  s2 = (((s2 & 0xFFFFFFF0) << 17)^  b);
  return s0 ^ s1 ^ s2;
}

void set_taus_seed() 
{
  s0 = 0x1e23d852;
  s1 = 0x81f38a1c;
  s2 = 0xfe1a133e;
}
