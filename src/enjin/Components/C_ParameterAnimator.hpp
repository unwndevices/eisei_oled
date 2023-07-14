#ifndef C_PARAMETERANIMATOR_HPP
#define C_PARAMETERANIMATOR_HPP

#include <memory>
#include <vector>
#include <iostream>
#include <functional>
#include "enjin/Components/Component.hpp"
#include "../utils/Easing.hpp"

struct ParameterKeyframe
{
    int time;
    float value;
    EasingFunction easingFunction;
};

class C_ParameterAnimator : public Component
{
public:
    C_ParameterAnimator(Object *owner);

    void Update(uint8_t deltaTime) override;

    void AddKeyframe(ParameterKeyframe keyframe);
    void ClearKeyframes();

    void StartAnimation();

    void SetParameterGetter(std::function<float()> getter);
    void SetParameterSetter(std::function<void(float)> setter);

private:
    std::vector<ParameterKeyframe> keyframes;
    int currentKeyframeIndex;
    ulong elapsedTime;
    bool isActive;

    std::function<float()> parameterGetter;
    std::function<void(float)> parameterSetter;

    float InterpolateParameter(ParameterKeyframe kf1, ParameterKeyframe kf2, float t);
};

#endif // C_PARAMETERANIMATOR_HPP
