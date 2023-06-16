#include "Polar.hpp"
#include <math.h>
#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif

namespace Polar
{
    Vector2 RadialToCartesian(float phase, uint8_t radius, Vector2 center)
    {
        Vector2 pos;
        float radians = phase * PI * 2.0f;
        pos.x = (int16_t)(radius * -cos(radians) + center.x);
        pos.y = (int16_t)(radius * -sin(radians) + center.y);
        return pos;
    }
} // namespace Polar
