#include "SceneGravity.hpp"

void SceneGravity::OnCreate()
{
    planet = std::make_shared<Planet>();
    local_objects.Add(planet);

    frequency_label = std::make_shared<Label>(45, 19, 1, 14, 1, 11);

    // setting custom transition for label
    frequency_label->position->SetPosition(Vector2(64, -20));
    frequency_label->in_transition.AddKeyframe({0, Vector2(64, -20), Easing::Step});
    frequency_label->in_transition.AddKeyframe({100, Vector2(64, -20), Easing::Step});
    frequency_label->in_transition.AddKeyframe({450, Vector2(64, 64 - 37), Easing::EaseOutQuad});
    frequency_label->transition->SetAnimation(frequency_label->in_transition);

    local_objects.Add(frequency_label);
}

void SceneGravity::OnDestroy()
{
    // Custom cleanup for SceneGravity
}

void SceneGravity::OnActivate()
{
    context.touch_timer.Restart();

    instances.bg->fill_transition->SetAnimation(instances.bg->fill_in_animation);
    instances.bg->graphic_transition->SetAnimation(instances.bg->graphic_in_animation);

    int i = 0;
    for (auto &satellite : instances.satellites)
    {
        satellite->pos_transition->SetAnimation(satellite->pos_animation_out);
        satellite->radius_transition->SetAnimation(satellite->radius_animation_out);
        satellite->EnterTransition(true);
        i++;
    }

    instances.main_planet->radius_animation_out.SetEndCallback(std::bind(&Planet::SetActive, planet.get()));
    instances.main_planet->radius_transition->SetAnimation(instances.main_planet->radius_animation_out);

    instances.bg->EnterTransition(true);
    instances.main_planet->EnterTransition(true);
    frequency_label->EnterTransition(true);

    interface.hw.GetTouchwheel().onClick.Connect(std::bind(&SceneGravity::ProcessTouchClick, this, std::placeholders::_1));
}

void SceneGravity::OnDeactivate()
{
    planet->SetVisibility(false);
    frequency_label->SetVisibility(false);
    instances.bg->SetVisibility(false);
    interface.hw.GetTouchwheel().onClick.DisconnectAll();
}

void SceneGravity::ProcessInput()
{
    if (interface.hw.GetTouchwheel().IsTouched(0.1f))
    {
        context.touch_timer.Restart();
        float touchwheel_input = interface.hw.GetTouchwheel().GetSpeed();
        data.interface_data.gravity += touchwheel_input * 100.0f;
        planet->SetFrequency(data.interface_data.gravity);
        frequency_label->SetValue(data.interface_data.gravity);
    }
}

void SceneGravity::ProcessTouchClick(TouchWheel::Halves side)
{
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