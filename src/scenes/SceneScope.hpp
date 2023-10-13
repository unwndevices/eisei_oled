#ifndef SCENESCOPE_HPP
#define SCENESCOPE_HPP

#include "BaseScene.hpp"

#include "enjin/UI/OverlayBg.hpp"
#include "enjin/UI/DragIcon.hpp"

class SceneScope : public BaseScene
{
public:
    SceneScope(SceneStateMachine &sceneStateMachine, SharedContext &context) : BaseScene(sceneStateMachine, context){};

    void OnCreate() override;
    void OnDestroy() override;

    void OnActivate() override;
    void OnDeactivate() override;

    void ProcessInput() override;
    void LateUpdate(uint16_t deltaTime) override;

    void Update(uint16_t deltaTime) override;
    void Draw(Display &display) override;

private:
    std::shared_ptr<DragIcon> width_icon, phase_icon;

    ObjectCollection local_objects;
};

#endif// SCENESCOPE_HPP
