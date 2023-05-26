#include "SceneMain.hpp"
#include "synthesis/SharedData.hpp"
#include "drivers/touchwheel.h"
#include <Fonts/VGATypewriter.h>

void debugState(Button::State state)
{
    log_d("State: %d", state);
}

void SceneMain::SetSwitchToScene(uint8_t id)
{
    // Stores the id of the scene that we will transition to.
    switchToState = id;
}

void SceneMain::OnCreate()
{
    buffer.setFont(&VGATypewriter8pt7b);
    buffer.setTextColor(0xFFFF);
    touch = TouchWheel::GetInstance();
    InitBackground();
    std::shared_ptr<Satellite> sat_1 = std::make_shared<Satellite>(17);
    objects.Add(sat_1);
    std::shared_ptr<Satellite> sat_2 = std::make_shared<Satellite>(31);
    objects.Add(sat_2);
    std::shared_ptr<Satellite> sat_3 = std::make_shared<Satellite>(45);
    objects.Add(sat_3);
    std::shared_ptr<Satellite> sat_4 = std::make_shared<Satellite>(59);
    objects.Add(sat_4);
    std::shared_ptr<Slider> slider_left = std::make_shared<Slider>(Side::LEFT);
    objects.Add(slider_left);
    CONNECT_SLOT(touch->onPositionChanged, Slider::ValueChanged, slider_left);
    aButton.Init();
    aButton.onStateChanged.Connect(debugState);
}

float fps = 0.0f;

void SceneMain::Update(uint16_t deltaTime)
{
    aButton.Update();
    objects.ProcessNewObjects();
    objects.Update(deltaTime);
    fps = 1.0f / (deltaTime / 1000.0f); // Divide by 1000 to convert from milliseconds to seconds
}
void SceneMain::LateUpdate(uint16_t deltaTime)
{
    objects.LateUpdate(deltaTime);
}

void SceneMain::Draw(Adafruit_SSD1351 &display)
{
    objects.Draw(buffer);
    for (int i = 1; i < 10; i++){
        display.drawCircle(64, 64, i * 2 , 0xF800);
    }
    display.fillScreen(0x0000);
    display.setCursor(50, 103);
    String value = String(fps, 1); //String(SharedData::base_mult, 3);
    display.println(value);
    //display.drawGrayscaleBitmap(0, 0, buffer.getBuffer(), buffer.width(), buffer.height());
}

void SceneMain::InitBackground()
{
    std::shared_ptr<Object> background = std::make_shared<Object>();
    auto sprite = background->AddComponent<C_Sprite>();
    auto position = background->AddComponent<C_Position>();
    position->SetPosition(0, 0);
    generateStars(250);
    sprite->Load(stars.getBuffer(), 128, 128, 16U);
    objects.Add(background);
}

void SceneMain::generateStars(uint16_t amount)
{
    randomSeed(touchRead(9));
    for (int k = 0; k < amount; k++)
    {
        int x = rand() % 128;
        int y = rand() % 128;
        stars.drawPixel(x, y, rand() % 12 + 2);
    }
}