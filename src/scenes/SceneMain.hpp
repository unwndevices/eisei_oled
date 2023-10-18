#ifndef SCENEMAIN_H
#define SCENEMAIN_H
#include "utils/utils.h"
#include "enjin/Scene.hpp"
#include "enjin/SceneStateMachine.hpp"
#include "enjin/ObjectCollection.hpp"
#include "enjin/Components.hpp"

#include "enjin/UI/Satellite.hpp"
#include "enjin/UI/TransmissionBeam.hpp"
#include "enjin/UI/FillUpGauge.hpp"

#include "SharedContext.hpp"

class SceneMain : public Scene
{
public:
    SceneMain(SceneStateMachine &sceneStateMachine, SharedContext &context) : context(context), sceneStateMachine(sceneStateMachine), interface(context.interface), data(context.data), stars(128, 128){};

    void OnCreate() override;
    void OnDestroy() override{};

    void OnActivate() override;
    void OnDeactivate() override;

    void ProcessInput() override;
    void ProcessButton(int id, Button::State state);
    void LateUpdate(uint16_t deltaTime) override;

    void SetSwitchToScene(uint8_t id);

    void Update(uint16_t deltaTime) override;
    void Draw(Display &display) override;

private:
    SceneStateMachine &sceneStateMachine;
    SharedContext &context;
    InterfaceManager &interface;
    Data &data;

    ObjectCollection objects;
    uint8_t switchToState = 0;
    int current_page = 0;
    void InitBackground();
    void generateStars(uint16_t amount);
    GFXcanvas8 stars;

    std::shared_ptr<Satellite> main_planet;
    std::vector<std::shared_ptr<Satellite>> satellites;
    std::shared_ptr<TransmissionBeam> transmission_beam;

};

#endif // !SCENEMAIN_H