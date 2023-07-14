#ifndef BASESCENE_HPP
#define BASESCENE_HPP

#include <Fonts/VGATypewriter.h>

#include "utils/utils.h"
#include "enjin/Scene.hpp"
#include "enjin/SceneStateMachine.hpp"
#include "enjin/ObjectCollection.hpp"
#include "enjin/Components.hpp"

#include "enjin/UI/Satellite.hpp"
#include "enjin/UI/Tooltip.hpp"
#include "enjin/UI/GravityFields.hpp"
#include "enjin/UI/TransmissionBeam.hpp"
#include "enjin/UI/FillUpGauge.hpp"

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
        instances.transmission_beam->SetWidth(data.interface_data.transmitter_width);
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
        instances.main_planet->SetRadius(12);
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
    }

    void LateUpdate(uint16_t deltaTime) override
    {
        objects.LateUpdate(deltaTime);
    };

    void Draw(Display &display) override
    {
        objects.Draw(buffer);
        display.Draw(0, 0, buffer.getBuffer(), buffer.width(), buffer.height());
    }

    // Other common methods...

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
        for (auto &satellite : instances.satellites)
        {
            satellite->drawBackground(bg);
        }

        sprite->Load(bg.getBuffer(), 128, 128);
        objects.Add(background);
    };
};

#endif // BaseScene_HPP
