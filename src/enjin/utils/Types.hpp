#ifndef TYPES_HPP
#define TYPES_HPP
#include <stdint.h>

typedef struct Vector2
{
    int16_t x;
    int16_t y;

    Vector2(int16_t x = 0, int16_t y = 0) : x(x), y(y) {}

    static inline Vector2 Lerp(const Vector2 &v1, const Vector2 &v2, float t)
    {
        return Vector2((int16_t)((float)v1.x + (float)(v2.x - v1.x) * t), (int16_t)((float)v1.y + (float)(v2.y - v1.y) * t));
    }

    Vector2 operator+(const Vector2 &other) const
    {
        return Vector2(x + other.x, y + other.y);
    }
    Vector2 operator+=(const Vector2 &other) const
    {
        return Vector2(x + other.x, y + other.y);
    }

    Vector2 operator-(const Vector2 &other) const
    {
        return Vector2(x - other.x, y - other.y);
    }

    Vector2 operator*(const int16_t &scalar) const
    {
        return Vector2(x * scalar, y * scalar);
    }

    Vector2 operator/(const int16_t &scalar) const
    {
        return Vector2(x / scalar, y / scalar);
    }
} Vector2;
#endif // !TYPES_HPP