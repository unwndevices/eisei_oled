#ifndef BASESCENE_HPP
#define BASESCENE_HPP

#include <Fonts/VGATypewriter.h>

#include "utils/utils.h"
#include "enjin/Scene.hpp"
#include "enjin/SceneStateMachine.hpp"
#include "enjin/ObjectCollection.hpp"
#include "enjin/Components.hpp"

#include "enjin/UI/Satellite.hpp"
#include "enjin/UI/GravityFields.hpp"
#include "enjin/UI/TransmissionBeam.hpp"

#include "SharedContext.hpp"

class BaseScene : public Scene
{
public:
    BaseScene(SceneStateMachine &sceneStateMachine, SharedContext &context) : context(context), sceneStateMachine(sceneStateMachine),
                                                                              objects(context.objects), instances(context.instances),
                                                                              interface(context.interface), data(context.data), bg(128, 128)
    {
    }

    void OnCreate() override
    {
        // Initialize common elements here
        buffer.setFont(&VGATypewriter8pt7b);
        buffer.setTextColor(15U);
        instances.transmission_beam = std::make_shared<TransmissionBeam>();
        instances.main_planet = std::make_shared<Satellite>(0, 12, 8u);
        instances.transmission_beam->SetPhase(data.phase[4]);
        instances.transmission_beam->SetWidth(data.interface_data.scope_attack);
        objects.Add(instances.transmission_beam);
        for (int i = 0; i < 4; i++)
        {
            auto satellite = std::make_shared<Satellite>(20 + i * 13, 4);
            instances.satellites.push_back(satellite);
            objects.Add(satellite);
        }
        InitBackground();
        objects.Add(instances.main_planet);
    }

    void OnDestroy() override{};

    void OnActivate() override
    {
        int i = 0;
        for (auto &satellite : instances.satellites)
        {
            satellite->pos_transition->ClearKeyframes();
            satellite->pos_transition->SetParameterSetter(std::bind(&Satellite::SetDistance, satellite.get(), std::placeholders::_1));
            satellite->pos_transition->AddKeyframe({0, satellite->pos_transition->GetCurrentValue(), Easing::Step});
            satellite->pos_transition->AddKeyframe({350, (uint8_t)(20 + i * 13), Easing::EaseInQuart});

            satellite->radius_transition->ClearKeyframes();
            satellite->radius_transition->SetParameterSetter(std::bind(&Satellite::SetRadius, satellite.get(), std::placeholders::_1));
            satellite->radius_transition->AddKeyframe({0, satellite->radius_transition->GetCurrentValue(), Easing::Step});
            satellite->radius_transition->AddKeyframe({350, 4, Easing::EaseInQuart});

            satellite->EnterTransition();
            i++;
        }

        instances.main_planet->radius_transition->ClearKeyframes();
        instances.main_planet->radius_transition->SetParameterSetter(std::bind(&Satellite::SetRadius, instances.main_planet.get(), std::placeholders::_1));
        instances.main_planet->radius_transition->AddKeyframe({0, instances.main_planet->radius_transition->GetCurrentValue(), Easing::Step});
        instances.main_planet->radius_transition->AddKeyframe({350, 12, Easing::EaseInQuart});

        instances.main_planet->EnterTransition();
    };

    void OnDeactivate() override{};

    void ProcessInput() override{};

    void Update(uint16_t deltaTime) override
    {
        objects.ProcessNewObjects();
        objects.Update(deltaTime);
        int i = 0;
        for (auto &satellite : instances.satellites)
        {
            satellite->SetPhase(data.phase[i]);
            i++;
        }
        instances.transmission_beam->SetPhase(data.phase[4]);

        fps = 1.0f / (deltaTime / 1000.0f); // Divide by 1000 to convert from milliseconds to seconds
    }

    void LateUpdate(uint16_t deltaTime) override
    {
        objects.LateUpdate(deltaTime);
    };

    void Draw(Display &display) override
    {
        objects.Draw(buffer);
        buffer.setTextColor(10);
        buffer.setCursor(50, 103);
        String value = String(fps, 1);
        buffer.println(value);
        display.Draw(0, 0, buffer.getBuffer(), buffer.width(), buffer.height());
    }

protected:
    SceneStateMachine &sceneStateMachine;
    SharedContext &context;
    InterfaceManager &interface;
    Data &data;
    ObjectInstances &instances;

    ObjectCollection &objects;

    uint8_t switchToState = 0;
    int current_page = 0;

    GFXcanvas8 bg;

    void generateStars(uint16_t amount)
    {
        randomSeed(touchRead(9));
        for (int k = 0; k < amount; k++)
        {
            int x = rand() % 128;
            int y = rand() % 128;
            bg.drawPixel(x, y, rand() % 1 + 2);
        }
    }
    void InitBackground()
    {
        std::shared_ptr<Object> background = std::make_shared<Object>();
        auto position = background->AddComponent<C_Position>();
        position->SetPosition(0, 0);
        auto sprite = background->AddComponent<C_Sprite>(128, 128);
        sprite->SetDrawLayer(DrawLayer::Default);
        generateStars(250);
        for (int i = 0; i < 8; i++)
        {
            Vector2 temp_position = RadialToCartesian((float)i / 8.0f, (uint8_t)63, Vector2(64, 64));
            int16_t x = temp_position.x;
            int16_t y = temp_position.y;
            bg.drawLine(64, 64, x, y, 3);
        }

        sprite->Load(bg.getBuffer(), 128, 128);
        objects.Add(background);
    };
};

#endif // BaseScene_HPP
