#include <stdlib.h>
#include <time.h>

typedef struct{
    float x1;
    float x2;
    int category;
} Point;

float choose_random_float_in_range(float lower, float upper){
    return (float) rand() / (float) (RAND_MAX / (upper - lower)) + lower;
}