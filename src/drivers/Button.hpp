#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <Arduino.h>
#include "enjin/Signal.hpp"
#include "USB.h"

class Button
{
public:
    enum State
    {
        IDLE,
        PRESSED,
        CLICKED,
        RELEASED,
        LONG_PRESSED,
        LONG_RELEASED
    };

    Button(int pin = 0, int debounceTime = 20)
        : pin(pin),
          id(pin),
          debounceTime(debounceTime),
          lastDebounceTime(0),
          state(IDLE), prevState(IDLE),
          pressStartTime(0),
          elapsedTime(0),
          longPressTime(1700),
          clickTime(300),
          previousReading(false),
          longPressFlag(false) {}

    void SetLongPressTime(unsigned long time)
    {
        longPressTime = time;
    }

    void Init(int pin)
    {
        pinMode(pin, INPUT_PULLUP);
    }

    void Init()
    {
        pinMode(pin, INPUT_PULLUP);
    }

    void Update()
    {
        reading = (bool)(!digitalRead(pin));

        if (reading != previousReading)
        {
            lastDebounceTime = millis();
            previousReading = reading;
            return;
        }

        if ((millis() - lastDebounceTime) > debounceTime)
        {
            switch (state)
            {
            case IDLE:
                if (reading)
                {
                    state = PRESSED;
                    pressStartTime = millis();
                    elapsedTime = 0;
                }
                break;

            case PRESSED:
                if (reading)
                {
                    elapsedTime = millis() - pressStartTime;
                }
                else if (!reading)
                {
                    if ((millis() - pressStartTime) < clickTime)
                    {
                        state = CLICKED;
                    }
                    else
                    {
                        state = RELEASED;
                    }
                }
                else if ((millis() - pressStartTime) > longPressTime)
                {
                    state = LONG_PRESSED;
                }
                break;

            case RELEASED:
                if (reading)
                {
                    state = PRESSED;
                    pressStartTime = millis();
                }
                else
                {
                    state = IDLE;
                }
                break;

            case LONG_PRESSED:
                if (!reading)
                {
                    state = LONG_RELEASED;
                }
                break;

            case LONG_RELEASED:
                if (reading)
                {
                    state = PRESSED;
                    pressStartTime = millis();
                }
                else
                {
                    state = IDLE;
                }
                break;

            case CLICKED:
                state = IDLE;
                break;
            }

            if (prevState != state)
            {
                prevState = state;
                onStateChanged.Emit(id, state);
            }
        }
    }

    State GetState()
    {
        return state;
    }

    bool IsPressed()
    {
        return reading;
    }

    float GetHoldTimeNormalized()
    {
        return (float)elapsedTime / (float)longPressTime;
    }

    Signal<int, Button::State> onStateChanged;

private:
    int pin, id;
    int debounceTime;
    unsigned long lastDebounceTime;
    State state, prevState;
    bool previousReading, reading;
    unsigned long pressStartTime, elapsedTime;
    unsigned long longPressTime, clickTime;
    bool longPressFlag;
};

#endif // !BUTTON_HPP