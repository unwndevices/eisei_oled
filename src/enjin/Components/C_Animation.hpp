#ifndef C_Animation_hpp
#define C_Animation_hpp

#include <memory>
#include <map>
#include "Component.hpp"
#include "C_Sprite.hpp"
#include "../Animation.hpp"

enum class AnimationState
{
    None,
    Idle,
    Walk,
    SittingTransition,
    Sitting
};

class C_Animation : public Component
{
public:
    C_Animation(Object *owner);

    void Awake() override;

    void Update(uint8_t deltaTime) override;

    // Add animation to object. We need its state as well
    // so that we can switch to it.
    void AddAnimation(AnimationState state,
                      std::shared_ptr<Animation> animation);

    // Set current animation states.
    void SetAnimationState(AnimationState state);

    // Returns current animation state.
    const AnimationState &GetAnimationState() const;

private:
    std::shared_ptr<C_Sprite> sprite;
    std::map<AnimationState, std::shared_ptr<Animation>> animations;

    // We store a reference to the current animation so we
    // can quickly update and draw it.
    std::pair<AnimationState, std::shared_ptr<Animation>> currentAnimation;
};

#endif /* C_Animation_hpp */