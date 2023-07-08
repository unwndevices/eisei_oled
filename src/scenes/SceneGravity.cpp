#include "SceneGravity.hpp"
#include <Fonts/VGATypewriter.h>

void SceneGravity::SetSwitchToScene(uint8_t id)
{
    // Stores the id of the scene that we will transition to.
    switchToState = id;
}

void SceneGravity::OnCreate()
{
    buffer.setFont(&VGATypewriter8pt7b);
    buffer.setTextColor(15U);

    InitBackground();
    // gravity_fields = std::make_shared<GravityFields>(satellites);
    // objects.Add(gravity_fields);
    //  tooltip

    dial = std::make_shared<ButtonDial>(String("Maj"), 62, 40, 12);
    objects.Add(dial);
    interface.hw.GetButton(Pages::Gravity).onStateChanged.Connect(std::bind(&SceneGravity::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    interface.hw.GetButton(Sats::a).onStateChanged.Connect(std::bind(&SceneGravity::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    interface.hw.GetButton(Pages::Phase).onStateChanged.Connect(std::bind(&SceneGravity::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    interface.hw.GetButton(Pages::Mass).onStateChanged.Connect(std::bind(&SceneGravity::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    current_page = SW_GRAVITY;

    interface.hw.GetTouchwheel().onButtonClick.Connect(std::bind(&SceneGravity::ProcessTouchButton, this, std::placeholders::_1));

    // touch_timer.Restart();
}

void SceneGravity::Update(uint16_t deltaTime)
{
    // // Check if the touchwheel has been inactive for too long.
    // if (touch_active)
    // {
    //     touch_timer += deltaTime;
    // }
    // if (touch_active && touch_timer > touch_timeout)
    // {
    //     touch_active = false;
    //     current_page = 0;
    // }

    objects.ProcessNewObjects();
    objects.Update(deltaTime);
    fps = 1.0f / (deltaTime / 1000.0f); // Divide by 1000 to convert from milliseconds to seconds
}
void SceneGravity::LateUpdate(uint16_t deltaTime)
{
    objects.LateUpdate(deltaTime);
}

void SceneGravity::Draw(Display &display)
{
    objects.Draw(buffer);
    buffer.setTextColor(3);
    buffer.setCursor(50, 103);
    String value = String(fps, 1); // String(SharedData::base_mult, 3);
    buffer.println(value);

    display.Draw(0, 0, buffer.getBuffer(), buffer.width(), buffer.height());
}

void SceneGravity::InitBackground()
{
    std::shared_ptr<Object> background = std::make_shared<Object>();
    auto position = background->AddComponent<C_Position>();
    position->SetPosition(0, 0);
    auto sprite = background->AddComponent<C_Sprite>(128, 128);
    sprite->SetDrawLayer(DrawLayer::Default);
    generateStars(250);
    sprite->Load(stars.getBuffer(), 128, 128);
    objects.Add(background);
}

void SceneGravity::generateStars(uint16_t amount)
{
    randomSeed(touchRead(9));
    for (int k = 0; k < amount; k++)
    {
        int x = rand() % 128;
        int y = rand() % 128;
        stars.drawPixel(x, y, rand() % 1 + 2);
    }
}

void SceneGravity::ProcessButton(int id, Button::State state)
{
    if (state == Button::State::CLICKED)
    {
        current_page = id;
        // touch_active = true;
        // touch_timer = 0;
    }
}

void SceneGravity::ProcessInput()
{
}

void SceneGravity::ProcessTouchButton(int buttonID)
{
    log_d("%d", buttonID);
    dial->SetValue(buttonID);
};