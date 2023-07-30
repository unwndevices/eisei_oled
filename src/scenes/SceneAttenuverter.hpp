#ifndef SCENEATTENUVERTER_HPP
#define SCENEATTENUVERTER_HPP

#include "BaseScene.hpp"

#include "enjin/UI/OverlayBg.hpp"
#include "enjin/UI/FillUpGauge.hpp"

class SceneAttenuverter : public BaseScene
{
public:
    SceneAttenuverter(SceneStateMachine &sceneStateMachine, SharedContext &context) : BaseScene(sceneStateMachine, context),
                                                                                      label(String("")), value(nullptr){};

    void OnCreate() override;

    void OnDestroy() override{};

    void OnActivate() override;

    void OnDeactivate() override;
    void ProcessInput() override;
    void LateUpdate(uint16_t deltaTime) override;

    void SetSwitchToScene(uint8_t id);

    void Update(uint16_t deltaTime) override;
    void Draw(Display &display) override;

    void SetData(int id);

private:
    ObjectCollection local_objects;

    std::shared_ptr<OverlayBg> bg;
    std::shared_ptr<FillUpGauge> gauge;

    String label;
    float *value;

    void ProcessButton(int id, Button::State state);
    void ProcessIncrement(TouchWheel::Direction direction);
};

#endif // SCENEATTENUVERTER_HPP
