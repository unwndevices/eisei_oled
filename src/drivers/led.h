#ifndef LED_H
#define LED_H
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

    /// useful functions
    inline void pulse(uint16_t speed, uint16_t reps = 1, bool fade = true);

    /// write value to led
    void set(uint16_t value);
    void set(float value);

    /// write value to led, then hold it for a set time
    void set(uint16_t value, uint16_t time);
    void set(float value, uint16_t time);

    void update();

    void directWrite(uint16_t value);
    void write(uint16_t value);

private:
    uint16_t read();

    uint8_t _ledId;
    static uint8_t ledAmount;

    volatile uint16_t baseValue = 0;
    volatile uint16_t holdValue = 0;
    volatile uint16_t holdTime = 0;
    volatile ullong holdTimeStart = 0;
    volatile bool isHold = false;
    /// curve-corrected brightness output lookup table
    static int brightnessLut[];

    volatile uint16_t pulseSpeed = 0;
    volatile uint16_t pulseReps = 1;
    volatile bool pulseFade = true;
    volatile bool isPulsing = true;
};

inline void Led::pulse(uint16_t speed, uint16_t reps, bool fade)
{
    pulseSpeed = speed;
    pulseReps = reps;
    pulseFade = fade;
    isPulsing = true;
};

class Leds
{
public:
    static Led sats[4];
    void update();

    void init();
    void setLeds(float value);
    void setLeds(uint16_t value);
    void setLeds(uint16_t value, uint16_t time);

    static void task(void *handle);
    static uint16_t maxBrightness;

private:
    TaskHandle_t taskLed;
};

#endif