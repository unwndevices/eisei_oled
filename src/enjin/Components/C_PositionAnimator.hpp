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

class C_PositionAnimator : public Component
{
public:
    C_PositionAnimator(Object *owner);

    void Update(uint8_t deltaTime) override;

    void AddKeyframe(PositionKeyframe keyframe);
    void ClearKeyframes();

    void StartAnimation();

private:
    std::shared_ptr<C_Position> position;

    std::vector<PositionKeyframe> keyframes;
    int currentKeyframeIndex;
    ulong elapsedTime;
    bool isActive;

    Vector2 InterpolatePosition(PositionKeyframe kf1, PositionKeyframe kf2, float t);
};

#endif// C_POSITIONANIMATOR_HPP
