// Include files for SSE4
#include "emmintrin.h"
#include "immintrin.h"
#include "xmmintrin.h"
#include "time_meas.h"
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#ifdef OMP
#include <omp.h>
#endif
#define TRIALS 1000000

// example SIMD macros, not necessary to be used, write your own


// routines to be written
void componentwise_multiply_real_scalar(int16_t *x,int16_t *y,int16_t *z,uint16_t N) {

    int i;
    for(i=0;i<N;i++){
        z[i] = (uint16_t) ((((uint32_t) x[i]) * y[i]) >> 15);
    }


}

static inline void componentwise_multiply_real_sse4(int16_t *x,int16_t *y,int16_t *z,uint16_t N) {
    __m128i *x128 = (__m128i *)x;
    __m128i *y128 = (__m128i *)y;
    __m128i *z128 = (__m128i *)z;
    uint16_t i;
    N>>=3;
    for(i=0; i<N; i++)
    {
        z128[i] = _mm_mulhrs_epi16(x128[i], y128[i]);
    }
}

static inline void componentwise_multiply_real_avx2(int16_t *x,int16_t *y,int16_t *z,uint16_t N) {
    __m256i *x256 = (__m256i *)x;
    __m256i *y256 = (__m256i *)y;
    __m256i *z256 = (__m256i *)z;
    uint16_t i;
    N>>=4;
    for(i=0; i<N; i++)
    {
        z256[i] = _mm256_mulhrs_epi16(x256[i], y256[i]);
    }

}

int main(int argc, char *argv[]){

    int SIZE;
    int16_t *x;
    int16_t *y;
    int16_t *z;
    int i;


    if(argc != 2){
        printf("wrong # of parameters\n");
        exit(1);
    }

    SIZE = atoi(argv[1]);

    x = (int16_t *) aligned_alloc(32,sizeof(int16_t) * SIZE);
    y = (int16_t *) aligned_alloc(32,sizeof(int16_t) * SIZE);
    z = (int16_t *) aligned_alloc(32,sizeof(int16_t) * SIZE);

    srand(time(NULL));   // should only be called once
    for(i=0;i<SIZE;i++){
        x[i] = rand();
        y[i] = rand();
    }
    time_stats_t ts;

    reset_meas(&ts);

#ifdef OMP
#pragma omp parallel
#endif
{
    for(i=0;i<TRIALS;i++){
        start_meas(&ts);
        componentwise_multiply_real_scalar(x,y,z,SIZE);
        stop_meas(&ts);
    }
}


    printf("SCALAR:\n");
    printf("max:    %10lld\n",ts.max);
    printf("trials: %10d\n",ts.trials);
    printf("avg:    %10.0f\n",(double)(ts.diff/ts.trials));
    printf("\n");

    reset_meas(&ts);
#ifdef OMP
#pragma omp for
#endif
    for(i=0;i<TRIALS;i++){
        start_meas(&ts);
        componentwise_multiply_real_sse4(x,y,z,SIZE);
        stop_meas(&ts);
    }

    printf("SSE4:\n");
    printf("max:    %10lld\n",ts.max);
    printf("trials: %10d\n",ts.trials);
    printf("avg:    %10.0f\n",(double)(ts.diff/ts.trials));
    printf("\n");

    reset_meas(&ts);
#ifdef OMP
#pragma omp parallel
#endif
{
    for(i=0;i<TRIALS;i++){
        start_meas(&ts);
        componentwise_multiply_real_avx2(x,y,z,SIZE);
        stop_meas(&ts);
    }
}
    printf("AVX2:\n");
    printf("max:    %10lld\n",ts.max);
    printf("trials: %10d\n",ts.trials);
    printf("avg:    %10.0f\n",(double)(ts.diff/ts.trials));
    printf("\n");

    printf("%.20f\n",omp_get_wtick());
    printf("%f\n",omp_get_wtime());
    printf("%f\n",omp_get_wtime());

    return 0;
}
