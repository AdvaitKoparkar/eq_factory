#include<stdio.h>
#include "../include/filter_bank.h"

int main()
{
    sample_t a[3] = {1, 0, 0}, b[3] = {1, 0, 0};
    sample_t signal[10] = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 0.9};
    sample_t res = 0.0f;
    biquad_t *bq = init_biquad(a, b);
    uint8_t i = 0;
    for (i = 0; i < 10; ++i) {
        if ((process_sample(signal[i], bq, &res) == FB_SUCCESS)) {
            printf("%03d : %.02f\n", i, res);
        }
        else {
            printf("error\n");
        }
    }
    close_biquad(bq);
}