#ifndef RATIO_H
#define RATIO_H
#include <Arduino.h>
#include "utils/utils.h"

#include "enjin/Scene.hpp"
#include "enjin/SceneStateMachine.hpp"
#include "enjin/ObjectCollection.hpp"
#include "enjin/Components.hpp"

#include "assets/ratio.h"
#include "enjin/UI/ConstantList.hpp"
#include "enjin/UI/RatioDial.hpp"
#include "enjin/UI/Tooltip.hpp"

#include "InterfaceManager.hpp"

class SceneRatio : public Scene
{
public:
    SceneRatio(SceneStateMachine &sceneStateMachine, InterfaceManager &interface) : sceneStateMachine(sceneStateMachine), interface(interface){};

    void OnCreate() override;

    void OnDestroy() override{};

    void OnActivate() override;

    void OnDeactivate() override{};
    void ProcessInput() override;
    void LateUpdate(uint16_t deltaTime) override;

    void SetSwitchToScene(uint8_t id);

    void Update(uint16_t deltaTime) override;
    void Draw(Display &display) override;

private:
    SceneStateMachine &sceneStateMachine;
    InterfaceManager &interface;

    ObjectCollection objects;
    uint8_t switchToState = 0;

    std::shared_ptr<ConstantList> const_list;
    std::shared_ptr<RatioDial> const_dial;
    std::shared_ptr<Tooltip> const_tooltip;

    void InitBackground();
    void ProcessButton(int id, Button::State state);
    void ProcessIncrement(TouchWheel::Direction direction);
};

#endif // !RATIO_H