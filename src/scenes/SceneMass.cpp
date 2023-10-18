#include "SceneMass.hpp"

void SceneMass::OnCreate()
{
    buffer.setFont(&VGATypewriter8pt7b);
    buffer.setTextColor(15U);
    bg = std::make_shared<OverlayBg>(4);
    objects.Add(bg);
    slider = std::make_shared<Slider>(String("hold"));
    objects.Add(slider);
    shaper = std::make_shared<BeamShaper>();
    objects.Add(shaper);
    bg->SetVisibility(false);
    shaper->SetHold(0.0f);
    
    slider->SetVisibility(false);
    shaper->SetVisibility(false);
    // Custom initialization for SceneMass
}

void SceneMass::OnDestroy()
{
    // Custom cleanup for SceneMass
}

void SceneMass::OnActivate()
{
    bg->SetVisibility(true);
    slider->SetVisibility(true);
    shaper->SetVisibility(true);
    shaper->EnterTransition();
    slider->EnterTransition();
}

void SceneMass::OnDeactivate()
{
    bg->SetVisibility(false);
    slider->SetVisibility(false);
    shaper->SetVisibility(false);
}

float value = 0.0f;
float value2 = 0.0f;
void SceneMass::ProcessInput()
{
    if (interface.hw.GetTouchwheel().IsTouched())
    {
        float touchwheel_input = interface.hw.GetTouchwheel().GetSpeed();
        if (touchwheel_input != 0.0f)
        {
            if (interface.hw.GetTouchwheel().GetSideVertical() == TouchWheel::Halves::TOP)
            {
                if (touchwheel_input >= 0.01f || touchwheel_input <= -0.01f)
                {
                    value += touchwheel_input;
                    value = constrain(value, 0.0f, 1.0f);
                    shaper->SetShape(value);
                }
            }
            else if (interface.hw.GetTouchwheel().GetSideVertical() == TouchWheel::Halves::BOTTOM)
            {
                value2 += touchwheel_input * -1.0f;
                value2 = constrain(value2, 0.0f, 1.0f);
                shaper->SetHold(value2);
                slider->SetValue(value2);
            }
        }
    }
}

void SceneMass::Update(uint16_t deltaTime)
{
    // Custom update logic for SceneMass
    BaseScene::Update(deltaTime); // Call the base class's Update method
}

void SceneMass::LateUpdate(uint16_t deltaTime)
{
    // Custom late update logic for SceneMass
    BaseScene::LateUpdate(deltaTime); // Call the base class's Draw method
}

void SceneMass::Draw(Display &display)
{
    // Custom drawing code for SceneMass
    BaseScene::Draw(display); // Call the base class's Draw method
}