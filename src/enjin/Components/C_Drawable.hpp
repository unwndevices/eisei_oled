
#ifndef C_DRAWABLE_HPP
#define C_DRAWABLE_HPP

#include <Adafruit_GFX.h>

enum class DrawLayer
{
    Default,
    Background,
    Entities,
    Foreground,
    UI
};

class C_Drawable
{
public:
    C_Drawable();
    virtual ~C_Drawable();

    virtual void Draw(GFXcanvas8 &canvas) = 0;
    virtual bool ContinueToDraw() const = 0;

    void SetSortOrder(int order);
    int GetSortOrder() const;

    void SetDrawLayer(DrawLayer drawLayer);
    DrawLayer GetDrawLayer() const;

private:
    int sortOrder;
    DrawLayer layer;
};

#endif //! C_DRAWABLE_HPP
