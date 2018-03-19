complex.h

changed short to int16\_t
changed int to int32\_t

fft.c
removed uselsess include
in twiddle factor use -32768.0 for negative, remove some error. 
positive has no way to have the exact correct number.

fixed error in bit4 reorder Q17 and Q15, previously was scaling only those that got swapped. extremely buggy behavior

filled in butterfly for Q15 and Q245

butterfly with k1 = 0 moved out of the loop because there is no point on doing math operations that might generate some errors ( example -> multiplication by positive 1 is not possibile in decimal representation, max number is 0.9999

added set_taus_feed to initialized RNG

fixed_point.c

changed data types,
multiply moved to define

rangen_double.c  slight comsetic changes

WHITE NOISE EXPLAINATION:

SOME WORDS ON SCALING, COMMENT GRAPHS

DISTORTION VS SNR
