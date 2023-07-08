#ifndef SCENEBASE_HPP
#define SCENEBASE_HPP

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

class SceneBase : public Scene
{
public:
    SceneBase(SceneStateMachine &sceneStateMachine, SharedContext &context) : context(context), sceneStateMachine(sceneStateMachine),
                                                                              interface(context.interface), data(context.data), bg(128, 128)
    {
        // Initialize common elements here
    }

    void OnCreate() override
    {

        // buffer.setFont(&VGATypewriter8pt7b);
        buffer.setTextColor(15U);
        transmission_beam = std::make_shared<TransmissionBeam>();
        transmission_beam->SetPhase(data.phase[4]);
        transmission_beam->SetWidth(data.interface_data.transmitter_width);

        objects.Add(transmission_beam);

        for (int i = 0; i < 4; i++)
        {
            auto satellite = std::make_shared<Satellite>(20 + i * 13, 4);
            satellites.push_back(satellite);
            objects.Add(satellite);
        }

        InitBackground();

        main_planet = std::make_shared<Satellite>(0, 12, 8u);
        objects.Add(main_planet);
    }

    void OnDestroy() override{};

    void Update(uint16_t deltaTime) override
    {
        objects.ProcessNewObjects();
        objects.Update(deltaTime);
    }

    void LateUpdate(uint16_t deltaTime) override
    {
        objects.LateUpdate(deltaTime);
        int i = 0;
        for (auto &satellite : satellites)
        {
            satellite->SetPhase(data.phase[i]);
            i++;
        }
        transmission_beam->SetPhase(data.phase[4]);
    };

    void Draw(Display &display) override
    {
        objects.Draw(buffer);
        display.Draw(0, 0, buffer.getBuffer(), buffer.width(), buffer.height());
    }

    void OnActivate() override{};
    void OnDeactivate() override{};

    void ProcessInput() override{};
    // Other common methods...

protected:
    SceneStateMachine &sceneStateMachine;
    SharedContext &context;
    InterfaceManager &interface;
    Data &data;

    ObjectCollection objects;
    uint8_t switchToState = 0;
    int current_page = 0;

    GFXcanvas8 bg;

    std::shared_ptr<Satellite> main_planet;
    std::vector<std::shared_ptr<Satellite>> satellites;
    std::shared_ptr<TransmissionBeam> transmission_beam;

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
        for (auto &satellite : satellites)
        {
            satellite->drawBackground(bg);
        }

        sprite->Load(bg.getBuffer(), 128, 128);
        objects.Add(background);
    };
};

#endif// SCENEBASE_HPP
