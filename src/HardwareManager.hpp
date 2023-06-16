#ifndef HARDWAREMANAGER_HPP
#define HARDWAREMANAGER_HPP

#include "drivers/drivers.hpp"

class Leds
{
public:
    static Led sats[4];

    void Init();
    void setLeds(float value);
    void setLeds(uint16_t value);
    void setLeds(uint16_t value, uint16_t time);

    static void Task(void *handle);
    static uint16_t maxBrightness;

private:
    TaskHandle_t taskLed;
};

////////////// BUTTONS //////////////
static const int NUM_BUTTONS = 8;
const int buttonPins[NUM_BUTTONS] = {SW_GRAVITY, SW_PHASE, SW_MASS, SW_RATIO, SW_A, SW_B, SW_C, SW_D};

enum Pages
{
    Gravity = 0,
    Phase,
    Mass,
    Ratio
};

enum Sats
{
    a = 4,
    b,
    c,
    d
};

class ButtonManager
{
public:
    ButtonManager() : buttons{SW_GRAVITY, SW_PHASE, SW_MASS, SW_RATIO, SW_A, SW_B, SW_C, SW_D} {};
    void Init()
    {
        for (int i = 0; i < NUM_BUTTONS; i++)
        {
            buttons[i].Init();
        }
    };
    void Update()
    {
        for (int i = 0; i < NUM_BUTTONS; i++)
        {
            buttons[i].Update();
        }
    };
    Button buttons[NUM_BUTTONS];
};

class Hardware
{
public:
    Hardware(){};
    void Init()
    {
        // display before buttons to avoid setting GPIO13 to spi
        leds.Init();
        buttonManager.Init();
        touchwheel.Init();
    };

    void Update()
    {
        buttonManager.Update();
        touchwheel.Update();
    }

    Led &getLed(int index) { return leds.sats[index]; }
    Button &getButton(int index) { return buttonManager.buttons[index]; }
    TouchWheel &getTouchwheel() { return touchwheel; }

    Leds leds;
    ButtonManager buttonManager;
    TouchWheel touchwheel;
};
#endif // HARDWAREMANAGER_HPP
