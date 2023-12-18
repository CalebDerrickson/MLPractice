#include "xor.h"
#define COMMON_H
#include "common.h"


static float forward(Xor m, float x1, float x2);
static Xor rand_xor();
static void print_xor(Xor m);
static Xor finite_diff(Xor m);
static Xor apply_diff(Xor m, Xor g);

static float cost(Xor m)
{
    float result = 0.0f;
    for (size_t i = 0; i < train_count; ++i) {
        float x1 = train[i][0];
        float x2 = train[i][1];
        float y = forward(m, x1, x2);
        float d = y - train[i][2];
        result += d*d;
    }
    result /= train_count;
    return result;
}

int xor()
{
    rate = 1e-1;
    eps = 1e-1;

    srand(time(NULL));
    Xor m = rand_xor();

    for (size_t i = 0; i < 1e6; i++) {
        Xor g = finite_diff(m);
        m = apply_diff(m, g);
        // printf("Cost: %f\n", cost(m));
    }

    printf("cost = %f\n", cost(m));

    printf("------------------------------\n");
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            printf("%llu ^ %llu = %f\n", i, j, forward(m, i, j));
        }
    }
    printf("------------------------------\n");
    printf("\"OR\" neuron:\n");
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            printf("%zu | %zu = %f\n", i, j, sigmoidf(m.or_w1*i + m.or_w2*j + m.or_b));
        }
    }
    printf("------------------------------\n");
    printf("\"NAND\" neuron:\n");
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            printf("~(%zu & %zu) = %f\n", i, j, sigmoidf(m.nand_w1*i + m.nand_w2*j + m.nand_b));
        }
    }
    printf("------------------------------\n");
    printf("\"AND\" neuron:\n");
    for (size_t i = 0; i < 2; ++i) {
        for (size_t j = 0; j < 2; ++j) {
            printf("%zu & %zu = %f\n", i, j, sigmoidf(m.and_w1*i + m.and_w2*j + m.and_b));
        }
    }


    return 0;
}

float forward(Xor m, float x1, float x2)
{
    float n1 = sigmoidf(m.or_w1*x1 + m.or_w2 * x2 + m.or_b);
    float n2 =sigmoidf(m.nand_w1*x1 + m.nand_w2 * x2 + m.nand_b);
    
    return sigmoidf(m.and_w1*n1 + m.and_w2*n2 + m.and_b);   
}

Xor rand_xor()
{
    Xor m;

    m.or_w1   = rand_float();
    m.or_w2   = rand_float();
    m.or_b    = rand_float();
    m.and_w1  = rand_float();
    m.and_w2  = rand_float();
    m.and_b   = rand_float();
    m.nand_w1 = rand_float();
    m.nand_w2 = rand_float();
    m.nand_b  = rand_float();

    return m;
}

void print_xor(Xor m)
{
    printf("or_w1 = %f\n",   m.or_w1);
    printf("or_w2 = %f\n",   m.or_w2);
    printf("or_b = %f\n",    m.or_b);
    printf("and_w1 = %f\n",  m.and_w1);
    printf("and_w2 = %f\n",  m.and_w2);
    printf("and_b = %f\n",   m.and_b);
    printf("nand_w1 = %f\n", m.nand_w1);
    printf("nand_w2 = %f\n", m.nand_w2);
    printf("nand_b = %f\n",  m.nand_b);
}

Xor finite_diff(Xor m)
{
    Xor g;
    float c = cost(m);
    float saved;

    saved = m.or_w1;
    m.or_w1 += eps;
    g.or_w1 = (cost(m) - c)/eps;
    m.or_w1 = saved;

    saved = m.or_w2;
    m.or_w2 += eps;
    g.or_w2 = (cost(m) - c)/eps;
    m.or_w2 = saved;

    saved = m.or_b;
    m.or_b += eps;
    g.or_b = (cost(m) - c)/eps;
    m.or_b = saved;

    saved = m.nand_w1;
    m.nand_w1 += eps;
    g.nand_w1 = (cost(m) - c)/eps;
    m.nand_w1 = saved;

    saved = m.nand_w2;
    m.nand_w2 += eps;
    g.nand_w2 = (cost(m) - c)/eps;
    m.nand_w2 = saved;

    saved = m.nand_b;
    m.nand_b += eps;
    g.nand_b = (cost(m) - c)/eps;
    m.nand_b = saved;

    saved = m.and_w1;
    m.and_w1 += eps;
    g.and_w1 = (cost(m) - c)/eps;
    m.and_w1 = saved;

    saved = m.and_w2;
    m.and_w2 += eps;
    g.and_w2 = (cost(m) - c)/eps;
    m.and_w2 = saved;

    saved = m.and_b;
    m.and_b += eps;
    g.and_b = (cost(m) - c)/eps;
    m.and_b = saved;

    return g;
}

Xor apply_diff(Xor m, Xor g)
{
    m.or_w1 -= rate*g.or_w1;
    m.or_w2 -= rate*g.or_w2;
    m.or_b -= rate*g.or_b;
    m.nand_w1 -= rate*g.nand_w1;
    m.nand_w2 -= rate*g.nand_w2;
    m.nand_b -= rate*g.nand_b;
    m.and_w1 -= rate*g.and_w1;
    m.and_w2 -= rate*g.and_w2;
    m.and_b -= rate*g.and_b;
    return m;
}