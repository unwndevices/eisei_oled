#include "utils.h"

float limit_and_loop_float(float val, float min, float max)
{
    if (val < min)
    {
        return fmod((fmod(val - min, max - min) + (max - min)), max - min) + min;
    }
    else if (val > max)
    {
        return fmod((fmod(val - min, max - min) + (max - min)), max - min) + min;
    }
    else
    {
        return val;
    }
}
int limit_and_loop(int val, int min, int max)
{
    if (val < min)
    {
        return max - ((min - val) % (max - min));
    }
    else if (val > max)
    {
        return min + ((val - min) % (max - min));
    }
    else
    {
        return val;
    }
}
