#include "twice.h"
#define COMMON_H
#include "common.h"

MLAPI int twice();
static float cost(float w, float b);


static float cost(float w, float b)
{
    float result = 0.0f;
    for (size_t i = 0; i < train_count; i++){
        float x = train[i][0];
        float y = x*w + b;
        float d = y - train[i][1];
        result += d*d;
    }
    result /= train_count;

    return result;
}


int twice() {

    float w = rand_float()*10.0f;
    float b = rand_float()*50.0f;


    for(size_t i = 0; i < 1e6; i++){
        float cost_step = cost(w, b);
        //printf("Step: %zu \t Cost: %f\n", i, cost_step);
        float db = (cost(w, b + eps) - cost_step)/eps;
        float dw = (cost(w + eps, b) - cost_step)/eps;
        w -= rate*(dw);
        b -= rate*(db);
    }
    printf("----------------------\n");
    printf("w: %f \t w_pred: %f\n", 2.0f, w);
    printf("b: %f \t b_pred: %f\n", 0.0f, b);
    printf("Final cost: %f", cost(w, b));
    return 0;
}