#include "Touchwheel.hpp"
#include "utils/utils.h"
#include "enjin/utils/Easing.hpp"

void CapTouch::Init(uint8_t pin)
{
    _pin = pin;
    pinMode(_pin, INPUT);
    // Intialize history and smoothed value to an average of a few readings
    for (int i = 0; i < 20; i++)
    {
        raw += touchRead(_pin);
        delay(10);
    }
    raw = raw / 20;
    p3 = raw;
    p2 = raw;
    p1 = raw;
    smoothed = raw;
    baseline = raw;
}

int CapTouch::GetValue()
{
    raw = touchRead(_pin);

    p1 = raw; // Latest point in the history

    // Glitch detector
    if (abs(p3 - p1) < 5)
    { // The latest point and the two points back are pretty close
        if (abs(p2 - p3) > 3)
        { // The point in the middle is too different from the adjacent points -- ignore
            p2 = p3;
        }
    }

    // Smooth the de-glitched data to take out some noise
    smoothed = p3 * (1 - dataSmoothingFactor) + smoothed * dataSmoothingFactor;

    // Dynamic baseline tracking -- a much longer view of the de-glitched data
    if (count > 50)
    {
        baseline = p3 * (1 - baselineSmoothingFactor) + baseline * baselineSmoothingFactor;
    }

    // Shift the history
    p3 = p2;
    p2 = p1;
    // Replace this with code to read the actual sensor value
    _lastValue = smoothed - baseline;
    if (_lastValue < _threshold)
    {
        _lastValue = 0;
    }
    return _lastValue;
}
////////////////////////////////////////////

void TouchWheel::Init()
{
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        t[i].Init(touchGpio[i]);
    }
};
bool TouchWheel::ReadValues()
{
    // Read values from sensors
    int total = 0;
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        sensorValues[i] = ReadSensorValue(i);
    }

    // Find the sensor with the highest value
    int maxValue = 0;
    int maxSensor = 0;
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        if (sensorValues[i] > maxValue)
        {
            maxValue = sensorValues[i];
            maxSensor = i;
        }
    }

    if (maxValue >= touchThreshold)
    {
        // Approximate position using linear interpolation
        int nextSensor = (maxSensor + 1) % NUM_SENSORS;
        int prevSensor = (maxSensor - 1 + NUM_SENSORS) % NUM_SENSORS;

        float sum = sensorValues[maxSensor] + sensorValues[prevSensor] + sensorValues[nextSensor];

        // Proportions
        float c1 = sensorValues[prevSensor] / sum;
        float c2 = sensorValues[maxSensor] / sum;
        float c3 = sensorValues[nextSensor] / sum;

        // Offset
        float offset = 0.0f;
        if (c1 > c3)
        {
            offset = -c1;
        }
        else
        {
            offset = c3;
        }

        float offsetPad = static_cast<float>(maxSensor) + offset; // Offset relative to pad (e.g., for an 8 pad wheel, 0-7, representing 0-360 degrees)
        if (offsetPad < 0)
            offsetPad += NUM_SENSORS; // Bound check in case we go negative for half of the first wheel

        // Calculate the touch position
        float position = offsetPad / NUM_SENSORS;

        if (!touched)
        {
            touched = true;
            startPosition = position;
            lastPosition = position;
        }

        if (position > lastPosition)
        {
            direction = 1;
            speed = position - lastPosition;
            distance += speed;
        }
        else if (position < lastPosition)
        {
            direction = -1;
            speed = lastPosition - position;
            distance += speed;
        }
        else
        {
            direction = 0;
            speed = 0;
        }

        if (speed != 0)
        {
            lastPosition = position;
        }
    }
    else
    {
        touched = false;
        speed = 0.0f;
    }

    return touched;
}
