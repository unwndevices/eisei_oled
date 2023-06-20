#ifndef SCENEMAIN_H
#define SCENEMAIN_H
#include "utils/utils.h"
#include "enjin/Scene.hpp"
#include "enjin/SceneStateMachine.hpp"
#include "enjin/ObjectCollection.hpp"
#include "enjin/Components.hpp"

#include "enjin/UI/Satellite.hpp"
#include "enjin/UI/Slider.hpp"
#include "enjin/UI/Tooltip.hpp"
#include "enjin/UI/GravityFields.hpp"
#include "enjin/UI/TransmissionBeam.hpp"

#include "SharedData.hpp"

#include "HardwareManager.hpp"

class SceneMain : public Scene
{
public:
    SceneMain(SceneStateMachine &sceneStateMachine) : sceneStateMachine(sceneStateMachine), stars(128, 128){};

    void OnCreate() override;

    void OnDestroy() override{};

    void OnActivate() override{};

    void OnDeactivate() override{};
    void ProcessInput() override;
    void ProcessButton(int id, Button::State state);
    void LateUpdate(uint16_t deltaTime) override;

    void SetSwitchToScene(uint8_t id);

    void Update(uint16_t deltaTime) override;
    void Draw(Display &display) override;

private:
    SceneStateMachine &sceneStateMachine;
    ObjectCollection objects;
    uint8_t switchToState = 0;
    int current_page = 0;
    void InitBackground();
    void generateStars(uint16_t amount);
    GFXcanvas8 stars;

    std::shared_ptr<Satellite> main_planet;
    std::vector<std::shared_ptr<Satellite>> satellites;
    
    std::shared_ptr<Tooltip> tooltip_freq;
    std::shared_ptr<Slider> slider;
    std::shared_ptr<GravityFields> gravity_fields;
    std::shared_ptr<TransmissionBeam> transmission_beam;

    unsigned long touch_timer = 0;
    unsigned long touch_timeout = 5000;
    bool touch_active = false;
};

#endif // !SCENEMAIN_H