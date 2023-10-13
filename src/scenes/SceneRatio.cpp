#include "SceneRatio.hpp"
#include "enjin/UI/assets/icons.h"

void SceneRatio::SetSwitchToScene(uint8_t id)
{
    // Stores the id of the scene that we will transition to.
    switchToState = id;
}

void SceneRatio::OnCreate()
{
    buffer.setFont(&VGATypewriter8pt7b);
    drag_left = std::make_shared<DragSlider>(icons[Icons::gravity_icon]);
    local_objects.Add(drag_left);
    drag_right = std::make_shared<DragSlider>(icons[Icons::orbit_icon], true);
    local_objects.Add(drag_right);

    label = std::make_shared<Label>(45, 19, 2);
    local_objects.Add(label);
}

void SceneRatio::OnActivate()
{
    context.touch_timer.Restart();
    drag_left->SetValue(data.interface_data.gravity_ratio);
    drag_right->SetValue(data.interface_data.orbit_ratio);
    instances.bg->EnterTransition();

    drag_left->EnterTransition(true);
    drag_right->EnterTransition(true);
    label->SetVisibility(false);
}

void SceneRatio::OnDeactivate()
{
    instances.bg->SetVisibility(false);
    drag_left->SetVisibility(false);
    drag_right->SetVisibility(false);
    label->SetVisibility(false);
}

void SceneRatio::Update(uint16_t deltaTime)
{
    BaseScene::Update(deltaTime); // Call the base class's Update method

    // Process our new objects at the beginning of each frame.
    local_objects.ProcessNewObjects();
    local_objects.Update(deltaTime);
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
    display.Draw(0, 0, buffer.getBuffer(), buffer.width(), buffer.height());
}

void SceneRatio::ProcessIncrement(TouchWheel::Direction direction)
{
}

void SceneRatio::ProcessInput()
{
    if (interface.hw.GetTouchwheel().IsTouched())
    {
        context.touch_timer.Restart();

        float touchwheel_input = interface.hw.GetTouchwheel().GetSpeed();
        if (touchwheel_input != 0.0f)
        {
            if (interface.hw.GetTouchwheel().GetSideHorizontal() == TouchWheel::Halves::RIGHT)
            {
                drag_right->SetActive(true);
                drag_left->SetActive(false);
                data.interface_data.orbit_ratio += touchwheel_input;
                data.interface_data.orbit_ratio = constrain(data.interface_data.orbit_ratio, -1.0f, 1.0f);
                drag_right->SetValue(data.interface_data.orbit_ratio);
                label->SetVisibility(true);
                if (data.interface_data.orbit_ratio < 0.0f)
                    label->SetValue(abs(data.interface_data.orbit_ratio), "", "/ ");
                else if (data.interface_data.orbit_ratio > 0.0f)
                    label->SetValue(data.interface_data.orbit_ratio, "", "* ");
                else
                    label->SetValue(0.0f);
            }
            else if (interface.hw.GetTouchwheel().GetSideHorizontal() == TouchWheel::Halves::LEFT)
            {
                drag_left->SetActive(true);
                drag_right->SetActive(false);

                data.interface_data.gravity_ratio += touchwheel_input;
                data.interface_data.gravity_ratio = constrain(data.interface_data.gravity_ratio, -1.0f, 1.0f);
                drag_left->SetValue(data.interface_data.gravity_ratio);
                label->SetVisibility(true);
                if (data.interface_data.gravity_ratio < 0.0f)
                    label->SetValue(abs(data.interface_data.gravity_ratio), "", "/ ");
                else if (data.interface_data.gravity_ratio > 0.0f)
                    label->SetValue(data.interface_data.gravity_ratio, "", "* ");
                else
                    label->SetValue(0.0f);
            }
        }
    }
    else
    {
        drag_left->SetActive(false);
        drag_right->SetActive(false);
        label->SetVisibility(false);
    }
}