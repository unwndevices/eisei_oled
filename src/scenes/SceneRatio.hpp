#ifndef RATIO_H
#define RATIO_H

#include "BaseScene.hpp"

#include "assets/ratio.h"
#include "enjin/UI/OverlayBg.hpp"
#include "enjin/UI/ConstantList.hpp"
#include "enjin/UI/RatioDial.hpp"
#include "enjin/UI/FillUpGauge.hpp"

class SceneRatio : public BaseScene
{
public:
    SceneRatio(SceneStateMachine &sceneStateMachine, SharedContext &context) : BaseScene(sceneStateMachine, context){};

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
    ObjectCollection local_objects;

    // std::shared_ptr<OverlayBg> overlay;
    // std::shared_ptr<ConstantList> const_list;
    // std::shared_ptr<RatioDial> const_dial;
    // std::shared_ptr<Tooltip> const_tooltip;
    std::shared_ptr<FillUpGauge> overlay;

    void ProcessButton(int id, Button::State state);
    void ProcessIncrement(TouchWheel::Direction direction);
};

#endif // !RATIO_H