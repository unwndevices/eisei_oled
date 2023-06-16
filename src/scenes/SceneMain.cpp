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
    buffer.setTextColor(0x15);
    transmission_beam = std::make_shared<TransmissionBeam>();
    transmission_beam->SetPhase(system_data.transmitter_phase);
    transmission_beam->SetWidth(system_data.transmitter_width);

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
    //  tooltip
    tooltip_freq = std::make_shared<Tooltip>(Vector2(73, 36), 0);
    objects.Add(tooltip_freq);

    hw->getButton(Gravity).onStateChanged.Connect(std::bind(&SceneMain::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    hw->getButton(Mass).onStateChanged.Connect(std::bind(&SceneMain::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    hw->getButton(Ratio).onStateChanged.Connect(std::bind(&SceneMain::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));

    // SLIDERs
    // std::shared_ptr<Slider> slider_left = std::make_shared<Slider>(Side::LEFT);
    // objects.Add(slider_left);
}

void SceneMain::Update(uint16_t deltaTime)
{
    // Check if the touchwheel has been inactive for too long.
    if (touch_active)
    {
        touch_timer += deltaTime;
    }
    if (touch_active && touch_timer > touch_timeout)
    {
        touch_active = false;
        current_page = 0;
    }

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
        satellite->SetPhase(phase[i]);
        i++;
    }
}

void SceneMain::Draw(Adafruit_SSD1327 &display)
{
    objects.Draw(buffer);
    buffer.setTextColor(3);
    buffer.setCursor(50, 103);
    String value = String(fps, 1); // String(SharedData::base_mult, 3);
    buffer.println(value);

    display.drawGrayscaleBitmap(0, 0, buffer.getBuffer(), buffer.width(), buffer.height());
}

void SceneMain::InitBackground()
{
    std::shared_ptr<Object> background = std::make_shared<Object>();
    auto position = background->AddComponent<C_Position>();
    position->SetPosition(0, 0);
    auto sprite = background->AddComponent<C_Sprite>();
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
        stars.drawPixel(x, y, rand() % 1 + 2);
    }
}

void SceneMain::ProcessButton(int id, Button::State state)
{
    switch (state)
    {
    case Button::State::PRESSED:
        current_page = id;
        touch_active = true;
        touch_timer = 0;
        break;

    default:
        break;
    }

    switch (current_page)
    {
    case SW_GRAVITY:
        tooltip_freq->SetActive(true);
        break;
    case SW_RATIO:
        sceneStateMachine.SwitchTo(2);
        break;
    case SW_MASS:
        transmission_beam->SetMode(C_TransmissionBeam::WIDTH_EDIT);
        break;
    default:
        break;
    }
}

void SceneMain::ProcessInput()
{
    if (current_page == SW_GRAVITY) // TODO use better id for pages
    {
        if (touch_active)
        {
            if (touch_timer)
            {
                float touchwheel_input = hw->getTouchwheel().GetSpeed();
                if (touchwheel_input != 0.0f)
                {
                    touch_timer = 0;
                    system_data.frequency += touchwheel_input * 40.0f;
                    tooltip_freq->SetValue(system_data.frequency);
                }
            }
        }
    }

    else if (current_page == SW_MASS) // TODO use better id for pages
    {
        if (hw->getButton(Pages::Mass).GetState() == Button::State::LONG_PRESSED)
        {
            float touchwheel_input = hw->getTouchwheel().GetPosition();
            if (hw->getTouchwheel().IsTouched())
            {
                touch_timer = 0;
                system_data.transmitter_phase = touchwheel_input;
                transmission_beam->SetPhase(touchwheel_input);
            }
        }
        else
        {
            float touchwheel_input = hw->getTouchwheel().GetSpeed();
            if (touchwheel_input != 0.0f)
            {
                touch_timer = 0;
                system_data.transmitter_width = fmin(system_data.transmitter_width + touchwheel_input * 0.1f, 1.0f);
                transmission_beam->SetWidth(system_data.transmitter_width);
            }
        }
    }
    else
    {
        tooltip_freq->SetActive(false);
        transmission_beam->SetMode(C_TransmissionBeam::IDLE);
    }
}