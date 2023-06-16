#include "C_Animation.hpp"
#include "../Object.hpp"

C_Animation::C_Animation(Object *owner) : Component(owner),
                                          currentAnimation(AnimationState::None, nullptr)
{
}

void C_Animation::Awake()
{
    sprite = owner->GetComponent<C_Sprite>();
}

void C_Animation::SetAnimationState(AnimationState state)
{
    // We only set an animation of it is not already
    // our current animation.
    if (currentAnimation.first == state)
    {
        return;
    }

    auto animation = animations.find(state);
    if (animation != animations.end())
    {
        currentAnimation.first = animation->first;
        currentAnimation.second = animation->second;

        currentAnimation.second->Reset();
    }
}

const AnimationState &C_Animation::GetAnimationState() const
{
    return currentAnimation.first;
}

void C_Animation::Update(uint8_t deltaTime)
{
    if (currentAnimation.first != AnimationState::None)
    {
        bool newFrame = currentAnimation.second->UpdateFrame(deltaTime);

        if (newFrame)
        {
            const FrameData *data = currentAnimation.second->GetCurrentFrame();
            if (data->isBool)
            {
                sprite->Load((const bool *)data->texture, data->width, data->height);
            }
            else
            {
                sprite->Load(data->texture, data->width, data->height);
            }
            sprite->LoadFrame(data->id);
        }
    }
}

void C_Animation::AddAnimation(
    AnimationState state, std::shared_ptr<Animation> animation)
{
    auto inserted = animations.insert(std::make_pair(state, animation));

    if (currentAnimation.first == AnimationState::None)
    {
        SetAnimationState(state);
    }
}