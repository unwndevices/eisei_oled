#include "C_ParameterAnimator.hpp"
#include "enjin/Object.hpp"

C_ParameterAnimator::C_ParameterAnimator(Object *owner) : Component(owner), currentKeyframeIndex(0), elapsedTime(0), isActive(false)
{
}

void C_ParameterAnimator::StartAnimation()
{
    elapsedTime = 0;
    currentKeyframeIndex = 0;
    isActive = true;
}

void C_ParameterAnimator::AddKeyframe(ParameterKeyframe keyframe)
{
    keyframes.push_back(keyframe);
    // Sort keyframes by time
    std::sort(keyframes.begin(), keyframes.end(), [](const ParameterKeyframe &a, const ParameterKeyframe &b)
              { return a.time < b.time; });
}

void C_ParameterAnimator::ClearKeyframes()
{
    keyframes.clear();
    currentKeyframeIndex = 0;
    elapsedTime = 0;
}

void C_ParameterAnimator::Update(uint8_t deltaTime)
{
    if (!isActive || keyframes.empty())
    {
        return;
    }

    ParameterKeyframe currentKeyframe = keyframes[currentKeyframeIndex];
    ParameterKeyframe nextKeyframe = currentKeyframe;

    elapsedTime += deltaTime;

    if (elapsedTime < currentKeyframe.time)
    {
        return;
    }

    if (currentKeyframeIndex + 1 < keyframes.size())
    {
        nextKeyframe = keyframes[currentKeyframeIndex + 1];
    }

    float t = ((float)(elapsedTime - currentKeyframe.time) / (float)(nextKeyframe.time - currentKeyframe.time));
    if (t > 0.99f)
        t = 1.0f;
    else if (t < 0.01f)
        t = 0.0f;

    if (elapsedTime >= nextKeyframe.time)
    {
        t = 1.0f;
        if (currentKeyframeIndex >= keyframes.size() - 1)
        {
            return;
        }
        else
        {
            ++currentKeyframeIndex;
        }
    }

    float interpolatedParameter = InterpolateParameter(currentKeyframe, nextKeyframe, t);
    parameterSetter(interpolatedParameter);
}

///////////////////
///// PRIVATE /////
///////////////////

float C_ParameterAnimator::InterpolateParameter(ParameterKeyframe kf1, ParameterKeyframe kf2, float t)
{
    float p1 = kf1.value;
    float p2 = kf2.value;
    float easing = kf2.easingFunction(t);
    return p1 + easing * (p2 - p1);
}

void C_ParameterAnimator::SetParameterGetter(std::function<float()> getter)
{
    parameterGetter = getter;
}

void C_ParameterAnimator::SetParameterSetter(std::function<void(float)> setter)
{
    parameterSetter = setter;
}
