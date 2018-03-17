#define a 1664525lu
#define mod 4294967296.0                /* is 2**32 */

static unsigned long seed, iy, ir[98];

/*
@defgroup _uniformdouble 
@ingroup numerical Uniform linear congruential random number generator.
*/

/*!\brief Initialization routine for Uniform/Gaussian random number generators. */
void randominit();

/*!\brief Uniform linear congruential random number generator on \f$[0,1)\f$.  Returns a double-precision floating-point number.*/
double uniformrandom();

/*
@defgroup _gaussdouble Gaussian random number generator based on modified Box-Muller transformation.   
@ingroup numerical 
*/

/*!\brief Gaussian random number generator based on modified Box-Muller transformation.Returns a double-precision floating-point number. */
double gaussdouble(double mean, double variance);
