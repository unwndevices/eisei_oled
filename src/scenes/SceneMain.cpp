#include "SceneMain.hpp"
#include <Fonts/VGATypewriter.h>

void SceneMain::SetSwitchToScene(uint8_t id)
{
    // Stores the id of the scene that we will transition to.
    switchToState = id;
}

void SceneMain::OnCreate()
{
    buffer.setFont(&VGATypewriter8pt7b);
    buffer.setTextColor(15U);
    transmission_beam = std::make_shared<TransmissionBeam>();
    transmission_beam->SetPhase(data.phase[4]);
    transmission_beam->SetWidth(data.interface_data.scope_width);

    objects.Add(transmission_beam);

    for (int i = 0; i < 4; i++)
    {
        auto satellite = std::make_shared<Satellite>(20 + i * 13, 4);
        satellites.push_back(satellite);
        objects.Add(satellite);
    }

    InitBackground();

    main_planet = std::make_shared<Satellite>(0, 12, 8u);
    objects.Add(main_planet);
    // gravity_fields = std::make_shared<GravityFields>(satellites);
    // objects.Add(gravity_fields);

    // SLIDERs
    // std::shared_ptr<Slider> slider_left = std::make_shared<Slider>(Side::LEFT);
    // objects.Add(slider_left);
    // interface.hw.GetButton(Pages::Gravity).onStateChanged.Connect(std::bind(&SceneMain::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    // interface.hw.GetButton(Sats::a).onStateChanged.Connect(std::bind(&SceneMain::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    // interface.hw.GetButton(Pages::Phase).onStateChanged.Connect(std::bind(&SceneMain::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    // interface.hw.GetButton(Pages::Mass).onStateChanged.Connect(std::bind(&SceneMain::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    // current_page = SW_GRAVITY;
}

void SceneMain::OnActivate()
{
}
void SceneMain::OnDeactivate()
{
}

void SceneMain::Update(uint16_t deltaTime)
{
    objects.ProcessNewObjects();
    objects.Update(deltaTime);
    fps = 1.0f / (deltaTime / 1000.0f); // Divide by 1000 to convert from milliseconds to seconds
}
void SceneMain::LateUpdate(uint16_t deltaTime)
{
    objects.LateUpdate(deltaTime);
    int i = 0;
    for (auto &satellite : satellites)
    {
        satellite->SetPhase(data.phase[i]);
        i++;
    }
    transmission_beam->SetPhase(data.phase[4]);
}

void SceneMain::Draw(Display &display)
{
    objects.Draw(buffer);
    // buffer.setTextColor(3);
    // buffer.setCursor(50, 103);
    // String value = String(fps, 1); // String(SharedData::base_mult, 3);
    // buffer.println(value);

    display.Draw(0, 0, buffer.getBuffer(), buffer.width(), buffer.height());
}

void SceneMain::InitBackground()
{
    std::shared_ptr<Object> background = std::make_shared<Object>();
    auto position = background->AddComponent<C_Position>();
    position->SetPosition(0, 0);
    auto sprite = background->AddComponent<C_Sprite>(128, 128);
    sprite->SetDrawLayer(DrawLayer::Default);
    generateStars(250);
    for (int i = 0; i < 8; i++)
    {
        Vector2 temp_position = RadialToCartesian((float)i / 8.0f, (uint8_t)63, Vector2(64, 64));
        int16_t x = temp_position.x;
        int16_t y = temp_position.y;
        stars.drawLine(64, 64, x, y, 3);
    }
    for (auto &satellite : satellites)
    {
        satellite->drawBackground(stars);
    }

    sprite->Load(stars.getBuffer(), 128, 128);
    objects.Add(background);
}

void SceneMain::generateStars(uint16_t amount)
{
    randomSeed(touchRead(9));
    for (int k = 0; k < amount; k++)
    {
        int x = rand() % 128;
        int y = rand() % 128;
        stars.drawPixel(x, y, rand() % 1 + 2);
    }
}

void SceneMain::ProcessButton(int id, Button::State state)
{
    if (state == Button::State::CLICKED)
    {
        current_page = id;
        if (current_page == SW_MASS)
        {
            transmission_beam->SetMode(C_TransmissionBeam::WIDTH_EDIT);
        }
        else
        {
            transmission_beam->SetMode(C_TransmissionBeam::IDLE);
        }
    }
}

void SceneMain::ProcessInput()
{
    if (1)
    {
        if (current_page == SW_GRAVITY)
        {
            float touchwheel_input = interface.hw.GetTouchwheel().GetSpeed();
            if (touchwheel_input != 0.0f)
            {
                data.interface_data.gravity += touchwheel_input * 40.0f;
                //                tooltip_freq->SetValue(data.interface_data.gravity);
            }
        }
        else if (current_page == SW_MASS) // TODO use better id for pages
        {

            float touchwheel_input = interface.hw.GetTouchwheel().GetSpeed();
            if (touchwheel_input != 0.0f)
            {
                data.interface_data.scope_width = fmin(data.interface_data.scope_width + touchwheel_input * 0.1f, 1.0f);
                transmission_beam->SetWidth(data.interface_data.scope_width);
            }
        }
        else if (current_page == SW_PHASE)
        {
            float touchwheel_input = interface.hw.GetTouchwheel().GetSpeed();
            if (touchwheel_input != 0.0f)
            {

                data.interface_data.orbit_rate += touchwheel_input * 10.0f;
                // tooltip_freq->SetValue(data.interface_data.lfo_rate);
            }
        }
    }

    // if (interface.GetButtonState(Pages::Gravity) == Button::State::PRESSED)
    // {
    //     float progress = interface.hw.GetButton(Pages::Gravity).GetHoldTimeNormalized();
    //     interface.hw.GetLeds().SetSequential(progress);
    //     if (progress > 0.99f)
    //     {
    //     }
    // }
    // else
    // {
    //     interface.hw.GetLeds().SetLeds(0.0f);
    //     // overlay->SetVisibility(false);
    // }
}