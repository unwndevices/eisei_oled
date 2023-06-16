#include "Animation.hpp"

Animation::Animation(FacingDirection direction) : frames(0), currentFrameIndex(0), currentFrameTime(0), direction(direction), releaseFirstFrame(true) {}

void Animation::AddFrame(const uint8_t *texture, int textureID, int width, int height, uint16_t frameTime, bool isBool)
{
    FrameData data;
    data.texture = texture;
    data.id = textureID;
    data.width = width;
    data.height = height;
    data.displayTimeMs = frameTime;
    data.isBool = isBool;
    frames.push_back(data);
}

void Animation::AddFrameAction(unsigned int frame,
                               AnimationAction action)
{
    if (frame < frames.size())
    {
        auto actionKey = actions.find(frame);

        if (actionKey == actions.end())
        {
            // We set the bit at the frame position whenever
            // we first add an action at that position.
            framesWithActions.SetBit(frame);
            actions.insert(std::make_pair(frame,
                                          std::vector<AnimationAction>{action}));
        }
        else
        {
            actionKey->second.emplace_back(action);
        }
    }
}

const FrameData *Animation::GetCurrentFrame() const
{
    if (frames.size() > 0)
    {
        return &frames[currentFrameIndex];
    }

    return nullptr;
}

bool Animation::UpdateFrame(uint16_t deltaTime)
{
    if (releaseFirstFrame)
    {
        // To be called when we are releasing the
        // first frame of an animation.
        RunActionForCurrentFrame();
        releaseFirstFrame = false;
        return true;
    }

    if (frames.size() > 1)
    {
        currentFrameTime += deltaTime;

        if (currentFrameTime >= frames[currentFrameIndex].displayTimeMs)
        {
            currentFrameTime = 0;
            IncrementFrame();
            RunActionForCurrentFrame();
            return true;
        }
    }

    return false;
}

void Animation::IncrementFrame()
{
    // check if we reached the last frame
    if (currentFrameIndex == (frames.size() - 1))
    {
        currentFrameIndex = 0;
    }
    else
    {
        currentFrameIndex++;
    }
}

void Animation::Reset()
{
    currentFrameIndex = 0;
    currentFrameTime = 0;
    releaseFirstFrame = true;
}

void Animation::SetDirection(FacingDirection dir)
{
    if (direction != dir)
    {
        direction = dir;
        for (auto &f : frames)
        {
            f.width *= -1;
        }
    }
}

FacingDirection Animation::GetDirection() const
{
    return direction;
}

void Animation::RunActionForCurrentFrame()
{
    if (actions.size() > 0)
    {
        if (framesWithActions.GetBit(currentFrameIndex))
        {
            auto actionsToRun = actions.at(currentFrameIndex);

            for (auto f : actionsToRun)
            {
                f();
            }
        }
    }
}