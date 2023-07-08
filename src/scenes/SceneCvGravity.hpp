#ifndef SCENECVGRAVITY_HPP
#define SCENECVGRAVITY_HPP

#include "utils/utils.h"
#include "enjin/Scene.hpp"
#include "enjin/SceneStateMachine.hpp"
#include "enjin/ObjectCollection.hpp"
#include "enjin/Components.hpp"

#include "enjin/UI/BeamShaper.hpp"
#include "enjin/UI/Slider.hpp"
#include "enjin/UI/OverlayBg.hpp"
#include "SharedContext.hpp"
#include <Fonts/VGATypewriter.h>

class SceneCvGravity : public Scene
{
public:
    SceneCvGravity(SceneStateMachine &sceneStateMachine, SharedContext &context) : context(context), sceneStateMachine(sceneStateMachine), interface(context.interface), data(context.data){};

    void OnCreate() override
    {
        buffer.setFont(&VGATypewriter8pt7b);
        buffer.setTextColor(15U);
        bg = std::make_shared<OverlayBg>(4);
        objects.Add(bg);
        slider = std::make_shared<Slider>(String("hold"));
        objects.Add(slider);
        shaper = std::make_shared<BeamShaper>();
        objects.Add(shaper);
        //interface.hw.GetButton(Pages::Gravity).onStateChanged.Connect(std::bind(&SceneCvGravity::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
        // interface.hw.GetTouchwheel().onClick.Connect(std::bind(&SceneCvGravity::ProcessTouch, this, std::placeholders::_1));
        //interface.hw.GetTouchwheel().onButtonClick.Connect(std::bind(&SceneCvGravity::ProcessTouchButton, this, std::placeholders::_1));
    };
    void OnDestroy() override{};

    void OnActivate() override
    {
        touchTimer.Restart();
        sceneTimer.Restart();
        shaper->SetVisibility(true);
        shaper->EnterTransition();
        slider->EnterTransition();
    };
    void OnDeactivate() override{};

    float value = 0.0f;
    float value2 = 0.0f;

    void ProcessInput() override
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

    void LateUpdate(uint16_t deltaTime) override
    {
        objects.LateUpdate(deltaTime);
    };

    void Update(uint16_t deltaTime) override
    {
        if (touchTimer.IsElapsed(5000))
        {
            current_page = 0;
        }
        if (sceneTimer.IsElapsed(3000))
        {
        }
        objects.ProcessNewObjects();
        objects.Update(deltaTime);
        fps = 1.0f / (deltaTime / 1000.0f); // Divide by 1000 to convert from milliseconds to seconds
    };
    void Draw(Display &display) override
    {
        objects.Draw(buffer);
        display.Draw(0, 0, buffer.getBuffer(), buffer.width(), buffer.height());
    };

private:
    SceneStateMachine &sceneStateMachine;
    SharedContext &context;
    InterfaceManager &interface;
    Data &data;

    ObjectCollection objects;
    uint8_t switchToState = 0;
    int current_page = 0;

    void InitBackground();
    void ProcessButton(int id, Button::State state);
    void ProcessTouch(TouchWheel::Halves pos){
        // if (pos == TouchWheel::Halves::TOP)
        // {
        //     value += 0.01;
        // }
        // else if (pos == TouchWheel::Halves::BOTTOM)
        // {
        //     value -= 0.01;
        // }
    };
    void ProcessTouchButton(int buttonID)
    {
        log_d("%d", buttonID);
    };

    std::shared_ptr<BeamShaper> shaper;
    std::shared_ptr<Slider> slider;
    std::shared_ptr<OverlayBg> bg;
};

void SceneCvGravity::ProcessButton(int id, Button::State state)
{
    if (state == Button::State::CLICKED)
    {
        current_page = id;
        touchTimer.Restart();
    }
    if (id == SW_GRAVITY)
    {
        if (state == Button::State::PRESSED)
        {
            log_d("test");
        }
    }
}
#endif// SCENECVGRAVITY_HPP
