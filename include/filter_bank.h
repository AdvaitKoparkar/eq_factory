#ifndef IIR_FACTORY_H
#define IIR_FACTORY_H

typedef float dtype_t;
typedef double acc_dtype_t;

typedef enum status_e {
    FB_SUCCESS = 0,
    FB_FAILURE = 1
} status_t;

typedef struct sample_s {
    dtype_t val;
    struct sample_s *next;
} sample_t;

typedef struct buffer3_s {
    sample_t *ptr;
    sample_t *buffer[3];    
} buffer3_t;

typedef struct biquad_s {
    buffer3_t a;
    buffer3_t b;
    buffer3_t y;
    buffer3_t x;
} biquad_t;


// init biquad
status_t init_biquad(buffer3_t *a, buffer3_t *b, buffer3_t *state, biquad_t *bq);

// process sample
status_t process_sample(dtype_t *x, biquad_t *bq, dtype_t *y);

// close biquad
status_t close_biquad(biquad_t *bq);

#endif // IIR_FACTORY_H