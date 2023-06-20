#include "SceneRatio.hpp"
#include "SharedData.hpp"
void SceneRatio::SetSwitchToScene(uint8_t id)
{
    // Stores the id of the scene that we will transition to.
    switchToState = id;
}

void SceneRatio::OnCreate()
{
    const_list = std::make_shared<ConstantList>(getConstants());
    const_list->SetPosition(Vector2(68, 18));
    objects.Add(const_list);
    const_dial = std::make_shared<RatioDial>();
    objects.Add(const_dial);
    // tooltip
    const_tooltip = std::make_shared<Tooltip>(Vector2(17, 58), 2);
    objects.Add(const_tooltip);
    hw->getButton(Gravity).onStateChanged.Connect(std::bind(&SceneRatio::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
    hw->getTouchwheel().onIncrementChanged.Connect(std::bind(&SceneRatio::ProcessIncrement, this, std::placeholders::_1));
    InitBackground();
}

void SceneRatio::OnActivate()
{
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
    std::shared_ptr<Object> background = std::make_shared<Object>();
    auto sprite = background->AddComponent<C_Sprite>();
    auto position = background->AddComponent<C_Position>();
    position->SetPosition(1, 1);
    sprite->Load((const uint8_t *)ratio_overlay, 127, 127, 0x7);
    objects.Add(background);
}

void SceneRatio::ProcessButton(int id, Button::State state)
{

    switch (id)
    {
    case SW_C:
        if (state == Button::State::PRESSED)
        {
        }
        break;
    case SW_D:
        if (state == Button::State::PRESSED)
        {
        }

        break;
    case SW_GRAVITY:
        if (state == Button::State::PRESSED)
            sceneStateMachine.SwitchTo(1);
        break;
    default:
        break;
    }
}

void SceneRatio::ProcessIncrement(TouchWheel::Direction direction)
{
    if (hw->getTouchwheel().GetStartingSide() == TouchWheel::Halves::RIGHT)
    {
        if (direction == TouchWheel::Direction::DECREASE)
        {
            const_list->MoveUp();
            system_data.ratio = const_list->GetCurrentSelectionValue();
            const_dial->SetValue(system_data.ratio);
            const_tooltip->SetValue(system_data.ratio);
        }
        if (direction == TouchWheel::Direction::INCREASE)
        {
            const_list->MoveDown();
            system_data.ratio = const_list->GetCurrentSelectionValue();
            const_dial->SetValue(system_data.ratio);
            const_tooltip->SetValue(system_data.ratio);
        }
    }
}

void SceneRatio::ProcessInput()
{
    if (hw->getTouchwheel().IsTouched())
    {
        if (hw->getTouchwheel().GetStartingSide() == TouchWheel::Halves::LEFT)
        {
            float touchwheel_input = hw->getTouchwheel().GetSpeed();
            if (touchwheel_input != 0.0f)
            {
                system_data.ratio += touchwheel_input;
                const_dial->SetValue(system_data.ratio);
                const_tooltip->SetValue(system_data.ratio);
                const_list->SetSelectedIndexByValue(system_data.ratio);
            }
        }
    }
}