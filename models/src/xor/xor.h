#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "defines.h"

typedef float sample[3];

// OR gate data
sample or_train[] = {
    {0, 0, 0},
    {0, 1, 1},
    {1, 0, 1},
    {1, 1, 1},
};

// AND gate data
sample and_train[] = {
    {0, 0, 0},
    {0, 1, 0},
    {1, 0, 0},
    {1, 1, 1},
};

// NAND gate data
sample nand_train[] = {
    {0, 0, 1},
    {0, 1, 1},
    {1, 0, 1},
    {1, 1, 0},
};

// XOR gate data
sample xor_train[] = {
    {0, 0, 0},
    {0, 1, 1},
    {1, 0, 1},
    {1, 1, 0},
};



sample *train = xor_train;
#define train_count 4

// typedef struct Xor{
// 
//     float or_w1;
//     float or_w2;
//     float or_b;
// 
//     float and_w1;
//     float and_w2;
//     float and_b;
// 
//     float nand_w1;
//     float nand_w2;
//     float nand_b;
// 
// } Xor;



MLAPI int xor();