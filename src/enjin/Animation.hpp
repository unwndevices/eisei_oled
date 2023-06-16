#ifndef Animation_hpp
#define Animation_hpp

#include <stdint.h>
#include <vector>
#include <functional>
#include <map>

#include "utils/Bitmask.hpp"

enum class FacingDirection
{
    None,
    Left,
    Right
};

struct FrameData
{
    const uint8_t *texture;
    int id;
    int width;
    int height;
    uint16_t displayTimeMs;
    bool isBool;
};

using AnimationAction = std::function<void(void)>;
class Animation
{
public:
    Animation(FacingDirection direction);

    void AddFrame(const uint8_t *texture, int textureID, int width, int height, uint16_t frameTime, bool isBool = false);
    void AddFrameAction(unsigned int frame, AnimationAction action);

    const FrameData *GetCurrentFrame() const;

    bool UpdateFrame(uint16_t deltaTime);

    void Reset();

    void SetDirection(FacingDirection dir);

    FacingDirection GetDirection() const;

private:
    void IncrementFrame();
    void RunActionForCurrentFrame();

    std::vector<FrameData> frames;
    int currentFrameIndex;
    uint16_t currentFrameTime;
    FacingDirection direction;

    bool releaseFirstFrame;
    Bitmask framesWithActions;
    std::map<int, std::vector<AnimationAction>> actions;
};

#endif /* Animation_hpp */