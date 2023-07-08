#ifndef EASING_HPP
#define EASING_HPP

// Easing function that returns the input value unmodified.
using EasingFunction = float (*)(float t);

class Easing
{
public:
    static float Step(float t)
    {
        return 0.0f;
    }

    static float Linear(float t)
    {
        return t;
    }

    // Easing function that starts slow and accelerates.
    static float EaseInQuad(float t)
    {
        return t * t;
    }

    // Easing function that starts fast and decelerates.
    static float EaseOutQuad(float t)
    {
        return t * (2 - t);
    }

    // Easing function that starts and ends slow, with a peak in the middle.
    static float EaseInOutQuad(float t)
    {
        if (t < 0.5)
            return 2 * t * t;
        else
            return -1 + (4 - 2 * t) * t;
    }

    // Easing function that starts and ends fast, with a dip in the middle.
    static float EaseInOutCubic(float t)
    {
        if (t < 0.5)
            return 4 * t * t * t;
        else
            return (t - 1) * (2 * t - 2) * (2 * t - 2) + 1;
    }

    // Easing function that starts and ends slow, with a steep climb in the middle.
    static float EaseInOutQuint(float t)
    {
        if (t < 0.5)
            return 16 * t * t * t * t * t;
        else
            return 1 + 16 * (t - 1) * t * t * t * t;
    }

    // Easing function that starts fast and ends slow.
    static float EaseInCubic(float t)
    {
        return t * t * t;
    }

    // Easing function that starts slow and ends fast.
    static float EaseOutCubic(float t)
    {
        float t1 = t - 1;
        return t1 * t1 * t1 + 1;
    }

    // Easing function that starts and ends slow, with a peak and dip in the middle.
    static float EaseInOutCirc(float t)
    {
        if (t < 0.5)
            return (1 - sqrt(1 - 4 * t * t)) * 0.5;
        else
            return (sqrt(-(2 * t - 3) * (2 * t - 1)) + 1) * 0.5;
    }

    // Easing function that starts slow, accelerates, and ends slow.
    static float EaseInQuart(float t)
    {
        return t * t * t * t;
    }

    // Easing function that starts fast, decelerates, and ends fast.
    static float EaseOutQuart(float t)
    {
        float t1 = t - 1;
        return 1 - t1 * t1 * t1 * t1;
    }

    // Easing function that starts and ends slow, with a peak and dip in the middle.
    static float EaseInOutSine(float t)
    {
        return -0.5 * (cos(PI * t) - 1);
    }
};
#endif /* EASING_HPP */
