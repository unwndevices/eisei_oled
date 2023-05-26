#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <Arduino.h>
#include <Signal.hpp>

class Button
{
public:
    enum State
    {
        IDLE,
        PRESSED,
        RELEASED,
        CLICKED,
        LONG_PRESSED,
        LONG_RELEASED
    };

    Button(int pin, int debounceTime = 10)
        : pin(pin),
          debounceTime(debounceTime),
          clickTime(100),
          longPressTime(1000),
          lastState(true),
          prevState(IDLE) {}

    void SetClickTime(unsigned long time)
    {
        clickTime = time;
    }

    void SetLongPressTime(unsigned long time)
    {
        longPressTime = time;
    }

    void Init()
    {
        pinMode(pin, INPUT_PULLUP);
    }

    void Update()
    {
        bool reading = digitalRead(pin);

        if (reading != lastState)
        {
            lastDebounceTime = millis();
        }

        if (millis() - lastDebounceTime > debounceTime)
        {
            if (!reading)
            {
                state = PRESSED;
                pressStartTime = millis();
            }
            else if (reading)
            {
                state = RELEASED;

                if (millis() - pressStartTime < clickTime)
                {
                    state = CLICKED;
                }
                else if (millis() - pressStartTime > longPressTime)
                {
                    state = LONG_RELEASED;
                }
            }
            if (!reading && millis() - pressStartTime > longPressTime)
            {
                state = LONG_PRESSED;
            }
            
            if (prevState != state)
            {
                prevState = state;
                onStateChanged.Emit(state);
            }
        }
        lastState = reading;
    }

    State
    GetState()
    {
        return state;
    }

    Signal<Button::State> onStateChanged;

private:
    const int pin;
    const int debounceTime;
    unsigned long lastDebounceTime;
    State state, prevState;
    bool lastState;
    unsigned long pressStartTime;
    unsigned long clickTime;
    unsigned long longPressTime;
};

#endif // !BUTTON_HPP