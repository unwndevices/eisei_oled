#include "SceneOrbit.hpp"

void SceneOrbit::OnCreate()
{
    buffer.setFont(&VGATypewriter8pt7b);
    label = std::make_shared<Label>(47, 19, 2, 0);
    label->SetValue(data.interface_data.orbit_rate, "hz");
    label->SetVisibility(false);
    local_objects.Add(label);
}

void SceneOrbit::OnDestroy()
{
}

void SceneOrbit::OnActivate()
{
    context.touch_timer.Restart();

    int i = 0;
    for (auto &satellite : instances.satellites)
    {
        satellite->pos_transition->SetAnimation(satellite->pos_animation_out);
        satellite->radius_transition->SetAnimation(satellite->radius_animation_out);

        satellite->EnterTransition();
        i++;
    }

    instances.main_planet->radius_animation_out.SetEndCallback([=]()
                                                               { label->SetVisibility(true); });
    instances.main_planet->radius_transition->SetAnimation(instances.main_planet->radius_animation_out);
    instances.main_planet->EnterTransition();

    interface.hw.GetTouchwheel().onClick.Connect(std::bind(&SceneOrbit::ProcessTouchClick, this, std::placeholders::_1));
}

void SceneOrbit::OnDeactivate()
{
    label->SetVisibility(false);
    interface.hw.GetTouchwheel().onClick.DisconnectAll();
}

void SceneOrbit::ProcessInput()
{
    if (interface.hw.GetTouchwheel().IsTouched(0.1f))
    {
        context.touch_timer.Restart();

        float touchwheel_input = interface.hw.GetTouchwheel().GetSpeed();

        data.interface_data.orbit_rate += touchwheel_input * 1.0f;
        label->SetValue(data.interface_data.orbit_rate, "hz");
    }
}

void SceneOrbit::ProcessTouchClick(TouchWheel::Halves side)
{
    context.touch_timer.Restart();

    if (side == TouchWheel::Halves::TOP)
    {
        data.interface_data.orbit_rate += 0.01f;
        label->SetValue(data.interface_data.orbit_rate, "hz");
    }
    else
    {
        data.interface_data.orbit_rate -= 0.01f;
        label->SetValue(data.interface_data.orbit_rate, "hz");
    }
}

void SceneOrbit::Update(uint16_t deltaTime)
{
    // Custom update logic for SceneOrbit
    BaseScene::Update(deltaTime); // Call the base class's Update method

    local_objects.ProcessNewObjects();
    local_objects.Update(deltaTime);
}

void SceneOrbit::LateUpdate(uint16_t deltaTime)
{
    // Custom late update logic for SceneOrbit
    BaseScene::LateUpdate(deltaTime); // Call the base class's Draw method
    local_objects.LateUpdate(deltaTime);
}

void SceneOrbit::Draw(Display &display)
{
    // Custom drawing code for SceneOrbit
    BaseScene::Draw(display); // Call the base class's Draw method
    local_objects.Draw(buffer);
    display.Draw(0, 0, buffer.getBuffer(), buffer.width(), buffer.height());
}