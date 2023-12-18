#include "gates.h"
#define COMMON_H
#include "common.h"

float costOR(float w1, float w2, float b)
{
    float result = 0.0f;
    for (size_t i = 0; i < train_count; i++){
        float x1 = trainOR[i][0];
        float x2 = trainOR[i][1];
        float y = sigmoidf(x1*w1 + x2*w2 + b);
        float d = y - trainOR[i][2];
        result += d*d;
    }
    result /= (float)train_count;

    return result;
}


int gates() {

    rate = 1e-1;
    eps = 1e-1;

    float w1 = rand_float();
    float w2 = rand_float();
    float b = rand_float();
    float c = costOR(w1, w2, b);

    for(size_t i = 0 ; i < 1e6; i++) {

        c = costOR(w1, w2, b);

        float dw1 = (costOR(w1 + eps, w2, b) - c) / eps;
        float dw2 = (costOR(w1, w2 + eps, b) - c) / eps;
        float db  = (costOR(w1, w2, b + eps) - c) / eps;

        w1 -= rate*dw1;
        w2 -= rate*dw2;
        b -= rate*db;

        
    }
    
    printf("c: %f \t w1: %f \t w2: %f \t b: %f \n", 
        c, w1, w2, b);

    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
             printf("%llu | %llu = %f\n", i, j, sigmoidf(i*w1 + j*w2 + b));
        }
    }
    return 0;
}