#ifndef C_PARAMETERANIMATOR_HPP
#define C_PARAMETERANIMATOR_HPP

#include <memory>
#include <vector>
#include <iostream>
#include <functional>
#include "enjin/Components/Component.hpp"
#include "../utils/Easing.hpp"

template <typename T>
struct ParameterKeyframe
{
    int time;
    T value;
    EasingFunction easingFunction;
};

template <typename T>
class C_ParameterAnimator : public Component
{
public:
    C_ParameterAnimator(Object *owner);

    void Update(uint8_t deltaTime) override;

    void AddKeyframe(ParameterKeyframe<T> keyframe);
    void ClearKeyframes();

    void StartAnimation();

    void SetParameterGetter(std::function<T()> getter);
    void SetParameterSetter(std::function<void(T)> setter);
    void SetEndCallback(std::function<void()> callback);

    bool hasFinished() { return !isActive; }

    T GetCurrentValue()
    {
        if (parameterGetter)
        {
            return parameterGetter();
        }
        else
        {
            return T();
        }
    }

private:
    std::vector<ParameterKeyframe<T>> keyframes;
    int currentKeyframeIndex;
    ulong elapsedTime;
    bool isActive;

    std::function<T()> parameterGetter;
    std::function<void(T)> parameterSetter;
    std::function<void()> endCallback;

    T InterpolateParameter(ParameterKeyframe<T> kf1, ParameterKeyframe<T> kf2, float t);
};

template <typename T>
C_ParameterAnimator<T>::C_ParameterAnimator(Object *owner) : Component(owner), currentKeyframeIndex(0), elapsedTime(0), isActive(false)
{
}

template <typename T>
void C_ParameterAnimator<T>::Update(uint8_t deltaTime)
{
    if (keyframes.size() == 0)
    {
        return;
    }

    if (!isActive)
    {
        return;
    }

    elapsedTime += deltaTime;

    if (currentKeyframeIndex >= keyframes.size() - 1)
    {
        isActive = false;

        if (endCallback)
        {
            endCallback();
        }

        return;
    }

    auto currentKeyframe = keyframes[currentKeyframeIndex];
    auto nextKeyframe = keyframes[currentKeyframeIndex + 1];

    float t = static_cast<float>(elapsedTime - currentKeyframe.time) / static_cast<float>(nextKeyframe.time - currentKeyframe.time);

    if (t > 1.0f)
    {
        t = 1.0f;
        currentKeyframeIndex++;
        elapsedTime = currentKeyframe.time;
    }

    T value = InterpolateParameter(currentKeyframe, nextKeyframe, t);

    if (parameterSetter)
    {
        parameterSetter(value);
    }
}

template <typename T>
void C_ParameterAnimator<T>::AddKeyframe(ParameterKeyframe<T> keyframe)
{
    keyframes.push_back(keyframe);
    std::sort(keyframes.begin(), keyframes.end(), [](const ParameterKeyframe<T> &a, const ParameterKeyframe<T> &b)
              { return a.time < b.time; });
}

template <typename T>
void C_ParameterAnimator<T>::ClearKeyframes()
{
    keyframes.clear();
    endCallback = nullptr;
}

template <typename T>
void C_ParameterAnimator<T>::StartAnimation()
{
    currentKeyframeIndex = 0;
    elapsedTime = 0;
    isActive = true;
}

template <typename T>
void C_ParameterAnimator<T>::SetParameterGetter(std::function<T()> getter)
{
    parameterGetter = getter;
}

template <typename T>
void C_ParameterAnimator<T>::SetParameterSetter(std::function<void(T)> setter)
{
    parameterSetter = setter;
}

template <typename T>
void C_ParameterAnimator<T>::SetEndCallback(std::function<void()> callback)
{
    endCallback = callback;
}

template <typename T>
T C_ParameterAnimator<T>::InterpolateParameter(ParameterKeyframe<T> kf1, ParameterKeyframe<T> kf2, float t)
{
    float p1 = (float)kf1.value;
    float p2 = (float)kf2.value;
    float easing = kf2.easingFunction(t);
    float result = p1 + easing * (p2 - p1);
    return static_cast<T>(result);
}

#endif // C_PARAMETERANIMATOR_HPP
