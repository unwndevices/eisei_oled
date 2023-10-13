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
    void SetLeds(int16_t value);
    void SetLeds(int16_t value, uint16_t time);

    void SetBlink(uint8_t id, int16_t value, uint16_t time)
    {
        sats[id].setBlink(value, time);
    };

    void StopBlink(uint8_t id)
    {
        sats[id].stopBlink();
    };

    void StopHoldAll()
    {
        for (int i = 0; i < 4; i++)
        {
            sats[i].stopHold();
        }
    };

    void StopBlinkAll()
    {
        for (int i = 0; i < 4; i++)
        {
            sats[i].stopBlink();
        }
    };

    void SetSequential(float value);

    static void Task(void *handle);
    static uint16_t maxBrightness;

private:
    TaskHandle_t taskLed;
};

////////////// BUTTONS //////////////
static const int NUM_BUTTONS = 8;
const int buttonPins[NUM_BUTTONS] = {SW_GRAVITY, SW_PHASE, SW_MASS, SW_RATIO, SW_A, SW_B, SW_C, SW_D};

#include <unordered_map>

class ButtonManager
{
public:
    ButtonManager(){};

    void Init()
    {
        for (auto &button : buttons)
        {
            button.second.Init();
        }
    }

    void Update()
    {
        for (auto &button : buttons)
        {
            button.second.Update();
            buttonStates[button.first] = button.second.GetState();
        }
    }

    Button::State GetButton(int id)
    {
        return buttonStates[id];
    }

    void AddButton(int id, Button button)
    {
        buttons[id] = button;
        buttons[id].Init(id);
    }

    void RemoveButton(int id)
    {
        buttons.erase(id);
        buttonStates.erase(id);
    }

    std::unordered_map<int, Button> buttons;

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
    uint8_t *GetBuffer()
    {
        return driver.getBuffer();
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
        display.Init(127);
        touchwheel.Init();
    };

    void Update()
    {
        buttonManager.Update();
        touchwheel.Update();
    }

    void AddButton(int id, Button button)
    {
        buttonManager.AddButton(id, button);
    }

    Button::State GetButtonState(int id)
    {
        return buttonManager.GetButton(id);
    }

    Leds &GetLeds() { return leds; }
    Led &GetLed(int index) { return leds.sats[index]; }
    Button &GetButton(int id) { return buttonManager.buttons[id]; }
    TouchWheel &GetTouchwheel() { return touchwheel; }
    Display &GetDisplay() { return display; }

private:
    Leds leds;
    ButtonManager buttonManager;
    TouchWheel touchwheel;
    Display display;
};

#endif // HARDWARE_HPP
