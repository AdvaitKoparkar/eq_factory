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
    
    free(buf->ptr);
    buf->ptr = NULL;

    int ctr = 0;
    for (ctr = 0; ctr < 3; ++ctr) {
        free(&buf->buffer[ctr]);
        buf->buffer[ctr] = NULL;
    }
}

static status_t rotate_buffer3(buffer3_t *buf)
{
    status_t status = FB_SUCCESS;
    if (buf == NULL) {
        status = FB_FAILURE;
        goto EXIT;
    }
    buf->ptr = buf->ptr->next;
EXIT:
    if (status != FB_SUCCESS) {
        free(buf);
        buf = NULL;
    }
    return status;
}

static status_t buffer3_prod(buffer3_t *a, buffer3_t *b, acc_dtype_t *result)
{
    status_t status = FB_SUCCESS;
    if (a == NULL || b == NULL || result == NULL) {
        status = FB_FAILURE;
        goto EXIT;
    }
    
    int ctr = 0;
    sample_t *a_ptr = a->ptr;
    sample_t *b_ptr = b->ptr;
    for (ctr = 0; ctr < 3; ++ctr) {
        if (a_ptr == NULL || b_ptr == NULL) {
            status = FB_FAILURE;
            goto EXIT;
        }

        *result += a_ptr->val * b_ptr->val;        
    }

EXIT:
    if (status != FB_SUCCESS) {
        free(a);
        free(b);
        a = NULL;
        b = NULL;
        result = NULL;
    }
    return status;
}
