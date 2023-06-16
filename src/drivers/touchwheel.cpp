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
        int previousSensor = (maxSensor - 1) % NUM_SENSORS;
        if (previousSensor < 0)
        {
            previousSensor = NUM_SENSORS - 1;
        };

        float position = (float)maxSensor + (float)(sensorValues[nextSensor] - sensorValues[maxSensor]) / (float)(sensorValues[nextSensor] + sensorValues[maxSensor]);

        // Scale position to the range of 0.0 to 1.0
        position /= (float)NUM_SENSORS;
        // position = limit_and_loop_float(position, 0.0f, 1.0f);
        if (!touched)
        {
            // distance = 0.0f;
            touched = true;
            lastPosition = position;
        }
        // Calculate direction and speed of movement
        float adjPosition = position;
        if (adjPosition < (lastPosition - 0.5f))
        {
            adjPosition += 1.0f;
        }
        else if (adjPosition > (lastPosition + 0.5f))
        {
            adjPosition -= 1.0f;
        }
        if (adjPosition > lastPosition)
        {
            direction = 1;
            speed = adjPosition - lastPosition;
            distance += speed;
        }
        else if (adjPosition < lastPosition)
        {
            direction = -1;
            speed = lastPosition - adjPosition;
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
        // distance = 0.0f;
        speed = 0.0f;
    }
    return touched;
}
