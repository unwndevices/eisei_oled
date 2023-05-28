#ifndef RATIO_H
#define RATIO_H
#include <Arduino.h>
#include "utils/utils.h"

#include <Scene.hpp>
#include <SceneStateMachine.hpp>
#include <ObjectCollection.hpp>
#include <Components.hpp>

#include "assets/ratio.h"

class SceneRatio : public Scene
{
public:
    SceneRatio(SceneStateMachine &sceneStateMachine) : sceneStateMachine(sceneStateMachine){};

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

    //
    void InitBackground();
};

#endif // !RATIO_H