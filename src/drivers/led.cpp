#include "led.h"
#include "led_LUT.h"
#include "pinout.h"

uint8_t Led::ledAmount = 0;

Led::Led(uint8_t pin)
{
    uint8_t _pin = pin;
    _ledId = ledAmount; // used to assign a different pwm channel to each led
    ++ledAmount;
    pinMode(_pin, OUTPUT);
    ledcAttachPin(_pin, _ledId);
    ledcSetup(_ledId, 5000, BIT_PWM);
}

void Led::set(uint16_t value)
{
    baseValue = constrain(value, 0, LED_ON);
}

void Led::set(float value)
{
    baseValue = (uint16_t)(constrain(value, 0.0f, 1.0f) * LED_ON);
}

void Led::set(uint16_t value, uint16_t time)
{
    holdValue = constrain(value, 0, LED_ON);
    holdTime = time;
    isHold = true;
}

void Led::set(float value, uint16_t time)
{
    holdValue = (uint16_t)(constrain(value, 0.0f, 1.0f) * LED_ON);
    holdTime = time;
    isHold = true;
}

void Led::write(uint16_t value)
{
    ledcWrite(_ledId, brightnessLut[value]);
}

void Led::directWrite(uint16_t value)
{
    ledcWrite(_ledId, value);
}
uint16_t Led::read()
{
    return ledcRead(_ledId);
}

void Led::update()
{
    if (isHold && !holdTimeStart)
    {
        holdTimeStart = millis();
        write(holdValue);
    }
    else if (isHold && holdTimeStart)
    {
        if (holdTime <= millis() - holdTimeStart)
        {
            isHold = false;
            holdTimeStart = 0;
            write(baseValue);
        }
    }

    else if (!isHold)
    {
        if (baseValue != read())
            write(baseValue);
    }
}