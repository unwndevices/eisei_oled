
#ifndef C_DRAWABLE_HPP
#define C_DRAWABLE_HPP

#include <memory>
#include <Adafruit_GFX.h>
#include "Component.hpp"
#include "C_Position.hpp"

enum class DrawLayer
{
    Default,
    Background,
    Entities,
    Foreground,
    Overlay,
    UI
};

enum class BlendMode
{
    Normal,
    Add,
    Sub,
    Opacity50,
    Opacity25
};

enum class Anchor
{
    TOP_LEFT,
    TOP_RIGHT,
    BOTTOM_LEFT,
    BOTTOM_RIGHT,
    CENTER,
    CENTER_LEFT,
    CENTER_RIGHT,
    CENTER_TOP,
    CENTER_BOTTOM
};
class C_Drawable : public virtual Component
{
public:
    C_Drawable(uint8_t width, uint8_t height);
    virtual ~C_Drawable();

    virtual void Draw(GFXcanvas8 &canvas) = 0;
    virtual bool ContinueToDraw() const = 0;

    void SetSortOrder(int order);
    int GetSortOrder() const;
    void SetBlendMode(BlendMode mode) { blendMode = mode; }
    BlendMode GetBlendMode() const { return blendMode; }
    void SetDrawLayer(DrawLayer drawLayer);
    DrawLayer GetDrawLayer() const;
    void SetVisibility(bool visibility) { is_visible = visibility; }
    bool GetVisibility() { return is_visible; }
    void SetAnchorPoint(Anchor anchor);
    void AddOffset(Vector2 offset)
    {
        anchorOffset -= offset;
    }
    void SetOffset(Vector2 offset)
    {
        anchorOffset = offset;
    }
    Vector2 GetOffsetPosition()
    {
        Vector2 pos = position->GetPosition() - anchorOffset;
        return pos;
    }

    uint8_t GetWidth() { return width; };
    uint8_t GetHeight() { return height; };

protected:
    std::shared_ptr<C_Position> position;
    Vector2 anchorOffset;

    int sortOrder;
    DrawLayer layer;
    BlendMode blendMode;
    Anchor anchor;
    bool is_visible;
    uint8_t width, height;
};

#endif //! C_DRAWABLE_HPP
