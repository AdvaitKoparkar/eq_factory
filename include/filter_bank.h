#ifndef IIR_FACTORY_H
#define IIR_FACTORY_H
#include <stdint.h>

#define _FB_MIN -1.0
#define _FB_MAX 1.0
#define max(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a > _b ? _a : _b; })
#define min(a,b) ({ __typeof__ (a) _a = (a); __typeof__ (b) _b = (b); _a < _b ? _a : _b; })
#define _FB_CLIP(x) max(_FB_MIN, min(_FB_MAX, x))

typedef float sample_t;

typedef enum status_e {
    FB_SUCCESS = 0,
    FB_FAILURE = 1
} status_t;

/*
    ring buffer implementation 
*/
typedef struct buffer3_s {
    uint8_t ptr;
    uint8_t sz;
    sample_t buffer[3];    
} buffer3_t;

/*
    represents 6 coefs of biquad normalized s.t. a[0] = 1
*/
typedef struct biquad_s {
    buffer3_t a; // [a0, a1, a2]
    buffer3_t b; // [b0, b1, b2]
    buffer3_t y;
    buffer3_t x;
} biquad_t;

// init biquad
biquad_t* init_biquad(sample_t a[3], sample_t b[3]);

/* 
    process sample
*/
status_t process_sample(sample_t x, biquad_t *bq, sample_t *y);

// close biquad
status_t close_biquad(biquad_t *bq);

#endif // IIR_FACTORY_H