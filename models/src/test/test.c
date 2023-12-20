// THIS FILE IS JUST USED TO 
// TEST FUNCTIONS/IMPLEMENTATIONS
// IN COMMON.H

#define COMMON_H
#include "common.h"
#include <time.h>

int test()
{
    size_t arch[] = {2, 2, 1};
    NN nn = nn_alloc(arch, ARRAY_LEN(arch));
    NN_PRINT(nn);

    return 0;
}