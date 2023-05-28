#include "SceneSplashScreen.hpp"

void SceneSplashScreen::SetSwitchToScene(uint8_t id)
{
    // Stores the id of the scene that we will transition to.
    switchToState = id;
}

void SceneSplashScreen::OnCreate()
{

    std::shared_ptr<Object> sky = std::make_shared<Object>();
    auto sprite = sky->AddComponent<C_Sprite>();
    auto position = sky->AddComponent<C_Position>();
    auto animation = sky->AddComponent<C_Animation>();
    std::shared_ptr<Animation> skyLoop = std::make_shared<Animation>(FacingDirection::Right);
    for (auto i = 0; i < 11; i++)
    {
        skyLoop->AddFrame((const uint8_t *)sky_loop, i, 128, 128, 240, true);
    }
    animation->AddAnimation(AnimationState::Idle, skyLoop);
    position->SetPosition(0, -30);
    auto keyframes = sky->AddComponent<C_KeyframeAnimator>();
    keyframes->AddKeyframe({2500, (Vector2){0, -30}, AnimationState::Idle, EasingFunction(Easing::Step)});
    keyframes->AddKeyframe({4500, (Vector2){0, -30 + 127}, AnimationState::Idle, EasingFunction(Easing::EaseOutQuad)});

    std::shared_ptr<Object> sky2 = std::make_shared<Object>();
    sprite = sky2->AddComponent<C_Sprite>();
    position = sky2->AddComponent<C_Position>();
    animation = sky2->AddComponent<C_Animation>();
    animation->AddAnimation(AnimationState::Idle, skyLoop);
    position->SetPosition(0, -157);

    keyframes = sky2->AddComponent<C_KeyframeAnimator>();
    keyframes->AddKeyframe({2500, (Vector2){0, -157}, AnimationState::Idle, EasingFunction(Easing::Step)});
    keyframes->AddKeyframe({4500, (Vector2){0, -157 + 127}, AnimationState::Idle, EasingFunction(Easing::EaseOutQuad)});

    objects.Add(sky);
    objects.Add(sky2);

    std::shared_ptr<Object> roof = std::make_shared<Object>();
    sprite = roof->AddComponent<C_Sprite>();
    animation = roof->AddComponent<C_Animation>();
    std::shared_ptr<Animation> roof_anim = std::make_shared<Animation>(FacingDirection::Right);
    roof_anim->AddFrame((const uint8_t *)roof_sprite, 0, 128, 50, 120);
    animation->AddAnimation(AnimationState::Idle, roof_anim);
    position = roof->AddComponent<C_Position>();
    position->SetPosition(0, 77);

    keyframes = roof->AddComponent<C_KeyframeAnimator>();
    keyframes->AddKeyframe({2500, (Vector2){0, 77}, AnimationState::Idle, EasingFunction(Easing::Step)});
    keyframes->AddKeyframe({3700, (Vector2){0, 77 + 90}, AnimationState::Idle, EasingFunction(Easing::EaseOutQuad)});

    objects.Add(roof);
    InitCat();
};

void SceneSplashScreen::Update(uint16_t deltaTime)
{
    // Process our new objects at the beginning of each frame.
    objects.ProcessNewObjects();
    objects.Update(deltaTime);
}
void SceneSplashScreen::LateUpdate(uint16_t deltaTime)
{
    objects.LateUpdate(deltaTime);
}

void SceneSplashScreen::Draw(Adafruit_SSD1327 &display)
{
    objects.Draw(buffer);
    display.drawGrayscaleBitmap(0, 0, buffer.getBuffer(), buffer.width(), buffer.height());
}

void SceneSplashScreen::InitCat()
{
    std::shared_ptr<Object> cat = std::make_shared<Object>();
    // Adds a component by calling our previously written template function.
    auto sprite = cat->AddComponent<C_Sprite>();
    auto position = cat->AddComponent<C_Position>();
    auto animation = cat->AddComponent<C_Animation>();
    position->SetPosition(-100, 46);

    std::shared_ptr<Animation> walkAnimation = std::make_shared<Animation>(FacingDirection::Right);
    walkAnimation->AddFrame((const uint8_t *)cat_walk, 0, 78, 38, 120);
    walkAnimation->AddFrame((const uint8_t *)cat_walk, 1, 78, 38, 120);
    walkAnimation->AddFrame((const uint8_t *)cat_walk, 2, 78, 38, 120);
    walkAnimation->AddFrame((const uint8_t *)cat_walk, 3, 78, 38, 120);
    animation->AddAnimation(AnimationState::Walk, walkAnimation);
    animation->SetAnimationState(AnimationState::Walk);

    std::shared_ptr<Animation> sittingAnimation = std::make_shared<Animation>(FacingDirection::Right);
    for (auto i = 0; i < 11; i++)
    {
        sittingAnimation->AddFrame((const uint8_t *)cat_sitting, i, 45, 42, 120);
    }
    animation->AddAnimation(AnimationState::Sitting, sittingAnimation);

    std::shared_ptr<Animation> sittingTransition = std::make_shared<Animation>(FacingDirection::Right);
    sittingTransition->AddFrame((const uint8_t *)cat_sits, 0, 73, 48, 120);
    sittingTransition->AddFrame((const uint8_t *)cat_sits, 1, 73, 48, 120);
    sittingTransition->AddFrame((const uint8_t *)cat_sits, 2, 73, 48, 120);
    sittingTransition->AddFrame((const uint8_t *)cat_sits, 3, 73, 48, 120);
    animation->AddAnimation(AnimationState::SittingTransition, sittingTransition);

    auto keyframes = cat->AddComponent<C_KeyframeAnimator>();

    keyframes->AddKeyframe({0, (Vector2){-78, 50}, AnimationState::Walk, EasingFunction(Easing::Step)});
    keyframes->AddKeyframe({1440, (Vector2){25, 50}, AnimationState::Walk, EasingFunction(Easing::Linear)});
    keyframes->AddKeyframe({1450, (Vector2){25, 40}, AnimationState::SittingTransition, EasingFunction(Easing::Step)});
    keyframes->AddKeyframe({1920, (Vector2){34, 46}, AnimationState::Sitting, EasingFunction(Easing::Step)});
    keyframes->AddKeyframe({2500, (Vector2){34, 46}, AnimationState::Sitting, EasingFunction(Easing::Step)});
    keyframes->AddKeyframe({3700, (Vector2){34, 46 + 90}, AnimationState::Sitting, EasingFunction(Easing::EaseOutQuad)});
    objects.Add(cat);
}