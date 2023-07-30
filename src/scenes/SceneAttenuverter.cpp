#include "SceneAttenuverter.hpp"

void SceneAttenuverter::SetSwitchToScene(uint8_t id)
{
    // Stores the id of the scene that we will transition to.
    switchToState = id;
}

void SceneAttenuverter::OnCreate()
{
    bg = std::make_shared<OverlayBg>(3);
    bg->SetDrawLayer(DrawLayer::Overlay);
    local_objects.Add(bg);

    gauge = std::make_shared<FillUpGauge>();
    local_objects.Add(gauge);
}

void SceneAttenuverter::OnActivate()
{
    bg->fill_transition->ClearKeyframes();
    bg->fill_transition->SetParameterSetter(std::bind(&OverlayBg::SetOpacity, bg.get(), std::placeholders::_1));
    bg->fill_transition->AddKeyframe({0, bg->fill_transition->GetCurrentValue(), Easing::Step});
    bg->fill_transition->AddKeyframe({250, 5U, Easing::Linear});

    bg->graphic_transition->ClearKeyframes();
    bg->graphic_transition->SetParameterSetter(std::bind(&OverlayBg::SetFrame, bg.get(), std::placeholders::_1));
    bg->graphic_transition->AddKeyframe({0, bg->graphic_transition->GetCurrentValue(), Easing::Step});
    bg->graphic_transition->AddKeyframe({250, 3U, Easing::Linear});


    gauge->SetString(label);
    gauge->SetValue(*value);

    gauge->EnterTransition();
    bg->EnterTransition();
}

void SceneAttenuverter::OnDeactivate()
{
    gauge->SetVisibility(false);
    bg->SetVisibility(false);
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
    switch (id)
    {
    case SW_A:
        label = "cv a";
        value = &data.cv_data.input_a;
        break;
    case SW_B:
        label = "cv b";
        value = &data.cv_data.input_b;
        break;
    case SW_C:
        label = "cv c";
        value = &data.cv_data.input_c;
        break;
    case SW_D:
        label = "cv d";
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
        float touchwheel_input = interface.hw.GetTouchwheel().GetSpeed();
        if (touchwheel_input != 0.0f)
        {
            *value += touchwheel_input;
            *value = constrain(*value, -1.0f, 1.0f);
            gauge->SetValue(*value);
        }
    }
}