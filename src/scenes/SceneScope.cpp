#include "SceneScope.hpp"
#include "enjin/UI/assets/icons.h"

void SceneScope::OnCreate()
{
    buffer.setFont(&VGATypewriter8pt7b);
    buffer.setTextColor(15U);

    width_icon = std::make_shared<DragIcon>(icons[Icons::scope_width_icon], 50);
    local_objects.Add(width_icon);
    phase_icon = std::make_shared<DragIcon>(icons[Icons::scope_phase_icon], 50, true);
    local_objects.Add(phase_icon);
}

void SceneScope::OnDestroy()
{
    // Custom cleanup for SceneScope
}

void SceneScope::OnActivate()
{
    context.touch_timer.Restart();
    instances.transmission_beam->SetMode(2);
    width_icon->EnterTransition(true);
    phase_icon->EnterTransition(true);
}

void SceneScope::OnDeactivate()
{
    instances.transmission_beam->SetMode(0);
    width_icon->SetVisibility(false);
    phase_icon->SetVisibility(false);
}

void SceneScope::ProcessInput()
{

    if (interface.hw.GetTouchwheel().IsTouched())
    {
        if (interface.hw.GetTouchwheel().GetSideHorizontal() == TouchWheel::Halves::LEFT)
        {
            width_icon->SetActive(true);
            phase_icon->SetActive(false);
        }
        else if (interface.hw.GetTouchwheel().GetSideHorizontal() == TouchWheel::Halves::RIGHT)
        {
            phase_icon->SetActive(true);
            width_icon->SetActive(false);
        }
    }
    else
    {
        phase_icon->SetActive(false);
        width_icon->SetActive(false);
    }

    if (interface.hw.GetTouchwheel().IsTouched(0.1f))
    {
        context.touch_timer.Restart();
        float touchwheel_input = interface.hw.GetTouchwheel().GetSpeed();
        if (interface.hw.GetTouchwheel().GetSideHorizontal() == TouchWheel::Halves::LEFT)
        {
            data.interface_data.scope_width += touchwheel_input * 0.7f;
            data.interface_data.scope_width = fmin(data.interface_data.scope_width, 1.0f);
            instances.transmission_beam->SetWidth(data.interface_data.scope_width);
        }
        else if (interface.hw.GetTouchwheel().GetSideHorizontal() == TouchWheel::Halves::RIGHT)
        {
            data.interface_data.scope_phase += touchwheel_input * 0.7f;
            data.interface_data.scope_phase = fmod(data.interface_data.scope_phase, 1.0f);
            instances.transmission_beam->SetPhase(data.interface_data.scope_phase);
        }
    }
}

void SceneScope::Update(uint16_t deltaTime)
{
    BaseScene::Update(deltaTime); // Call the base class's Update method
    local_objects.ProcessNewObjects();
    local_objects.Update(deltaTime);
}

void SceneScope::LateUpdate(uint16_t deltaTime)
{
    BaseScene::LateUpdate(deltaTime); // Call the base class's Draw method
    local_objects.LateUpdate(deltaTime);
}

void SceneScope::Draw(Display &display)
{
    BaseScene::Draw(display); // Call the base class's Draw method
    local_objects.Draw(buffer);
    display.Draw(0, 0, buffer.getBuffer(), buffer.width(), buffer.height());
}