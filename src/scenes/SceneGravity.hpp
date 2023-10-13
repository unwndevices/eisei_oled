#ifndef SCENEGRAVITY_HPP
#define SCENEGRAVITY_HPP
#include "BaseScene.hpp"
#include <Fonts/VGATypewriter.h>

#include "enjin/UI/OverlayBg.hpp"
#include "enjin/UI/Planet.hpp"
#include "enjin/UI/Label.hpp"

class SceneGravity : public BaseScene
{
public:
    SceneGravity(SceneStateMachine &sceneStateMachine, SharedContext &context) : BaseScene(sceneStateMachine, context){};

    void OnCreate() override;
    void OnDestroy() override;

    void OnActivate() override;
    void OnDeactivate() override;

    void ProcessInput() override;
    void ProcessTouchClick(TouchWheel::Halves side);
    void LateUpdate(uint16_t deltaTime) override;

    void Update(uint16_t deltaTime) override;
    void Draw(Display &display) override;


private:
    ObjectCollection local_objects;
    std::shared_ptr<Planet> planet;
    std::shared_ptr<Label> frequency_label;
};

#endif // SceneGravity_HPP
