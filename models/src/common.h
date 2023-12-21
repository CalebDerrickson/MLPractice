//*****************
//  DECLARATIONS
//*****************


#ifdef COMMON_H

// Includes should be relegated to what we've #defined
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "defines.h"


#ifndef COMMON_ASSERT
#include <assert.h>
#define COMMON_ASSERT assert
#endif // COMMON_ASSERT

#ifndef COMMON_MALLOC
#define COMMON_MALLOC malloc
#endif //COMMON_MALLOC

#define MAT_PRINT(m) mat_print(m, #m, 0)

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

#define ARRAY_LEN(xs) sizeof((xs)) / sizeof((xs)[0])
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
static void mat_print(Mat A, const char* name, size_t padding);
static void mat_sig(Mat A);

typedef struct NN {
    size_t num_layers;
    // pointers to weight matrices to each layer
    Mat* ws; 
    // pointers to bias matrices to each layer
    Mat* bs; 
    // pointers to activation layers. Always has size num_layers+1
    Mat *as;  
} NN;

/**
 * @brief Allocates and returns memory for a "complete" neural network of specified parameters.
 * @param arch The architecture of the Neural Network. This is given as an array with elements
 * {SIZE OF INPUT ARRAY, {NUMBER OF HIDDEN LAYERS}, SIZE OF OUTPUT ARRAY}
 * @param arch_count The size of the arch array
 * @returns Neural Network of specified parameters
 */
static NN nn_alloc(size_t *arch, size_t arch_count);
static void nn_print(NN nn, const char* name);
static void nn_rand(NN nn, float low, float high);
static void nn_forward(NN nn);
static float nn_cost(NN nn, Mat ti, Mat to);

// Might need to pass in epsilon to change it 
static void nn_finite_diff(NN nn, NN g, Mat ti, Mat to);

static void nn_learn(NN nn, NN g);

#define NN_PRINT(nn) nn_print(nn, #nn)
#define NN_INPUT(nn) (nn).as[0]
#define NN_OUTPUT(nn) (nn).as[(nn).num_layers]

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
    // Zeroing memory 
    memset(m.elements, 0, m.rows * m.cols * sizeof(float));
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

void mat_print(Mat A, const char* name, size_t padding)
{
    printf("%*s%s = \n", (int)padding, "", name);
    for (size_t i = 0; i < A.rows; i++) {
        i == 0 ? printf("%*s[[", (int) padding, "") : printf(" ");
        if (i != 0) {printf("%*s[", (int) padding, "");}
        
        for (size_t j = 0; j < A.cols; j++) {
            if (j != 0) {
                printf(" %f", MAT_AT(A, i, j));
            }
            else {
                printf("%f", MAT_AT(A, i, j));
            }
        }

        if (i != A.rows -1) {printf("]\n");}
    }
    printf("]]%*s\n", (int) padding, "");
}

void mat_sig(Mat A)
{
    for (size_t i = 0; i < A.rows; i++) {
        for (size_t j = 0; j < A.cols; j++) {
            MAT_AT(A, i, j) = sigmoidf(MAT_AT(A, i, j));
        }
    }
}

NN nn_alloc(size_t *arch, size_t arch_count)
{
    COMMON_ASSERT(arch_count > 0);
    NN nn;
    nn.num_layers = arch_count - 1;
    
    nn.ws = COMMON_MALLOC(sizeof(*nn.ws) * nn.num_layers);
    COMMON_ASSERT(nn.ws != NULL);

    nn.bs = COMMON_MALLOC(sizeof(*nn.bs) * nn.num_layers);
    COMMON_ASSERT(nn.bs != NULL);

    nn.as =  COMMON_MALLOC(sizeof(*nn.bs) * (nn.num_layers+1));
    COMMON_ASSERT(nn.as != NULL);

    nn.as[0] = mat_alloc(arch[0], 1);
    for (size_t i = 1; i < arch_count; i++) {
        nn.ws[i-1] = mat_alloc(arch[i], nn.as[i-1].rows);
        nn.bs[i-1] = mat_alloc(arch[i], 1);
        nn.as[i] = mat_alloc(arch[i], 1);
    }
    return nn;
}

void nn_print(NN nn, const char* name)
{
    char buff[256];
    printf("%s = [\n", name);
    for (size_t i = 0; i < nn.num_layers; i++) {
        snprintf(buff, sizeof(buff), "ws %llu", i);
        mat_print(nn.ws[i], buff, 4);
        snprintf(buff, sizeof(buff), "bs %llu", i);
        mat_print(nn.bs[i], buff, 4);
    }

    printf("]\n");
}

void nn_rand(NN nn, float low, float high)
{
    for (size_t i = 0; i < nn.num_layers; i++) {
        mat_rand(nn.ws[i], low, high);
        mat_rand(nn.bs[i], low, high);
    }
}

void nn_forward(NN nn)
{
    for (size_t i = 0; i < nn.num_layers; i++) {
        mat_dot(nn.as[i+1], nn.ws[i], nn.as[i]);
        mat_sum(nn.as[i+1], nn.bs[i]);
        mat_sig(nn.as[i+1]);
    }
}

float nn_cost(NN nn, Mat ti, Mat to)
{
    assert(ti.rows == to.rows);
    assert(to.cols == NN_OUTPUT(nn).cols);
    size_t n = ti.rows;

    float c = 0;
    for (size_t i = 0; i < n; i++) {
        Mat x = mat_row(ti, i);
        Mat y = mat_row(to, i);

        // This is a hack to get shapes correct.
        // Either change ti and to to have correct shapes,
        // or keep this.
        Mat x_T = mat_transpose(x);

        mat_copy(NN_INPUT(nn), x_T);
        
        free(x_T.elements);

        nn_forward(nn);
        
        size_t q = to.cols;
        for (size_t j = 0; j < q; j++) {
            float d = MAT_AT(NN_OUTPUT(nn), j, 0) - MAT_AT(y, 0, j);
            c+= d*d;
        }
    }
    return c/n;
}

void nn_finite_diff(NN nn, NN g, Mat ti, Mat to)
{
    float saved;
    float c = nn_cost(nn, ti, to);
    for (size_t i = 0; i < nn.num_layers; i++) {
        for (size_t j = 0; j < nn.ws[i].rows; j++) {
            for (size_t k = 0; k < nn.ws[i].cols; k++) {
                saved = MAT_AT(nn.ws[i], j, k);
                MAT_AT(nn.ws[i], j, k)+= eps;
                MAT_AT(g.ws[i], j, k) = (nn_cost(nn, ti, to) - c) / eps;
                MAT_AT(nn.ws[i], j, k) = saved;
            }
        }
        for (size_t j = 0; j < nn.bs[i].rows; j++) {
            for (size_t k = 0; k < nn.bs[i].cols; k++) {
                saved = MAT_AT(nn.bs[i], j, k);
                MAT_AT(nn.bs[i], j, k)+= eps;
                MAT_AT(g.bs[i], j, k) = (nn_cost(nn, ti, to) - c) / eps;
                MAT_AT(nn.bs[i], j, k) = saved;
            }
        }
    }
}

void nn_learn(NN nn, NN g) 
{
    for (size_t i = 0; i < nn.num_layers; i++) {
        for (size_t j = 0; j < nn.ws[i].rows; j++) {
            for (size_t k = 0; k < nn.ws[i].cols; k++) {
                MAT_AT(nn.ws[i], j, k) -= rate*MAT_AT(g.ws[i], j, k);
            }
        }
        for (size_t j = 0; j < nn.bs[i].rows; j++) {
            for (size_t k = 0; k < nn.bs[i].cols; k++) {
                MAT_AT(nn.bs[i], j, k) -= rate*MAT_AT(g.bs[i], j, k);
            }
        }
    }

}
#endif // COMMON_H