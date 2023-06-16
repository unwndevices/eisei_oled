#ifndef C_KeyframeAnimator_HPP
#define C_KeyframeAnimator_HPP

#include <memory>
#include <vector>
#include "Component.hpp"
#include "C_Animation.hpp"
#include "C_Position.hpp"
#include "../utils/Easing.hpp"

struct Keyframe
{
    int time;
    Vector2 position;
    AnimationState state;
    EasingFunction easingFunction;
};

class C_KeyframeAnimator : public Component
{
public:
    C_KeyframeAnimator(Object *owner);

    void Update(uint8_t deltaTime) override;

    void AddKeyframe(Keyframe keyframe);
    void ClearKeyframes();

    // Sets the number of times the animation should loop (-1 disables looping)
    void SetLoops(int loopCount)
    {
        if (loopCount < 0)
        {
            this->looping = false;
            loopCount = 1;
        }
        else
        {
            this->looping = true;
            this->loopCount = loopCount;
        }
    };

private:
    std::shared_ptr<C_Position> position;
    std::shared_ptr<C_Animation> animation;

    bool looping;
    int loopCount;

    std::vector<Keyframe> keyframes;
    int currentKeyframeIndex;
    ulong elapsedTime;

    Vector2 InterpolatePosition(Keyframe kf1, Keyframe kf2, float t);
};

#endif /* C_KeyframeAnimator_HPP */