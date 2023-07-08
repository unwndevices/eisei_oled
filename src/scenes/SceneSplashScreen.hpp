#ifndef SCENESPLASHSCREEN_H
#define SCENESPLASHSCREEN_H
#include <Arduino.h>
#include "utils/utils.h"
#include "enjin/Scene.hpp"
#include "enjin/SceneStateMachine.hpp"
#include "enjin/ObjectCollection.hpp"
#include "enjin/Components.hpp"
#include "assets/intro.h"

#include "SharedContext.hpp"

class SceneSplashScreen : public Scene
{
public:
    SceneSplashScreen(SceneStateMachine &sceneStateMachine, SharedContext &context) : context(context), sceneStateMachine(sceneStateMachine), interface(context.interface), data(context.data){};

    void OnCreate() override;

    void OnDestroy() override{};

    void OnActivate() override{};

    void OnDeactivate() override{};
    void ProcessInput() override{};
    void LateUpdate(uint16_t deltaTime) override;

    void NextScene()
    {
        sceneStateMachine.SwitchTo(1);
    };
    void SetSwitchToScene(uint8_t id);

    void Update(uint16_t deltaTime) override;
    void Draw(Display &display) override;

private:
    SceneStateMachine &sceneStateMachine;
    SharedContext &context;
    InterfaceManager &interface;
    Data &data;

    ObjectCollection objects;
    uint8_t switchToState = 0;

    void InitCat();
};

#endif // !SPLASHSCREEN_H