#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "defines.h"

// OR gate data
float trainOR[][3] = {
    {0, 0, 0},
    {0, 1, 1},
    {1, 1, 1},
    {1, 0, 1},
};

#define train_count (unsigned int)(sizeof(trainOR)/sizeof(trainOR[0]))

MLAPI int gates();