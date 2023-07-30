#include "SceneGravity.hpp"

void SceneGravity::OnCreate()
{
    bg = std::make_shared<OverlayBg>(3);
    bg->SetDrawLayer(DrawLayer::Overlay);

    local_objects.Add(bg);

    planet = std::make_shared<Planet>();
    local_objects.Add(planet);

    frequency_label = std::make_shared<Label>(45, 19, 14, 1, 11);

    // setting custom transition for label
    frequency_label->transition->AddKeyframe({0, Vector2(64, -20), Easing::Step});
    frequency_label->transition->AddKeyframe({100, Vector2(64, -20), Easing::Step});
    frequency_label->transition->AddKeyframe({450, Vector2(64, 64 - 37), Easing::EaseOutQuad});

    local_objects.Add(frequency_label);
}

void SceneGravity::OnDestroy()
{
    // Custom cleanup for SceneGravity
}

void SceneGravity::OnActivate()
{
    bg->fill_transition->ClearKeyframes();
    bg->fill_transition->SetParameterSetter(std::bind(&OverlayBg::SetOpacity, bg.get(), std::placeholders::_1));
    bg->fill_transition->AddKeyframe({0, bg->fill_transition->GetCurrentValue(), Easing::Step});
    bg->fill_transition->AddKeyframe({250, 5U, Easing::Linear});

    bg->graphic_transition->ClearKeyframes();
    bg->graphic_transition->SetParameterSetter(std::bind(&OverlayBg::SetFrame, bg.get(), std::placeholders::_1));
    bg->graphic_transition->AddKeyframe({0, bg->graphic_transition->GetCurrentValue(), Easing::Step});
    bg->graphic_transition->AddKeyframe({250, 3U, Easing::Linear});

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
    instances.main_planet->radius_transition->SetEndCallback(std::bind(&Planet::SetActive, planet.get()));
    instances.main_planet->radius_transition->AddKeyframe({0, instances.main_planet->radius_transition->GetCurrentValue(), Easing::Step});
    instances.main_planet->radius_transition->AddKeyframe({250, 29, Easing::EaseInCubic});

    bg->EnterTransition();
    instances.main_planet->EnterTransition();
    frequency_label->EnterTransition();

    interface.hw.GetTouchwheel().onClick.Connect(std::bind(&SceneGravity::ProcessTouchClick, this, std::placeholders::_1));
}

void SceneGravity::OnDeactivate()
{
    planet->SetVisibility(false);
    frequency_label->SetVisibility(false);
    bg->SetVisibility(false);
    interface.hw.GetTouchwheel().onClick.DisconnectAll();
}

void SceneGravity::ProcessInput()
{
    if (interface.hw.GetTouchwheel().IsTouched(0.1f))
    {
        float touchwheel_input = interface.hw.GetTouchwheel().GetSpeed();

        data.interface_data.gravity += touchwheel_input * 100.0f;
        planet->SetFrequency(data.interface_data.gravity);
        frequency_label->SetValue(data.interface_data.gravity);
    }
}

void SceneGravity::ProcessTouchClick(TouchWheel::Halves side)
{
    log_d("Touchwheel clicked");
    if (side == TouchWheel::Halves::TOP)
    {
        data.interface_data.gravity += 0.1f;
        planet->SetFrequency(data.interface_data.gravity);
        frequency_label->SetValue(data.interface_data.gravity);
    }
    else
    {
        data.interface_data.gravity -= 0.1f;
        planet->SetFrequency(data.interface_data.gravity);
        frequency_label->SetValue(data.interface_data.gravity);
    }
}

void SceneGravity::Update(uint16_t deltaTime)
{
    // Custom update logic for SceneGravity
    BaseScene::Update(deltaTime); // Call the base class's Update method

    local_objects.ProcessNewObjects();
    local_objects.Update(deltaTime);
}

void SceneGravity::LateUpdate(uint16_t deltaTime)
{
    // Custom late update logic for SceneGravity
    BaseScene::LateUpdate(deltaTime); // Call the base class's Draw method
    local_objects.LateUpdate(deltaTime);
}

void SceneGravity::Draw(Display &display)
{
    // Custom drawing code for SceneGravity
    BaseScene::Draw(display); // Call the base class's Draw method
    local_objects.Draw(buffer);
    display.Draw(0, 0, buffer.getBuffer(), buffer.width(), buffer.height());
}