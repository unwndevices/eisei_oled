#include "SceneRatio.hpp"
#include "synthesis/SharedData.hpp"

void SceneRatio::SetSwitchToScene(uint8_t id)
{
    // Stores the id of the scene that we will transition to.
    switchToState = id;
}

void SceneRatio::OnCreate()
{
    std::shared_ptr<Object> ratioIcon = std::make_shared<Object>();
    std::shared_ptr<Object> bar1 = std::make_shared<Object>();
    std::shared_ptr<Object> bar2 = std::make_shared<Object>();
    std::shared_ptr<Object> bar3 = std::make_shared<Object>();
    std::shared_ptr<Object> bar4 = std::make_shared<Object>();

    InitBackground();
}

void SceneRatio::Update(uint16_t deltaTime)
{
    // Process our new objects at the beginning of each frame.
    objects.ProcessNewObjects();
    objects.Update(deltaTime);
}
void SceneRatio::LateUpdate(uint16_t deltaTime)
{
    objects.LateUpdate(deltaTime);
}

void SceneRatio::Draw(Adafruit_SSD1351 &display)
{
    objects.Draw(buffer);
    display.setCursor(50, 103);
    //String value = String(SharedData::base_mult, 3);
    //buffer.println(value);
    display.drawGrayscaleBitmap(0, 0, buffer.getBuffer(), buffer.width(), buffer.height());
}

void SceneRatio::InitBackground()
{
    std::shared_ptr<Object> background = std::make_shared<Object>();
    auto sprite = background->AddComponent<C_Sprite>();
    auto position = background->AddComponent<C_Position>();
    position->SetPosition(1, 4);
    sprite->Load((const uint8_t *)ratio_bg, 127, 127);
    objects.Add(background);
}