#include "SceneGravity.hpp"

void SceneGravity::OnCreate()
{
    buffer.setFont(&VGATypewriter8pt7b);
    buffer.setTextColor(15U);
    tooltip = std::make_shared<Tooltip>(Vector2(64, 64), 2);
    objects.Add(tooltip);
}

void SceneGravity::OnDestroy()
{
    // Custom cleanup for SceneGravity
}

void SceneGravity::OnActivate()
{
    tooltip->SetVisibility(true);
}

void SceneGravity::OnDeactivate()
{
    tooltip->SetVisibility(false);
}

void SceneGravity::ProcessInput()
{
    if (interface.hw.GetTouchwheel().IsTouched())
    {
        float touchwheel_input = interface.hw.GetTouchwheel().GetSpeed();
        if (touchwheel_input != 0.0f)
        {

            data.interface_data.gravity += touchwheel_input * 10.0f;
            tooltip->SetValue(data.interface_data.gravity);
        }
    }
}

void SceneGravity::Update(uint16_t deltaTime)
{
    // Custom update logic for SceneGravity
    BaseScene::Update(deltaTime); // Call the base class's Update method
}

void SceneGravity::LateUpdate(uint16_t deltaTime)
{
    // Custom late update logic for SceneGravity
    BaseScene::LateUpdate(deltaTime); // Call the base class's Draw method
}

void SceneGravity::Draw(Display &display)
{
    // Custom drawing code for SceneGravity
    BaseScene::Draw(display); // Call the base class's Draw method
}