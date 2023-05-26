#ifndef TOUCHWHEEL_H
#define TOUCHWHEEL_H

#include <Arduino.h>
#include <Signal.hpp>

#define NUM_SENSORS 8

class CapTouch
{
public:
    CapTouch(){};
    CapTouch(uint8_t pin) { _pin = pin; };

    void Init(uint8_t pin);
    int GetValue();
    bool IsPressed() { return _pressed; };
    void SetThreshold(uint16_t threshold) { _threshold = threshold; };

private:
    // HW
    uint8_t _pin = 0;

    // SMOOTHING
    float p1 = 0.0, p2 = 0.0, p3 = 0.0; // 3-Point history
    float raw = 0.0;                    // Current reading
    float baseline = 0.0;
    float smoothed = 0.0;
    unsigned long count = 0;

    // Smoothing factors. The closer to one (1.0) the smoother the data. Smoothing
    // introduces a delay.
    const float dataSmoothingFactor = 0.35;
    const float baselineSmoothingFactor = 0.9995;

    // state
    uint16_t _threshold = 2000;
    bool _pressed = false;
    int _lastValue = 0;
};

class TouchWheel
{
public:
    static TouchWheel *instance;
    static TouchWheel *GetInstance()
    {
        if (instance == nullptr)
        {
            instance = new TouchWheel();
        }
        return instance;
    };

    // Constructor that initializes the number of sensors
    uint8_t touchGpio[8]{7, 9, 8, 3, 10, 5, 4, 6};
    CapTouch t[NUM_SENSORS];
    void Init();
    float GetPosition() { return lastPosition; };
    // Method to read values from the wheel and calculate position, direction, and speed
    float GetSpeed() { return speed * (float)direction; };
    float GetDistance()
    {
        return distance;
    };
    bool ReadValues();
    void Update()
    {
        if (ReadValues())
        {
            onPositionChanged.Emit(GetSpeed());
        }
    };

    int sensorValues[10];
    int direction;
    float speed;

    Signal<float> onPositionChanged;

private:
    TouchWheel(){};
    TouchWheel(const TouchWheel &) = delete;
    TouchWheel &operator=(const TouchWheel &) = delete;
    ///////////////////////////////////////////////////////////
    int touchThreshold = 9000;
    bool touched = false;
    float distance = 0.0f;
    float startPosition = 0.0;
    float endPosition = 0.0;
    float lastPosition = 0.0;
    ///////////////////////////////////////////////////////////
    int ReadSensorValue(int sensorNum) { return t[sensorNum].GetValue(); };
};

#endif // !TOUCHWHEEL_H