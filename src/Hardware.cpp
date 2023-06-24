#include "Hardware.hpp"

////////////// LEDS //////////////

uint16_t Leds::maxBrightness = 1024;

Led Leds::sats[4]{LED_1,
                  LED_2,
                  LED_3,
                  LED_4};

void Leds::Init()
{
    xTaskCreatePinnedToCore(this->Task, "Leds", 1024, this, 1, &taskLed, 0); // TODO test core and stack depth
}

void Leds::setLeds(float value)
{
    for (int i = 0; i < 4; i++)
    {
        sats[i].set(value);
    }
}

void Leds::setLeds(uint16_t value)
{
    for (int i = 0; i < 4; i++)
    {
        sats[i].set(value);
    }
}

void Leds::setLeds(uint16_t value, uint16_t time)
{
    for (int i = 0; i < 4; i++)
    {
        sats[i].set(value, time);
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

////////////// BUTTONS //////////////
