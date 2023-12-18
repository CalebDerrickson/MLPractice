// THIS FILE IS JUST USED TO 
// TEST FUNCTIONS/IMPLEMENTATIONS
// IN COMMON.H

#define COMMON_H
#include "common.h"
#include <time.h>

int test()
{
    srand(time(NULL));
    Mat m1 = mat_alloc(3, 1);
    mat_fill(m1, 1);
    Mat m2 = mat_alloc(1, 3);
    float ele[3] = {3, 1, 2};
    for (int i = 0; i < 3; i++) {
        m2.elements[i] = ele[i];
        m1.elements[i] = ele[i];
    }

    Mat m3 = mat_alloc(3, 3);

    mat_dot(m3, m1, m2);
    mat_print(m3);

    return 0;
}