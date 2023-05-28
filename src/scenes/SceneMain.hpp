#ifndef SCENEMAIN_H
#define SCENEMAIN_H

#include "utils/utils.h"
#include "drivers/TouchWheel.h"
#include "drivers/Button.hpp"
#include <Scene.hpp>
#include <SceneStateMachine.hpp>
#include <ObjectCollection.hpp>
#include <Components.hpp>

#include <UI/Satellite.hpp>
#include <UI/Slider.hpp>

class SceneMain : public Scene
{
public:
    SceneMain(SceneStateMachine &sceneStateMachine) : sceneStateMachine(sceneStateMachine), stars(128, 128), touch(nullptr), aButton(11){};

    void OnCreate() override;

    void OnDestroy() override{};

    void OnActivate() override{};

    void OnDeactivate() override{};
    void ProcessInput() override{};
    void LateUpdate(uint16_t deltaTime) override;

    void SetSwitchToScene(uint8_t id);

    void Update(uint16_t deltaTime) override;
    void Draw(Adafruit_SSD1327 &display) override;

private:
    SceneStateMachine &sceneStateMachine;
    ObjectCollection objects;
    uint8_t switchToState = 0;

    void InitBackground();
    void generateStars(uint16_t amount);
    GFXcanvas8 stars;

    std::shared_ptr<Satellite> background;

    std::shared_ptr<Slider> slider;

    TouchWheel *touch = nullptr;
    Button aButton;
};

#endif // !SCENEMAIN_H