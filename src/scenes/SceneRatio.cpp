#include "SceneRatio.hpp"

void SceneRatio::SetSwitchToScene(uint8_t id)
{
    // Stores the id of the scene that we will transition to.
    switchToState = id;
}

void SceneRatio::OnCreate()
{
    // overlay = std::make_shared<OverlayBg>(2);
    // local_objects.Add(overlay);

    // const_list = std::make_shared<ConstantList>(getConstants());
    // local_objects.Add(const_list);
    // const_dial = std::make_shared<RatioDial>();
    // local_objects.Add(const_dial);
    // // tooltip
    // const_tooltip = std::make_shared<Tooltip>(Vector2(17, 58), 2);
    // local_objects.Add(const_tooltip);

    overlay = std::make_shared<FillUpGauge>();
    overlay->SetVisibility(true);
    overlay->SetString("grav");
    overlay->SetValue(data.interface_data.gravity_multiplier);
    // overlay->SetStringRight("fine");
    local_objects.Add(overlay);

    // hw->getButton(Gravity).onStateChanged.Connect(std::bind(&SceneRatio::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
}

void SceneRatio::OnActivate()
{
    // const_list->EnterTransition();
}

void SceneRatio::Update(uint16_t deltaTime)
{
    BaseScene::Update(deltaTime); // Call the base class's Update method

    // Process our new objects at the beginning of each frame.
    local_objects.ProcessNewObjects();
    local_objects.Update(deltaTime);
    fps = 1.0f / (deltaTime / 1000.0f); // Divide by 1000 to convert from milliseconds to seconds
}
void SceneRatio::LateUpdate(uint16_t deltaTime)
{
    BaseScene::LateUpdate(deltaTime); // Call the base class's Draw method

    local_objects.LateUpdate(deltaTime);
}

void SceneRatio::Draw(Display &display)
{
    BaseScene::Draw(display); // Call the base class's Draw method

    local_objects.Draw(buffer);
    buffer.setTextColor(3);
    buffer.setCursor(40, 13);
    String value = String(fps, 1); // String(SharedData::base_mult, 3);
    buffer.println(value);
    display.Draw(0, 0, buffer.getBuffer(), buffer.width(), buffer.height());
}

void SceneRatio::ProcessButton(int id, Button::State state)
{

    switch (id)
    {
    case SW_C:
        if (state == Button::State::CLICKED)
        {
        }
        break;
    case SW_D:
        if (state == Button::State::CLICKED)
        {
        }
        break;
    default:
        break;
    }
}

void SceneRatio::ProcessIncrement(TouchWheel::Direction direction)
{
}

void SceneRatio::ProcessInput()
{
    if (interface.hw.GetTouchwheel().IsTouched())
    {
        float touchwheel_input = interface.hw.GetTouchwheel().GetSpeed();
        if (touchwheel_input != 0.0f)
        {
            if (interface.hw.GetTouchwheel().GetSideHorizontal() == TouchWheel::Halves::LEFT)
            {
                overlay->SetString("orbit");
                data.interface_data.lfo_multiplier += touchwheel_input;
                data.interface_data.lfo_multiplier = constrain(data.interface_data.lfo_multiplier, -1.0f, 1.0f);
                overlay->SetValue(data.interface_data.lfo_multiplier);

                // data.interface_data.ratio += touchwheel_input;
                // const_dial->SetValue(data.interface_data.ratio);
                // const_tooltip->SetValue(data.interface_data.ratio);
                // const_list->SetSelectedIndexByValue(data.interface_data.ratio);
            }
            else if (interface.hw.GetTouchwheel().GetSideHorizontal() == TouchWheel::Halves::RIGHT)
            {
                overlay->SetString("grav");
                data.interface_data.gravity_multiplier += touchwheel_input;
                data.interface_data.gravity_multiplier = constrain(data.interface_data.gravity_multiplier, -1.0f, 1.0f);
                overlay->SetValue(data.interface_data.gravity_multiplier);

                // TouchWheel::Direction increment = interface.hw.GetTouchwheel().GetIncrement();
                // if (increment == TouchWheel::Direction::DECREASE)
                // {
                //     const_list->MoveUp();
                //     data.interface_data.ratio = const_list->GetCurrentSelectionValue();
                //     const_dial->SetValue(data.interface_data.ratio);
                //     const_tooltip->SetValue(data.interface_data.ratio);
                // }
                // else if (increment == TouchWheel::Direction::INCREASE)
                // {
                //     const_list->MoveDown();
                //     data.interface_data.ratio = const_list->GetCurrentSelectionValue();
                //     const_dial->SetValue(data.interface_data.ratio);
                //     const_tooltip->SetValue(data.interface_data.ratio);
                // }
            }
        }
    }
}