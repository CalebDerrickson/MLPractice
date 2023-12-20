//*****************
//  DECLARATIONS
//*****************


#ifdef COMMON_H

// Includes should be relegated to what we've #defined
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "defines.h"


#ifndef COMMON_ASSERT
#include <assert.h>
#define COMMON_ASSERT assert
#endif // COMMON_ASSERT

#ifndef COMMON_MALLOC
#define COMMON_MALLOC malloc
#endif //COMMON_MALLOC

#define MAT_PRINT(m) mat_print(m, #m)

static float eps = 1e-3;
static float rate = 1e-3;

static float rand_float(void);
static float sigmoidf(float x);

typedef struct Mat{
    size_t rows;
    size_t cols;
    size_t stride;
    float* elements;
} Mat;

#define MAT_AT(m, i, j) m.elements[(i)*(m).stride + (j)]

static Mat mat_alloc(size_t rows, size_t cols);
static void mat_rand(Mat m, float low, float high);
static void mat_fill(Mat m, float val);
static void mat_dot(Mat C, Mat A, Mat B);
static Mat mat_row(Mat m, size_t row);
// static Mat mat_cols(Mat m, size_t col);
static void mat_copy(Mat dst, Mat src);
static Mat mat_transpose(Mat m);
static void mat_sum(Mat B, Mat A);
static void mat_print(Mat A, const char* name);
static void mat_sig(Mat A);

#endif //COMMON_H

//*****************
//  DEFINTITIONS
//*****************

#ifdef COMMON_H

float sigmoidf(float x)
{
    return 1.f / (1.f + expf(-x));
}

float rand_float(void)
{
    return (float)rand()/ (float)RAND_MAX;
}

Mat mat_alloc(size_t rows, size_t cols)
{
    Mat m;
    m.rows = rows;
    m.cols = cols;  
    m.stride = cols;
    m.elements = COMMON_MALLOC(sizeof(*m.elements)*rows*cols);
    COMMON_ASSERT(m.elements != NULL);
    return m;
}

void mat_rand(Mat m, float low, float high)
{

    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            MAT_AT(m, i, j) = low + (high - low)*rand_float();
        }
    }
}

void mat_fill(Mat m, float val)
{
    for (size_t i = 0; i < m.rows; i++) {
        for (size_t j = 0; j < m.cols; j++) {
            MAT_AT(m, i, j) = val;
        }
    }
}

void mat_dot(Mat C, Mat A, Mat B)
{
    COMMON_ASSERT(A.cols == B.rows);
    COMMON_ASSERT(C.rows == A.rows);
    COMMON_ASSERT(C.cols == B.cols);
    
    size_t n = A.cols;

    for (size_t i = 0; i < C.rows; i++) {
        for (size_t j = 0; j < C.cols; j++) {
            MAT_AT(C, i, j) = 0;
            for(size_t k = 0; k < n; k++) {
                MAT_AT(C, i, j) += MAT_AT(A, i, k)*MAT_AT(B, k, j); 
            }
        }
    }
}

Mat mat_row(Mat m, size_t row)
{
    return (Mat) { 
        .rows = 1,
        .cols = m.cols,
        .stride = m.stride,
        .elements = &MAT_AT(m, row, 0), 
    };
}

// THIS ALLOCATES MEMORY
Mat mat_transpose(Mat m)
{
    Mat transposed = mat_alloc(m.cols, m.rows);

    // Copy elements with transposed indices
    for (size_t i = 0; i < transposed.rows; i++) {
        for (size_t j = 0; j < transposed.cols; j++) {
            transposed.elements[i * transposed.stride + j] = m.elements[j * m.stride + i];
        }
    }

    return transposed;
}


void mat_copy(Mat dst, Mat src)
{
    COMMON_ASSERT(dst.rows == src.rows);
    COMMON_ASSERT(dst.cols == dst.cols);

    for (size_t i = 0; i < dst.rows; i++) {
        for (size_t j = 0; j < dst.cols; j++) {
            MAT_AT (dst, i, j) = MAT_AT(src, i, j);
        }
    }

}


void mat_sum(Mat B, Mat A)
{

    COMMON_ASSERT(B.rows == A.rows);
    COMMON_ASSERT(B.cols == A.cols);
    for (size_t i = 0; i < B.rows; i++) {
        for (size_t j = 0; j < B.cols; j++) {
            MAT_AT(B, i, j) += MAT_AT(A, i, j);
        }
    }

}

void mat_print(Mat A, const char* name)
{
    printf("%s = \n[", name);
    for (size_t i = 0; i < A.rows; i++) {
        if (i != 0) {printf(" ");}
        printf("[");
        
        for (size_t j = 0; j < A.cols; j++) {
            if (j != 0) {
                printf(", %f", MAT_AT(A, i, j));
            }
            else {
                printf("%f", MAT_AT(A, i, j));
            }
        }

        if (i != A.rows -1) {printf("]\n");}
    }
    printf("]]\n");
}

void mat_sig(Mat A)
{
    for (size_t i = 0; i < A.rows; i++) {
        for (size_t j = 0; j < A.cols; j++) {
            MAT_AT(A, i, j) = sigmoidf(MAT_AT(A, i, j));
        }
    }
}

#endif // COMMON_H