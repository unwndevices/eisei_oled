#include "SceneOrbit.hpp"

void SceneOrbit::OnCreate()
{
    label = std::make_shared<Label>(47, 19, 0);
    label->SetValue(0.1f, "s");
    label->SetVisibility(false);
    local_objects.Add(label);
}

void SceneOrbit::OnDestroy()
{
    // Custom cleanup for SceneOrbit
}

void SceneOrbit::OnActivate()
{
    int i = 0;
    for (auto &satellite : instances.satellites)
    {
        satellite->pos_transition->ClearKeyframes();
        satellite->pos_transition->SetParameterSetter(std::bind(&Satellite::SetDistance, satellite.get(), std::placeholders::_1));
        satellite->pos_transition->AddKeyframe({0, satellite->pos_transition->GetCurrentValue(), Easing::Step});
        satellite->pos_transition->AddKeyframe({350, (uint8_t)(35 + i * 8), Easing::EaseInQuart});

        satellite->radius_transition->ClearKeyframes();
        satellite->radius_transition->SetParameterSetter(std::bind(&Satellite::SetRadius, satellite.get(), std::placeholders::_1));
        satellite->radius_transition->AddKeyframe({0, satellite->radius_transition->GetCurrentValue(), Easing::Step});
        satellite->radius_transition->AddKeyframe({350, 2, Easing::EaseInQuart});

        satellite->EnterTransition();
        i++;
    }

    instances.main_planet->radius_transition->ClearKeyframes();
    instances.main_planet->radius_transition->SetParameterSetter(std::bind(&Satellite::SetRadius, instances.main_planet.get(), std::placeholders::_1));
    instances.main_planet->radius_transition->SetEndCallback([=]()
                                                             { label->SetVisibility(true); });
    instances.main_planet->radius_transition->AddKeyframe({0, instances.main_planet->radius_transition->GetCurrentValue(), Easing::Step});
    instances.main_planet->radius_transition->AddKeyframe({350, 29, Easing::EaseInQuart});

    instances.main_planet->EnterTransition();
}

void SceneOrbit::OnDeactivate()
{
}

void SceneOrbit::ProcessInput()
{
    if (interface.hw.GetTouchwheel().IsTouched())
    {
        float touchwheel_input = interface.hw.GetTouchwheel().GetSpeed();
        if (touchwheel_input != 0.0f)
        {
            
        }
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