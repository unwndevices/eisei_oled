#include "SceneSplashScreen.hpp"

void SceneSplashScreen::SetSwitchToScene(uint8_t id)
{
    // Stores the id of the scene that we will transition to.
    switchToState = id;
}

void SceneSplashScreen::OnCreate()
{

    std::shared_ptr<Object> splash = std::make_shared<Object>();
    auto sprite = splash->AddComponent<C_Sprite>(128, 128);
    auto position = splash->AddComponent<C_Position>();
    auto animation = splash->AddComponent<C_Animation>();
    std::shared_ptr<Animation> intro_full = std::make_shared<Animation>(FacingDirection::Right);
    for (auto i = 0; i < 101; i++)
    {
        intro_full->AddFrame((const uint8_t *)intro, i, 128, 128, 82);
    }
    intro_full->AddFrame((const uint8_t *)intro, 101, 128, 128, 150);
    intro_full->AddFrame((const uint8_t *)intro, 101, 128, 128, 50);

    std::unordered_map<FacingDirection, std::shared_ptr<Animation>, EnumClassHash> idleAnimations;
    idleAnimations.insert(std::make_pair(FacingDirection::Right, intro_full));

    animation->AddAnimation(AnimationState::Idle, idleAnimations);

    animation->AddAnimationAction(AnimationState::Idle, FacingDirection::Right, 102, std::bind(&SceneSplashScreen::NextScene, this));
    position->SetPosition(0, 0);
    objects.Add(splash);
};

void SceneSplashScreen::Update(uint16_t deltaTime)
{
    context.touch_timer.Restart();
    // Process our new objects at the beginning of each frame.
    objects.ProcessNewObjects();
    objects.Update(deltaTime);
}
void SceneSplashScreen::LateUpdate(uint16_t deltaTime)
{
    objects.LateUpdate(deltaTime);
}

void SceneSplashScreen::Draw(Display &display)
{
    objects.Draw(buffer);
    display.Draw(0, 0, buffer.getBuffer(), buffer.width(), buffer.height());
}
