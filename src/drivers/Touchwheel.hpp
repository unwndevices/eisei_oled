#ifndef TOUCHWHEEL_HPP
#define TOUCHWHEEL_HPP

#include <Arduino.h>
#include "enjin/Signal.hpp"
#include "enjin/utils/Timer.hpp"

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
    const float dataSmoothingFactor = 0.75;
    const float baselineSmoothingFactor = 0.9995;

    // state
    uint16_t _threshold = 600;
    bool _pressed = false;
    int _lastValue = 0;
};

class TouchWheel
{
public:
    enum Halves
    {
        LEFT,
        RIGHT,
        TOP,
        BOTTOM
    };
    enum Direction
    {
        IDLE,
        DECREASE,
        INCREASE
    };

    TouchWheel(){};

    // Constructor that initializes the number of sensors
    uint8_t touchGpio[8]{9, 8, 3, 10, 5, 4, 6, 7};
    CapTouch t[NUM_SENSORS];
    void Init();
    float GetPosition() { return lastPosition; };
    // Method to read values from the wheel and calculate position, direction, and speed
    float GetSpeed() { return speed * (float)direction; };
    float GetDistance()
    {
        return distance;
    };
    int GetButtonPress() const;

    Direction GetIncrement()
    {
        if (incrementDistance <= -0.1f)
        {
            incrementDistance = 0.0f;
            return DECREASE;
        }
        else if (incrementDistance >= 0.1f)
        {
            incrementDistance = 0.0f;
            return INCREASE;
        }
        else
        {
            return IDLE;
        }
    };
    bool ReadValues();

    void Update()
    {
        float prevPosition = GetPosition();
        if (ReadValues())
        {
            onPositionChanged.Emit(GetSpeed());
            incrementDistance += GetSpeed();
            if (!clickDetected)
            {
                clickDetected = true;
                pressTime = millis();
            }
        }
        else
        {
            if (clickDetected)
            {
                releaseTime = millis();
                if (releaseTime - pressTime <= clickTime)
                {
                    // float buttonPosition = GetPosition();
                    // int buttonId = -1;

                    // for (int id = 0; id < numButtons; id++)
                    // {
                    //     float lowerBound = 0.003f + static_cast<float>(id) / numButtons;
                    //     float upperBound = static_cast<float>(id + 1) / numButtons - 0.003f;

                    //     if (buttonPosition >= lowerBound && buttonPosition < upperBound)
                    //     {
                    //         buttonId = id;
                    //         break;
                    //     }
                    // }

                    // if (buttonId != -1)
                    // {
                    //     onButtonClick.Emit(static_cast<int>(buttonId));
                    // }

                    onClick.Emit(GetSideVertical());
                }
                    clickDetected = false;
            }
            incrementDistance = 0.0f;
        }
    }

    bool IsTouched(float threshold = 0.0f)
    {
        if (threshold == 0.0f)
            return touched;
        else if (distance > threshold)
        {
            return true;
        }
        else
            return false;
    };

    Halves GetSideHorizontal()
    {
        if ((startPosition >= 0.0f && startPosition <= 0.25f) || (startPosition > 0.75f && startPosition <= 1.0f))
            return LEFT;
        else
        {
            return RIGHT;
        }
    };

    Halves GetSideVertical()
    {
        if (startPosition >= 0.0f && startPosition <= 0.5f)
            return TOP;
        else
        {
            return BOTTOM;
        }
    }

    int sensorValues[10];
    int direction;
    float speed;

    Signal<float> onPositionChanged;
    // Signal<Direction> onIncrementChanged;
    Signal<Halves> onClick;
    Signal<int> onButtonClick;

    void SetNumButtons(int num) { numButtons = num; }

private:
    ///////////////////////////////////////////////////////////
    int touchThreshold = 4000;
    bool touched = false;
    float distance = 0.0f;
    float startPosition = 0.0f;
    float endPosition = 0.0f;
    float lastPosition = 0.0f;

    float incrementDistance = 0.0f;
    Direction increment = IDLE;
    float lastIncrement = 0.0f;

    bool clickDetected = false;
    unsigned long pressTime = 0;
    unsigned long releaseTime = 0;
    const unsigned long clickTime = 300;
    int numButtons = 12; // Default number of buttons

    Timer timer;
    ///////////////////////////////////////////////////////////
    int ReadSensorValue(int sensorNum) { return t[sensorNum].GetValue(); };
};

#endif // TOUCHWHEEL_HPP
