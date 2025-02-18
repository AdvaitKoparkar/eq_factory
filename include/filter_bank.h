#ifndef IIR_FACTORY_H
#define IIR_FACTORY_H
#include <stdint.h>

typedef float sample_t;

typedef enum status_e {
    FB_SUCCESS = 0,
    FB_FAILURE = 1
} status_t;

/*
    ring buffer implementation 

    once initialized, ring buffer looks like this:
        buf[0] => buf[1] => buf[2] => buf[0]
                   ^ptr
    
    buf: circular buffer containing 3 samples
    ptr: entry point to circular buffer - points to loc of earliest sample in ring buffer
*/
typedef struct buffer3_s {
    uint8_t ptr;
    sample_t buffer[3];    
} buffer3_t;

typedef struct biquad_s {
    buffer3_t a;
    buffer3_t b;
    buffer3_t y;
    buffer3_t x;
} biquad_t;


// init biquad
status_t init_biquad(buffer3_t *a, buffer3_t *b, biquad_t *bq);

// process sample
status_t process_sample(sample_t *x, biquad_t *bq, sample_t *y);

// close biquad
status_t close_biquad(biquad_t *bq);

#endif // IIR_FACTORY_H