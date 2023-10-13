#ifndef RATIO_H
#define RATIO_H

#include "BaseScene.hpp"

#include "enjin/UI/OverlayBg.hpp"
#include "enjin/UI/ConstantList.hpp"
#include "enjin/UI/Label.hpp"
#include "enjin/UI/RatioDial.hpp"
#include "enjin/UI/FillUpGauge.hpp"
#include "enjin/UI/DragSlider.hpp"

class SceneRatio : public BaseScene
{
public:
    SceneRatio(SceneStateMachine &sceneStateMachine, SharedContext &context) : BaseScene(sceneStateMachine, context){};

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
    std::shared_ptr<DragSlider> drag_left, drag_right;
    std::shared_ptr<Label> label;

    ObjectCollection local_objects;

    void ProcessIncrement(TouchWheel::Direction direction);
};

#endif // !RATIO_H