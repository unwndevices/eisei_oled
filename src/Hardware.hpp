#ifndef HARDWARE_HPP
#define HARDWARE_HPP
#include <unordered_map>

#include "drivers/drivers.hpp"

class Leds
{
public:
    static Led sats[4];

    void Init();
    void SetLeds(float value);
    void SetLeds(uint16_t value);
    void SetLeds(uint16_t value, uint16_t time);
    void SetSequential(float value);

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
            buttonStates[i] = buttons[i].GetState();
        }
    };

    Button::State GetButton(int id)
    {
        return buttonStates[id];
    }

    Button buttons[NUM_BUTTONS];

private:
    std::unordered_map<int, Button::State> buttonStates;
};

class Display
{
public:
    Display() : driver(128, 128, &SPI, OLED_DC, OLED_RESET, OLED_CS, 37000000){};
    void Init(uint8_t contrast = 128)
    {
        SPI.begin(OLED_CLK, -1, OLED_MOSI, OLED_CS);
        driver.begin();
        driver.clearDisplay();
        driver.display();
        driver.setContrast(contrast);
        driver.setRotation(2); // rotate 180deg
    };

    void Draw(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h)
    {
        driver.drawGrayscaleBitmap(x, y, bitmap, w, h);
    }
    void Show()
    {
        driver.display();
    }

private:
    Adafruit_SSD1327 driver;
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
        display.Init(127);
    };

    void Update()
    {
        buttonManager.Update();
        touchwheel.Update();
    }

    Button::State GetButtonState(int id)
    {
        return buttonManager.GetButton(id);
    }

    Leds &GetLeds() { return leds; }
    Led &GetLed(int index) { return leds.sats[index]; }
    Button &GetButton(int index) { return buttonManager.buttons[index]; }
    TouchWheel &GetTouchwheel() { return touchwheel; }
    Display &GetDisplay() { return display; }

private:
    Leds leds;
    ButtonManager buttonManager;
    TouchWheel touchwheel;
    Display display;
};

#endif // HARDWARE_HPP
