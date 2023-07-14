#ifndef SCENEMASS_HPP
#define SCENEMASS_HPP
#include "BaseScene.hpp"

#include "enjin/UI/BeamShaper.hpp"
#include "enjin/UI/Slider.hpp"
#include "enjin/UI/OverlayBg.hpp"

class SceneMass : public BaseScene
{
public:
    SceneMass(SceneStateMachine &sceneStateMachine, SharedContext &context) : BaseScene(sceneStateMachine, context){};

    void OnCreate() override;
    void OnDestroy() override;

    void OnActivate() override;
    void OnDeactivate() override;

    void ProcessInput() override;
    void LateUpdate(uint16_t deltaTime) override;

    void Update(uint16_t deltaTime) override;
    void Draw(Display &display) override;

private:
    std::shared_ptr<BeamShaper> shaper;
    std::shared_ptr<Slider> slider;
    std::shared_ptr<OverlayBg> bg;
};

#endif // SCENEMASS_HPP
