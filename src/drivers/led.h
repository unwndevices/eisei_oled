#ifndef LED_HPP
#define LED_HPP
#include <Arduino.h>

#define BIT_PWM 10
#define MIN_HOLD 400

#define LED_OFF (uint16_t)0
#define LED_ON (uint16_t)1023

typedef unsigned long long ullong;

// TODO implement pulsing

class Led
{
public:
    Led(uint8_t pin);

    /// write value to led
    void set(int16_t value);
    void set(float value);

    /// write value to led, then hold it for a set time
    void set(int16_t value, int16_t time);
    void set(float value, int16_t time);

    void setBlink(int16_t value, uint16_t time = 200);
    void stopBlink();
    void stopHold()
    {
        isHold = false;
    };
    void update();

    void directWrite(uint16_t value);
    void write(uint16_t value);

private:
    uint16_t read();

    uint8_t _ledId;
    static uint8_t ledAmount;

    volatile int16_t baseValue = 0;
    volatile int16_t holdValue = 0;
    volatile int16_t holdTime = 0;
    volatile ullong holdTimeStart = 0;
    volatile bool isHold = false;
    /// curve-corrected brightness output lookup table
    static int brightnessLut[];

    volatile bool isBlinking = false;
    volatile int16_t blinkValue = 0;
    volatile uint16_t blinkInterval = 0;
    volatile ullong blinkStartTime = 0;
};

#endif // LED_HPP