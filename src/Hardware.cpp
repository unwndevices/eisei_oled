#include "Hardware.hpp"

////////////// LEDS //////////////

uint16_t Leds::maxBrightness = 1024;

Led Leds::sats[4]{LED_1,
                  LED_2,
                  LED_3,
                  LED_4};

void Leds::Init()
{
    xTaskCreatePinnedToCore(this->Task, "Leds", 2048, this, 1, &taskLed, 0); // TODO test core and stack depth
}

void Leds::SetLeds(float value)
{
    for (int i = 0; i < 4; i++)
    {
        sats[i].set(value);
    }
}

void Leds::SetLeds(int16_t value)
{
    for (int i = 0; i < 4; i++)
    {
        sats[i].set(value);
    }
}

void Leds::SetLeds(int16_t value, uint16_t time)
{
    for (int i = 0; i < 4; i++)
    {
        sats[i].set((int16_t)value, time);
    }
}

void Leds::Task(void *handle)
{
    while (1)
    {
        Leds *pThis = (Leds *)handle;
        for (int i = 0; i < 4; i++)
        {
            pThis->sats[i].update();
        }
        vTaskDelay(pdMS_TO_TICKS(5)); // TODO test delay values
    }
}

void Leds::SetSequential(float value)
{
    // Ensure value is within [0.0, 1.0]
    if (value < 0.0f)
        value = 0.0f;
    if (value > 1.0f)
        value = 1.0f;

    // Calculate brightness per LED
    int16_t brightness = (int16_t)(value * maxBrightness);

    // Distribute value among LEDs
    for (int i = 0; i < 4; ++i)
    {
        if (value > (i * 0.25f)) // check if value is high enough to illuminate this LED
        {
            float portion = (value - (i * 0.25f)) * 4; // calculate the portion of the total brightness this LED should receive
            if (portion > 1.0f)
                portion = 1.0f; // cap the portion at 1
            sats[i].set((int16_t)(portion * brightness));
        }
        else
        {
            sats[i].set((int16_t)0); // turn off this LED as the value is not high enough
        }
    }
}

////////////// BUTTONS //////////////
