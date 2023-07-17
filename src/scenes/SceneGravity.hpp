#ifndef SCENEGRAVITY_HPP
#define SCENEGRAVITY_HPP
#include "BaseScene.hpp"

#include "enjin/UI/Tooltip.hpp"

class SceneGravity : public BaseScene
{
public:
    SceneGravity(SceneStateMachine &sceneStateMachine, SharedContext &context) : BaseScene(sceneStateMachine, context){};

    void OnCreate() override;
    void OnDestroy() override;

    void OnActivate() override;
    void OnDeactivate() override;

    void ProcessInput() override;
    void LateUpdate(uint16_t deltaTime) override;

    void Update(uint16_t deltaTime) override;
    void Draw(Display &display) override;

private:
    std::shared_ptr<Tooltip> tooltip;
};

#endif // SceneGravity_HPP
