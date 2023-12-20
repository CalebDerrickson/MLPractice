#include "xor.h"
#include "defines.h"
#define COMMON_H
#include "common.h"

typedef struct Xor {
    Mat a0, a1, a2;

    Mat w1, b1;
    Mat w2, b2;
} Xor;

static Xor xor_alloc(void);
static float cost(Xor m, Mat ti, Mat to);
static void forward(Xor m);
static void print_xor(Mat w1, Mat w2, Mat b1, Mat b2);
static void finite_diff(Xor m, Xor g, Mat ti, Mat to);
static void apply_diff(Xor m, Xor g);

float td[] = {
    0, 0, 0,
    1, 0, 1,
    0, 1, 1,
    1, 1, 0,
};

int xor() 
{
    srand(time(0));

    size_t stride = 3;
    size_t n  = sizeof(td)/sizeof(td[0])/stride;
    Mat ti = {
        .rows = n,
        .cols = 2,
        .stride = stride,
        .elements = td,
    };

    Mat to = {
        .rows = n,
        .cols = 1,
        .stride = stride,
        .elements = td+2,
    };


    // Allocating Weights and bias matrices
    Xor m = xor_alloc();
    Xor g = xor_alloc();

    mat_rand(m.w1, 0, 1);
    mat_rand(m.w2, 0, 1);
    mat_rand(m.b1, 0, 1);
    mat_rand(m.b2, 0, 1);

    //printf("cost = %f\n", cost(m, ti, to));
    eps = 1e-1;
    rate = 1e-1;

    for (size_t i = 0; i < 1e5; i++){
        finite_diff(m, g, ti, to);
        apply_diff(m, g);
        // printf("%llu, cost = %f\n", i, cost(m, ti, to));
    }


    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            MAT_AT(m.a0, 0, 0) = i;
            MAT_AT(m.a0, 1, 0) = j; 
            forward(m);
            float y = *(m.a2.elements);
            printf("%llu ^ %llu = %f\n", i, j, y);
        }
    }

    return 0;
}

Xor xor_alloc(void)
{
    Xor m;
    m.a0 = mat_alloc(2, 1);
    m.w1 = mat_alloc(2, 2);
    m.b1 = mat_alloc(2, 1);
    m.a1 = mat_alloc(2, 1);
    m.w2 = mat_alloc(1, 2);
    m.b2 = mat_alloc(1, 1);
    m.a2 = mat_alloc(1, 1);
    return m;
}

void print_xor(Mat w1, Mat w2, Mat b1, Mat b2) 
{
    printf("=================\n");
    MAT_PRINT(w1);
    MAT_PRINT(b1);
    MAT_PRINT(w2);
    MAT_PRINT(b2);
    printf("=================\n");
}

void forward(Xor m)
{
    mat_dot(m.a1, m.w1, m.a0);
    mat_sum(m.a1, m.b1);
    mat_sig(m.a1);

    mat_dot(m.a2, m.w2, m.a1);
    mat_sum(m.a2, m.b2);
    mat_sig(m.a2);
}

float cost(Xor m, Mat ti, Mat to)
{
    assert(ti.rows == to.rows);
    assert(to.cols == m.a2.cols);
    size_t n = ti.rows;

    float c = 0;
    for (size_t i = 0; i < n; i++) {
        Mat x = mat_row(ti, i);
        Mat y = mat_row(to, i);
        
        Mat x_T = mat_transpose(x);

        mat_copy(m.a0, x_T);
        
        free(x_T.elements);

        forward(m);

        size_t q = to.cols;
        for (size_t j = 0; j < q; j++) {
            float d = MAT_AT(m.a2, j, 0) - MAT_AT(y, 0, j);
            c+= d*d;
        }
    }

    return c/n;
}

void finite_diff(Xor m, Xor g, Mat ti, Mat to)
{
    float saved;
    float c = cost(m, ti, to);
    for (size_t i = 0; i < m.w1.rows; i++) {
        for (size_t j = 0; j < m.w1.cols; j++) {
            saved = MAT_AT(m.w1, i, j);
            MAT_AT(m.w1, i, j) += eps;
            MAT_AT(g.w1, i, j) = (cost(m, ti, to) - c) /eps;
            MAT_AT(m.w1, i, j) = saved;
        }
    }

    for (size_t i = 0; i < m.b1.rows; i++) {
        for (size_t j = 0; j < m.b1.cols; j++) {
            saved = MAT_AT(m.b1, i, j);
            MAT_AT(m.b1, i, j) += eps;
            MAT_AT(g.b1, i, j) = (cost(m, ti, to) - c) /eps;
            MAT_AT(m.b1, i, j) = saved;
        }
    }

    for (size_t i = 0; i < m.w2.rows; i++) {
        for (size_t j = 0; j < m.w2.cols; j++) {
            saved = MAT_AT(m.w2, i, j);
            MAT_AT(m.w2, i, j) += eps;
            MAT_AT(g.w2, i, j) = (cost(m, ti, to) - c) /eps;
            MAT_AT(m.w2, i, j) = saved;
        }
    }

    for (size_t i = 0; i < m.b2.rows; i++) {
        for (size_t j = 0; j < m.b2.cols; j++) {
            saved = MAT_AT(m.b2, i, j);
            MAT_AT(m.b2, i, j) += eps;
            MAT_AT(g.b2, i, j) = (cost(m, ti, to) - c) /eps;
            MAT_AT(m.b2, i, j) = saved;
        }
    }
}

void apply_diff(Xor m, Xor g)
{
    for (size_t i = 0; i < m.w1.rows; i++) {
        for (size_t j = 0; j < m.w1.cols; j++) {
            MAT_AT(m.w1, i, j) -= rate*MAT_AT(g.w1, i, j);
        }
    }

    for (size_t i = 0; i < m.b1.rows; i++) {
        for (size_t j = 0; j < m.b1.cols; j++) {
            MAT_AT(m.b1, i, j) -= rate*MAT_AT(g.b1, i, j);
        }
    }

    for (size_t i = 0; i < m.w2.rows; i++) {
        for (size_t j = 0; j < m.w2.cols; j++) {
            MAT_AT(m.w2, i, j) -= rate*MAT_AT(g.w2, i, j);
        }
    }

    for (size_t i = 0; i < m.b2.rows; i++) {
        for (size_t j = 0; j < m.b2.cols; j++) {
            MAT_AT(m.b2, i, j) -= rate*MAT_AT(g.b2, i, j);
        }
    }

}