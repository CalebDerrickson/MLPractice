#include "xor.h"
#include "defines.h"
#define COMMON_H
#include "common.h"

float td[] = {
    0, 0, 0,
    1, 0, 1,
    0, 1, 1,
    1, 1, 0,
};

int xor() 
{
    // srand(time(0));
    srand(69);

    // Splitting training data.
    // input -> first two columns
    // output -> last column

    size_t stride = 3;
    size_t n  = sizeof(td)/sizeof(td[0])/stride;
    Mat ti = {
        .rows = n,
        .cols = 2,
        .stride = stride,
        .elements = td,
    };

    Mat to = {
        .rows = n,
        .cols = 1,
        .stride = stride,
        .elements = td+2,
    };


    size_t arch[] = {2, 2, 1};
    NN nn = nn_alloc(arch, ARRAY_LEN(arch));
    NN g = nn_alloc(arch, ARRAY_LEN(arch));
    nn_rand(nn, 0, 1);


    
    rate = 1e-1;
    for (size_t i = 0; i < 1; i++) {
#if 0
    eps = 1e-1;
    nn_finite_diff(nn, g, ti, to);
#else
    nn_back_prop(nn, g, ti, to);
#endif
    NN_PRINT(g);
        //nn_learn(nn, g);
        //printf("%llu: cost = %f\n", i, nn_cost(nn, ti, to));
    }
    
    printf("cost = %f\n", nn_cost(nn, ti, to));
    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            MAT_AT(NN_INPUT(nn), 0, 0) = i;
            MAT_AT(NN_INPUT(nn), 0, 1) = j;
            nn_forward(nn);
            printf("%llu ^ %llu = %f\n", i, j, MAT_AT(NN_OUTPUT(nn), 0, 0));
        }
    }

    return 0;
}

