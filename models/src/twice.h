#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "defines.h"

float train[][2] = {
    {0, 0},
    {1, 2},
    {3, 6},
    {4, 8},
    {5, 10},
    {6, 12},
};
#define train_count (sizeof(train)/sizeof(train[0]))

MLAPI int twice();