#include "../include/filter_bank.h"
#include "stdlib.h"

/*
    private functions
*/
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

    buf->buffer[(buf->ptr+buf->sz)%3] = x;
    if (buf->sz < 3) buf->sz += 1;
    else rotate_buffer3(buf);
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
    *result = 0.0;
    for (ctr = 0; ctr < 3; ++ctr) {
        sample_t a_val = a->buffer[(a->ptr+ctr)%3];
        sample_t b_val = b->buffer[(b->ptr+ctr)%3];
        *result += a_val*b_val;
    }
EXIT:
    if (status != FB_SUCCESS) {
        result = NULL;
    }
    return status;
}

biquad_t* init_biquad(sample_t a[3], sample_t b[3])
{
    status_t status = FB_SUCCESS;
    uint8_t ctr = 0;

    biquad_t* bq = (biquad_t*) malloc(sizeof(biquad_t));
    if (bq == NULL) {
        status = FB_FAILURE;
        goto EXIT;
    }
    
    // copy buffers
    for (ctr = 0; ctr < 3; ++ctr) {
        push_buffer(a[ctr], &bq->a);
        push_buffer(b[ctr], &bq->b);
    }

    sample_t a0 = bq->a.buffer[0];
    for (ctr = 0; ctr < 3; ++ctr) {
        bq->x.buffer[ctr] = 0;
        bq->y.buffer[ctr] = 0;
        
        // normalize
        bq->a.buffer[ctr] /= a0;
        bq->b.buffer[ctr] /= a0;
    }
    bq->a.buffer[0] = 0;
    bq->a.ptr += 1;
    bq->b.ptr = 0;
    bq->x.ptr = 0;
    bq->y.ptr = 0;
EXIT:
    if (status != FB_SUCCESS) {
        close_biquad(bq);
        return NULL;
    }
    return bq;
}

status_t process_sample(sample_t x, biquad_t *bq, sample_t *y)
{
    status_t status = FB_SUCCESS;
    sample_t res0 = 0.0f, res1 = 0.0f, res = 0.0f;
    
    if (bq == NULL || y == NULL) {
        status = FB_FAILURE;
        goto EXIT;
    }
    
    if ((status = push_buffer(x, &bq->x)) != FB_SUCCESS) {
        goto EXIT;
    }

    if ((status = buffer3_prod(&bq->b, &bq->x, &res0)) != FB_SUCCESS) {
        goto EXIT;
    }
    if ((status = buffer3_prod(&bq->a, &bq->y, &res1)) != FB_SUCCESS) {
        goto EXIT;
    }
    res = _FB_CLIP(res0 - res1);

    if ((status = push_buffer(x, &bq->x)) != FB_SUCCESS) {
        goto EXIT;
    }
EXIT:
    push_buffer(res, &bq->y);
    *y = res;
    return status;
}

status_t close_biquad(biquad_t *bq)
{
    status_t status = FB_SUCCESS;
    if (bq == NULL) {
        goto EXIT;
    }

    free(bq);
    bq = NULL;
EXIT:
    return status;
}