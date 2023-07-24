#ifndef SCENECVGRAVITY_HPP
#define SCENECVGRAVITY_HPP

#include "utils/utils.h"
#include "enjin/Scene.hpp"
#include "enjin/SceneStateMachine.hpp"
#include "enjin/ObjectCollection.hpp"
#include "enjin/Components.hpp"

#include "enjin/UI/Planet.hpp"
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
        bg->SetDrawLayer(DrawLayer::Background);
        objects.Add(bg);
        planet = std::make_shared<Planet>(31);
        objects.Add(planet);

        interface.hw.GetButton(Pages::Gravity).onStateChanged.Connect(std::bind(&SceneCvGravity::ProcessButton, this, std::placeholders::_1, std::placeholders::_2));
        interface.hw.GetTouchwheel().onClick.Connect(std::bind(&SceneCvGravity::ProcessTouch, this, std::placeholders::_1));
        interface.hw.GetTouchwheel().onButtonClick.Connect(std::bind(&SceneCvGravity::ProcessTouchButton, this, std::placeholders::_1));
    };
    void OnDestroy() override{};

    void OnActivate() override
    {
        touchTimer.Restart();
        sceneTimer.Restart();
        planet->SetVisibility(true);
    };
    void OnDeactivate() override{};

    float value = 220.0f;
    float value2 = 0.0f;

    void ProcessInput() override
    {
        if (interface.hw.GetTouchwheel().IsTouched())
        {
            float touchwheel_input = interface.hw.GetTouchwheel().GetSpeed() * 10.0f;;
            if (touchwheel_input != 0.0f)
            {
                if (interface.hw.GetTouchwheel().GetSideVertical() == TouchWheel::Halves::TOP)
                {
                    if (touchwheel_input >= 0.01f || touchwheel_input <= -0.01f)
                    {
                        value += touchwheel_input;
                        planet->SetFrequency(value);
                    }
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
    void ProcessTouch(TouchWheel::Halves pos)
    {

        if (pos == TouchWheel::Halves::BOTTOM)
        {
            planet->GenerateTerrain();
        }
    };
    
    void ProcessTouchButton(int buttonID)
    {
        log_d("%d", buttonID);
    };

    std::shared_ptr<Planet> planet;
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
