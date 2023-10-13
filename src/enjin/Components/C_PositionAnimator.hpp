#ifndef C_POSITIONANIMATOR_HPP
#define C_POSITIONANIMATOR_HPP

#include <memory>
#include <vector>
#include <iostream>
#include "enjin/Components/Component.hpp"
#include "enjin/Components/C_Position.hpp"
#include "../utils/Easing.hpp"

struct PositionKeyframe
{
    int time;
    Vector2 position;
    EasingFunction easingFunction;
};

class PositionAnimation
{
public:
    void AddKeyframe(PositionKeyframe keyframe)
    {
        keyframes.push_back(keyframe);
        std::sort(keyframes.begin(), keyframes.end(), [](const PositionKeyframe &a, const PositionKeyframe &b)
                  { return a.time < b.time; });
    }

    const std::vector<PositionKeyframe> &GetKeyframes() const
    {
        return keyframes;
    }

    void SetEndCallback(std::function<void()> callback)
    {
        endCallback = callback;
    }

    std::function<void()> GetEndCallback() const
    {
        return endCallback;
    }

    void UpdateFirstKeyframeValue(Vector2 value)
    {
        if (!keyframes.empty())
        {
            keyframes[0].position = value;
        }
    }

private:
    std::vector<PositionKeyframe> keyframes;
    std::function<void()> endCallback;
};

class C_PositionAnimator : public Component
{
public:
    C_PositionAnimator(Object *owner);

    void Update(uint8_t deltaTime) override;

    void StartAnimation(bool reset = false);
    void SetAnimation(PositionAnimation &animation);

private:
    PositionAnimation *currentAnimation = nullptr;

    std::shared_ptr<C_Position> position;

    int currentKeyframeIndex;
    ulong elapsedTime;
    bool isActive;

    Vector2 InterpolatePosition(PositionKeyframe kf1, PositionKeyframe kf2, float t);
};

#endif // C_POSITIONANIMATOR_HPP
