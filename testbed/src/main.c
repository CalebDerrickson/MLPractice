#include <stdio.h>
#include <stdlib.h>
#include <time.h>


float train[][2] = {
    {0, 0},
    {1, 2},
    {3, 6},
    {4, 8},
    {5, 10},
    {6, 12},
};

float rand_float(void)
{
    return (float)rand()/ (float)RAND_MAX;
}


int main() {

    srand(time(NULL));
    float w = rand_float()*10.0f;
    printf("%f", w);
    return 0;
}