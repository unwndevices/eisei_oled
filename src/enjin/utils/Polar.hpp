#ifndef POLAR_HPP
#define POLAR_HPP

#include <stdint.h>
#include "Types.hpp"

namespace Polar
{
    Vector2 RadialToCartesian(float phase, uint8_t radius, Vector2 center = Vector2(64, 64));

} // namespace Polar

#endif // !POLAR_HPP