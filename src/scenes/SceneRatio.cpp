#include "SceneRatio.hpp"

void SceneRatio::SetSwitchToScene(uint8_t id)
{
    // Stores the id of the scene that we will transition to.
    switchToState = id;
}

void SceneRatio::OnCreate()
{
    overlay = std::make_shared<OverlayBg>(2);
    objects.Add(overlay);

    const_list = std::make_shared<ConstantList>(getConstants());
    objects.Add(const_list);
    const_dial = std::make_shared<RatioDial>();
    objects.Add(const_dial);
    // tooltip
    const_tooltip = std::make_shared<Tooltip>(Vector2(17, 58), 2);
    objects.Add(const_tooltip);

    // hw->getButton(Gravity).onStateChanged.Connect(std::bind(&SceneRatio::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
}

void SceneRatio::OnActivate()
{
    const_list->EnterTransition();
}

void SceneRatio::Update(uint16_t deltaTime)
{
    // Process our new objects at the beginning of each frame.
    objects.ProcessNewObjects();
    objects.Update(deltaTime);
    fps = 1.0f / (deltaTime / 1000.0f); // Divide by 1000 to convert from milliseconds to seconds
}
void SceneRatio::LateUpdate(uint16_t deltaTime)
{
    objects.LateUpdate(deltaTime);
}

void SceneRatio::Draw(Display &display)
{
    buffer.fillScreen(0);
    objects.Draw(buffer);
    buffer.setTextColor(3);
    buffer.setCursor(40, 13);
    String value = String(fps, 1); // String(SharedData::base_mult, 3);
    buffer.println(value);
    display.Draw(0, 0, buffer.getBuffer(), buffer.width(), buffer.height());
}

void SceneRatio::InitBackground()
{
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

                data.interface_data.ratio += touchwheel_input;
                const_dial->SetValue(data.interface_data.ratio);
                const_tooltip->SetValue(data.interface_data.ratio);
                const_list->SetSelectedIndexByValue(data.interface_data.ratio);
            }
            else if (interface.hw.GetTouchwheel().GetSideHorizontal() == TouchWheel::Halves::RIGHT)
            {
                TouchWheel::Direction increment = interface.hw.GetTouchwheel().GetIncrement();
                if (increment == TouchWheel::Direction::DECREASE)
                {
                    const_list->MoveUp();
                    data.interface_data.ratio = const_list->GetCurrentSelectionValue();
                    const_dial->SetValue(data.interface_data.ratio);
                    const_tooltip->SetValue(data.interface_data.ratio);
                }
                else if (increment == TouchWheel::Direction::INCREASE)
                {
                    const_list->MoveDown();
                    data.interface_data.ratio = const_list->GetCurrentSelectionValue();
                    const_dial->SetValue(data.interface_data.ratio);
                    const_tooltip->SetValue(data.interface_data.ratio);
                }
            }
        }
    }
}