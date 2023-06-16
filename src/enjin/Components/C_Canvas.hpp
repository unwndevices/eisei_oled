#ifndef C_CANVAS_HPP
#define C_CANVAS_HPP

#include <Adafruit_GFX.h>
#include "Component.hpp"
#include "C_Drawable.hpp"

class C_Canvas : public Component, public C_Drawable
{
public:
    C_Canvas(Object *owner, uint8_t width, uint8_t height);
    void Draw(GFXcanvas8 &canvas);
    bool ContinueToDraw() const override;
    void LateUpdate(uint8_t deltaTime) override;

    GFXcanvas8 _canvas;
    uint8_t _width, _height;
    uint8_t _matte;
    Vector2 _position;

private:
};

#endif /* C_CANVAS_HPP */