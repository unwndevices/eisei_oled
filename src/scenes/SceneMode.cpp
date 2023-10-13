#include "SceneMode.hpp"

void SceneMode::SetSwitchToScene(uint8_t id)
{
    // Stores the id of the scene that we will transition to.
    switchToState = id;
}

void SceneMode::OnCreate()
{
    buffer.setFont(&VGATypewriter8pt7b);
    std::vector<std::string> items;
    items.push_back(data.modes[0].name);
    items.push_back(data.modes[1].name);
    items.push_back("pipi");
    items.push_back("caca");
    list = std::make_shared<ItemList>(items);
    local_objects.Add(list);
}

void SceneMode::OnActivate()
{
    std::vector<std::string> items;
    items.push_back(data.modes[0].name);
    items.push_back(data.modes[1].name);
    items.push_back(data.modes[2].name);
    items.push_back(data.modes[3].name);
    list->UpdateList(items);
    context.touch_timer.Restart();
    // drag_left->SetValue(data.interface_data.gravity_ratio);
    // drag_right->SetValue(data.interface_data.orbit_ratio);
    instances.bg->EnterTransition();
    list->SetVisibility(true);
}

void SceneMode::OnDeactivate()
{
    instances.bg->SetVisibility(false);
    list->SetVisibility(false);
    // drag_left->SetVisibility(false);
    // drag_right->SetVisibility(false);
    // label->SetVisibility(false);
}

void SceneMode::Update(uint16_t deltaTime)
{
    BaseScene::Update(deltaTime); // Call the base class's Update method

    // Process our new objects at the beginning of each frame.
    local_objects.ProcessNewObjects();
    local_objects.Update(deltaTime);
}
void SceneMode::LateUpdate(uint16_t deltaTime)
{
    BaseScene::LateUpdate(deltaTime); // Call the base class's Draw method

    local_objects.LateUpdate(deltaTime);
}

void SceneMode::Draw(Display &display)
{
    BaseScene::Draw(display); // Call the base class's Draw method

    local_objects.Draw(buffer);
    display.Draw(0, 0, buffer.getBuffer(), buffer.width(), buffer.height());
}

void SceneMode::ProcessIncrement(TouchWheel::Direction direction)
{
}

void SceneMode::ProcessInput()
{
    if (interface.hw.GetTouchwheel().IsTouched())
    {
        context.touch_timer.Restart();

        float touchwheel_input = interface.hw.GetTouchwheel().GetSpeed();
        if (touchwheel_input != 0.0f)
        {
            TouchWheel::Direction increment = interface.hw.GetTouchwheel().GetIncrement();
            if (increment == TouchWheel::Direction::DECREASE)
            {
                list->MoveUp();
            }
            else if (increment == TouchWheel::Direction::INCREASE)
            {
                list->MoveDown();
            }
        }
    }
    else
    {
        // drag_left->SetActive(false);
        // drag_right->SetActive(false);
        // label->SetVisibility(false);
    }
}