#include "emmintrin.h"
#include "immintrin.h"
#include "xmmintrin.h"
#include "time_meas.h"
#include "complex.h"
#include <stdint.h>
#include "sched.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <unistd.h>
#include "libpfc.h"
#define TRIALS 1000000

typedef unsigned long long ull;
typedef signed long long sll;

const static int16_t reflip_256[32]  __attribute__((aligned(32))) = {1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1,1,-1};
const static int16_t reflip_128[16]  __attribute__((aligned(32))) = {1,-1,1,-1,1,-1,1,-1};

static inline void componentwise_multiply_complex_struct_avx2(complex16 *x,complex16 *y, complex16 *z, uint32_t N)
{
    __m256i *x256 = (__m256i *)x;
    __m256i *y256 = (__m256i *)y;
    __m256i *z256 = (__m256i *)z;

    __m256i const perm_mask  = _mm256_set_epi8(29,28,31,30,25,24,27,26,21,20,23,22,17,16,19,18,13,12,15,14,9,8,11,10,5,4,7,6,1,0,3,2);

    register __m256i mmtmpb_real;
    register __m256i mmtmpb_imag;

    uint32_t i;
    N>>=4;
    for(i=0; i<N; i++)
    {

    // first negate imaginary parts of b
    mmtmpb_real    = _mm256_sign_epi16(y256[i],*(__m256i*)reflip_256);

    // multiply and add for real parts -> a*c - b*d
    mmtmpb_real    = _mm256_madd_epi16(x256[i],mmtmpb_real);

    //shift left imaginary part
    mmtmpb_real    = _mm256_srli_epi32(mmtmpb_real, 15);

    // swap b two by two
    mmtmpb_imag    = _mm256_shuffle_epi8(y256[i],perm_mask);

    // multiply and add for imaginary parts -> a*d + b*c
    mmtmpb_imag    = _mm256_madd_epi16(x256[i],mmtmpb_imag);

    //shift left imaginary part
    mmtmpb_imag    =_mm256_slli_epi32 (mmtmpb_imag, 1);

    // merge
    z256[i] = _mm256_blend_epi16(mmtmpb_imag,mmtmpb_real,0xAA);
    }

}

static inline void componentwise_multiply_complex_array_avx2(int16_t *a,int16_t *b,int16_t *c,int16_t *d,int16_t *x,int16_t *y,uint16_t N)
{

    __m256i *x256 = (__m256i *)x;
    __m256i *y256 = (__m256i *)y;
    __m256i *a256 = (__m256i *)a;
    __m256i *b256 = (__m256i *)b;
    __m256i *c256 = (__m256i *)c;
    __m256i *d256 = (__m256i *)d;
    __m256i ac256;
    __m256i bc256;
    __m256i bd256;
    __m256i ad256;


    uint16_t i;
    N>>=4;
    for(i=0; i<N; i++)
    {


        ac256 = _mm256_mulhrs_epi16(a256[i], c256[i]);
        bc256 = _mm256_mulhrs_epi16(b256[i], c256[i]);
        bd256 = _mm256_mulhrs_epi16(b256[i], d256[i]);
        ad256 = _mm256_mulhrs_epi16(a256[i], d256[i]);

        x256[i] = _mm256_sub_epi16(ac256,bd256);
        y256[i] = _mm256_add_epi16(bc256,ad256);

    }

}

static inline void componentwise_multiply_complex_array_sse4(int16_t *a,int16_t *b,int16_t *c,int16_t *d,int16_t *x,int16_t *y,uint16_t N)
{

    __m128i *x128 = (__m128i *)x;
    __m128i *y128 = (__m128i *)y;
    __m128i *a128 = (__m128i *)a;
    __m128i *b128 = (__m128i *)b;
    __m128i *c128 = (__m128i *)c;
    __m128i *d128 = (__m128i *)d;
    __m128i ac128;
    __m128i bc128;
    __m128i bd128;
    __m128i ad128;


    uint16_t i;
    N>>=3;
    for(i=0; i<N; i++)
    {


        ac128 = _mm_mulhrs_epi16(a128[i], c128[i]);
        bc128 = _mm_mulhrs_epi16(b128[i], c128[i]);
        bd128 = _mm_mulhrs_epi16(b128[i], d128[i]);
        ad128 = _mm_mulhrs_epi16(a128[i], d128[i]);

        x128[i] = _mm_sub_epi16(ac128,bd128);
        y128[i] = _mm_add_epi16(bc128,ad128);

    }

}

static inline void componentwise_multiply_complex_struct_sse4(complex16 *x,complex16 *y, complex16 *z, int N)
{

    __m128i *x128 = (__m128i *)x;
    __m128i *y128 = (__m128i *)y;
    __m128i *z128 = (__m128i *)z;

    __m128i const perm_mask  = _mm_set_epi8(13,12,15,14,9,8,11,10,5,4,7,6,1,0,3,2);

    register __m128i mmtmpb_real;
    register __m128i mmtmpb_imag;

    uint16_t i;
    N>>=3;
    for(i=0; i<N; i++)
    {

    // first negate imaginary parts of b
    mmtmpb_real    = _mm_sign_epi16(y128[i],*(__m128i*)reflip_128);
    // multiply and add for real parts -> a*c - b*d
    mmtmpb_real    = _mm_madd_epi16(x128[i],mmtmpb_real);

    //shift left imaginary part
    mmtmpb_real    = _mm_srli_epi32(mmtmpb_real, 15);

    // swap b two by two
    mmtmpb_imag    = _mm_shuffle_epi8(y128[i],perm_mask);

    // multiply and add for imaginary parts -> a*d + b*c
    mmtmpb_imag    = _mm_madd_epi16(x128[i],mmtmpb_imag);

    //shift left imaginary part
    mmtmpb_imag    =_mm_slli_epi32 (mmtmpb_imag, 1);

    // merge
    z128[i] = _mm_blend_epi16(mmtmpb_imag,mmtmpb_real,0xAA);
    }

}


void componentwise_multiply_complex_struct_scalar( complex16 * restrict x,complex16 * restrict y,complex16 * restrict z, int N) {
    int i;
    for(i=0;i<N;i++){
        z[i].r = (uint16_t) ((((int32_t) x[i].r) * ((int32_t)y[i].r) - ((int32_t) x[i].i) * ((int32_t) y[i].i)) >> 15);
        z[i].i = (uint16_t) ((((int32_t) x[i].r) * ((int32_t)y[i].i) + ((int32_t) x[i].i) * ((int32_t) y[i].r)) >> 15);
    }
}

void componentwise_multiply_complex_array_scalar( int16_t * restrict a,int16_t * restrict b,int16_t * restrict c,int16_t * restrict d,int16_t * restrict x,int16_t * restrict y, int  N) {
    int i;
    for(i=0;i<N;i++){
        x[i] = (uint16_t) ((((int32_t) a[i]) * ((int32_t)c[i]) - ((int32_t) b[i]) * ((int32_t) d[i])) >> 15);
        y[i] = (uint16_t) ((((int32_t) a[i]) * ((int32_t)d[i]) + ((int32_t) b[i]) * ((int32_t) c[i])) >> 15);
    }
}


void componentwise_multiply_real_scalar(int16_t * restrict x, int16_t * restrict y, int16_t * restrict z, int N) {

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

static inline void componentwise_multiply_real_avx2(int16_t *x,int16_t *y,int16_t *z,uint32_t N) {

    __m256i *x256 = (__m256i *)x;
    __m256i *y256 = (__m256i *)y;
    __m256i *z256 = (__m256i *)z;

    uint32_t i;

    N>>=4;
    for(i=0; i<N; i++)
    {
        z256[i] = _mm256_mulhrs_epi16(x256[i], y256[i]);
    }

}

int main(int argc, char *argv[]){

    int32_t SIZE;
    int16_t *x __attribute__((aligned(32)));
    int16_t *y __attribute__((aligned(32)));
    int16_t *z __attribute__((aligned(32)));
    int16_t *a __attribute__((aligned(32)));
    int16_t *b __attribute__((aligned(32)));
    int16_t *c __attribute__((aligned(32)));
    int16_t *d __attribute__((aligned(32)));
    complex16 *xc __attribute__((aligned(32)));
    complex16 *yc __attribute__((aligned(32)));
    complex16 *zc __attribute__((aligned(32)));
    int i;
    int enable_print;
    int mode;
    int enable_complex;
    FILE *fp;


    if(argc < 5){
        printf("wrong # of parameters: SIZE enable_print mode enable_complex\n");
        exit(1);
    }

    struct sched_param param;
    param.sched_priority = 99;
    if (sched_setscheduler(0, SCHED_FIFO, & param) != 0) {
        perror("sched_setscheduler");
        exit(EXIT_FAILURE);  
    }



    SIZE = atoi(argv[1]);
    enable_print = atoi(argv[2]);
    mode = atoi(argv[3]);
    enable_complex = atoi(argv[4]);
    if(enable_complex > 2) exit(2);

    srand(time(NULL));   // should only be called once

    x = (int16_t *) aligned_alloc(32,sizeof(int16_t) * SIZE);
    y = (int16_t *) aligned_alloc(32,sizeof(int16_t) * SIZE);
    z = (int16_t *) aligned_alloc(32,sizeof(int16_t) * SIZE);
    for(i=0;i<SIZE;i++){
        x[i] = rand();
        y[i] = rand();
    }

    mode+=3*enable_complex;

    xc = (complex16 *) aligned_alloc(32,sizeof(complex16) * SIZE);
    yc = (complex16 *) aligned_alloc(32,sizeof(complex16) * SIZE);
    zc = (complex16 *) aligned_alloc(32,sizeof(complex16) * SIZE);
    for(i=0;i<SIZE;i++){
        xc[i].r = rand();
        xc[i].i = rand();
        yc[i].r = rand();
        yc[i].i = rand();
    }

    a = (int16_t *) aligned_alloc(32,sizeof(int16_t) * SIZE/2);
    b = (int16_t *) aligned_alloc(32,sizeof(int16_t) * SIZE/2);
    c = (int16_t *) aligned_alloc(32,sizeof(int16_t) * SIZE/2);
    d = (int16_t *) aligned_alloc(32,sizeof(int16_t) * SIZE/2);
    for(i=0;i<SIZE/2;i++){
        a[i] = rand();
        b[i] = rand();
        c[i] = rand();
        d[i] = rand();
    }


    fp = fopen("results.txt","w");



/*
    time_stats_t ts;
    reset_meas(&ts);
    start_meas(&ts);
*/

    pfcPinThread(3);
    if(pfcInit() != 0){
        printf("Could not open /sys/module/pfc/* handles; Is module loaded?\n");
        exit(1);
    }

    static const PFC_CNT ZERO_CNT[7]  = {0,0,0,0,0,0,0};
                 PFC_CNT CNT[7]       = {0,0,0,0,0,0,0};
                 PFC_CFG CFG[7]       = {2,2,2,0,0,0,0};

    /* Reconfigure PMCs and clear their counts. */
    pfcWrCfgs(0, 7,      CFG);
    pfcWrCnts(0, 7, ZERO_CNT);

    memset(CNT, 0, sizeof(CNT));

    /************** Hot section **************/
    PFCSTART(CNT);


    switch(mode){
        case 0:
            for(i=0;i<TRIALS;i++)
                componentwise_multiply_real_scalar(x,y,z,SIZE);
            
            break;

        case 1:
            for(i=0;i<TRIALS;i++)
                componentwise_multiply_real_sse4(x,y,z,SIZE);
            break;

        case 2:
            for(i=0;i<TRIALS;i++)
                componentwise_multiply_real_avx2(x,y,z,SIZE);
            break;

        case 3:
            for(i=0;i<TRIALS;i++)
                componentwise_multiply_complex_struct_scalar(xc,yc,zc,SIZE);
            break;

        case 4:
            for(i=0;i<TRIALS;i++)
                componentwise_multiply_complex_struct_sse4(xc,yc,zc,SIZE);
            break;

        case 5:
            for(i=0;i<TRIALS;i++)
                componentwise_multiply_complex_struct_avx2(xc,yc,zc,SIZE);
            break;

        case 6:
            for(i=0;i<TRIALS;i++)
                componentwise_multiply_complex_array_scalar(a,b,c,d,x,y,SIZE/2);
            break;

        case 7:
            for(i=0;i<TRIALS;i++)
                componentwise_multiply_complex_array_sse4(a,b,c,d,x,y,SIZE/2);
            break;

        case 8:
            for(i=0;i<TRIALS;i++)
                componentwise_multiply_complex_array_avx2(a,b,c,d,x,y,SIZE/2);
            break;
    }
    PFCEND (CNT);
    /*
    stop_meas(&ts);
*/
    if(enable_print)
        for(i=0;i<SIZE;i++){
            fprintf(fp,"%d",*z+i);
        }
    pfcRemoveBias(CNT, 1);

    printf("%6d %10lld %10lld %10lld\n", SIZE, (sll)CNT[0]/TRIALS, (sll)CNT[1]/TRIALS, (sll)CNT[2]/TRIALS);

    pfcFini();
    
    fclose(fp);
    
    return 0;
}
