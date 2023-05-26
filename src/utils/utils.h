#ifndef UTILS_H
#define UTILS_H
#include <Arduino.h>

typedef struct Ratio
{
    float a;
    float b;
    float c;
    float d;
} Ratio;

float limit_and_loop_float(float val, float min, float max);
int limit_and_loop(int val, int min, int max);

#endif // !UTILS_H
