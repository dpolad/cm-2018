//Q15 multiply
// x and y are 16-bit integers (Q15)
#define FIX_MPY(x,y)  ((int16_t)(((int32_t)x * (int32_t)y)>>15))

//25-bit by 18-bit multiply, with output scaled to 25-bits, Q24xQ17
// x is 25-bits stored as 32, y is 18-bits stored as 32
#define FIX_MPY25by18(x,y) ((int32_t)(((int64_t)x * (int64_t)y)>>17))

//Saturated addition for Q15
short SAT_ADD16(int16_t x,int16_t y);

//Saturated addition for Q24
int SAT_ADD25(int32_t x,int32_t y);
