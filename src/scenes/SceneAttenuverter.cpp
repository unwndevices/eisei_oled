#include "SceneAttenuverter.hpp"
#include <Fonts/VGATypewriter.h>

void SceneAttenuverter::SetSwitchToScene(uint8_t id)
{
    // Stores the id of the scene that we will transition to.
    switchToState = id;
}

void SceneAttenuverter::OnCreate()
{
    buffer.setFont(&VGATypewriter8pt7b);
}

void SceneAttenuverter::OnActivate()
{
    context.touch_timer.Restart();

    instances.bg->fill_transition->SetAnimation(instances.bg->fill_in_animation);
    instances.bg->graphic_transition->SetAnimation(instances.bg->graphic_in_animation);

    instances.gauge->SetString(label);
    instances.gauge->SetValue(*value);

    if (current_id < Scenes::Ratio)
    {
        if (previous_id < current_id)
            instances.gauge->pos_transition->SetAnimation(instances.gauge->from_bottom);
        else if (previous_id > current_id)
            instances.gauge->pos_transition->SetAnimation(instances.gauge->from_top);
    }
    else
    {
        if (previous_id < SW_A)
        {
            instances.gauge->pos_transition->SetAnimation(instances.gauge->from_left);
        }
        else if (previous_id < current_id)
            instances.gauge->pos_transition->SetAnimation(instances.gauge->from_right);
        else if (previous_id > current_id)
            instances.gauge->pos_transition->SetAnimation(instances.gauge->from_left);
    }
    instances.gauge->EnterTransition(true);
    instances.bg->EnterTransition(false);
}

void SceneAttenuverter::OnDeactivate()
{
    instances.gauge->SetVisibility(false);
    instances.bg->SetVisibility(false);
}

void SceneAttenuverter::Update(uint16_t deltaTime)
{
    BaseScene::Update(deltaTime); // Call the base class's Update method

    // Process our new objects at the beginning of each frame.
    local_objects.ProcessNewObjects();
    local_objects.Update(deltaTime);
}
void SceneAttenuverter::LateUpdate(uint16_t deltaTime)
{
    BaseScene::LateUpdate(deltaTime); // Call the base class's Draw method

    local_objects.LateUpdate(deltaTime);
}

void SceneAttenuverter::Draw(Display &display)
{
    BaseScene::Draw(display); // Call the base class's Draw method
    local_objects.Draw(buffer);
    display.Draw(0, 0, buffer.getBuffer(), buffer.width(), buffer.height());
}

void SceneAttenuverter::ProcessButton(int id, Button::State state)
{
    switch (id)
    {
    default:
        break;
    }
}

void SceneAttenuverter::SetData(int id)
{
    previous_id = current_id;
    current_id = id;
    switch (id)
    {
    case Scenes::Gravity:
        label = "gravity";
        value = &data.cv_data.cv_gravity;
        break;
    case Scenes::Orbit:
        label = "orbit";
        value = &data.cv_data.cv_orbit;
        break;
    case Scenes::Scope:
        label = "scope";
        value = &data.cv_data.cv_scope;
        break;
    case Scenes::Ratio:
        label = "ratio";
        value = &data.cv_data.cv_ratio;
        break;
    case SW_A:
        label = "fm a";
        value = &data.cv_data.input_a;
        break;
    case SW_B:
        label = "fm b";
        value = &data.cv_data.input_b;
        break;
    case SW_C:
        label = "fm c";
        value = &data.cv_data.input_c;
        break;
    case SW_D:
        label = "fm d";
        value = &data.cv_data.input_d;
        break;
    default:
        break;
    }
}

void SceneAttenuverter::ProcessIncrement(TouchWheel::Direction direction)
{
}

void SceneAttenuverter::ProcessInput()
{
    if (interface.hw.GetTouchwheel().IsTouched())
    {
        context.touch_timer.Restart();

        float touchwheel_input = interface.hw.GetTouchwheel().GetSpeed();
        if (touchwheel_input != 0.0f)
        {
            *value += touchwheel_input;
            *value = constrain(*value, -1.0f, 1.0f);
            instances.gauge->SetValue(*value);
        }
    }
}