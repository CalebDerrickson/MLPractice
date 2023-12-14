#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// OR gate data
float trainOR[][3] = {
    {0, 0, 0},
    {0, 1, 1},
    {1, 1, 1},
    {1, 0, 1},
    {0, 0, 0},
};

#define train_count (sizeof(train)/sizeof(train[0]))

int gates();