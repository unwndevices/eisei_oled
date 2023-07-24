#ifndef SCENEORBIT_HPP
#define SCENEORBIT_HPP

#include "BaseScene.hpp"
#include "enjin/UI/Label.hpp"

class SceneOrbit : public BaseScene
{
public:
    SceneOrbit(SceneStateMachine &sceneStateMachine, SharedContext &context) : BaseScene(sceneStateMachine, context){};

    void OnCreate() override;
    void OnDestroy() override;

    void OnActivate() override;
    void OnDeactivate() override;

    void ProcessInput() override;
    void LateUpdate(uint16_t deltaTime) override;

    void Update(uint16_t deltaTime) override;
    void Draw(Display &display) override;

private:
    ObjectCollection local_objects;
    std::shared_ptr<Label> label;
};

#endif// SCENEORBIT_HPP
