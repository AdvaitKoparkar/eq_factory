#include "../include/filter_bank.h"
#include "stdlib.h"

/*
    private functions
*/
static void free_buffer(buffer3_t *buf)
{
    if (!buf) {
        return;
    }
    
    free(buf);
    buf = NULL;
}

static status_t rotate_buffer3(buffer3_t *buf)
{
    status_t status = FB_SUCCESS;
    if (buf == NULL) {
        status = FB_FAILURE;
        goto EXIT;
    }
    buf->ptr = (buf->ptr+1)%3;
EXIT:
    return status;
}

static status_t push_buffer(sample_t x, buffer3_t *buf)
{
    status_t status = FB_SUCCESS;
    if (!buf || !x) {
        status = FB_FAILURE;
        goto EXIT;
    }

    buf->buffer[buf->ptr] = x;
    status = rotate_buffer3(buf);
EXIT:
    return status;
}

static status_t buffer3_prod(buffer3_t *a, buffer3_t *b, sample_t *result)
{
    status_t status = FB_SUCCESS;
    if (a == NULL || b == NULL || result == NULL) {
        status = FB_FAILURE;
        goto EXIT;
    }
    
    int ctr = 0;
    uint8_t a_ptr = (a->ptr+2)%3;
    uint8_t b_ptr = (b->ptr+2)%3;
    *result = 0.0;
    for (ctr = 0; ctr < 3; ++ctr) {
        sample_t a_val = a->buffer[a_ptr++];
        sample_t b_val = b->buffer[b_ptr++];
        *result += (a_val*b_val);

        a_ptr %= 3;
        b_ptr %= 3;
    }

EXIT:
    if (status != FB_SUCCESS) {
        result = NULL;
    }
    return status;
}

static void free_biquad(biquad_t *bq)
{
    if (!bq) return;
    free_buffer(&bq->a);
    free_buffer(&bq->b);
    free_buffer(&bq->x);
    free_buffer(&bq->y);
    bq = NULL;
}


status_t init_biquad(buffer3_t *a, buffer3_t *b, biquad_t *bq)
{
    status_t status = FB_SUCCESS;
    if (!a || !b) {
        status = FB_FAILURE;
        goto EXIT;
    }

    bq = malloc(sizeof(biquad_t));

    // init buffers
    bq->a = *a;
    bq->b = *b;
EXIT:
    if (status != FB_SUCCESS) {
        free_biquad(bq);
    }
    return status;
}