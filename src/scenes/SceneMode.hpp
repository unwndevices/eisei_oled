#ifndef SCENEMODE_HPP
#define SCENEMODE_HPP

#include "BaseScene.hpp"

#include "enjin/UI/OverlayBg.hpp"
#include "enjin/UI/ItemList.hpp"

class SceneMode : public BaseScene
{
public:
    SceneMode(SceneStateMachine &sceneStateMachine, SharedContext &context) : BaseScene(sceneStateMachine, context){};

    void OnCreate() override;

    void OnDestroy() override{};

    void OnActivate() override;

    void OnDeactivate() override;
    void ProcessInput() override;
    void LateUpdate(uint16_t deltaTime) override;

    void SetSwitchToScene(uint8_t id);

    void Update(uint16_t deltaTime) override;
    void Draw(Display &display) override;

private:
    std::shared_ptr<ItemList> list;

    ObjectCollection local_objects;

    void ProcessIncrement(TouchWheel::Direction direction);
};

#endif // SCENEMODE_HPP
