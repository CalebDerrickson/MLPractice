#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>



static float rand_float(void)
{
    return (float)rand()/ (float)RAND_MAX;
}

static float sigmoidf (float x)
{
    return 1.f / (1.f + expf(-x));
}

static float eps = 1e-3;
static float rate = 1e-3;