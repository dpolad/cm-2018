/*! \mainpage Useful Numerical Functions
\section scope_numerical Scope
The present document specifies several numerical functions for testing of digital communication systems.

-# Generation of Uniform Random Bits
-# Generation of Quantized Gaussian Random Variables
-# Generation of QAM-modulated random sequences (with provision for OFDM signal formats)

@defgroup numerical

@defgroup _taus 
@ingroup numerical Tausworthe Uniform Random Variable Generator.*/

unsigned int s0, s1, s2, b;

//----------------------------------------------
//
/*!\brief Tausworthe Uniform Random Generator.  This is based on the hardware implementation described in 
  Lee et al, "A Hardware Gaussian Noise Generator Usign the Box-Muller Method and its Error Analysis," IEEE Trans. on Computers, 2006.
*/
//
unsigned int taus();

void set_taus_seed();
