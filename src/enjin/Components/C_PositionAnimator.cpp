#include "C_PositionAnimator.hpp"
#include "enjin/Object.hpp"

C_PositionAnimator::C_PositionAnimator(Object *owner) : Component(owner), currentKeyframeIndex(0), elapsedTime(0), isActive(false)
{
    position = owner->GetComponent<C_Position>();

    if (!position)
    {
        std::cerr << "C_Tooltip requires C_Position component.\n";
    }
}

void C_PositionAnimator::StartAnimation()
{
    elapsedTime = 0;
    currentKeyframeIndex = 0;
    isActive = true;
}

void C_PositionAnimator::AddKeyframe(PositionKeyframe keyframe)
{
    keyframes.push_back(keyframe);
    // Sort keyframes by time
    std::sort(keyframes.begin(), keyframes.end(), [](const PositionKeyframe &a, const PositionKeyframe &b)
              { return a.time < b.time; });
}

void C_PositionAnimator::ClearKeyframes()
{
    keyframes.clear();
    currentKeyframeIndex = 0;
    elapsedTime = 0;
}

void C_PositionAnimator::Update(uint8_t deltaTime)
{
    if (!isActive || keyframes.empty())
    {
        return;
    }

    PositionKeyframe currentKeyframe = keyframes[currentKeyframeIndex];
    PositionKeyframe nextKeyframe = currentKeyframe;

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

    Vector2 interpolatedPosition = InterpolatePosition(currentKeyframe, nextKeyframe, t);
    this->position->SetPosition(interpolatedPosition);
}

///////////////////
///// PRIVATE /////
///////////////////

Vector2 C_PositionAnimator::InterpolatePosition(PositionKeyframe kf1, PositionKeyframe kf2, float t)
{
    Vector2 p1 = kf1.position;
    Vector2 p2 = kf2.position;
    float easing = kf2.easingFunction(t);
    Vector2 position;
    position.x = p1.x + (int16_t)((p2.x - p1.x) * easing);
    position.y = p1.y + (int16_t)((p2.y - p1.y) * easing);
    return position;
}